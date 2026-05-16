#pragma once
#include "order.h"
#include <atomic>
#include <functional>

class FixSessionReader {
public:
  void start(const std::string &filepath);
  void stop();
  void setOrderCallback(const std::function<void(Order)> &callback);

private:
  Order parseFixMessage(const std::string &line);
  std::atomic_bool running_;
  std::function<void(Order)> onOrder_;
};
