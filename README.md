[![Build](https://github.com/xeekworx/WindowTool/actions/workflows/build.yml/badge.svg)](https://github.com/xeekworx/WindowTool/actions/workflows/build.yml)
[![Test](https://github.com/xeekworx/WindowTool/actions/workflows/test.yml/badge.svg)](https://github.com/xeekworx/WindowTool/actions/workflows/test.yml)

# WindowTool

WindowTool is a versatile C++ tool designed for automated interaction with window elements on the Windows operating system. It provides a command-line interface to perform specific actions on windows identified by their title. Key functionalities include finding windows, closing them, or simulating button clicks. This tool is particularly useful for automating GUI tasks, testing user interfaces, or controlling windowed applications programmatically.

## Usage
To use `WindowTool.exe`, follow the command-line syntax below:
| Action  | Description                                                 |
| ------- | ----------------------------------------------------------- |
| find    | Outputs "True" if the window is found or "False otherwise.  |
| close   | Attempt to close the window.                                |
| click   | Click a button with the name given a the third argument.    |

`0` is returned for success or `1` for error. If the action occurred as asked, the tool will output (stdout) `True` or `False`.  
#### Example:
```ps
PS C:\Users\xeek> WindowTool.exe "Title of Window" click "Button Text"
True
```
```ps
PS C:\Users\xeek> WindowTool.exe "Title of Window" find
True
```

## Installation
You can put the tool anywhere you need it, and rename if if desired.

## Building and Testing
To build and test WindowTool, follow these steps:
1. Clone the repository.
2. Open `WindowTool.sln` in Visual Studio.
3. Build the solution by pressing `F7` or using the `Build` menu.
4. To run tests, navigate to the `Test Explorer` in Visual Studio and run all tests.
