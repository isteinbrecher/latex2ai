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
 * \brief Define suites for LaTeX2AI.
 */


#include "IllustratorSDK.h"
#include "l2a_suites.h"


extern "C"
{
    AIToolSuite* sAITool = NULL;
    AIUnicodeStringSuite* sAIUnicodeString = NULL;
    AIPluginSuite* sAIPlugin = NULL;
    SPBlocksSuite* sSPBlocks = NULL;
    AIStringFormatUtilsSuite* sAIStringFormatUtils = NULL;

    // Non default suites needed for L2A.
    AIDocumentSuite* sAIDocument = NULL;
    AIDocumentListSuite* sAIDocumentList = NULL;
    AIPlacedSuite* sAIPlaced = NULL;
    AIArtSuite* sAIArt = NULL;
    AIRealMathSuite* sAIRealMath = NULL;
    AITransformArtSuite* sAITransformArt = NULL;
    AIIsolationModeSuite* sAIIsolationMode = NULL;
    AIMatchingArtSuite* sAIMatchingArt = NULL;
    AIMdMemorySuite* sAIMdMemorySuite = NULL;
    AIUndoSuite* sAIUndo = NULL;
    AIAnnotatorSuite* sAIAnnotator = NULL;
    AIAnnotatorDrawerSuite* sAIAnnotatorDrawer = NULL;
    AIDocumentViewSuite* sAIDocumentView = NULL;
    AIHitTestSuite* sAIHitTest = NULL;
    AIActionManagerSuite* sAIActionManager = NULL;
    AIPathSuite* sAIPath = NULL;
    AIPathStyleSuite* sAIPathStyle = NULL;
    AILayerSuite* sAILayer = NULL;
}

ImportSuite gImportSuites[] = {kAIToolSuite, kAIToolVersion, &sAITool, kAIUnicodeStringSuite, kAIUnicodeStringVersion,
    &sAIUnicodeString, kAIPluginSuite, kAIPluginSuiteVersion, &sAIPlugin, kSPBlocksSuite, kSPBlocksSuiteVersion,
    &sSPBlocks, kAIStringFormatUtilsSuite, kAIStringFormatUtilsSuiteVersion, &sAIStringFormatUtils,

    // Non default suites needed for L2A.
    kAIDocumentSuite, kAIDocumentSuiteVersion, &sAIDocument,
    //
    kAIDocumentListSuite, kAIDocumentListSuiteVersion, &sAIDocumentList,
    //
    kAIPlacedSuite, kAIPlacedSuiteVersion, &sAIPlaced,
    //
    kAIArtSuite, kAIArtSuiteVersion, &sAIArt,
    //
    kAIRealMathSuite, kAIRealMathSuiteVersion, &sAIRealMath,
    //
    kAITransformArtSuite, kAITransformArtSuiteVersion, &sAITransformArt,
    //
    kAIIsolationModeSuite, kAIIsolationModeSuiteVersion, &sAIIsolationMode, kAIMatchingArtSuite,
    //
    kAIMatchingArtSuiteVersion, &sAIMatchingArt,
    //
    kAIMdMemorySuite, kAIMdMemorySuiteVersion, &sAIMdMemorySuite,
    //
    kAIUndoSuite, kAIUndoSuiteVersion, &sAIUndo,
    //
    kAIAnnotatorSuite, kAIAnnotatorSuiteVersion, &sAIAnnotator,
    //
    kAIAnnotatorDrawerSuite, kAIAnnotatorDrawerSuiteVersion, &sAIAnnotatorDrawer,
    //
    kAIDocumentViewSuite, kAIDocumentViewSuiteVersion, &sAIDocumentView,
    //
    kAIHitTestSuite, kAIHitTestSuiteVersion, &sAIHitTest,
    //
    kAIActionManagerSuite, kAIActionManagerSuiteVersion, &sAIActionManager,
    //
    kAIPathSuite, kAIPathSuiteVersion, &sAIPath,
    //
    kAIPathStyleSuite, kAIPathStyleSuiteVersion, &sAIPathStyle,
    //
    kAILayerSuite, kAILayerSuiteVersion, &sAILayer,

    NULL, 0, NULL};
