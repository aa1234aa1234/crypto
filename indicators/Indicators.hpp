#pragma once
#include <vector>
#include <cmath>
#include <queue>
#include <typeindex>
#include "Candle.h"

namespace indicators {
	inline double sma(const std::vector<double>& prices, int index, int period) {
		if(prices.size() < period) return NAN;

		double sum = 0.0;
		for(int i = (index > prices.size() ? prices.size() : index) - period + 1; i<=index; i++) sum += prices[i];

		return sum/period;
	}

	inline double ema(const std::vector<double>& prices, int index, int period)
	{
		double alpha = 2. / (period + 1.0);
		double ema = indicators::sma(prices, period - 1, period);

		for (int i = period; i<=index; i++)
		{
			ema = alpha * prices[i] + (1.0 - alpha) * ema;
		}

		return ema;
	}


	struct IndicatorType
	{
		std::type_index type;
		std::vector<int> periods;

		bool operator==(const IndicatorType& other) const
		{
			bool flag = true;
			if (other.periods.size() != periods.size()) return false;
			for (int i = 0; i<periods.size(); i++) if (periods[i] != other.periods[i]) { flag = false; return false; }
			return flag && other.type == type;
		}
	};



	class Indicator
	{
	public:
		virtual ~Indicator() = default;
		[[nodiscard]] virtual double getValue() = 0;
		virtual void recalculate(const Candle& candle) = 0;
		virtual std::string getTypeId() = 0;
		virtual IndicatorType getType() = 0;
	};

	class SMA : public Indicator
	{
		IndicatorType type{std::type_index(typeid(SMA))};
		double sum = 0;
		int period = 0;
		double lastValue = 0;
		std::queue<double> prices;

		void calculateValue(const std::vector<double>& prices, int index, int period)
		{
			if(prices.size() < period) return;

			double sum = 0.0;
			for(int i = (index > prices.size() ? prices.size() : index) - period + 1; i<=index; i++) sum += prices[i];

			this->sum = sum;
		}
	public:
		SMA(const std::vector<double>& prices, int index, int period) : period(period)
		{
			calculateValue(prices, index, period);
		}

		SMA(int period) : period(period)
		{
			type.periods = {period};
		}

		void recalculate(const Candle& candle) override
		{
			sum += candle.close;
			prices.push(candle.close);
			if (prices.size() > period)
			{
				sum -= prices.front(); prices.pop();
			}
		}

		double getValue() override { lastValue=prices.size() == period ? sum/period : -1; return lastValue; }

		std::string getTypeId() override { return typeid(SMA).name() + period; }
		IndicatorType getType() override { return type; }
	};

	class EMA : public Indicator
	{
		IndicatorType type{std::type_index(typeid(EMA))};
		int period;
		double lastValue = 0;
		const double alpha = 2. / (period + 1.0);
	public:
		EMA(int period, SMA* sma) : period(period)
		{
			type.periods = {period};
			lastValue = sma->getValue();
		}

		void recalculate(const Candle& candle) override
		{
			lastValue = alpha * candle.close + lastValue * (1-alpha);
		}

		double getValue() override { return lastValue; }

		std::string getTypeId() override { return typeid(EMA).name() + period; }
		IndicatorType getType() override { return type; }
	};

	class RSI : public Indicator
	{
		IndicatorType type{std::type_index(typeid(RSI))};
		int period;
	public:
		RSI(int period) : period(period) {}

		IndicatorType getType() override { return type; }
	};
}

template<>
	struct std::hash<indicators::IndicatorType>
{
	size_t operator()(const indicators::IndicatorType& indicator) const
	{
		return std::hash<std::type_index>()(indicator.type);
	}
};


//Trend:
//50 EMA > 200 EMA

//Momentum:
//RSI > 50

//Strength:
//ADX > 25

//Risk:
//ATR-based stop loss

//SMA crossover
// EMA crossover
// RSI mean reversion
// Bollinger Bands
// MACD
// breakout systems
// combinations of indicators

//api token PyubX3eliL6CQZue15khySlmfjGpuEyBUXcPrNJmOI3Mk50MRtVX3xrtjkZzNOwtBjNUm4rAj-rzPiRuvpllhw
