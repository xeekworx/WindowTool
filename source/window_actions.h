#pragma once
#include <vector>
#include <string>
#include "window.h"

namespace window_tool {
	class window_actions
	{
	public:
		const int SUCCESS_RESULT = 0;
		const int INVALID_ARGS_RESULT = -1;
		const int NO_WINDOWS_FOUND_RESULT = -2;

		window_actions(const std::vector<std::wstring>& arguments);

		int run();
		void display_usage() const;

	private:
		std::vector<std::wstring> _arguments;
		std::wstring _app_name;

		bool on_find(const window& win) const;
		bool on_close(const window& win) const;
		bool on_click_button(const window& win, const std::wstring& button_name) const;
	};
}
