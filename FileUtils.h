#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <vector>
#include <string>

void readRatesFromFile(const std::string &fileName, std::vector<std::string> &tenors, std::vector<double> &rates, bool skipfirstline);

#endif // FILE_UTILS_H
