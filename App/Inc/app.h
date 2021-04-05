/*
 * app.h
 *
 *  Created on: Mar 29, 2021
 *      Author: bormaley
 */

#ifndef APP_H_
#define APP_H_

#include "lsm303agr.h"
#include "main.h"

extern "C" void main_app_wrp();

class MainApp {
public:
	static MainApp& get_instance()
	{
		static MainApp mainApp;
		return mainApp;
	}

	MainApp(MainApp const&)           = delete;
	void operator=(MainApp const&)    = delete;
	MainApp(MainApp&&)                = delete;
	MainApp& operator=(MainApp&&)     = delete;

	void run();
private:
	MainApp();

	LSM303DLHC lsm303dhlc;
};

#endif /* APP_H_ */
