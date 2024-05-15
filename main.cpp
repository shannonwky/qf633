#include <fstream>
#include <ctime>
#include <chrono>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm> // For the trim function



#include "Date.cpp"
#include "Market.cpp"
#include "Pricer.cpp"
#include "Swap.cpp"

#include "Market.h"
#include "Pricer.h"
#include "EuropeanTrade.h"
#include "AmericanTrade.h"
#include "Bond.h"
#include "Swap.h"


#include <iostream>
#include <string>

using namespace std;

void readFromFile(const string& fileName, string& outPut){
  string lineText;
  ifstream MyReadFile(fileName);
  // Skip the first line
    if (getline(MyReadFile, lineText)) {
        // Successfully read and skipped the first line
    }
  while (getline (MyReadFile, lineText)) {
    outPut.append(lineText);
  }
  MyReadFile.close();
}

// Trim whitespace from the beginning and end of a string
std::string trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\r\n");
    size_t end = str.find_last_not_of(" \t\r\n");
    if (start == std::string::npos) {
        return ""; // Empty string or whitespace only
    }
    return str.substr(start, end - start + 1);
}

void readRatesFromFile(const string& fileName, vector<string>& tenors, vector<double>& rates) {
    ifstream inputFile(fileName);

    if (!inputFile.is_open()) {
        cerr << "Error: Could not open the file." << endl;
        return;
    }

    string line;
    // Skip the first line (header)
    getline(inputFile, line);

    while (getline(inputFile, line)) {
        size_t colonPos = line.find(':');
        size_t percentPos = line.find('%');

        if (colonPos == string::npos || percentPos == string::npos) {
            cerr << "Error: Incorrect format in line: " << line << endl;
            continue;
        }

        try {
            string tenor = line.substr(0, colonPos); // Convert to string
            // Convert "0" to string literal
            if (tenor == "0") {
                tenor = "0"; // or tenor = "0" + tenor;
            }
            double rate = stod(line.substr(colonPos + 1, percentPos - colonPos - 1));
            
            tenors.push_back(tenor);
            rates.push_back(rate);
        } catch (const exception& e) {
            cerr << "Exception occurred while parsing line: " << line << endl;
            cerr << e.what() << endl;
        }
    }

    inputFile.close();
}


int main()
{
  //task 1, create an market data object, and update the market data from from txt file 
  std::time_t t = std::time(0);
  auto now_ = std::localtime(&t);
  Date valueDate;
  valueDate.year = now_->tm_year + 1900;
  valueDate.month = now_->tm_mon + 1;
  valueDate.year = now_->tm_mday;

  Market mkt = Market(valueDate);
  /*
  load data from file and update market object with data
  */


// Read rates and interpolate
  RateCurve curve;
    vector<string> tenors;
    vector<double> rates;

    // Assuming your file is named "curve.txt"
    readRatesFromFile("curve.txt", tenors, rates);

    // Display the contents of tenors and rates
    for (size_t i = 0; i < tenors.size(); ++i) {
        curve.addRate(tenors[i], rates[i]);
    }
  
  curve.display();

  // Get rate for a specific tenor (example)
    //std::cout << "Rate at 4M: " << curve.getRate("4M") << std::endl;



  //task 2, create a portfolio of bond, swap, european option, american option
  //for each time, at least should have long / short, different tenor or expiry, different underlying
  //totally no less than 16 trades
  vector<Trade*> myPortfolio;

  //Bonds
  Trade* bond = new Bond(Date(2024, 1, 1), Date(2034, 1, 1), 10000000, 103.5);
  myPortfolio.push_back(bond);


  // Example: Adding a dummy Swap trade to the portfolio
  // Date startDate = Date(2023, 1, 1); // Example start date
  // Date endDate = Date(2025, 1, 1);   // Example end date
  // double notional = 100;
  // double tradeRate = 0.05;  // 5% trade rate
  // double frequency = 1;     // Annual payments
  // Swap* swapTrade = new Swap(startDate, endDate, notional, tradeRate, tradeRate, frequency);
  // myPortfolio.push_back(swapTrade);

  //task 3, create a pricer and price the portfolio, output the pricing result of each deal.
  // Create a CRRBinomialTreePricer with 10 time steps
  Pricer* treePricer = new CRRBinomialTreePricer(10);
  std::ofstream logFile("pricing_log.txt");

  // Iterate through the portfolio and price each trade
  for (auto trade : myPortfolio) {
      double pv = treePricer->Price(mkt, trade);
      logFile << "Trade with expiry: " << trade->GetExpiry() << " has PV: " << pv << std::endl;
  }

  logFile.close();

  // Clean up
  delete treePricer;
  for (auto trade : myPortfolio) {
      delete trade;
  }




  //task 4, analyzing pricing result
  // a) compare CRR binomial tree result for an european option vs Black model
  // b) compare CRR binomial tree result for an american option vs european option

  //final
  cout << "Project build successfully!" << endl;
  return 0;

}
