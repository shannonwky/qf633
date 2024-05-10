#include <ctime>
#include <fstream>
#include "Market.h"

int main()
{
    if(0){
        // current time
        std::time_t t = std::time(0);
        auto now_ = std::localtime(&t);

        Date valueDate;
        valueDate.year = now_->tm_year + 1900;
        valueDate.month = now_->tm_mon + 1;
        valueDate.day = now_->tm_mday;

        Market mkt = Market(valueDate);

        mkt.Print();
    }

    if(0){
        std::ofstream fout("simpleMkt.dat");
        if(fout){
            std::time_t t = std::time(0);
            auto now_ = std::localtime(&t);
            Date date = Date(now_->tm_year + 1900,now_->tm_mon + 1,now_->tm_mday);
            Market mkt = Market(date);

            fout << mkt;
        }else{
            std::cerr << "something went worng!" << std::endl;
        }
    }

    if(1){
        std::ifstream fin("simpleMkt.dat");
        if(fin){
            Market mkt;
            fin >> mkt;
            mkt.Print();
        }else{
            std::cerr << "something went worng!" << std::endl;
        }
    }
    

    return 0;
}