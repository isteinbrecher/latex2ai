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
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
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
 * \brief Errors for l2a applications.
 */


#include "IllustratorSDK.h"
#include "l2a_error.h"

#include "utility/string_functions.h"


L2A::ERR::Exception::Exception(
    const char* __file__, const int __line__, const char* __function__, const ai::UnicodeString& error_string)
    : ExceptionBase()
{
    ai::UnicodeString full_error_string(
        "--------------------------------------------------------------------------------\n");
    full_error_string += ai::UnicodeString(__file__);
    full_error_string += "::";
    char buff[100];
    snprintf(buff, sizeof(buff), "%d", __line__);
    full_error_string += ai::UnicodeString(buff);
    full_error_string += "\n";
    full_error_string += ai::UnicodeString(__function__);
    full_error_string += "\n--------------------------------------------------------------------------------\n\n";
    full_error_string += error_string;
    sAIUser->MessageAlert(full_error_string);
}
