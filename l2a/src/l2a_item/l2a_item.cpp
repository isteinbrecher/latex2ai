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
 * \brief Errors for l2a applications.
 */


#include "IllustratorSDK.h"
#include "l2a_item.h"

#include "l2a_suites.h"
#include "l2a_error/l2a_error.h"
#include "l2a_latex/l2a_latex.h"
#include "l2a_ai_functions/l2a_ai_functions.h"
#include "l2a_forms/l2a_forms.h"
#include "utility/parameter_list.h"
#include "utility/file_system.h"
#include "utility/string_functions.h"
#include "utility/math.h"
#include "utility/utils.h"
#include "l2a_names.h"
#include "l2a_constants.h"


/**
 *
 */
L2A::Item::Item(const AIRealPoint& position)
{
    // Set new empty property for this item.
    property_ = L2A::Property();
    property_.SetFromLastInput();

    // Loop until either the pdf was created sucessfully, or the user aborted the creation process.
    ai::FilePath pdf_file;
    while (true)
    {
        // Get user input for the item. If the form is canceled the creation of this object is also canceled.
        ItemFormReturnValues return_value = OpenUserForm(property_);
        if (return_value == ItemFormReturnValues::cancel)
            return;
        else if (return_value == ItemFormReturnValues::ok)
        {
            // Do nothing in this case.
        }
        else
            l2a_error("Unexpected return value");

        // Create the pdf file.
        L2A::LATEX::LatexCreationResult result =
            L2A::LATEX::CreateLatexWithDebug(property_.GetLaTeXCode(), pdf_file, ai::UnicodeString("create"));
        if (result == L2A::LATEX::LatexCreationResult::fail_quit)
            return;
        else if (result == L2A::LATEX::LatexCreationResult::item_created)
            break;
        // In the case of an error and redo, do nothing here, as we are already in the while loop.
    }

    // Convert pdf with postscript.
    pdf_file = L2A::LATEX::SplitPdfPages(pdf_file, 1).at(0);

    // Get the path to the pdf file for this item and copy the created item there.
    ai::FilePath next_item;
    unsigned int item_index = L2A::UTIL::GetNextItemIndex(next_item);
    L2A::UTIL::CopyFileL2A(pdf_file, next_item);

    // Create the placed item.
    placed_item_ = L2A::AI::CreatePlacedItem(next_item);
    L2A::AI::SetPlacement(placed_item_, property_);

    // Set the name and tag for the item.
    SetNote();
    L2A::AI::SetName(placed_item_, L2A::NAMES::ai_item_name_prefix_ + L2A::UTIL::IntegerToString(item_index, 3));

    // Move the file to the cursor position.
    MoveItem(position);

    // Everything was successfull, we write the input from this item to the application directory, so we can use it with
    // the next one.
    property_.WriteLastInput();
}

/**
 *
 */
L2A::Item::Item(const AIArtHandle& placed_item_handle)
{
    // Set placed item object.
    placed_item_ = placed_item_handle;

    // Get the data from the art item.
    property_ = L2A::Property();
    property_.SetFromString(L2A::AI::GetNote(placed_item_));

    // Check that the placed options in AI and the set LaTeX2AI options are the same.
    {
        PlaceMethod method_ai;
        PlaceAlignment alignment_ai;
        AIBoolean clip_ai;
        L2A::AI::GetPlacement(placed_item_, method_ai, alignment_ai, clip_ai);

        PlaceMethod method_l2a;
        AIBoolean clip_l2a;
        std::tuple<PlaceMethod&, AIBoolean&> method_tuple{method_l2a, clip_l2a};
        method_tuple =
            L2A::UTIL::KeyToValue(PlacedArtMethodEnums(), PlacedArtMethodEnumsAI(), property_.GetPlacedMethod());
        PlaceAlignment alignment_l2a =
            L2A::UTIL::KeyToValue(TextAlignTuples(), TextAlignEnumsAI(), property_.GetTextAlignment());

        if (method_ai != method_l2a || alignment_ai != alignment_l2a || clip_ai != clip_l2a)
        {
            // The options do not match, apply the ones from the LaTeX2AI options.
            sAIUser->MessageAlert(ai::UnicodeString(
                "The Illustrator placement values for a LaTeX2AI item do not match! The ones from the LaTeX2AI "
                "settings are applied. This can happen if the placement values are changed manually via Illustrator."));
            L2A::AI::SetPlacement(placed_item_, property_);
        }
    }
}

/**
 *
 */
void L2A::Item::Change()
{
    // Create a copy of the current input parameters.
    L2A::Property new_input = property_;

    // Do this until the creation process is canceled or the process was successfull.
    ai::FilePath pdf_file;
    L2A::PropertyCompare diff;
    while (true)
    {
        // Get input from user.
        ItemFormReturnValues return_value = OpenUserForm(new_input);
        if (return_value == ItemFormReturnValues::ok)
            // Compare the current input and the new one.
            diff = property_.Compare(new_input);
        else if (return_value == ItemFormReturnValues::cancel)
            return;
        else if (return_value == ItemFormReturnValues::redo_boundary)
        {
            RedoBoundary();
            return;
        }
        else if (return_value == ItemFormReturnValues::redo_latex)
        {
            diff.changed_latex = true;
            // We have to set the new input here, since it will be set as the property of this label at the end of this
            // function.
            new_input = property_;
        }

        if (diff.changed_latex)
        {
            // Redo the latex item.
            L2A::LATEX::LatexCreationResult result =
                L2A::LATEX::CreateLatexWithDebug(new_input.GetLaTeXCode(), pdf_file, ai::UnicodeString("edit"));
            if (result == L2A::LATEX::LatexCreationResult::fail_quit)
                return;
            else if (result == L2A::LATEX::LatexCreationResult::item_created)
                break;
            // In the case of an error and redo, do nothing here, as we are already in the while loop.
        }
        else
            // Nothing in the LaTeX changes, continue with this function.
            break;
    }

    if (diff.changed_latex)
    {
        // Convert pdf with postscript.
        pdf_file = L2A::LATEX::SplitPdfPages(pdf_file, 1).at(0);

        // Move new latex file to l2a folder (replace the old one). If the old one does not exist (for whatever reason)
        // a new one is created.
        ai::FilePath placed_item_file = L2A::AI::GetPlacedItemPath(placed_item_);
        if (!L2A::UTIL::IsFile(placed_item_file)) L2A::UTIL::GetNextItemIndex(placed_item_file);
        L2A::UTIL::CopyFileL2A(pdf_file, placed_item_file);

        // Everything worked fine, set the property of the current item.
        property_ = new_input;

        // Relink the placed item with the pdf file.
        L2A::AI::RelinkPlacedItem(placed_item_, placed_item_file);

        // Redo the boundary.
        RedoBoundary();
    }

    if (diff.changed_align || diff.changed_placement)
    {
        // Placement changed.
        property_ = new_input;
        L2A::AI::SetPlacement(placed_item_, property_);
    }

    // If something changed add the information to the placed item note.
    if (diff.Changed())
    {
        property_ = new_input;
        SetNote();
    }
}

/**
 *
 */
void L2A::Item::RedoBoundary()
{
    // If object is not streched and not diamond -> do nothing.
    if (!IsStreched() && !IsDiamond()) return;

    // Get the position of the reference point.
    AIRealPoint old_position = GetPosition();

    // Get the angle.
    AIReal angle = GetAngle();

    // Rotate the object back to the initial position.
    AIRealMatrix artMatrix;
    sAIRealMath->AIRealMatrixSetRotate(&artMatrix, (ASReal)(-angle));
    L2A::AI::TransformArt(placed_item_, artMatrix);

    // Set the transformation matrix to identity matrix.
    artMatrix.a = 1.;
    artMatrix.b = 0.;
    artMatrix.c = 0.;
    artMatrix.d = -1.;
    L2A::AI::SetPlacedMatrix(placed_item_, artMatrix);

    // Rotate back
    sAIRealMath->AIRealMatrixSetRotate(&artMatrix, (ASReal)(angle));
    L2A::AI::TransformArt(placed_item_, artMatrix);

    // Relink the file so that the boundary box is reset.
    L2A::AI::RelinkPlacedItem(placed_item_, L2A::AI::GetPlacedItemPath(placed_item_));
    SetNote();

    // Move back to original position.
    MoveItem(old_position);
}

/**
 *
 */
void L2A::Item::SetNote() const { L2A::AI::SetNote(placed_item_, property_.ToString()); }

/**
 *
 */
ai::UnicodeString L2A::Item::GetAIName() const { return L2A::AI::GetName(placed_item_); }

/**
 *
 */
void L2A::Item::MoveItem(const AIRealPoint& position)
{
    // Get the current position of the placed art item.
    AIRealPoint old_position = GetPosition();

    // For some objects that are far out of the artwork, multiple translations can be nessesary to reach the desired
    // position transform the object until the desired position is reached.
    AIReal position_error = 1.;
    int counter = 0;
    const int n_max = 3;

    while (position_error > L2A::CONSTANTS::eps_pos_)
    {
        if (n_max == counter)
            l2a_error(
                "L2A::Item::MoveItem Desired position not reached in " + L2A::UTIL::IntegerToString(n_max) + " tries!");

        // Transform the art item to the desired position.
        AIRealMatrix artMatrix;
        sAIRealMath->AIRealMatrixSetTranslate(&artMatrix, position.h - old_position.h, position.v - old_position.v);
        L2A::AI::TransformArt(placed_item_, artMatrix);

        // Check error in new position.
        old_position = GetPosition();
        position_error = L2A::UTIL::MATH::GetDistance(position, old_position);
        counter++;
    }
}

/**
 *
 */
AIRealPoint L2A::Item::GetPosition() const
{
    std::vector<PlaceAlignment> alignment_vector;
    alignment_vector.clear();
    alignment_vector.push_back(L2A::AI::GetPropertyAlignment(property_));
    return GetPosition(alignment_vector).at(0);
}

/**
 *
 */
std::vector<AIRealPoint> L2A::Item::GetPosition(const std::vector<PlaceAlignment>& placements) const
{
    // Vector of points to return.
    std::vector<AIRealPoint> positions;

    // Get the placement options for this item.
    PlaceMethod place_method;
    bool item_is_cliped;
    L2A::AI::GetPropertyPlacedMethodClip(property_, place_method, item_is_cliped);

    // If the item is fixed size and not cliped, the clip option is activated for calcualting the position.
    bool set_clip = place_method == kAsIs && !item_is_cliped;
    if (set_clip) L2A::AI::SetPlacement(placed_item_, place_method, L2A::AI::GetPropertyAlignment(property_), true);

    // Variables for the placement factor and the points on the item.
    AIReal pos_fac[2];
    AIRealPoint position;

    // Boundary coordinates for item.
    AIRealRect bounds = L2A::AI::GetArtBounds(placed_item_);

    if ((!IsRotated()) && (!IsDiamond()))
    {
        // Item is rectangle that is not rotated. This should be the default case.

        // Get the coordinates of the placement point.
        for (const auto& alignment : placements)
        {
            L2A::AI::AlignmentToFac(alignment, pos_fac);
            position.h = (ASReal)(bounds.left + pos_fac[0] * (bounds.right - bounds.left));
            position.v = (ASReal)(bounds.bottom + pos_fac[1] * (bounds.top - bounds.bottom));
            positions.push_back(position);
        }
    }
    else
    {
        // Angles of the basis vectors
        AIReal angle_1 = GetAngle(0);
        AIReal angle_2 = GetAngle(1);

        // Scale factor of the basis vectors.
        AIReal scale_1 = GetStrech(0);
        AIReal scale_2 = GetStrech(1);

        // Dimensions of the pdf file.
        AIRealRect image_box = L2A::AI::GetPlacedBoundingBox(placed_item_);
        AIReal pdf_height = image_box.top - image_box.bottom;
        AIReal pdf_width = image_box.right - image_box.left;

        // Vectors to each corner of the item.
        AIReal vec0[2] = {0., 0.};
        AIReal vec1[2] = {scale_1 * pdf_width * cos(angle_1), scale_1 * pdf_width * sin(angle_1)};
        AIReal vec2[2] = {scale_2 * pdf_height * cos(angle_2), scale_2 * pdf_height * sin(angle_2)};
        AIReal vec3[2] = {vec1[0] + vec2[0], vec1[1] + vec2[1]};

        // Get the minimum distance for both directions.
        AIReal diff_x = min(min(min(vec0[0], vec1[0]), vec2[0]), vec3[0]);
        AIReal diff_y = min(min(min(vec0[1], vec1[1]), vec2[1]), vec3[1]);

        // Position of the bottom left node.
        AIReal pos_node[2] = {bounds.left - diff_x, bounds.bottom - diff_y};

        // Get the coordinates of the placement point.
        for (const auto& alignment : placements)
        {
            L2A::AI::AlignmentToFac(alignment, pos_fac);
            position.h = (ASReal)(pos_node[0] + pos_fac[0] * vec1[0] + pos_fac[1] * vec2[0]);
            position.v = (ASReal)(pos_node[1] + pos_fac[0] * vec1[1] + pos_fac[1] * vec2[1]);
            positions.push_back(position);
        }
    }

    // If item was cliped for this function only, set back to original clipping state.
    if (set_clip) L2A::AI::SetPlacement(placed_item_, property_);

    return positions;
}

/**
 *
 */
void L2A::Item::Draw(AIAnnotatorMessage* message, const std::map<PlaceAlignment, AIRealPoint>& item_boundaries) const
{
    // Get the color for this item.
    AIRGBColor item_color;
    if (IsDiamond())
        item_color = L2A::CONSTANTS::color_diamond_;
    else if (IsStreched())
        item_color = L2A::CONSTANTS::color_scaled_;
    else
        item_color = L2A::CONSTANTS::color_ok_;

    // Get the hidden and locked status of the item.
    bool is_hidden;
    bool is_locked;
    L2A::AI::GetIsHiddenLocked(placed_item_, is_hidden, is_locked);
    if (is_hidden) return;
    if (is_locked)
    {
        item_color.red = ai::uint16(0.5 * item_color.red);
        item_color.green = ai::uint16(0.5 * item_color.green);
        item_color.blue = ai::uint16(0.5 * item_color.blue);
    }

    // Set drawing options.
    sAIAnnotatorDrawer->SetLineWidth(message->drawer, (AIReal)(L2A::CONSTANTS::line_width_));
    sAIAnnotatorDrawer->SetColor(message->drawer, item_color);
    sAIAnnotatorDrawer->SetLineDashed(message->drawer, false);

    // Get the coordinates of all Array with the relevant view positions. The first 5 are for the rectangle, the last
    // two for the baseline.
    std::vector<PlaceAlignment> boundary_placements = {
        kTopLeft, kTopRight, kBotRight, kBotLeft, kTopLeft, kMidLeft, kMidRight};
    std::vector<AIPoint> boundary_positions_view;
    for (const auto& placement : boundary_placements)
    {
        // Look for the entry in the dictionary.
        auto find_key = item_boundaries.find(placement);
        if (find_key != item_boundaries.end())
            boundary_positions_view.push_back(L2A::AI::ArtworkPointToViewPoint(find_key->second));
        else
            l2a_error("Key in item_boundaries could not be found!");
    }

    // Draw the boundary.
    AIErr error = sAIAnnotatorDrawer->DrawPolygon(message->drawer, &boundary_positions_view[0], 5, false);
    l2a_check_ai_error(error);

    // Draw the placement point.
    AIPoint placement_point =
        L2A::AI::ArtworkPointToViewPoint(item_boundaries.at(L2A::AI::GetPropertyAlignment(property_)));
    AIRect centre;
    centre.left = placement_point.h - L2A::CONSTANTS::radius_;
    centre.right = placement_point.h + L2A::CONSTANTS::radius_;
    centre.bottom = placement_point.v - L2A::CONSTANTS::radius_;
    centre.top = placement_point.v + L2A::CONSTANTS::radius_;
    error = sAIAnnotatorDrawer->DrawEllipse(message->drawer, centre, true);
    l2a_check_ai_error(error);

    if (property_.IsBaseline())
    {
        // Dash data for dashed line to display baseline items.
#if ILLUSTRATOR_VERSION == 16
        std::vector<AIReal> dash_data_ = {20, 7};
#else
        std::vector<AIFloat> dash_data_ = {20, 7};
#endif

        // Draw the base line of a baseline item.
        error = sAIAnnotatorDrawer->SetLineDashedEx(message->drawer, &dash_data_[0], (ai::int32)dash_data_.size());
        error = sAIAnnotatorDrawer->DrawLine(message->drawer, boundary_positions_view[5], boundary_positions_view[6]);
        l2a_check_ai_error(error);
    }
}

/**
 *
 */
L2A::ItemFormReturnValues L2A::Item::OpenUserForm(L2A::Property& input_property) const
{
    L2A::UTIL::ParameterList form_input_parameter_list;
    ai::UnicodeString key_latex("latex_exists");
    ai::UnicodeString key_boundary_box("boundary_box_state");
    if (!sAIArt->ValidArt(placed_item_, true))
    {
        form_input_parameter_list.SetOption(key_latex, false);
        form_input_parameter_list.SetOption(key_boundary_box, ai::UnicodeString("none"));
    }
    else
    {
        form_input_parameter_list.SetOption(key_latex, true);
        if (IsDiamond())
            form_input_parameter_list.SetOption(key_boundary_box, ai::UnicodeString("diamond"));
        else if (IsStreched())
            form_input_parameter_list.SetOption(key_boundary_box, ai::UnicodeString("streched"));
        else
            form_input_parameter_list.SetOption(key_boundary_box, ai::UnicodeString("ok"));
    }
    form_input_parameter_list.SetSubList(ai::UnicodeString("property_list"), input_property.ToParameterList());

    std::shared_ptr<L2A::UTIL::ParameterList> form_return_parameter_list;
    ai::UnicodeString return_string =
        L2A::Form(ai::UnicodeString("l2a_item"), form_input_parameter_list, form_return_parameter_list).return_string;

    if (return_string == ai::UnicodeString("ok"))
    {
        input_property.SetFromParameterList(*form_return_parameter_list);
        return ItemFormReturnValues::ok;
    }
    else if (return_string == ai::UnicodeString("redo_boundary_box"))
        return ItemFormReturnValues::redo_boundary;
    else if (return_string == ai::UnicodeString("redo_latex"))
        return ItemFormReturnValues::redo_latex;
    else if (return_string == ai::UnicodeString("cancel"))
        return ItemFormReturnValues::cancel;
    else
        l2a_error("Unexpected result. Got return string: \"" + return_string + "\"");
}

/**
 *
 */
AIReal L2A::Item::GetAngle(unsigned short director) const
{
    // Get the values from the matrix.
    AIRealMatrix artMatrix = L2A::AI::GetPlacedMatrix(placed_item_);

    if (director == 0)
        return -atan2(artMatrix.b, artMatrix.a);
    else
        return atan2(-artMatrix.d, artMatrix.c);
}

/**
 *
 */
bool L2A::Item::IsRotated() const
{
    // check if item is rotated
    if (abs(GetAngle()) < L2A::CONSTANTS::eps_angle_)
        return false;
    else
        return true;
}

/**
 *
 */
bool L2A::Item::IsDiamond() const
{
    // Check if the angle between the two directors is pi/2
    AIReal angle_1 = GetAngle(0);
    AIReal angle_2 = GetAngle(1);
    // Use the strech tollerance here, because not the angles are compared, but their cosines.
    if (abs(cos(angle_2 - angle_1)) < L2A::CONSTANTS::eps_strech_)
        return false;
    else
        return true;
}

/**
 *
 */
AIReal L2A::Item::GetStrech(unsigned short director) const
{
    // Get the values from the matrix.
    AIRealMatrix artMatrix = L2A::AI::GetPlacedMatrix(placed_item_);

    if (director == 0)
        return sqrt(artMatrix.b * artMatrix.b + artMatrix.a * artMatrix.a);
    else
        return sqrt(artMatrix.c * artMatrix.c + artMatrix.d * artMatrix.d);
}

/**
 *
 */
bool L2A::Item::IsStreched() const
{
    // Check if item is streched, both strech factors must be smaller than eps.
    if ((abs(1. - GetStrech(0)) < L2A::CONSTANTS::eps_strech_) &&
        (abs(1. - GetStrech(1)) < L2A::CONSTANTS::eps_strech_))
        return false;
    else
        return true;
}

/**
 *
 */
void L2A::RedoItems()
{
    // First get all and all selected l2a items.
    std::vector<AIArtHandle> all_items;
    std::vector<AIArtHandle> selected_items;
    L2A::AI::GetDocumentItems(all_items, L2A::AI::SelectionState::all);
    L2A::AI::GetDocumentItems(selected_items, L2A::AI::SelectionState::selected);

    // Add number of items to parameter list for form.
    L2A::UTIL::ParameterList redo_all_parameter_list;
    const unsigned int n_all_items = (unsigned int)all_items.size();
    const unsigned int n_selected_items = (unsigned int)selected_items.size();
    redo_all_parameter_list.SetOption(ai::UnicodeString("n_all_items"), n_all_items);
    redo_all_parameter_list.SetOption(ai::UnicodeString("n_selected_items"), n_selected_items);

    // Get the user input for the redo all function.
    std::shared_ptr<L2A::UTIL::ParameterList> new_parameter_list;
    if (L2A::Form(ai::UnicodeString("l2a_redo"), redo_all_parameter_list, new_parameter_list).canceled) return;

    // Get the result from the form.
    const bool redo_latex = new_parameter_list->GetIntOption(ai::UnicodeString("redo_latex"));
    const bool redo_boundary = new_parameter_list->GetIntOption(ai::UnicodeString("redo_boundary"));
    if (redo_latex && !redo_boundary) l2a_error("Got unexpected combination of options.");
    std::vector<AIArtHandle> redo_items;
    const bool is_all_items =
        new_parameter_list->GetStringOption(ai::UnicodeString("item_type")) == ai::UnicodeString("all");
    if (is_all_items)
        redo_items = all_items;
    else
        redo_items = selected_items;

    // Check if something needs to be done.
    if (!redo_latex && !redo_boundary || redo_items.size() == 0) return;

    // Create an Item for all placed items that need to be redone.
    unsigned int locked_counter = 0;
    std::vector<L2A::Item> l2a_items;
    for (const auto& placed_item : redo_items)
    {
        bool is_hidden;
        bool is_locked;
        L2A::AI::GetIsHiddenLocked(placed_item, is_hidden, is_locked);
        if (is_hidden || is_locked)
            locked_counter++;
        else
            l2a_items.push_back(L2A::Item(placed_item));
    }

    // Aller the user that locked and or hidden items were selected.
    if (locked_counter > 0)
    {
        ai::UnicodeString message_text("The LaTeX2AI Redo function got ");
        message_text += L2A::UTIL::IntegerToString(locked_counter);
        message_text += " hidden / locked items. They will be skipped.";
        if (is_all_items && redo_latex)
            message_text += "\nThe LaTeX2AI label PDFs in the PDF direcory will not be cleaned.";
        sAIUser->MessageAlert(message_text);
    }

    if (redo_latex)
    {
        // Loop over every element and get the latex code as string.
        ai::UnicodeString global_latex_code("\n\n");
        for (const auto& item : l2a_items)
        {
            global_latex_code += "% Name of the item in Illustrator: \"" + item.GetAIName() + "\"\n";
            global_latex_code += item.GetProperty().GetLaTeXCode();
            global_latex_code += "\n\n";
        }

        // Create a latex document with the code from all items to redo.
        ai::FilePath pdf_path;
        L2A::LATEX::LatexCreationResult result =
            L2A::LATEX::CreateLatexWithDebug(global_latex_code, pdf_path, ai::UnicodeString("redo_all"));
        if (result == L2A::LATEX::LatexCreationResult::fail_quit ||
            result == L2A::LATEX::LatexCreationResult::fail_redo)
            return;

        // Split up the created items into the individual pdf files.
        std::vector<ai::FilePath> pdf_files = L2A::LATEX::SplitPdfPages(pdf_path, (unsigned int)l2a_items.size());

        if (is_all_items && locked_counter == 0)
        {
            // Delete all the items for this document, and restart the counter.
            ai::UnicodeString document_name = L2A::UTIL::GetDocumentName();
            ai::FilePath pdf_file_directory = L2A::UTIL::GetPdfFileDirectory();
            ai::UnicodeString pattern = "\\" + document_name + "_LaTeX2AI_*.pdf";
            std::vector<ai::FilePath> old_pdf_files = L2A::UTIL::FindFilesInFolder(pdf_file_directory, pattern);
            for (const auto& pdf : old_pdf_files) L2A::UTIL::RemoveFile(pdf);

            for (unsigned int i = 0; i < l2a_items.size(); i++)
            {
                // Move the new pdf to the header directory.
                ai::FilePath item_pdf_path = pdf_file_directory;
                item_pdf_path.AddComponent(L2A::NAMES::GetPdfItemName(document_name, i + 1));
                L2A::UTIL::CopyFileL2A(pdf_files[i], item_pdf_path);
                L2A::AI::RelinkPlacedItem(l2a_items[i].GetPlacedItemMutable(), item_pdf_path);
                l2a_items[i].SetNote();
                L2A::AI::SetName(l2a_items[i].GetPlacedItemMutable(), L2A::NAMES::GetAIItemName(i + 1));
            }
        }
        else
        {
            // Replace the old pdfs with the new ones, and relink the items.
            for (unsigned int i_rename = 0; i_rename < (unsigned int)l2a_items.size(); i_rename++)
            {
                ai::FilePath item_pdf_path = L2A::AI::GetPlacedItemPath(l2a_items[i_rename].GetPlacedItemMutable());
                L2A::UTIL::CopyFileL2A(pdf_files[i_rename], item_pdf_path);
                L2A::AI::RelinkPlacedItem(l2a_items[i_rename].GetPlacedItemMutable(), item_pdf_path);
                l2a_items[i_rename].SetNote();
            }
        }
    }

    // Redo the boundaries of all items.
    if (redo_boundary)
        for (auto& item : l2a_items) item.RedoBoundary();
}

/**
 *
 */
void L2A::RelinkCopiedItems()
{
    // If the document is not saved, nothing needs to be done in this function.
    if (!L2A::UTIL::IsFile(L2A::UTIL::GetDocumentPath(false))) return;

    // Get all selected items and store them. Those items have to be reselected at the end of this function, to ensure
    // that all copied items are selected and the user can directly modify (e.g. move) them.
    std::vector<AIArtHandle> selected_items_all;
    L2A::AI::GetItems(selected_items_all, L2A::AI::SelectionState::selected);

    // Get the selected items. Those are candidates for items that were just paste into the document.
    std::vector<AIArtHandle> selected_items;
    L2A::AI::GetDocumentItems(selected_items, L2A::AI::SelectionState::selected);

    // No L2A items were changed, nothing needs to be done here.
    if (selected_items.size() == 0) return;

    // Get the deselected items. Those will be used to compare to the selected ones.
    std::vector<AIArtHandle> deselected_items;
    L2A::AI::GetDocumentItems(deselected_items, L2A::AI::SelectionState::deselected);

    // No L2A items existed prior to the change, nothing needs to be done here.
    if (selected_items.size() == 0) return;

    // Get all paths to the linked pdfs of the unselected items. Those are assumed to be correct.
    std::vector<ai::UnicodeString> linked_paths;
    for (const auto& item : deselected_items) linked_paths.push_back(L2A::AI::GetPlacedItemPath(item).GetFullPath());

    // This vector will contain all items that need to be relinked.
    std::vector<AIArtHandle> copied_items;

    for (const auto& item : selected_items)
    {
        ai::FilePath placed_item_path = L2A::AI::GetPlacedItemPath(item);

        // Check if the item links to a file that exists. If the linked file does not exist, the item will be skipped,
        // as the TeX file has to be recompiled.
        if (!L2A::UTIL::IsFile(placed_item_path)) continue;

        // Check if the selected item links to a file that other items in this document already link to.
        if (std::find(linked_paths.begin(), linked_paths.end(), placed_item_path.GetFullPath()) != linked_paths.end())
        {
            copied_items.push_back(item);
            continue;
        }

        // Check if the item links to a file with a name which is not valid for this document (i.e. files copied from
        // other documents).
        ai::UnicodeString placed_item_path_string = placed_item_path.GetFileName();
        ai::UnicodeString compare_string = placed_item_path_string.substr(0, placed_item_path_string.size() - 7);
        const bool same_name = compare_string == L2A::UTIL::GetDocumentName() + L2A::NAMES::pdf_item_post_fix_;
        const bool same_directory = placed_item_path.GetParent() == L2A::UTIL::GetPdfFileDirectory();
        if (!(same_directory && same_name))
        {
            copied_items.push_back(item);
            continue;
        }
    }

    // Copy the pdfs and relink the items.
    if (copied_items.size() > 0)
    {
        // Make sure the directory exists.
        L2A::UTIL::CreateDirectoryL2A(L2A::UTIL::GetPdfFileDirectory());

        for (auto& item : copied_items)
        {
            // Copy the pdf file.
            ai::FilePath next_item;
            unsigned int item_index = L2A::UTIL::GetNextItemIndex(next_item);
            L2A::UTIL::CopyFileL2A(L2A::AI::GetPlacedItemPath(item), next_item);

            // Relink the placed item.
            L2A::AI::SetPlacedItemPath(item, next_item);

            // Set the name for the  AI item.
            L2A::AI::SetName(item, L2A::NAMES::ai_item_name_prefix_ + L2A::UTIL::IntegerToString(item_index, 3));
        }

        // Select the items again that were selected after the copy mechanism.
        L2A::AI::SelectItems(selected_items_all);
    }
}
