/**
 * Update the theme with the AppSkinInfo retrieved from the host product.
 */
function updateThemeWithAppSkinInfo(appSkinInfo) {
    //Update the background color of the panel
    var panelBackgroundColor = appSkinInfo.panelBackgroundColor.color
    document.body.bgColor = toHex(panelBackgroundColor)

    var styleId = "hostStyle"
    addRule(styleId, "body", "color:" + reverseColor(panelBackgroundColor))
}

function addRule(stylesheetId, selector, rule) {
    var stylesheet = document.getElementById(stylesheetId)

    if (stylesheet) {
        stylesheet = stylesheet.sheet
        if (stylesheet.addRule) {
            stylesheet.addRule(selector, rule)
        } else if (stylesheet.insertRule) {
            stylesheet.insertRule(
                selector + " { " + rule + " }",
                stylesheet.cssRules.length
            )
        }
    }
}

function reverseColor(color, delta) {
    return toHex(
        {
            red: Math.abs(255 - color.red),
            green: Math.abs(255 - color.green),
            blue: Math.abs(255 - color.blue),
        },
        delta
    )
}

/**
 * Convert the Color object to string in hexadecimal format;
 */
function toHex(color, delta) {
    function computeValue(value, delta) {
        var computedValue = !isNaN(delta) ? value + delta : value
        if (computedValue < 0) {
            computedValue = 0
        } else if (computedValue > 255) {
            computedValue = 255
        }

        computedValue = computedValue.toString(16)
        return computedValue.length == 1 ? "0" + computedValue : computedValue
    }

    var hex = ""
    if (color) {
        with (color) {
            hex =
                computeValue(red, delta) +
                computeValue(green, delta) +
                computeValue(blue, delta)
        }
    }
    return "#" + hex
}

function onAppThemeColorChanged(event) {
    // Should get a latest HostEnvironment object from application.
    var skinInfo = JSON.parse(
        window.__adobe_cep__.getHostEnvironment()
    ).appSkinInfo
    // Gets the style information such as color info from the skinInfo,
    // and redraw all UI controls of your extension according to the style info.
    updateThemeWithAppSkinInfo(skinInfo)
}

function evalScript(script, callback) {
    new CSInterface().evalScript(script, callback)
}

function check_git_hash(xml) {
    const plugin_hash = xml.find("full_data").attr("git_hash")
    const ui_hash = get_git_sha()
    if (plugin_hash != ui_hash) {
        alert(
            "The git hash of the LaTeX2AI plugin and the UI does not match! Did you forget to update one of them?"
            + "\nThe plugin hash is: " + plugin_hash.slice(0, 8)
            + "\nThe UI hash is: " + ui_hash.slice(0, 8)
        )
    }
}
