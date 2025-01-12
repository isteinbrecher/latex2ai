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
 * \brief Define suites for LaTeX2AI.
 */


#ifndef L2A_SUITES_H_
#define L2A_SUITES_H_


#include "IllustratorSDK.h"

#include "AIAnnotator.h"
#include "AIAnnotatorDrawer.h"
#include "AIDocumentList.h"
#include "AIIsolationMode.h"
#include "AIStringFormatUtils.h"
#include "AITransformArt.h"
#include "Suites.hpp"

extern "C" AIToolSuite* sAITool;
extern "C" AIUnicodeStringSuite* sAIUnicodeString;
extern "C" AIPluginSuite* sAIPlugin;
extern "C" SPBlocksSuite* sSPBlocks;
extern "C" AIStringFormatUtilsSuite* sAIStringFormatUtils;

extern "C" AIDocumentSuite* sAIDocument;
extern "C" AIDocumentListSuite* sAIDocumentList;
extern "C" AIPlacedSuite* sAIPlaced;
extern "C" AIArtSuite* sAIArt;
extern "C" AIRealMathSuite* sAIRealMath;
extern "C" AITransformArtSuite* sAITransformArt;
extern "C" AIIsolationModeSuite* sAIIsolationMode;
extern "C" AIMatchingArtSuite* sAIMatchingArt;
extern "C" AIMdMemorySuite* sAIMdMemorySuite;
extern "C" AIUndoSuite* sAIUndo;
extern "C" AIAnnotatorSuite* sAIAnnotator;
extern "C" AIAnnotatorDrawerSuite* sAIAnnotatorDrawer;
extern "C" AIDocumentViewSuite* sAIDocumentView;
extern "C" AIHitTestSuite* sAIHitTest;
extern "C" AIActionManagerSuite* sAIActionManager;
extern "C" AIPathSuite* sAIPath;
extern "C" AIPathStyleSuite* sAIPathStyle;
extern "C" AILayerSuite* sAILayer;

#endif  // L2A_SUITES_H_
