#include "test_tools.h"
#include "test_window.h"
#include <future>
#include <gtest/gtest.h>
#include <iostream>

using namespace window_tool::tests;

test_window::test_window() : 
	_hwnd(NULL),
	_hwnd_button(NULL),
	_button_clicked(false),
	_window_minimized(false),
	_window_closed(false)
{
}

test_window::~test_window()
{
	::PostThreadMessageW(::GetThreadId(_thread.native_handle()), WM_QUIT, 0, 0);
	_thread.join();
}

test_window* test_window::create(const std::wstring& title, bool topmost)
{
	return create(title, L"", topmost);
}

test_window* test_window::create(const std::wstring& title, const std::wstring& button_text, bool topmost)
{
	auto instance = new test_window();
	std::promise<bool> created_promise;
	std::future<bool> created_future = created_promise.get_future();

	instance->_thread = std::thread([&created_promise, instance, title, button_text, topmost]()
		{
			bool result = test_window::create_internal(instance, title, button_text, topmost);
			created_promise.set_value(result);
			if (result) test_window::message_loop(instance);
		});

	bool createdSuccessfully = created_future.get();
	if (!createdSuccessfully)
	{
		return nullptr;
	}

	return instance;
}

bool test_window::create_internal(
	test_window* instance,
	const std::wstring& title,
	const std::wstring& button_text,
	bool topmost
)
{
	const LPCWSTR wnd_class_name = L"TestWindowClass";
	HMODULE hInstance = ::GetModuleHandleW(nullptr);

	WNDCLASS wc = { 0 };
	if (!::GetClassInfoW(hInstance, wnd_class_name, &wc))
	{
		wc.lpfnWndProc = WindowProc;
		wc.hInstance = hInstance;
		wc.lpszClassName = wnd_class_name;

		if (!::RegisterClassW(&wc))
		{
			ADD_FAILURE()
				<< "RegisterClassW failed with the class name: "
				<< wnd_class_name;
			return false;
		}
	}

	DWORD dwExStyle = WS_EX_OVERLAPPEDWINDOW;
	if (topmost) dwExStyle |= WS_EX_TOPMOST;

	instance->_hwnd = ::CreateWindowExW(
		dwExStyle,
		wc.lpszClassName,   // lpClassName
		title.c_str(),      // windowName
		WS_OVERLAPPEDWINDOW,
		0, 0, 256, 128,     // x, y, width, height
		NULL,               // hWndParent
		NULL,               // hMenu
		hInstance,			// hInstance
		instance			// lpParam
	);

	if (!instance->_hwnd)
	{
		ADD_FAILURE()
			<< "CreateWindowEx failed with the window named, "
			<< title
			<< ", with the error: "
			<< tools::get_error_string(::GetLastError());
		return false;
	}

	if (!button_text.empty())
	{
		instance->_hwnd_button = ::CreateWindowW(
			L"BUTTON",              // lpClassName
			button_text.c_str(),    // windowName (button text)
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			0, 0, 100, 24,          // x, y, width, height
			instance->_hwnd,        // hWndParent
			(HMENU)1,               // hMenu (button id)
			hInstance,				// hInstance
			NULL				    // lpParam
		);

		if (!instance->_hwnd_button)
		{
			ADD_FAILURE() <<
				"CreateWindow for 'BUTTON' failed with the window & button, [ "
				<< title << ", " << button_text
				<< " ] with the error: "
				<< tools::get_error_string(::GetLastError());
		}
	}

	::UpdateWindow(instance->_hwnd);
	::ShowWindow(instance->_hwnd, SW_SHOW);

	return true;
}

bool test_window::was_button_clicked() const
{
	return _button_clicked;
}

bool test_window::has_button() const
{
	return _hwnd_button != NULL;
}

bool window_tool::tests::test_window::was_closed() const
{
	return _window_closed;
}

bool window_tool::tests::test_window::was_minimized() const
{
	return _window_minimized;
}

std::wstring test_window::get_title() const
{
	return test_window::get_window_text(_hwnd);
}

std::wstring test_window::get_button_text() const
{
	return test_window::get_window_text(_hwnd_button);
}

std::wstring test_window::get_window_text(HWND hwnd)
{
	if (!hwnd || !::IsWindow(hwnd)) return std::wstring();

	int length = ::GetWindowTextLengthW(hwnd);
	if (length <= 0) return std::wstring();

	std::wstring text(static_cast<size_t>(length), L'\0');
	::GetWindowTextW(hwnd, text.data(), length + 1);

	return text;
}

LRESULT test_window::handle_message(test_window* current_window, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_COMMAND:
		_button_clicked = LOWORD(wParam) == 1;
		break;
	case WM_SIZE:
		_window_minimized = wParam == SIZE_MINIMIZED;
		break;
	case WM_CLOSE:
		_window_closed = true;
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(_hwnd, uMsg, wParam, lParam);
}

LRESULT test_window::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	test_window* current_window = nullptr;

	if (uMsg == WM_NCCREATE)
	{
		auto create_info = (::CREATESTRUCTW*)lParam;
		current_window = (test_window*)create_info->lpCreateParams;
		::SetWindowLongPtrW(hWnd, GWLP_USERDATA, (LONG_PTR)current_window);

		current_window->_hwnd = hWnd;
	}
	else
	{
		current_window = (test_window*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	}

	if (current_window)
	{
		return current_window->handle_message(current_window, uMsg, wParam, lParam);
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void test_window::message_loop(test_window* instance) {
	::MSG msg;

	while (::GetMessageW(&msg, nullptr, 0, 0))
	{
		::TranslateMessage(&msg);
		::DispatchMessageW(&msg);
	}
}