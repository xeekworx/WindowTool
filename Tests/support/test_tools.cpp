#include "test_tools.h"

namespace window_tool::tests::tools {

    std::wstring& rtrim(std::wstring& str, const wchar_t* characters)
    {
        str.erase(str.find_last_not_of(characters) + 1);
        return str;
    }

    std::wstring& ltrim(std::wstring& str, const wchar_t* characters)
    {
        str.erase(0, str.find_first_not_of(characters));
        return str;
    }

    std::wstring& trim(std::wstring& str, const wchar_t* characters)
    {
        return ltrim(rtrim(str, characters), characters);
    }

    std::wstring random_string(const std::wstring& prefix, unsigned length, bool include_digits)
    {
        static const std::wstring base_characters =
            L"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            L"abcdefghijklmnopqrstuvwxyz";

        static const std::wstring characters_with_digits =
            L"0123456789" + base_characters;

        std::wstring result = prefix;

        std::wstring characters = include_digits
            ? characters_with_digits
            : base_characters;

        for (unsigned i = 0; i < length; ++i)
            result += characters[rand() % characters.length()];

        return result;
    }

    std::wstring get_error_string(DWORD error_code) {
        LPWSTR buffer = nullptr;

        size_t size = ::FormatMessageW(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr,
            error_code,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPWSTR)&buffer,
            0,
            nullptr
        );

        std::wstring message(buffer, size);

        // Free the buffer allocated by FormatMessage
        ::LocalFree(buffer);

        return message;
    }
}