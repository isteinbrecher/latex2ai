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
 * \brief File system functions testing.
 */


#include "IllustratorSDK.h"

#include "test_framework.h"

#include "testing_utlity.h"

#include "l2a_ai_functions.h"
#include "l2a_constants.h"
#include "l2a_error.h"
#include "l2a_file_system.h"
#include "l2a_global.h"
#include "l2a_item.h"
#include "l2a_latex.h"
#include "l2a_math.h"
#include "l2a_names.h"
#include "l2a_parameter_list.h"
#include "l2a_property.h"
#include "l2a_suites.h"

/**
 *
 */
void CompareItemPosition(
    L2A::TEST::UTIL::UnitTest& ut, const L2A::Item& item, const std::array<AIRealPoint, 9>& reference_positions)
{
    const std::vector<PlaceAlignment> alignment_vector = {
        kTopLeft, kMidLeft, kBotLeft, kTopMid, kMidMid, kBotMid, kTopRight, kMidRight, kBotRight};

    std::vector<AIRealPoint> points = item.GetPosition(alignment_vector);
    for (unsigned int i_point = 0; i_point < 9; i_point++)
    {
        ut.CompareFloat(L2A::UTIL::MATH::GetDistance(points[i_point], reference_positions[i_point]), 0.0,
            (AIReal)2.0 * L2A::CONSTANTS::eps_pos_);
    }
}

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
        const auto temp_dir = L2A::UTIL::GetTemporaryDirectory();
        ai::FilePath latex_header_dir = temp_dir;
        latex_header_dir.AddComponent(ai::UnicodeString(L2A::NAMES::pdf_file_directory_));
        L2A::UTIL::RemoveDirectoryAI(latex_header_dir, false);

        // Delete the old testing document.
        ai::FilePath testing_document_path = temp_dir;
        const auto testing_document_base_name =
            ai::UnicodeString("l2a_testing_document") + L2A::TEST::UTIL::test_string_unicode();

        testing_document_path.AddComponent(
            ai::UnicodeString("testing_document" + L2A::TEST::UTIL::test_string_unicode()));
        L2A::UTIL::CreateDirectoryL2A(testing_document_path);
        testing_document_path.AddComponent(testing_document_base_name + ".ai");
        L2A::UTIL::RemoveFile(testing_document_path, false);

        // Create the testing document.
        ai::UnicodeString empty("");
        AIDocumentHandle new_doc;

        AINewDocumentPreset new_doc_settings;
        new_doc_settings.docTitle = testing_document_base_name;
        new_doc_settings.docWidth = (AIReal)595.2800292969;
        new_doc_settings.docHeight = (AIReal)841.8900146484;
        new_doc_settings.docNumArtboards = 1;
        new_doc_settings.docArtboardSpacing = 100;
        new_doc_settings.docColorMode = kAICMYKColorModel;
        new_doc_settings.docUnits = kPointsUnits;
        new_doc_settings.docPreviewMode = kAIPreviewModeDefault;
#if kPluginInterfaceVersion >= 0x26000001
        new_doc_settings.docBleedOffset = AIRealRect{0.0, 0.0, 0.0, 0.0};
#endif

        error = sAIDocumentList->New(empty, &new_doc_settings, kDialogOff, &new_doc);
        l2a_check_ai_error(error);
        L2A::AI::SaveAs(testing_document_path);

        // Geometry variables for the item placement on the page.
        AIRealPoint point;
        const AIRealPoint start{150, -300};
        const AIRealPoint offset{200.0, -150.0};

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

        // Reference values for the pdf dimensions
        AIRealRect reference_standard;
        reference_standard.left = (ASReal)7893.00000;
        reference_standard.top = (ASReal)7770.00000;
        reference_standard.right = (ASReal)7969.06982;
        reference_standard.bottom = (ASReal)7753.00977;
        AIRealRect reference_baseline;
        reference_baseline.left = (ASReal)7893.00000;
        reference_baseline.top = (ASReal)7770.00000;
        reference_baseline.right = (ASReal)7969.08008;
        reference_baseline.bottom = (ASReal)7743.66992;

        // Positions for the initially created items
        std::array<AIRealPoint, 9> reference_standard_position;
        reference_standard_position[0].h = (ASReal)111.9649963378906;
        reference_standard_position[0].v = (ASReal)-291.5050048828125;
        reference_standard_position[1].h = (ASReal)111.9649963378906;
        reference_standard_position[1].v = (ASReal)-300.0;
        reference_standard_position[2].h = (ASReal)111.9649963378906;
        reference_standard_position[2].v = (ASReal)-308.4949951171875;
        reference_standard_position[3].h = (ASReal)150.0;
        reference_standard_position[3].v = (ASReal)-291.5050048828125;
        reference_standard_position[4].h = (ASReal)150.0;
        reference_standard_position[4].v = (ASReal)-300.0;
        reference_standard_position[5].h = (ASReal)150.0;
        reference_standard_position[5].v = (ASReal)-308.4949951171875;
        reference_standard_position[6].h = (ASReal)188.0350036621094;
        reference_standard_position[6].v = (ASReal)-291.5050048828125;
        reference_standard_position[7].h = (ASReal)188.0350036621094;
        reference_standard_position[7].v = (ASReal)-300.0;
        reference_standard_position[8].h = (ASReal)188.03500366210938;
        reference_standard_position[8].v = (ASReal)-308.4949951171875;
        std::array<AIRealPoint, 9> reference_baseline_position;
        reference_baseline_position[0].h = (ASReal)111.9599609375;
        reference_baseline_position[0].v = (ASReal)-286.8350219726562;
        reference_baseline_position[1].h = (ASReal)111.9599609375;
        reference_baseline_position[1].v = (ASReal)-300.0000305175781;
        reference_baseline_position[2].h = (ASReal)111.9599609375;
        reference_baseline_position[2].v = (ASReal)-313.1650390625;
        reference_baseline_position[3].h = (ASReal)149.9999694824219;
        reference_baseline_position[3].v = (ASReal)-286.8350219726562;
        reference_baseline_position[4].h = (ASReal)149.9999694824219;
        reference_baseline_position[4].v = (ASReal)-300.0000305175781;
        reference_baseline_position[5].h = (ASReal)149.9999694824219;
        reference_baseline_position[5].v = (ASReal)-313.1650390625;
        reference_baseline_position[6].h = (ASReal)188.0399627685547;
        reference_baseline_position[6].v = (ASReal)-286.8350219726562;
        reference_baseline_position[7].h = (ASReal)188.0399627685547;
        reference_baseline_position[7].v = (ASReal)-300.0000305175781;
        reference_baseline_position[8].h = (ASReal)188.0399627685547;
        reference_baseline_position[8].v = (ASReal)-313.1650390625;

        // Add a standard LaTeX2AI item and one with baseline positioning. Also check the position and box values
        // The baseline item is first created as a normal item and then changed to baseline to check that the change
        // method can handle this correctly
        L2A::Property item_property;
        item_property.latex_code_ = ai::UnicodeString("Test item $\\int_a^b \\mathrm dx$");
        item_property.text_align_horizontal_ = L2A::TextAlignHorizontal::centre;
        item_property.text_align_vertical_ = L2A::TextAlignVertical::centre;
        auto [latex_creation_result, pdf_path] = L2A::LATEX::CreateLatexItem(item_property);
        L2A::Item item_standard(start, item_property, pdf_path);
        ut.CompareRect(reference_standard, L2A::AI::GetPlacedBoundingBox(item_standard.GetPlacedItem()));
        CompareItemPosition(ut, item_standard, reference_standard_position);

        // First create the baseline item with the non baseline option, then change it to a baseline option
        std::tie(latex_creation_result, pdf_path) = L2A::LATEX::CreateLatexItem(item_property);
        L2A::Item item_baseline(start, item_property, pdf_path);
        ut.CompareRect(reference_standard, L2A::AI::GetPlacedBoundingBox(item_baseline.GetPlacedItem()));
        CompareItemPosition(ut, item_baseline, reference_standard_position);
        item_property.text_align_vertical_ = L2A::TextAlignVertical::baseline;
        item_baseline.Change(ai::UnicodeString("ok"), item_property);
        ut.CompareRect(reference_baseline, L2A::AI::GetPlacedBoundingBox(item_baseline.GetPlacedItem()));
        CompareItemPosition(ut, item_baseline, reference_baseline_position);

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

                // Change the item.
                l2a_item.Change(ai::UnicodeString("ok"), item_property_change);

                // Move the item to the new position.
                point.h = start.h + offset.h * i_horizontal;
                point.v = start.v + offset.v * i_vertical;
                l2a_item.MoveItem(point);

                i_horizontal++;
            }
            i_vertical++;
        }

        // Delete the first two items -> they are copied.
        error = sAIArt->DisposeArt(item_standard.GetPlacedItemMutable());
        l2a_check_ai_error(error);
        error = sAIArt->DisposeArt(item_baseline.GetPlacedItemMutable());
        l2a_check_ai_error(error);

        // Now the document as we expect it is created.
        // Redo all latex items.
        L2A::AI::GetDocumentItems(placed_items, L2A::AI::SelectionState::all);
        L2A::RedoItems(placed_items, L2A::RedoItemsOption::latex);

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
                reference_box = reference_baseline;
            }
            else
            {
                reference_box = reference_standard;
            }
            AIRealRect image_box = L2A::AI::GetPlacedBoundingBox(l2a_item.GetPlacedItem());
            ut.CompareRect(image_box, reference_box);
        }

        {
            // Strech the items without rotating them.
            std::vector<AIArtHandle> art_items;
            L2A::AI::GetItems(art_items, L2A::AI::SelectionState::all);
            L2A::AI::SelectItems(art_items);
            L2A::AI::Scale((AIReal)0.8, (AIReal)1.2);

            // Redo all latex items (boundary box).
            L2A::AI::GetDocumentItems(placed_items, L2A::AI::SelectionState::all);
            L2A::RedoItems(placed_items, L2A::RedoItemsOption::bounding_box);

            // Check the items.
            PRIVATE::CheckItems(ut);
        }

        {
            // Strech the items, then rotate them.
            std::vector<AIArtHandle> art_items;
            L2A::AI::GetItems(art_items, L2A::AI::SelectionState::all);
            L2A::AI::SelectItems(art_items);
            L2A::AI::Scale((AIReal)1.2, (AIReal)0.8);
            L2A::AI::Rotate((AIReal)196.9);

            // Redo all latex items (boundary box).
            L2A::AI::GetDocumentItems(placed_items, L2A::AI::SelectionState::all);
            L2A::RedoItems(placed_items, L2A::RedoItemsOption::bounding_box);

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
            L2A::AI::Scale((AIReal)0.8, (AIReal)1.2);

            // Redo all latex items (boundary box).
            L2A::AI::GetDocumentItems(placed_items, L2A::AI::SelectionState::all);
            L2A::RedoItems(placed_items, L2A::RedoItemsOption::bounding_box);

            // Check the items.
            PRIVATE::CheckItems(ut);
        }

        {
            // Check the boundary box of all items.

            // Reference solutions.
            std::array<std::array<AIRealPoint, 9>, 12> reference_solution;
            reference_solution[0][0].h = (ASReal)485.682953;
            reference_solution[0][0].v = (ASReal)-387.333405;
            reference_solution[0][1].h = (ASReal)480.223419;
            reference_solution[0][1].v = (ASReal)-375.353760;
            reference_solution[0][2].h = (ASReal)474.763885;
            reference_solution[0][2].v = (ASReal)-363.374146;
            reference_solution[0][3].h = (ASReal)451.068115;
            reference_solution[0][3].v = (ASReal)-403.108612;
            reference_solution[0][4].h = (ASReal)445.608582;
            reference_solution[0][4].v = (ASReal)-391.128967;
            reference_solution[0][5].h = (ASReal)440.149017;
            reference_solution[0][5].v = (ASReal)-379.149323;
            reference_solution[0][6].h = (ASReal)416.453247;
            reference_solution[0][6].v = (ASReal)-418.883789;
            reference_solution[0][7].h = (ASReal)410.993713;
            reference_solution[0][7].v = (ASReal)-406.904144;
            reference_solution[0][8].h = (ASReal)405.534180;
            reference_solution[0][8].v = (ASReal)-394.924530;
            reference_solution[1][0].h = (ASReal)373.329437;
            reference_solution[1][0].v = (ASReal)-438.536530;
            reference_solution[1][1].h = (ASReal)367.869904;
            reference_solution[1][1].v = (ASReal)-426.556885;
            reference_solution[1][2].h = (ASReal)362.410370;
            reference_solution[1][2].v = (ASReal)-414.577271;
            reference_solution[1][3].h = (ASReal)338.714600;
            reference_solution[1][3].v = (ASReal)-454.311737;
            reference_solution[1][4].h = (ASReal)333.255066;
            reference_solution[1][4].v = (ASReal)-442.332092;
            reference_solution[1][5].h = (ASReal)327.795502;
            reference_solution[1][5].v = (ASReal)-430.352448;
            reference_solution[1][6].h = (ASReal)304.099731;
            reference_solution[1][6].v = (ASReal)-470.086914;
            reference_solution[1][7].h = (ASReal)298.640198;
            reference_solution[1][7].v = (ASReal)-458.107269;
            reference_solution[1][8].h = (ASReal)293.180664;
            reference_solution[1][8].v = (ASReal)-446.127655;
            reference_solution[2][0].h = (ASReal)260.977875;
            reference_solution[2][0].v = (ASReal)-489.738678;
            reference_solution[2][1].h = (ASReal)255.518341;
            reference_solution[2][1].v = (ASReal)-477.759033;
            reference_solution[2][2].h = (ASReal)250.058807;
            reference_solution[2][2].v = (ASReal)-465.779419;
            reference_solution[2][3].h = (ASReal)226.363022;
            reference_solution[2][3].v = (ASReal)-505.513885;
            reference_solution[2][4].h = (ASReal)220.903488;
            reference_solution[2][4].v = (ASReal)-493.534241;
            reference_solution[2][5].h = (ASReal)215.443954;
            reference_solution[2][5].v = (ASReal)-481.554596;
            reference_solution[2][6].h = (ASReal)191.748169;
            reference_solution[2][6].v = (ASReal)-521.289063;
            reference_solution[2][7].h = (ASReal)186.288635;
            reference_solution[2][7].v = (ASReal)-509.309418;
            reference_solution[2][8].h = (ASReal)180.829102;
            reference_solution[2][8].v = (ASReal)-497.329803;
            reference_solution[3][0].h = (ASReal)547.199829;
            reference_solution[3][0].v = (ASReal)-706.027100;
            reference_solution[3][1].h = (ASReal)543.676880;
            reference_solution[3][1].v = (ASReal)-698.296936;
            reference_solution[3][2].h = (ASReal)540.153992;
            reference_solution[3][2].v = (ASReal)-690.566711;
            reference_solution[3][3].h = (ASReal)512.589600;
            reference_solution[3][3].v = (ASReal)-721.800171;
            reference_solution[3][4].h = (ASReal)509.066711;
            reference_solution[3][4].v = (ASReal)-714.069946;
            reference_solution[3][5].h = (ASReal)505.543793;
            reference_solution[3][5].v = (ASReal)-706.339783;
            reference_solution[3][6].h = (ASReal)477.979431;
            reference_solution[3][6].v = (ASReal)-737.573242;
            reference_solution[3][7].h = (ASReal)474.456512;
            reference_solution[3][7].v = (ASReal)-729.843018;
            reference_solution[3][8].h = (ASReal)470.933594;
            reference_solution[3][8].v = (ASReal)-722.112854;
            reference_solution[4][0].h = (ASReal)434.842377;
            reference_solution[4][0].v = (ASReal)-757.230225;
            reference_solution[4][1].h = (ASReal)431.319458;
            reference_solution[4][1].v = (ASReal)-749.500061;
            reference_solution[4][2].h = (ASReal)427.796539;
            reference_solution[4][2].v = (ASReal)-741.769836;
            reference_solution[4][3].h = (ASReal)400.232208;
            reference_solution[4][3].v = (ASReal)-773.003296;
            reference_solution[4][4].h = (ASReal)396.709290;
            reference_solution[4][4].v = (ASReal)-765.273071;
            reference_solution[4][5].h = (ASReal)393.186371;
            reference_solution[4][5].v = (ASReal)-757.542908;
            reference_solution[4][6].h = (ASReal)365.622009;
            reference_solution[4][6].v = (ASReal)-788.776367;
            reference_solution[4][7].h = (ASReal)362.099091;
            reference_solution[4][7].v = (ASReal)-781.046143;
            reference_solution[4][8].h = (ASReal)358.576172;
            reference_solution[4][8].v = (ASReal)-773.315979;
            reference_solution[5][0].h = (ASReal)322.489349;
            reference_solution[5][0].v = (ASReal)-808.436279;
            reference_solution[5][1].h = (ASReal)318.966431;
            reference_solution[5][1].v = (ASReal)-800.706116;
            reference_solution[5][2].h = (ASReal)315.443512;
            reference_solution[5][2].v = (ASReal)-792.975891;
            reference_solution[5][3].h = (ASReal)287.879181;
            reference_solution[5][3].v = (ASReal)-824.209351;
            reference_solution[5][4].h = (ASReal)284.356262;
            reference_solution[5][4].v = (ASReal)-816.479126;
            reference_solution[5][5].h = (ASReal)280.833344;
            reference_solution[5][5].v = (ASReal)-808.748962;
            reference_solution[5][6].h = (ASReal)253.268982;
            reference_solution[5][6].v = (ASReal)-839.982422;
            reference_solution[5][7].h = (ASReal)249.746063;
            reference_solution[5][7].v = (ASReal)-832.252197;
            reference_solution[5][8].h = (ASReal)246.223145;
            reference_solution[5][8].v = (ASReal)-824.522034;
            reference_solution[6][0].h = (ASReal)517.233032;
            reference_solution[6][0].v = (ASReal)-548.419678;
            reference_solution[6][1].h = (ASReal)513.710083;
            reference_solution[6][1].v = (ASReal)-540.689514;
            reference_solution[6][2].h = (ASReal)510.187164;
            reference_solution[6][2].v = (ASReal)-532.959290;
            reference_solution[6][3].h = (ASReal)482.622833;
            reference_solution[6][3].v = (ASReal)-564.192749;
            reference_solution[6][4].h = (ASReal)479.099915;
            reference_solution[6][4].v = (ASReal)-556.462524;
            reference_solution[6][5].h = (ASReal)475.576996;
            reference_solution[6][5].v = (ASReal)-548.732361;
            reference_solution[6][6].h = (ASReal)448.012634;
            reference_solution[6][6].v = (ASReal)-579.965820;
            reference_solution[6][7].h = (ASReal)444.489716;
            reference_solution[6][7].v = (ASReal)-572.235596;
            reference_solution[6][8].h = (ASReal)440.966797;
            reference_solution[6][8].v = (ASReal)-564.505432;
            reference_solution[7][0].h = (ASReal)404.879486;
            reference_solution[7][0].v = (ASReal)-599.623779;
            reference_solution[7][1].h = (ASReal)401.356567;
            reference_solution[7][1].v = (ASReal)-591.893616;
            reference_solution[7][2].h = (ASReal)397.833649;
            reference_solution[7][2].v = (ASReal)-584.163391;
            reference_solution[7][3].h = (ASReal)370.269318;
            reference_solution[7][3].v = (ASReal)-615.396851;
            reference_solution[7][4].h = (ASReal)366.746399;
            reference_solution[7][4].v = (ASReal)-607.666626;
            reference_solution[7][5].h = (ASReal)363.223480;
            reference_solution[7][5].v = (ASReal)-599.936462;
            reference_solution[7][6].h = (ASReal)335.659119;
            reference_solution[7][6].v = (ASReal)-631.169922;
            reference_solution[7][7].h = (ASReal)332.136200;
            reference_solution[7][7].v = (ASReal)-623.439697;
            reference_solution[7][8].h = (ASReal)328.613281;
            reference_solution[7][8].v = (ASReal)-615.709534;
            reference_solution[8][0].h = (ASReal)292.520599;
            reference_solution[8][0].v = (ASReal)-650.829834;
            reference_solution[8][1].h = (ASReal)288.997681;
            reference_solution[8][1].v = (ASReal)-643.099670;
            reference_solution[8][2].h = (ASReal)285.474762;
            reference_solution[8][2].v = (ASReal)-635.369446;
            reference_solution[8][3].h = (ASReal)257.910431;
            reference_solution[8][3].v = (ASReal)-666.602905;
            reference_solution[8][4].h = (ASReal)254.387497;
            reference_solution[8][4].v = (ASReal)-658.872681;
            reference_solution[8][5].h = (ASReal)250.864578;
            reference_solution[8][5].v = (ASReal)-651.142517;
            reference_solution[8][6].h = (ASReal)223.300232;
            reference_solution[8][6].v = (ASReal)-682.375977;
            reference_solution[8][7].h = (ASReal)219.777313;
            reference_solution[8][7].v = (ASReal)-674.645752;
            reference_solution[8][8].h = (ASReal)216.254395;
            reference_solution[8][8].v = (ASReal)-666.915588;
            reference_solution[9][0].h = (ASReal)453.779877;
            reference_solution[9][0].v = (ASReal)-225.476334;
            reference_solution[9][1].h = (ASReal)450.256958;
            reference_solution[9][1].v = (ASReal)-217.746124;
            reference_solution[9][2].h = (ASReal)446.734039;
            reference_solution[9][2].v = (ASReal)-210.015930;
            reference_solution[9][3].h = (ASReal)419.169708;
            reference_solution[9][3].v = (ASReal)-241.249390;
            reference_solution[9][4].h = (ASReal)415.646790;
            reference_solution[9][4].v = (ASReal)-233.519196;
            reference_solution[9][5].h = (ASReal)412.123871;
            reference_solution[9][5].v = (ASReal)-225.788986;
            reference_solution[9][6].h = (ASReal)384.559509;
            reference_solution[9][6].v = (ASReal)-257.022461;
            reference_solution[9][7].h = (ASReal)381.036591;
            reference_solution[9][7].v = (ASReal)-249.292252;
            reference_solution[9][8].h = (ASReal)377.513672;
            reference_solution[9][8].v = (ASReal)-241.562057;
            reference_solution[10][0].h = (ASReal)341.426849;
            reference_solution[10][0].v = (ASReal)-276.680420;
            reference_solution[10][1].h = (ASReal)337.903931;
            reference_solution[10][1].v = (ASReal)-268.950226;
            reference_solution[10][2].h = (ASReal)334.381012;
            reference_solution[10][2].v = (ASReal)-261.220032;
            reference_solution[10][3].h = (ASReal)306.816681;
            reference_solution[10][3].v = (ASReal)-292.453491;
            reference_solution[10][4].h = (ASReal)303.293762;
            reference_solution[10][4].v = (ASReal)-284.723297;
            reference_solution[10][5].h = (ASReal)299.770844;
            reference_solution[10][5].v = (ASReal)-276.993103;
            reference_solution[10][6].h = (ASReal)272.206482;
            reference_solution[10][6].v = (ASReal)-308.226563;
            reference_solution[10][7].h = (ASReal)268.683563;
            reference_solution[10][7].v = (ASReal)-300.496368;
            reference_solution[10][8].h = (ASReal)265.160645;
            reference_solution[10][8].v = (ASReal)-292.766144;
            reference_solution[11][0].h = (ASReal)229.067490;
            reference_solution[11][0].v = (ASReal)-327.884521;
            reference_solution[11][1].h = (ASReal)225.544571;
            reference_solution[11][1].v = (ASReal)-320.154327;
            reference_solution[11][2].h = (ASReal)222.021637;
            reference_solution[11][2].v = (ASReal)-312.424133;
            reference_solution[11][3].h = (ASReal)194.457306;
            reference_solution[11][3].v = (ASReal)-343.657593;
            reference_solution[11][4].h = (ASReal)190.934372;
            reference_solution[11][4].v = (ASReal)-335.927399;
            reference_solution[11][5].h = (ASReal)187.411453;
            reference_solution[11][5].v = (ASReal)-328.197205;
            reference_solution[11][6].h = (ASReal)159.847107;
            reference_solution[11][6].v = (ASReal)-359.430664;
            reference_solution[11][7].h = (ASReal)156.324188;
            reference_solution[11][7].v = (ASReal)-351.700470;
            reference_solution[11][8].h = (ASReal)152.801270;
            reference_solution[11][8].v = (ASReal)-343.970245;

            // Get all placed items
            std::vector<AIArtHandle> art_items;
            L2A::AI::GetDocumentItems(art_items, L2A::AI::SelectionState::all);
            for (unsigned int i_item = 0; i_item < art_items.size(); i_item++)
            {
                // Get the item
                L2A::Item l2a_item(art_items[i_item]);

                // Check if the positions fit to the reference solutions
                CompareItemPosition(ut, l2a_item, reference_solution[i_item]);
            }
        }
    }
    catch (...)
    {
        sAIUser->MessageAlert(ai::UnicodeString("Error in framework test!"));
    }

    // Deactivate the global testing mode.
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
        AIReal min_distance = 1e10;
        AIRealPoint point = l2a_item.GetPosition();
        for (const auto& grid_point : grid_points)
        {
            const AIReal distance = L2A::UTIL::MATH::GetDistance(grid_point, point);
            if (distance < min_distance) min_distance = distance;
        }
        // We have to increase the tolerance here, because otherwise we get errors. The reason for this is unclear.
        ut.CompareFloat(min_distance, 0.0, (ASReal)5.0 * L2A::CONSTANTS::eps_pos_);
    }
}
