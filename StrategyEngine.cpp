//
// Created by sw_303 on 2026-08-06.
//

#include "StrategyEngine.h"
#include "IndicatorsEngine.h"
#include "indicators.hpp"

StrategyEngine::~StrategyEngine()
{
    if (indicatorsengine) delete indicatorsengine;
}

void StrategyEngine::run(const Candle& candle)
{
    auto sma200 = indicatorsengine->getIndicator<indicators::SMA>({200});
    auto sma50 = indicatorsengine->getIndicator<indicators::SMA>({50});
    auto ema200 = indicatorsengine->getIndicator<indicators::EMA>({200});
    auto ema50 = indicatorsengine->getIndicator<indicators::EMA>({50});
    auto rsi14 = indicatorsengine->getIndicator<indicators::RSI>({14});
}
