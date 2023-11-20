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

#ifndef L2A_VERSION_
#define L2A_VERSION_

#include "IllustratorSDK.h"

namespace L2A
{
    namespace GLOBAL
    {
        class Version
        {
           public:
            /**
             * \brief Initialize the version from a string.
             */
            Version(std::string version_string);

            /**
             * \brief Initialize the version from an integer.
             */
            Version(unsigned int version);

            /**
             * \brief Overload the relevant comparison operators.
             */
            bool operator==(const Version& other) const { return version_ == other.version_; }
            bool operator<(const Version& other) const { return version_ < other.version_; }
            bool operator<=(const Version& other) const { return version_ <= other.version_; }
            bool operator>(const Version& other) const { return version_ > other.version_; }
            bool operator>=(const Version& other) const { return version_ >= other.version_; }

            /**
             * \brief Convert the version to a string.
             */
            ai::UnicodeString ToString() const;

            /**
             * \brief Get the interger representing this version.
             */
            unsigned int GetVersionInt() const { return version_; }

           private:
            /**
             * \brief Derive the other operators from the basic ones.
             */
            void SetVersion(const unsigned int version, const size_t version_type);

            //! Integer to store the version number [major, minor, patch]. For each version number 8 bits are used.
            unsigned int version_;
        };

        /**
         * \brief Check if the current version is up to date with the GitHub version.
         */
        void CheckGithubVersion();
    }  // namespace GLOBAL
}  // namespace L2A


#endif
