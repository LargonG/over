#pragma once

#include <fmt/core.h>

namespace over {

void Run();

}  // namespace over

int main() {
  try {
    over::Run();
  } catch (std::exception& e) {
    fmt::println("std::exception error: {}", e.what());
    throw;
  } catch (...) {
    fmt::println(
        "Unexpected error, does not inherits std::exception, please, correct");
    throw;
  }
}
