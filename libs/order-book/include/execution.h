#include <chrono>
#include <string>
struct Execution {
  std::string buy_id;
  std::string sell_id;
  std::string symbol;
  double trade_price;
  double quantity;
  std::chrono::system_clock::time_point arrival;
};
