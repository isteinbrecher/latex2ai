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
 * \brief Define constants for LaTeX2AI.
 */


#ifndef L2A_DEFINES_H_
#define L2A_DEFINES_H_

//! Include information regarding the curent git version.
#include "auto_generated/version.h"

//! Name of the plugin.
#define L2A_PLUGIN_NAME "LaTeX2AI"

//! Icon IDs.
#define TOOL_ICON_CREATE_LIGHT_ID 16051       // icon for create mode in light mode
#define TOOL_ICON_CREATE_DARK_ID 16052        // icon for create mode in dark mode
#define TOOL_ICON_REDO_LIGHT_ID 16053         // icon for redo mode in light mode
#define TOOL_ICON_REDO_DARK_ID 16054          // icon for redo mode in dark mode
#define TOOL_ICON_OPTIONS_LIGHT_ID 16055      // icon for options in light mode
#define TOOL_ICON_OPTIONS_DARK_ID 16056       // icon for options in dark mode
#define TOOL_ICON_SAVE_AS_PDF_LIGHT_ID 16057  // icon for save as pdf in light mode
#define TOOL_ICON_SAVE_AS_PDF_DARK_ID 16058   // icon for save as pdf in dark mode
#define TOOL_ICON_TESTING_LIGHT_ID 16059      // icon for testing in light mode
#define TOOL_ICON_TESTING_DARK_ID 16060       // icon for testing in dark mode

#define CURSOR_ICON_CREATE 16061  // cursor for create mode
#define CURSOR_ICON_EDIT 16062    // cursor for edit mode

namespace L2A
{
    namespace CONSTANTS
    {
        //! Tollerance for positions.
        static const double eps_pos_ = 0.002;

        //! Tollerance for angles.
        static const double eps_angle_ = 0.001;

        //! Tollerance for strech values.
        static const double eps_strech_ = 0.001;

        //! Tollerance for comparing values during testing.
        static const double eps_tests_ = 0.001;

        //! Radius of placement point for annotation.
        static const int radius_ = 7;

        //! Line width for annotation.
        static const int line_width_ = 5;

        //! Color for OK bounding box.
        static const AIRGBColor color_ok_ = {0, 65000, 0};

        //! Color for scaled bounding box.
        static const AIRGBColor color_scaled_ = {0, 0, 65000};

        //! Color for diamond bounding box.
        static const AIRGBColor color_diamond_ = {65000, 0, 0};
    }  // namespace CONSTANTS
}  // namespace L2A


#endif  // L2AID_H_
