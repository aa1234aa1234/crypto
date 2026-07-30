//
// Created by sw_303 on 2026-07-11.
//

#ifndef INDICATORSENGINE_H
#define INDICATORSENGINE_H
#include <map>

#include "Indicators.hpp"
#include "Candle.h"
#include <cstdarg>
#include <unordered_map>

class IndicatorsEngine {
    std::vector<indicators::IndicatorType> indicator_order;
    std::unordered_map<indicators::IndicatorType,indicators::Indicator*> indicators{};
public:
    IndicatorsEngine();
    ~IndicatorsEngine();

    void addIndicator(indicators::Indicator* indicator);
    void update(const Candle& candle);
    template<typename T>
    T* getIndicator(std::vector<int> params);
};



#endif //INDICATORSENGINE_H
