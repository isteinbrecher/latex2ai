// -----------------------------------------------------------------------------
// MIT License
//
// Copyright (c) 2020 Ivo Steinbrecher (https://github.com/stoani89/LaTeX2AI)
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
 * \brief An object that stores all options for a l2a item.
 */


#ifndef L2A_PROPERTY_H_
#define L2A_PROPERTY_H_

#include "IllustratorSDK.h"

#include <array>
#include <tuple>


// Forward declaration.
namespace L2A
{
    namespace UTIL
    {
        class ParameterList;
    }
    namespace TEST
    {
        namespace UTIL
        {
            class UnitTest;
        }
        void TestFramework(L2A::TEST::UTIL::UnitTest& ut);
    }  // namespace TEST
}  // namespace L2A


namespace L2A
{
    /**
     *\brief Horizontal alignment of text.
     */
    enum class TextAlignHorizontal
    {
        //! None.
        none,
        //! Align with left side.
        left,
        //! Align with centre.
        centre,
        //! Align with right side.
        right
    };

    /**
     *\brief Get the flag to string array for TextAlignHorizontal.
     */
    inline std::array<std::tuple<L2A::TextAlignHorizontal, ai::UnicodeString>, 3> TextAlignHorizontalToStringArray()
    {
        std::array<std::tuple<TextAlignHorizontal, ai::UnicodeString>, 3> to_string_array;
        to_string_array[0] = std::make_tuple(L2A::TextAlignHorizontal::left, ai::UnicodeString("left"));
        to_string_array[1] = std::make_tuple(L2A::TextAlignHorizontal::centre, ai::UnicodeString("centreH"));
        to_string_array[2] = std::make_tuple(L2A::TextAlignHorizontal::right, ai::UnicodeString("right"));
        return to_string_array;
    };

    /**
     *\brief Vertical alignment of text.
     */
    enum class TextAlignVertical
    {
        //! None.
        none,
        //! Align with top.
        top,
        //! Align with centre.
        centre,
        //! Align with text baseline.
        baseline,
        //! Align with bottom.
        bottom
    };

    /**
     *\brief Get the flag to string array for TextAlignVertical.
     */
    inline std::array<std::tuple<L2A::TextAlignVertical, ai::UnicodeString>, 4> TextAlignVerticalToStringArray()
    {
        std::array<std::tuple<TextAlignVertical, ai::UnicodeString>, 4> to_string_array;
        to_string_array[0] = std::make_tuple(L2A::TextAlignVertical::centre, ai::UnicodeString("centreV"));
        to_string_array[1] = std::make_tuple(L2A::TextAlignVertical::top, ai::UnicodeString("top"));
        to_string_array[2] = std::make_tuple(L2A::TextAlignVertical::bottom, ai::UnicodeString("bottom"));
        to_string_array[3] = std::make_tuple(L2A::TextAlignVertical::baseline, ai::UnicodeString("baseline"));
        return to_string_array;
    };

    /**
     *\brief Vertical alignment of text.
     */
    enum class PlacedArtOptions
    {
        //! None.
        none,
        //! Scale the placed art to the boundary bod of the item in ai.
        fill_to_boundary_box,
        //! Keep size of the item.
        keep_scale,
        //! Keep size of the item and clip at boundary box.
        keep_scale_clip
    };

    /**
     *\brief Get the flag to string array for PlacedArtOptions.
     */
    inline std::array<std::tuple<L2A::PlacedArtOptions, ai::UnicodeString>, 3> PlacedArtOptionsToStringArray()
    {
        std::array<std::tuple<PlacedArtOptions, ai::UnicodeString>, 3> to_string_array;
        to_string_array[0] =
            std::make_tuple(L2A::PlacedArtOptions::fill_to_boundary_box, ai::UnicodeString("fill_to_boundary_box"));
        to_string_array[1] = std::make_tuple(L2A::PlacedArtOptions::keep_scale, ai::UnicodeString("keep_scale"));
        to_string_array[2] =
            std::make_tuple(L2A::PlacedArtOptions::keep_scale_clip, ai::UnicodeString("keep_scale_clip"));
        return to_string_array;
    };

    /**
     * \brief Compare flags for property items.
     */
    struct PropertyCompare
    {
        //! Latex code changed.
        bool changed_latex = false;
        //! Alignment of the item changed.
        bool changed_align = false;
        //! Placement option of the item changed.
        bool changed_placement = false;
        //! Cursor position changed.
        bool changed_cursor = false;

        /**
         * \brief Check if anything changed.
         */
        bool Changed() const { return changed_align || changed_latex || changed_placement || changed_cursor; }
    };

    /**
     * \brief Object that stores the properties for a LaTeX2AI item.
     *        It has methods to get new inout from the user, and compare different parameter sets.
     */
    class Property
    {
        // Define the friend class for testing.
        friend void L2A::TEST::TestFramework(L2A::TEST::UTIL::UnitTest& ut);

       public:
        /**
         * \brief Default Constructor
         */
        Property();

        /**
         * \brief Set the default element vlaues in this object.
         */
        void DefaultPropertyValues();

        /**
         * \brief Set the parameters form a parameter list object.
         */
        void L2A::Property::SetFromParameterList(const L2A::UTIL::ParameterList& property_parameter_list);

        /**
         * \brief Set the parameters form a string.
         */
        void Property::SetFromString(const ai::UnicodeString& string);

        /**
         * \brief Convert the parameters of this item to a string.
         */
        L2A::UTIL::ParameterList ToParameterList() const;

        /**
         * \brief Convert the parameters of this item to a string.
         */
        ai::UnicodeString ToString() const;

        /**
         * \brief Compare with other property.
         */
        PropertyCompare Compare(const Property& other_property) const;

        /**
         * \brief Check if this item is a baseline item.
         */
        bool IsBaseline() const
        {
            if (text_align_vertical_ == TextAlignVertical::baseline)
                return true;
            else
                return false;
        }

        /**
         * \brief Get the input parameters from the user via a form.
         * @return False if the form was canceled.
         */
        bool GetUserInput();

        /**
         * \brief Get the latex code for this property
         */
        ai::UnicodeString GetLaTeXCode() const;

        /**
         * \brief Get the text alignment enums.
         */
        void GetTextAlignment(TextAlignHorizontal& text_align_horizontal, TextAlignVertical& text_align_vertical) const
        {
            text_align_horizontal = text_align_horizontal_;
            text_align_vertical = text_align_vertical_;
        }

        /**
         * \brief Get the placement options.
         */
        PlacedArtOptions GetPlacedOption() const { return placed_option_; }

        /**
         * \brief Write the contents of this property to the last input file.
         */
        void WriteLastInput() const;

        /**
         * \brief Set the parameters acording to the last input file.
         *
         * If the file can not be parsed to a xml, it will be deleted.
         */
        void SetFromLastInput();

       private:
        //! Horizontal and Vertical alignment of the text.
        TextAlignHorizontal text_align_horizontal_;
        TextAlignVertical text_align_vertical_;

        //! LaTeX code of the item.
        ai::UnicodeString latex_code_;

        //! Placement option of the item.
        PlacedArtOptions placed_option_;

        //! Position of the cursor in the form.
        unsigned int cursor_position_;
    };
}  // namespace L2A

#endif
