// WindowTool.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "window_actions.h"
#include <iostream>
#include <unordered_set>
#include <vector>
#include <functional>

bool should_show_help(const std::vector<std::wstring>& arguments, std::function<void()> display_func);

int wmain(int argc, wchar_t* argv[])
{
	std::vector<std::wstring> arguments;
	for (int i = 0; i < argc; ++i)
		arguments.push_back(argv[i]);

	window_tool::window_actions actions(arguments);

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
