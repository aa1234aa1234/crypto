//
// Created by sw_303 on 2026-07-11.
//

#include "IndicatorsEngine.h"

#include <stdexcept>

#include "Candle.h"

IndicatorsEngine::IndicatorsEngine()
{

}

IndicatorsEngine::~IndicatorsEngine()
{

}

void IndicatorsEngine::addIndicator(indicators::Indicator* indicator)
{
    indicators[indicator->getType()] = indicator;
}

void IndicatorsEngine::update(const Candle& candle)
{
    for (auto& p : indicators)
    {
        p.second->recalculate(candle);
    }
}

template<typename T>
T* IndicatorsEngine::getIndicator(std::vector<int> params)
{
    if (indicators.size() < 1) return nullptr;
    auto type = indicators::IndicatorType{std::type_index(typeid(T)), std::vector<int>(params)};
    if (indicators.find(type) != indicators.end()) return static_cast<T*>(indicators[type]);
    throw std::runtime_error(std::string("indicator not found ") + std::string(typeid(T).name()));
}


