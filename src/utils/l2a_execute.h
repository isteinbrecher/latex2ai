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
         * \brief Execute a command line. Return the exit code and the command output.
         */
        CommandResult ExecuteCommandLine(const ai::UnicodeString& command);

        namespace INTERNAL
        {
            /**
             * \brief Execute system command and get stdout result. Do not throw errors in this function.
             *
             * Taken from https://github.com/RaymiiOrg/cpp-command-output
             *
             * If you want stderr, use shell redirection (2&>1).
             */
            CommandResult ExecuteCommandLineStd(const ai::UnicodeString& command);

            /**
             * \brief Execute a command line. Return the exit code and the command output. Do not throw errors in this
             * function.
             *
             * This function can only be used on windows, as the ExecuteCommandLineStd function opens a console window
             * under Windows
             */
            CommandResult ExecuteCommandLineWindowsNoConsole(const ai::UnicodeString& command);
        }  // namespace INTERNAL

        /**
         * \brief Open a file on disk with the default application.
         */
        void OpenFileWithDefaultApplication(const ai::FilePath& file_path);

        /**
         * \brief Open a folder in the GUI of the OS
         */
        void OpenFolder(const ai::FilePath& folder_path);
    }  // namespace UTIL
}  // namespace L2A

#endif
