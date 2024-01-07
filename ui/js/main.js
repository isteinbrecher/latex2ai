// Set global variables to check is something changed when closing the form
var placed_option_input = null;
var text_align_horizontal_input = null;
var text_align_vertical_input = null;
var latex_text_input = null;
var cursor_position_input = null;


$(function () {
	var csInterface = new CSInterface();

	// Set the current skin colors
	updateThemeWithAppSkinInfo(csInterface.hostEnvironment.appSkinInfo);

	// Update the color of the panel when the theme color of the product changed
	csInterface.addEventListener(CSInterface.THEME_COLOR_CHANGED_EVENT, onAppThemeColorChanged);

	// Add all the event listeners we need
	csInterface.addEventListener('com.adobe.csxs.events.latex2ai.item.update', update_create_item);
	csInterface.addEventListener('com.adobe.csxs.events.latex2ai.item.close', csInterface.closeExtension);


	document.addEventListener("keydown", (event) => {
		if (event.shiftKey && event.key === "Enter" && document.activeElement.id === "latex_text") {
			$('#latex_text').val(document.activeElement.id);
		}
		else if (event.key === "Enter") {
			event.preventDefault();
			var event = new CSEvent('com.adobe.csxs.events.latex2ai.item.compile', 'APPLICATION', 'ILST', 'LaTeX2AIUI');
			event.data = get_form_return_xml_string("ok", true);
			csInterface.dispatchEvent(event);
		}

	});

	// Set the functions for the possible actions on the form
	$('#button_ok').click(function (event) {
		event.preventDefault();
		var event = new CSEvent('com.adobe.csxs.events.latex2ai.item.compile', 'APPLICATION', 'ILST', 'LaTeX2AIUI');
		event.data = get_form_return_xml_string("ok", true);
		csInterface.dispatchEvent(event);
	});
	$('#button_redo_latex').click(function (event) {
		event.preventDefault();
		var event = new CSEvent('com.adobe.csxs.events.latex2ai.item.compile', 'APPLICATION', 'ILST', 'LaTeX2AIUI');
		event.data = get_form_return_xml_string("redo_latex", true);
		csInterface.dispatchEvent(event);
	});
	$('#button_redo_boundary').click(function (event) {
		event.preventDefault();
		var event = new CSEvent('com.adobe.csxs.events.latex2ai.item.compile', 'APPLICATION', 'ILST', 'LaTeX2AIUI');
		event.data = get_form_return_xml_string("redo_boundary", true);
		csInterface.dispatchEvent(event);
	});

	// Set the functions to update the "clip" check box
	$('#scaling_stretch').click(function (event) { update_checkbox_clip(); });
	$('#scaling_remain').click(function (event) { update_checkbox_clip(); });

	$('#button_cancel').click(function (event) {
		event.preventDefault();
		// No callback needed here
		csInterface.closeExtension();
	});

	// let the native plug-in part of this sample know that we are ready to receive events now..	
	var panelReadyEvent = new CSEvent('com.adobe.csxs.events.latex2ai.item.ready', 'APPLICATION', 'ILST', 'LaTeX2AIUI');
	csInterface.dispatchEvent(panelReadyEvent);
});


function update_checkbox_clip() {
	// Activate / Deactivate the clip checkbox based on the selection of the scaling radio box
	if ($("#scaling_stretch").prop("checked")) {
		$("#scaling_clip").prop("disabled", true);
	}
	else {
		$("#scaling_clip").prop("disabled", false);
	}
}

function get_form_return_xml_string(return_value, add_item_data) {
	var root_xml_document = $.parseXML("<form_return/>");
	if (return_value != null) {
		root_xml_document.documentElement.setAttribute("return_value", return_value);
	}
	if (add_item_data) {
		root_xml_document.documentElement.appendChild(item_content_to_xml().documentElement);
	}
	return new XMLSerializer().serializeToString(root_xml_document);
}

function item_content_to_xml() {
	// Convert the item contents to an xml object that can be send back to the main plugin
	var xml_document = $.parseXML("<l2a_item/>");

	// Set the LaTeX code and the cursor position
	var latex = xml_document.createElement("latex");
	latex.appendChild(xml_document.createTextNode($('#latex_text').val()));
	latex.setAttribute("cursor_position", $('#latex_text').prop('selectionEnd'));
	xml_document.documentElement.appendChild(latex);

	// Set the placement option
	// Get the placement check box id
	var placement_id = $('input[name="placement"]:checked').attr('id');
	let placement_id_split = placement_id.split("_");
	xml_document.documentElement.setAttribute("text_align_vertical", placement_id_split[1]);
	xml_document.documentElement.setAttribute("text_align_horizontal", placement_id_split[2]);

	var placed_option;
	if ($("#scaling_stretch").prop("checked")) {
		placed_option = "fill_to_boundary_box";
	}
	else {
		if ($("#scaling_clip").prop("checked")) {
			placed_option = "keep_scale_clip";
		}
		else {
			placed_option = "keep_scale";
		}
	}
	xml_document.documentElement.setAttribute("placed_option", placed_option);

	// Return the created xml document
	return xml_document;
};

function update_create_item(event) {

	var xmlData = $.parseXML(event.data);
	var $xml = $(xmlData);

	// TODO: To check if item could be found
	//if ($xml.find("item").length > 0) {


	var l2a_xml = $xml.find('LaTeX2AI_item');

	// Store the variables coming from the main plugin
	placed_option_input = l2a_xml.attr('placed_option');
	text_align_horizontal_input = l2a_xml.attr('text_align_horizontal');
	text_align_vertical_input = l2a_xml.attr('text_align_vertical');
	latex_text_input = l2a_xml.find('latex').text();
	cursor_position_input = l2a_xml.find('latex').attr('cursor_position');

	// Set the text align
	$('#placement_' + text_align_vertical_input + "_" + text_align_horizontal_input).prop('checked', true);

	// Set the placed item option
	switch (placed_option_input) {
		case "fill_to_boundary_box":
			$("#scaling_stretch").prop("checked", true);
			break;
		case "keep_scale":
			$("#scaling_remain").prop("checked", true);
			$("#scaling_clip").prop("checked", false);
			break;
		case "keep_scale_clip":
			$("#scaling_remain").prop("checked", true);
			$("#scaling_clip").prop("checked", true);
			break;
		default:
		// Could not match the string
	}
	update_checkbox_clip()

	// Activate / Deactivate the buttons
	if (xmlData.documentElement.getAttribute("latex_exists") == "1") {
		$("#button_redo_latex").prop("disabled", false);
		$("#button_redo_boundary").prop("disabled", false);
		$("#boundary_state").prop("innerHTML", xmlData.documentElement.getAttribute("boundary_box_state"));
	}
	else {
		$("#button_redo_latex").prop("disabled", true);
		$("#button_redo_boundary").prop("disabled", true);
		$("#boundary_state").prop("innerHTML", "none");
	}

	// Set the item text
	$('#latex_text').val(latex_text_input);

	// We have to show the form before we can set the focus
	$('#dashForm').show();
	$('#latex_text').focus();
	$('#latex_text').prop('selectionEnd', cursor_position_input);



	// <?xml version="1.0"?>
	//<LaTeX2AI_item placed_option="fill_to_boundary_box" text_align_horizontal="centreH" text_align_vertical="centreV">
	//   <latex cursor_position="0">$a^2+b^2=c^2$</latex>
	// </LaTeX2AI_item>







	//	var dashedStroke = ($xml.find('dashed').text() == 'true') ? true : false;
	//	var adjustDash = ($xml.find('adjust').text() == 'true') ? true : false;
	//	var dash1 = $xml.find('dash1').text();
	//	var dash2 = $xml.find('dash2').text();
	//	var dash3 = $xml.find('dash3').text();
	//	var gap1 = $xml.find('gap1').text();
	//	var gap2 = $xml.find('gap2').text();
	//	var gap3 = $xml.find('gap3').text();

	//$('#waveForm').hide();





	//	



	//$('#dash1').val(dash1);
	//$('#dash2').val(dash2);
	//$('#dash3').val(dash3);
	//$('#gap1').val(gap1);
	//$('#gap2').val(gap2);
	//$('#gap3').val(gap3);
}







/**
 * Update the theme with the AppSkinInfo retrieved from the host product.
 */
function updateThemeWithAppSkinInfo(appSkinInfo) {
	//Update the background color of the panel
	var panelBackgroundColor = appSkinInfo.panelBackgroundColor.color;
	document.body.bgColor = toHex(panelBackgroundColor);

	var styleId = "ppstyle";
	addRule(styleId, ".default", "font-size:" + appSkinInfo.baseFontSize + "px" + "; color:" + reverseColor(panelBackgroundColor) + "; background-color:" + toHex(panelBackgroundColor, 20));
	addRule(styleId, "button", "border-color: " + toHex(panelBackgroundColor, -50));
}

function addRule(stylesheetId, selector, rule) {
	var stylesheet = document.getElementById(stylesheetId);

	if (stylesheet) {
		stylesheet = stylesheet.sheet;
		if (stylesheet.addRule) {
			stylesheet.addRule(selector, rule);
		} else if (stylesheet.insertRule) {
			stylesheet.insertRule(selector + ' { ' + rule + ' }', stylesheet.cssRules.length);
		}
	}
}

function reverseColor(color, delta) {
	return toHex({ red: Math.abs(255 - color.red), green: Math.abs(255 - color.green), blue: Math.abs(255 - color.blue) }, delta);
}

/**
 * Convert the Color object to string in hexadecimal format;
 */
function toHex(color, delta) {
	function computeValue(value, delta) {
		var computedValue = !isNaN(delta) ? value + delta : value;
		if (computedValue < 0) {
			computedValue = 0;
		} else if (computedValue > 255) {
			computedValue = 255;
		}

		computedValue = computedValue.toString(16);
		return computedValue.length == 1 ? "0" + computedValue : computedValue;
	}

	var hex = "";
	if (color) {
		with (color) {
			hex = computeValue(red, delta) + computeValue(green, delta) + computeValue(blue, delta);
		};
	}
	return "#" + hex;
}

function onAppThemeColorChanged(event) {
	// Should get a latest HostEnvironment object from application.
	var skinInfo = JSON.parse(window.__adobe_cep__.getHostEnvironment()).appSkinInfo;
	// Gets the style information such as color info from the skinInfo, 
	// and redraw all UI controls of your extension according to the style info.
	updateThemeWithAppSkinInfo(skinInfo);
}

function evalScript(script, callback) {
	new CSInterface().evalScript(script, callback);
}