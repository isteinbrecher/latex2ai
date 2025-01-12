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
 * \brief Interact with the LaTeX2AI UI
 */

#include "IllustratorSDK.h"

#include "l2a_ui_manager.h"


/**
 *
 */
L2A::UI::Manager::Manager()
{
    forms_[FormsEnum::item] = std::make_unique<Item>();
    forms_[FormsEnum::redo] = std::make_unique<Redo>();
    forms_[FormsEnum::debug] = std::make_unique<Debug>();
    forms_[FormsEnum::options] = std::make_unique<Options>();
}

/**
 *
 */
void L2A::UI::Manager::RegisterCSXSEventListeners()
{
    for (auto& form : forms_)
    {
        auto error = csxs::event::EventErrorCode::kEventErrorCode_Success;
        error = form.second->RegisterCSXSEventListeners();
        if (error != csxs::event::EventErrorCode::kEventErrorCode_Success)
            l2a_error("Error in RegisterCSXSEventListeners");
    }
}

/**
 *
 */
void L2A::UI::Manager::RemoveEventListeners()
{
    for (auto& form : forms_)
    {
        auto error = csxs::event::EventErrorCode::kEventErrorCode_Success;
        error = form.second->RemoveEventListeners();
        if (error != csxs::event::EventErrorCode::kEventErrorCode_Success) l2a_error("Error in RemoveEventListeners");
    }
}
