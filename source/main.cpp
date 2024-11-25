#include "version.h"
#include "window_actions.h"
#include <iostream>
#include <unordered_set>
#include <vector>
#include <functional>

bool should_show_help(const std::vector<std::wstring>& arguments, std::function<void()> display_func);
bool should_show_version(const std::vector<std::wstring>& arguments);

int wmain(int argc, wchar_t* argv[])
{
	std::vector<std::wstring> arguments;
	for (int i = 0; i < argc; ++i)
		arguments.push_back(argv[i]);

	window_tool::window_actions actions(arguments);

	if (should_show_version(arguments))
		return 0;

	if (should_show_help(arguments, [actions]() { actions.display_usage(); }))
		return 0;

	return actions.run();
}

bool should_show_help(const std::vector<std::wstring>& arguments, std::function<void()> display_func)
{
	std::unordered_set<std::wstring> possibleHelpOptions = {
		L"--help", L"/help", L"/?", L"-?", L"-h"
	};

	if (arguments.size() > 1 && possibleHelpOptions.find(arguments[1]) != possibleHelpOptions.end())
	{
		display_func();
		return true;
	}

	return false;
}

bool should_show_version(const std::vector<std::wstring>& arguments)
{
	std::unordered_set<std::wstring> possibleVersionOptions = {
		L"--version", L"-v"
	};

	if (arguments.size() > 1 && possibleVersionOptions.find(arguments[1]) != possibleVersionOptions.end())
	{
		std::wcout << L"v" << APP_VERSION << std::endl;
		return true;
	}

	return false;
}
