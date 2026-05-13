#include "instrument_store.h"
#include <fstream>
#include <string>

using namespace std;

void InstrumentStore::loadFromCSV(const string &filePath) {

  ifstream file(filePath);

  string line;
  getline(file, line);
  int lineNumber = 1;
  while (getline(file, line)) {
    vector<string> fields;
    stringstream ss(line);
    string token;
    lineNumber++;
    while (getline(ss, token, ','))
      fields.push_back(trim(token));
    if (fields[0].empty())
      throw runtime_error("Missing symbol on line " + to_string(lineNumber));

    Instrument inst;

    inst.symbol = fields[0];
    inst.description = fields[1];
    inst.assetClass = parseAssetClass(fields[2]);
    inst.currency = fields[3];
    inst.tickSize = stod(fields[4]);
    inst.contractSize = stod(fields[5]);

    inst.strike =
        fields[6].empty() ? nullopt : optional<double>{stod(fields[6])};
    inst.optionType = parseOptionType(fields[7]);
    inst.optionStyle = parseOptionStyle(fields[8]);
    inst.underlying = fields[9].empty() ? nullopt : optional<string>{fields[9]};
    inst.expiry = parseDate(fields[10]);

    instruments_[inst.symbol] = inst;
  }
}

AssetClass InstrumentStore::parseAssetClass(const string &s) {
  if (s == "Equity")
    return AssetClass::Equity;
  if (s == "Future")
    return AssetClass::Future;
  if (s == "Option")
    return AssetClass::Option;
  if (s == "FX")
    return AssetClass::FX;
  throw runtime_error("Unknown asset class: " + s);
}

optional<OptionType> InstrumentStore::parseOptionType(const string &s) {
  if (s.empty())
    return nullopt;
  if (s == "Call")
    return OptionType::Call;
  if (s == "Put")
    return OptionType::Put;
  throw runtime_error("Unknown option type: " + s);
}

optional<OptionStyle> InstrumentStore::parseOptionStyle(const string &s) {
  if (s.empty())
    return nullopt;
  if (s == "European")
    return OptionStyle::European;
  if (s == "American")
    return OptionStyle::American;
  throw runtime_error("Unknown option style: " + s);
}

optional<chrono::year_month_day> InstrumentStore::parseDate(const string &s) {
  if (s.empty())
    return nullopt;

  int y = stoi(s.substr(0, 4));
  int m = stoi(s.substr(5, 2));
  int d = stoi(s.substr(8, 2));

  return chrono::year{y} / chrono::month{static_cast<unsigned>(m)} /
         chrono::day{static_cast<unsigned>(d)};
}
const Instrument &InstrumentStore::lookup(const string &symbol) const {
  auto it = instruments_.find(symbol);
  if (it == instruments_.end())
    throw runtime_error("Instrument not found: " + symbol);
  return it->second;
}

size_t InstrumentStore::size() const { return instruments_.size(); }

string InstrumentStore::trim(const string &s) {
  size_t start = s.find_first_not_of(" \t\r\n");
  size_t end = s.find_last_not_of(" \t\r\n");
  return (start == string::npos) ? "" : s.substr(start, end - start + 1);
}
