#pragma once
#include "execution.h"
#include "position.h"
#include <string>
#include <unordered_map>
class PositionManager {

public:
  void onExecution(const Execution &exec);

  Position getPosition(const std::string &symbol) const;

  const std::unordered_map<std::string, Position> &all() const;

private:
  std::unordered_map<std::string, Position> positions_;
};
