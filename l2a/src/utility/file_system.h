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
 * \brief Utility functions for file writing / reading.
 */

#ifndef UTIL_FILE_H_
#define UTIL_FILE_H_


#include "IllustratorSDK.h"


namespace L2A
{
    namespace UTIL
    {
        /**
         * \brief Checks if the path is a file and exists.
         */
        bool IsFile(const ai::FilePath& file);

        /**
         * \brief Checks if the path is a directory and exists.
         */
        bool IsDirectory(const ai::FilePath& directory);

        /**
         * \brief Checks if a file is write protected.
         */
        bool IsWriteable(const ai::FilePath& file);

        /**
         * \brief Remove a file on the file system. If the file does not exist, an error is thrown.
         */
        void RemoveFile(const ai::FilePath& file, const bool& fail_if_not_exist = true);

        /**
         * \brief Remove a directory on the file system. If the directory does not exist, an error is thrown.
         */
        void RemoveDirectoryAI(const ai::FilePath& directroy, const bool& fail_if_not_exist = true);

        /**
         * \brief Wrties a string to an UTF8 file.
         * @param path path object of the file to save
         * @param text contains string that will be saved in the file
         * @param overwrite if this is true, an existing file will be overwritten, otherwise an error will be thrown.
         */
        void WriteFileUTF8(const ai::FilePath& path, const ai::UnicodeString& text, const bool& overwrite = false);

        /**
         * \brief Reads an ai::UnicodeString from a file
         */
        ai::UnicodeString ReadFileUTF8(const ai::FilePath& path);

        /**
         * \brief Create a directory if it does not exist yet.
         */
        void CreateDirectoryL2A(const ai::FilePath& directory);

        /**
         * \brief Copy a file on the disc.
         */
        void CopyFileL2A(const ai::FilePath& source, const ai::FilePath& target);

        /**
         * \brief Return the path to the temp directory on the system.
         */
        ai::FilePath GetTemporaryDirectory();

        /**
         * \brief Return the path to the application data directory.
         */
        ai::FilePath GetApplicationDataDirectory();

        /**
         * \brief Get path to the current AI document.
         */
        ai::FilePath GetDocumentPath(bool fail_if_not_saved = true);

        /**
         * \brief Get the name of the document, without the .ai extension.
         */
        ai::UnicodeString GetDocumentName();

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

        /**
         * \brief Search the path to ghostscript on the system.
         */
        ai::UnicodeString GetGhostScriptCommand();

        /**
         * \brief Search the path to forms executable on the system.
         *
         * The folder where the plug in is located will be searched for the forms executable.
         */
        ai::FilePath GetFormsPath();

        /**
         * \brief Get the direcotry where l2a files will be placed for the current document.
         */
        ai::FilePath GetPdfFileDirectory();

        /**
         * \brief Find all files in a folder matching a pattern.
         */
        std::vector<ai::FilePath> FindFilesInFolder(const ai::FilePath& folder, const ai::UnicodeString& pattern);

        /**
         * \brief Get the index of the next available l2a pdf file for the current document.
         */
        unsigned int GetNextItemIndex(ai::FilePath& next_path);

        /**
         * \brief Return the full file path for a given path. This also resolves ".." inside the file path.
         */
        ai::FilePath GetFullFilePath(const ai::FilePath& path);

        /**
         * \brief Set the working directory of the plugin.
         */
        void SetWorkingDirectory(const ai::FilePath& path);

        /**
         * \brief Check if two file paths point to the same file object.
         */
        bool IsEqualFile(const ai::FilePath& path_a, const ai::FilePath& path_b);

        /*
         * \brief Encode a file to base 64.
         */
        std::string encode_file_base64(const ai::FilePath& path);

        /*
         * \brief Write a base64 encoded string to a file.
         */
        void decode_file_base64(const ai::FilePath& path, const std::string& encoded_string);
    }  // namespace UTIL
}  // namespace L2A

#endif
