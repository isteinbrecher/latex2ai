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
 * \brief File system functions testing.
 */


#include "IllustratorSDK.h"
#include "test_framework.h"

#include "l2a_suites.h"
#include "test_utlity.h"
#include "l2a_item/l2a_item.h"
#include "l2a_global/l2a_global.h"
#include "l2a_ai_functions/l2a_ai_functions.h"
#include "utility/file_system.h"
#include "l2a_names.h"
#include "l2a_property/l2a_property.h"
#include "l2a_error/l2a_error.h"
#include "utility/parameter_list.h"
#include "l2a_constants.h"


/**
 *
 */
void L2A::TEST::TestFramework(L2A::TEST::UTIL::UnitTest& ut)
{
    // Set the global testing flag, this will prevent unwanted message windows to pop up.
    L2A::GlobalMutable().is_testing_ = true;

    try
    {
        AIErr error;

        // Delete the old LaTeX header.
        ai::FilePath latex_header_dir = L2A::Global().path_temp_;
        latex_header_dir.AddComponent(ai::UnicodeString(L2A::NAMES::pdf_file_directory_));
        L2A::UTIL::RemoveDirectoryAI(latex_header_dir, false);

        // Delete the old testing document.
        ai::FilePath testing_document_path = L2A::Global().path_temp_;
        testing_document_path.AddComponent(ai::UnicodeString("testing_document"));
        L2A::UTIL::CreateDirectoryL2A(testing_document_path);
        testing_document_path.AddComponent(ai::UnicodeString("l2a_testing_document.ai"));
        L2A::UTIL::RemoveFile(testing_document_path, false);

        // Create the testing document.
        ai::UnicodeString empty("");
        AIDocumentHandle new_doc;

        AINewDocumentPreset new_doc_settings;
        new_doc_settings.docTitle = ai::UnicodeString("l2a_testing_document");
        new_doc_settings.docWidth = (AIReal)595.2800292969;
        new_doc_settings.docHeight = (AIReal)841.8900146484;
        new_doc_settings.docNumArtboards = 1;
        new_doc_settings.docColorMode = kAICMYKColorModel;
        new_doc_settings.docUnits = kPointsUnits;
        new_doc_settings.docPreviewMode = kAIPreviewModeDefault;
        new_doc_settings.docArtboardSpacing = 100;

        error = sAIDocumentList->New(empty, &new_doc_settings, kDialogOff, &new_doc);
        l2a_check_ai_error(error);
        L2A::AI::SaveAs(testing_document_path);

        // Geometry variables for the item placement on the page.
        AIRealPoint point;
        AIRealPoint start;
        start.h = 150;
        start.v = -300;
        AIRealPoint offset;
        offset.h = 200.0;
        offset.v = -150.0;

        // Draw the grid.
        {
            // Define variables for drawing.
            std::vector<AIPathSegment> segments;
            AIPathSegment segment;

            // Style for all lines.
            AIPathStyle path_style;
            path_style.fillPaint = false;
            path_style.strokePaint = true;
            path_style.stroke.color.kind = kGrayColor;
            path_style.stroke.color.c.g.gray = kAIRealOne;
            path_style.stroke.width = 0.5;

            // Draw the enclosing rectangle.
            std::array<std::array<int, 2>, 4> factors{{{{0, 0}}, {{1, 0}}, {{1, 1}}, {{0, 1}}}};
            segments.clear();
            for (const auto& factor : factors)
            {
                point = start;
                point.h += factor[0] * 2 * offset.h;
                point.v += factor[1] * 3 * offset.v;
                segment.in = segment.out = segment.p = point;
                segment.corner = false;
                segments.push_back(segment);
            }
            L2A::AI::DrawPath(segments, path_style, true);

            // Draw the vertical lines.
            for (unsigned int i_line = 1; i_line < 3; i_line++)
            {
                segments.clear();

                point = start;
                point.v += i_line * offset.v;
                segment.in = segment.out = segment.p = point;
                segment.corner = false;
                segments.push_back(segment);

                point = start;
                point.h += 2 * offset.h;
                point.v += i_line * offset.v;
                segment.in = segment.out = segment.p = point;
                segment.corner = false;
                segments.push_back(segment);

                L2A::AI::DrawPath(segments, path_style, true);
            }

            // Draw the horizontal line.
            segments.clear();

            point = start;
            point.h += offset.h;
            segment.in = segment.out = segment.p = point;
            segment.corner = false;
            segments.push_back(segment);

            point.v += 1 * offset.v;
            segment.in = segment.out = segment.p = point;
            segments.push_back(segment);

            point.v += 1 * offset.v;
            segment.in = segment.out = segment.p = point;
            segments.push_back(segment);

            point.v += 1 * offset.v;
            segment.in = segment.out = segment.p = point;
            segments.push_back(segment);

            L2A::AI::DrawPath(segments, path_style, true);
        }

        // Add a standard LaTeX2AI item and one with baseline positioning.
        L2A::Property item_property;
        item_property.latex_code_ = ai::UnicodeString("Test item $\\int_a^b \\mathrm dx$");

        item_property.text_align_horizontal_ = L2A::TextAlignHorizontal::centre;
        item_property.text_align_vertical_ = L2A::TextAlignVertical::centre;
        L2A::GlobalMutable().testing_form_return_parameter_list_ =
            std::make_shared<L2A::UTIL::ParameterList>(item_property.ToParameterList());
        L2A::Item item_standard(start);

        item_property.text_align_vertical_ = L2A::TextAlignVertical::baseline;
        L2A::GlobalMutable().testing_form_return_parameter_list_ =
            std::make_shared<L2A::UTIL::ParameterList>(item_property.ToParameterList());
        L2A::Item item_baseline(start);

        // Copy the items and change the positioning.
        std::vector<L2A::TextAlignHorizontal> horizontal_alignment = {
            L2A::TextAlignHorizontal::left, L2A::TextAlignHorizontal::centre, L2A::TextAlignHorizontal::right};
        std::vector<L2A::TextAlignVertical> vertical_alignment = {L2A::TextAlignVertical::top,
            L2A::TextAlignVertical::centre, L2A::TextAlignVertical::baseline, L2A::TextAlignVertical::bottom};

        std::vector<AIArtHandle> items_to_select;
        std::vector<AIArtHandle> placed_items;
        unsigned int i_horizontal = 0;
        unsigned int i_vertical = 0;
        for (const auto& vertical : vertical_alignment)
        {
            i_horizontal = 0;
            for (const auto& horizontal : horizontal_alignment)
            {
                // Check which item has to be copied.
                items_to_select.clear();
                if (vertical == L2A::TextAlignVertical::baseline)
                    items_to_select.push_back(item_baseline.GetPlacedItemMutable());
                else
                    items_to_select.push_back(item_standard.GetPlacedItemMutable());

                // Select and copy the item.
                L2A::AI::SelectItems(items_to_select);
                L2A::AI::Move(0.0, 0.0, true);

                // Get the active item (the one that was just copied).
                L2A::AI::GetDocumentItems(placed_items, L2A::AI::SelectionState::selected);
                L2A::Item l2a_item(placed_items[0]);

                // Set parameter list for item change.
                L2A::Property item_property_change;
                item_property_change.latex_code_ = ai::UnicodeString("Test item $\\int_a^b \\mathrm dx$");
                item_property_change.text_align_horizontal_ = horizontal;
                item_property_change.text_align_vertical_ = vertical;
                L2A::GlobalMutable().testing_form_return_parameter_list_ =
                    std::make_shared<L2A::UTIL::ParameterList>(item_property_change.ToParameterList());

                // Change the item.
                l2a_item.Change();

                // Move the item to the new position.
                point.h = start.h + offset.h * i_horizontal;
                point.v = start.v + offset.v * i_vertical;
                l2a_item.MoveItem(point);

                i_horizontal++;
            }
            i_vertical++;
        }

        // Delete the first two items -> hey are copied.
        error = sAIArt->DisposeArt(item_standard.GetPlacedItemMutable());
        l2a_check_ai_error(error);
        error = sAIArt->DisposeArt(item_baseline.GetPlacedItemMutable());
        l2a_check_ai_error(error);

        // Now the document as we expect it is created.
        // Redo all latex items.
        std::shared_ptr<L2A::UTIL::ParameterList> form_return_list = std::make_shared<L2A::UTIL::ParameterList>();
        form_return_list->SetOption(ai::UnicodeString("redo_latex"), 1);
        form_return_list->SetOption(ai::UnicodeString("redo_boundary"), 1);
        form_return_list->SetOption(ai::UnicodeString("item_type"), ai::UnicodeString("all"));
        L2A::GlobalMutable().testing_form_return_parameter_list_ = form_return_list;
        L2A::RedoItems();

        // Check the dimensions of the placed items, i.e. this is the most basic check we have for the LaTeX document.
        std::vector<L2A::Item> l2a_items;
        L2A::AI::GetDocumentItems(placed_items, L2A::AI::SelectionState::all);
        for (auto& placed_item : placed_items) l2a_items.push_back(L2A::Item(placed_item));
        ut.CompareInt((int)l2a_items.size(), 12);
        for (auto& l2a_item : l2a_items)
        {
            AIRealRect reference_box;
            if (l2a_item.GetProperty().IsBaseline())
            {
                reference_box.left = (ASReal)7893.00000;
                reference_box.top = (ASReal)7770.00000;
                reference_box.right = (ASReal)7969.08008;
                reference_box.bottom = (ASReal)7743.66992;
            }
            else
            {
                reference_box.left = (ASReal)7893.00000;
                reference_box.top = (ASReal)7770.00000;
                reference_box.right = (ASReal)7969.06982;
                reference_box.bottom = (ASReal)7753.00977;
            }
            AIRealRect image_box = L2A::AI::GetPlacedBoundingBox(l2a_item.GetPlacedItemMutable());
            ut.CompareRect(image_box, reference_box);
        }

        {
            // Strech the items without rotating them.
            std::vector<AIArtHandle> art_items;
            L2A::AI::GetItems(art_items, L2A::AI::SelectionState::all);
            L2A::AI::SelectItems(art_items);
            L2A::AI::Scale(0.8, 1.2);

            // Redo all latex items (boundary box).
            form_return_list = std::make_shared<L2A::UTIL::ParameterList>();
            form_return_list->SetOption(ai::UnicodeString("redo_latex"), 0);
            form_return_list->SetOption(ai::UnicodeString("redo_boundary"), 1);
            form_return_list->SetOption(ai::UnicodeString("item_type"), ai::UnicodeString("all"));
            L2A::GlobalMutable().testing_form_return_parameter_list_ = form_return_list;
            L2A::RedoItems();

            // Check the items.
            PRIVATE::CheckItems(ut);
        }

        {
            // Strech the items, then rotate them.
            std::vector<AIArtHandle> art_items;
            L2A::AI::GetItems(art_items, L2A::AI::SelectionState::all);
            L2A::AI::SelectItems(art_items);
            L2A::AI::Scale(1.2, 0.8);
            L2A::AI::Rotate(196.9);

            // Redo all latex items (boundary box).
            form_return_list = std::make_shared<L2A::UTIL::ParameterList>();
            form_return_list->SetOption(ai::UnicodeString("redo_latex"), 0);
            form_return_list->SetOption(ai::UnicodeString("redo_boundary"), 1);
            form_return_list->SetOption(ai::UnicodeString("item_type"), ai::UnicodeString("all"));
            L2A::GlobalMutable().testing_form_return_parameter_list_ = form_return_list;
            L2A::RedoItems();

            // Check the rotation of the items.
            L2A::AI::GetDocumentItems(placed_items, L2A::AI::SelectionState::all);
            for (const auto& placed_item : placed_items)
            {
                L2A::Item l2a_item(placed_item);
                ut.CompareInt(abs(l2a_item.GetAngle() - 3.14159265358979323846 * (196.9 - 360) / 180.0) <
                        L2A::CONSTANTS::eps_angle_,
                    true);
            }

            // Check the items.
            PRIVATE::CheckItems(ut);
        }

        {
            // Strech the items (this will create diamond items (since the items are already rotated).
            std::vector<AIArtHandle> art_items;
            L2A::AI::GetItems(art_items, L2A::AI::SelectionState::all);
            L2A::AI::SelectItems(art_items);
            L2A::AI::Scale(0.8, 1.2);

            // Redo all latex items (boundary box).
            form_return_list = std::make_shared<L2A::UTIL::ParameterList>();
            form_return_list->SetOption(ai::UnicodeString("redo_latex"), 0);
            form_return_list->SetOption(ai::UnicodeString("redo_boundary"), 1);
            form_return_list->SetOption(ai::UnicodeString("item_type"), ai::UnicodeString("all"));
            L2A::GlobalMutable().testing_form_return_parameter_list_ = form_return_list;
            L2A::RedoItems();

            // Check the items.
            PRIVATE::CheckItems(ut);
        }
    }
    catch (...)
    {
        sAIUser->MessageAlert(ai::UnicodeString("Error in framework test!"));
    }

    // Deactivate the global testing mode.
    L2A::GlobalMutable().testing_form_return_parameter_list_ = NULL;
    L2A::GlobalMutable().is_testing_ = false;
}

/**
 *
 */
void L2A::TEST::PRIVATE::CheckItems(L2A::TEST::UTIL::UnitTest& ut)
{
    // Get the position of the grid points.
    std::vector<AIArtHandle> art_items;
    std::vector<AIRealPoint> grid_points;
    L2A::AI::GetItems(art_items, L2A::AI::SelectionState::all, kPathArt);
    for (auto& item : art_items) L2A::AI::GetPathPoints(item, grid_points, true);

    // Get all placed items.
    L2A::AI::GetDocumentItems(art_items, L2A::AI::SelectionState::all);
    for (const auto& placed_item : art_items)
    {
        // Get the item.
        L2A::Item l2a_item(placed_item);

        // Check if the position fits to one of the grid points.
        bool match = false;
        AIRealPoint point = l2a_item.GetPosition();
        for (const auto& grid_point : grid_points)
        {
            if ((abs(powf(grid_point.h - point.h, 2.)) + abs(powf(grid_point.v - point.v, 2.))) <
                L2A::CONSTANTS::eps_pos_)
            {
                match = true;
                break;
            }
        }
        ut.CompareInt(match, true);
    }
}
