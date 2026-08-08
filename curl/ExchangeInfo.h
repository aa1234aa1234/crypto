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

    static void from_json(const nlohmann::json& object, price_info& info)
    {
        info.stockcode = object["stk_cd"].get<std::string>();
        info.name = object["stk_nm"].get<std::string>();
        info.current_price = object["cur_prc"].get<double>();
        info.close_price = object["close_pric"].get<double>();
        info.low_price = object["low_pric"].get<double>();
        info.high_price = object["high_pric"].get<double>();
        info.open_price = object["open_pric"].get<double>();
        info.volume = object["trde_qty"].get<long>();
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
