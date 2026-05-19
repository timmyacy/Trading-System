#include "fix_session_reader.h"
#include "instrument_store.h"
#include "market_dispatcher.h"
#include "order_book.h"
#include "position_manager.h"
#include "udp_feed_subscriber.h"
#include <iostream>
#include <thread>
int main() {
  InstrumentStore store;
  store.loadFromCSV("config/instruments.csv");

  OrderBook orderBook;
  PositionManager positionManager;

  orderBook.setExecutionCallback(
      [&](Execution exec) { positionManager.onExecution(exec); });

  FixSessionReader fixReader;
  fixReader.setOrderCallback(
      [&](Order order) { orderBook.acceptNewOrder(order); });

  fixReader.start("config/orders.fix");

  for (const auto &[symbol, pos] : positionManager.all()) {
    std::cout << symbol << "  qty=" << pos.quantity
              << "  avg=" << pos.averageEntryPrice
              << "  pnl=" << pos.realisedPnl << "\n";
  }
  // Tests for spam values
  // MarketDispatcher dispatcher;
  // UDPFeedSubscriber subscriber;
  // std::thread udpThread([&]() { subscriber.start(9000, dispatcher); });
  // std::this_thread::sleep_for(std::chrono::seconds(40));
  return 0;
}
