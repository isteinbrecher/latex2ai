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
        "com.adobe.csxs.events.latex2ai.debug.update",
        update_create_form
    )
    csInterface.addEventListener(
        "com.adobe.csxs.events.latex2ai.debug.close",
        csInterface.closeExtension
    )

    // Set the functions for the possible actions on the form
    $("#button_ok").click(function (event) {
        event.preventDefault()
        var event = new CSEvent(
            "com.adobe.csxs.events.latex2ai.debug.ok",
            "APPLICATION",
            "ILST",
            "LaTeX2AIUI"
        )
        event.data = ""
        csInterface.dispatchEvent(event)
    })

    $("#button_open_log").click(function (event) {
        event.preventDefault()
        var event = new CSEvent(
            "com.adobe.csxs.events.latex2ai.debug.open_log",
            "APPLICATION",
            "ILST",
            "LaTeX2AIUI"
        )
        event.data = ""
        csInterface.dispatchEvent(event)
    })

    $("#button_create_debug").click(function (event) {
        event.preventDefault()
        var event = new CSEvent(
            "com.adobe.csxs.events.latex2ai.debug.create_debug",
            "APPLICATION",
            "ILST",
            "LaTeX2AIUI"
        )
        event.data = ""
        csInterface.dispatchEvent(event)
    })

    $("#button_cancel").click(function (event) {
        event.preventDefault()
        var event = new CSEvent(
            "com.adobe.csxs.events.latex2ai.debug.cancel",
            "APPLICATION",
            "ILST",
            "LaTeX2AIUI"
        )
        event.data = ""
        csInterface.dispatchEvent(event)
    })
    document.addEventListener("keydown", (event) => {
        if (event.key === "Escape") {
            event.preventDefault()
            var event = new CSEvent(
                "com.adobe.csxs.events.latex2ai.debug.cancel",
                "APPLICATION",
                "ILST",
                "LaTeX2AIUI"
            )
            event.data = ""
            csInterface.dispatchEvent(event)
        }
    })

    // let the native plug-in part of this sample know that we are ready to receive events now..
    var panelReadyEvent = new CSEvent(
        "com.adobe.csxs.events.latex2ai.debug.ready",
        "APPLICATION",
        "ILST",
        "LaTeX2AIUI"
    )
    csInterface.dispatchEvent(panelReadyEvent)
})

function update_create_form(event) {
    var xmlData = $.parseXML(event.data)
    var $xml = $(xmlData)

    check_git_hash($xml)

    var l2a_xml = $xml.find("form_data")

    is_redo = l2a_xml.attr("action") == "redo_items"
    if (is_redo) {
        $("#button_ok").prop("disabled", true)
    }
}
