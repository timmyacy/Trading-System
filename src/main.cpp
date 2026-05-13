#include "instrument_store.h"
#include <iostream>

int main() {
  InstrumentStore store;

  try {
    store.loadFromCSV("config/instruments.csv");
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }
  std::cout << "Loaded " << store.size() << " instruments\n";

  auto &aapl = store.lookup("AAPL");
  std::cout << aapl.symbol << " " << aapl.currency << "\n";

  auto &opt = store.lookup("AAPL240119C150");
  std::cout << opt.symbol << " strike: " << opt.strike.value() << "\n";

  auto &fut = store.lookup("ESZ4");
  std::cout << fut.symbol << " contractSize: " << fut.contractSize << "\n";
}
