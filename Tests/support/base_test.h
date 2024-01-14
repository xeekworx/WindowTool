#pragma once
#include <gtest/gtest.h>
#include <ostream>
#include <sstream>

namespace window_tool::tests {
    class base_test : public ::testing::Test {
    protected:
        const std::wstring default_app_name = L"WindowTool.exe";
        std::wstreambuf* old_cout_buffer;
        std::wostringstream captured_out;

        void SetUp() override {
            old_cout_buffer = std::wcout.rdbuf();
            std::wcout.rdbuf(captured_out.rdbuf());
        }

        void TearDown() override {
            std::wcout.rdbuf(old_cout_buffer);
        }
    };
}