#pragma once
#include <Windows.h>
#include <string>
#include <list>
#include "text_compare_type.h"

namespace window_tool {
	class window
	{
	private:
		HWND _hWnd;

	public:
		window(HWND hWnd);
		~window();

		HWND get_handle() const;
		bool is_valid() const;
		bool is_visible() const;
		bool minimize() const;
		bool click_button(const std::wstring& name) const;
		bool close() const;
		std::wstring get_text() const;

		bool compare_title(
			const std::wstring& title,
			text_compare_type type = text_compare_type::absolute,
			bool case_insensitive = false
		) const;

		static bool find_all(
			const std::wstring& title,
			std::list<window>& windows,
			text_compare_type type = text_compare_type::absolute,
			bool case_insensitive = false,
			unsigned timeout_ms = 0
		);
	};
}