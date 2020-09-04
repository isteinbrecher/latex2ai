# LaTeX2AI
LaTeX2AI is a plugin for Adobe Illustrator CS6 that enables the use of text labels typeset in LaTeX inside an Illustrator document.

## Requirements
Currently LaTeX2AI only works on Adobe Illustrator for Windows.
The following software has to be installed on the system:
- A LaTeX compiler such as [TeX Live](https://www.tug.org/texlive) or [MiKTeX](https://miktex.org)
- [Ghostscript](https://www.ghostscript.com)

## Install LaTeX2AI from pre-build binaries
Binaries can be downloaded from the [GitHub release page](https://github.com/stoani89/LaTeX2AI/releases).
They constist of two files `LaTeX2AI.aip` and `LaTeX2AIForms.exe`.
Both files have to be copied into the same directory and this directory has to be set as the Adobe Illustrator Plugin directory.

## Build LaTeX2AI from source

### Requirements
To build LaTeX2AI from source additional requirements have to be met:
- A C++ compiler has to be available on the system. This repository includes a Visual Studio 2017 solution for Windows.
- `python3` and `git` have to be available on the system.

### Getting started
1. Download and unpack the [Adobe Illustrator CS6 SDK](http://download.macromedia.com/pub/developer/illustrator/sdk/AI_CS6_SDK_Win_682.6.1.zip).
1. Go to the samplecode folder `Adobe Illustrator CS6 SDK/samplecode` and run
	```
	git clone https://github.com/stoani89/LaTeX2AI.git
	```
1. Go to `Adobe Illustrator CS6 SDK/samplecode/LaTeX2AI` and run
	```
	git submodule update --init
	```
1. Open `Adobe Illustrator CS6 SDK/samplecode/LaTeX2AI/LaTeX2AI.sln` with Visual Studio and compile the solution.


# How to use LaTeX2AI
The plugin adds four buttons to the main toolbar.

- ![Create / Edit](/l2a/resources/create_light.png) **Create / Edit**: Either create a new LaTeX2AI item or edit an existing one in the document.
- ![Redo items](/l2a/resources/redo_light.png) **Redo LaTeX2AI items**: This allows for the LaTeX recompilation and / or boundary box reset of all existing LaTeX2AI items.
- ![LaTeX2AI options](/l2a/resources/options_light.png) **LaTeX2AI options**: Open a form where the global LaTeX2AI options can be set.
- ![Save document as PDF](/l2a/resources/save_as_pdf_light.png) **Save as PDF**: Save the current `.ai` document as a `.pdf` document with the same name. The LaTeX2AI items are included into the created `.pdf` document.