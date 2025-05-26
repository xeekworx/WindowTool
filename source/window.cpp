#include "window.h"
#include <functional>
#include <algorithm>

using namespace window_tool;

window::window(HWND hWnd)
{
	this->_hWnd = hWnd;
}

window::~window()
{
}

HWND window::get_handle() const
{
	return _hWnd;
}

bool window::is_valid() const
{
	return ::IsWindow(_hWnd) == TRUE;
}

bool window::is_visible() const
{
	return ::IsWindowVisible(_hWnd) == TRUE;
}

bool window_tool::window::minimize() const
{
	::ShowWindow(_hWnd, SW_MINIMIZE);
	return ::IsIconic(_hWnd) == TRUE;
}

bool window::click_button(const std::wstring& name) const
{
	if (!is_valid()) return false;

	// Case insensitive search
	// https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-findwindowexw

	HWND hButton = ::FindWindowExW(_hWnd, NULL, NULL, name.c_str());

	if (hButton != NULL)
	{
		::SendMessageW(_hWnd, WM_COMMAND, MAKEWPARAM(::GetDlgCtrlID(hButton), 0), (LPARAM)hButton);
		return true;
	}

	return false;
}

bool window::close() const
{
	if (!is_valid()) return false;
	return 0 == ::SendMessageW(_hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);
}

std::wstring window::get_text() const
{
	if (!is_valid()) return std::wstring();

	int titleLength = ::GetWindowTextLengthW(_hWnd);
	if (titleLength <= 0) return std::wstring();

	std::wstring title;
	title.resize(static_cast<size_t>(titleLength));

	::GetWindowTextW(_hWnd, title.data(), titleLength + 1);
	return title;
}

bool window_tool::window::compare_title(
	const std::wstring& title,
	text_compare_type type,
	bool case_insensitive
) const
{
	std::wstring window_title = this->get_text();
	std::wstring find_text = title;

	if (window_title.length() < find_text.length()) {
		return false;
	}

	if (case_insensitive) {
		std::transform(window_title.begin(), window_title.end(), window_title.begin(), ::towlower);
		std::transform(find_text.begin(), find_text.end(), find_text.begin(), ::towlower);
	}

	switch (type) {
	default:
	case text_compare_type::absolute:
		return window_title.compare(find_text) == 0;

	case text_compare_type::contains:
		return window_title.find(find_text) != std::wstring::npos;

	case text_compare_type::starts_with:
		return window_title.substr(0, find_text.length()) == find_text;

	case text_compare_type::ends_with:
		return window_title.substr(window_title.length() - find_text.length()) == find_text;
	}
}

bool window::find_all(
	const std::wstring& title,
	std::list<window>& windows,
	text_compare_type type,
	bool case_insensitive,
	unsigned timeout_ms)
{
	const unsigned interval_ms = 100;
	unsigned waited = 0;
	while (true) {
		windows.clear();
		::EnumWindows([](HWND hwnd, LPARAM lParam) -> BOOL
		{
			std::list<window>& windowList = *reinterpret_cast<std::list<window>*>(lParam);

			windowList.push_back(window(hwnd));

			return TRUE; // Continue
		}, reinterpret_cast<LPARAM>(&windows));

		windows.remove_if([title, type, case_insensitive](const window& win)
		{
			return !win.compare_title(title, type, case_insensitive);
		});

		if (!windows.empty())
			return true;

		if (timeout_ms == 0 || waited >= timeout_ms)
			break;

		::Sleep(interval_ms);
		waited += interval_ms;
	}
	return false;
}
