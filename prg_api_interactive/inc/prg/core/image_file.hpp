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

#if !defined    IMAGE_FILE_HPP
#   define         IMAGE_FILE_HPP

#include <string>
#include <stdexcept>

namespace prg
{

    //forward declaration of GfxImageBuffer class
    class Image;
    class ImageFileImpl;

    /**
        \class ImageFile
        \brief A class for loading and saving different image files types into an image object.

     */
    class ImageFile final
    {
    public:
        /**
            \class ImageFileException
            \brief Exceptions for ImageFile.
         */
        class ImageFileException;

        /**
            \brief Constructor.
            \param fname A string containing the name of the file to load.
        */
        explicit ImageFile(
            const std::string& fname
        );
        /**
            \brief Destructor.
         */
        ~ImageFile();

        /** \fn void load( Image& load_into ) const
            \brief Loads the specified image into an image.
            \param load_into The image to the load the image data into.
        */
        void load( Image& load_into ) const;

        /** \fn Image load() const
            \brief Loads and returns an image using C++11 move semantics.
            \return an image containing the pixel data.
        */
        Image load() const;

        /** \fn void save( const Image& save_from ) const
            \brief Saves the image to the specified file.
            \param save_from The image to take the data from to save to the file.
        */
        void save( const Image& save_from ) const;

        /** \fn void reset( const std::string& fname )
            \brief Change the name of the file used by the ImageFile object.
            \param fname The new file name to be used by the ImageFile object.
        */
        void reset( const std::string& fname );

        /** \fn const std::string& getFileName() const
            \brief Returns the name of the file currently used by the file.
            \return The name of the file.
        */
        const std::string& getFileName() const;

    private:
        ImageFile( const ImageFile& ) = delete;

        ImageFile operator=( const ImageFile& ) = delete;

        ImageFileImpl*      image_file_impl_ { nullptr };
    };

    class ImageFile::ImageFileException : public std::runtime_error
    {
    public:
        explicit ImageFileException( const std::string& error ) : std::runtime_error( error ) {}
    };

}//prg

#endif//IMAGE_FILE_HPP
