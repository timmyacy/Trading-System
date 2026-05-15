#include "position_manager.h"
#include "order_book.h"
#include <cstddef>

void PositionManager::onExecution(const Execution &exec) {
  Position position{};
  if (positions_[exec.symbol].symbol.empty()) {
    position.symbol = exec.symbol;
    position.averageEntryPrice = 0;
    position.quantity = 0;
    position.realisedPnl = 0;
    position.unrealisedPnl = 0;
    positions_.insert({exec.symbol, position});
  } else {
    positions_[exec.symbol] = position;
  }

  if (exec.side == Side::BUY) {

    position.quantity += exec.quantity;
    double newAvgPrice = ((exec.quantity * exec.trade_price) +
                          (position.quantity * position.averageEntryPrice)) /
                         position.quantity;

    position.averageEntryPrice = newAvgPrice;
  };
  if (exec.side == Side::SELL) {

    position.realisedPnl +=
        exec.quantity * (exec.trade_price - position.averageEntryPrice);
    position.quantity -= exec.quantity;
    double newAvgPrice = ((exec.quantity * exec.trade_price) +
                          (position.quantity * position.averageEntryPrice)) /
                         position.quantity;

    position.averageEntryPrice = newAvgPrice;
  };
}
// public:
//   void onExecution(const Execution &exec);

//   Position getPosition(const std::string &symbol);

//   std::unordered_map<std::string, Position> &all();

// private:
//   std::unordered_map<std::string, Position> positions_;
// };

// First check if a position for exec.symbol exists in positions_ — if not,
// create a blank one with zero quantity, zero average price, zero P&L and
// insert it. Then check the side — is this execution a buy or sell. Based on
// that either: Adding to position (buying when long, selling when short):

// New average price = (old quantity × old avg price + trade qty × trade price)
// / new total quantity Add trade quantity to position quantity

// Reducing position (selling when long, buying when short):

// Realised PnL += trade quantity × (trade price − average entry price)
// Subtract trade quantity from position quantity

// Flipping (selling more than you own):

// Close the existing position first — lock in P&L on the existing quantity
// Open a new position in the opposite direction with the remainder
