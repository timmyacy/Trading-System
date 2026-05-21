#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>
struct SimInstrument {
  std::string symbol;
  double basePrice;
};

std::vector<SimInstrument> instruments = {
    {"AAPL", 150.00}, {"MSFT", 300.00},  {"GOOGL", 140.00},
    {"TSLA", 200.00}, {"ESZ4", 4500.00}, {"GBPUSD", 1.26},
};

int main() {
  std::mt19937 generator(std::random_device{}());
  std::uniform_int_distribution<int> symbolPicker(0, instruments.size() - 1);
  std::uniform_int_distribution<int> sidePicker(1, 2);
  std::uniform_int_distribution<int> quantityPicker(1, 1000);
  std::normal_distribution<double> noiseDist(0.0, 0.50);

  std::ofstream file("config/orders.fix");

  for (int i = 1; i <= 10000; i++) {
    SimInstrument &inst = instruments[symbolPicker(generator)];
    int side = sidePicker(generator);
    int quantity = quantityPicker(generator);
    double price = inst.basePrice + noiseDist(generator);

    std::ostringstream id;
    id << "ORD" << std::setw(5) << std::setfill('0') << i;

    file << "11=" << id.str() << "|35=D"
         << "|55=" << inst.symbol << "|54=" << side << "|44=" << std::fixed
         << std::setprecision(2) << price << "|38=" << quantity << "\n";
  }

  file.close();
  std::cout << "10,000 FIX orders written to config/orders.fix\n";
  return 0;
}
