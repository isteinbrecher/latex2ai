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
 * \brief Base class for a UI form
 */

#include "IllustratorSDK.h"

#include "l2a_ui_base.h"


/**
 *
 */
void L2A::UI::FormBase::SetEventListeners(std::vector<EventListenerData>& event_listener_data)
{
    if (event_listener_data.size() == 0) l2a_error("SetEventListeners requires at least one event listener");
    event_listener_data_ = std::move(event_listener_data);
}

/**
 *
 */
csxs::event::EventErrorCode L2A::UI::FormBase::RegisterCSXSEventListeners()
{
    csxs::event::EventErrorCode result = csxs::event::kEventErrorCode_Success;
    for (auto& event_data : event_listener_data_)
    {
        if (event_data.has_context_)
        {
            result = htmlPPLib.AddEventListener(event_data.event_id_.c_str(), event_data.event_listener_, this);
        }
        else
        {
            result = htmlPPLib.AddEventListener(event_data.event_id_.c_str(), event_data.event_listener_, nullptr);
        }

        if (result != csxs::event::kEventErrorCode_Success)
        {
            return result;
        }
    }
    return result;
}

/**
 *
 */
csxs::event::EventErrorCode L2A::UI::FormBase::RemoveEventListeners()
{
    csxs::event::EventErrorCode result = csxs::event::kEventErrorCode_Success;
    for (auto& event_data : event_listener_data_)
    {
        if (event_data.has_context_)
        {
            result = htmlPPLib.RemoveEventListener(event_data.event_id_.c_str(), event_data.event_listener_, this);
        }
        else
        {
            result = htmlPPLib.RemoveEventListener(event_data.event_id_.c_str(), event_data.event_listener_, nullptr);
        }

        if (result != csxs::event::kEventErrorCode_Success)
        {
            return result;
        }
    }

    return result;
}
