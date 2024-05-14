#include "Swap.h"

double Swap::Payoff(double marketRate) const {
    double annuity = getAnnuity(marketRate);
    return annuity * (tradeRate - marketRate);
}

double Swap::getAnnuity(double marketRate) const {
    double annuity = 0.0;
    double period = 1.0 / frequency;
    Date currentDate = startDate;
    double totalYears = maturityDate - startDate;
    int numPeriods = static_cast<int>(totalYears * frequency);

    for (int i = 0; i < numPeriods; ++i) {
        double time = (currentDate - startDate) + period;
        double discountFactor = exp(-marketRate * time);
        annuity += notional * discountFactor;
        // Advance currentDate by the period length
        int daysToAdd = static_cast<int>(365.0 / frequency);
        currentDate.day += daysToAdd;
        if (currentDate.day > 30) { // Simplified month-end adjustment
            currentDate.month += currentDate.day / 30;
            currentDate.day %= 30;
        }
        if (currentDate.month > 12) {
            currentDate.year += currentDate.month / 12;
            currentDate.month %= 12;
        }
    }

    return annuity;
}