#include "../../source/window_actions.h"
#include "../support/base_test.h"
#include "../support/test_tools.h"
#include "../support/test_window.h"
#include "gtest/gtest.h"
#include <memory>

namespace window_tool::tests {
    class ActionCloseWindow : public base_test {

    };

    TEST_F(ActionCloseWindow, WithWindow) {
        // Arrange
        const std::wstring window_title = tools::random_string(L"Test Window ");
        std::vector<std::wstring> arguments = { default_app_name, window_title, L"close" };
        window_tool::window_actions actions(arguments);

        auto win = std::unique_ptr<test_window>(test_window::create(window_title, true));

        // Act
        int result = actions.run();
        std::wstring output_str = captured_out.str();

        // Assert
        EXPECT_TRUE(win->was_closed());
        EXPECT_EQ(tools::trim(output_str), L"True");
        EXPECT_EQ(result, 0);
    }

    TEST_F(ActionCloseWindow, WithoutWindow) {
        // Arrange
        const std::wstring window_title = tools::random_string(L"Test Window ");
        std::vector<std::wstring> arguments = { default_app_name, window_title, L"close" };
        window_tool::window_actions actions(arguments);

        // Act
        int result = actions.run();
        std::wstring output_str = captured_out.str();

        // Assert
        EXPECT_EQ(tools::trim(output_str), L"False");
        EXPECT_EQ(result, 0);
    }
}
