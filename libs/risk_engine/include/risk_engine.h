#pragma once

#include "position_manager.h"
#include "tick.h"
#include "vol_surface.h"
#include <map>
#include <string>
class RiskEngine {

public:
  RiskEngine(PositionManager &positionManager, VolatilitySurface &volSurface);
  void onTick(const Tick &);
  void runReport() const;
  void computeVar() const;

private:
  PositionManager &positionManager_;
  VolatilitySurface &volSurface_;
  std::map<std::string, double> latestPrices_;
};
