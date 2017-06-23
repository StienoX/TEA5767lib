#include "hwlib.hpp"
#include "radioReceiver.hpp"
#include "potmeterInput.hpp"

void check_freq_read_and_write() {
   auto scl = hwlib::target::pin_oc( hwlib::target::pins::scl );
   auto sda = hwlib::target::pin_oc( hwlib::target::pins::sda );
   
   auto i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda( scl,sda );
   
   radioReceiver radio(i2c_bus);
   
   float set_frequency = 96.8;
   
   radio.set_freq(set_frequency);
   hwlib::wait_ms(3000);
   int read_frequency = radio.read_freq() * 10;
   hwlib::cout << (int) read_frequency; // Be aware of rounding/tuning errors
   
}

void check_over_limit() {
	
   auto scl = hwlib::target::pin_oc( hwlib::target::pins::scl );
   auto sda = hwlib::target::pin_oc( hwlib::target::pins::sda );
   
   auto i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda( scl,sda );
   
   radioReceiver radio(i2c_bus);
   
   float set_frequency = 2291.1;
   
   radio.set_freq(set_frequency); //check if sound is not muted and/or if chip is in search mode.
}

void check_standby() {
	
   auto scl = hwlib::target::pin_oc( hwlib::target::pins::scl );
   auto sda = hwlib::target::pin_oc( hwlib::target::pins::sda );
   
   auto i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda( scl,sda );
   
   radioReceiver radio(i2c_bus);
   
   radio.standby();
   hwlib::wait_ms(3000);
   radio.wake_up();
   radio.set_freq(91.1);
}

void check_mute() {
	
   auto scl = hwlib::target::pin_oc( hwlib::target::pins::scl );
   auto sda = hwlib::target::pin_oc( hwlib::target::pins::sda );
   
   auto i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda( scl,sda );
   
   radioReceiver radio(i2c_bus);
   
   radio.mute();
   hwlib::wait_ms(3000);
   radio.unmute();
}

int main( void ){	
    
   // kill the watchdog
   WDT->WDT_MR = WDT_MR_WDDIS;
   
   
   check_mute();
   hwlib::wait_ms(1000);
   check_freq_read_and_write();
   hwlib::wait_ms(1000);
   check_over_limit();
   hwlib::wait_ms(1000);
   check_standby();
   
   return 0;
}

