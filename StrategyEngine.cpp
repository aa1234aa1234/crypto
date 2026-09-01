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

static bool is_price_near(double price, indicators::Indicator* indicator)
{
    constexpr double DISTANCE_THRESHOLD = 0.01;
    double distance = std::abs(price-indicator->getValue()) / indicator->getValue();
    return distance<=DISTANCE_THRESHOLD;
}

void StrategyEngine::update_regime(const Candle& candle)
{
    auto sma200 = indicatorsengine->getIndicator<indicators::SMA>({200});
    auto sma50 = indicatorsengine->getIndicator<indicators::SMA>({50});
    auto ema200 = indicatorsengine->getIndicator<indicators::EMA>({200});
    auto ema50 = indicatorsengine->getIndicator<indicators::EMA>({50});
    auto ema21 = indicatorsengine->getIndicator<indicators::EMA>({21});
    auto ema9 = indicatorsengine->getIndicator<indicators::EMA>({9});
    auto rsi14 = indicatorsengine->getIndicator<indicators::RSI>({14});
    auto atr14 = indicatorsengine->getIndicator<indicators::ATR>({14});

    double sma200slope = ((sma200->getValue() - sma200->previous(20))/sma200->previous(20));
    double sma50slope = ((sma50->getValue() - sma50->previous(20))/atr14->getValue());
    double ema50slope = ((ema50->getValue() - ema50->previous(10))/atr14->getValue());
    double ema21slope = ((ema21->getValue() - ema21->previous(10))/atr14->getValue());

    trendscore = 0.0;

    if (ema9->getValue() > ema21->getValue())
    {
        trendscore += 0.25;
    }
    else trendscore -= 0.20;

    if (ema21->getValue() > ema50->getValue()) trendscore += 0.250;
    else trendscore -= 0.250;

    if (ema21slope > 0.06) trendscore += 0.20;
    else if (ema21slope < -0.10) trendscore -= 0.20;

    if (ema50slope > 0.10) trendscore += 0.20;
    else if (ema50slope < -0.10) trendscore -= 0.20;

    if (candle.close > sma200->getValue()) trendscore += 0.10;
    else trendscore -= 0.20;

    if (trendscore >= 0.50) market_state = UPTREND;
    else if (trendscore <= -0.60) market_state = DOWNTREND;
    else market_state = SIDEWAYS;

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
    std::string backtest = "";
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
    double ema50slope = ((ema50->getValue() - ema50->previous(10))/atr14->getValue());
    double ema21slope = ((ema21->getValue() - ema21->previous(10))/atr14->getValue());
    double candle_range = (highrc->getValue()-lowrc->getValue()) / candle.close;

    //add a candle range to signal
    //update to atr later
    /*double range = candle.high - candle.low;
    double atr = ...;

    range < 2.0 * atr
    range/atr < candle.close*/

    int signal = false;

    switch (market_state)
    {
    case UPTREND:
        signal = (lowrc->getValue() <= ema50->getValue()*(1+0.01) && is_price_near(candle.close, ema50) && rsi14->getValue() >= 50);
        if (signal && position == FLAT)
        {
            position = LONG;
            asset = static_cast<int>(wallet/candle.close), wallet -= asset*candle.close;
            backtest += string_format("entered LONG position at: %lf with %lf assets\n", candle.close, asset);
            backtest += string_format("assets: %lf\nbought at: %lf\nwallet: %lf\n", asset, candle.close, wallet);
        }
        break;
    case DOWNTREND:
        signal = (
        highrc->getValue() >= ema50->getValue() * (1-0.01) &&
        is_price_near(candle.close, ema50) &&
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
    "candle range: %.4f\n",

    candle.close / sma200->getValue(),
    sma50->getValue() / sma200->getValue(),
    sma200slope,
    (candle.close - ema50->getValue()) / ema50->getValue(),
    (candle.low - ema50->getValue()) / ema50->getValue(),
    ema50slope,
    trendscore,
    ema21slope,
    candle_range);
    std::cout << backtest;
    //if (position == LONG) printf("assets: %lf\ncurrent price: %lf\nwallet: %lf\n", asset, candle.close, wallet);
    //else if (position == SHORT) printf("assets: %lf\nsold at: %lf\nwallet: %lf\n", asset, candle.close, wallet);
	std::cout << "-----------------------" << std::endl << std::endl;
    //std::cout << ema200->getValue() << std::endl << rsi14->getValue() << std::endl;
	runcnt++;
}
