//
// Created by RAhR RAh on 4/3/20.
//

#include "OrderBook.h"
#include <map>
#include <iostream>
using namespace std;
//should rewrite using pointers, but don't know how generic pointer works
vector<Deal> OrderBook::take_lmt(LimitOrder *lmtorder)
{
    vector<Deal> total_deal;
    if(lmtorder->buy){
        //first check unmatched sell orders
        if (not unmatched_mkt_sell.empty())
        {
            for (auto i = unmatched_mkt_sell.begin(); 
                i != unmatched_mkt_sell.end() and lmtorder->volume !=0; ++i)
            {
                Deal deal;
                int matched_volume = min(lmtorder->volume, (*i)->volume);

                deal.maker_id = lmtorder->id;
                deal.taker_id = (*i)->id;
                deal.price = lmtorder->price; 
                deal.volume = matched_volume;

                lmtorder->volume -= matched_volume;
                (*i)->volume -= matched_volume;
                if ((*i)->volume == 0){unmatched_mkt_sell.pop_front();};
                total_deal.push_back(deal);
            }
        }    
        //check limited sell orders
        for (auto i = asks.begin();
            i != asks.end() && i->first <= lmtorder->price && lmtorder->volume !=0; ++i)
            {
                for (auto j = i->second.begin();
                     j != i->second.end() && lmtorder->volume != 0; ++j)
                {
                    Deal deal;
                    int matched_volume = min(lmtorder->volume, (*j)->volume);

                    deal.taker_id = lmtorder->id;
                    deal.maker_id = (*j)->id;
                    deal.price = lmtorder->price;
                    deal.volume = matched_volume;

                    lmtorder->volume -= matched_volume;
                    (*j)->volume -= matched_volume;
                    if ((*j)->volume == 0){i->second.pop_front();};
                    total_deal.push_back(deal);
                }

                if (i->second.empty()){asks.erase(i);}
                
            }

            // add remaining order to the book
        bids[lmtorder->price].push_back(lmtorder);

    }else{
        if (not unmatched_mkt_buy.empty())
        {
            for (auto i = unmatched_mkt_buy.begin();
                 i != unmatched_mkt_buy.end() and lmtorder->volume != 0; ++i)
            {
                Deal deal;
                int matched_volume = min(lmtorder->volume, (*i)->volume);

                deal.maker_id = lmtorder->id;
                deal.taker_id = (*i)->id;
                deal.price = lmtorder->price;
                deal.volume = matched_volume;

                lmtorder -= matched_volume;
                (*i)->volume -= matched_volume;
                if ((*i)->volume == 0)
                {
                    unmatched_mkt_buy.pop_front();
                };
                total_deal.push_back(deal);
            }
        }
        //check limited sell orders
        for (auto i = bids.begin();
             i != bids.end() && i->first <= lmtorder->price && lmtorder->volume != 0; ++i)
        {
            for (auto j = i->second.begin();
                 j != i->second.end() && lmtorder->volume != 0; ++j)
            {
                Deal deal;
                int matched_volume = min(lmtorder->volume, (*j)->volume);

                deal.taker_id = lmtorder->id;
                deal.maker_id = (*j)->id;
                deal.price = lmtorder->price;
                deal.volume = matched_volume;

                lmtorder->volume -= matched_volume;
                (*j)->volume -= matched_volume;
                if ((*j)->volume == 0){i->second.pop_front();}
                total_deal.push_back(deal);
            }

            if (i->second.empty()){bids.erase(i);}
        }

        // add remaining order to the book
        asks[lmtorder->price].push_back(lmtorder);
    };
    return total_deal;
};

//should rewrite using pointers, but don't know how generic pointer works
vector<Deal> OrderBook::take_mkt(MarketOrder *mktorder){
    vector<Deal> total_deal;
    if (mktorder->buy)
    {
        for (auto i = asks.begin(); 
            i != asks.end() && mktorder->volume!=0; i++)
        {
            for (auto j = i->second.begin();
                 j != i->second.end() && mktorder->volume != 0; ++j)
            {
                Deal deal;
                int matched_volume = min(mktorder->volume, (*j)->volume);

                deal.taker_id = mktorder->id;
                deal.maker_id = (*j)->id;
                deal.price = (*j)->price;
                deal.volume = matched_volume;

                mktorder->volume -= matched_volume;
                (*j)->volume -= matched_volume;
                if ((*j)->volume == 0){i->second.pop_front();}
                total_deal.push_back(deal);
            }

            if (i->second.empty()){asks.erase(i);}
            
        }
        
        if (mktorder->volume != 0){unmatched_mkt_buy.push_back(mktorder);}
        
    }else{
        for (auto i = bids.begin();
             i != bids.end() && mktorder->volume != 0; ++i)
        {
            for (auto j = i->second.begin();
                 j != i->second.end() && mktorder->volume != 0; ++j)
            {
                Deal deal;
                int matched_volume = min(mktorder->volume, (*j)->volume);

                deal.taker_id = mktorder->id;
                deal.maker_id = (*j)->id;
                deal.price = (*j)->price;
                deal.volume = matched_volume;

                mktorder->volume -= matched_volume;
                (*j)->volume -= matched_volume;
                if ((*j)->volume == 0){i->second.pop_front();};
                total_deal.push_back(deal);
            }

            if (i->second.empty()){bids.erase(i);}
        }

        if (mktorder->volume != 0){unmatched_mkt_sell.push_back(mktorder);}
    }
    return total_deal;
};