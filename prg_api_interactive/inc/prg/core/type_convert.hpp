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

#if !defined    TYPE_CONVERT_HPP
#   define      TYPE_CONVERT_HPP

#   include <iostream>
#   include <string>
#   include <sstream>

using namespace std;

namespace prg
{

    template< typename t_from = long, typename t_to = std::string >
    class type_convert final
    {
    public:
        type_convert() = delete;
        type_convert& operator=( const type_convert& ) = delete;

        type_convert( const t_from& from )
        {
            do_convert(from);
        }

        type_convert& operator=( const t_from& from )
        {
            do_convert(from);
            return *this;
        }

        t_to& operator*()
        {
            return to_;
        }

        friend ostream& operator<<( ostream& o, const type_convert<t_from, t_to>& c )
        {
            return o << c.to_;
        }

    private:
        t_to& do_convert(const t_from& from)
        {
            std::stringstream s;

            s << from;

            s >> to_;

            return to_;
        }

        t_to  to_;
    };

}//namespace prg

#endif // TYPE_CONVERT_HPP
