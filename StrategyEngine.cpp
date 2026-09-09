//
// Created by sw_303 on 2026-08-06.
//

#include "StrategyEngine.h"
#include "IndicatorsEngine.h"
#include "indicators.hpp"
#include <iostream>
#include "string_format.h"

StrategyEngine::~StrategyEngine()
{
}

static bool is_price_near(double price, indicators::Indicator* indicator, double atr14)
{
    const double DISTANCE_THRESHOLD = 1.0;
    double distance = std::abs(price-indicator->getValue()) / atr14;
    return distance<=DISTANCE_THRESHOLD;
}

static void add_trendscore(double& trendscore, double score)
{
    trendscore += score;
}

template<typename T>
static T get_prev(std::vector<T>& v, int idx)
{
    if (idx >= v.size()) return -1;
    return v[v.size() - 1 - idx];
}

void StrategyEngine::update_regime(const Candle& candle)
{
    static std::vector<double> trendscore_hist;
    auto sma200 = indicatorsengine->getIndicator<indicators::SMA>({200});
    auto sma50 = indicatorsengine->getIndicator<indicators::SMA>({50});
    auto ema50 = indicatorsengine->getIndicator<indicators::EMA>({50});
    auto ema21 = indicatorsengine->getIndicator<indicators::EMA>({21});
    auto ema9 = indicatorsengine->getIndicator<indicators::EMA>({9});
    auto atr14 = indicatorsengine->getIndicator<indicators::ATR>({14});

    double sma200slope = ((sma200->getValue() - sma200->previous(20))/sma200->previous(20));
    double sma50slope = ((sma50->getValue() - sma50->previous(20))/atr14->getValue());
    double ema50slope = ((ema50->getValue() - ema50->previous(20))/atr14->getValue());
    double ema21slope = ((ema21->getValue() - ema21->previous(20))/atr14->getValue());

    static int uptrend_confirm = 0, downtrend_confirm = 0;

    trendscore = 0.0;


    if (ema9->getValue() > ema21->getValue())
    {
        //trendscore += 0.20;
        trendscore += 0.20 * std::clamp((ema9->getValue()-ema21->getValue())/atr14->getValue(), -1.0, 1.0);
        backtest += "ema9 > ema21 trendscore + 20\n";
    }
    else
    {
        //trendscore -= 0.20;
        trendscore += 0.20 * std::clamp((ema9->getValue()-ema21->getValue())/atr14->getValue(), -1.0, 1.0);
        backtest += "ema9 <= ema21 trendscore - 20\n";
    }

    if (ema21->getValue() > ema50->getValue())
    {
        //trendscore += 0.250;
        trendscore += 0.20 * std::clamp((ema21->getValue()-ema50->getValue())/atr14->getValue(), -1.0, 1.0);
        backtest += "ema21 > ema50 trendscore + 25\n";
    }
    else
    {
        //trendscore -= 0.250;
        trendscore += 0.20 * std::clamp((ema21->getValue()-ema50->getValue())/atr14->getValue(), -1.0, 1.0);
        backtest += "ema21 <= ema50 trendscore - 25\n";
    }

    if (ema21slope > 0.6)
    {
        trendscore += 0.20;
        backtest += "ema21 slope > 0.08 trendscore + 20\n";
    }
    else if (ema21slope < -0.6)
    {
        trendscore -= 0.20;
        backtest += "ema21 slope <= 0.08 trendscore - 20\n";
    }

    if (ema50slope > 0.8)
    {
        trendscore += 0.20;
        backtest += "ema50 slope > 0.10 trendscore + 20\n";
    }
    else if (ema50slope < -0.80) {
        trendscore -= 0.20;
        backtest += "ema50 slope < -0.10 trendscore - 20\n";
    }

    trendscore += 0.10 * std::clamp((ema9->getValue()-candle.close)/candle.close, -1.0, 1.0);

    if (candle.close > sma200->getValue())
    {
        trendscore += 0.20;
        backtest += "candle > sma200 trendscore + 20\n";
    }
    else
    {
        trendscore -= 0.20;
        backtest += "candle <= sma200 trendscore - 10\n";
    }



    if (trendscore >= 0.60)
    {
        if (uptrend_confirm < 2) { uptrend_confirm++; downtrend_confirm=0; }
        else
        {
            market_state = UPTREND;
            if (trendscore <= 0.40 || std::abs(trendscore-get_prev<double>(trendscore_hist, 5))/get_prev<double>(trendscore_hist, 5) >= 0.8) market_state = SIDEWAYS;
            uptrend_confirm = 0;
        }
    }
    else if (trendscore <= -0.60)
    {
        if (downtrend_confirm < 2) { downtrend_confirm++; uptrend_confirm=0; }
        else
        {
            downtrend_confirm = 0;
            market_state = DOWNTREND;
            if (trendscore >= -0.40 || std::abs(trendscore-get_prev<double>(trendscore_hist, 10))/get_prev<double>(trendscore_hist, 10) >= 0.8) market_state = SIDEWAYS;
        }
    }
    else
    {
        market_state = SIDEWAYS;
        uptrend_confirm = 0;
        downtrend_confirm = 0;
    }

    trendscore_hist.push_back(trendscore);

    // if (candle.close > sma200->getValue() && sma50->getValue() > sma200->getValue() && ema50slope > 0.1)
    // {
    //     market_state = UPTREND;
    // }
    //
    // else if (candle.close < sma200->getValue() && sma50->getValue() < sma200->getValue() && ema50slope < -0.1)
    // {
    //     market_state = DOWNTREND;
    // }
    //
    // else market_state = SIDEWAYS;
}

void StrategyEngine::run(const Candle& candle)
{
	static int runcnt = 1;
    static double short_quantity = 0;
    static double long_entry = 0.0f;
    backtest = "";
    auto sma200 = indicatorsengine->getIndicator<indicators::SMA>({200});
    auto sma50 = indicatorsengine->getIndicator<indicators::SMA>({50});
    auto ema200 = indicatorsengine->getIndicator<indicators::EMA>({200});
    auto ema50 = indicatorsengine->getIndicator<indicators::EMA>({50});
    auto ema21 = indicatorsengine->getIndicator<indicators::EMA>({21});
    auto ema9 = indicatorsengine->getIndicator<indicators::EMA>({9});
    auto rsi14 = indicatorsengine->getIndicator<indicators::RSI>({14});
    auto atr14 = indicatorsengine->getIndicator<indicators::ATR>({14});
    auto lowrc = indicatorsengine->getIndicator<indicators::RC<LOW>>({});
    auto highrc = indicatorsengine->getIndicator<indicators::RC<HIGH>>({});

    update_regime(candle);

    double sma200slope = ((sma200->getValue() - sma200->previous(20))/sma200->previous(20));
    double sma50slope = ((sma50->getValue() - sma50->previous(20))/atr14->getValue());
    double ema50slope = ((ema50->getValue() - ema50->previous(20))/atr14->getValue());
    double ema21slope = ((ema21->getValue() - ema21->previous(20))/atr14->getValue());
    double candle_range = (highrc->getValue()-lowrc->getValue());

    //add a candle range to signal
    //update to atr later
    /*double range = candle.high - candle.low;
    double atr = ...;

    range < 2.0 * atr
    range/atr < candle.close*/

    int signal = false;
    bool strong_uptrend, pullback, bullish_reversal, momentum;

    switch (market_state)
    {
    case UPTREND:
        if (position == LONG && lowrc->getValue() <= long_entry)
        {
            position = FLAT;
            wallet += candle.close * asset, asset = 0;
            backtest += string_format("exited LONG position at %lf\n", candle.close);
            backtest += string_format("assets: %lf\nsold at: %lf\nwallet: %lf\n", asset, candle.close, wallet);
            break;
        }
        // strong_uptrend =candle.close > sma200->getValue() && ema21->getValue() > ema50->getValue() && ema50slope > 0.5;
        //
        // pullback = lowrc->getValue() <= ema50->getValue() + 0.5 * atr14->getValue();
        //
        // bullish_reversal =candle.close > candle.open && candle.close > ema50->getValue();
        //
        // momentum =rsi14->getValue() > 50 && ema9->getValue() > ema21->getValue();
        //
        // signal =(strong_uptrend && pullback && bullish_reversal && momentum);
        signal = (
            lowrc->getValue() <= ema50->getValue()*(1+0.01) &&
            is_price_near(candle.close, ema50, atr14->getValue()) &&
            (rsi14->getValue() >= 50 && (ema9->getValue()-ema21->getValue())/atr14->getValue() >= 0.05));
        if (signal && position == FLAT)
        {
            position = LONG;
            long_entry = candle.close;
            asset = static_cast<int>(wallet/candle.close), wallet -= asset*candle.close;
            backtest += string_format("entered LONG position at: %lf with %lf assets\n", candle.close, asset);
            backtest += string_format("assets: %lf\nbought at: %lf\nwallet: %lf\n", asset, candle.close, wallet);
        }
        break;
    case DOWNTREND:
        signal = (
        highrc->getValue() >= ema50->getValue() * (1-0.01) &&
        is_price_near(candle.close, ema50, atr14->getValue()) &&
        rsi14->getValue() <= 50
        );
        if (signal && position == LONG)
        {
            position = FLAT;
            wallet += candle.close * asset, asset = 0;
            backtest += string_format("exited LONG position at %lf\n", candle.close);
            backtest += string_format("assets: %lf\nsold at: %lf\nwallet: %lf\n", asset, candle.close, wallet);
        }
        if (position == FLAT && signal)
        {
            position = SHORT;
            short_quantity += static_cast<int>(wallet/candle.close);
            wallet += short_quantity * candle.close;
            backtest += string_format("entered SHORT position at: %lf with %lf assets\n", candle.close, short_quantity);
            backtest += string_format("short quantity: %lf\nbought at: %lf\nwallet: %lf\n", short_quantity, candle.close, wallet);
        }
        break;
    case SIDEWAYS:
        if (position == LONG)
        {
            backtest += string_format("exited LONG position at %lf\n", candle.close);
            wallet += asset * candle.close;
            backtest += string_format("assets: %lf\nsold at: %lf\nwallet: %lf\n", asset, candle.close, wallet);

            asset = 0;
        }
        else if (position == SHORT)
        {
            wallet -= short_quantity * candle.close;
            backtest += string_format("covered SHORT position at: %lf with %lf short quantity\n", candle.close, short_quantity);
            backtest += string_format("wallet: %lf\n", wallet);
            short_quantity = 0;
        }
        position = FLAT;
        break;
    }

    std::cout << position << std::endl;
    std::cout << "market state: " << market_state << std::endl;
	//if(position == LONG) asset = static_cast<int>(wallet/candle.close), wallet -= asset*candle.close;
	//if(position == SHORT) wallet += candle.close * asset, asset = 0;
	std::cout << "-----------------------" << std::endl << "backtest run " << runcnt << std::endl;
	std::cout << "position: " << position << std::endl;
	printf("sma200: %lf\nsma50: %lf\nema200: %lf\nema50: %lf\nrsi14: %lf\n", sma200->getValue(), sma50->getValue(), ema200->getValue(), ema50->getValue(), rsi14->getValue());
    backtest += string_format("close/sma200: %.4f\n"
    "sma50/sma200: %.4f\n"
    "sma200 slope: %.4f\n"
    "close/ema50 distance: %.4f\n"
    "low/ema50 distance: %.4f\n"
    "ema50 slope: %lf\n"
    "trendscore : %lf\n"
    "ema21 slope: %lf\n"
    "ema9: %lf\n"
    "atr14: %lf\n"
    "candle range: %.4f\n"
    "wallet: %lf\n",

    candle.close / sma200->getValue(),
    sma50->getValue() / sma200->getValue(),
    sma200slope,
    (candle.close - ema50->getValue()) / ema50->getValue(),
    (candle.low - ema50->getValue()) / ema50->getValue(),
    ema50slope,
    trendscore,
    ema21slope,
    ema9->getValue(),
    atr14->getValue(),
    candle_range,
    wallet);
    std::cout << backtest;
    //if (position == LONG) printf("assets: %lf\ncurrent price: %lf\nwallet: %lf\n", asset, candle.close, wallet);
    //else if (position == SHORT) printf("assets: %lf\nsold at: %lf\nwallet: %lf\n", asset, candle.close, wallet);
	std::cout << "-----------------------" << std::endl << std::endl;
    //std::cout << ema200->getValue() << std::endl << rsi14->getValue() << std::endl;
	runcnt++;
}
