$(function () {
    var csInterface = new CSInterface()

    // Set the current skin colors
    updateThemeWithAppSkinInfo(csInterface.hostEnvironment.appSkinInfo)

    // Update the color of the panel when the theme color of the product changed
    csInterface.addEventListener(
        CSInterface.THEME_COLOR_CHANGED_EVENT,
        onAppThemeColorChanged
    )

    // Add all the event listeners we need
    csInterface.addEventListener(
        "com.adobe.csxs.events.latex2ai.redo.update",
        update_form
    )
    csInterface.addEventListener(
        "com.adobe.csxs.events.latex2ai.redo.close",
        csInterface.closeExtension
    )

    // Set the functions for the possible actions on the form
    $("#button_ok").click(function (event) {
        event.preventDefault()
        var event = new CSEvent(
            "com.adobe.csxs.events.latex2ai.redo.ok",
            "APPLICATION",
            "ILST",
            "LaTeX2AIUI"
        )
        event.data = get_form_return_xml_string("ok", true)
        csInterface.dispatchEvent(event)
    })
    $("#button_cancel").click(function (event) {
        event.preventDefault()
        // No callback needed here
        csInterface.closeExtension()
    })

    // let the native plug-in part of this sample know that we are ready to receive events now..
    var panelReadyEvent = new CSEvent(
        "com.adobe.csxs.events.latex2ai.redo.ready",
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
    var xml_document = $.parseXML("<l2a_redo/>")

    // Set the action type
    xml_document.documentElement.setAttribute(
        "action_type",
        $("input[name='action']:checked").val()
    )

    // Set the action type
    xml_document.documentElement.setAttribute(
        "items",
        $("input[name='items']:checked").val()
    )

    // Return the created xml document
    return xml_document
}

function update_form(event) {
    var xmlData = $.parseXML(event.data)
    var $xml = $(xmlData)

    check_git_hash($xml)

    var redo_xml = $xml.find("form_data")
    $("#items_all_label").prop(
        "innerHTML",
        "All Items (" + redo_xml.attr("n_all_items") + ")"
    )
    $("#items_selected_label").prop(
        "innerHTML",
        "Selected Items (" + redo_xml.attr("n_selected_items") + ")"
    )
}
