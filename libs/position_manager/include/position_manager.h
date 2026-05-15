#pragma once
#include "order_book.h"
#include "position.h"
#include <string>
#include <unordered_map>
class PositionManager {

public:
  void onExecution(const Execution &exec);

  Position getPosition(const std::string &symbol);

  std::unordered_map<std::string, Position> &all();

private:
  std::unordered_map<std::string, Position> positions_;
};
