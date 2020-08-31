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
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
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
 * \brief Function wrappers for Illustrator functions.
 */


#include "IllustratorSDK.h"
#include "l2a_ai_functions/l2a_ai_functions.h"

#include "l2a_property/l2a_property.h"
#include "l2a_error/l2a_error.h"
#include "l2a_suites.h"
#include "l2a_names.h"
#include "utility/string_functions.h"
#include "utility/file_system.h"

#include "AIDocumentAction.h"
#include "AIPDFFormatAction.h"
#include "AIObjectAction.h"


/**
 *
 */
AIArtHandle L2A::AI::CreatePlacedItem(ai::FilePath pdf_path)
{
    ASErr error = kNoErr;

    // Create the placed item with the place request function.
    AIPlaceRequestData request;
    request.m_lPlaceMode = kVanillaPlace;
    request.m_pFilePath = &pdf_path;
    request.m_filemethod = 1;
    error = sAIPlaced->ExecPlaceRequest(request);
    l2a_check_ai_error(error);

    // Get the new art handle.
    return request.m_hNewArt;
}

/**
 *
 */
PlaceAlignment L2A::AI::GetPropertyAlignment(const L2A::Property& item_property)
{
    // Get the alignment from the property.
    L2A::TextAlignHorizontal text_align_horizontal;
    L2A::TextAlignVertical text_align_vertical;
    item_property.GetTextAlignment(text_align_horizontal, text_align_vertical);

    // Set the corret alignment.
    if (text_align_horizontal == L2A::TextAlignHorizontal::left && text_align_vertical == L2A::TextAlignVertical::top)
        return kTopLeft;
    else if (text_align_horizontal == L2A::TextAlignHorizontal::centre &&
        text_align_vertical == L2A::TextAlignVertical::top)
        return kTopMid;
    else if (text_align_horizontal == L2A::TextAlignHorizontal::right &&
        text_align_vertical == L2A::TextAlignVertical::top)
        return kTopRight;
    else if (text_align_horizontal == L2A::TextAlignHorizontal::left &&
        text_align_vertical == L2A::TextAlignVertical::centre)
        return kMidLeft;
    else if (text_align_horizontal == L2A::TextAlignHorizontal::centre &&
        text_align_vertical == L2A::TextAlignVertical::centre)
        return kMidMid;
    else if (text_align_horizontal == L2A::TextAlignHorizontal::right &&
        text_align_vertical == L2A::TextAlignVertical::centre)
        return kMidRight;
    else if (text_align_horizontal == L2A::TextAlignHorizontal::left &&
        text_align_vertical == L2A::TextAlignVertical::baseline)
        return kMidLeft;
    else if (text_align_horizontal == L2A::TextAlignHorizontal::centre &&
        text_align_vertical == L2A::TextAlignVertical::baseline)
        return kMidMid;
    else if (text_align_horizontal == L2A::TextAlignHorizontal::right &&
        text_align_vertical == L2A::TextAlignVertical::baseline)
        return kMidRight;
    else if (text_align_horizontal == L2A::TextAlignHorizontal::left &&
        text_align_vertical == L2A::TextAlignVertical::bottom)
        return kBotLeft;
    else if (text_align_horizontal == L2A::TextAlignHorizontal::centre &&
        text_align_vertical == L2A::TextAlignVertical::bottom)
        return kBotMid;
    else if (text_align_horizontal == L2A::TextAlignHorizontal::right &&
        text_align_vertical == L2A::TextAlignVertical::bottom)
        return kBotRight;
    else
        l2a_error("Placement could not me determined.");
}

/**
 *
 */
void L2A::AI::GetPropertyPlacedMethodClip(const L2A::Property& item_property, PlaceMethod& place_method, bool& clip)
{
    L2A::PlacedArtOptions placed_options = item_property.GetPlacedOption();

    if (placed_options == L2A::PlacedArtOptions::fill_to_boundary_box)
    {
        place_method = kConform;
        clip = false;
    }
    else if (placed_options == L2A::PlacedArtOptions::keep_scale)
    {
        place_method = kAsIs;
        clip = false;
    }
    else if (placed_options == L2A::PlacedArtOptions::keep_scale_clip)
    {
        place_method = kAsIs;
        clip = true;
    }
    else
        l2a_error("Place method could not be determined.");
}

/**
 *
 */
void L2A::AI::SetPlacement(const AIArtHandle& placed_item, const L2A::Property& item_property)
{
    // Get placement enum.
    PlaceAlignment alignment = GetPropertyAlignment(item_property);

    // Set correct alignment.
    PlaceMethod method;
    bool clip;
    GetPropertyPlacedMethodClip(item_property, method, clip);

    // Set the placed item options.
    SetPlacement(placed_item, method, alignment, clip);
}

/**
 *
 */
void L2A::AI::SetPlacement(
    const AIArtHandle& placed_item, const PlaceMethod& method, const PlaceAlignment& alignment, const bool& clip)
{
    ASErr error = kNoErr;
    error = sAIPlaced->SetPlaceOptions(placed_item, method, alignment, clip);
    l2a_check_ai_error(error);
}

/**
 *
 */
ai::FilePath L2A::AI::GetPlacedItemPath(const AIArtHandle& placed_item)
{
    AIErr error;
    ai::UnicodeString path;
    error = sAIPlaced->GetPlacedFilePathFromArt(placed_item, path);
    l2a_check_ai_error(error);
    return ai::FilePath(path);
}

/**
 *
 */
void L2A::AI::SetPlacedItemPath(AIArtHandle& placed_item, const ai::FilePath& path)
{
    AIErr error;
    error = sAIPlaced->SetPlacedFileSpecification(placed_item, path);
    l2a_check_ai_error(error);
}

/**
 *
 */
void L2A::AI::RelinkPlacedItem(AIArtHandle& placed_item, const ai::FilePath& path)
{
    AIErr error;

    // Request for creating a placed item.
    AIPlaceRequestData request;
    request.m_lPlaceMode = kForceReplace;
    request.m_filemethod = 1;
    request.m_pFilePath = &path;
    request.m_hOldArt = placed_item;
    error = sAIPlaced->ExecPlaceRequest(request);
    l2a_check_ai_error(error);

    // Relink the new art to the object.
    if (request.m_hNewArt == NULL && request.m_hOldArt != NULL)
        placed_item = request.m_hOldArt;
    else if (request.m_hOldArt == NULL && request.m_hNewArt != NULL)
        placed_item = request.m_hNewArt;
    else
        l2a_error("Got two return pointers. This is unexpected.");
}

/**
 *
 */
void L2A::AI::SetNote(const AIArtHandle& item, const ai::UnicodeString& note)
{
    ASErr error = kNoErr;
    error = sAIArt->SetNote(item, note);
    l2a_check_ai_error(error);
}

/**
 *
 */
ai::UnicodeString L2A::AI::GetNote(const AIArtHandle& item)
{
    ASErr error = kNoErr;
    ai::UnicodeString return_string;
    error = sAIArt->GetNote(item, return_string);
    l2a_check_ai_error(error);
    return return_string;
}

/**
 *
 */
void L2A::AI::SetName(const AIArtHandle& item, const ai::UnicodeString& name)
{
    ASErr error = kNoErr;
    error = sAIArt->SetArtName(item, name);
    l2a_check_ai_error(error);
}

/**
 *
 */
ai::UnicodeString L2A::AI::GetName(const AIArtHandle& item)
{
    ASErr error = kNoErr;
    ai::UnicodeString return_string;
    error = sAIArt->GetArtName(item, return_string, NULL);
    l2a_check_ai_error(error);
    return return_string;
}

/**
 *
 */
AIRealRect L2A::AI::GetArtBounds(const AIArtHandle& item)
{
    ASErr error = kNoErr;
    AIRealRect bounds;
    error = sAIArt->GetArtBounds(item, &bounds);
    l2a_check_ai_error(error);
    return bounds;
}

/**
 *
 */
AIRealMatrix L2A::AI::GetPlacedMatrix(const AIArtHandle& placed_item)
{
    ASErr error = kNoErr;
    AIRealMatrix matrix;
    error = sAIPlaced->GetPlacedMatrix(placed_item, &matrix);
    l2a_check_ai_error(error);
    return matrix;
}

/**
 *
 */
AIRealRect L2A::AI::GetPlacedBoundingBox(const AIArtHandle& placed_item)
{
    ASErr error = kNoErr;
    AIRealRect image_box;
    error = sAIPlaced->GetPlacedBoundingBox(placed_item, &image_box);
    l2a_check_ai_error(error);
    return image_box;
}

/**
 *
 */
void L2A::AI::TransformArt(const AIArtHandle& placed_item, AIRealMatrix& artMatrix)
{
    ASErr error = kNoErr;
    error = sAITransformArt->TransformArt(placed_item, &artMatrix, 0., kTransformObjects);
    l2a_check_ai_error(error);
}

/**
 *
 */
void L2A::AI::SetPlacedMatrix(const AIArtHandle& placed_item, AIRealMatrix& placed_matrix)
{
    ASErr error = kNoErr;
    error = sAIPlaced->SetPlacedMatrix(placed_item, &placed_matrix);
    l2a_check_ai_error(error);
}

/**
 *
 */
void L2A::AI::AlignmentToFac(const PlaceAlignment& alignment, double (&pos_fac)[2])
{
    if (alignment == kTopLeft)
    {
        pos_fac[0] = 0.;
        pos_fac[1] = 1.;
    }
    else if (alignment == kTopMid)
    {
        pos_fac[0] = 0.5;
        pos_fac[1] = 1.;
    }
    else if (alignment == kTopRight)
    {
        pos_fac[0] = 1.;
        pos_fac[1] = 1.;
    }
    else if (alignment == kMidLeft)
    {
        pos_fac[0] = 0.;
        pos_fac[1] = 0.5;
    }
    else if (alignment == kMidMid)
    {
        pos_fac[0] = 0.5;
        pos_fac[1] = 0.5;
    }
    else if (alignment == kMidRight)
    {
        pos_fac[0] = 1.;
        pos_fac[1] = 0.5;
    }
    else if (alignment == kBotLeft)
    {
        pos_fac[0] = 0.;
        pos_fac[1] = 0.;
    }
    else if (alignment == kBotMid)
    {
        pos_fac[0] = 0.5;
        pos_fac[1] = 0.;
    }
    else if (alignment == kBotRight)
    {
        pos_fac[0] = 1.;
        pos_fac[1] = 0.;
    }
    else
        l2a_error("Wrong alignment given!");
}

/**
 *
 */
bool L2A::AI::IsL2AItem(const AIArtHandle& item)
{
    // Get the name of the placed item.
    ai::UnicodeString item_name = GetName(item);

    // Check if the name is the one of an L2A item.
    if (L2A::UTIL::StartsWith(item_name, ai::UnicodeString(L2A::NAMES::ai_item_name_prefix_)))
        return true;
    else
        return false;
}

/**
 *
 */
bool L2A::AI::IsIsolationMode()
{
    // Check the type of isolation mode.
    AIIsolationModeType isolation_mode_type = sAIIsolationMode->GetIsolationModeType();
    if (isolation_mode_type == kAIIMLeafArt)
        return true;
    else
        return false;
}

/**
 *
 */
void L2A::AI::ExitIsolationMode()
{
    AIErr error;

    // Get the parent art.
    AIArtHandle parent;
    ai::AutoBuffer<AIArtHandle> tree;
    sAIIsolationMode->GetIsolatedArtAndParents(&parent, &tree);

    if (tree.GetCount() == 2)
    {
        // Parent is the main tree root -> deactivate isolation mode.
        error = sAIIsolationMode->CancelIsolationMode();
    }
    else
    {
        // Isolate the parent art.
        parent = tree[size_t(0)];
        error = sAIIsolationMode->EnterIsolationMode(parent, false);
    }

    // Check for errors.
    l2a_check_ai_error(error);
}

/**
 *
 */
void L2A::AI::GetItems(std::vector<AIArtHandle>& items, SelectionState selected, ai::int16 type)
{
    ASErr error = kNoErr;

    // Clear the input vector.
    items.clear();

    // Look for art items in the document. This is done by checking for selected and / or unselected items.
    AIArtHandle** placed_items;
    ai::int32 n_items;

    // Set the spec for which items are searched.
    AIMatchingArtSpec placed_spec[2];
    ai::int16 num_specs;
    if (selected == SelectionState::all)
    {
        placed_spec[0].type = type;
        placed_spec[0].whichAttr = kArtSelected;
        placed_spec[0].attr = kArtSelected;
        placed_spec[1].type = type;
        placed_spec[1].whichAttr = kArtSelected;
        placed_spec[1].attr = 0;
        num_specs = 2;
    }
    else if (selected == SelectionState::selected)
    {
        placed_spec[0].type = type;
        placed_spec[0].whichAttr = kArtSelected;
        placed_spec[0].attr = kArtSelected;
        num_specs = 1;
    }
    else if (selected == SelectionState::deselected)
    {
        placed_spec[0].type = type;
        placed_spec[0].whichAttr = kArtSelected;
        placed_spec[0].attr = 0;
        num_specs = 1;
    }
    else
        l2a_error("Got unexpected SelectionState!");

    // Look for art items.
    error = sAIMatchingArt->GetMatchingArt(placed_spec, num_specs, &placed_items, &n_items);
    l2a_check_ai_error(error);

    // Loop over art items and add them to the vector.
    for (int i = 0; i < n_items; i++) items.push_back((*placed_items)[i]);

    // Free the memory.
    error = sAIMdMemorySuite->MdMemoryDisposeHandle((AIMdMemoryHandle)placed_items);
    l2a_check_ai_error(error);
}

/**
 *
 */
void L2A::AI::GetDocumentItems(std::vector<AIArtHandle>& l2a_items, SelectionState selected)
{
    // Clear the input vector.
    l2a_items.clear();

    // Get the art items in the document.
    std::vector<AIArtHandle> art_items;
    GetItems(art_items, selected, kPlacedArt);

    // Loop over placed art items and check if they are LaTeX2AI items
    for (const auto& item : art_items)
        if (IsL2AItem(item)) l2a_items.push_back(item);
}

/**
 *
 */
bool L2A::AI::GetSingleIsolationItem(AIArtHandle& item)
{
    // Check if isolation mode is active.
    if (L2A::AI::IsIsolationMode())
    {
        // Check if l2a items are selected.
        std::vector<AIArtHandle> l2a_placed_items;
        L2A::AI::GetDocumentItems(l2a_placed_items, L2A::AI::SelectionState::selected);

        if (l2a_placed_items.size() == 1)
        {
            // Found exactly one item, this is the item that was double clicked on.
            item = l2a_placed_items[0];

            // Return to previous selection mode.
            L2A::AI::ExitIsolationMode();

            return true;
        }
        else if (l2a_placed_items.size() > 1)
            l2a_error("Found " + L2A::UTIL::IntegerToString((unsigned int)l2a_placed_items.size()) +
                " items, expected 0 or 1. This should not happen.");
    }

    // Default return value.
    return false;
}

/**
 *
 */
void L2A::AI::UndoActivate() { UndoSetActive(true); }

/**
 *
 */
void L2A::AI::UndoDeactivate() { UndoSetActive(false); }

/**
 *
 */
void L2A::AI::UndoSetActive(bool active)
{
    AIErr error;
    error = sAIUndo->SetSilent((AIBoolean)(!active));
    l2a_check_ai_error(error);
}

/**
 *
 */
unsigned int L2A::AI::GetDocumentCount()
{
    ai::int32 n;
    AIErr error;
    error = sAIDocumentList->Count(&n);
    l2a_check_ai_error(error);
    return (unsigned int)n;
}

/**
 *
 */
AIPoint L2A::AI::ArtworkPointToViewPoint(const AIRealPoint& artwork_point)
{
    AIPoint view_point;
    AIErr error = sAIDocumentView->ArtworkPointToViewPoint(NULL, &artwork_point, &view_point);
    l2a_check_ai_error(error);
    return view_point;
}

/**
 *
 */
AIRect L2A::AI::ArtworkBoundsToViewBounds(const AIRealRect& artwork_bounds)
{
    ASErr result = kNoErr;

    AIRealPoint tlArt, brArt;
    tlArt.h = artwork_bounds.left;
    tlArt.v = artwork_bounds.top;
    brArt.h = artwork_bounds.right;
    brArt.v = artwork_bounds.bottom;

    // Convert artwork coordinates to view coordinates.
    AIPoint tlView, brView;
    result = sAIDocumentView->ArtworkPointToViewPoint(NULL, &tlArt, &tlView);
    l2a_check_ai_error(result);
    result = sAIDocumentView->ArtworkPointToViewPoint(NULL, &brArt, &brView);
    l2a_check_ai_error(result);

    AIRect view_bounds;
    view_bounds.left = tlView.h;
    view_bounds.top = tlView.v;
    view_bounds.right = brView.h;
    view_bounds.bottom = brView.v;
    return view_bounds;
}


/**
 *
 */
void L2A::AI::SaveToPDF()
{
    AIErr result;

    // First it is checked if the active document is saved. It is advisable to save the document before exporting to
    // pdf.
    AIBoolean is_modified = true;
    result = sAIDocument->GetDocumentModified(&is_modified);
    l2a_check_ai_error(result);
    if (is_modified)
    {
        AIAnswer save = sAIUser->YesNoAlert(ai::UnicodeString(
            "The curent document is not saved. It is advisable to save the document before exporting to pdf."));
        if (save == AIAnswer::kAIAnswer_Yes)
        {
            AIDocumentHandle doc;
            result = sAIDocument->GetDocument(&doc);
            l2a_check_ai_error(result);
            result = sAIDocumentList->Save(doc);
            l2a_check_ai_error(result);
        }
    }

    // Get the name of the pdf file.
    ai::FilePath pdf_file = L2A::UTIL::GetDocumentPath().GetParent();
    pdf_file.AddComponent(L2A::UTIL::GetDocumentName() + ".pdf");

    // Set the parameters for the action.
    AIActionParamValueRef action_parameters;

    {
        // The save as option is not available via the api, therefore it is realized with an action event. The following
        // parameters are taken from a recorded action in AI. The meaning of them is not clear, but with the chosen
        // values the function performs as expected. The numbering is the index of the parameters in the recorded
        // script.

        result = sAIActionManager->AINewActionParamValue(&action_parameters);
        l2a_check_ai_error(result);

        // 1
        result = sAIActionManager->AIActionSetInteger(action_parameters, 'optn', 2);
        l2a_check_ai_error(result);

        // 2
        result = sAIActionManager->AIActionSetBoolean(action_parameters, 'vpdf', false);
        l2a_check_ai_error(result);

        // 3
        result = sAIActionManager->AIActionSetBoolean(action_parameters, 'usrq', false);
        l2a_check_ai_error(result);

        // 4
        result = sAIActionManager->AIActionSetStringUS(action_parameters, 'usps', ai::UnicodeString(""));
        l2a_check_ai_error(result);

        // 5
        result = sAIActionManager->AIActionSetBoolean(action_parameters, 'msrq', false);
        l2a_check_ai_error(result);

        // 6
        result = sAIActionManager->AIActionSetStringUS(action_parameters, 'msps', ai::UnicodeString(""));
        l2a_check_ai_error(result);

        // Parameters 7 and 8 are not set due to the unclear meaning of the enum. This does not affect the event.

        // 9
        result = sAIActionManager->AIActionSetBoolean(action_parameters, 'ebcp', true);
        l2a_check_ai_error(result);

        // 10
        result = sAIActionManager->AIActionSetBoolean(action_parameters, 'ebac', true);
        l2a_check_ai_error(result);

        // 11
        result = sAIActionManager->AIActionSetBoolean(action_parameters, 'ebca', true);
        l2a_check_ai_error(result);

        // 12
        result = sAIActionManager->AIActionSetBoolean(action_parameters, 'ebpt', false);
        l2a_check_ai_error(result);

        // 13
        result = sAIActionManager->AIActionSetStringUS(action_parameters, 'name', pdf_file.GetFullPath());
        l2a_check_ai_error(result);

        // 14
        result = sAIActionManager->AIActionSetStringUS(action_parameters, 'frmt', ai::UnicodeString("PDF File Format"));
        l2a_check_ai_error(result);

        // 15
        result = sAIActionManager->AIActionSetStringUS(action_parameters, 'extn', ai::UnicodeString("pdf"));
        l2a_check_ai_error(result);

        // 16
        result = sAIActionManager->AIActionSetBoolean(action_parameters, 'smab', true);
        l2a_check_ai_error(result);

        // 17
        result = sAIActionManager->AIActionSetBoolean(action_parameters, 'sall', true);
        l2a_check_ai_error(result);

        // 18
        result = sAIActionManager->AIActionSetStringUS(action_parameters, 'sran', ai::UnicodeString(""));
        l2a_check_ai_error(result);
    }

    // Perform the save copy as action.
    result = sAIActionManager->PlayActionEvent("adobe_saveACopyAs", kDialogOff, action_parameters);
    l2a_check_ai_error(result);
}

/**
 *
 */
void L2A::AI::SaveAs(const ai::FilePath& document_path)
{
    AIErr result;

    // Safety checks.
    if (L2A::UTIL::IsFile(document_path)) l2a_error("The file '" + document_path.GetFullPath() + "' already exists!");
    if (L2A::UTIL::IsDirectory(document_path))
        l2a_error("The path '" + document_path.GetFullPath() + "' is a directory!");

    // Set the parameters for the action.
    AIActionParamValueRef action_parameters;

    {
        // The save as option is not available via the api, therefore it is realized with an action event. The following
        // parameters are taken from a recorded action in AI. The meaning of them is not clear, but with the chosen
        // values the function performs as expected. The numbering is the index of the parameters in the recorded
        // script.

        result = sAIActionManager->AINewActionParamValue(&action_parameters);
        l2a_check_ai_error(result);

        // 1
        result = sAIActionManager->AIActionSetBoolean(action_parameters, 'cmpr', true);
        l2a_check_ai_error(result);

        // 2
        result = sAIActionManager->AIActionSetBoolean(action_parameters, 'pdf', true);
        l2a_check_ai_error(result);

        // 3
        result = sAIActionManager->AIActionSetInteger(action_parameters, 'crtr', 16);
        l2a_check_ai_error(result);

        // 4
        result = sAIActionManager->AIActionSetInteger(action_parameters, 'ext.', 1);
        l2a_check_ai_error(result);

        // 5
        result = sAIActionManager->AIActionSetInteger(action_parameters, 'incl', 0);
        l2a_check_ai_error(result);

        // 6
        result = sAIActionManager->AIActionSetReal(action_parameters, 'rato', 100.0);
        l2a_check_ai_error(result);

        // 7
        result = sAIActionManager->AIActionSetInteger(action_parameters, 'prfl', 1);
        l2a_check_ai_error(result);

        // 8
        result = sAIActionManager->AIActionSetBoolean(action_parameters, 'smab', false);
        l2a_check_ai_error(result);

        // 9
        result = sAIActionManager->AIActionSetStringUS(action_parameters, 'name', document_path.GetFullPath());
        l2a_check_ai_error(result);

        // 10
        result = sAIActionManager->AIActionSetStringUS(
            action_parameters, 'frmt', ai::UnicodeString("Adobe Illustrator Any Format Writer"));
        l2a_check_ai_error(result);

        // 11
        result = sAIActionManager->AIActionSetStringUS(action_parameters, 'extn', ai::UnicodeString("ai,ait"));
        l2a_check_ai_error(result);
    }

    // Perform the save as action.
    result = sAIActionManager->PlayActionEvent(kAISaveDocumentAsAction, kDialogOff, action_parameters);
    l2a_check_ai_error(result);
}

/**
 *
 */
void L2A::AI::Rotate(const double angle)
{
    // Set the parameters for the action.
    AIErr result;
    AIActionParamValueRef action_parameters;

    // Rotation without rotating the boundary box is not available via the api, therefore it is realized with an
    // action event. The following parameters are taken from a recorded action in AI.
    result = sAIActionManager->AINewActionParamValue(&action_parameters);
    l2a_check_ai_error(result);
    result =
        sAIActionManager->AIActionSetUnitReal(action_parameters, kAIRotateSelectionAngleKey, unitAngle, (ASReal)angle);
    l2a_check_ai_error(result);

    // Perform the rotation action.
    // An alternative to the action name is kAIRotateSelectionAction, but this gave some problems when reseting the
    // boundary box.
    result = sAIActionManager->PlayActionEvent("ai_plugin_bBox_rotate", kDialogOff, action_parameters);
    l2a_check_ai_error(result);
}

/**
 *
 */
void L2A::AI::ResetBoundingBox()
{
    // Set the parameters for the action.
    AIErr result;
    AIActionParamValueRef action_parameters;

    // Reseting of the boundary box is not available via the api, therefore it is realized with an
    // action event, where the correspongind menu command is called. In future versions of this SDK, the name of the
    // menu items will be defined in the file "AIMenuCommandString.h".
    result = sAIActionManager->AINewActionParamValue(&action_parameters);
    l2a_check_ai_error(result);
    result = sAIActionManager->AIActionSetString(action_parameters, 'itnm', "AI Reset Bounding Box");
    l2a_check_ai_error(result);

    // Perform the reset boundary box action.
    result = sAIActionManager->PlayActionEvent(kAIDoMenuCommandAction, kDialogOff, action_parameters);
    l2a_check_ai_error(result);
}

/**
 *
 */
void L2A::AI::Move(const double delta_x, const double delta_y, bool copy)
{
    // Set the parameters for the action.
    AIErr result;
    AIActionParamValueRef action_parameters;

    result = sAIActionManager->AINewActionParamValue(&action_parameters);
    l2a_check_ai_error(result);

    result = sAIActionManager->AIActionSetReal(action_parameters, kAITranslateSelectionHorizontalKey, (ASReal)delta_x);
    l2a_check_ai_error(result);
    result = sAIActionManager->AIActionSetReal(action_parameters, kAITranslateSelectionVerticalKey, (ASReal)delta_y);
    l2a_check_ai_error(result);
    result = sAIActionManager->AIActionSetBoolean(action_parameters, kAITranslateSelectionCopyKey, copy);
    l2a_check_ai_error(result);

    // Perform the move action.
    result = sAIActionManager->PlayActionEvent(kAITranslateSelectionAction, kDialogOff, action_parameters);
    l2a_check_ai_error(result);
}

/**
 *
 */
void L2A::AI::Scale(const double scale_x, const double scale_y, bool copy)
{
    // Set the parameters for the action.
    AIErr result;
    AIActionParamValueRef action_parameters;

    result = sAIActionManager->AINewActionParamValue(&action_parameters);
    l2a_check_ai_error(result);

    result = sAIActionManager->AIActionSetBoolean(action_parameters, kAIScaleSelectionUniformKey, false);
    l2a_check_ai_error(result);
    result = sAIActionManager->AIActionSetBoolean(action_parameters, kAIScaleSelectionLinesKey, false);
    l2a_check_ai_error(result);
    result =
        sAIActionManager->AIActionSetReal(action_parameters, kAIScaleSelectionHorizontalKey, (AIReal)(100.0 * scale_x));
    l2a_check_ai_error(result);
    result =
        sAIActionManager->AIActionSetReal(action_parameters, kAIScaleSelectionVerticalKey, (AIReal)(100.0 * scale_y));
    l2a_check_ai_error(result);
    result = sAIActionManager->AIActionSetBoolean(action_parameters, kAIScaleSelectionCopyKey, copy);
    l2a_check_ai_error(result);

    // Perform the scale action.
    result = sAIActionManager->PlayActionEvent(kAIScaleSelectionAction, kDialogOff, action_parameters);
    l2a_check_ai_error(result);
}

/**
 *
 */
void L2A::AI::SelectItems(std::vector<AIArtHandle>& items)
{
    // First clear the selection.
    sAIMatchingArt->DeselectAll();

    // Loop over items and select them.
    AIErr error;
    for (auto& item : items)
    {
        error = sAIArt->SetArtUserAttr(item, kArtSelected, kArtSelected);
        l2a_check_ai_error(error);
    }

    // Update the document item properties.
    error = sAIDocument->SyncDocument();
    l2a_check_ai_error(error);
}

/**
 *
 */
void L2A::AI::DrawPath(const std::vector<AIPathSegment>& segments, AIPathStyle style, bool closed)
{
    AIErr error;
    AIArtHandle path;

    error = sAIArt->NewArt(kPathArt, kPlaceAboveAll, NULL, &path);
    l2a_check_ai_error(error);

    error = sAIPath->SetPathSegments(path, 0, (ai::int16)segments.size(), segments.data());
    l2a_check_ai_error(error);

    error = sAIPath->SetPathClosed(path, closed);
    l2a_check_ai_error(error);

    error = sAIPathStyle->SetPathStyle(path, &style);
    l2a_check_ai_error(error);
}

/**
 *
 */
void L2A::AI::GetPathPoints(AIArtHandle& path_item, std::vector<AIRealPoint>& points, bool append_to_vector)
{
    AIErr error;

    ai::int16 n_segments;
    error = sAIPath->GetPathSegmentCount(path_item, &n_segments);
    l2a_check_ai_error(error);

    std::vector<AIPathSegment> segments(n_segments);
    error = sAIPath->GetPathSegments(path_item, 0, n_segments, segments.data());
    l2a_check_ai_error(error);

    if (!append_to_vector)
    {
        points.clear();
        points.reserve(n_segments);
    }
    else
    {
        points.reserve(points.size() + n_segments);
    }

    // Add points to return vector.
    for (const auto& segment : segments) points.push_back(segment.p);
};
