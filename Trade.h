#pragma once
#include<string>
#include "Date.h"

using namespace std;

class Trade {
public:
    Trade(){};
    Trade(const string& _type,Date _tradeDate): tradeType(_type), tradeDate(_tradeDate) {};
    inline string getType(){ return tradeType; };
    virtual double Payoff(double marketPrice) const = 0;
    virtual const Date& GetExpiry() const 
    { 
    return expiryDate; //tft: maybe it's expiry instead? kiv
    }
    virtual void Print() const = 0;
    // virtual void Print() const {
    //     cout << "Trade Type: " << tradeType << endl;
    //     cout << "Trade Date: " << tradeDate << endl;
    //     cout << "Expiry Date: " << expiryDate << endl;
    // }


    //virtual Date GetExpiry() const = 0; //Added new
    virtual ~Trade(){};
    

protected:   
    string tradeType;
    Date tradeDate;
    Date expiryDate;
};