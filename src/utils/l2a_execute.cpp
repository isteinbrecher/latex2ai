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


#include "IllustratorSDK.h"
#include "l2a_execute.h"

#include "l2a_file_system.h"
#include "l2a_error.h"
#include "l2a_string_functions.h"

#include <array>


/**
 *
 */
L2A::UTIL::CommandResult L2A::UTIL::ExecuteCommandLine(const ai::UnicodeString& command, const bool quiet, const unsigned long max_time_ms)
{
#ifdef _WIN32
    if (quiet)
    {
        return INTERNAL::ExecuteCommandLineWindowsNoConsole(command, max_time_ms);
    }
    else
    {
        return INTERNAL::ExecuteCommandLineStd(command);
    }
#else
    return INTERNAL::ExecuteCommandLineStd(command);
#endif
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
L2A::UTIL::CommandResult L2A::UTIL::INTERNAL::ExecuteCommandLineStd(
    const ai::UnicodeString& command)
{
    std::array<char, 8192> buffer{};
    std::string result;
#ifdef _WIN32
#define popen _popen
#define pclose _pclose
#define WEXITSTATUS
#endif
    FILE* pipe = popen(command.as_Platform().c_str(), "r");
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
    return CommandResult{exitcode, ai::UnicodeString(result)};
}

/**
 *
 */
L2A::UTIL::CommandResult L2A::UTIL::INTERNAL::ExecuteCommandLineWindowsNoConsole(
    const ai::UnicodeString& command, const unsigned long max_time_ms)
{
#ifndef _WIN32
    l2a_error("You are using the function for the wrong OS! Use the system calls via ExecuteCommandLine!");
#else
    // This code is mainly a combination of
    // https://www.codeproject.com/Tips/333559/CreateProcess-and-wait-for-result
    // https://docs.microsoft.com/en-us/windows/win32/procthread/creating-a-child-process-with-redirected-input-and-output

    // Convert the string to platform text.
    std::string cmdLine = command.as_Platform();
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
    STARTUPINFO startupInfo = {0};
    startupInfo.cb = sizeof(startupInfo);
    startupInfo.hStdError = g_hChildStd_OUT_Wr;
    startupInfo.hStdOutput = g_hChildStd_OUT_Wr;
    startupInfo.dwFlags |= STARTF_USESTDHANDLES;
    BOOL result = CreateProcess(nullptr, (char*)(cmdLine.c_str()), nullptr, nullptr, TRUE,
        NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW, nullptr, nullptr, &startupInfo, &processInformation);

    // Check if the process could be created.
    if (!result)
    {
        // Get the error from the system
        LPVOID lpMsgBuf;
        DWORD dw = GetLastError();
        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, nullptr);

        // Display the error
        // CString strError = (LPTSTR) lpMsgBuf;
        // TRACE(_T("::executeCommandLine() failed at CreateProcess()\nCommand=%s\nMessage=%s\n\n"), cmdLine, strError);

        // Free resources created by the system
        LocalFree(lpMsgBuf);

        // Create error message.
        l2a_error("Error, process '" + command.as_Platform() + "' could not be created!");
    }
    else
    {
        // Successfully created the process.  Wait for it to finish.
        WaitForSingleObject(processInformation.hProcess, max_time_ms);

        // Get the exit code.
        DWORD exitCode;
        result = GetExitCodeProcess(processInformation.hProcess, &exitCode);

        // Close the handles.
        CloseHandle(processInformation.hProcess);
        CloseHandle(processInformation.hThread);
        CloseHandle(g_hChildStd_OUT_Wr);

        // Read the output from the command.
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

        if (!result)
        {
            l2a_error("Executed command but couldn't get exit code.");
        }

        // Return exit code, and command output as unicode string.
        return CommandResult{(int)exitCode, ai::UnicodeString(result_string)};
    }
#endif
}
