/*
    Copyright (c) 2014  Steven Mead,
                        School of Computing,
                        University of Teesside,
                        Middlesbrough,
                        UK TS1 3BA
                        email: steven.j.mead@tees.ac.uk

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software
    in a product, an acknowledgment in the product documentation would be
    appreciated but is not required.

    2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

    3. This notice may not be removed or altered from any source
    distribution.
*/
#pragma once

#if !defined    LOG_HPP
#   define      LOG_HPP

#   include <prg/core/types.hpp>
#   include <fstream>
#   include <string>
#   include <sstream>
#   include <queue>
#   include <thread>
#   include <mutex>

namespace prg
{
    enum class ELogType : prg::byte
    {
        LT_Simple,
        LT_Info,
        LT_Warning,
        LT_Error,
        LT_Debug
    };


    /**
        \brief The base class of all log message types.
     */
    class Message final : public std::stringstream
    {
    public:
        /**
            \brief Constructor.
            \param eType an enumerated value indicating the message type.
        */
        explicit Message( ELogType eType = ELogType::LT_Info );
        /**
            \brief Copy constructor.
            \param copy The other Message object to copy.
        */
        Message( const Message& copy );

        /**
            \brief Destructor.
        */
        ~Message() = default;

        Message& operator=( const Message& copy );
        /**
            \brief getMessageType()
            \return The enumerated type of the message.
        */
        ELogType getMessageType() const
        {
            return e_message_type_;
        }

    private:
        ELogType        e_message_type_;
    };

    /**
        \class Log
        \brief A simple logging class (not threaded).
     */
    class Log final
    {
    public:
        /**
            \return Returns the singleton instance of the log class.
        */
        static Log& instance();

        /** \fn Log& setLogLevel( ELogType log_level = ELogType::LT_Info )
            \brief Sets the logging level.
            Set the logging levelany messages that have a log level less than the
            current logging level are ignored.
            \param log_level The log level value which is one of the values within ELogType
            \return A reference to the logging object (useful for daisy chaining).
        */
        Log& setLogLevel( ELogType log_level = ELogType::LT_Info )
        {
          log_level_ = log_level;
          return *this;
        }

        /** \fn Log& add( const Message& log_text )
            \brief Adds a new Message object.
            \return A reference to the logging object (useful for daisy chaining).
        */
        Log& add( const Message& log_text );

        /** \fn Log& simple( const std::string& info_message )
            \brief Adds a simple, unclassified message to the log
            \return A reference to the logging object (useful for daisy chaining).
        */
        Log& simple( const std::string& simple_message );

        /** \fn Log& info( const std::string& info_message )
            \brief Adds a simple info message to the log
            \return A reference to the logging object (useful for daisy chaining).
        */
        Log& info( const std::string& info_message );

        /** \fn Log& warn( const std::string& warning_message )
            \brief Adds a simple warning message to the log
            \return A reference to the logging object (useful for daisy chaining).
        */
        Log& warn( const std::string& warning_message );

        /** \fn Log& error( const std::string& error_message )
            \brief Adds a simple error message to the log
            \return A reference to the logging object (useful for daisy chaining).
        */
        Log& error( const std::string& error_message );

        /** \fn Log& debug( const std::string& error_message )
            \brief Adds a simple debug message to the log
            \return A reference to the logging object (useful for daisy chaining).
        */
        Log& debug( const std::string& debug_message );

        /** \fn Log& toConsole( bool to_console = false )
            \brief Changes whether messages are also output to standard output STDOUT (errors to STDERR).
            \return A reference to the logging object (useful for daisy chaining).
        */
        Log& toConsole( bool to_console = false );

    private:
        //Hidden Constructor/Destructor/Assignment operator
        Log();
        ~Log();
        Log( const Log& )                   = delete;
        Log( Log&& )                        = delete;
        Log& operator=( const Log& copy )   = delete;
        Log& operator=( Log&& copy )        = delete;

        static void write();

        //Data members
        ELogType              log_level_  { ELogType::LT_Info };
        std::ofstream				  file_       { "log.txt" };
        bool                  to_console_ { false };
        std::queue< Message > messages_;

        std::thread           write_log_thread_;
        std::mutex            write_log_mutex_;
        static bool           write_log_execute_;
    };

    extern Log& logger;

}//namespace prg

#endif // LOG_HPP
