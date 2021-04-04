/**
 * @file RF24Log.h
 *
 * Copyright (C)
 *  2021        Brendan Doherty (2bndy5)
 *
 * This General Public License does not permit incorporating your program into
 * proprietary programs.  If your program is a subroutine library, you may
 * consider it more useful to permit linking proprietary applications with the
 * library.  If this is what you want to do, use the GNU Lesser General
 * Public License instead of this License.
 */

#ifndef LOGGING_H
#define LOGGING_H

#include "RF24Log_config.h"

/* macros for testing
#define ARDUINO // tricking Intellisense
 */

static const PROGMEM char* const levelDesc[4] = {"DEBUG", "INFO", "WARN", "ERROR"};

/**
 * @defgroup logLevels Logging Levels
 * These values are used to delimit certain levels of logging messages
 * @{
 */
/** This level is the default and will output **no** messages of any level */
#define NOT_SET 0
/** This level is for debugging outputs and will output messages of @ref DEBUG, @ref INFO, @ref WARN, & @ref ERROR levels */
#define DEBUG   10
/** This level is for informative outputs and will output messages of @ref INFO, @ref WARN, & @ref ERROR levels */
#define INFO    20
/** This level is for warning outputs and will output messages of @ref WARN & @ref ERROR levels */
#define WARN    30
/** This level is for error outputs and will output messages of only @ref ERROR levels */
#define ERROR   40

/**
 * @}
 * A templated class for implementing a single logging object.
 * Copied @ref logging objects (using RF24Logger(RF24Logger*)) will re-use
 * the original handler and level.
 * @see the pre-instantiated @ref logging variable
 */
template <typename StreamType>
class RF24Logger
{
private:

    uint8_t level; /** the logging level used to filter logging messages */
    StreamType* handler; /** the output stream */

public:

    /** Empty constructor. level defaults to @ref NOT_SET, and instance has no handler. */
    RF24Logger() : level(NOT_SET), handler(nullptr)
    {
    }

    /**
     * Instance constructor. The , and the instance's
     * handler is initialized.
     * @param stream The handler to which all logging output will be directed.
     * @param lvl The instance's logging level defaults to @ref NOT_SET if not specified.
     */
    RF24Logger(StreamType* stream, uint8_t lvl = NOT_SET) : level(lvl), handler(stream)
    {
    }

    /**
     * Copy constructor. Instance's log level and handler are set to @p obj instance's
     * corresponding values.
     * @param obj An instantiated RF24Logger object from which values are copied from.
     */
    RF24Logger(RF24Logger* obj) : level(obj->level), handler(obj->handler)
    {
    }

    /**
     * Set the handler to which all logging messages are directed.
     * @param stream The output stream to be used as the handler.
     */
    void setHandler(StreamType* stream)
    {
        handler = stream;
    }

    /**
     * Set the logging level that's to filter logging messages passed to log()
     * @param lvl The logging level must be in range [0, 60).
     */
    void setLevel(uint8_t lvl)
    {
        level = rf24_min(lvl, ERROR + 9);
    }

    /**
     * @brief Log a message
     * @param lvl The logging level used for the specified message.
     * @param origin The orgin of the message.
     * @param msg The specified message.
     */
    template <typename St, typename... Ts>
    void log(uint8_t lvl, St origin, Ts... msg)
    {
        if ((lvl < level && level) || handler == nullptr)
            return;

        *handler
#ifdef ARDUINO
        << millis()
#else // !defined(ARDUINO)
        << timestamp()
#endif // !defined(ARDUINO)
        << ':';

        if (lvl % 10 == 0)
            *handler << (char*)pgm_read_ptr(&levelDesc[lvl / 10 - 1]) << ':';
        else
            *handler << "Lvl " << lvl << ':';

        *handler << origin << ": ";
        outputData(msg...) << endl;
    }

    /**
     * @brief output a @ref INFO level message
     * @param msg The message to output.
     */
    template <typename St, typename... Ts>
    void info(St origin, Ts... msg)
    {
        log(INFO, origin, msg...);
    }

    /**
     * @brief output a @ref DEBUG level message
     * @param msg The message to output.
     */
    template <typename St, typename... Ts>
    void debug(St origin, Ts... msg)
    {
        log(DEBUG, origin, msg...);
    }

    /**
     * @brief output a @ref WARN level message
     * @param msg The message to output.
     */
    template <typename St, typename... Ts>
    void warn(St origin, Ts... msg)
    {
        log(WARN, origin, msg...);
    }

    /**
     * @brief output a @ref ERROR level message
     * @param msg The message to output.
     */
    template <typename St, typename... Ts>
    void error(St origin, Ts... msg)
    {
        log(ERROR, origin, msg...);
    }

protected:

#ifndef ARDUINO
    /**
     * @brief output a timestamp for the proceeding message
     * @note this is for non-Arduino platforms
     * @returns A c-string of the current time
     */
    char* timestamp()
    {
        time_t now;
        time(&now);
        return ctime(now);
    }
#endif

    /**
     * @brief output an arbitrary type of data
     *
     * This is our base case for parameter unpacking
     * @param data The data to output
     * @returns The instantiated output stream object passed to setHandler()
     */
    template <typename Tdata>
    StreamType &outputData(Tdata data)
    {
        *handler << data;
        return *handler;
    }

    /**
     * @brief output an arbitrary type of data
     *
     * This is our recursive scenario for parameter unpacking
     * @param data The data to output
     * @param rest the pack of sequential parameters
     * @returns The instantiated output stream object passed to setHandler()
     */
    template <typename Tdata, typename... Rest>
    StreamType &outputData(Tdata data, Rest... rest)
    {
        *handler << data;
        return outputData(rest...);
    }

};

/** @brief A convenient variable to get started quickly on supported platforms */
RF24Logger<Stream_t> logging;

#endif // LOGGING_H

/**
 * @example{lineno} BasicLogger.ino
 * A basic logging example using RF24Log library
 */

/**
 * @example{lineno} MultipleLoggers.ino
 * A example using RF24Log library to log multiple "libraries" (basic class structures
 * in this example). We expect that only 1 of the libraries will actually print
 * something during `loop()` because we filter out the logging from the MyLib2 object
 * using a different/higher logging level (specifically set by the MyLib2 class' c'tor).
 */