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
        L2A::ERR::check_ai_error(error);
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
        L2A::ERR::check_ai_error(error);
        error = sAIArt->DisposeArt(item_baseline.GetPlacedItemMutable());
        L2A::ERR::check_ai_error(error);

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

        {
            // Check the boundary box of all items.

            // Reference solutions.
            std::vector<std::vector<AIRealPoint>> reference_solution(12, std::vector<AIRealPoint>(9));
            reference_solution[0][0].h = 485.682953;
            reference_solution[0][0].v = -387.333405;
            reference_solution[0][1].h = 480.223419;
            reference_solution[0][1].v = -375.353760;
            reference_solution[0][2].h = 474.763885;
            reference_solution[0][2].v = -363.374146;
            reference_solution[0][3].h = 451.068115;
            reference_solution[0][3].v = -403.108612;
            reference_solution[0][4].h = 445.608582;
            reference_solution[0][4].v = -391.128967;
            reference_solution[0][5].h = 440.149017;
            reference_solution[0][5].v = -379.149323;
            reference_solution[0][6].h = 416.453247;
            reference_solution[0][6].v = -418.883789;
            reference_solution[0][7].h = 410.993713;
            reference_solution[0][7].v = -406.904144;
            reference_solution[0][8].h = 405.534180;
            reference_solution[0][8].v = -394.924530;
            reference_solution[1][0].h = 373.329437;
            reference_solution[1][0].v = -438.536530;
            reference_solution[1][1].h = 367.869904;
            reference_solution[1][1].v = -426.556885;
            reference_solution[1][2].h = 362.410370;
            reference_solution[1][2].v = -414.577271;
            reference_solution[1][3].h = 338.714600;
            reference_solution[1][3].v = -454.311737;
            reference_solution[1][4].h = 333.255066;
            reference_solution[1][4].v = -442.332092;
            reference_solution[1][5].h = 327.795502;
            reference_solution[1][5].v = -430.352448;
            reference_solution[1][6].h = 304.099731;
            reference_solution[1][6].v = -470.086914;
            reference_solution[1][7].h = 298.640198;
            reference_solution[1][7].v = -458.107269;
            reference_solution[1][8].h = 293.180664;
            reference_solution[1][8].v = -446.127655;
            reference_solution[2][0].h = 260.977875;
            reference_solution[2][0].v = -489.738678;
            reference_solution[2][1].h = 255.518341;
            reference_solution[2][1].v = -477.759033;
            reference_solution[2][2].h = 250.058807;
            reference_solution[2][2].v = -465.779419;
            reference_solution[2][3].h = 226.363022;
            reference_solution[2][3].v = -505.513885;
            reference_solution[2][4].h = 220.903488;
            reference_solution[2][4].v = -493.534241;
            reference_solution[2][5].h = 215.443954;
            reference_solution[2][5].v = -481.554596;
            reference_solution[2][6].h = 191.748169;
            reference_solution[2][6].v = -521.289063;
            reference_solution[2][7].h = 186.288635;
            reference_solution[2][7].v = -509.309418;
            reference_solution[2][8].h = 180.829102;
            reference_solution[2][8].v = -497.329803;
            reference_solution[3][0].h = 547.199829;
            reference_solution[3][0].v = -706.027100;
            reference_solution[3][1].h = 543.676880;
            reference_solution[3][1].v = -698.296936;
            reference_solution[3][2].h = 540.153992;
            reference_solution[3][2].v = -690.566711;
            reference_solution[3][3].h = 512.589600;
            reference_solution[3][3].v = -721.800171;
            reference_solution[3][4].h = 509.066711;
            reference_solution[3][4].v = -714.069946;
            reference_solution[3][5].h = 505.543793;
            reference_solution[3][5].v = -706.339783;
            reference_solution[3][6].h = 477.979431;
            reference_solution[3][6].v = -737.573242;
            reference_solution[3][7].h = 474.456512;
            reference_solution[3][7].v = -729.843018;
            reference_solution[3][8].h = 470.933594;
            reference_solution[3][8].v = -722.112854;
            reference_solution[4][0].h = 434.842377;
            reference_solution[4][0].v = -757.230225;
            reference_solution[4][1].h = 431.319458;
            reference_solution[4][1].v = -749.500061;
            reference_solution[4][2].h = 427.796539;
            reference_solution[4][2].v = -741.769836;
            reference_solution[4][3].h = 400.232208;
            reference_solution[4][3].v = -773.003296;
            reference_solution[4][4].h = 396.709290;
            reference_solution[4][4].v = -765.273071;
            reference_solution[4][5].h = 393.186371;
            reference_solution[4][5].v = -757.542908;
            reference_solution[4][6].h = 365.622009;
            reference_solution[4][6].v = -788.776367;
            reference_solution[4][7].h = 362.099091;
            reference_solution[4][7].v = -781.046143;
            reference_solution[4][8].h = 358.576172;
            reference_solution[4][8].v = -773.315979;
            reference_solution[5][0].h = 322.489349;
            reference_solution[5][0].v = -808.436279;
            reference_solution[5][1].h = 318.966431;
            reference_solution[5][1].v = -800.706116;
            reference_solution[5][2].h = 315.443512;
            reference_solution[5][2].v = -792.975891;
            reference_solution[5][3].h = 287.879181;
            reference_solution[5][3].v = -824.209351;
            reference_solution[5][4].h = 284.356262;
            reference_solution[5][4].v = -816.479126;
            reference_solution[5][5].h = 280.833344;
            reference_solution[5][5].v = -808.748962;
            reference_solution[5][6].h = 253.268982;
            reference_solution[5][6].v = -839.982422;
            reference_solution[5][7].h = 249.746063;
            reference_solution[5][7].v = -832.252197;
            reference_solution[5][8].h = 246.223145;
            reference_solution[5][8].v = -824.522034;
            reference_solution[6][0].h = 517.233032;
            reference_solution[6][0].v = -548.419678;
            reference_solution[6][1].h = 513.710083;
            reference_solution[6][1].v = -540.689514;
            reference_solution[6][2].h = 510.187164;
            reference_solution[6][2].v = -532.959290;
            reference_solution[6][3].h = 482.622833;
            reference_solution[6][3].v = -564.192749;
            reference_solution[6][4].h = 479.099915;
            reference_solution[6][4].v = -556.462524;
            reference_solution[6][5].h = 475.576996;
            reference_solution[6][5].v = -548.732361;
            reference_solution[6][6].h = 448.012634;
            reference_solution[6][6].v = -579.965820;
            reference_solution[6][7].h = 444.489716;
            reference_solution[6][7].v = -572.235596;
            reference_solution[6][8].h = 440.966797;
            reference_solution[6][8].v = -564.505432;
            reference_solution[7][0].h = 404.879486;
            reference_solution[7][0].v = -599.623779;
            reference_solution[7][1].h = 401.356567;
            reference_solution[7][1].v = -591.893616;
            reference_solution[7][2].h = 397.833649;
            reference_solution[7][2].v = -584.163391;
            reference_solution[7][3].h = 370.269318;
            reference_solution[7][3].v = -615.396851;
            reference_solution[7][4].h = 366.746399;
            reference_solution[7][4].v = -607.666626;
            reference_solution[7][5].h = 363.223480;
            reference_solution[7][5].v = -599.936462;
            reference_solution[7][6].h = 335.659119;
            reference_solution[7][6].v = -631.169922;
            reference_solution[7][7].h = 332.136200;
            reference_solution[7][7].v = -623.439697;
            reference_solution[7][8].h = 328.613281;
            reference_solution[7][8].v = -615.709534;
            reference_solution[8][0].h = 292.520599;
            reference_solution[8][0].v = -650.829834;
            reference_solution[8][1].h = 288.997681;
            reference_solution[8][1].v = -643.099670;
            reference_solution[8][2].h = 285.474762;
            reference_solution[8][2].v = -635.369446;
            reference_solution[8][3].h = 257.910431;
            reference_solution[8][3].v = -666.602905;
            reference_solution[8][4].h = 254.387497;
            reference_solution[8][4].v = -658.872681;
            reference_solution[8][5].h = 250.864578;
            reference_solution[8][5].v = -651.142517;
            reference_solution[8][6].h = 223.300232;
            reference_solution[8][6].v = -682.375977;
            reference_solution[8][7].h = 219.777313;
            reference_solution[8][7].v = -674.645752;
            reference_solution[8][8].h = 216.254395;
            reference_solution[8][8].v = -666.915588;
            reference_solution[9][0].h = 453.779877;
            reference_solution[9][0].v = -225.476334;
            reference_solution[9][1].h = 450.256958;
            reference_solution[9][1].v = -217.746124;
            reference_solution[9][2].h = 446.734039;
            reference_solution[9][2].v = -210.015930;
            reference_solution[9][3].h = 419.169708;
            reference_solution[9][3].v = -241.249390;
            reference_solution[9][4].h = 415.646790;
            reference_solution[9][4].v = -233.519196;
            reference_solution[9][5].h = 412.123871;
            reference_solution[9][5].v = -225.788986;
            reference_solution[9][6].h = 384.559509;
            reference_solution[9][6].v = -257.022461;
            reference_solution[9][7].h = 381.036591;
            reference_solution[9][7].v = -249.292252;
            reference_solution[9][8].h = 377.513672;
            reference_solution[9][8].v = -241.562057;
            reference_solution[10][0].h = 341.426849;
            reference_solution[10][0].v = -276.680420;
            reference_solution[10][1].h = 337.903931;
            reference_solution[10][1].v = -268.950226;
            reference_solution[10][2].h = 334.381012;
            reference_solution[10][2].v = -261.220032;
            reference_solution[10][3].h = 306.816681;
            reference_solution[10][3].v = -292.453491;
            reference_solution[10][4].h = 303.293762;
            reference_solution[10][4].v = -284.723297;
            reference_solution[10][5].h = 299.770844;
            reference_solution[10][5].v = -276.993103;
            reference_solution[10][6].h = 272.206482;
            reference_solution[10][6].v = -308.226563;
            reference_solution[10][7].h = 268.683563;
            reference_solution[10][7].v = -300.496368;
            reference_solution[10][8].h = 265.160645;
            reference_solution[10][8].v = -292.766144;
            reference_solution[11][0].h = 229.067490;
            reference_solution[11][0].v = -327.884521;
            reference_solution[11][1].h = 225.544571;
            reference_solution[11][1].v = -320.154327;
            reference_solution[11][2].h = 222.021637;
            reference_solution[11][2].v = -312.424133;
            reference_solution[11][3].h = 194.457306;
            reference_solution[11][3].v = -343.657593;
            reference_solution[11][4].h = 190.934372;
            reference_solution[11][4].v = -335.927399;
            reference_solution[11][5].h = 187.411453;
            reference_solution[11][5].v = -328.197205;
            reference_solution[11][6].h = 159.847107;
            reference_solution[11][6].v = -359.430664;
            reference_solution[11][7].h = 156.324188;
            reference_solution[11][7].v = -351.700470;
            reference_solution[11][8].h = 152.801270;
            reference_solution[11][8].v = -343.970245;

            // All placement options.
            std::vector<PlaceAlignment> alignment_vector = {
                kTopLeft, kMidLeft, kBotLeft, kTopMid, kMidMid, kBotMid, kTopRight, kMidRight, kBotRight};

            // Get all placed items.
            std::vector<AIArtHandle> art_items;
            L2A::AI::GetDocumentItems(art_items, L2A::AI::SelectionState::all);
            for (unsigned int i_item = 0; i_item < art_items.size(); i_item++)
            {
                // Get the item.
                L2A::Item l2a_item(art_items[i_item]);

                // Check if the position fits to one of the reference solutions.
                bool match = true;
                std::vector<AIRealPoint> points = l2a_item.GetPosition(alignment_vector);
                for (unsigned int i_point = 0; i_point < 9; i_point++)
                {
                    const AIRealPoint& boundary_point = points[i_point];
                    const AIRealPoint& reference_point = reference_solution[i_item][i_point];
                    if (powf(powf(boundary_point.h - reference_point.h, 2.) +
                                powf(boundary_point.v - reference_point.v, 2.),
                            0.5) > L2A::CONSTANTS::eps_pos_)
                    {
                        match = false;
                        break;
                    }
                }
                ut.CompareInt(match, true);
            }
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
