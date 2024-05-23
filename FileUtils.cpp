#include "FileUtils.h"
#include <fstream>
#include <iostream>
#include <sstream>

void readRatesFromFile(const std::string &fileName, std::vector<std::string> &tenors, std::vector<double> &rates, bool skipfirstline)
{
    std::ifstream inputFile(fileName);

    if (!inputFile.is_open())
    {
        std::cerr << "Error: Could not open the file." << std::endl;
        return;
    }

    std::string line;
    // Skip the first line (header)
    if (skipfirstline == true)
    {
        std::getline(inputFile, line);
    }

    while (std::getline(inputFile, line))
    {
        size_t colonPos = line.find(':');
        size_t percentPos = line.find('%');

        if (colonPos == std::string::npos || percentPos == std::string::npos)
        {
            std::cerr << "Error: Incorrect format in line: " << line << std::endl;
            continue;
        }

        try
        {
            std::string tenor = line.substr(0, colonPos); // Convert to string
            // Convert "0" to string literal
            if (tenor == "0")
            {
                tenor = "0"; // or tenor = "0" + tenor;
            }
            double rate = std::stod(line.substr(colonPos + 1, percentPos - colonPos - 1));

            tenors.push_back(tenor);
            rates.push_back(rate);
        }
        catch (const std::exception &e)
        {
            std::cerr << "Exception occurred while parsing line: " << line << std::endl;
            std::cerr << e.what() << std::endl;
        }
    }

    inputFile.close();
}

void readStockPricesFromFile(const std::string &fileName, std::unordered_map<std::string, double> &stockPrices)
{
    std::ifstream inputFile(fileName);

    if (!inputFile.is_open())
    {
        std::cerr << "Error: Could not open the file." << std::endl;
        return;
    }

    std::string line;
    while (std::getline(inputFile, line))
    {
        std::istringstream iss(line);
        std::string stockName;
        double price;

        if (!(iss >> stockName >> price))
        {
            std::cerr << "Error: Incorrect format in line: " << line << std::endl;
            continue;
        }

        stockPrices[stockName] = price;
    }

    inputFile.close();
}