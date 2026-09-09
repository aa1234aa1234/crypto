#pragma once
#include <vector>
#include <cmath>
#include <queue>
#include <typeindex>
#include "Candle.h"
#include <algorithm>
#define LOW 0
#define HIGH 1

namespace indicators {

	template<size_t N>
	struct string
	{
		char data[N + 1];

		constexpr string(const char (&str)[N])
		{
			std::copy(str, str + N, data);
		}
	};

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
	protected:
		std::deque<double> value_history;
	public:
		virtual ~Indicator() = default;
		[[nodiscard]] virtual double getValue() = 0;
		virtual void recalculate(const Candle& candle) = 0;
		double previous(int idx=1) { return value_history[value_history.size() - 1 - idx]; }
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
			//calculateValue(prices, index, period);
			for (auto& p : prices)
			{
				Candle cCandle{"", p};
				SMA::recalculate(cCandle);
			}
		}

		SMA(int period) : period(period)
		{
			type.periods = {period};
		}

		void recalculate(const Candle& candle) override
		{
			value_history.push_back(getValue());
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
			value_history.push_back(lastValue);
			lastValue = alpha * candle.close + lastValue * (1-alpha);
		}

		double getValue() override { return lastValue; }

		std::string getTypeId() override { return typeid(EMA).name() + period; }
		IndicatorType getType() override { return type; }
	};

	template<int T>
	class RC : public Indicator
	{
		IndicatorType type{std::type_index(typeid(RC))};
		std::string curr_date;
		double low,high;
	public:
		RC(double low) : low(0), high(0) {}
		~RC() = default;
		void recalculate(const Candle& candle) override
		{
			#if T == LOW
				value_history.push_back(low);
			#else
				value_history.push_back(high);
			#endif
			if (curr_date != candle.date.substr(0,10))
			{
				curr_date = candle.date.substr(0, 10);
				low = candle.close;
				high = candle.close;
				return;
			}
			low = candle.close < low ? candle.close : low;
			high = candle.close > high ? candle.close : high;

		}

		double getValue() override { return T == LOW ? low : high; }
		double low_or_high(int g) { return g == LOW ? low : high;}

		std::string getTypeId() override { return typeid(RC).name() + T; }
		IndicatorType getType() override { return type; }
	};

	class ATR : public Indicator
	{
		IndicatorType type{std::type_index(typeid(ATR))};
		int period;
		double value;

		RC<LOW>* running_candle;
	public:
		template<int T>
		ATR(int period, RC<T>* rc) : period(period), running_candle(rc)
		{
			type.periods = {period};
		}
		~ATR() = default;

		void recalculate(const Candle& candle) override
		{
			static double prev_close = 0.0f;
			static int count = 0;
			if (count == 0)
			{
				prev_close = candle.close;
				count++;
				return;
			}

			// const double tr = std::max({
			// 	running_candle->low_or_high(HIGH) - running_candle->low_or_high(LOW),
			// 	std::abs(running_candle->low_or_high(HIGH) - prev_close),
			// 	std::abs(running_candle->low_or_high(LOW) - prev_close)
			// });
			const double tr = std::abs(candle.close - prev_close);

			prev_close = candle.close;
			if (count < period)
			{
				value += tr;
				count++;
				if (count == period)
				{
					value /= period;
					value_history.push_back(value);
				}
				return;
			}
			value_history.push_back(value);
			value = ((period - 1) * value + tr) / period;
		}

		double getValue() override { return value; }

		std::string getTypeId() override { return typeid(ATR).name() + period; }
		IndicatorType getType() override { return type; }
	};

	class RSI : public Indicator
	{
		IndicatorType type{std::type_index(typeid(RSI))};
		int period;
		double averageGain, averageLoss, sumGain, sumLoss, prevPrice;
		bool flag = false;
	public:
		RSI(int period) : period(period)
		{
			type.periods = {period};
		}

		void recalculate(const Candle& candle) override
		{
			static int count = 0;
			value_history.push_back(getValue());
			if (!count)
			{
				prevPrice = candle.close;
				count++;
				return;
			}

			double change = candle.close - prevPrice;
			prevPrice = candle.close;

			double gain = std::max(change,0.0), loss = std::max(-change, 0.0);

			if (!flag)
			{
				sumGain += gain, sumLoss += loss;
				count++;
				if (count == period + 1)
				{
					averageGain = sumGain / count;
					averageLoss = sumLoss / count;
					flag = true;
				}
				return;
			}

			averageGain = (averageGain * (period - 1) + gain) / period;
			averageLoss = (averageLoss * (period - 1) + loss) / period;
		}

		double getValue() override
		{
			if (!flag) return -1;
			if (averageLoss == 0.0) return 100.0;

			return 100.0 - (100.0 / (1.0 + (averageGain / averageLoss)));
		}

		std::string getTypeId() override { return typeid(RSI).name() + period; }
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
