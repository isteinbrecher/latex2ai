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
 * \brief Utility functions for executing system commands.
 */

#ifndef UTIL_EXECUTE_H_
#define UTIL_EXECUTE_H_


#include "IllustratorSDK.h"


namespace L2A
{
    namespace UTIL
    {
        /**
         * \brief Structure to return the results from a call to an external command
         */
        struct CommandResult
        {
            //! The exit status returned by the command
            int exit_status_;

            //! The output string by the command
            ai::UnicodeString output_;
        };

        /**
         * \brief Execute system command and get stdout result
         *
         * Taken from https://github.com/RaymiiOrg/cpp-command-output
         *
         * If you want stderr, use shell redirection (2&>1).
         */
        CommandResult ExecuteCommandLineNew(const ai::UnicodeString& command);

        /**
         * \brief Execute a command line. Return the exit code.
         */
        int ExecuteCommandLine(const ai::UnicodeString& command);

        /**
         * \brief Execute a command line. Return the exit code and the comand output.
         */
        int ExecuteCommandLine(const ai::UnicodeString& command, ai::UnicodeString& command_output,
            const bool quiet = true, const unsigned long max_time_ms = INFINITE);

        /**
         * \brief Execute a command line. Return the exit code and the comand output. Do not throw erros in this
         * function.
         */
        int ExecuteCommandLineNoErrors(const ai::UnicodeString& command, ai::UnicodeString& command_output,
            ai::UnicodeString& error_message, const unsigned long max_time_ms);

        /**
         * \brief Execute a file on disc.
         */
        int ExecuteFile(const ai::FilePath& file_path);
    }  // namespace UTIL
}  // namespace L2A

#endif
