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
 * \brief Errors for l2a applications.
 */


#ifndef L2A_ERROR_H_
#define L2A_ERROR_H_


namespace L2A
{
    namespace ERR
    {
        /**
         * \brief Base class for L2A Errors.
         */
        class ExceptionBase : public exception
        {
           public:
            /**
             * \brief Constructor with error string and detailed information about the error.
             */
            ExceptionBase() {};
        };

        /**
         * \brief Class for L2A Errors.
         */
        class Exception : public ExceptionBase
        {
           public:
            /**
             * \brief Constructor with error string and detailed information about the error.
             */
            Exception(const char* __file__, const int __line__, const char* __function__,
                const ai::UnicodeString& error_string, const bool display_error_message = true);
        };

        /**
         * \brief Class for L2A Warnings.
         *
         * This class is intended for events that should be displayed to the user but are not considered as an error.
         */
        class Warning : public ExceptionBase
        {
           public:
            /**
             * \brief Constructor with warning string.
             */
            Warning(const ai::UnicodeString& warning_string) { sAIUser->MessageAlert(warning_string); };
        };

        /**
         * \brief Convert the Illustrator integer error to 4 characters.
         */
        ai::UnicodeString AIErrorCodeToString(const int& err);

        /**
         * \brief Convert the Illustrator integer error to 4 characters.
         */
        ai::UnicodeString AIErrorCodeToErrorString(const int& err);
    }  // namespace ERR
}  // namespace L2A


/**
 * \brief This macro throws an error and adds information where in the code the error happens.
 */
#define l2a_error(error_string) \
    throw L2A::ERR::Exception(__FILE__, __LINE__, __FUNCTION__, ai::UnicodeString(error_string))

/**
 * \brief This macro throws an that only displays the error message in the debug mode.
 */
#if _DEBUG
#define l2a_error_quiet(error_string) l2a_error(error_string)
#else
#define l2a_error_quiet(error_string) \
    throw L2A::ERR::Exception(__FILE__, __LINE__, __FUNCTION__, ai::UnicodeString(error_string), false)
#endif

/**
 * \brief This macro checks the AI error code and throws an L2A error if something went wrong.
 */
#define l2a_check_ai_error(err) \
    if (err) l2a_error(L2A::ERR::AIErrorCodeToErrorString(err))

/**
 * \brief Throw a warning.
 */
#define l2a_warning(warning) throw L2A::ERR::Warning(ai::UnicodeString(warning));

#endif  // L2A_ERROR_H_
