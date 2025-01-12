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
 * \brief Define a class that represents this plugin.
 */


#ifndef L2A_PLUGIN_H_
#define L2A_PLUGIN_H_


#include "Plugin.hpp"

#include "l2a_annotator.h"
#include "l2a_ui_manager.h"


//! Check if the Illustrator version is supported.
//! The only reason we define the max value here is so the user is aware that a new SDK version is being used.
#define L2A_ILLUSTRATOR_VERSION_MIN 0x18000001
#define L2A_ILLUSTRATOR_VERSION_MAX 0x29000001

#if (kPluginInterfaceVersion < L2A_ILLUSTRATOR_VERSION_MIN) || (kPluginInterfaceVersion >= L2A_ILLUSTRATOR_VERSION_MAX)
#error Currently LaTeX2AI only supports Illustrator CS6, Illustrator CC2017, Illustrator CC2018, Illustrator 2021, Illustrator 2022, Illustrator 2023 and Illustrator 2024
#endif


/**
 * \brief Creates a new L2APlugin.
 * @param pluginRef IN unique reference to this plugin.
 * @return pointer to new L2APlugin.
 */
Plugin* AllocatePlugin(SPPluginRef pluginRef);

/**
 * \brief Reloads the L2APlugin class state when the plugin is reloaded by the application.
 * @param plugin IN pointer to plugin being reloaded.
 */
void FixupReload(Plugin* plugin);

/**
 * \brief Plugin to add LaTeX text to Adobe Illustrator.
 */
class L2APlugin : public Plugin
{
   public:
    /**
     * \brief Constructor.
     * @param pluginRef IN reference to this plugin.
     */
    L2APlugin(SPPluginRef pluginRef);

    /**
     * \brief Destructor.
     */
    ~L2APlugin() {};

    /**
     * \brief Restores state of L2APlugin during reload.
     */
    FIXUP_VTABLE_EX(L2APlugin, Plugin);

    /**
     * \brief Call the plugin for certain notifier messages.
     */
    virtual ASErr Notify(AINotifierMessage* message);

   public:
    /**
     * \brief Return a reference to the UI manager
     */
    L2A::UI::Manager& GetUiManager() { return *ui_manager_; }

   protected:
    /**
     * \brief Set a link to this plugin in the global object
     */
    ASErr SetGlobal(Plugin* plugin);

    /**
     * \brief Calls Plugin::Message and handles any errors returned.
     * @param caller IN sender of the message.
     * @param selector IN nature of the message.
     * @param message IN pointer to plugin and call information.
     * @return kNoErr on success, other ASErr otherwise.
     */
    virtual ASErr Message(char* caller, char* selector, void* message);

    /**
     * \brief Calls Plugin::Startup and initialisation functions, such as AddTools and AddMenus.
     * @param message IN pointer to plugin and call information.
     * @return kNoErr on success, other ASErr otherwise.
     */
    virtual ASErr StartupPlugin(SPInterfaceMessage* message);

    /**
     * \brief Removes the plugin.
     * @param message IN message sent by the plugin manager.
     * @return kNoErr on success, other ASErr otherwise.
     */
    virtual ASErr ShutdownPlugin(SPInterfaceMessage* message);

    /**
     * \brief Is called when the mouse button is pressed.
     * @param message IN pointer to plugin and call information.
     * @return kNoErr on success, other ASErr otherwise.
     */
    virtual ASErr ToolMouseDown(AIToolMessage* message);

    /**
     * \brief Registers this plug-in to receive the selection changed notifier.
     * @param message IN message data.
     * @return kNoErr on success, other ASErr otherwise.
     */
    virtual ASErr AddNotifier(SPInterfaceMessage* message);

    /**
     * \brief Is called when a tool is selected.
     * @param message IN message data.
     * @return kNoErr on success, other ASErr otherwise.
     */
    virtual ASErr SelectTool(AIToolMessage* message);

    /**
     * \brief Sets the platform cursor to the icon for the current tool selected.
     * @param message IN pointer to plugin and call information.
     * @return kNoErr on success, other ASErr otherwise.
     */
    virtual ASErr TrackToolCursor(AIToolMessage* message);

    /**
     * \brief Is called when a tool is deselected.
     * @param message IN message data.
     * @return kNoErr on success, other ASErr otherwise.
     */
    virtual ASErr DeselectTool(AIToolMessage* message);

   private:
    /**
     * \brief Adds the tools for this plugin to the application toolbar.
     * @param message IN pointer to plugin and call information.
     * @return kNoErr on success, other ASErr otherwise.
     */
    ASErr AddTools(SPInterfaceMessage* message);

    /**
     * \brief Adds an annotator.
     * @param message IN message data.
     * @return kNoErr on success, other ASErr otherwise.
     */
    ASErr AddAnnotator(SPInterfaceMessage* message);

    /**
     * \brief Performs plugin tasks that could not be performed until
     * the application was started.
     * @return kNoErr on success, other ASErr otherwise.
     */
    ASErr PostStartupPlugin();

   private:
    //! Store handle for each tool of the plugin
    std::vector<AIToolHandle> tool_handles_;

    //! Handle for the selection changed notifier
    AINotifierHandle notify_selection_changed_;

    //! Handle for document actions.
    AINotifierHandle notify_document_save_;
    AINotifierHandle notify_document_save_as_;
    AINotifierHandle notify_active_doc_view_title_changed_;

    //! Handle for plug plug actions
    AINotifierHandle notify_CSXS_plugplug_setup_complete_;

    //! Handle for the resource manager added by this plug-in used for setting cursor
    AIResourceManagerHandle resource_manager_handle_;

    //! Annotator object
    std::unique_ptr<L2A::Annotator> annotator_;

    //! User Interface manager
    std::unique_ptr<L2A::UI::Manager> ui_manager_;
};

#endif  // L2A_PLUGIN_H_
