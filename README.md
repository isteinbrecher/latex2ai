# LaTeX2AI
LaTeX2AI is a plugin for Adobe Illustrator CS6 that enables the use of text labels typeset in LaTeX inside an Illustrator document.

## Requirements
Currently LaTeX2AI only works on Adobe Illustrator for Windows.
The following software has to be installed on the system:
- A LaTeX compiler such as [TeX Live](https://www.tug.org/texlive) or [MiKTeX](https://miktex.org)
- [Ghost script](https://www.ghostscript.com)

## Install LaTeX2AI
Install LaTeX2AI from rebuild binaries can be downloaded from the [GitHub release page](https://github.com/stoani89/LaTeX2AI/releases).
They consist of two files `LaTeX2AI.aip` and `LaTeX2AIForms.exe`.
Both files have to be copied into the same directory and this directory has to be set as the Adobe Illustrator Plugin directory.

## Build LaTeX2AI from source

### Requirements
To build LaTeX2AI from source additional requirements have to be met:
- A C++ compiler has to be available on the system. This repository includes a Visual Studio 2017 solution for Windows.
- `python3` and `git` have to be available on the system.
- An environment variable `PYTHON_EXE` has to be defined which points to a valid `python3` executable on the system.

### Getting started
1. Download and unpack the [Adobe Illustrator CS6 SDK](http://download.macromedia.com/pub/developer/illustrator/sdk/AI_CS6_SDK_Win_682.6.1.zip).
1. Go to the sample code folder `Adobe Illustrator CS6 SDK/sample code` and run
	```
	git clone https://github.com/stoani89/LaTeX2AI.git
	```
1. Go to `Adobe Illustrator CS6 SDK/sample code/LaTeX2AI` and run
	```
	git sub module update --init
	```
1. Open `Adobe Illustrator CS6 SDK/sample code/LaTeX2AI/LaTeX2AI.sln` with Visual Studio and compile the solution.


# How to use LaTeX2AI
LaTeX2AI uses the *Placed Art* feature of Illustrator to embed LaTeX labels (compiled to `.pdf`) inside an Illustrator document.

The plugin adds four buttons to the main toolbar:

- ![Create / Edit](/l2a/resources/create_light.png?raw=true "Create / Edit") **Create / Edit**: Either create a new LaTeX2AI label or edit an existing one in the document.
- ![Redo items](/l2a/resources/redo_light.png?raw=true "Redo labels") **Redo LaTeX2AI labels**: This allows for the LaTeX recompilation and / or boundary box reset of all existing LaTeX2AI labels.
- ![LaTeX2AI options](/l2a/resources/options_light.png?raw=true "LaTeX2AI options") **LaTeX2AI options**: Open a form where the global LaTeX2AI options can be set.
- ![Save document as PDF](/l2a/resources/save_as_pdf_light.png?raw=true "Save document as PDF") **Save as PDF**: Save the current `.ai` document as a `.pdf` document with the same name. The LaTeX2AI labels are included into the created `.pdf` document.

Additionally, LaTeX2AI labels can be edited by double clicking on them.

## Create / Edit a LaTeX2AI label
LaTeX2AI labels are created and or edited with the following form:

![Create / Edit](/doc/create-edit.png?raw=true "The LaTeX2aI Create / Edit form")

### Placement
This option defines how a label behaves when it is changed and its size changes.
Take for example the well-known formula `$\sum_{k=0}^{\nifty}\frac{x^k}{k!}$` which is placed inside a rectangle:

![Placement small](/doc/placement_small.png?raw=true)

The green box indicates the boundary box of the LaTeX2I label and the dot describes the placement of the label, i.e. if the size changes the position of this dot relative to the label will stay the same.
If the previous label is now changed to `\displaystyle` we get the following result:

![Placement large](/doc/placement_large.png?raw=true)

The size of the label changed due to a change in the underlying LaTeX code, but the position (the indicated dot) stayed the same.

#### Baseline placement
Additionally there is the possibility of a *baseline* placement.
This will result in a label where the baseline is exactly in the vertical center of the label.
The resulting label can now be easily adjusted and snapped to align with another baseline label or Illustrator text:

![Placement baseline](/doc/placement_baseline.png?raw=true)

### Stretch behaviour
This option controls how a LaTeX2AI text label behaves when the size of the boundary box is changed in Illustrator:

- **stretch** The label will always be stretched to the boundary box dimensions.
- **remain original size** Only the boundary box is stretched the label will remain in the correct size.

The following Figure illustrates the different options:

![Stretched labels](/doc/stretched_labels.png?raw=true)

Please note that the **remain original size** option leads to issues if the item is bend (see the red boxes in the previous Figure).

Every time a label is changed the boundary box is reset so that it matches the size of the actual places PDF file.
With the **Redo items** button the boundary box of all labels can be redone.
It is generally advisable to redo all boundary boxes before exporting the Illustrator document, to ensure that the font size will be correct for each label.

## File structure
LaTeX2AI requires a certain file structure.
It is assumed that all Illustrator files in the same directory use the same LaTeX header `LaTeX2AI_header.tex` (if no one exits in the directory, it will be created the first time it is needed).
This header can be edited to include packages and macros needed for the labels (LaTeX2AI supports `\input` commands in the header, also recursively).
A sub directory of that directory will be created with the name `LaTeX2AI` which stores the `.pdf` files for the labels of all Illustrator documents in that directory.


# Changelog
- **Pre-release**
  - New features:
    - Allow for `\input` commands in LaTeX headers.
    - Add option to edit LaTeX header in the settings form.
  - Bug fixes:
    - Replace hardcoded path to python executable with environment variable `PYTHON_EXE`
- **v0.0.1:** Initial release
