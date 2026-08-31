//
// Created by sw_303 on 2026-08-06.
//

#ifndef STRATEGYENGINE_H
#define STRATEGYENGINE_H
#include "Candle.h"


class IndicatorsEngine;

class StrategyEngine {
	double wallet=500, asset;
    double trendscore = 0.0;
    enum MarketState
    {
        UPTREND,
        SIDEWAYS,
        DOWNTREND,
    } market_state;

    IndicatorsEngine* indicatorsengine;

    void update_regime(const Candle& candle);
public:
    enum Position
    {
        FLAT=0,
        LONG,
        SHORT
    } position = FLAT;

    StrategyEngine(IndicatorsEngine* indicatorsengine) : indicatorsengine(indicatorsengine) {}
    ~StrategyEngine();

    void run(const Candle& candle);
};



#endif //STRATEGYENGINE_H
