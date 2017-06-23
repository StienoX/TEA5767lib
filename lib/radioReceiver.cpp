// Copyright Stein Bout 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "hwlib.hpp"
#include "radioReceiver.hpp"
#include <math.h> 

radioReceiver::radioReceiver(hwlib::i2c_bus_bit_banged_scl_sda & i2c_bus):
	i2c_bus ( i2c_bus )
	{};
	
radioReceiver::radioReceiver(char byte_arr[5], hwlib::i2c_bus_bit_banged_scl_sda & i2c_bus):
	i2c_bus ( i2c_bus )
	{
	byte_array[0] = byte_arr[0];
	byte_array[1] = byte_arr[1];
	byte_array[2] = byte_arr[2];
	byte_array[3] = byte_arr[3];
	byte_array[4] = byte_arr[4];
	};
	

	
void radioReceiver::set_freq(float new_freq) {
	if (byte_array[3] & 0x10) { //check xtal
	
		if (byte_array[2] & 0x10) { //check hilo
			freq = 4*(new_freq * 1000 + 225) / 32.768; // 32 768Hz high side injection
		} else {
			freq = 4*(new_freq * 1000 - 225) / 32.768; // 32 768Hz low side injection
		}
		
		
	} else {
		
		if (byte_array[2] & 0x10) {
			freq = 4*(new_freq * 1000 + 225) / 50; // 50 000Hz high side injection
		} else {
			freq = 4*(new_freq * 1000 - 225) / 50; // 50 000Hz low side injection
		}
		
		
	}
	set_bit(0,7,0);   // mute off
	transform_freq(); // puts freq into the byte_array
	write_to_tea();   // write to the TEA5756
}

float radioReceiver::read_freq() {
	read_from_tea();
	if (byte_array[3] & 0x10) { //check xtal
	
		if (byte_array[2] & 0x10) { //check hilo
			return ((freq * 32.768 / 4 - 225 ) / 1000); // 32 768Hz high side injection
		} else {
			return ((freq * 32.768 / 4 + 225 ) / 1000); // 32 768Hz low side injection
		}
		
		
	} else {
		
		if (byte_array[2] & 0x10) {
			return ( (freq * 50 / 4 - 225 ) / 1000); // 50 000Hz high injection
		} else {
			return ((freq * 50 / 4 + 225 ) / 1000); // 50 000Hz low injection
		}
		
		
	}
}

void radioReceiver::search_up() {
	
	set_bit(3,0,1);
	set_bit(3,1,1);
	set_bit(3,2,1);
	set_bit(3,3,1);
	//set_bit(3,4,1);
	set_bit(2,7,1);   // search up on
	set_bit(2,6,1);   // stop level on
	set_bit(0,7,1);   // mute on
	set_bit(0,6,1);   // search on
	transform_freq(); // stores freq in byte_array
	
	write_to_tea();      // writes data to tea
	hwlib::wait_ms(2500); // wait 500ms
	set_bit(0,6,0);      // search off
	set_bit(0,7,0);      // unmute
	read_from_tea();
	overwrite_freq();
	set_bit(3,0,0);
	set_bit(3,1,0);
	set_bit(3,2,0);
	set_bit(3,3,0);
	write_to_tea();
}

void radioReceiver::search_down() {
	
	set_bit(3,7,0);   // search up down
	set_bit(3,6,1);   // stop level on
	set_bit(0,7,0);   // mute off
	set_bit(0,6,1);   // search on
	transform_freq(); // stores freq in byte_array
	
	
	write_to_tea();   // writes data to tea
}

void radioReceiver::set_bit(int byte_pos, int bit, bool set_to) {
	char mask = pow( 2 , bit);
	if(set_to) {
		byte_array[byte_pos] |= mask; // set bit.
	} else {
		byte_array[byte_pos] &= ~mask; //unset bit.
	}
	
}

void radioReceiver::write_to_tea() {
	i2c_bus.write(address, byte_array, 5);
}
 
void radioReceiver::read_from_tea() {
	i2c_bus.read(address, read_byte_array, 5);
}

void radioReceiver::transform_freq() {
	bool bit7 = byte_array[0] & 0x80; //tempary stores bit 7 and 6
	bool bit6 = byte_array[0] & 0x40;
	byte_array[0] = (freq >> 8);   // Moves last 8 bits to the front 
	byte_array[1] = (freq & 0xFF); // checks all bits with 0000 0000 1111 1111 so only the first bits are remaining
	set_bit(0,7,bit7); //writes bit 7 and 6 to the byte_array
	set_bit(0,6,bit6);
}

//needs some round changes.. because we are using the 32.768 crystal and we are not taking 100mhz steps but slightly below that so we need to round that..
void radioReceiver::overwrite_freq() {
	bool bit7 = byte_array[0] & 0x80; //tempary stores bit 7 and 6
	bool bit6 = byte_array[0] & 0x40;
	byte_array[0] = read_byte_array[0];
	byte_array[1] = read_byte_array[1];
	set_bit(0,7,bit7); //writes bit 7 and 6 to the byte_array
	set_bit(0,6,bit6);
}

void radioReceiver::standby() {
	set_bit(3,6,1); 
	write_to_tea();
}

void radioReceiver::wake_up() {
	set_bit(3,6,0); 
	write_to_tea();
}

void radioReceiver::mute() {
	set_bit(0,7,1); 
	write_to_tea();
}

void radioReceiver::unmute() {
	set_bit(0,7,0); 
	write_to_tea();
}