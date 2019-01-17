// --------------------------------------
// i2c_scanner (Sming version)
//
// Version 1
//    This program (or code that looks like it)
//    can be found in many places.
//    For example on the Arduino.cc forum.
//    The original author is not know.
// Version 2, Juni 2012, Using Arduino 1.0.1
//     Adapted to be as simple as possible by Arduino.cc user Krodal
// Version 3, Feb 26  2013
//    V3 by louarnold
// Version 4, March 3, 2013, Using Arduino 1.0.3
//    by Arduino.cc user Krodal.
//    Changes by louarnold removed.
//    Scanning addresses changed from 0...127 to 1...119,
//    according to the i2c scanner by Nick Gammon
//    http://www.gammon.com.au/forum/?id=10896
// Version 5, March 28, 2013
//    As version 4, but address scans now to 127.
//    A sensor seems to use address 120.
// Version 6, March 15  2015
//    Sming version by Skurydin Alexey
//
//
// This sketch tests the standard 7-bit addresses
// Devices with higher bit address might not be seen properly.
//

#include <user_config.h>
#include <SmingCore/SmingCore.h>
#include <Libraries/LiquidCrystal/LiquidCrystal_I2C.h>
#define I2C_LCD_ADDR 0x27

LiquidCrystal_I2C lcd(I2C_LCD_ADDR, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

Timer procTimer;

void scanBus()
{
	byte error, address;
	int nDevices;

	Serial.println("Scanning...");

	nDevices = 0;
	for(address = 1; address < 127; address++) {
		// The i2c_scanner uses the return value of
		// the Write.endTransmisstion to see if
		// a device did acknowledge to the address.
		Wire.beginTransmission(address);
		error = Wire.endTransmission();

		WDT.alive(); // Second option: notify Watch Dog what you are alive (feed it)

		if(error == 0) {


                        delay(250);
                        lcd.clear();
                        lcd.setCursor(0, 0);
                        lcd.print("I2C device 0x");
			Serial.print("I2C device found at address 0x");
			if(address < 16){
			        lcd.print("0");
				Serial.print("0");
			}
			Serial.print(address, HEX);
                        lcd.print(address, HEX);
			Serial.println("  ");

			nDevices++;
		} else if(error == 4) {
			Serial.print("Unknow error at address 0x");
			if(address < 16)
				Serial.print("0");
			Serial.println(address, HEX);
		}
	}
	if(nDevices == 0)
		Serial.println("No I2C devices found\n");
	else
		Serial.println("done\n");
}

void init()
{
	Serial.begin(SERIAL_BAUD_RATE);  // 115200 by default
	Serial.systemDebugOutput(false); // Disable debug output

	WDT.enable(false); // First (but not the best) option: fully disable watch dog timer

	// Default I2C pins (SDA: 2, SCL:0)

	// You can change pins:
//	Wire.pins(4,5); // SDA, SCL
	Wire.begin();
    lcd.begin(16, 2); // initialize the lcd for 16 chars 2 lines, turn on backlight

    // ------- Quick 3 blinks of backlight  -------------
    for(int i = 0; i < 3; i++) {
        lcd.backlight();
        delay(150);
        lcd.noBacklight();
        delay(250);
    }
    lcd.backlight(); // finish with backlight on

    //-------- Write characters on the display ------------------
    // NOTE: Cursor Position: (CHAR, LINE) start at 0
    lcd.setCursor(0, 0);
    lcd.print("SMING: Let's do");
    lcd.setCursor(0, 1);
    lcd.print("smart things!");
	procTimer.initializeMs(1000, scanBus).start();
}
