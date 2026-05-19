#include "risk_engine.h"
#include "position_manager.h"
#include "tick.h"
#include "vol_surface.h"
#include <cmath>
#include <iostream>
#include <map>
RiskEngine::RiskEngine(PositionManager &positionManager,
                       VolatilitySurface &volSurface)
    : positionManager_(positionManager), volSurface_(volSurface) {}
void RiskEngine::onTick(const Tick &tick) {
  latestPrices_[tick.symbol] = tick.last;
};
void RiskEngine::runReport() const {
  std::cout << "------------ Risk Report ------------\n";

  for (const auto &[symbol, pos] : positionManager_.all()) {
    double currentPrice = latestPrices_.count(symbol) ? latestPrices_.at(symbol)
                                                      : pos.averageEntryPrice;
    double unrealisedPnl =
        (currentPrice - pos.averageEntryPrice) * pos.quantity;
    std::cout << symbol << "  qty=" << pos.quantity
              << "  price=" << currentPrice
              << "  unrealisedPnl=" << unrealisedPnl
              << "  realisedPnl=" << pos.realisedPnl << "\n";
  }
  std::cout << "------------------------------------------\n";
}
void RiskEngine::computeVar() const {
  double vaR = 0.0;
  for (const auto &[symbol, pos] : positionManager_.all()) {
    double currentPrice = latestPrices_.count(symbol) ? latestPrices_.at(symbol)
                                                      : pos.averageEntryPrice;
    double annualVol = volSurface_.getVol(currentPrice, 30);
    double dailyVol = annualVol / std::sqrt(252.0);
    vaR += pos.quantity * currentPrice * dailyVol;
  }
  vaR = vaR * 1.645;
  std::cout << "VaR (95%, 1-day): $" << vaR << "\n";
};
