//
// Created by sw_303 on 2026-07-11.
//

#include "IndicatorsEngine.h"

#include <stdexcept>
#include <utility>

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
    indicator_order.push_back(indicator->getType());
}

void IndicatorsEngine::update(const Candle& candle)
{
    for (auto& p : indicator_order)
    {
        indicators[p]->recalculate(candle);
    }
}

template<typename T>
T* IndicatorsEngine::getIndicator(std::vector<int> params)
{
    if (indicators.size() < 1) return nullptr;
    auto type = indicators::IndicatorType{std::type_index(typeid(T)), std::vector<int>(std::move(params))};
    if (indicators.find(type) != indicators.end()) return static_cast<T*>(indicators[type]);
    throw std::runtime_error(std::string("indicator not found ") + std::string(typeid(T).name()));
}

template indicators::SMA* IndicatorsEngine::getIndicator<indicators::SMA>(std::vector<int> params);
template indicators::EMA* IndicatorsEngine::getIndicator<indicators::EMA>(std::vector<int> params);


