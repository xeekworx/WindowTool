#include "../../source/window_actions.h"
#include "../support/base_test.h"
#include "../support/test_tools.h"
#include "../support/test_window.h"
#include "gtest/gtest.h"
#include <memory>
#include <thread>

namespace window_tool::tests {
    class ActionWaitWindow : public base_test {
    };

    TEST_F(ActionWaitWindow, WaitsForWindowToAppear) {
        // Arrange
        const std::wstring window_title = tools::random_string(L"Test Window ");
        std::vector<std::wstring> arguments = { default_app_name, window_title, L"wait", L"1500" };
        window_tool::window_actions actions(arguments);

        // Start a thread to create the window after a short delay
        std::thread delayed_window([window_title]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            auto win = std::unique_ptr<test_window>(test_window::create(window_title, true));
            std::this_thread::sleep_for(std::chrono::milliseconds(2000)); // Keep window alive for a bit
        });

        // Act
        int result = actions.run();
        std::wstring output_str = captured_out.str();
        delayed_window.join();

        // Assert
        EXPECT_EQ(tools::trim(output_str), L"True");
        EXPECT_EQ(result, 0);
    }

    TEST_F(ActionWaitWindow, TimesOutIfWindowDoesNotAppear) {
        // Arrange
        const std::wstring window_title = tools::random_string(L"Test Window ");
        std::vector<std::wstring> arguments = { default_app_name, window_title, L"wait", L"1000" };
        window_tool::window_actions actions(arguments);

        // Act
        int result = actions.run();
        std::wstring output_str = captured_out.str();

        // Assert
        EXPECT_EQ(tools::trim(output_str), L"False");
        EXPECT_EQ(result, 0);
    }
}