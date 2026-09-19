#pragma once

#include <owlet/utils/defer.h>

#include <exception>

#define WLT_DEF_EMPTY_CALLBACKS                           \
    void ExceptionCallback(std::exception& e) noexcept {} \
    void UnknownExceptionCallback() noexcept {}           \
    void ExitCallback() {}

namespace owlet {
void Run();

void ExceptionCallback(std::exception& e) noexcept;
void UnknownExceptionCallback() noexcept;
void ExitCallback();

}    // namespace owlet

int main() {
    auto def = owlet::Defer(owlet::ExitCallback);
    try {
        owlet::Run();
    } catch (std::exception& e) {
        owlet::ExceptionCallback(e);
    } catch (...) {
        owlet::UnknownExceptionCallback();
    }
    return 0;
}
