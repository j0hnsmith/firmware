/*
 * Copyright 2012 BrewPi/Elco Jacobs.
 *
 * This file is part of BrewPi.
 * 
 * BrewPi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * BrewPi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with BrewPi.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef TICKS_H_
#define TICKS_H_

#include "Arduino.h"

typedef unsigned long ticks_millis_t;
typedef unsigned long ticks_micros_t;
typedef unsigned int ticks_seconds_t;
typedef unsigned char ticks_seconds_tiny_t;

class Ticks {
	
};

class MockTicks {
public:
	MockTicks(uint8_t increment) : _increment(increment), _ticks(0) { }

	ticks_millis_t millis() { return _ticks+=_increment; }
	ticks_micros_t micros() { return _ticks+=_increment; }	
	ticks_seconds_t seconds() { return millis()>>10; }	
private:

	uint32_t _increment;
	uint32_t _ticks;			
};

/**
 * Externally provided clock. 
 */

class ExternalTicks {
	public:
	ExternalTicks() : _ticks(0) { }

	ticks_millis_t millis() { return _ticks; }
	ticks_micros_t micros() { return _ticks*1000; }	
	ticks_seconds_t seconds() { return millis()/1000; }	
		
	void setMillis(ticks_millis_t now)	{ _ticks = now; }
	void incMillis(ticks_millis_t advance)	{ _ticks += advance; }
private:
	ticks_millis_t _ticks;			
};



class NoOpDelay {
public:	
	void seconds(uint16_t seconds)	{ millis(seconds<<10); }
	void millis(uint32_t millis)	{ }	
	void microseconds(uint32_t micros) { }
};

#include "avr_ticks.h"

#if BREWPI_EMULATE   // emulator is slow, so increment ticks by 100 for each request
	typedef MockTicks TicksImpl;
	#define TICKS_IMPL_CONFIG 100

	typedef NoOpDelay DelayImpl;
	#define DELAY_IMPL_CONFIG

#elif BREWPI_SIMULATE				// use value holder
	typedef ExternalTicks TicksImpl;
	typedef HardwareDelay DelayImpl;
	#define TICKS_IMPL_CONFIG		// no configuration necessary
	#define DELAY_IMPL_CONFIG
	
#else // use regular hardware timer/delay
	typedef HardwareTicks TicksImpl;
	#define TICKS_IMPL_CONFIG

	typedef HardwareDelay DelayImpl;
	#define DELAY_IMPL_CONFIG

#endif	// BREWPI_EMULATE


extern TicksImpl ticks;
extern DelayImpl wait;



#endif /* TICKS_H_ */
