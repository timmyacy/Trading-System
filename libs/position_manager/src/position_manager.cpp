#include "position_manager.h"
#include <cstddef>

void PositionManager::onExecution(const Execution &exec) {
  Position &position = positions_[exec.symbol];
  if (position.symbol.empty()) {
    position.symbol = exec.symbol;
  }

  if (exec.side == Side::BUY) {
    if (position.quantity < 0) {
      // cover shorts
      double coverQty = std::min((double)exec.quantity, -position.quantity);
      position.realisedPnl +=
          coverQty * (position.averageEntryPrice - exec.trade_price);
      position.quantity += exec.quantity;
      if (position.quantity > 0) {
        // flipped to long
        position.averageEntryPrice = exec.trade_price;
      }
      // if still short, avgEntry stays as the original short entry
    } else {
      // adding to or opening a long position
      double newAvg = ((position.quantity * position.averageEntryPrice) +
                       ((double)exec.quantity * exec.trade_price)) /
                      (position.quantity + exec.quantity);
      position.quantity += exec.quantity;
      position.averageEntryPrice = newAvg;
    }
  }

  if (exec.side == Side::SELL) {
    if (position.quantity > 0) {
      if ((double)exec.quantity >= position.quantity) {
        // close entire long, open short with remainder
        double remainder = exec.quantity - position.quantity;
        position.realisedPnl +=
            position.quantity * (exec.trade_price - position.averageEntryPrice);
        position.quantity = -remainder;
        position.averageEntryPrice = (remainder > 0) ? exec.trade_price : 0;
      } else {
        // partial close
        position.realisedPnl +=
            exec.quantity * (exec.trade_price - position.averageEntryPrice);
        position.quantity -= exec.quantity;
      }
    } else if (position.quantity == 0) {
      // open a new short
      position.quantity = -(double)exec.quantity;
      position.averageEntryPrice = exec.trade_price;
    } else {
      // already short so add to short
      double existingShort = -position.quantity;
      double totalShort = existingShort + exec.quantity;
      position.averageEntryPrice = (existingShort * position.averageEntryPrice +
                                    exec.quantity * exec.trade_price) /
                                   totalShort;
      position.quantity -= exec.quantity;
    }
  }
};
Position PositionManager::getPosition(const std::string &symbol) const {
  return positions_.at(symbol);
}

const std::unordered_map<std::string, Position> &PositionManager::all() const {
  return positions_;
}
