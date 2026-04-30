#pragma once

#include <fmt/core.h>

namespace over {

void Run();

}  // namespace over

int main() {
  try {
    over::Run();
  } catch (std::exception& e) {
    fmt::println("{}", e.what());
    throw;
  } catch (...) {
    fmt::println(
        "Unexpected error, does not incherit std::exception, please, correct");
    throw;
  }
}
