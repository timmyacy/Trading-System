#include "position_manager.h"
#include <cstddef>

void PositionManager::onExecution(const Execution &exec) {
  Position &position = positions_[exec.symbol];
  if (position.symbol.empty()) {
    position.symbol = exec.symbol;
  }

  if (exec.side == Side::BUY) {

    double newAvgPrice = ((position.quantity * position.averageEntryPrice) +
                          (exec.quantity * exec.trade_price)) /
                         (position.quantity + exec.quantity);

    position.quantity += exec.quantity;
    position.averageEntryPrice = newAvgPrice;
  }

  if (exec.side == Side::SELL) {

    // Cover for if we are selling fpr more than we own
    if (exec.quantity > position.quantity) {
      double remainder = exec.quantity - position.quantity;
      // Lock in PnL with remainder qty and then open short with remainder
      position.realisedPnl +=
          position.quantity * (exec.trade_price - position.averageEntryPrice);
      position.quantity = -remainder;
      position.averageEntryPrice = exec.trade_price;
    } else {
      position.realisedPnl +=
          exec.quantity * (exec.trade_price - position.averageEntryPrice);
      position.quantity -= exec.quantity;

      if (position.quantity == 0) {
        position.averageEntryPrice = 0;
      };
    }
  };
};
Position PositionManager::getPosition(const std::string &symbol) const {
  return positions_.at(symbol);
}

const std::unordered_map<std::string, Position> &PositionManager::all() const {
  return positions_;
}
