/*
 *  The Mana World Server
 *  Copyright 2004 The Mana World Development Team
 *
 *  This file is part of The Mana World.
 *
 *  The Mana World  is free software; you can redistribute  it and/or modify it
 *  under the terms of the GNU General  Public License as published by the Free
 *  Software Foundation; either version 2 of the License, or any later version.
 *
 *  The Mana  World is  distributed in  the hope  that it  will be  useful, but
 *  WITHOUT ANY WARRANTY; without even  the implied warranty of MERCHANTABILITY
 *  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 *  more details.
 *
 *  You should  have received a  copy of the  GNU General Public  License along
 *  with The Mana  World; if not, write to the  Free Software Foundation, Inc.,
 *  59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 *  $Id$
 */


#ifndef _TMWSERV_LOGGER_H_
#define _TMWSERV_LOGGER_H_

#include <iosfwd>
#include <sstream>

namespace utils
{

/**
 * A very simple logger that writes messages to a log file.
 * If the log file is not set, the messages are routed to the standard output
 * or the standard error depending on the level of the message.
 * By default, the messages will be timestamped but the logger can be
 * configured to not prefix the messages with a timestamp.
 *
 * Limitations:
 *     - not thread-safe.
 *
 * Example of use:
 *
 * <pre>
 * \#include "logger.h"
 *
 * int main(void)
 * {
 *     using namespace utils;
 *
 *     Logger::setLogFile("/path/to/logfile");
 *
 *     // log messages using helper macros.
 *     LOG_DEBUG("level: " << 3)
 *     LOG_INFO("init sound")
 *     LOG_WARN("not implemented")
 *     LOG_ERROR("resource not found")
 *     LOG_FATAL("unable to init graphics")
 *
 *     // log messages using APIs.
 *     std::ostringstream os;
 *     os << "level: " << 3;
 *     Logger::output(os.str(), Logger::DEBUG);
 *
 *     Logger::output("init sound", Logger::INFO);
 *     Logger::output("not implemented", Logger::WARN);
 *     Logger::output("resource not found", Logger::ERROR);
 *     Logger::output("unable to init graphics", Logger::FATAL);
 *
 *     return 0;
 * }
 * </pre>
 */
class Logger
{
    public:
        enum Level
        {
            FATAL = 0,
            ERROR,
            WARN,
            INFO,
            DEBUG
        };

        /**
         * Sets the log file.
         *
         * This method will open the log file for writing, the former file
         * contents are removed.
         *
         * @param logFile the log file name (may include path).
         *
         * @exception std::ios::failure if the log file could not be opened.
         */
        static void setLogFile(std::string const &logFile);

        /**
         * Add/removes the timestamp.
         *
         * @param flag if true, a log message will always be timestamped
         *             (default = true).
         */
        static void setTimestamp(bool flag = true)
        { mHasTimestamp = flag; }

        /**
         * Sets tee mode.
         *
         * @param flag if true, write messages to both the standard (or error)
         *        output and the log file (if set) (default = true).
         */
        static void setTeeMode(bool flag = true)
        { mTeeMode = flag; }

        /**
         * Sets the verbosity level of the logger.
         *
         * @param verbosity is the level of verbosity.
         */
        static void setVerbosity(Level verbosity)
        { mVerbosity = verbosity; }

        /**
         * Logs a generic message.
         *
         * @param msg the message to log.
         *
         * @param atVerbosity the minimum verbosity level
         *        to log this
         *
         * @exception std::ios::failure.
         */
        static void output(std::string const &msg, Level atVerbosity);

        static Level mVerbosity;   /**< Verbosity level. */
    private:
        static bool mHasTimestamp; /**< Timestamp flag. */
        static bool mTeeMode;      /**< Tee mode flag. */

        /**
         * Logs a generic message.
         *
         * @param os the output stream.
         * @param msg the message to log.
         * @param prefix the message prefix.
         *
         * @exception std::ios::failure.
         */
        static void output(std::ostream &os, std::string const &msg, char const *prefix);
};


} // namespace utils

// HELPER MACROS

#define LOG(level, msg)                                                \
    do if (::utils::Logger::mVerbosity >= ::utils::Logger::level) {    \
        std::ostringstream os;                                          \
        os << msg;                                                      \
        ::utils::Logger::output(os.str(), ::utils::Logger::level);     \
    } while (0)

#define LOG_DEBUG(msg)  LOG(DEBUG, msg)
#define LOG_INFO(msg)   LOG(INFO, msg)
#define LOG_WARN(msg)   LOG(WARN, msg)
#define LOG_ERROR(msg)  LOG(ERROR, msg)
#define LOG_FATAL(msg)  LOG(FATAL, msg)

#endif // _TMWSERV_LOGGER_H_
