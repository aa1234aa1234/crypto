//
// Created by sw_303 on 2026-08-08.
//

#ifndef EXCHANGEINFO_H
#define EXCHANGEINFO_H
#include <string>

#include "Candle.h"
#include "json.hpp"

struct price_info
{
    std::string stockcode;
    std::string name;
    double current_price;
    double close_price;
    double low_price;
    double high_price;
    double open_price;
    long volume;

    static void from_json(const nlohmann::json& object, price_info& info, std::string type="dom")
    {
        //auto a = std::atof(object["atn_stk_infr"][0]["cur_prc"].get<std::string>().c_str());
        info.stockcode = object["stk_cd"].get<std::string>();
        info.name = object["stk_nm"].get<std::string>();
        info.current_price = std::atof(object["cur_prc"].get<std::string>().c_str());
        if (type == "dom")
        {
            info.close_price = std::atof(object["close_pric"].get<std::string>().c_str());
            info.volume = std::atol(object["trde_qty"].get<std::string>().c_str());
        }
        else
        {
            info.close_price = std::atof(object["cur_prc"].get<std::string>().c_str());
            info.volume = std::atol(object["stk_cnt"].get<std::string>().c_str());
        }
        info.low_price = std::atof(object["low_pric"].get<std::string>().c_str());
        info.high_price = std::atof(object["high_pric"].get<std::string>().c_str());
        info.open_price = std::atof(object["open_pric"].get<std::string>().c_str());
    }

    nlohmann::json to_json()
    {
        nlohmann::json object;
        object["stk_cd"] = stockcode;
        object["stk_nm"] = name;
        object["cur_prc"] = current_price;
        object["close_pric"] = close_price;
        object["low_pric"] = low_price;
        object["high_pric"] = high_price;
        object["open_pric"] = open_price;
        object["trde_qty"] = volume;
        return object;
    }

    Candle to_candle()
    {
        Candle candle{"", close_price, high_price, low_price, open_price, volume};
        return candle;
    }
};

struct token_info
{
    std::string token;

    static void from_json(const nlohmann::json& object, token_info& info)
    {
        info.token = object["token"].get<std::string>();
    }

    nlohmann::json to_json()
    {
        nlohmann::json object;
        object["token"] = token;
        return object;
    }
};

#endif //EXCHANGEINFO_H
