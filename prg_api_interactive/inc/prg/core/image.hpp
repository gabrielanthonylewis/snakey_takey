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

/**
 * \file image.hpp
 * \author Steven Mead
 * \date 05-10-2009
 */
#pragma once

#if !defined    IMAGE_HPP
#   define      IMAGE_HPP

#   include <prg/core/types.hpp>
#   include <prg/core/colour.hpp>
#   include <string>
#   include <stdexcept>

namespace prg
{

    /** The forward declations of classes/structures that are used by the image class */
    class ImageImpl;
    class Font;
    struct RGBA;

    /**
        \class Image
        \brief A class for creating and manipulating images.

     */
    class Image
    {

    public:
        /**
            \class ImageException
         */
        class ImageException;

        /**
            \brief Default Constructor.
         */
        Image();

        /**
            \brief Constructor
            \param width The width, in pixels, of the image.
            \param height The height, in pixels, of the image.
            \param clear_to A colour to clear the image to when it's created, a default value of Colour(0,0,0,255) is provided.
         */
        Image(
            uint            width,
            uint            height,
            const Colour&   clear_to = { 0,0,0,255 }
        );

        /**
            \brief Destructor
         */
        virtual ~Image();

        /**
            \brief Copy Constructor.
            \param copy An existing instance of the image class.
         */
        Image( const Image& copy );

        /**
            \brief Move Constructor.
            \param Move an existing instance of the image class.
         */
        Image( Image&& move );

        /**
            \brief Assignment operator.
            \param copy An existing instance of the image class.
            \return a reference to current image object, useful for daisy chaining.
        */
        Image& operator= ( const Image& copy );

        /**
            \brief Move Assignment operator.
            \param move An existing instance of the image class.
            \return a reference to current image object, useful for daisy chaining.
        */
        Image& operator= ( Image&& move );

        /** \fn Image& clear( const Colour& colour )
            \brief Clears the image to a specified colour.
            \param colour The colour to clear the colour to.
            \return a reference to current image object, useful for daisy chaining.
        */
        Image&
        clear(
            const Colour& colour
        );

        /** \fn Image& setPixel( uint x, uint y, const Colour& colour )
            \brief Sets pixel(x, y) to the specified colour.
            \param x The x ordinate
            \param y The y ordinate
            \param colour The colour to clear the colour to.
            \return a reference to current image object, useful for daisy chaining.
        */
        Image&
        setPixel(
            const uint      x,
            const uint      y,
            const Colour&   colour
        );

        /** \fn Image& setOpacity( float opacity )
            \brief Sets the opacity of this image, which affects how it is blended over other images.
            \param opacity A value in the range 0.0 (fully transparent) to 1.0 (full opaque) specifying the opacity value.
            \return a reference to current image object, useful for daisy chaining.
        */
        Image&
        setOpacity(
            const float     opacity
        );

        /** \fn Image& setTransparentColour( const Colour& colour )
            \brief Specifies a colour that is completely invisible when drawing, useful for sprites etc.
            \param colour A Colour object specifying the colour that is transparent.
            \return a reference to current image object, useful for daisy chaining.
        */
        Image&
        setTransparentColour(
            const Colour&   colour
        );

        /** \fn Image& resize( uint new_width, uint new_height )
            \brief Resizes the image to the given image dimensions - Does not scale, image data
                   outside of the new width and height is lost.
            \param new_width An unsigned integer specifying the new width of the image in pixels.
            \param new_height An unsigned integer specifying the new height of the image in pixels.
            \return a reference to current image object, useful for daisy chaining.
        */
        Image&
        resize(
            const uint      new_width,
            const uint      new_height
        );

        /** \fn Colour getPixel( uint x, uint y ) const
            \brief Returns the colour at a specified pixel.
            \param x The x coordinate of the pixel to query.
            \param y The y coordinate of the pixel to query.
            \return A Colour object representing the colour at x, y.
        */
        Colour
        getPixel(
            const uint      x,
            const uint      y
        ) const;

        /** \fn uint getWidth() const
            \brief Returns the width (in pixels) of the image.
            \return An unsigned integer.
        */
        uint
        getWidth() const;

        /** \fn uint getHeight() const
            \brief Returns the height (in pixels) of the image.
            \return An unsigned integer.
        */
        uint
        getHeight() const;

        /** \fn uint getPixelCount() const
            \brief Returns the number of pixels in the image (basically width * height).
            \return An unsigned integer.
        */
        uint
        getPixelCount() const;
        /** \fn Image& getSubImage( Image& dest_image, const Colour& base_colour, uint src_x1, uint src_y1, uint src_x2, uint src_y2 )
            \brief Copies the contents of the source image onto the destination image provided.  The destination image is resized to the size of the width = x2 - x1 and height = y2 - y1.
            \param base_colour The colour that the destination image will be cleared to.
            \param src_x1 The left coordinate of source image to copy from.
            \param src_y1 The top coordinate of the source image to copy from.
            \param src_x2 The right coordinate of the source image to copy from.
            \param src_y2 The bottom coordinate of the source image to copy from.
            \return a new Image object containing the sub-image.
        */
        Image getSubImage(
            const Colour&   base_colour,
            uint  	        src_x1,
            uint            src_y1,
            uint            src_x2,
            uint            src_y2
        ) const;
        /** \fn Image& blit( const Image& src_image, int src_x1, int src_y1, int src_x2, int src_y2, int dest_x = 0, int dest_y = 0 )
            \brief Blits the source image over the image the method is being invoked on.  Attempts to blit the image to itself are ignored.
            \param src_image The image that is used to copy from.
            \param src_x1 The left coordinate of source image to copy from.
            \param src_y1 The bottom coordinate of the source image to copy from.
            \param src_x2 The right coordinate of the source image to copy from.
            \param src_y2 The top coordinate of the source image to copy from.
            \param dest_x The left position within the destination image to copy to (can be negative).
            \param dest_y The bottom position within the destinate image to copy to (can be negative).
            \return a reference to current image object, useful for daisy chaining.
        */
        Image&
        blit(
            const Image&    src_image,
            const int       src_x1,
            const int       src_y1,
            const int       src_x2,
            const int       src_y2,
            const int       dest_x = 0,
            const int       dest_y = 0
        );

        /** \fn Image& blit( const Image& src_image, int dest_x = 0, int dest_y = 0 )
            \brief Blits the source image over the image the method is being invoked on.  This copies the entire source image, which by default is placed at (0,0) in the destination image.
            \param src_image The image that is used to copy from
            \param dest_x The left position within the destination image to copy to (can be negative).
            \param dest_y The bottom position within the destinate image to copy to (can be negative).
            \return a reference to current image object, useful for daisy chaining.
        */
        Image&
        blit(
            const Image&    src_image,
            const int       dest_x = 0,
            const int       dest_y = 0
        );

        /** \fn Image& blitFast( const Image& src_image, int src_x1, int src_y1, int src_x2, int src_y2, int dest_x = 0, int dest_y = 0 )
            \brief Similar to blit with same signature, however alpha channel and/or transparency colour is ignored thus considerably faster.
            \param src_image The image that is used to copy from.
            \param src_x1 The left coordinate of source image to copy from.
            \param src_y1 The bottom coordinate of the source image to copy from.
            \param src_x2 The right coordinate of the source image to copy from.
            \param src_y2 The top coordinate of the source image to copy from.
            \param dest_x The left position within the destination image to copy to (can be negative).
            \param dest_y The bottom position within the destinate image to copy to (can be negative).
            \return a reference to current image object, useful for daisy chaining.
        */
        Image&
        blitFast(
            const Image&    src_image,
            const int       src_x1,
            const int       src_y1,
            const int       src_x2,
            const int       src_y2,
            const int       dest_x = 0,
            const int       dest_y = 0
        );

        /** \fn Image& blitFast( const Image& src_image, int dest_x = 0, int dest_y = 0 )
            \brief Similar to blit with same signature, however alpha channel and/or transparency colour is ignored thus considerably faster.
            \param src_image The image that is used to copy from
            \param dest_x The left position within the destination image to copy to (can be negative).
            \param dest_y The bottom position within the destinate image to copy to (can be negative).
            \return a reference to current image object, useful for daisy chaining.
        */
        prg::Image&
        blitFast(
            const Image&    src_image,
            const int       dest_x = 0,
            const int       dest_y = 0
        );

        /** \fn Image& drawLine( int x1, int y1, int x2, int y2, const Colour& colour )
            \brief Draws a coloured line between 2 points.
            \param x1 The starting X coordinate of the line.
            \param y1 The starting Y coordinate of the line.
            \param x2 The ending X coordinate of the line.
            \param y2 The ending Y coordinate of the line.
            \param colour The colour to draw the line.
            \return a reference to current image object, useful for daisy chaining.
        */
        Image&
        drawLine(
            const int       x1,
            const int       y1,
            const int       x2,
            const int       y2,
            const Colour&   colour
        );

        /** Image& drawCircle( int xc, int yc, int radius, const Colour& rgba )
             \brief Draws a hollow coloured circle.
             \param xc The X mid-point of the circle.
             \param yc The Y mid-point of the circle.
             \param radius The radius of the circle.
             \param rgba The colour to draw the circle.
             \return a reference to current image object, useful for daisy chaining.
        */
        Image&
        drawCircle(
            //center of circle
            const int       xc,
            const int       yc,
            //radius of circle
            const int       radius,
            //colour of the circle
            const Colour&   rgba
        );

    protected:
        friend class Font;
        ImageImpl* image_impl_ { nullptr };   //!<This is the real image object.

    };//class Image

    //Full definition of the ImageException class.
    //
    class Image::ImageException : public std::runtime_error
    {
    public:
        explicit ImageException( const std::string& error ) : std::runtime_error( error ) {}
    };

    /*! Creates an alias for the Image class */
    using Canvas = Image;   //a meaningful synonym for the Image class

}//namespace prg

#endif // IMAGE_HPP
