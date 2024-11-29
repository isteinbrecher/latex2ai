$(function () {
    var csInterface = new CSInterface()

    // Set the current skin colors
    updateThemeWithAppSkinInfo(csInterface.hostEnvironment.appSkinInfo)

    // On Windows we add the notification that the path to the executables can be left empty
    var is_mac = CSInterface.prototype
        .getOSInformation()
        .toLowerCase()
        .includes("mac")
    if (is_mac) {
        $("#tex_folder_label").prop(
            "innerHTML",
            "Folder with LaTeX executables"
        )
    } else {
        $("#tex_folder_label").prop(
            "innerHTML",
            "Folder with LaTeX executables (may be left empty)"
        )
    }

    // Update the color of the panel when the theme color of the product changed
    csInterface.addEventListener(
        CSInterface.THEME_COLOR_CHANGED_EVENT,
        onAppThemeColorChanged
    )

    // Add all the event listeners we need
    csInterface.addEventListener(
        "com.adobe.csxs.events.latex2ai.options.update",
        update_form
    )
    csInterface.addEventListener(
        "com.adobe.csxs.events.latex2ai.options.close",
        csInterface.closeExtension
    )

    // Set the functions for the possible actions on the form
    $("#button_save_and_exit").click(function (event) {
        // Send data to main application to store in global object
        event.preventDefault()
        var event = new CSEvent(
            "com.adobe.csxs.events.latex2ai.options.save",
            "APPLICATION",
            "ILST",
            "LaTeX2AIUI"
        )
        event.data = get_form_return_xml_string(null, true)
        csInterface.dispatchEvent(event)
    })
    $("#button_cancel").click(function (event) {
        // Close the form without any actions
        event.preventDefault()
        // No callback needed here
        csInterface.closeExtension()
    })
    $("#button_default").click(function (event) {
        // Request the default options from the main application
        event.preventDefault()
        var event = new CSEvent(
            "com.adobe.csxs.events.latex2ai.options.get_default_values",
            "APPLICATION",
            "ILST",
            "LaTeX2AIUI"
        )
        csInterface.dispatchEvent(event)
    })
    $("#button_header").click(function (event) {
        // Depending on the state of the document, open the header or create the default header
        event.preventDefault()

        var button = $("#button_header")
        if (button.val() == "Open") {
            var event = new CSEvent(
                "com.adobe.csxs.events.latex2ai.options.open_header",
                "APPLICATION",
                "ILST",
                "LaTeX2AIUI"
            )
        } else if (button.val() == "Create default") {
            var event = new CSEvent(
                "com.adobe.csxs.events.latex2ai.options.create_default_header",
                "APPLICATION",
                "ILST",
                "LaTeX2AIUI"
            )
        } else {
            alert("Button has unexpected value")
        }
        csInterface.dispatchEvent(event)
    })
    $("#button_folder_tex").click(function (event) {
        // Ask the user to select the tex folder. Send all the data, so we can open the form at the currently set folder
        event.preventDefault()
        var event = new CSEvent(
            "com.adobe.csxs.events.latex2ai.options.select_tex",
            "APPLICATION",
            "ILST",
            "LaTeX2AIUI"
        )
        event.data = get_form_return_xml_string(null, true)
        csInterface.dispatchEvent(event)
    })
    $("#button_command_gs").click(function (event) {
        // Ask the user to select the gs command. Send all the data, so we can open the form at the currently set folder
        event.preventDefault()
        var event = new CSEvent(
            "com.adobe.csxs.events.latex2ai.options.select_gs",
            "APPLICATION",
            "ILST",
            "LaTeX2AIUI"
        )
        event.data = get_form_return_xml_string(null, true)
        csInterface.dispatchEvent(event)
    })

    // let the native plug-in part of this sample know that we are ready to receive events now..
    var panelReadyEvent = new CSEvent(
        "com.adobe.csxs.events.latex2ai.options.ready",
        "APPLICATION",
        "ILST",
        "LaTeX2AIUI"
    )
    csInterface.dispatchEvent(panelReadyEvent)
})

function get_form_return_xml_string(return_value, add_item_data) {
    var root_xml_document = $.parseXML("<form_return/>")
    if (return_value != null) {
        root_xml_document.documentElement.setAttribute(
            "return_value",
            return_value
        )
    }
    if (add_item_data) {
        root_xml_document.documentElement.appendChild(
            form_content_to_xml().documentElement
        )
    }
    return new XMLSerializer().serializeToString(root_xml_document)
}

function form_content_to_xml() {
    // Convert the item contents to an xml object that can be send back to the main plugin
    var xml_document = $.parseXML("<l2a_options/>")

    // Set the options
    xml_document.documentElement.setAttribute(
        "tex_engine",
        $("#tex_engine").prop("value")
    )
    xml_document.documentElement.setAttribute(
        "latex_command_options",
        $("#tex_options").prop("value")
    )
    xml_document.documentElement.setAttribute(
        "latex_bin_path",
        $("#tex_folder").prop("value")
    )
    xml_document.documentElement.setAttribute(
        "gs_command",
        $("#gs_command").prop("value")
    )
    xml_document.documentElement.setAttribute(
        "item_ui_finish_on_enter",
        bool_to_string($("#item_ui_finish_on_enter").prop("checked"))
    )
    xml_document.documentElement.setAttribute(
        "warning_boundary_boxes",
        bool_to_string($("#warning_save_boundary_box").prop("checked"))
    )
    xml_document.documentElement.setAttribute(
        "warning_ai_not_saved",
        bool_to_string($("#warning_save_illustrator").prop("checked"))
    )

    return xml_document
}

function update_form(event) {
    // Update the items in the form
    // We do this by simply checking which items are given and set them accordingly
    var xmlData = $.parseXML(event.data)
    var $xml = $(xmlData)
    check_git_hash($xml)

    var form_data = $xml.find("form_data")

    // Set the LaTeX2AI information
    var latex2ai_info = form_data.find("latex2ai_info")
    if (latex2ai_info.length > 0) {
        $("#l2a_version").prop("innerHTML", latex2ai_info.attr("version"))
        $("#l2a_git_sha").prop("innerHTML", latex2ai_info.attr("git_sha"))
    }

    // Set the LaTeX2AI options
    var latex2ai_data = form_data.find("latex2ai_options")
    if (latex2ai_data.length > 0) {
        // TeX stuff
        if_found_update_value(latex2ai_data, "gs_command", "gs_command")
        if_found_update_value(latex2ai_data, "latex_bin_path", "tex_folder")
        if_found_update_value(
            latex2ai_data,
            "latex_command_options",
            "tex_options"
        )
        if_found_update_value(latex2ai_data, "latex_engine", "tex_engine")

        // Item creation options
        if_found_update_checkbox(
            latex2ai_data,
            "item_ui_finish_on_enter",
            "item_ui_finish_on_enter"
        )

        // Warnings
        if_found_update_checkbox(
            latex2ai_data,
            "warning_boundary_boxes",
            "warning_save_boundary_box"
        )
        if_found_update_checkbox(
            latex2ai_data,
            "warning_ai_not_saved",
            "warning_save_illustrator"
        )
    }

    // Set the header stuff
    var header_xml = form_data.find("document_header")
    if (header_xml.length > 0) {
        var document_state = header_xml.attr("document_state")
        var label = $("#header_label")
        var button = $("#button_header")
        if (document_state == "saved") {
            var header_path = header_xml.attr("header_path")
            if (header_path == "") {
                label.prop("value", "No LaTeX2AI header created yet")
                label.prop("className", "fake_label")
                button.val("Create default")
            } else {
                label.prop("value", header_path)
                label.prop("className", "")
                button.val("Open")
            }
            button.show()
        } else if (document_state == "not_saved") {
            label.prop(
                "value",
                "Current Adobe Illustrator document is not saved"
            )
            label.prop("className", "fake_label")
            button.val("")
            button.hide()
        } else if (document_state == "no_documents") {
            label.prop("value", "No opened Adobe Illustrator documents")
            label.prop("className", "fake_label")
            button.val("")
            button.hide()
        }
    }
}

function if_found_update_value(xml, xml_name, html_id) {
    // Check if the xml_name attribute is present, if so, set the value of the html item
    var attribute = xml.attr(xml_name)
    if (attribute !== undefined) {
        $("#" + html_id).prop("value", attribute)
    }
}

function if_found_update_checkbox(xml, xml_name, html_id) {
    // Check if the xml_name attribute is present, if so, set the html checkbox
    var attribute = xml.attr(xml_name)
    if (attribute !== undefined) {
        if (attribute == "1") {
            $("#" + html_id).prop("checked", true)
        } else if (attribute == "0") {
            $("#" + html_id).prop("checked", false)
        } else {
            alert(
                'Got unexpected value in if_found_update_checkbox "' +
                    attribute +
                    '"'
            )
        }
    }
}

function bool_to_string(value) {
    if (value == true) return "1"
    else if (value == false) return "0"
    else alert("Got unexpected value in bool_to_string")
}
