// Copyright Stein Bout 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)


#ifndef RADIORECEIVER_HPP
#define RADIORECEIVER_HPP
#include "hwlib.hpp"

///
/// @class radioReceiver
/// @author Stein Bout
/// @date 21/06/2017
/// @file radioReceiver.hpp
/// @brief the class radioReceiver uses the i2c_bus to send data to the TEA5767 chip. The data includes the frequency and the settings of the chip.
class radioReceiver { 
private:
   
   //bool hilo = 1; // 0 - low side injection; 1 - high side injection
   //bool xtal = 0; // 0 - 32.768 kHz crystal frequency; 0 - 13/6.5 mHz crystal frequency
   //bool band = 0; // 0 - euro/us FM band; 1 - japan FM band
   //bool mono = 0; // 0 - stereo; 1 - mono
   
   char unsigned byte_array[5] = {0x80,0x00,0xB0,0x10,0x00}; // byte_array to store the data that needs to be send to the chip.
   char unsigned read_byte_array[5]; // read_byte_array to store the read data from the chip.
   hwlib::i2c_bus_bit_banged_scl_sda & i2c_bus; //i2c bus to use for the data transmission.
   int freq = 0; //store the calculated frequency using the set_freq function.
   char address = 0x60; //The address of the TEA5767
   
   void write_to_tea(); ///@brief write byte_array to the TEA5767
   void read_from_tea(); ///@brief reads and stores the data from the TEA5767 in read_byte_array.
   void transform_freq(); ///@brief puts the calculated frequency into the byte_array.
   void overwrite_freq(); ///@brief overwrites the frequency in the byte array with the frequency read from the TEA5767.
   
public:

   ///@brief Constructor for the radioReceiver wit default settings.
   ///@param hwlib::i2c_bus_bit_banged_scl_sda & i2c_bus: a refrence to the i2c_bus from the hwlib libary.
   radioReceiver(hwlib::i2c_bus_bit_banged_scl_sda & i2c_bus); 
   
   ///@brief Constructor for the radioReceiver with costom settings.
   ///@param char byte_array[5]: is used for the costom settings.
   ///@param hwlib::i2c_bus_bit_banged_scl_sda & i2c_bus: a refrence to the i2c_bus from the hwlib libary.
   radioReceiver(char byte_array[5], hwlib::i2c_bus_bit_banged_scl_sda & i2c_bus); 
   
   ///@brief set a new frequency. An example is: set_freq(91.1); The new frequency will be calculated and the chip will tune to the new radio station (in this case wil ltune to 91.1).
   ///@param float new_freq: is the value for the new frequency. 
   void set_freq(float new_freq);
   
   ///@brief reads the frequency and returns it as a float. 
   ///@return return a float. An example is: 91.1 this is the frequency the radio is currently tuned to.
   float read_freq(); 
   
   ///@brief Does a autonimous search up
   ///@note Still work in progress
   void search_up(); 
   
   ///@brief Does a autonimous search down
   ///@note Still work in progress
   void search_down(); 
   
   ///@brief Set a bit in the byte_array for to change the settings of the chip really easy. Note: function does not write to the chip any changes made will be processed on the next write action.
   ///@param int byte_pos: is the between 0 and 4 for all the bytes send. so 0 is the first byte send and 4 is the last byte send.
   ///@param int bit: is the bit from the byte you want to change. A byte has only 8 bits so we will be using 0 to 7, where 0 is the first bit and 7 is the last bit.
   ///@param bool set_to: decides if the selected bit needs to change to a 0 or a 1.
   void set_bit(int byte_pos, int bit, bool set_to); 
   
   ///@brief disables the chip expect for the i2c input/output
   void standby();
   
   ///@brief enables the chip. (gets it out of standby mode)
   void wake_up();
   
   ///@brief mutes the received sound.
   void mute();
   
   ///@brief unmutes the sound.
   void unmute();
};

#endif // RADIORECEIVER_HPP