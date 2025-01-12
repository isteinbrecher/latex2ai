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

#include "l2a_execute.h"

#include "l2a_error.h"
#include "l2a_file_system.h"
#include "l2a_string_functions.h"

#include <array>


/**
 *
 */
L2A::UTIL::CommandResult L2A::UTIL::ExecuteCommandLine(const ai::UnicodeString& command)
{
#ifdef WIN_ENV
    return INTERNAL::ExecuteCommandLineWindowsNoConsole(command);
#else
    return INTERNAL::ExecuteCommandLineStd(command);
#endif
}

/**
 *
 */
L2A::UTIL::CommandResult L2A::UTIL::INTERNAL::ExecuteCommandLineStd(const ai::UnicodeString& command)
{
    std::array<char, 8192> buffer{};
    std::string result;
#ifdef WIN_ENV
#define popen _popen
#define pclose _pclose
#define WEXITSTATUS
    l2a_error(
        "ExecuteCommandLineStd is not tested for Windows. If this is adaped, check that unicode works as expected!");
#endif
    FILE* pipe = popen(L2A::UTIL::StringAiToStd(command).c_str(), "r");
    if (pipe == nullptr)
    {
        l2a_error("popen() failed!");
    }
    try
    {
        std::size_t bytesread;
        while ((bytesread = std::fread(buffer.data(), sizeof(buffer.at(0)), sizeof(buffer), pipe)) != 0)
        {
            result += std::string(buffer.data(), bytesread);
        }
    }
    catch (...)
    {
        pclose(pipe);
        l2a_error("popen() failed!");
    }
    // Workaround "error: cannot take the address of an rvalue of type 'int'" on MacOS
    // see e.g. https://github.com/BestImageViewer/geeqie/commit/75c7df8b96592e10f7936dc1a28983be4089578c
    int res = pclose(pipe);
    const int exitcode = WEXITSTATUS(res);
    return CommandResult{exitcode, L2A::UTIL::StringStdToAi(result)};
}

/**
 *
 */
L2A::UTIL::CommandResult L2A::UTIL::INTERNAL::ExecuteCommandLineWindowsNoConsole(const ai::UnicodeString& command)
{
#ifndef WIN_ENV
    l2a_error("You are using the function for the wrong OS! Use the system calls via ExecuteCommandLine!");
#else
    // This code is mainly a combination of
    // https://www.codeproject.com/Tips/333559/CreateProcess-and-wait-for-result
    // https://docs.microsoft.com/en-us/windows/win32/procthread/creating-a-child-process-with-redirected-input-and-output

    // Convert the string to platform text.
    auto command_wstr = L2A::UTIL::StringAiToStdW(command);

    SECURITY_ATTRIBUTES saAttr;
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = nullptr;
    HANDLE g_hChildStd_OUT_Rd = nullptr;
    HANDLE g_hChildStd_OUT_Wr = nullptr;
    if (!CreatePipe(&g_hChildStd_OUT_Rd, &g_hChildStd_OUT_Wr, &saAttr, 0))
    {
        l2a_error("StdoutRd CreatePipe");
    }
    if (!SetHandleInformation(g_hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0))
    {
        l2a_error("Stdout SetHandleInformation");
    }

    // Create the process.
    PROCESS_INFORMATION processInformation = {0};
    STARTUPINFOW startupInfo = {0};
    startupInfo.cb = sizeof(startupInfo);
    startupInfo.hStdError = g_hChildStd_OUT_Wr;
    startupInfo.hStdOutput = g_hChildStd_OUT_Wr;
    startupInfo.dwFlags |= STARTF_USESTDHANDLES;
    BOOL result = CreateProcessW(nullptr, &command_wstr[0], nullptr, nullptr, TRUE,
        NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW, nullptr, nullptr, &startupInfo, &processInformation);

    // Check if the process could be created.
    if (!result)
    {
        // Get the error from the system
        LPWSTR lpMsgBuf;
        const DWORD dw = GetLastError();
        FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&lpMsgBuf, 0, nullptr);
        ai::UnicodeString error_string(lpMsgBuf);

        // Free resources created by the system
        LocalFree(lpMsgBuf);

        // Create error message.
        l2a_error("Error, process '" + command + "' could not be created! Got error: " + error_string);
    }
    else
    {
        // Successfully created the process. First close the write handle so the process can finish.
        CloseHandle(g_hChildStd_OUT_Wr);

        // Now we continuously read the ouput of the process
        DWORD dwRead;
        static const int BUFSIZE = 4096;
        CHAR chBuf[BUFSIZE];
        BOOL bSuccess = FALSE;
        std::string result_string = "";
        for (;;)
        {
            bSuccess = ReadFile(g_hChildStd_OUT_Rd, chBuf, BUFSIZE, &dwRead, NULL);
            if (!bSuccess || dwRead == 0) break;

            std::string s(chBuf, dwRead);
            result_string += s;
        }

        // Wait for the process to finish
        WaitForSingleObject(processInformation.hProcess, INFINITE);

        // Get the exit code
        DWORD exitCode;
        result = GetExitCodeProcess(processInformation.hProcess, &exitCode);

        // Close all remaining handles
        CloseHandle(processInformation.hProcess);
        CloseHandle(processInformation.hThread);
        CloseHandle(g_hChildStd_OUT_Rd);

        if (!result)
        {
            l2a_error("Executed command but couldn't get exit code.");
        }

        // Return exit code, and command output as unicode string
        return CommandResult{(int)exitCode, L2A::UTIL::StringStdToAi(result_string)};
    }
#endif
}

/**
 *
 */
void L2A::UTIL::OpenFileWithDefaultApplication(const ai::FilePath& file_path)
{
#ifdef WIN_ENV
    ShellExecute(0, 0, L2A::UTIL::StringAiToStd(file_path.GetFullPath()).c_str(), 0, 0, SW_SHOW);
#else
    std::string command = "open ";
    std::string full_commad = command + "\"" + L2A::UTIL::StringAiToStd(file_path.GetFullPath()) + "\"";
    system(full_commad.c_str());
#endif
}

/**
 *
 */
void L2A::UTIL::OpenFolder(const ai::FilePath& folder_path)
{
    if (!L2A::UTIL::IsDirectory(folder_path)) l2a_error("Expected a directory in L2A::UTIL::OpenFolder");
#ifdef WIN_ENV
    std::string command = "explorer ";
#else
    std::string command = "open ";
#endif
    std::string full_commad = command + "\"" + L2A::UTIL::StringAiToStd(folder_path.GetFullPath()) + "\"";
    system(full_commad.c_str());
}
