#include "hwlib.hpp"
#include "radioReceiver.hpp"
#include "potmeterInput.hpp"
int main( void ){	
    
   // kill the watchdog
   WDT->WDT_MR = WDT_MR_WDDIS;
   
   namespace target = hwlib::target;
   
   auto scl = target::pin_oc( target::pins::scl );
   auto sda = target::pin_oc( target::pins::sda );
   
   auto i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda( scl,sda );
   
   auto input = due::pin_adc(due::ad_pins(2));
   
   potmeter meter(0, 4076, 1 ,input); // 4076 is the highest value and 0 is the lowest.
   radioReceiver radio(i2c_bus);
   
   float new_freq;
   
   radio.set_freq(91.1); 
   
   hwlib::wait_ms(10000); //wait 10 secs
   
   while(1) {
	   new_freq = meter.get_pot_freq();
	   radio.set_freq(new_freq);
	   hwlib::cout << (int) new_freq * 10<< "  -";
	   hwlib::wait_ms(50);
   }
}