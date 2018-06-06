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

#if !defined    FONT_HPP
#   define		FONT_HPP

#   include <string>
#   include <prg/core/types.hpp>

namespace prg
{

    class Colour;
    class Image;
    class FontImpl;

    /**
        \class Font
        \brief A class for handling bitmap fonts.
     */
    class Font final
    {
    public:
        static const Font   &MINUTE,
                            &TINY,
                            &SMALL,
                            &MEDIUM,
                            &BIG,
                            &LARGE,
                            &MASSIVE;

        Font() = delete;

        /**
            \brief Constructor.
            \param font_name The name of the font to convert to a bitmapped font.
            \param height The height of the font in pixels.
        */
        Font(
            const std::string& font_name,
            uint height = 8
        );

        /**
            \brief Copy Constructor.
            \param copy The font object to copy from.
        */
        Font( const Font& copy );

        /**
            \brief Move Constructor.
            \param move the font object whose internals are to be moved to another font object.
        */
        Font( Font&& move );

        /**
            \brief Destructor.
        */
        ~Font();
        /**
            \brief Assignment operator (Copy).
            \param copy The font object to copy from.
        */
        Font& operator=( const Font& copy );

        /**
            \brief Assignment operator (Move).
            \param move the font object whose internals are to be moved to another font object.
        */
        Font& operator=( Font&& move );

        /** \fn void print( Image& image, int x, int y, const Colour& colour, const std::string& text ) const
            \brief Renders a string of characters to an image.
            \param image The image that the rasterised text is rendered to.
            \param x The x coordinate in the image to place the rendered text.
            \param y The y coordinate in the image to place the rendered text.
            \param colour The colour to render the rasterised text.
            \param text The text to render.
        */
        void
        print(
            Image& image,
            const int           x,
            const int           y,
            const Colour&       colour,
            const std::string&  text,
            const float         blend_value = 1.F
        ) const;

        /** \fn uint* computePrintDimensions( uint dims[2], const std::string& text ) const
            \brief Computes the width and height that the specified string would render to.
            \param dims A 1D array of 2 elements to receive the width (element 0) and height (element 1) into.
            \param text A constant string to compute the width and height for.
            \return The array passed into the function, as a pointer to the first element of the array.
            \remarks Note, there is currently a bug that the height of all letters with descenders (e.g. 'g' and 'p') below the baseline are not treated correctly.
        */
        uint*
        computePrintDimensions(
            uint    dims[2],
            const   std::string& text
        ) const;

    private:
        FontImpl* font_impl_ { nullptr };
    };

}//namespace prg

#endif // FONT_HPP
