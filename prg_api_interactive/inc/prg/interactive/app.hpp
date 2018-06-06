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

#if !defined    APP_HPP
#   define      APP_HPP

#include <stdexcept>
#include <prg/core/types.hpp>
#include <prg/interactive/app_state.hpp>
#include <memory>

namespace prg {

// ** Forward declaration of prg classes **
class AppImpl;
class Colour;
class Font;
class IKeyEvent;
class IMouseEvent;
class IAppState;

using std::string;
/**
    \class App
    \brief The application class is responsible for the underlying Windowing API (currently Windows and XWindows).
    \remarks For your convenience a system-wide global reference is created whose identifier is \b application.
 */
class App final {

    public:

        class AppException;

        /** \fn App& instance()
            \brief App is a 'Singleton' class, instance returns a reference to a single instance of the App class.
            \return A reference to the singleton App instance.
         */
        static
        App& instance();
        /** \fn App& addState( const std::string& state_name, IAppState& app_state )
            \brief Registers an IAppState object with the application.
            \param state_name The name associated with the IAppState object.
            \param app_state The IAppState object to register with the application.
            \return The reference to the App object itself, useful for daisy chaining.
            \remarks \li This function registers instances of classes that are derived from the IAppState interface class with the application.\li Only one IAppState can be used at any one time, but many can be registered with the application for use, see setState on how to change the state object during the execution of the application.
         */
        App&
        addState(
            const std::string&  state_name,
            IAppState&          app_state
        );
        /** \fn bool setState( const std::string& state_name )
            \brief Sets the application IAppState object to be used by the application.
            \param state_name The name given by the client code to refer to the application state object.
            \return true or false - true if the named event loop was successfully changed, false otherwise.
            \remarks When more than one IAppState object is registered with the application you may wish to change to another, for instance moving from a menu system state to the main application state.  This function will change the application state object specified by the name registered with the addState function.
         */
        bool
        setState(
            const std::string& state_name
        );
        /** \fn void setClearColour( const Colour& colour )
            \brief Sets the colour that the background is cleared to before rendering occurs.
         */
        void
        setClearColour(
            const Colour& colour
        );
        /** \fn void setMousePosition( uint x, uint y )
            \brief Moves the mouse cursor to a specified position within the application window.
            \param x The specified x position
            \param y The specified y position
         */
        void
        setMousePosition(
            const uint x,
            const uint y
        );
        /** \fn void setMouseVisibility( bool visible = true )
            \brief Hides or Shows the mouse cursor.
            \param visible If true the mouse cursor is visible, false the mouse cursor is hidden.
         */
        void
        setMouseVisibility(
            const bool visible = true
        );
        /** \fn void takeScreenShot( const std::string& file_name = "screenshot.bmp" )
            \brief Saves the current screen buffer to an image file and saves it in the working directory of the application.
            \param file_name The output file name to save the file as, if none provided then the default name of "screenshot.bmp" will be used.
         */
        void
        takeScreenShot(
            const std::string& file_name = "screenshot.bmp"
        ) const;
        /** \fn bool run( uint width, uint height, const   string& app_name, const string& app_author )
            \brief Run the application main loop.
            \param app_name A string that contains the application name that appears in the title bar of the window.
            \param app_author A string that contains the name of the author of the application.
            \param width An unsigned integer representing the width (in pixels) of the application window.
            \param height An unsigned integer representing the height (in pixels) of the application window.
            \param full_screen A Boolean indicating to go full screen mode (Windows only, Linux no effect).
            \return A boolean indicating whether the application had terminated correctly.
         */
        bool
        run(
            const string& app_name,
            const string& app_author,
            const uint    width,
            const uint    height,
            const bool    full_screen = false
        );
        /** \fn void exit()
            \brief Requests that the application terminates.
         */
        void
        exit();
        /** \fn const uint getScreenWidth() const
            \brief Queries the width of the application screen/viewport in pixels.
            \return An unsigned integer with the screen/viewport width in pixels.
         */
        const uint
        getScreenWidth() const;
        /** \fn const uint getScreenHeight() const
            \brief Queries the height of the application screen/viewport in pixels.
            \return An unsigned integer with the screen/viewport height in pixels.
         */
        const uint
        getScreenHeight() const;
        /** \fn const IMouseEvent::MouseEvent& getLastMouseEvent() const
            \brief Returns the last mouse event.
         */
        const IMouseEvent::MouseEvent&
        getLastMouseEvent() const;
        /** \fn const IKeyEvent::KeyEvent& getLastKeyEvent() const
            \brief Returns the last keyboard event.
         */
        const IKeyEvent::KeyEvent&
        getLastKeyEvent() const;
        /** \fn void addMouseListener( IMouseEvent& me )
            \brief Adds an object derived from IMouseEvent to the list of listeners of mouse events,
            \param me A reference to an object derived from IMouseEvent that needs to be notifed of mouse events.
         */
        void
        addMouseListener( IMouseEvent& me );
        /** \fn void addKeyListener( IKeyEvent& ke )
            \brief Adds an object derived from IKeyEvent to the list of listeners of keyboard events,
            \param ke A reference to an object derived from IKeyEvent that needs to be notifed of keyboard events.
         */
        void
        addKeyListener( IKeyEvent& ke );
        /** \fn void removeMouseListener( IMouseEvent& me )
            \brief Removes a previously registered IMouseEvent object from the mouse event listeners list.
            \param me A reference to an object derived from IMouseEvent that no longer requires mouse event notification.
            \remarks \li 1) If 'me' is not in the listener list, the request is ignored.  No error code or message is returned.
         */
        void
        removeMouseListener( IMouseEvent& me );
        /** \fn void removeKeyListener( IKeyEvent& ke )
            \brief Removes a previous registered IKeyEvent object from the key event listeners list.
            \param ke A reference to an object derived from IKeyEvent that no longer requires key event notification.
            \remarks \li If 'ke' is not in the listeners list, the request is ignored.  No error code or message is reported.
         */
        void
        removeKeyListener( IKeyEvent& ke );
		/** \fn bool isQuiting() const
			\brief Indicates if the application is quiting.
			\return true (the application is running, false otherwise).
		 */
		bool
		isQuiting() const;
		/** \fn double upTime()
			\brief Returns the applications execution 'up' time.
			\return A double indicating the number of seconds and milliseconds the application has been running.
		*/
		double
		upTime();

    private:
        App();
        ~App();
        App( const App& )               = delete;
        App( App&& )                    = delete; //Move Constructor
        App& operator=( const App& )    = delete;
        App& operator=( App&& )         = delete; //Move assignment

        std::unique_ptr<AppImpl> app_impl_;
};

//Full definition of the AppException class.
//
class App::AppException : public std::runtime_error {
    public:
        explicit AppException( const std::string& error ) : std::runtime_error( error ) {}
};

/** \var application
    \brief Handy global reference to the instance of App, to avoid having to write App::instance() all the time.
 */
extern App&	application;

}//namespace App

#endif // APP_HPP
