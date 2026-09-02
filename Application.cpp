//
// Created by sw_303 on 2026-06-18.
//

#include "Application.h"

#include <bits/this_thread_sleep.h>
#include <curl/curl.h>

bool Application::isRunning = false;

Application::Application()
{
	engine = new Engine();
	csvReader.ReadCsv(engine->getCandles(), "../a.csv");
	engine->initialize();
}

void Application::run()
{
	double deltatime=0, lasttime=0;
	lasttime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	while (isRunning)
	{
		double currenttime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		deltatime = currenttime - lasttime;
		//if (deltatime < 60000) continue;
		engine->run();
		lasttime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		std::this_thread::sleep_for(std::chrono::seconds(0));
	}

}
