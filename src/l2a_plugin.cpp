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
 * \brief Define a class that represents this plugin.
 */


#include "IllustratorSDK.h"

#include "l2a_plugin.h"

#include "AIMenuCommandNotifiers.h"
#include "SDKErrors.h"
#include "testing.h"

#include "l2a_ai_functions.h"
#include "l2a_annotator.h"
#include "l2a_constants.h"
#include "l2a_error.h"
#include "l2a_global.h"
#include "l2a_item.h"


/**
 */
Plugin* AllocatePlugin(SPPluginRef pluginRef) { return new L2APlugin(pluginRef); }

/**
 */
void FixupReload(Plugin* plugin) { L2APlugin::FixupVTable((L2APlugin*)plugin); }

/*
 */
L2APlugin::L2APlugin(SPPluginRef pluginRef)
    : Plugin(pluginRef),
      fNotifySelectionChanged(nullptr),
      fNotifyDocumentSave(nullptr),
      fNotifyDocumentSaveAs(nullptr),
      fNotifyDocumentOpened(nullptr),
      fResourceManagerHandle(nullptr)
{
    // Set the name that of this plugin in Illustrator.
    strncpy(fPluginName, L2A_PLUGIN_NAME, kMaxStringLength);
}

/*
 */
ASErr L2APlugin::SetGlobal(Plugin* plugin)
{
    L2A::GLOBAL::_l2a_plugin = ((L2APlugin*)plugin);
    return kNoErr;
}

/*
 *
 */
ASErr L2APlugin::Notify(AINotifierMessage* message)
{
    ASErr error = kNoErr;

    try
    {
        if (message->notifier == fNotifySelectionChanged)
        {
            // Selection of art items changed in the document.

            // Invalidate the entire document view bounds.
            annotator_->InvalAnnotation();

            // If the annotator is active, update the item vector.
            annotator_->ArtSelectionChanged();

            // Check if there is a single isolated l2a item.
            AIArtHandle placed_item;
            if (L2A::AI::GetSingleIsolationItem(placed_item))
            {
                // Create Item and change it's contents.
                L2A::Item change_item(placed_item);
                change_item.Change();
            }
        }
        else if (message->notifier == fNotifyDocumentOpened || message->notifier == fNotifyDocumentSave ||
                 message->notifier == fNotifyDocumentSaveAs)
        {
            L2A::AI::UndoActivate();
            L2A::CheckItemDataStructure();
        }
    }
    catch (L2A::ERR::Exception&)
    {
        sAIUser->MessageAlert(ai::UnicodeString("L2APlugin::Notify Error caught."));
    }

    // Check which tool is selected.
    AIToolHandle selected_tool;
    sAITool->GetSelectedTool(&selected_tool);
    if (fToolHandle.size() > 0 &&
        (selected_tool == fToolHandle[1] || selected_tool == fToolHandle[2] || selected_tool == fToolHandle[3]))
    {
        // The redo item is selected. Since this only acts as button we want to deselect the tool here.
        error = sAITool->SetSelectedToolByName(kSelectTool);
        l2a_check_ai_error(error);
    }

    return error;
}

/*
 */
ASErr L2APlugin::Message(char* caller, char* selector, void* message)
{
    ASErr error = kNoErr;
    try
    {
        // Call the message method of the base class to access mouse events e.t.c.
        error = Plugin::Message(caller, selector, message);

        if (error == kUnhandledMsgErr)
        {
            if (strcmp(caller, kCallerAIAnnotation) == 0)
            {
                if (strcmp(selector, kSelectorAIDrawAnnotation) == 0)
                    // Draw the l2a annotator.
                    annotator_->Draw((AIAnnotatorMessage*)message);
                else if (strcmp(selector, kSelectorAIInvalAnnotation) == 0)
                    // Invalidate the annotator.
                    // TODO: maybe this function call can be removed. Check if it is called at all / what the given
                    // invalidate box actually is.
                    annotator_->InvalAnnotation((AIAnnotatorMessage*)message);
            }
        }
        else
            aisdk::check_ai_error(error);
    }
    catch (ai::Error& ex)
    {
        error = ex;
    }
    catch (...)
    {
        error = kCantHappenErr;
    }
    return error;
}

/*
 */
ASErr L2APlugin::StartupPlugin(SPInterfaceMessage* message)
{
    ASErr error = kNoErr;
    try
    {
        ai::int32 pluginOptions = 0;

        // We need to call the base startup method to avoid memory leaks when closing the plugin.
        error = Plugin::StartupPlugin(message);
        aisdk::check_ai_error(error);

        error = sAIPlugin->GetPluginOptions(message->d.self, &pluginOptions);
        aisdk::check_ai_error(error);

        error = sAIPlugin->SetPluginOptions(message->d.self, pluginOptions | kPluginWantsResultsAutoSelectedOption);
        aisdk::check_ai_error(error);

        // Set the global l2a object. This object should only be used with the Get functions in L2A.
        L2A::GLOBAL::_l2a_global = new L2A::GLOBAL::Global();
        L2A::GlobalMutable().SetUp();
        if (L2A::Global().IsSetup())
        {
            error = AddNotifier(message);
            aisdk::check_ai_error(error);

            error = AddTools(message);
            aisdk::check_ai_error(error);

            error = AddAnnotator(message);
            aisdk::check_ai_error(error);

#ifdef _DEBUG
            // In the debug mode perform all unit tests at startup.
            static const bool print_status = false;
            L2A::TEST::TestingMain(print_status);
#endif
        }
    }
    catch (ai::Error& ex)
    {
        error = ex;
    }
    catch (...)
    {
        error = kCantHappenErr;
    }
    return error;
}

/*
 */
ASErr L2APlugin::ShutdownPlugin(SPInterfaceMessage* message)
{
    ASErr error = kNoErr;
    try
    {
        // If it was created, delete the global object.
        if (L2A::GLOBAL::_l2a_global != nullptr) delete L2A::GLOBAL::_l2a_global;

        // Dereference the annotator -> the object will be delete here, otherwise we would have a memory leak later.
        annotator_ = nullptr;

        // We need to call the base shutdown method to avoid memory leaks when closing the plugin.
        error = Plugin::ShutdownPlugin(message);
        aisdk::check_ai_error(error);
    }
    catch (ai::Error& ex)
    {
        error = ex;
    }
    catch (...)
    {
        error = kCantHappenErr;
    }
    return error;
}

/*
 */
ASErr L2APlugin::ToolMouseDown(AIToolMessage* message)
{
    ASErr error = kNoErr;

    if (message->tool == this->fToolHandle[0])
    {
        // Selected tool is item create.

        try
        {
            if (annotator_->IsArtHit())
            {
                ai::UnicodeString undo_string("Undo Change LaTeX2AI item");
                ai::UnicodeString redo_string("Redo Change LaTeX2AI item");
                sAIUndo->SetUndoTextUS(undo_string, redo_string);

                // Get the existing item and change it.
                L2A::Item hit_item(annotator_->GetArtHit());
                hit_item.Change();
            }
            else
            {
                // Check if the current insertion point is locked.
                if (!L2A::AI::GetLockedInsertionPoint())
                {
                    ai::UnicodeString undo_string("Undo Create LaTeX2AI item");
                    ai::UnicodeString redo_string("Redo Create LaTeX2AI item");
                    sAIUndo->SetUndoTextUS(undo_string, redo_string);

                    // Create am item at the clicked position.
                    L2A::Item(message->cursor);
                }
                else
                    sAIUser->MessageAlert(
                        ai::UnicodeString("You tried to create a LaTeX2AI item inside a locked or hidden layer / "
                                          "group. This is not possible."));
            }
        }
        catch (L2A::ERR::Exception&)
        {
            sAIUser->MessageAlert(ai::UnicodeString("L2APlugin::ToolMouseDown Error caught."));
        }
    }

    return error;
}

/*
 */
ASErr L2APlugin::AddTools(SPInterfaceMessage* message)
{
    AIErr error = kNoErr;

    // Set the number of tools.
#ifdef _DEBUG
    static const unsigned int n_tools = 5;
#else
    static const unsigned int n_tools = 4;
#endif

    // Data used to add tool.
    AIAddToolData data;

    // General options for the tool.
    ai::int32 options = kToolWantsToTrackCursorOption;

    // Define the name and icon for the tools.
    char toolGroupName[256];
    std::vector<std::string> tool_title = {"LaTeX2AI create edit mode",  //
        "LaTeX2AI redo items",                                           //
        "LaTeX2AI options",                                              //
        "LaTeX2AI save to PDF",                                          //
        "Perform LaTeX2AI Tests"};
    std::vector<std::string> tool_tip = {
        "LaTeX2AI create edit mode: Create a new LaTeX2AI item, or edit and existing item by clicking on it.",  //
        "LaTeX2AI redo items: Redo multiple items in this document.",                                           //
        "LaTeX2AI options: Set local and global options for LaTeX2AI",                                          //
        "LaTeX2AI save to PDF: Save a copy of the current Illustrator file to a pdf file with the same name.",  //
        "Perform certain tests to ensure LaTeX2AI is working as expected."};

    // Define icons.
    std::vector<ai::uint32> light_icon_id = {TOOL_ICON_CREATE_LIGHT_ID, TOOL_ICON_REDO_LIGHT_ID,
        TOOL_ICON_OPTIONS_LIGHT_ID, TOOL_ICON_SAVE_AS_PDF_LIGHT_ID, TOOL_ICON_TESTING_LIGHT_ID};
    std::vector<ai::uint32> dark_icon_id = {TOOL_ICON_CREATE_DARK_ID, TOOL_ICON_REDO_DARK_ID, TOOL_ICON_OPTIONS_DARK_ID,
        TOOL_ICON_SAVE_AS_PDF_DARK_ID, TOOL_ICON_TESTING_DARK_ID};

    // Create all tools.
    fToolHandle.resize(n_tools);
    for (short i = 0; i < n_tools; i++)
    {
        // Define the name and icon for the tool.
        data.title = ai::UnicodeString(tool_title[i]);
        data.tooltip = ai::UnicodeString(tool_tip[i]);
        data.normalIconResID = light_icon_id[i];
        data.darkIconResID = dark_icon_id[i];
        data.iconType = ai::IconType::kSVG;

        if (i == 0)
        {
            // The first tool creates a new tool palette.
            strcpy(toolGroupName, tool_title[i].c_str());
            // New group on tool palette.
            data.sameGroupAs = kNoTool;
        }
        else
        {
            error = sAITool->GetToolNumberFromName(toolGroupName, &data.sameGroupAs);
            l2a_check_ai_error(error);
        }

        // Each tool icon is on its own.
        data.sameToolsetAs = kNoTool;

        // Add the tool.
        error = sAITool->AddTool(message->d.self, tool_title[i].c_str(), data, options, &fToolHandle[i]);
        l2a_check_ai_error(error);
    }

    return error;
}

/**
 *
 */
ASErr L2APlugin::AddAnnotator(SPInterfaceMessage* message)
{
    ASErr result = kNoErr;
    try
    {
        annotator_ = std::make_unique<L2A::Annotator>(message);
    }
    catch (ai::Error& ex)
    {
        result = ex;
    }
    catch (...)
    {
        result = kCantHappenErr;
    }
    return result;
}

/*
 *
 */
ASErr L2APlugin::AddNotifier(SPInterfaceMessage* /*message*/)
{
    ASErr result = kNoErr;
    try
    {
        result = sAINotifier->AddNotifier(
            fPluginRef, L2A_PLUGIN_NAME, kAIArtSelectionChangedNotifier, &fNotifySelectionChanged);
        aisdk::check_ai_error(result);
        result =
            sAINotifier->AddNotifier(fPluginRef, L2A_PLUGIN_NAME, kAISaveCommandPreNotifierStr, &fNotifyDocumentSave);
        aisdk::check_ai_error(result);
        aisdk::check_ai_error(result);
        result = sAINotifier->AddNotifier(
            fPluginRef, L2A_PLUGIN_NAME, kAISaveAsCommandPostNotifierStr, &fNotifyDocumentSaveAs);
        aisdk::check_ai_error(result);
        result =
            sAINotifier->AddNotifier(fPluginRef, L2A_PLUGIN_NAME, kAIDocumentOpenedNotifier, &fNotifyDocumentOpened);
        aisdk::check_ai_error(result);
    }
    catch (ai::Error& ex)
    {
        result = ex;
    }
    catch (...)
    {
        result = kCantHappenErr;
    }
    return result;
}

/**
 *
 */
ASErr L2APlugin::SelectTool(AIToolMessage* message)
{
    AIErr error = kNoErr;

    if (message->tool == this->fToolHandle[0])
    {
        // Create / edit tool is selected.
        // Activate the annotator.
        annotator_->SetAnnotatorActive();
    }
    else if (message->tool == this->fToolHandle[1] && L2A::AI::GetDocumentCount() > 0)
    {
        // Activate undo.
        ai::UnicodeString undo_string("Undo Update LaTeX2AI item");
        ai::UnicodeString redo_string("Redo Update LaTeX2AI item");
        sAIUndo->SetUndoTextUS(undo_string, redo_string);
        L2A::AI::UndoActivate();

        // Redo tool is selected.
        L2A::RedoItems();

        // Deselect all so a call to the plugin will be given, where the tools can be deselected.
        error = sAIMatchingArt->DeselectAll();
        l2a_check_ai_error(error);
    }
    else if (message->tool == this->fToolHandle[2])
    {
        // Open the global options dialog.
        L2A::GlobalMutable().SetFromUserForm();

        // If a document is opend, deselect all artwork  so a call to the plugin will be given, where the tools can be
        // deselected. And the selection tool will be activated.
        if (L2A::AI::GetDocumentCount() > 0)
        {
            error = sAIMatchingArt->DeselectAll();
            l2a_check_ai_error(error);
        }
    }
    else if (message->tool == this->fToolHandle[3] && L2A::AI::GetDocumentCount() > 0)
    {
        L2A::AI::SaveToPDF();

        // Deselect all so a call to the plugin will be given, where the tools can be deselected.
        error = sAIMatchingArt->DeselectAll();
        l2a_check_ai_error(error);
    }
#ifdef _DEBUG
    else if (message->tool == this->fToolHandle[4])
    {
        // Test the LaTeX2AI framework.
        L2A::TEST::TestFramework();
    }
#endif

    return error;
}

/**
 *
 */
ASErr L2APlugin::DeselectTool(AIToolMessage*)
{
    ASErr result = kNoErr;
    try
    {
        // Deactivate the annotator
        annotator_->SetAnnotatorInactive();
    }
    catch (ai::Error& ex)
    {
        result = ex;
    }
    catch (...)
    {
        result = kCantHappenErr;
    }
    return result;
}

/**
 *
 */
ASErr L2APlugin::TrackToolCursor(AIToolMessage* message)
{
    AIErr error = kNoErr;

    if (message->tool == this->fToolHandle[0])
    {
        // Create edit mode is active.

        // Check if cursor is over an art item.
        ai::int32 cursor_id;
        if (annotator_->CheckForArtHit(message))
            cursor_id = CURSOR_ICON_EDIT;
        else
        {
            if (L2A::AI::GetLockedInsertionPoint())
                cursor_id = CURSOR_ICON_LOCKED;
            else
                cursor_id = CURSOR_ICON_CREATE;
        }
        error = sAIUser->SetSVGCursor(cursor_id, fResourceManagerHandle);
        l2a_check_ai_error(error);
    }

    return error;
}

/**
 *
 */
ASErr L2APlugin::PostStartupPlugin()
{
    AIErr result = kNoErr;
    result = sAIUser->CreateCursorResourceMgr(fPluginRef, &fResourceManagerHandle);
    return result;
}
