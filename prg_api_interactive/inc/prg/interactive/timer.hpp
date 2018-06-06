/*
    Copyright (c) 2013  Steven Mead,
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

#if !defined    TIMER_HPP
#   define      TIMER_HPP

#   include <prg/core/types.hpp>
#   include <string>

namespace prg {

/** Forward declarations */
class ITimerEvent;
class TimerImpl;

/**
    \class Timer
    \brief A platform independent adapter class for handling timers.
 */
class Timer final {
    public:
        Timer();
        /**
            \brief Constructor for initialising a usable timer.
            \param timer_id A byte for timer ID that the owning application loop can use to to identify which timer has fired.
            \param ms An unsigned integer representing the number of milliseconds between timer firing, note this value will be clamped in the range 5 to UINT_MAX (see limits.h)
            \param timer_event A pointer to the event loop object whose onTimer function will be invoked upon the timer firing.
            \remark Overloaded constructor to create a timer object.
        */
        Timer(
            const ushort  timer_id,
            const uint    ms,
            ITimerEvent&  timer_event
        );

        /** Copy constructor - Not publically available.
         */
        Timer( const Timer& copy ) = delete;
        Timer( Timer&& move ) = delete;

        /**
            \brief Destructor.
        */
        ~Timer();
        /**
            \brief Assignment operator (Copy)
            \param copy The timer object to copy the timer information from.
        */
        Timer& operator= ( const Timer& copy );

        /**
            \brief Assignment operator (Move)
            \param move a timer object's internals to another timer object.
        */
        Timer& operator= ( Timer&& move );

        /** \fn bool running() const
            \brief Identifies if the timer is active or not.
            \return Bool indicating whether the timer is active.
        */
        bool running() const { return running_; }

        /** \fn void start()
            \brief Starts the timer and thus the invocation of IAppState's onTimer handler.
        */
        void start();
        /** \fn void stop()
            \brief Stop the timer and thus stops the invocation of the IAppState's onTimer handler.
        */
        void stop();

        /** \fn void reset()
            \brief Restarts the timer with a new interval.
            \param new_interval An unsigned integer representing the number of milliseconds between timer firing, note this value will be clamped in the range 5 to UINT_MAX (see limits.h)
        */
        void reset( const prg::uint new_interval );

        /** \fn const float deltaTime() const
            \brief Returns the number of seconds since the timer was last invoked.
            \return A float representing the number seconds as a fraction since the timer was last invoked.
        */
        const float
        deltaTime() const;

        /** \fn const float stopTime() const
            \brief Returns the number of seconds since the timer was started.
            \return A float representing the number seconds.
        */
        const float
        stopTime() const;
        /** \fn const ushort getID() const
            \brief Returns the ID of this timer.  This enables the invoking application to determine which timer has been fired.
            \return A byte representing the timer that has been fired.
        */
        const ushort
        getID() const;
        /** \fn static const float sysDeltaTime()
            \brief Returns the time in seconds since the application last updated.
            \return A float representing the delta time of the application.
        */
		static
        const float
        sysDeltaTime();

        /** \fn static const float sysUpTime()
            \brief Returns the time in seconds since the application started.
            \return A float representing the time since the application started.
        */
		static
        const float
        sysUpTime();

        /** \fn static std::string getDateTime()
            \brief Gets the current date and time and returns it as a C++ string object.
            \return std::string object populated with the date and time.
        */
        static
        std::string
        getDateTime();

    private:
        friend class TimerImpl;
        ushort              id_                 { 0 },
                            internal_id_        { 0 };
        bool                running_            { false };
        ITimerEvent*        timer_event_        { nullptr };
        TimerImpl*          timer_impl_         { nullptr };

};

} // namespace prg

#endif // TIMER_HPP
