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
 * \brief Utility functions for file writing / reading.
 */


#include "IllustratorSDK.h"
#include "file_system.h"

#include "l2a_suites.h"
#include "l2a_error/l2a_error.h"
#include "utility/string_functions.h"
#include "l2a_names.h"

#include <filesystem>

// File encoding.
#include <codecvt>

// To get the application path folder.
#include "shlobj.h"


/**
 *
 */
bool L2A::UTIL::IsFile(const ai::FilePath& file)
{
    bool is_file = false;
    bool exists = file.Exists(true, NULL, &is_file);
    return (exists && is_file);
}

/**
 *
 */
bool L2A::UTIL::IsDirectory(const ai::FilePath& directory)
{
    bool is_file = true;
    bool exists = directory.Exists(true, NULL, &is_file);
    return (exists && (!is_file));
}

/**
 *
 */
bool L2A::UTIL::IsWriteable(const ai::FilePath& file)
{
    bool file_exits = IsFile(file);
    std::fstream stream;
    stream.open(file.GetFullPath().as_Platform(), std::fstream::out | std::fstream::app);
    bool is_writeable = stream.is_open();
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
    // Check that it exists and is file.
    if (IsFile(file))
    {
        const int return_value = remove(file.GetFullPath().as_Platform().c_str());

        // Check if file could be deleted.
        if (return_value != 0) l2a_error("The given path " + file.GetFullPath() + " could not be deleted!");
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
        try
        {
            std::filesystem::remove_all(directory.GetFullPath().as_Platform());
        }
        catch (...)
        {
            l2a_error("The folder \"" + directory.GetFullPath() + "\" could not be deleted!");
        }
    }
    else if (IsFile(directory))
        l2a_error("The given path " + directory.GetFullPath() + " is a file!");
    else if (fail_if_not_exist)
        l2a_error("The given path " + directory.GetFullPath() + " does not exist!");
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
    std::ofstream f(path.GetFullPath().as_Platform());
    f << text.as_UTF8();
    f.close();
}

/**
 *
 */
ai::UnicodeString L2A::UTIL::ReadFileUTF8(const ai::FilePath& path)
{
    // Check if the file exists.
    if (!IsFile(path)) l2a_error("The file '" + path.GetFullPath() + "' does not exist!");

    // Open the file stream.
    std::wifstream wif(path.GetFullPath().as_Platform());
    wif.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
    std::wstringstream wss;
    wss << wif.rdbuf();

    // Convert file contents to ai::UnicodeString.
    return ai::UnicodeString(wss.str().c_str());
}

/**
 *
 */
void L2A::UTIL::CreateDirectoryL2A(const ai::FilePath& directory)
{
    // This vector stores all parts of the directory that do not exist.
    ai::FilePath path = directory;
    std::vector<ai::UnicodeString> parts;
    while (!IsDirectory(path))
    {
        // Check if the path is not a valid file.
        if (IsFile(path)) l2a_error("A parent of the path '" + directory.GetFullPath() + "' is a file!");

        // Add the current item to the vector.
        parts.push_back(path.GetFileName());

        // Go one level higher.
        path = path.GetParent();
    }

    // Create the missing directories.
    size_t n_parts = parts.size();
    for (size_t i = 0; i < n_parts; i++)
    {
        path.AddComponent(parts[n_parts - i - 1]);
        CreateDirectory(path.GetFullPath().as_Platform().c_str(), NULL);
    }
}

void L2A::UTIL::CopyFileL2A(const ai::FilePath& source, const ai::FilePath& target)
{
    // Check if source exists.
    if (!IsFile(source)) l2a_error("The source file '" + source.GetFullPath() + "'does not exist!");

    // Check if destination directory exists.
    if (!IsDirectory(target.GetParent())) l2a_error("The target dir '" + target.GetFullPath() + "'does not exist!");

    // Copy the file.
    bool copy_ok =
        CopyFile(source.GetFullPath().as_Platform().c_str(), target.GetFullPath().as_Platform().c_str(), false);

    // Check if everything was ok
    if (!copy_ok) l2a_error("Error in copy process!");
}

/**
 *
 */
ai::FilePath L2A::UTIL::GetTemporaryDirectory()
{
    TCHAR pathBuffer[MAX_PATH];
    GetTempPath(MAX_PATH, pathBuffer);
    return ai::FilePath(ai::UnicodeString(pathBuffer));
}

/**
 *
 */
ai::FilePath L2A::UTIL::GetApplicationDataDirectory()
{
    TCHAR pathBuffer[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, pathBuffer)))
        return ai::FilePath(ai::UnicodeString(pathBuffer));
    else
        l2a_error("The application data directory could not be retrieved!");
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

    // Check if the path is a file.
    if (!IsFile(path) && fail_if_not_saved)
        throw L2A::ERR::Warning(ai::UnicodeString(
            "The document is not saved! Almost all functionality of LaTeX2AI requires the document to be saved."));

    return path;
}

/**
 *
 */
ai::UnicodeString L2A::UTIL::GetDocumentName() { return GetDocumentPath(false).GetFileNameNoExt(); }

/**
 *
 */
int L2A::UTIL::ExecuteCommandLine(const ai::UnicodeString& command)
{  // This code is mainly from
    // https://www.codeproject.com/Tips/333559/CreateProcess-and-wait-for-result

    // Convert the string to platform text.
    std::string cmdLine = command.as_Platform();

    // Create the process.
    PROCESS_INFORMATION processInformation = {0};
    STARTUPINFO startupInfo = {0};
    startupInfo.cb = sizeof(startupInfo);
    BOOL result = CreateProcess(NULL, (char*)(cmdLine.c_str()), NULL, NULL, FALSE,
        NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW, NULL, NULL, &startupInfo, &processInformation);

    // Check if the process could be created.
    if (!result)
    {
        // Get the error from the system
        LPVOID lpMsgBuf;
        DWORD dw = GetLastError();
        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL,
            dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);

        // Display the error
        // CString strError = (LPTSTR) lpMsgBuf;
        // TRACE(_T("::executeCommandLine() failed at CreateProcess()\nCommand=%s\nMessage=%s\n\n"), cmdLine, strError);

        // Free resources created by the system
        LocalFree(lpMsgBuf);

        // Create error message.
        l2a_error("Error, process '" + command + "' could not be created!");
    }
    else
    {
        // Successfully created the process.  Wait for it to finish.
        WaitForSingleObject(processInformation.hProcess, INFINITE);

        // Get the exit code.
        DWORD exitCode;
        result = GetExitCodeProcess(processInformation.hProcess, &exitCode);

        // Close the handles.
        CloseHandle(processInformation.hProcess);
        CloseHandle(processInformation.hThread);

        if (!result) l2a_error("Executed command but couldn't get exit code.");

        // Everything succeeded and return the exit code.
        return (int)exitCode;
    }
}

/**
 *
 */
int L2A::UTIL::ExecuteFile(const ai::FilePath& file_path)
{
    if (L2A::UTIL::IsFile(file_path))
    {
        std::string command = "\"" + file_path.GetFullPath().as_Platform() + "\"";

        return system(command.c_str());
    }
    else
        l2a_error("The file " + file_path.GetFullPath() + " does not exist!");
}

/**
 *
 */
ai::UnicodeString L2A::UTIL::GetGhostScriptCommand()
{
    // Get the path to the programs folder.
    TCHAR pathBuffer[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PROGRAM_FILES, NULL, 0, pathBuffer)))
    {
        ai::FilePath program_folder = ai::FilePath(ai::UnicodeString(pathBuffer));
        program_folder.AddComponent(ai::UnicodeString("gs"));

        // Check if the ghostscript folder exists.
        if (IsDirectory(program_folder))
        {
            ai::FilePath gs_folder;
            for (auto& p : std::filesystem::directory_iterator(program_folder.GetFullPath().as_Platform()))
            {
                // Check if the directory starts with "gs".
                gs_folder = ai::FilePath(ai::UnicodeString(p.path().string()));
                if (L2A::UTIL::StartsWith(gs_folder.GetFileName(), ai::UnicodeString("gs"), true))
                {
                    // We do not care about the version -> use the first "gs*" folder that we find.

                    // Check if an execuable can be found.
                    gs_folder.AddComponent(ai::UnicodeString("bin"));
                    gs_folder.AddComponent(ai::UnicodeString("gswin64c.exe"));
                    if (IsFile(gs_folder))
                        return gs_folder.GetFullPath();
                    else
                        break;
                }
            }
        }
    }

    // Default return value.
    return ai::UnicodeString("");
}

/**
 *
 */
ai::FilePath L2A::UTIL::GetFormsPath()
{
    // Default value.
    std::vector<ai::FilePath> forms_paths;

    // Look in the installed plugins folder as well as the additional extensions folder.
    std::vector<ai::FilePath> plugin_directories(2);
    sAIFolders->FindFolder(kAIPluginsFolderType, false, plugin_directories[0]);
    sAIFolders->FindFolder(kAIAdditionalAIPluginsFolderType, false, plugin_directories[1]);
    for (const auto& plugin_directory : plugin_directories)
    {
        if (L2A::UTIL::IsDirectory(plugin_directory))
        {
            // Search for the executable in the folder.
            for (const auto& item :
                std::filesystem::recursive_directory_iterator(plugin_directory.GetFullPath().as_Platform()))
            {
                ai::FilePath current_item(ai::UnicodeString(item.path()));
                if (current_item.GetFileName() == "LaTeX2AIForms.exe") forms_paths.push_back(current_item);
            }
        }
    }

    if (forms_paths.size() > 1)
    {
        // More than one found forms executable could lead to undefined behaviour.
        ai::UnicodeString error_string("LaTeX2AI found more than one LaTeX2AIForms.exe executable.");
        error_string += "\nThere should only be one.";
        error_string += "\nThe found paths are:";
        for (const auto& forms_exe : forms_paths) error_string += "\n" + forms_exe.GetFullPath();
        l2a_error(error_string);
    }
    else if (forms_paths.size() == 1)
        return forms_paths[0];
    else
        return ai::FilePath(ai::UnicodeString(""));
}

/**
 *
 */
ai::FilePath L2A::UTIL::GetPdfFileDirectory()
{
    // Get the document path.
    ai::FilePath path = L2A::UTIL::GetDocumentPath();

    // Check that the header directory exists.
    path = path.GetParent();
    path.AddComponent(ai::UnicodeString(L2A::NAMES::pdf_file_directory_));
    L2A::UTIL::CreateDirectoryL2A(path);

    // Return the header directory.
    return path;
}

/**
 *
 */
std::vector<ai::FilePath> L2A::UTIL::FindFilesInFolder(const ai::FilePath& folder, const ai::UnicodeString& pattern)
{
    // Set up the return vector.
    std::vector<ai::FilePath> file_vector;
    file_vector.clear();

    // Create the search string.
    ai::UnicodeString search_string = folder.GetFullPath() + "\\" + pattern;

    // Find the files.
    WIN32_FIND_DATA search_data;
    memset(&search_data, 0, sizeof(WIN32_FIND_DATA));
    HANDLE handle = FindFirstFile(search_string.as_Platform().c_str(), &search_data);
    while (handle != INVALID_HANDLE_VALUE)
    {
        // Add file path to output vector.
        ai::FilePath file_path = folder;
        file_path.AddComponent(ai::UnicodeString(search_data.cFileName));
        file_vector.push_back(file_path);

        // Look for next file.
        if (FindNextFile(handle, &search_data) == FALSE) break;
    }
    FindClose(handle);

    return file_vector;
}

/**
 *
 */
unsigned int L2A::UTIL::GetNextItemIndex(ai::FilePath& next_path)
{
    unsigned int i = 1;
    ai::UnicodeString document_name = GetDocumentName();
    ai::FilePath pdf_directory = GetPdfFileDirectory();
    ai::FilePath file;

    // Loop through the header directory.
    bool while_value = true;
    while (while_value)
    {
        file = pdf_directory;
        file.AddComponent(L2A::NAMES::GetPdfItemName(document_name, i));
        if (IsFile(file))
            i++;
        else
            while_value = false;
    }

    next_path = file;
    return i;
}

/**
 *
 */
ai::FilePath L2A::UTIL::GetFullFilePath(const ai::FilePath& path)
{
    std::filesystem::path path_std = path.GetFullPath().as_Platform();
    return ai::FilePath(ai::UnicodeString(std::filesystem::absolute(path_std).string()));
}
