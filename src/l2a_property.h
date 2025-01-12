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
 * \brief An object that stores all options for a l2a item.
 */


#ifndef L2A_PROPERTY_H_
#define L2A_PROPERTY_H_

#include "IllustratorSDK.h"

#include "l2a_version.h"

#include <array>


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
    inline std::array<std::pair<TextAlignHorizontal, TextAlignVertical>, 12> TextAlignPairs()
    {
        return {std::make_pair(TextAlignHorizontal::left, TextAlignVertical::top),
            std::make_pair(TextAlignHorizontal::centre, TextAlignVertical::top),
            std::make_pair(TextAlignHorizontal::right, TextAlignVertical::top),
            std::make_pair(TextAlignHorizontal::left, TextAlignVertical::centre),
            std::make_pair(TextAlignHorizontal::centre, TextAlignVertical::centre),
            std::make_pair(TextAlignHorizontal::right, TextAlignVertical::centre),
            std::make_pair(TextAlignHorizontal::left, TextAlignVertical::baseline),
            std::make_pair(TextAlignHorizontal::centre, TextAlignVertical::baseline),
            std::make_pair(TextAlignHorizontal::right, TextAlignVertical::baseline),
            std::make_pair(TextAlignHorizontal::left, TextAlignVertical::bottom),
            std::make_pair(TextAlignHorizontal::centre, TextAlignVertical::bottom),
            std::make_pair(TextAlignHorizontal::right, TextAlignVertical::bottom)};
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
        //! Scale the placed art to the boundary bod of the item in ai. This is the only supported option by LaTeX2AI.
        fill_to_boundary_box
    };

    /**
     *\brief Define the PlacedArtMethod enum conversions.
     */
    inline std::array<PlacedArtMethod, 1> PlacedArtMethodEnums() { return {PlacedArtMethod::fill_to_boundary_box}; }
    inline std::array<std::pair<PlaceMethod, bool>, 1> PlacedArtMethodEnumsAI()
    {
        return {std::make_pair(kConform, false)};
    }

    /**
     *\brief Method used to generate the pdf hash.
     */
    enum class HashMethod
    {
        //! None
        none,
        //! CRC64 algorithm
        crc64
    };

    /**
     *\brief Define the HashMethod enum conversions.
     */
    inline std::array<HashMethod, 1> HashMethodEnums() { return {HashMethod::crc64}; }
    inline std::array<ai::UnicodeString, 1> HashMethodStrings() { return {ai::UnicodeString("crc64")}; }

    /**
     * \brief Compare flags for property items.
     */
    struct PropertyCompare
    {
        //! Latex code changed.
        bool changed_latex = false;
        //! Alignment of the item changed.
        bool changed_align = false;
        //! Cursor position changed.
        bool changed_cursor = false;

        /**
         * \brief Check if anything changed.
         */
        bool Changed() const { return changed_align || changed_latex || changed_cursor; }
    };

    /**
     * \brief Object that stores the properties for a LaTeX2AI item.
     *        It has methods to get new inout from the user, and compare different parameter sets.
     */
    class Property
    {
        // Define the friend function for testing.
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
        void SetFromParameterList(const L2A::UTIL::ParameterList& property_parameter_list);

        /**
         * \brief Set the parameters form a string.
         */
        void SetFromString(const ai::UnicodeString& string);

        /**
         * \brief Convert the parameters of this item to a string.
         * @param write_pdf_content If the content of the pdf file should be written to the parameter list. This content
         * can be large and should only be written if it is actually needed.
         */
        L2A::UTIL::ParameterList ToParameterList(const bool write_pdf_content = false) const;

        /**
         * \brief Convert the parameters of this item to a string.
         * @param write_pdf_content If the content of the pdf file should be written to the parameter list. This content
         * can be large and should only be written if it is actually needed.
         */
        ai::UnicodeString ToString(const bool write_pdf_content = false) const;

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
         * \brief Get the alignment options needed for Illustrator
         */
        PlaceAlignment GetAIAlignment() const;

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

        /**
         * \brief Get the pdf contents of the property.
         */
        ai::UnicodeString GetPDFFileContents() const { return pdf_file_encoded_; }

        /**
         * \brief Get the hash of the encoded pdf file.
         */
        ai::UnicodeString GetPDFFileHash() const { return pdf_file_hash_; }

        /**
         * \brief Encode a pdf file and store it in this property.
         */
        void SetPDFFile(const ai::FilePath& pdf_file);

        /**
         * \brief Get the version of LaTeX2AI which was used to create this item.
         */
        const semver::version& GetVersion() const { return version_; }

       private:
        //! Horizontal and Vertical alignment of the text.
        TextAlignHorizontal text_align_horizontal_;
        TextAlignVertical text_align_vertical_;

        //! LaTeX code of the item.
        ai::UnicodeString latex_code_;

        //! Position of the cursor in the form.
        unsigned int cursor_position_;

        //! Encoded pdf file.
        ai::UnicodeString pdf_file_encoded_;

        //! Hash of encoded pdf file.
        ai::UnicodeString pdf_file_hash_;

        //! Method used to get the file hash.
        HashMethod pdf_file_hash_method_;

        //! Version used to created this property
        //! This version will not be saved when the item is written to text, but rather the current version will be
        //! saved. This means that all compatibility issues have to be resoled in the time between reading and writing
        //! the property.
        semver::version version_;
    };
}  // namespace L2A

#endif
