#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <vector>
#include <string>
#include "Market.h"

void readRatesFromFile(const std::string &fileName, std::vector<std::string> &tenors, std::vector<double> &rates, bool skipfirstline);
void readVolCurveFromFile(const std::string &fileName, VolCurve &volCurve);
void readStockPricesFromFile(const std::string &fileName, std::unordered_map<std::string, double> &stockPrices);
void readBondPricesFromFile(const std::string &fileName, std::unordered_map<std::string, double> &bondPrices);

#endif // FILE_UTILS_H
