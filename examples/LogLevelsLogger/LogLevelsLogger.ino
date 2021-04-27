/**
 * Created on: 31 Mar 2021
 *     Author: Witold Markowski (wmarkow)
 *
 * Copyright (C)
 *    2021        Witold Markowski (wmarkow)
 *
 * This General Public License does not permit incorporating your program into
 * proprietary programs.  If your program is a subroutine library, you may
 * consider it more useful to permit linking proprietary applications with the
 * library.  If this is what you want to do, use the GNU Lesser General
 * Public License instead of this License.
 */

#include <Arduino.h>
#include <string.h>

#include <RF24Logger.h>
#include <RF24Loggers/ArduinoPrintLogger.h>

// Create hardware serial port log appender
ArduinoPrintLogger rf24SerialLogHandler(&Serial);

// Define global vendor id (it is stored in FLASH memory)
const char PROGMEM vendorID[] = "RF24LogExample";

void setup()
{
  // configure serial port baudrate
  Serial.begin(115200);

  // set maximal log level to ALL
  rf24SerialLogHandler.setLogLevel(RF24LogLevel::ALL);
  // set serial port appender
  rf24Logger.setHandler(&rf24SerialLogHandler);

  RF24LOGGER_info(vendorID, "RF24Log/examples/LogLevelsLogger");
}

void loop()
{
  if (Serial.available()) {
    char input = Serial.parseInt();
    Serial.print("Set log level (in octal) to ");
    Serial.println(input, OCT);
    rf24SerialLogHandler.setLogLevel(input);
  }

  RF24LOGGER_log(1, vendorID, "this is a custom sublevel without a domain.");
  RF24LOGGER_error(vendorID, "Error message");
  RF24LOGGER_log(RF24LogLevel::ERROR + 1, vendorID, "Error message sub-level 1");
  RF24LOGGER_log(RF24LogLevel::ERROR + 7, vendorID, "Error message sub-level 7");

  RF24LOGGER_warn(vendorID, "Warning message");
  RF24LOGGER_log(RF24LogLevel::WARN + 1, vendorID, "Warning message sub-level 1");
  RF24LOGGER_log(RF24LogLevel::WARN + 7, vendorID, "Warning message sub-level 7");

  RF24LOGGER_info(vendorID, "Info message");
  RF24LOGGER_log(RF24LogLevel::INFO + 1, vendorID, "Info message sub-level 1");
  RF24LOGGER_log(RF24LogLevel::INFO + 7, vendorID, "Info message sub-level 7");

  RF24LOGGER_debug(vendorID, "Debug message");
  RF24LOGGER_log(RF24LogLevel::DEBUG + 1, vendorID, "Debug message sub-level 1");
  RF24LOGGER_log(RF24LogLevel::DEBUG + 7, vendorID, "Debug message sub-level 7");

  RF24LOGGER_warn(vendorID, "wierd order? Its in octal!");
  RF24LOGGER_log(RF24LogLevel::ERROR - 1, vendorID, "This is not an Error message");
  RF24LOGGER_log(RF24LogLevel::DEBUG + 8, vendorID, "This is not a Debug message");
  RF24LOGGER_log(0x75, vendorID, "This is level 0x75");

  Serial.println("");
  delay(5000);
}
