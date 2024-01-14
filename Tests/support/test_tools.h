#pragma once
#include <string>
#include <Windows.h>

namespace window_tool::tests::tools
{
	std::wstring& rtrim(std::wstring& str, const wchar_t* characters = L" \t\n\r\f\v");
	std::wstring& ltrim(std::wstring& str, const wchar_t* characters = L" \t\n\r\f\v");
	std::wstring& trim(std::wstring& str, const wchar_t* characters = L" \t\n\r\f\v");
	std::wstring random_string(const std::wstring& prefix = L"", unsigned length = 4, bool include_digits = false);
	std::wstring get_error_string(DWORD error_code);
}
