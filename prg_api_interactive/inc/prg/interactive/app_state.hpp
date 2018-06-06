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

#if !defined    APP_STATE_HPP
#   define      APP_STATE_HPP

#   include <prg/core/types.hpp>
#   include <prg/core/image.hpp>
#   include <climits>

// MACRO TO ADD A VIRTUAL DESTRUCTOR
// ENSURE THE DEFAULT COMPILER DEFINED:
// - CONSTRUCTOR
// - COPY CONSTRUCTOR
// - MOVE CONSTRUCTOR
// - ASSIGNMENT OPERATOR (COPY)
// - ASSIGNMENT OPERATOR (MOVE, C++11)
// For more information see:
// page 518 of The C++ Programming Language (4th Edition) by Bjarne Stroustrup
#define     PRG_INTERFACE_CLASS_ONLY(class_name) \
                virtual ~class_name()                               {}\
                class_name()                                        = default;\
                class_name( const class_name &copy )                = default;\
                class_name( class_name &&move )                     = default;\
                class_name& operator=( const class_name &copy )     = default;\
                class_name& operator=( class_name &&move )          = default;
//*
//* EVENT HANDLING STATE CODES
//*
namespace prg
{

// ** Forward declarations
class Timer; //required by ITimerEvent

/**
    \class IMouseEvent
    \brief Interface that is inherited by classes that need to be notified of mouse events within the application
 */
class IMouseEvent
{
public:
  PRG_INTERFACE_CLASS_ONLY( IMouseEvent )

  /**
      \struct MouseEvent
      \brief A structure that is passed to the mouse event handler functions.
   */
  struct MouseEvent final {
    enum EMouseInfo : int {
      MB_NULL         = 0x0,

      // Mouse Button States
      MB_DOWN         = 0x01,
      MB_UP           = 0x02,

      // Mouse Button Identifiers
      LM_BTN          = 0x01,
      MM_BTN          = 0x02,
      RM_BTN          = 0x03
    };

    int         last_x          { 0 },	//Previous Position
                last_y          { 0 };

    int         pos_x           { 0 },	//Current Position
                pos_y           { 0 };

    void reset()
    {
      button = button_state = EMouseInfo::MB_NULL;
    }

    EMouseInfo  button          { EMouseInfo::MB_NULL },
                button_state    { EMouseInfo::MB_NULL };
  };
  /** \fn virtual bool onMotion( const MouseEvent& mouse_event ) = 0
      \brief Called when the mouse is moved but no mouse buttons are pressed.
      \param mouse_event An object of type IMouseEvent::MouseEvent that contains the current state of the mouse.
      \return true or false.  If false, then the mouse event stops at this object, that it is not passed on to any other IMouseEvent objects.
  */
  virtual
  bool
  onMotion( const MouseEvent& mouse_event ) = 0;

  /** \fn virtual bool onButton( const   MouseEvent& mouse_event ) = 0
      \brief Called when the mouse is moved but no mouse buttons are pressed.
      \param mouse_event An object of type IMouseEvent::MouseEvent that contains the current state of the mouse.
      \return true or false.  If false, then the mouse event stops at this object, that it is not passed on to any other IMouseEvent objects.
  */
  virtual
  bool
  onButton( const MouseEvent& mouse_event ) = 0;
};
/**
    \class IKeyEvent
    \brief Interface that is inherited by classes that need to be notified of keyboard events within the application
 */
class IKeyEvent
{
public:
  PRG_INTERFACE_CLASS_ONLY( IKeyEvent )

  /**
      \struct KeyEvent
      \brief A structure that is passed to the keyboard event handler functions.
   */
  struct KeyEvent final {
    enum EKeyInfo : int {
      KB_NULL         = 0x0,

      //Key States
      KB_DOWN         = 0x01,
      KB_UP           = 0x02,

      //Convenience - actual ASCII values if treated as type char
      KB_DEL_KEY      = 0x08,
      KB_TAB_KEY      = 0x09,
      KB_RET_KEY      = 0x0D,
      KB_ESC_KEY      = 0x1B,
      KB_SPC_KEY      = 0x20,

      // Function keys
      KB_F01_KEY      = (CHAR_MAX+1),
      KB_F02_KEY,
      KB_F03_KEY,
      KB_F04_KEY,
      KB_F05_KEY,
      KB_F06_KEY,
      KB_F07_KEY,
      KB_F08_KEY,
      KB_F09_KEY,
      KB_F10_KEY,
      KB_F11_KEY,
      KB_F12_KEY,

      //Arrow keys
      KB_LEFT_KEY,
      KB_UP_KEY,
      KB_RIGHT_KEY,
      KB_DOWN_KEY
    };

    enum EKeyModifier : unsigned short {
      KB_MOD_NULL   = 0x00,
      KB_MOD_LSHIFT = 0x01,
      KB_MOD_RSHIFT = 0x02,
      KB_MOD_LALT   = 0x04,
      KB_MOD_RALT   = 0x08,
      KB_MOD_LCTRL  = 0x10,
      KB_MOD_RCTRL  = 0x20,
      KB_MOD_ANY    = 0x8000
    };

    char toAscii() const
    {
      return ( key <= CHAR_MAX) ? static_cast<char>( key ) : static_cast<char>( EKeyInfo::KB_NULL );
    }

    void reset()
    {
      key = 0;
      key_state = EKeyInfo::KB_NULL;
    }

    bool modifierActive(const EKeyModifier modifier = EKeyModifier::KB_MOD_ANY) const;

    int         key                 { 0 };
    word        key_modifier_flags  { EKeyModifier::KB_MOD_NULL };
    EKeyInfo    key_state           { EKeyInfo::KB_NULL };

  };
  /** \fn virtual bool onKey( const   KeyEvent& key_event ) = 0
      \brief Called when a key is pressed on the computer's keyboard.
      \param key_event An object of type IKeyEvent::KeyEvent that contains the current state of the keyboard.
      \return true or false.  If false, then the mouse event stops at this object, that it is not passed on to any other IKeyEvent objects.
  */
  virtual
  bool
  onKey( const KeyEvent& key_event ) = 0;
};
/**
    \class ITimerEvent
    \brief Interface that is inherited by classes that need to be notified of timers that have expired within the application.
    \remarks Timer firing is approximate as the underlying Operating Systems are not real-time.
 */
class ITimerEvent
{
public:
  PRG_INTERFACE_CLASS_ONLY( ITimerEvent )

  /** \fn virtual void onTimer( Timer& timer ) = 0
      \brief Called when a user defined application timer expires.  A stub function implemenation is provided, which is called if the child class does not override it.
      \param timer An object of type 'Timer', which is a reference to the user's timer object.
      \return No return value.
  */
  virtual
  void
  onTimer( Timer& timer ) = 0;
};
/**
    \class IAppState
    \brief Interface that is inherited by classes that need to be notified of application events.
 */
class IAppState      //Inherits ITimerEvent interface
{
public:
  PRG_INTERFACE_CLASS_ONLY( IAppState )

  /** \fn  virtual bool onCreate() = 0
      \brief Called when the application starts to ensure that everything is initialised properly.
      \return true or false.  false if something went wrong.  Note, prefer the use of exceptions.
  */
  virtual
  bool
  onCreate() = 0;
  /** \fn virtual bool onDestroy() = 0
      \brief Called when the application ends to ensure that everything is cleaned up properly.
      \return true or false.  false if something went wrong.  Note, prefer the use of exceptions.
  */
  virtual
  bool
  onDestroy() = 0;
  /** \fn virtual void onEntry() = 0
      \brief Called everytime the application enters this application state.  This may happen more than once if the application changes state object.
      \return true or false.  false if something went wrong.  Note, prefer the use of exceptions.
  */
  virtual
  void
  onEntry() = 0;
  /** \fn virtual void onExit() = 0
      \brief Called everytime the application exits this application state.  This may happen more than once if the application changes state object.
      \return No return value.
  */
  virtual
  void
  onExit() = 0;
  /** \fn virtual void onUpdate() = 0
      \brief Called to update the application.
      \return No return value.
  */
  virtual
  void
  onUpdate() = 0;
  /** \fn virtual void onRender( Canvas&     canvas ) = 0
      \brief Called to prepare the screen buffer prior to output to the display window.
      \param canvas The Canvas (synonym for Image) object to draw to.
      \return No return value.
  */
  virtual
  void
  onRender( Canvas& canvas ) = 0;
};

} // namespace prg

#endif // APP_STATE_HPP
