#pragma once
#include <memory>
#include <thread>
#include <Windows.h>

namespace window_tool::tests {
	class test_window
	{
	public:
		static test_window* create(
			const std::wstring& title,
			bool topmost = false
		);

		static test_window* create(
			const std::wstring& title,
			const std::wstring& button_text = L"",
			bool topmost = false
		);

		~test_window();

		bool was_button_clicked() const;
		bool has_button() const;
		bool was_closed() const;
		std::wstring get_title() const;
		std::wstring get_button_text() const;

	private:
		std::thread _thread;
		HWND _hwnd, _hwnd_button;
		bool _button_clicked;
		bool _window_closed;

		test_window();

		static bool create_internal(
			test_window* instance,
			const std::wstring& title,
			const std::wstring& button_text,
			bool topmost
		);

		static std::wstring get_window_text(HWND hwnd);
		LRESULT handle_message(test_window* current_window, UINT uMsg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		static void message_loop(test_window* instance);
	};
}