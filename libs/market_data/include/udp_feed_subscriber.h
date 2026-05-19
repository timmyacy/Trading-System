#pragma once
#include "market_dispatcher.h"
#include <atomic>

class UDPFeedSubscriber {
public:
  void start(int port, MarketDispatcher &dispatcher);
  void stop();

private:
  std::atomic_bool running_;
};
