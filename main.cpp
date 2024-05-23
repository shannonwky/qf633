#include <fstream>
#include <ctime>
#include <chrono>
#include <iostream>

#include "Date.h"
#include "Payoff.h"
#include "Market.h"
#include "Pricer.h"
#include "BlackModelPricer.h"
#include "EuropeanTrade.h"
#include "AmericanTrade.h"
#include "Bond.h"
#include "Swap.h"
#include "Trade.h"
#include "TreeProduct.h"
#include "Types.h"
#include "FileUtils.h"

using namespace std;

int main()
{
  // task 1, create an market data object, and update the market data from from txt file
  // 1.1 date
  std::cout << "task1:\n";
  std::time_t t = std::time(0);
  auto now_ = std::localtime(&t);
  Date valueDate;
  valueDate.year = now_->tm_year + 1900;
  valueDate.month = now_->tm_mon + 1;
  valueDate.year = now_->tm_mday;
  Market mkt = Market(valueDate);

  // 1.2 ratecurve
  // Read rates and interpolate
  RateCurve curve;
  vector<string> tenors;
  vector<double> rates;
  // Assuming your file is named "curve.txt"
  readRatesFromFile("curve.txt", tenors, rates, true);
  // Display the contents of tenors and rates
  for (size_t i = 0; i < tenors.size(); ++i)
  {
    curve.addRate(tenors[i], rates[i]);
  }
  mkt.addCurve("USD-SOFR", curve);
  // curve.display();
  // Get rate for a specific tenor (example)
  // string tenor_check = "3.3Y";
  // double obtained_rate = curve.getRate(tenor_check);
  // std::cout << "Interpolated value at x=" << tenor_check << " is " << obtained_rate << std::endl;

  // 1.3 volcurve
  VolCurve volCurve("USD-ATM");
  readVolCurveFromFile("vol.txt", volCurve);
  mkt.addVolCurve("USD-ATM", volCurve);
  // volCurve.display();
  // Date tenorToCheck(2025, 1, 1); // Example date
  // double days = tenorToCheck - mkt.asOf;
  // Date tenor = Date::toDate(days);
  // double vol = volCurve.getVol(tenor);
  // std::cout << "Interpolated volatility for tenor " << tenorToCheck << " is " << vol << "%" << std::endl;
  // mkt.addVolCurve("USD-ATM", volCurve1);

  // 1.4 bond
  std::unordered_map<std::string, double> bondPrices;
  readBondPricesFromFile("bondPrices.txt", bondPrices);
  for (const auto &pair : bondPrices)
  {
    mkt.addBondPrice(pair.first, pair.second);
  }

  // 1.5 stock
  std::unordered_map<std::string, double> stockPrices;
  readStockPricesFromFile("stockPrices.txt", stockPrices);
  for (const auto &pair : stockPrices)
  {
    mkt.addStockPrice(pair.first, pair.second);
  }

  mkt.Print();
  std::cout << "\n\n"
            << std::endl;

  // task 2, create a portfolio of bond, swap, european option, american option
  // for each time, at least should have long / short, different tenor or expiry, different underlying
  // totally no less than 16 trades
  std::cout << "task2:\n";
  vector<Trade *> myPortfolio;

  // Bonds
  // Adding bonds
  Trade *bond1 = new Bond(Date(2024, 1, 1), Date(2034, 1, 1), 10000000, 103.5);
  Trade *bond2 = new Bond(Date(2024, 1, 1), Date(2029, 1, 1), 5000000, 99.0);
  Trade *bond3 = new Bond(Date(2024, 1, 1), Date(2030, 1, 1), 15000000, 101.0);
  Trade *bond4 = new Bond(Date(2024, 1, 1), Date(2027, 1, 1), 7500000, 98.0);
  myPortfolio.push_back(bond1);
  myPortfolio.push_back(bond2);
  myPortfolio.push_back(bond3);
  myPortfolio.push_back(bond4);

  // Swaps
  // Adding swaps
  Trade *swap1 = new Swap(Date(2023, 1, 1), Date(2025, 1, 1), 1000000, 0.05, 0.04, 1);
  Trade *swap2 = new Swap(Date(2023, 1, 1), Date(2026, 1, 1), 2000000, 0.04, 0.03, 2);
  Trade *swap3 = new Swap(Date(2023, 1, 1), Date(2024, 1, 1), 3000000, 0.06, 0.05, 1);
  Trade *swap4 = new Swap(Date(2023, 1, 1), Date(2027, 1, 1), 4000000, 0.03, 0.02, 2);
  myPortfolio.push_back(swap1);
  myPortfolio.push_back(swap2);
  myPortfolio.push_back(swap3);
  myPortfolio.push_back(swap4);

  // Adding European options
  Trade *europeanCall1 = new EuropeanOption(Date(2025, 5, 19), 105.0, Call);
  Trade *europeanPut1 = new EuropeanOption(Date(2026, 5, 19), 100.0, Put);
  Trade *europeanCall2 = new EuropeanOption(Date(2025, 11, 19), 110.0, Call);
  Trade *europeanPut2 = new EuropeanOption(Date(2026, 11, 19), 95.0, Put);
  myPortfolio.push_back(europeanCall1);
  myPortfolio.push_back(europeanPut1);
  myPortfolio.push_back(europeanCall2);
  myPortfolio.push_back(europeanPut2);

  // Adding American options
  Trade *americanCall1 = new AmericanOption(Date(2025, 5, 19), 110.0, Call);
  Trade *americanPut1 = new AmericanOption(Date(2026, 5, 19), 95.0, Put);
  Trade *americanCall2 = new AmericanOption(Date(2025, 11, 19), 115.0, Call);
  Trade *americanPut2 = new AmericanOption(Date(2026, 11, 19), 90.0, Put);
  myPortfolio.push_back(americanCall1);
  myPortfolio.push_back(americanPut1);
  myPortfolio.push_back(americanCall2);
  myPortfolio.push_back(americanPut2);

  // Printing the portfolio
  std::cout << "Portfolio created with " << myPortfolio.size() << " trades.\n";
  for (const auto &trade : myPortfolio)
  {
    trade->Print();
  }

  // std::cout << "\nThis is the beginning of Swap section: " << std::endl;
  // // Define the start and end dates for the swap
  // Date startDate(2023, 1, 1);
  // Date endDate(2025, 1, 1);

  // // Define the swap parameters
  // double notional = 1000000; // 1,000,000 units
  // double tradeRate = 0.05;   // 5% trade rate
  // double frequency = 1;      // Annual payments (1), semi-annual (2)...
  // double marketRate = 0.04;  // 4%

  // // Create the Swap object
  // Swap mySwap(startDate, endDate, notional, tradeRate, marketRate, frequency);

  // // Calculate the payoff of the swap
  // double annuity = mySwap.getAnnuity(curve);     // get annuity
  // double present_value = mySwap.Payoff(annuity); // get present value

  // // Output the results
  // std::cout << "End of Swap section\n"
  //           << std::endl;

  // myPortfolio.push_back(swapTrade);

  // task 3, create a pricer and price the portfolio, output the pricing result of each deal.
  //  Create a CRRBinomialTreePricer with 10 time steps
  //   Pricer* treePricer = new CRRBinomialTreePricer(10);
  //   std::ofstream logFile("pricing_log.txt");

  //   // Iterate through the portfolio and price each trade
  //   for (auto trade : myPortfolio) {
  //       double pv = treePricer->Price(mkt, trade);
  //       logFile << "Trade with expiry: " << trade->GetExpiry() << " has PV: " << pv << std::endl;
  //   }

  //   logFile.close();

  //   // Clean up
  //   delete treePricer;
  //   for (auto trade : myPortfolio) {
  //       delete trade;
  //   }

  // task 4, analyzing pricing result
  //  a) compare CRR binomial tree result for an european option vs Black model
  //  b) compare CRR binomial tree result for an american option vs european option

  // Setup Market Data
  Date asOf(2024, 5, 19);
  Market market(asOf);

  Date expiry(2025, 5, 19);
  double strike = 105.0;
  int nTimeSteps = 2;

  std::cout << "\nTask 4" << std::endl;

  // European fully working
  OptionType optType = Call;
  EuropeanOption europeanOption(expiry, strike, optType);
  CRRBinomialTreePricer crrPricer(nTimeSteps);
  double crrPrice = crrPricer.PriceTree(market, europeanOption);
  cout << "CRR Binomial Tree Price for European Call: " << crrPrice << endl;
  ////End of European

  // American fully working
  OptionType optType2 = Put;
  AmericanOption americanOption(expiry, strike, optType2);
  CRRBinomialTreePricer crrPricer2(nTimeSteps);
  double crrPrice2 = crrPricer2.PriceTree(market, americanOption);
  cout << "CRR Binomial Tree Price for American Put: " << crrPrice2 << endl;
  ////End of American

  // Black Model fully working
  //  Change of parameters in Pricer.cpp
  EuropeanOption option(expiry, 105, Put);
  BlackPricer pricer;
  double price = pricer.Price(market, &option);
  std::cout << "Black Model Option price: " << price << std::endl;

  // final
  cout << "\nProject build successfully!" << endl;
  return 0;
}
