#include <fstream>
#include <ctime>
#include <chrono>

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




using namespace std;

void readFromFile(const string& fileName, string& outPut){
  string lineText;
  ifstream MyReadFile(fileName);
  while (getline (MyReadFile, lineText)) {
    outPut.append(lineText);
  }
  MyReadFile.close();
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

    RateCurve curve;

  // Add rates (example)
  curve.addRate("0M", 5.56);
  curve.addRate("3M", 5.5);
  curve.addRate("6M", 5.45);
  curve.addRate("9M", 5.4);
  curve.addRate("1Y", 5.53);
  curve.addRate("2Y", 5.34);
  curve.addRate("5Y", 4.75);
  curve.addRate("10Y", 3.9);
  //curve.addRate("4M",curve.getRate("4M"));

  // Display the rate curve
  curve.display();

  // Get rate for a specific tenor (example)
  std::cout << "Rate at 4M: " << curve.getRate("4M") << std::endl;



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
