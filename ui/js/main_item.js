// Set global variables to check is something changed when closing the form
var placed_option_input = null
var text_align_horizontal_input = null
var text_align_vertical_input = null
var latex_text_input = null
var cursor_position_input = null

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
        "com.adobe.csxs.events.latex2ai.item.update",
        update_create_item
    )
    csInterface.addEventListener(
        "com.adobe.csxs.events.latex2ai.item.close",
        csInterface.closeExtension
    )

    document.addEventListener("keydown", (event) => {
        if (event.shiftKey && event.key === "Enter") {
            event.preventDefault()

            if (!(document.activeElement.id === "latex_text")) {
                return
            }

            // Add newline to the text form
            // Get the txt pre and post selection
            let selection_start = $("#latex_text").prop("selectionStart")
            let selection_end = $("#latex_text").prop("selectionEnd")
            let text = $("#latex_text").val()
            let pre = text.substring(0, selection_start)
            let post = text.substring(selection_end)
            $("#latex_text").val(pre + "\n" + post)
            $("#latex_text").prop("selectionStart", selection_start + 1)
            $("#latex_text").prop("selectionEnd", selection_start + 1)
        } else if (event.key === "Enter") {
            event.preventDefault()
            var event = new CSEvent(
                "com.adobe.csxs.events.latex2ai.item.ok",
                "APPLICATION",
                "ILST",
                "LaTeX2AIUI"
            )
            event.data = get_form_return_xml_string("ok", true)
            csInterface.dispatchEvent(event)
        } else if (event.key === "Escape") {
            event.preventDefault()
            csInterface.closeExtension()
        }
    })

    // Set the functions for the possible actions on the form
    $("#button_ok").click(function (event) {
        event.preventDefault()
        var event = new CSEvent(
            "com.adobe.csxs.events.latex2ai.item.ok",
            "APPLICATION",
            "ILST",
            "LaTeX2AIUI"
        )
        event.data = get_form_return_xml_string("ok", true)
        csInterface.dispatchEvent(event)
    })
    $("#button_redo_latex").click(function (event) {
        event.preventDefault()
        var event = new CSEvent(
            "com.adobe.csxs.events.latex2ai.item.ok",
            "APPLICATION",
            "ILST",
            "LaTeX2AIUI"
        )
        event.data = get_form_return_xml_string("redo_latex", true)
        csInterface.dispatchEvent(event)
    })
    $("#button_redo_boundary").click(function (event) {
        event.preventDefault()
        var event = new CSEvent(
            "com.adobe.csxs.events.latex2ai.item.ok",
            "APPLICATION",
            "ILST",
            "LaTeX2AIUI"
        )
        event.data = get_form_return_xml_string("redo_boundary", true)
        csInterface.dispatchEvent(event)
    })

    $("#button_cancel").click(function (event) {
        event.preventDefault()
        // No callback needed here
        csInterface.closeExtension()
    })

    // let the native plug-in part of this sample know that we are ready to receive events now..
    var panelReadyEvent = new CSEvent(
        "com.adobe.csxs.events.latex2ai.item.ready",
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
            item_content_to_xml().documentElement
        )
    }
    return new XMLSerializer().serializeToString(root_xml_document)
}

function item_content_to_xml() {
    // Convert the item contents to an xml object that can be send back to the main plugin
    var xml_document = $.parseXML("<l2a_item/>")

    // Set the LaTeX code and the cursor position
    var latex = xml_document.createElement("latex")
    latex.appendChild(xml_document.createTextNode($("#latex_text").val()))
    latex.setAttribute("cursor_position", $("#latex_text").prop("selectionEnd"))
    xml_document.documentElement.appendChild(latex)

    // Set the placement option
    // Get the placement check box id
    var placement_id = $('input[name="item_placement"]:checked').attr("id")
    let placement_id_split = placement_id.split("_")
    xml_document.documentElement.setAttribute(
        "text_align_vertical",
        placement_id_split[1]
    )
    xml_document.documentElement.setAttribute(
        "text_align_horizontal",
        placement_id_split[2]
    )

    var placed_option
    switch ($("#stretch_behaviour").prop("value")) {
        case "stretch":
            placed_option = "fill_to_boundary_box"
            break
        case "original":
            placed_option = "keep_scale"
            break
        case "original_clip":
            placed_option = "keep_scale_clip"
            break
        default:
        // Could not match the string
    }
    xml_document.documentElement.setAttribute("placed_option", placed_option)

    // Return the created xml document
    return xml_document
}

function update_create_item(event) {
    var xmlData = $.parseXML(event.data)
    var $xml = $(xmlData)

    var l2a_xml = $xml.find("LaTeX2AI_item")

    // Store the variables coming from the main plugin
    placed_option_input = l2a_xml.attr("placed_option")
    text_align_horizontal_input = l2a_xml.attr("text_align_horizontal")
    text_align_vertical_input = l2a_xml.attr("text_align_vertical")
    latex_text_input = l2a_xml.find("latex").text()
    cursor_position_input = l2a_xml.find("latex").attr("cursor_position")

    // Set the text align
    $(
        "#placement_" +
            text_align_vertical_input +
            "_" +
            text_align_horizontal_input
    ).prop("checked", true)

    // Set the placed item option
    switch (placed_option_input) {
        case "fill_to_boundary_box":
            $("#stretch_behaviour").prop("value", "stretch")
            break
        case "keep_scale":
            $("#stretch_behaviour").prop("value", "original")
            break
        case "keep_scale_clip":
            $("#stretch_behaviour").prop("value", "original_clip")
            break
        default:
        // Could not match the string
    }

    // Activate / Deactivate the buttons
    if (xmlData.documentElement.getAttribute("latex_exists") == "1") {
        $("#button_redo_latex").prop("disabled", false)
        $("#button_redo_boundary").prop("disabled", false)
        let boundary_box_state =
            xmlData.documentElement.getAttribute("boundary_box_state")
        $("#boundary_box_state").prop(
            "innerHTML",
            "Boundary box status: " + boundary_box_state
        )
        if (boundary_box_state == "ok") {
            $("#button_redo_boundary").prop("disabled", true)
        }
    } else {
        $("#button_redo_latex").prop("disabled", true)
        $("#button_redo_boundary").prop("disabled", true)
        $("#boundary_state").prop("innerHTML", "none")
    }

    // Set the item text
    $("#latex_text").val(latex_text_input)

    // We have to show the form before we can set the focus
    $("#dashForm").show()
    $("#latex_text").focus()
    $("#latex_text").prop("selectionEnd", cursor_position_input)
}
