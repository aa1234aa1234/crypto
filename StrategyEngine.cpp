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

static bool is_price_near(double price, indicators::Indicator* indicator, double atr14)
{
    const double DISTANCE_THRESHOLD = 2.0;
    double distance = std::abs(price-indicator->getValue()) / atr14;
    return distance<=DISTANCE_THRESHOLD;
}

static void add_trendscore(double& trendscore, double score)
{
    trendscore += score;
}

template<typename T>
static T get_prev(std::vector<T>& v, int idx)
{
    if (idx >= v.size()) return -1;
    return v[v.size() - 1 - idx];
}

static double calculate_effeciency(std::vector<double>& v, int period)
{
    if (period >= v.size()) return -1;
    double sum = 0.0;

    for (int i = 0; i<period; i++)
    {
        sum += std::abs(get_prev(v, i) - get_prev(v, i+1));
    }

    return std::abs((v.back()-get_prev(v, period)))/sum;
}

void StrategyEngine::update_regime(const Candle& candle)
{
    static std::vector<double> trendscore_hist, close_hist;
    auto sma200 = indicatorsengine->getIndicator<indicators::SMA>({200});
    auto sma50 = indicatorsengine->getIndicator<indicators::SMA>({50});
    auto ema50 = indicatorsengine->getIndicator<indicators::EMA>({50});
    auto ema21 = indicatorsengine->getIndicator<indicators::EMA>({21});
    auto ema9 = indicatorsengine->getIndicator<indicators::EMA>({9});
    auto atr14 = indicatorsengine->getIndicator<indicators::ATR>({14});

    double sma200slope = ((sma200->getValue() - sma200->previous(20))/sma200->previous(20));
    double sma50slope = ((sma50->getValue() - sma50->previous(20))/atr14->getValue());
    double ema50slope = ((ema50->getValue() - ema50->previous(10))/atr14->getValue());
    double ema21slope = ((ema21->getValue() - ema21->previous(5))/atr14->getValue());
    double ema9slope = ((ema9->getValue() - ema9->previous(5))/atr14->getValue());

    static int uptrend_confirm = 0, downtrend_confirm = 0;

    trendscore = 0.0;

    close_hist.push_back(candle.close);

    double efficiency = calculate_effeciency(close_hist, 10);
    backtest += string_format("efficiency: %lf\n", efficiency);

    if (ema9->getValue() > ema21->getValue())
    {
        //trendscore += 0.20;
        trendscore += 0.20 * std::clamp((ema9->getValue()-ema21->getValue())/atr14->getValue(), -1.0, 1.0);
        backtest += "ema9 > ema21 trendscore + 20\n";
    }
    else
    {
        //trendscore -= 0.20;
        trendscore += 0.20 * std::clamp((ema9->getValue()-ema21->getValue())/atr14->getValue(), -1.0, 1.0);
        backtest += "ema9 <= ema21 trendscore - 20\n";
    }

    if (ema9->getValue() > ema50->getValue())
    {
        //trendscore += 0.250;
        trendscore += 0.20 * std::clamp((ema9->getValue()-ema50->getValue())/atr14->getValue(), -1.0, 1.0);
        backtest += "ema21 > ema50 trendscore + 25\n";
    }

    if (ema9slope > 1.0)
    {
        trendscore += 0.20;
        backtest += "ema21 slope > 0.08 trendscore + 20\n";
    }
    else if (ema9slope < -0.6)
    {
        trendscore -= 0.20;
        backtest += "ema21 slope <= 0.08 trendscore - 20\n";
    }

    // if (ema50slope > 0.5)
    // {
    //     trendscore += 0.20;
    //     backtest += "ema50 slope > 0.10 trendscore + 20\n";
    // }
    // else if (ema50slope < -0.80) {
    //     trendscore -= 0.20;
    //     backtest += "ema50 slope < -0.10 trendscore - 20\n";
    // }
    trendscore += 0.10 * (ema50slope/0.5);
    backtest += string_format("%lf from ema50slope\n", 0.10*(ema50slope/0.5));

    trendscore += 0.05 * std::clamp((ema9->getValue()-candle.close)/atr14->getValue(), -1.0, 1.0);

    if (candle.close > sma200->getValue())
    {
        trendscore += 0.20 * std::clamp((candle.close-sma200->getValue())/atr14->getValue(),0.0,1.0);
        backtest += "candle > sma200 trendscore + 20\n";
    }
    else if ((candle.close-ema9->getValue())/atr14->getValue() <= -3.0 && candle.close < sma200->getValue())
    {
        //trendscore += 0.10 * std::clamp((candle.close-sma200->getValue())/atr14->getValue(),-1.0,1.0);
        trendscore -= 0.10;
        backtest += string_format("%lf",(candle.close-ema9->getValue()/atr14->getValue()));
        backtest += "candle <= sma200 trendscore - 10\n";
    }

    double displacement = candle.close - get_prev(close_hist, 10);
    backtest += string_format("displacement: %lf\n", displacement);
    if (trendscore >= 0.60)
    {
        backtest += string_format("trendscore >= 0.60\n");
        if (efficiency >= 0.30)
        {
            backtest += string_format("efficiency >= 0.30\n");
            if (displacement > atr14->getValue()) backtest += string_format("displacement > atr\n");
        }
    }

    if (trendscore >= 0.58 && efficiency >= 0.00)
    {
        if (uptrend_confirm < 2) { uptrend_confirm++; downtrend_confirm=0; }
        else
        {
            backtest += string_format("entered UPTREND at backtest number %d\n", runcnt);
            market_state = UPTREND;

            uptrend_confirm = 0;
        }
    }
    else if (trendscore <= -0.60 && efficiency >= 0.00)
    {
        if (downtrend_confirm < 2) { downtrend_confirm++; uptrend_confirm=0; }
        else
        {
            downtrend_confirm = 0;
            market_state = DOWNTREND;

        }
    }
    else
    {
        market_state = SIDEWAYS;
        uptrend_confirm = 0;
        downtrend_confirm = 0;
    }

    switch (market_state)
    {
    case UPTREND:
        if (trendscore <= 0.40) market_state = SIDEWAYS;
        break;
    case DOWNTREND:
        if (trendscore >= -0.40 || std::abs(trendscore-get_prev<double>(trendscore_hist, 10))/get_prev<double>(trendscore_hist, 10) >= 0.8) market_state = SIDEWAYS;
        break;
    }

    trendscore_hist.push_back(trendscore);

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
    static double short_quantity = 0;
    static double long_entry = 0.0f;
    static int pullback = 0, aboveema = 0, bullish = 0, nearema = 0, rsiok = 0, momentum = 0;
    static int f1=0,f2=0,f3=0,f4=0,f5=0,f6=0;
    backtest = "";
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
    double ema50slope = ((ema50->getValue() - ema50->previous(20))/atr14->getValue());
    double ema21slope = ((ema21->getValue() - ema21->previous(20))/atr14->getValue());
    double candle_range = (highrc->getValue()-lowrc->getValue());

    //add a candle range to signal
    //update to atr later
    /*double range = candle.high - candle.low;
    double atr = ...;

    range < 2.0 * atr
    range/atr < candle.close*/

    int signal = false;
    bool pullbackc, above_ema, bullishc, near_ema, rsi_ok, momentumc;

    switch (market_state)
    {
    case UPTREND:
        if (position == LONG && candle.close <= long_entry-atr14->getValue()*20)
        {
            position = FLAT;
            wallet += candle.close * asset, asset = 0;
            backtest += string_format("exited LONG position at %lf due to stopgap\n", candle.close);
            backtest += string_format("assets: %lf\nsold at: %lf\nwallet: %lf\n", asset, candle.close, wallet);
            break;
        }
        // strong_uptrend =candle.close > sma200->getValue() && ema21->getValue() > ema50->getValue() && ema50slope > 0.5;
        //
        // pullback = lowrc->getValue() <= ema50->getValue() + 0.5 * atr14->getValue();
        //
        // bullish_reversal =candle.close > candle.open && candle.close > ema50->getValue();
        //
        // momentum =rsi14->getValue() > 50 && ema9->getValue() > ema21->getValue();
        //
        // signal =(strong_uptrend && pullback && bullish_reversal && momentum);
        pullbackc = std::abs(lowrc->getValue() - ema50->getValue()) <= 12.0 * atr14->getValue();
        if (pullbackc) pullback++;
        above_ema = candle.close > ema50->getValue();
        if (candle.close > ema50->getValue()) aboveema++;
        bullishc = candle.close > candle.open;
        if (bullishc) bullish++;
        near_ema = is_price_near(lowrc->getValue(), ema50, atr14->getValue());
        if (near_ema) nearema++;
        rsi_ok = rsi14->getValue() >= 45;
        if (rsi_ok) rsiok++;
        momentumc = (ema9->getValue()-ema21->getValue())/atr14->getValue() >= 0.05;
        if (momentumc) momentum++;

        if (pullbackc)
        {
            f1++;
            if (above_ema)
            {
                f2++;
                if (bullishc)
                {
                    f3++;
                    if (near_ema)
                    {
                        backtest += string_format(
                    "CANDIDATE: close=%.2f EMA50=%.2f "
                        "distance=%.3f ATR RSI=%.2f "
                        "EMA9=%.2f EMA21=%.2f momentum=%.3f\n",
                        candle.close,
                        ema50->getValue(),
                        (candle.close - ema50->getValue()) / atr14->getValue(),
                        rsi14->getValue(),
                        ema9->getValue(),
                        ema21->getValue(),
                        (ema9->getValue() - ema21->getValue()) / atr14->getValue()
                        );
                        f4++;
                        if (rsi_ok)
                        {
                            f5++;
                            if (momentumc)
                            {
                                f6++;
                            }
                        }
                    }
                }
            }
        }
        signal = (
            std::abs(candle.close - ema9->getValue())/atr14->getValue() <= 0.8 || std::abs(candle.close - ema21->getValue())/atr14->getValue() <= 0.8 &&
            candle.close > ema50->getValue() &&
            // candle.close > candle.open &&
            // is_price_near(lowrc->getValue(), ema50, atr14->getValue()) &&
            // (candle.close - lowrc->getValue()) / atr14->getValue() <= 4.0 &&
            candle.close > ema9->getValue() &&
            rsi14->getValue() >= 45);
        if (signal && position == FLAT)
        {
            position = LONG;
            long_entry = candle.close;
            asset = static_cast<int>(wallet/candle.close), wallet -= asset*candle.close;
            backtest += string_format("entered LONG position at: %lf with %lf assets\n", candle.close, asset);
            backtest += string_format("assets: %lf\nbought at: %lf\nwallet: %lf\n", asset, candle.close, wallet);
        }
        break;
    case DOWNTREND:
        signal = (
        highrc->getValue() >= ema50->getValue() * (1-0.01) &&
        is_price_near(candle.close, ema50, atr14->getValue()) &&
        rsi14->getValue() <= 30
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
            backtest += string_format("entered SHORT position at: %lf with %lf assets\n", candle.close, short_quantity);
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
            backtest += string_format("wallet: %lf\n", wallet);
            short_quantity = 0;
        }
        position = FLAT;
        break;
    }
    backtest += string_format("f1: %d\nf2: %d\nf3: %d\nf4: %d\nf5: %d\nf6: %d\n",f1,f2,f3,f4,f5,f6);
    backtest += string_format("pullback distance: %lf ATR\n", std::abs(lowrc->getValue() - ema50->getValue()) / atr14->getValue());
    backtest += string_format(
    "LONG: pullback=%d above_ema=%d\nbullish=%d\nnear_ema=%d\n"
    "rsi=%d\nmomentum=%d\nsignal=%d\n",
    pullback,
    aboveema,
    bullish,
    nearema,
    rsiok,
    momentum,
    signal
);

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
    "ema9: %lf\n"
    "atr14: %lf\n"
    "candle range: %.4f\n"
    "ema21 value: %lf\n"
    "wallet: %lf\n",

    candle.close / sma200->getValue(),
    sma50->getValue() / sma200->getValue(),
    sma200slope,
    (candle.close - ema50->getValue()) / ema50->getValue(),
    (candle.low - ema50->getValue()) / ema50->getValue(),
    ema50slope,
    trendscore,
    ema21slope,
    ema9->getValue(),
    atr14->getValue(),
    candle_range,
    ema21->getValue(),
    wallet);
    std::cout << backtest;
    //if (position == LONG) printf("assets: %lf\ncurrent price: %lf\nwallet: %lf\n", asset, candle.close, wallet);
    //else if (position == SHORT) printf("assets: %lf\nsold at: %lf\nwallet: %lf\n", asset, candle.close, wallet);
	std::cout << "-----------------------" << std::endl << std::endl;
    //std::cout << ema200->getValue() << std::endl << rsi14->getValue() << std::endl;
	runcnt++;
}
