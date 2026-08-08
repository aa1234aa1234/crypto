//
// Created by sw_303 on 2026-08-06.
//

#ifndef STRATEGYENGINE_H
#define STRATEGYENGINE_H
#include "Candle.h"


class IndicatorsEngine;

class StrategyEngine {
    IndicatorsEngine* indicatorsengine;
public:
    StrategyEngine(IndicatorsEngine* indicatorsengine) : indicatorsengine(indicatorsengine) {}
    ~StrategyEngine();

    void run(const Candle& candle);
};



#endif //STRATEGYENGINE_H
