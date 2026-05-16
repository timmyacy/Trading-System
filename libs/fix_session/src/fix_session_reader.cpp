#include "fix_session_reader.h"
#include "order.h"
#include <fstream>
#include <sstream>
using namespace std;
Order FixSessionReader::parseFixMessage(const std::string &line) {
  stringstream ss(line);
  string item;
  Order order{};

  while (getline(ss, item, '|')) {
    int position = item.find('=');
    string tag = item.substr(0, position);
    string value = item.substr(position + 1);

    if (tag == "11") {
      order.id = value;
    }
    if (tag == "55") {
      order.symbol = value;
    }
    if (tag == "54") {
      order.order_type = value == "1" ? OrderType::BUY : OrderType::SELL;
    }
    if (tag == "44") {
      order.price = stod(value);
    }
    if (tag == "38") {
      order.quantity = stoi(value);
    };
  }
  return order;
};

void FixSessionReader::start(const std::string &filepath) {
  running_ = true;
  ifstream file(filepath);
  if (!file.is_open()) {
    throw std::runtime_error("Could not open file");
  }
  if (file.is_open()) {
    string line;
    while (getline(file, line) && running_) {
      stringstream ss(line);
      if (line.empty()) {
        continue;
      }
      Order order = parseFixMessage(line);
      if (onOrder_) {
        onOrder_(order);
      }
    }
  }
}

void FixSessionReader::stop() { running_ = false; };
void FixSessionReader::setOrderCallback(
    const std::function<void(Order)> &callback) {
  onOrder_ = callback;
}
