// Copyright Stein Bout 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef POTMETERINPUT_HPP
#define POTMETERINPUT_HPP
#include "hwlib.hpp"
#include <math.h>

///
/// @class potmeter
/// @author Stein Bout
/// @date 21/06/2017
/// @file potmeterInput.hpp
/// @brief This class uses the analog in from the arduino due to read a potentiometer and converts the value to a frequency
class potmeter { 
private:
   
   int min_value;
   int max_value;
   bool bandwith;
   double step;
   due::pin_adc & input;
   
public:
   ///@brief Constructor for the potmeter with max and min values of the potentiometer.
   ///@param int min_value: the minium value from the potentiometer.
   ///@param int max_value: the maxium value from the potentiometer.
   ///@param bool bandwith: 1 for europe/us; 0 for japan
   ///@param due::pin_adc & input: the analog input pin used when connecting the potentiometer.
   potmeter(int min_value, int max_value, bool bandwith, due::pin_adc & input):
      min_value( min_value ),
      max_value( max_value ),
	  bandwith( bandwith ),
	  input ( input )
	  {
	   
	  step = 20 / ((max_value - min_value)/1000) ;
	  };
	
	///@brief The get function wich calculates and returns the frequency based on the setting of the potentiometer.
	///@return float: return the (new) frequency for the radio.
	float get_pot_freq() {
		if(bandwith) {
			return ((input.get() * step * 10 + 876000)/10000);
		} else {
			return ((input.get() * step * 10 + 760000)/10000);
		}
	}
	
	
};

#endif // POTMETERINPUT_HPP