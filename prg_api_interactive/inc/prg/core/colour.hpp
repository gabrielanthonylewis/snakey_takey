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

#if !defined    COLOUR_HPP
#   define      COLOUR_HPP

#   include <prg/core/types.hpp>

namespace prg
{

    class Image;

    struct RGBA
    {
        byte    r { 0 },
                g { 0 },
                b { 0 },
                a { 255 };
    };

    /**
        \class Colour
        \brief A class for manipulating colour values.
     */
    class Colour final
    {
    public:
        static const Colour &BLACK,
               &WHITE,
               &GREY,
               &RED,
               &GREEN,
               &BLUE,
               &YELLOW,
               &MAGENTA;
        /**
            \brief The Default Constructor
            \param r Red componant of the colour.
            \param g Green componant of the colour.
            \param b Blue componant of the colour.
            \param a Alpha componant of the colour.
        */
        Colour(
            const byte r = 0,
            const byte g = 0,
            const byte b = 0,
            const byte a = 255
        );

        /**
            \brief A constructor that takes an RGBA structure.
            \param rgba An RGBA structure used to set colour values.
        */
        explicit Colour( const RGBA& rgba );

        /**
            \param copy The object to copy from.
        */
        Colour( const Colour& copy );

        /**
            The destructor.
        */
        ~Colour();

        /** \fn Colour& operator=( const Colour& copy )
            \brief Assignment operator.  Used to set the colour.
            \param copy The Colour object to copy from.
            \return A reference to the object itself.
        */
        Colour& operator=( const Colour& copy );

        /** \fn Colour& operator=( const RGBA& copy )
            \brief Assignment operator.  Used to set the colour.
            \param copy An RGBA structure to set the colour values from.
            \return A reference to the object itself.
        */
        Colour& operator=( const RGBA& copy );

        /** \fn const RGBA& getRGBA() const
            \brief Returns the RGBA object.
            \return A reference to the internal RGBA itself.
        */
        const RGBA& getRGBA() const
        {
            return rgba_;
        }

        /** \fn const byte getR() const
            \brief Returns the red componant of the colour.
            \return a byte representing the red colour value.
        */
        const byte getR() const
        {
            return rgba_.r;
        }

        /** \fn const byte getG() const
            \brief Returns the green componant of the colour.
            \return a byte representing the green colour value.
        */
        const byte getG() const
        {
            return rgba_.g;
        }

        /** \fn const byte getB() const
            \brief Returns the blue componant of the colour.
            \return a byte representing the blue colour value.
        */
        const byte getB() const
        {
            return rgba_.b;
        }

        /** \fn const byte getA() const
            \brief Returns the alpha componant of the colour.
            \return a byte representing the alpha colour value.
        */
        const byte getA() const
        {
            return rgba_.a;
        }

        /** \fn Colour& setR( const byte r )
            \brief Sets the red componant of the colour.
            \return a reference to the Colour object itself.
        */
        Colour& setR( const byte r );

        /** \fn Colour& setG( const byte g )
            \brief Sets the green component of the colour.
            \return a reference to the Colour object itself.
        */
        Colour& setG( const byte g );

        /** \fn Colour& setB( const byte b )
            \brief Sets the blue component of the colour.
            \return a reference to the Colour object itself.
        */
        Colour& setB( const byte b );

        /** \fn Colour& setA( const byte a )
            \brief Sets the alpha component of the colour.
            \return a reference to the Colour object itself.
        */
        Colour& setA( const byte a );

    private:
        RGBA    rgba_;
    };

    Colour& operator<< ( Colour& col, RGBA& rgba );

    RGBA& operator<< ( RGBA& rgba, const Colour& col );

    inline bool operator== ( const Colour& col1, const Colour& col2 )
    {
        return (    col1.getR() == col2.getR() &&
                    col1.getG() == col2.getG() &&
                    col1.getB() == col2.getB()      );
    }

    inline bool operator== ( const Colour& col1, const RGBA& col2 )
    {
        return (    col1.getR() == col2.r &&
                    col1.getG() == col2.g &&
                    col1.getB() == col2.b           );
    }

}//namespace prg

#endif // COLOUR_HPP
