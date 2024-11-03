// -----------------------------------------------------------------------------
// MIT License
//
// Copyright (c) 2020 Ivo Steinbrecher
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
 * \brief Functions to check the version of LaTeX2AI.
 */


#include "IllustratorSDK.h"
#include "l2a_version.h"

#include "l2a_constants.h"
#include "l2a_error/l2a_error.h"
#include "utility/string_functions.h"
#include "utility/file_system.h"
#include "../tpl/json/single_include/nlohmann/json.hpp"


/**
 *
 */
L2A::GLOBAL::Version::Version(std::string version_string) : version_(0)
{
    // The github versions have a prefix "v" -> remove this.
    if (version_string.at(0) == 'v') version_string = version_string.substr(1, version_string.size() - 1);

    // Split at each "."
    unsigned int version_type = 0;
    size_t pos = 0;
    size_t pos_old = 0;
    while (true)
    {
        pos_old = pos;
        pos = version_string.find('.', pos_old);
        if (pos != string::npos)
            SetVersion(std::stoi(version_string.substr(pos_old, pos - pos_old)), version_type);
        else
        {
            SetVersion(std::stoi(version_string.substr(pos_old, version_string.size() - pos_old)), version_type);
            break;
        }
        pos++;
        version_type++;
    }
}

/**
 *
 */
L2A::GLOBAL::Version::Version(unsigned int version) : version_(version)
{
    // Check that no unwanted data was given.
    if ((version_ & (0xff << (8 * 3))) != 0) l2a_error("Version number contains data out of range!");
}

/**
 * \brief Convert the version to a string.
 */
ai::UnicodeString L2A::GLOBAL::Version::ToString() const
{
    ai::UnicodeString return_string("");
    for (unsigned int version_type = 0; version_type < 3; version_type++)
    {
        return_string +=
            L2A::UTIL::IntegerToString((version_ & (0xff << (8 * (2 - version_type)))) >> (8 * (2 - version_type)));
        if (version_type != 2) return_string += ".";
    }
    return return_string;
}

/**
 *
 */
void L2A::GLOBAL::Version::SetVersion(const unsigned int version, const size_t version_type)
{
    // Check that the version is not larger than 255 -> max number that can be stored.
    if (version > 255) l2a_error("Version number can be a maximum of 256");

    // Clear the relevant bits.
    version_ &= ~(0xff << 8 * (2 - version_type));

    // Set the current bits.
    version_ |= ((version & 0xff) << 8 * (2 - version_type));
}
