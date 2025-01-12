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
 * \brief Utility functions for file writing / reading.
 */


#include "IllustratorSDK.h"

#include "l2a_file_system.h"

#include "base64.h"

#include "l2a_error.h"
#include "l2a_names.h"
#include "l2a_string_functions.h"
#include "l2a_suites.h"

#include <array>
#include <regex>

// File encoding.
#include <codecvt>

// To get the application path folder.
#ifdef WIN_ENV
#include "shlobj.h"
#endif


/**
 *
 */
ai::FilePath L2A::UTIL::FilePathStdToAi(const std::filesystem::path& path_std)
{
#ifdef WIN_ENV
    return ai::FilePath(L2A::UTIL::StringStdToAi(path_std.u8string()));
#else
    return ai::FilePath(L2A::UTIL::StringStdToAi(path_std.string()));
#endif
}

/**
 *
 */
std::filesystem::path L2A::UTIL::FilePathAiToStd(const ai::FilePath& path_ai)
{
#ifdef WIN_ENV
    return std::filesystem::path(L2A::UTIL::StringAiToStdW(path_ai.GetFullPath()));
#else
    return std::filesystem::path(L2A::UTIL::StringAiToStd(path_ai.GetFullPath()));
#endif
}


/**
 *
 */
bool L2A::UTIL::IsFile(const ai::FilePath& file)
{
    bool is_file = false;
    const bool exists = file.Exists(true, nullptr, &is_file);
    return (exists && is_file);
}

/**
 *
 */
bool L2A::UTIL::IsDirectory(const ai::FilePath& directory)
{
    bool is_file = true;
    const bool exists = directory.Exists(true, nullptr, &is_file);
    return (exists && (!is_file));
}

/**
 *
 */
bool L2A::UTIL::IsWriteable(const ai::FilePath& file)
{
    const bool file_exits = IsFile(file);
    std::fstream stream;
    stream.open(FilePathAiToStd(file), std::fstream::out | std::fstream::app);
    const bool is_writeable = stream.is_open();
    stream.close();
    if (!file_exits && is_writeable)
        // The file did not exits prior and it could be created, therefore it has to be deleted here.
        RemoveFile(file);
    return is_writeable;
}

/**
 *
 */
void L2A::UTIL::RemoveFile(const ai::FilePath& file, const bool& fail_if_not_exist)
{
    if (IsFile(file))
    {
        std::error_code ec;
        std::filesystem::remove(FilePathAiToStd(file), ec);
        if (ec.value() != 0) l2a_error("The given path " + file.GetFullPath() + " could not be deleted!");
    }
    else if (IsDirectory(file))
        l2a_error("The given path " + file.GetFullPath() + " is a directory!");
    else if (fail_if_not_exist)
        l2a_error("The given path " + file.GetFullPath() + " does not exist!");
}

/**
 *
 */
void L2A::UTIL::RemoveDirectoryAI(const ai::FilePath& directory, const bool& fail_if_not_exist)
{
    // Check that it exists and is directroy.
    if (IsDirectory(directory))
    {
        std::error_code ec;
        std::filesystem::remove_all(FilePathAiToStd(directory), ec);
        if (ec.value() != 0) l2a_error("The folder \"" + directory.GetFullPath() + "\" could not be deleted!");
    }
    else if (IsFile(directory))
        l2a_error("The given path " + directory.GetFullPath() + " is a file!");
    else if (fail_if_not_exist)
        l2a_error("The given path " + directory.GetFullPath() + " does not exist!");
}

/**
 *
 */
void L2A::UTIL::ClearDirectory(const ai::FilePath& directory, const bool fail_if_not_exist)
{
    if (!IsDirectory(directory))
    {
        if (fail_if_not_exist)
        {
            l2a_error("The given directory " + directory.GetFullPath() + " to clear does not exist!");
        }
        else
        {
            CreateDirectoryL2A(directory);
        }
    }

    for (const auto& entry : std::filesystem::directory_iterator(FilePathAiToStd(directory)))
    {
        std::error_code ec;
        std::filesystem::remove_all(entry.path(), ec);
        if (ec.value() != 0) l2a_error("The item \"" + entry.path().string() + "\" could not be deleted!");
    }
}

/**
 *
 */
void L2A::UTIL::WriteFileUTF8(const ai::FilePath& path, const ai::UnicodeString& text, const bool& overwrite)
{
    // Check if the folder exists.
    if (!IsDirectory(path.GetParent()))
        l2a_error("The folder '" + path.GetParent().GetFullPath() + "' for the file '" + path.GetFullPath() +
                  "' does not exist!");

    // Check if the file already exists.
    if (IsFile(path) && !overwrite)
        l2a_error("The file '" + path.GetFullPath() + "' alreday exists and the option overwrite is false!");

    // Write text to file.
    std::ofstream f(FilePathAiToStd(path));
    f << L2A::UTIL::StringAiToStd(text);
    f.close();
}

/**
 *
 */
ai::UnicodeString L2A::UTIL::ReadFileUTF8(const ai::FilePath& path)
{
    // Check if the file exists
    if (!IsFile(path)) l2a_error("The file '" + path.GetFullPath() + "' does not exist!");

    // Open the file stream
    std::ifstream istrm(UTIL::FilePathAiToStd(path));
    std::stringstream buffer;
    buffer << istrm.rdbuf();
    return L2A::UTIL::StringStdToAi(buffer.str());
}

/**
 *
 */
void L2A::UTIL::CreateDirectoryL2A(const ai::FilePath& directory)
{
    std::error_code ec;
    std::filesystem::create_directory(FilePathAiToStd(directory), ec);
    if (ec.value() != 0) l2a_error("Could not create the directory " + directory.GetFullPath());
}

void L2A::UTIL::CopyFileL2A(const ai::FilePath& source, const ai::FilePath& target)
{
    // Check if source exists
    if (!IsFile(source)) l2a_error("The source file '" + source.GetFullPath() + "'does not exist!");

    // Check if destination directory exists
    if (!IsDirectory(target.GetParent())) l2a_error("The target dir '" + target.GetFullPath() + "'does not exist!");

    // Copy the file
    std::error_code ec;
    std::filesystem::copy(
        FilePathAiToStd(source), FilePathAiToStd(target), std::filesystem::copy_options::overwrite_existing, ec);
    if (ec.value() != 0) l2a_error("Could not copy the file " + source.GetFullPath() + " to " + target.GetFullPath());
}

/**
 *
 */
ai::FilePath L2A::UTIL::GetTemporaryDirectory()
{
    ai::FilePath temp_directory;
    AIErr error = sAIFolders->FindFolder(kAITemporayFolder, false, temp_directory);
    l2a_check_ai_error(error);
    temp_directory.AddComponent(ai::UnicodeString("LaTeX2AI"));
    return temp_directory;
}

/**
 *
 */
ai::FilePath L2A::UTIL::ClearTemporaryDirectory()
{
    const auto temp_directory = GetTemporaryDirectory();
    ClearDirectory(temp_directory, false);
    return temp_directory;
}

/**
 *
 */
ai::FilePath L2A::UTIL::GetApplicationDataDirectory()
{
    ai::FilePath application_data_directory;
    AIErr error = sAIFolders->FindFolder(kAIUserSupportFolderType, false, application_data_directory);
    l2a_check_ai_error(error);
    application_data_directory.AddComponent(ai::UnicodeString("LaTeX2AI"));
    return application_data_directory;
}

/**
 *
 */
ai::FilePath L2A::UTIL::GetDocumentPath(bool fail_if_not_saved)
{
    ASErr error = kNoErr;
    ai::FilePath path;
    error = sAIDocument->GetDocumentFileSpecification(path);
    l2a_check_ai_error(error);

    // Check if the path is a file
    if (!IsFile(path) && fail_if_not_saved)
    {
        l2a_warning(
            "The document is not saved! Almost all functionality of LaTeX2AI requires the document to be saved.");
    }

    return path;
}

/**
 *
 */
ai::UnicodeString L2A::UTIL::GetDocumentName() { return GetDocumentPath(false).GetFileNameNoExt(); }

/**
 *
 */
ai::FilePath L2A::UTIL::GetPdfFileDirectory()
{
    ai::FilePath path = L2A::UTIL::GetDocumentPath();
    path = path.GetParent();
    path.AddComponent(ai::UnicodeString(L2A::NAMES::pdf_file_directory_));
    return path;
}

/**
 *
 */
std::vector<ai::FilePath> L2A::UTIL::FindFilesInFolder(const ai::FilePath& folder, const ai::UnicodeString& regex)
{
    const std::regex regex_string(L2A::UTIL::StringAiToStd(regex));
    std::vector<ai::FilePath> file_vector;
    for (auto const& dir_entry : std::filesystem::directory_iterator{FilePathAiToStd(folder)})
    {
        if (std::filesystem::is_regular_file(dir_entry))
        {
            const auto file_name = dir_entry.path().filename().u8string();
            if (std::regex_search(file_name, regex_string))
            {
                file_vector.push_back(FilePathStdToAi(dir_entry.path()));
            }
        }
    }

    // Sort the paths to ensure a deterministic ordering of the returned vector
    std::sort(file_vector.begin(), file_vector.end(),
        [](const ai::FilePath& a, const ai::FilePath& b)
        {
            return a.GetFullPath() < b.GetFullPath();  // Compare as UTF-8 strings
        });
    return file_vector;
}

/**
 *
 */
std::pair<bool, ai::FilePath> L2A::UTIL::FindExecutable(
    const std::vector<ai::FilePath>& possible_bin_paths, const ai::UnicodeString& executable_name)
{
    for (const auto& bin_path : possible_bin_paths)
    {
        auto executable_path = bin_path;
        executable_path.AddComponent(executable_name);
        if (IsFile(executable_path))
        {
            return {true, executable_path};
        }
    }
    return {false, ai::FilePath(ai::UnicodeString(""))};
}

/**
 *
 */
ai::FilePath L2A::UTIL::GetFullFilePath(const ai::FilePath& path)
{
    const auto path_std = FilePathAiToStd(path);
    const auto absolute_path = std::filesystem::absolute(path_std);
    return FilePathStdToAi(absolute_path);
}

/**
 *
 */
void L2A::UTIL::SetWorkingDirectory(const ai::FilePath& path) { std::filesystem::current_path(FilePathAiToStd(path)); }

/**
 *
 */
bool L2A::UTIL::IsEqualFile(const ai::FilePath& path_a, const ai::FilePath& path_b)
{
    // TODO: This function returns false if the files are equal and one of the paths points to a network folder and the
    // other one to a mounted drive.
    return path_a.Equal(path_b, true);
}

/*
 *
 */
std::string L2A::UTIL::encode_file_base64(const ai::FilePath& path)
{
    // https://www.cplusplus.com/reference/istream/istream/read/

    std::ifstream input_stream(FilePathAiToStd(path), std::ifstream::binary);
    if (!input_stream) l2a_error("Error in loading file");

    // Get length of the file.
    input_stream.seekg(0, input_stream.end);
    unsigned int length = (unsigned int)input_stream.tellg();
    input_stream.seekg(0, input_stream.beg);

    // Storage for data.
    std::vector<char> buffer_vector(length);
    char* buffer = buffer_vector.data();

    // Read data as a block.
    input_stream.read(buffer, length);
    if (!input_stream) l2a_error("Error in reading file");
    input_stream.close();

    // Encode file data.
    return base64::encode(buffer, length);
}

/*
 *
 */
void L2A::UTIL::decode_file_base64(const ai::FilePath& path, const ai::UnicodeString& encoded_string)
{
    auto char_vector = base64::decode(L2A::UTIL::StringAiToStd(encoded_string));
    std::ofstream output_stream(FilePathAiToStd(path), std::ofstream::binary);
    output_stream.write(char_vector.data(), char_vector.size());
    output_stream.close();
}
