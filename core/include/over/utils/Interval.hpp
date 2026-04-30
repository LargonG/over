#pragma once

#include <over/core/Types.hpp>

namespace over {

class Interval {
 public:
  Interval(float32 startTime, float32 interval) noexcept
      : _lastTime(startTime), _interval(interval) {}

  bool Check(float32 time) {
    if (time - _lastTime >= _interval) {
      _lastTime = time;
      return true;
    }
    return false;
  }

 private:
  float32 _lastTime;
  float32 _interval;
};
}  // namespace over
