/**
 * @file RF24StreamLogHandler.cpp
 *
 * Created on: 2 Oct 2020
 *     Author: Witold Markowski (wmarkow)
 *
 * Copyright (C)
 *      2020        Witold Markowski (wmarkow)
 *      2021        Brendan Doherty (2bndy5)
 *
 * This General Public License does not permit incorporating your program into
 * proprietary programs.  If your program is a subroutine library, you may
 * consider it more useful to permit linking proprietary applications with the
 * library.  If this is what you want to do, use the GNU Lesser General
 * Public License instead of this License.
 */
#include <Arduino.h>

#include "RF24StreamLogHandler.h"

/** description of the @ref ERROR base level */
const char rf24logLevelError[] = "ERROR";
/** description of the @ref WARN base level */
const char rf24logLevelWarn[] = " WARN";
/** description of the @ref INFO base level */
const char rf24logLevelInfo[] = " INFO";
/** description of the @ref DEBUG base level */
const char rf24logLevelDebug[] = "DEBUG";
/** collection of the base level descriptions */
const char *const rf24LogLevels[] = {rf24logLevelError,
                                     rf24logLevelWarn,
                                     rf24logLevelInfo,
                                     rf24logLevelDebug};

RF24StreamLogHandler::RF24StreamLogHandler(Print *stream)
{
    this->stream = stream;
}

void RF24StreamLogHandler::write(uint8_t logLevel,
                                 const char *vendorId,
                                 const char *message,
                                 va_list *args)
{
    appendTimestamp();
    stream->print(";");

    appendLogLevel(logLevel);
    stream->print(";");

    appendVendorId(vendorId);
    stream->print(";");

    // print formatted message
    appendFormattedMessage(message, args);
    stream->println("");
}

void RF24StreamLogHandler::appendTimestamp()
{
    char c[12];
    sprintf(c, "%10lu", millis());
    stream->print(c);
}

void RF24StreamLogHandler::appendLogLevel(uint8_t logLevel)
{
    uint8_t subLevel = logLevel & 0x07;

    if (logLevel >= RF24LogLevel::ERROR && logLevel <= RF24LogLevel::DEBUG + 7)
    {
        uint8_t logIndex = ((logLevel & 0x38) >> 3) - 1;
        stream->print(rf24LogLevels[logIndex]);

        if(subLevel == 0)
        {
            stream->print("  ");
        } else
        {
            stream->print(":");
            stream->print(subLevel);
        }

        return;
    }

    if (logLevel < 10)
    {
        stream->print("Lvl   ");
    }
    else if (logLevel < 100)
    {
        stream->print("Lvl  ");
    }
    else
    {
        stream->print("Lvl ");
    }
    stream->print(logLevel);
}

void RF24StreamLogHandler::appendVendorId(const char *vendorId)
{
    stream->print(vendorId);
}

void RF24StreamLogHandler::appendFormattedMessage(const char *format, va_list *args)
{
    for (; *format != 0; ++format)
    {
        if (*format == '%')
        {
            ++format;
            appendFormat(*format, args);
        }
        else
        {
            stream->print(*format);
        }
    }
}

void RF24StreamLogHandler::appendFormat(const char format, va_list *args)
{
    if (format == 's')
    {
        // print text from RAM
        register char *s = (char *)va_arg(*args, int);
        stream->print(s);

        return;
    }

    if (format == 'S')
    {
        // print text from FLASH
        register __FlashStringHelper *s = (__FlashStringHelper *)va_arg(*args, int);
        stream->print(s);

        return;
    }

    if (format == 'D' || format == 'F')
    {
        // print as double
        stream->print(va_arg(*args, double));

        return;
    }

    if (format == 'd' || format == 'i')
    {
        // print as integer
        stream->print(va_arg(*args, int), DEC);

        return;
    }

    stream->print(format);
}

#if defined (ARDUINO_ARCH_AVR)
void RF24StreamLogHandler::write(uint8_t logLevel,
                                 const __FlashStringHelper *vendorId,
                                 const __FlashStringHelper *message,
                                 va_list *args)
{
    appendTimestamp();
    stream->print(";");

    appendLogLevel(logLevel);
    stream->print(";");

    appendVendorId(vendorId);
    stream->print(";");

    // print formatted message
    appendFormattedMessage(message, args);
    stream->println("");
}

void RF24StreamLogHandler::appendVendorId(const __FlashStringHelper *vendorId)
{
    stream->print(vendorId);
}

void RF24StreamLogHandler::appendFormattedMessage(const __FlashStringHelper *format, va_list *args)
{
    PGM_P p = reinterpret_cast<PGM_P>(format);
    char c = pgm_read_byte(p++);
    for (; c != 0; c = pgm_read_byte(p++))
    {
        if (c == '%')
        {
            c = pgm_read_byte(p++);
            appendFormat(c, args);
        }
        else
        {
            stream->print(c);
        }
    }
}
#endif