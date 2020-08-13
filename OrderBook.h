//
// Created by RAhR RAh on 4/3/20.
//

#ifndef ORDERBOOK_ORDERBOOK_H
#define ORDERBOOK_ORDERBOOK_H
#include <map>
#include <deque>
#include <vector>
#include "Random_gen.h"
using namespace std;

struct MarketOrder
{
    bool buy; //true for buy order
    int volume;
    int id;
};

struct LimitOrder
{
    bool buy; //true for buy order
    float price;
    int volume;
    int id;
};

struct Deal
{
    float price;
    int volume;
    int maker_id;
    int taker_id;
    ostream &operator<<(ostream &os);
};

class OrderBook
{
private:

    map<float, deque<LimitOrder* > > asks;
    map<float, deque<LimitOrder*>, greater<float> > bids;
    deque<MarketOrder*> unmatched_mkt_sell; //unmatched market order
    deque<MarketOrder*> unmatched_mkt_buy;
    Random_gen Order_generator;
    int trader_id[] = {0,1,2,3,4,5,6,7,8,9,10};

public:
    ~OrderBook(){};
    
    vector<Deal> take_mkt(MarketOrder* mktorder);
    vector<Deal> take_lmt(LimitOrder *lmtorder);
    void withdraw_lmtorder(LimitOrder* lmtorder);
    void withdraw_mktorder(MarketOrder* lmtorder);
    ostream &operator<<(ostream &os);
};

#endif //ORDERBOOK_ORDERBOOK_H