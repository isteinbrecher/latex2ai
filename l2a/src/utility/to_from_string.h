// -----------------------------------------------------------------------------
// MIT License
//
// Copyright (c) 2020 Ivo Steinbrecher (https://github.com/stoani89/LaTeX2AI)
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
 * \brief Functions to convert enums from and to a string.
 */

#ifndef UTIL_TO_FROM_STRING_H_
#define UTIL_TO_FROM_STRING_H_



namespace L2A
{
    namespace UTIL
    {
        /**
         * \brief Convert a flag to a string.
         * @param flag_to_string_array Array with tuples containing the flag and the string representative of the flag.
         * @param flag Flag that should be converted to a string.
         * @param fail_on_default If no match can be found in the array, an error will be thrown.
         */
        template <typename Tflag, unsigned int n>
        ai::UnicodeString FlagToString(const std::array<std::tuple<Tflag, ai::UnicodeString>, n>& flag_to_string_array,
            const Tflag& flag, bool fail_on_default = true)
        {
            for (unsigned int i = 0; i < n; i++)
                if (std::get<0>(flag_to_string_array[i]) == flag) return std::get<1>(flag_to_string_array[i]);

            if (fail_on_default)
                throw L2A::ERR::Exception(ai::UnicodeString("FlagToString got unexpected flag!"));
            else
                return ai::UnicodeString("none");
        }

        /**
         * \brief Convert a string to flag.
         * @param flag_to_string_array Array with tuples containing the flag and the string representative of the flag.
         * @param flag String that should be converted to a flag.
         * @param fail_on_default If no match can be found in the array, an error will be thrown.
         */
        template <typename Tflag, unsigned int n>
        Tflag FlagFromString(const std::array<std::tuple<Tflag, ai::UnicodeString>, n>& flag_to_string_array,
            const ai::UnicodeString& string, bool fail_on_default = true)
        {
            for (unsigned int i = 0; i < n; i++)
                if (std::get<1>(flag_to_string_array[i]) == string) return std::get<0>(flag_to_string_array[i]);

            if (fail_on_default)
                throw L2A::ERR::Exception(ai::UnicodeString("FlagFromString got unexpected string " + string + "!"));
            else
                return Tflag::none;
        }

    }  // namespace UTIL
}  // namespace L2A

#endif
