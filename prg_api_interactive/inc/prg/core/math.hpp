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

#if !defined    PRG_MATH_HPP
#   define      PRG_MATH_HPP

#   include <prg/core/types.hpp>
#   include <cmath>

namespace prg {

    namespace math {

        //Global Constants
        constexpr float PI              { 3.1415926535897932384626433832795F };
        constexpr float M_180_OVER_PI   { 180.F / PI };
        constexpr float M_PI_OVER_180   { PI / 180.F };

        //inline'd utility functions
        inline constexpr float deg_to_rad( const float deg ) { return deg * M_PI_OVER_180; }
        inline constexpr float rad_to_deg( const float rad ) { return rad * M_180_OVER_PI; }

    }

}//namespace prg

#endif // PRG_MATH_HPP
