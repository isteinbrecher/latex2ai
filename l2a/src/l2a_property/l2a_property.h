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
     *\brief Define the TextAlignHorizontal enum conversions.
     */
    inline std::array<TextAlignHorizontal, 3> TextAlignHorizontalEnums()
    {
        return {TextAlignHorizontal::left, TextAlignHorizontal::centre, TextAlignHorizontal::right};
    }
    inline std::array<ai::UnicodeString, 3> TextAlignHorizontalStrings()
    {
        return {ai::UnicodeString("left"), ai::UnicodeString("centreH"), ai::UnicodeString("right")};
    }

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
     *\brief Define the TextAlignVertical enum conversions.
     */
    inline std::array<TextAlignVertical, 4> TextAlignVerticalEnums()
    {
        return {
            TextAlignVertical::top, TextAlignVertical::centre, TextAlignVertical::baseline, TextAlignVertical::bottom};
    }
    inline std::array<ai::UnicodeString, 4> TextAlignVerticalStrings()
    {
        return {ai::UnicodeString("top"), ai::UnicodeString("centreV"), ai::UnicodeString("baseline"),
            ai::UnicodeString("bottom")};
    }

    /**
     *\brief Define enum conversions for TextAlignHorizontal and TextAlignVertical.
     */
    inline std::array<std::tuple<TextAlignHorizontal, TextAlignVertical>, 12> TextAlignTuples()
    {
        return {std::make_tuple(TextAlignHorizontal::left, TextAlignVertical::top),
            std::make_tuple(TextAlignHorizontal::centre, TextAlignVertical::top),
            std::make_tuple(TextAlignHorizontal::right, TextAlignVertical::top),
            std::make_tuple(TextAlignHorizontal::left, TextAlignVertical::centre),
            std::make_tuple(TextAlignHorizontal::centre, TextAlignVertical::centre),
            std::make_tuple(TextAlignHorizontal::right, TextAlignVertical::centre),
            std::make_tuple(TextAlignHorizontal::left, TextAlignVertical::baseline),
            std::make_tuple(TextAlignHorizontal::centre, TextAlignVertical::baseline),
            std::make_tuple(TextAlignHorizontal::right, TextAlignVertical::baseline),
            std::make_tuple(TextAlignHorizontal::left, TextAlignVertical::bottom),
            std::make_tuple(TextAlignHorizontal::centre, TextAlignVertical::bottom),
            std::make_tuple(TextAlignHorizontal::right, TextAlignVertical::bottom)};
    }
    inline std::array<PlaceAlignment, 12> TextAlignEnumsAI()
    {
        return {kTopLeft, kTopMid, kTopRight, kMidLeft, kMidMid, kMidRight, kMidLeft, kMidMid, kMidRight, kBotLeft,
            kBotMid, kBotRight};
    }

    /**
     *\brief Method how the placed item is croped in AI.
     */
    enum class PlacedArtMethod
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
     *\brief Define the PlacedArtMethod enum conversions.
     */
    inline std::array<PlacedArtMethod, 3> PlacedArtMethodEnums()
    {
        return {PlacedArtMethod::fill_to_boundary_box, PlacedArtMethod::keep_scale, PlacedArtMethod::keep_scale_clip};
    }
    inline std::array<ai::UnicodeString, 3> PlacedArtMethodStrings()
    {
        return {ai::UnicodeString("fill_to_boundary_box"), ai::UnicodeString("keep_scale"),
            ai::UnicodeString("keep_scale_clip")};
    }
    inline std::array<std::tuple<PlaceMethod, bool>, 3> PlacedArtMethodEnumsAI()
    {
        return {std::make_tuple(kConform, false), std::make_tuple(kAsIs, false), std::make_tuple(kAsIs, true)};
    }

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
         * \brief Get the latex code for this property
         */
        ai::UnicodeString GetLaTeXCode() const;

        /**
         * \brief Get the text alignment enums.
         */
        std::tuple<TextAlignHorizontal, TextAlignVertical> GetTextAlignment() const
        {
            return std::make_tuple(text_align_horizontal_, text_align_vertical_);
        }

        /**
         * \brief Get the placement method.
         */
        PlacedArtMethod GetPlacedMethod() const { return placed_method_; }

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

        //! Placement method of the item.
        PlacedArtMethod placed_method_;

        //! Position of the cursor in the form.
        unsigned int cursor_position_;
    };
}  // namespace L2A

#endif
