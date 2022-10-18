/// Copyright (c) 2022 Yoshikawa Teru
/// This software is released under the MIT License, see LICENSE.
/// @file Sequence.h
/// @author Yoshikawa Teru
#ifndef RCT_POLLING_TIMER_H_
#define RCT_POLLING_TIMER_H_

#ifdef __MBED__
#include <mbed.h>

namespace rct {

struct PollingTimer {
  PollingTimer() noexcept {
    timer_.start();
  };
  bool operator()(const chrono::microseconds wait) {
    delta_ = timer_.elapsed_time();
    reset();
    return delta_ > wait;
  }
  auto delta_time() const noexcept {
    return delta_;
  }
  void reset() {
    timer_.reset();
  }
 private:
  Timer timer_ = {};
  chrono::microseconds delta_ = {};
};

}  // namespace rct

#elif defined(ARDUINO)

struct PollingTimer {
  PollingTimer() noexcept : pre_{millis()} {}
  bool operator()(const unsigned wait_ms) {
    delta_ = millis() - pre_;
    reset();
    return delta_ > wait_ms;
  }
  auto delta_time() const noexcept {
    return delta_;
  }
  void reset() {
    pre_ = millis();
  }
 private:
  unsigned long pre_;
  unsigned long delta_ = {};
};

#else
#include <chrono>
struct PollingTimer {
  PollingTimer() noexcept : pre_{std::chrono::steadly_clock::now()} {}
  bool operator()(const unsigned wait_ms) {
    delta_ = std::chrono::steadly_clock::now() - pre_;
    reset();
    return delta_ > wait_ms;
  }
  auto delta_time() const noexcept {
    return delta_;
  }
  void reset() {
    pre_ = std::chrono::steadly_clock::now();
  }
 private:
  decltype(std::chrono::steadly_clock::now()) pre_;
  decltype(pre_) delta_ = {};
};
#endif

#endif  // RCT_POLLING_TIMER_H_
