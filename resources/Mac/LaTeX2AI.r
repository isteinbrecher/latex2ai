//========================================================================================
//
//  $File$
//
//  $Revision$
//
//  Copyright 1987 Adobe Systems Incorporated. All rights reserved.
//
//  NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance
//  with the terms of the Adobe license agreement accompanying it.  If you have received
//  this file from a source other than Adobe, then your use, modification, or
//  distribution of it requires the prior written permission of Adobe.
//
//========================================================================================

/*
 * Two Resource files are required for Mac one for the standard definition resources and one
 * for HiDPI resources. The resource entries for both are required regardless of whether
 * you will be providing HiDPI icons for your tools. If you do not provide the two resource
 * files you will receive [CANT] Errors on startup.
 *
 * The resource files should follow the naming convention:
 * PluginName.r -> eg. Annotator.r - for standard image definitions
 * PluginName2x.r -> eg. Annotator2x.r - for HiDPI image definitions
 *
 *
 * Changes in resource handling:
 * In 10.8 SDK, Apple deprecated all .r/.rsrc resource reading APIs. Now all resources files
 * will need to be copied to <YourPlugin.aip>/Contents/Resources/<extension>/ as post build step.
 *
 * In this sample there are 9 resources used.
 *
 * The resources are added in the Run Script build phase of the target. Each resource is added
 * to a folder named after its extension.
 * e.g All png folders should be copied to <YourPlugin.aip>/Contents/Resources/png/.
 *
 * IDToFile.txt
 * --------------
 * Take a look in IDToFile.txt for how to define resources.
 */

#include <Carbon/Carbon.r>
