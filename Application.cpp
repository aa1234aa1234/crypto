//
// Created by sw_303 on 2026-06-18.
//

#include "Application.h"

bool Application::isRunning = false;

Application::Application()
{
	csvReader.ReadCsv(engine.getCandles(), "../a.csv");
}

void Application::run()
{
	engine.run();
}
