// -----------------------------------------------------------------------------
// MIT License
//
// Copyright (c) 2020-2025 Ivo Steinbrecher
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
// -----------------------------------------------------------------------------

/**
 * \brief Utility math functions.
 */

#ifndef UTIL_MATH_
#define UTIL_MATH_


#include "IllustratorSDK.h"


/**
 * \brief Overload operators for AIRealPoints.
 */
inline AIRealPoint operator-(const AIRealPoint& point)
{
    AIRealPoint return_point;
    return_point.h = -point.h;
    return_point.v = -point.v;
    return return_point;
}
inline AIRealPoint operator+(const AIRealPoint& lhs, const AIRealPoint& rhs)
{
    AIRealPoint return_point(lhs);
    return_point.h += rhs.h;
    return_point.v += rhs.v;
    return return_point;
}
inline AIRealPoint operator-(const AIRealPoint& lhs, const AIRealPoint& rhs)
{
    AIRealPoint return_point(lhs);
    return_point.h -= rhs.h;
    return_point.v -= rhs.v;
    return return_point;
}


namespace L2A
{
    namespace UTIL
    {
        namespace MATH
        { /**
           * \brief Calculate the norm of a vector.
           */
            AIReal GetNorm(const AIRealPoint& point);

            /**
             * \brief Calculate the distance between two points.
             */
            AIReal GetDistance(const AIRealPoint& point_a, const AIRealPoint& point_b);
        }  // namespace MATH
    }  // namespace UTIL
}  // namespace L2A


#endif
