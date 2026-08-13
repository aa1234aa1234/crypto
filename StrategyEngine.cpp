//
// Created by sw_303 on 2026-08-06.
//

#include "StrategyEngine.h"
#include "IndicatorsEngine.h"
#include "indicators.hpp"
#include <iostream>

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
    auto rsi14 = indicatorsengine->getIndicator<indicators::RSI>({14});

    double sma200slope = ((sma200->getValue() - sma200->previous(20))/sma200->previous(20));

    if (candle.close > sma200->getValue() && sma50->getValue() > sma200->getValue() && sma200slope > 0.01)
    {
        market_state = UPTREND;
    }

    else if (candle.close < sma200->getValue() && sma50->getValue() < sma200->getValue() && sma200slope < -0.01)
    {
        market_state = DOWNTREND;
    }

    else market_state = SIDEWAYS;
}

void StrategyEngine::run(const Candle& candle)
{
    auto sma200 = indicatorsengine->getIndicator<indicators::SMA>({200});
    auto sma50 = indicatorsengine->getIndicator<indicators::SMA>({50});
    auto ema200 = indicatorsengine->getIndicator<indicators::EMA>({200});
    auto ema50 = indicatorsengine->getIndicator<indicators::EMA>({50});
    auto rsi14 = indicatorsengine->getIndicator<indicators::RSI>({14});

    update_regime(candle);

    int signal = false;

    switch (market_state)
    {
    case UPTREND:
        signal = (candle.low <= ema50->getValue()*1.005 && is_price_near(candle.close, ema50) && rsi14->getValue() >= 50);
        if (signal && position == FLAT) position = LONG;
        break;
    case DOWNTREND:
        signal = (
        candle.high >= ema50->getValue() * 0.995 &&
        is_price_near(candle.close, ema50) &&
        rsi14->getValue() <= 50
        );
        if (signal && position == FLAT) position = SHORT;
        break;
    case SIDEWAYS:
        position = FLAT;
        break;
    }

    std::cout << position << std::endl;
    std::cout << ema200->getValue() << std::endl << rsi14->getValue() << std::endl;
}
