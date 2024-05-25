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

/*
for above, there is no need to put all header file here right? 
only need to put ones you need, e.g., market.h contains date.h and spline.h, 
so need to be incldued here again
*/

using namespace std;

int main()
{
  // task 1, create an market data object, and update the market data from from txt file
  // 1.1 date
  std::cout << "\nTask1:\n\n";
  std::time_t t = std::time(0);
  auto now_ = std::localtime(&t);
  Date valueDate;
  valueDate.year = now_->tm_year + 1900;
  valueDate.month = now_->tm_mon + 1;
  valueDate.day = now_->tm_mday;
  // cout << "Year Type:" << valueDate.year << endl;
  // cout << "Month Type:" << valueDate.month << endl;
  // cout << "Date Type:" << valueDate.day << endl;
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
  std::cout << "\nEnd of Task 1\n\n"
            << std::endl;

  // task 2, create a portfolio of bond, swap, european option, american option
  // for each time, at least should have long / short, different tenor or expiry, different underlying
  // totally no less than 16 trades
  std::cout << "Task 2:\n\n";
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
  Trade *europeanCall2 = new EuropeanOption(Date(2025, 11, 19), 110.0, BinaryCall);
  Trade *europeanPut2 = new EuropeanOption(Date(2026, 11, 19), 95.0, BinaryPut);
  myPortfolio.push_back(europeanCall1);
  myPortfolio.push_back(europeanPut1);
  myPortfolio.push_back(europeanCall2);
  myPortfolio.push_back(europeanPut2);

  // Adding American options
  Trade *americanCall1 = new AmericanOption(Date(2025, 5, 19), 110.0, Call);
  Trade *americanPut1 = new AmericanOption(Date(2026, 5, 19), 95.0, Put);
  Trade *americanCall2 = new AmericanOption(Date(2025, 11, 19), 115.0, BinaryCall);
  Trade *americanPut2 = new AmericanOption(Date(2026, 11, 19), 90.0, BinaryPut);
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
std::cout << "\nEnd of Task 2\n\n";
/*
when you use new, should use delete.
*/

// task 3, create a pricer and price the portfolio, output the pricing result of each deal.

std::cout << "\nTask 3:\n\n";
// Calculate prices for each trade
double nTimeSteps = 10;
double stockPrice = 100; // Get stock price from market object
double vol = 0.2; // Get volatility from market object
double rate = 0.03; // Get interest rate from market object

for (const auto &trade : myPortfolio) {
    if (AmericanOption* americanOption = dynamic_cast<AmericanOption*>(trade)) {
        OptionType optType = americanOption->getPayoffType();
        // Pricing logic for American option
        CRRBinomialTreePricer crrPricer(nTimeSteps);
        double crrPrice = crrPricer.PriceTree(mkt, *americanOption, stockPrice, vol, rate);
        // cout << "Market Type:" << mkt << endl;
        // cout << "Option Type:" << optType<< endl;
        cout << "CRR Binomial Tree Price for American Option: " << crrPrice << endl;
    }
    
    else if (EuropeanOption* europeanOption = dynamic_cast<EuropeanOption*>(trade)) {
        OptionType optType = europeanOption->getPayoffType();
        // Pricing logic for European option
        CRRBinomialTreePricer crrPricer(nTimeSteps);
        double crrPrice = crrPricer.PriceTree(mkt, *europeanOption, stockPrice, vol, rate);
        cout << "CRR Binomial Tree Price for European Option: " << crrPrice << endl;
    }

    else if (Swap* swap = dynamic_cast<Swap*>(trade)) {
        double annuity = swap->getAnnuity(curve);
        // Calculate payoff using the annuity and market rate
        double payoff = swap->Payoff(annuity);
        //cout << "Annunity for Swap: " << annuity << endl;
        cout << "Payoff for Swap: " << payoff << endl;
    }

}
std::cout << "\nEnd of Task 3\n\n";


  // task 3, create a pricer and price the portfolio, output the pricing result of each deal.
  //  Create a CRRBinomialTreePricer with 10 time steps
    // Pricer* treePricer = new CRRBinomialTreePricer(10);
    // std::ofstream logFile("pricing_log.txt");

    // // Iterate through the portfolio and price each trade
    // for (auto trade : myPortfolio) {
    //     double pv = treePricer->Price(mkt, trade);
    //     logFile << "Trade with expiry: " << trade->GetExpiry() << " has PV: " << pv << std::endl;
    // }

    // logFile.close();

    // // Clean up
    // delete treePricer;
    // for (auto trade : myPortfolio) {
    //     delete trade;
    // }


  // task 4, analyzing pricing result
  //  a) compare CRR binomial tree result for an european option vs Black model
  //  b) compare CRR binomial tree result for an american option vs european option

std::cout << "\nTask 4" << std::endl;


std::cout << "\nTask 4a: Compare CRR binomial tree result for an European option vs Black model" << std::endl;
vector<Trade *> myPortfolio3;

vector<double> europeanPrices1;
vector<double> BlackModelPrices;
BlackPricer blackPricer;

  // Adding European options
  Trade *europeanCall31 = new EuropeanOption(Date(2025, 5, 19), 105.0, Call);
  Trade *europeanPut31 = new EuropeanOption(Date(2026, 6, 20), 100.0, Put);
  Trade *europeanCall41 = new EuropeanOption(Date(2027, 7, 21), 110.0, BinaryCall);
  Trade *europeanPut41 = new EuropeanOption(Date(2028, 8, 22), 95.0, BinaryPut);
  myPortfolio3.push_back(europeanCall31);
  myPortfolio3.push_back(europeanPut31);
  myPortfolio3.push_back(europeanCall41);
  myPortfolio3.push_back(europeanPut41);


for (const auto &trade : myPortfolio3) {
    if (EuropeanOption* europeanOption = dynamic_cast<EuropeanOption*>(trade)) {
        OptionType optType = europeanOption->getPayoffType();
        Date expiry = europeanOption->GetExpiry();
        double strike = europeanOption->getStrike();
        //cout << "Payoff: " << optType << "Expiry: " << expiry << " vs " << "Strike: " << strike << endl;

        // // Pricing logic for European option
        CRRBinomialTreePricer crrPricer1(nTimeSteps);
        double europeanPrice = crrPricer1.PriceTree(mkt, *europeanOption, stockPrice, vol, rate);
        EuropeanOption option(expiry, strike, optType);
        double Blackprice = blackPricer.Price(mkt, &option, stockPrice, rate, vol, strike, optType);
        europeanPrices1.push_back(europeanPrice); // Store European option price
        BlackModelPrices.push_back(Blackprice); // Store European option price


    }
}

// Print comparison
for (size_t i = 0; i < europeanPrices1.size(); ++i) {
    cout << "CRR Binomial Tree Price for European Option: " << europeanPrices1[i] << " vs ";
    cout << "Black Model: " << BlackModelPrices[i] << endl;
}


std::cout << "\nTask 4b: Comparison CRR binomial tree result for an European option vs American option " << std::endl;

vector<Trade *> myPortfolio2;

  // Adding European options
  Trade *europeanCall3 = new EuropeanOption(Date(2025, 5, 19), 105.0, Call);
  Trade *europeanPut3 = new EuropeanOption(Date(2026, 6, 20), 100.0, Put);
  Trade *europeanCall4 = new EuropeanOption(Date(2027, 7, 21), 110.0, BinaryCall);
  Trade *europeanPut4 = new EuropeanOption(Date(2028, 8, 22), 95.0, BinaryPut);
  myPortfolio2.push_back(europeanCall3);
  myPortfolio2.push_back(europeanPut3);
  myPortfolio2.push_back(europeanCall4);
  myPortfolio2.push_back(europeanPut4);

  // Adding American options
  Trade *americanCall3 = new AmericanOption(Date(2025, 5, 19), 105.0, Call);
  Trade *americanPut3 = new AmericanOption(Date(2026, 6, 20), 100.0, Put);
  Trade *americanCall4 = new AmericanOption(Date(2027, 7, 21), 110.0, BinaryCall);
  Trade *americanPut4 = new AmericanOption(Date(2028, 8, 22), 95.0, BinaryPut);
  myPortfolio2.push_back(americanCall3);
  myPortfolio2.push_back(americanPut3);
  myPortfolio2.push_back(americanCall4);
  myPortfolio2.push_back(americanPut4);


vector<double> europeanPrices;
vector<double> americanPrices;

for (const auto &trade : myPortfolio2) {
    if (EuropeanOption* europeanOption = dynamic_cast<EuropeanOption*>(trade)) {
        OptionType optType = europeanOption->getPayoffType();
        // Pricing logic for European option
        CRRBinomialTreePricer crrPricer1(nTimeSteps);
        double europeanPrice = crrPricer1.PriceTree(mkt, *europeanOption, stockPrice, vol, rate);
        europeanPrices.push_back(europeanPrice); // Store European option price
    } else if (AmericanOption* americanOption = dynamic_cast<AmericanOption*>(trade)) {
        OptionType optType = americanOption->getPayoffType();
        // Pricing logic for American option
        CRRBinomialTreePricer crrPricer(nTimeSteps);
        double americanPrice = crrPricer.PriceTree(mkt, *americanOption, stockPrice, vol, rate);
        americanPrices.push_back(americanPrice); // Store American option price
    }
}

// Print comparison
for (size_t i = 0; i < europeanPrices.size(); ++i) {
    cout << "CRR Binomial Tree Price for European Option: " << europeanPrices[i] << " vs ";
    cout << "CRR Binomial Tree Price for American Option: " << americanPrices[i] << endl;
}

cout << "\nEnd of Task 4\n\n" << endl;


  // final
  cout << "\nProject build successfully!" << endl;
  return 0;
}
