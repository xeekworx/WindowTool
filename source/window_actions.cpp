#include "window_actions.h"
#include <filesystem>
#include <iostream>
#include <locale>
#include <string>

using namespace window_tool;

window_actions::window_actions(const std::vector<std::wstring>& arguments)
{
	_app_name = std::filesystem::path(arguments[0]).filename();
	_arguments.resize(arguments.size() - 1);
	std::copy(arguments.begin() + 1, arguments.end(), _arguments.begin());
}

int window_actions::run()
{
	if (_arguments.size() < 2)
	{
		std::wcout << L"Error: Too few arguments." << std::endl << std::endl;
		display_usage();
		return INVALID_ARGS_RESULT;
	}

	std::wstring window_title;
	std::wstring action;
	std::wstring button_name;

	if (_arguments.size() >= 1)
	{
		window_title = _arguments[0];
	}
	if (_arguments.size() >= 2)
	{
		action = _arguments[1];
		std::transform(action.begin(), action.end(), action.begin(), ::towlower);
	}
	if (_arguments.size() >= 3)
	{
		button_name = _arguments[2];
	}

	if (_arguments.size() < 3 && action == L"click")
	{
		std::wcout << L"Error: Too few arguments for action 'click'." << std::endl << std::endl;
		display_usage();
		return INVALID_ARGS_RESULT;
	}

	std::list<window> windows;
	window::find_all(window_title, windows);

	bool actionResult = false, found = false;
	for (auto& win : windows) {
		if (action == L"find") actionResult = on_find(win);
		else if (action == L"close") actionResult = on_close(win);
		else if (action == L"minimize") actionResult = on_minimize(win);
		else if (action == L"click") actionResult = on_click_button(win, button_name);
		else
		{
			std::wcout << L"Error: Invalid action argument." << std::endl << std::endl;
			display_usage();
			return INVALID_ARGS_RESULT;
		}

		found = true;
		break;
	}

	// Does not follow original CLI behavior
	if (!found)
	{
		//std::wcout << L"Error: No windows were found." << std::endl;
		//return NO_WINDOWS_FOUND_RESULT;
	}

	std::wcout << (actionResult ? L"True" : L"False") << std::endl;
	return SUCCESS_RESULT;
}

void window_actions::display_usage() const
{
	std::wcout << L"Usage: " << _app_name << L" \"Window Title\" \"Action\" [ButtonName]" << std::endl
		<< std::endl
		<< L"Options surrounded by [] are optional, but do not include the brackets." << std::endl
		<< "For the button name, take note of possible & symbols." << std::endl
		<< std::endl
		<< L"Actions that can be used:" << std::endl
		<< L"  find     Outputs \"True\" if the window is found" << std::endl
		<< L"  close    Attempt to close the window" << std::endl
		<< L"  minimize Minimize the window" << std::endl
		<< L"  click    Click a button with the name given" << std::endl;
}

bool window_actions::on_find(const window& win) const
{
	// If we've reached this point, the window was found and visible before this action was called.
	return true;
}

bool window_actions::on_close(const window& win) const
{
	return win.close();
}

bool window_tool::window_actions::on_minimize(const window& win) const
{
	return win.minimize();
}

bool window_actions::on_click_button(const window& win, const std::wstring& button_name) const
{
	return win.click_button(button_name);
}
