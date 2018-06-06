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

#if !defined    TYPES_HPP
#   define      TYPES_HPP

namespace prg {

    using uchar     = unsigned char;
    using ushort    = unsigned short;
    using uint      = unsigned int;
    using ulong     = unsigned long;

    using byte      = uchar;
    using word      = ushort;
    using dword     = uint;

    struct Point2D final {    //primitive 2D coodinate type
        int x_ { 0 },
            y_ { 0 };

        Point2D() = default;
        Point2D( const int x, const int y ) : x_( x ), y_( y ) {}
    };

    template<class T>
    T clamp( T val, const T min = 0, const T max = 1 )
    {
        if(val < min) val = min;
        else if(val > max) val = max;

        return val;
    }

}//namespace prg



#endif // TYPES_HPP
