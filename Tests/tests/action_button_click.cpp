#include "../../source/window_actions.h"
#include "../support/base_test.h"
#include "../support/test_tools.h"
#include "../support/test_window.h"
#include "gtest/gtest.h"
#include <memory>

namespace window_tool::tests {
    constexpr auto APP_NAME = L"WindowTool.exe";

    class ActionButtonClick : public base_test {

    };

    TEST_F(ActionButtonClick, WithWindowAndButton) {
        // Arrange
        const std::wstring window_title = tools::random_string(L"Test Window ");
        const std::wstring button_text = L"Click Me";
        std::vector<std::wstring> arguments = { default_app_name, window_title, L"click", button_text };
        window_tool::window_actions actions(arguments);

        auto win = std::unique_ptr<test_window>(test_window::create(window_title, button_text, true));

        // Act
        auto result = actions.run();
        auto output_str = captured_out.str();

        // Assert
        EXPECT_TRUE(win->was_button_clicked());
        EXPECT_EQ(tools::trim(output_str), L"True");
        EXPECT_EQ(result, 0);
    }

    TEST_F(ActionButtonClick, WithoutWindowAndButton) {
        // Arrange
        const std::wstring window_title = tools::random_string(L"Test Window ");
        const std::wstring button_text = L"Click Me";
        std::vector<std::wstring> arguments = { default_app_name, window_title, L"click", button_text };
        window_tool::window_actions actions(arguments);

        // Act
        auto result = actions.run();
        auto output_str = captured_out.str();

        // Assert
        EXPECT_EQ(tools::trim(output_str), L"False");
        EXPECT_EQ(result, 0);
    }

    TEST_F(ActionButtonClick, WithWindowAndWithoutButton) {
        // Arrange
        const std::wstring window_title = tools::random_string(L"Test Window ");
        const std::wstring button_text = L"Click Me";
        std::vector<std::wstring> arguments = { default_app_name, window_title, L"click", button_text };
        window_tool::window_actions actions(arguments);

        auto win = std::unique_ptr<test_window>(test_window::create(window_title, true));

        // Act
        auto result = actions.run();
        auto output_str = captured_out.str();

        // Assert
        EXPECT_FALSE(win->was_button_clicked());
        EXPECT_EQ(tools::trim(output_str), L"False");
        EXPECT_EQ(result, 0);
    }
}
