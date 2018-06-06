#pragma once

#if !defined    STRING_UTILITIES_HPP
#   define      STRING_UTILITIES_HPP

#   include <string>
#   include <sstream>

namespace prg
{

    std::string extractFileExtension(const std::string& fname);

    // No longer required with C++11 - Use the std::to_string() function instead.
    //
    template< class T > std::string toString( T& in )
    {
        std::ostringstream o;

        o << in;

        return o.str();
    }

    template< class T > std::string toString( T in )
    {
        std::ostringstream o;

        o << in;

        return o.str();
    }
}

#endif // STRING_UTILITIES_HPP
