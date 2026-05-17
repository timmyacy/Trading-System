#include "market_dispatcher.h"
#include <functional>
#include <string>

void MarketDispatcher::subscribe(const std::string &symbol,
                                 const std::function<void(Tick)> &callback) {
  if (!symbol.empty()) {
    listCallback[symbol].push_back(callback);
  }
};

void MarketDispatcher::dispatch(const Tick &tick) {
  auto it = listCallback.find(tick.symbol);
  if (it == listCallback.end())
    return;
  for (auto &callback : it->second) {
    callback(tick);
  }
}

int MarketDispatcher::subscriberCount(const std::string &symbol) const {
  auto it = listCallback.find(symbol);
  if (it == listCallback.end())
    return 0;
  return it->second.size();
}
