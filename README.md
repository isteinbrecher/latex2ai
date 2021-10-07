# LaTeX2AI
LaTeX2AI is a plugin for Adobe Illustrator that enables the use of text labels typeset in LaTeX inside an Illustrator document.
Currently LaTeX2AI supports:
- Illustrator CS6 (tested)
- Illustrator CC 2017
- Illustrator CC 2018
- Illustrator 2021 (tested)

## Requirements
Currently LaTeX2AI only works on Adobe Illustrator for Windows.
The following software dependencies are required:
- A LaTeX compiler such as [TeX Live](https://www.tug.org/texlive) or [MiKTeX](https://miktex.org)
- [Ghost script 64bit](https://www.ghostscript.com)

## Install LaTeX2AI
Install LaTeX2AI from rebuild binaries can be downloaded from the [GitHub release page](https://github.com/stoani89/LaTeX2AI/releases).
They consist of two files `LaTeX2AI.aip` and `LaTeX2AIForms.exe`.
There are two ways to add LaTeX2AI to Illustrator:
- Both files can be copied to the default plug-in directory `C:\Program Files\Adobe\Adobe Illustrator <YOUR VERSION>\Plug-ins`, preferably into a subdirectory `LaTeX2AI` (for this method administrator privileges are required).
- Both files can be copied into an arbitrary directory and this directory has to be set as the Adobe Illustrator Plugin directory.

## Build LaTeX2AI from source

### Requirements
The additional requirements to build LaTeX2AI from source are:
- A C++ compiler has to be available on the system. This repository includes a Visual Studio 2017 solution for Windows.
- `python3` and `git` have to be available on the system.
- Environment variables:
  - `PYTHON_EXE` has to points to a valid `python3` executable on the system.
  - `ILLUSTRATOR_EXE` has to point to the illustrator executable on the system (this path is used for debugging).

### Getting started
1. Download and unpack the SDK depending on your Illustrator version:
  - [Adobe Illustrator CS6 SDK](http://download.macromedia.com/pub/developer/illustrator/sdk/AI_CS6_SDK_Win_682.6.1.zip)
  - [Adobe Illustrator CC and newer](https://console.adobe.io/downloads)
1. Go to the sample code folder `Adobe Illustrator <YOUR VERSION> SDK/sample code` and run
	```
	git clone https://github.com/stoani89/LaTeX2AI.git
	```
1. Go to `Adobe Illustrator <YOUR VERSION> SDK/sample code/LaTeX2AI` and run
	```
	git submodule update --init
	```
1. Open `Adobe Illustrator <YOUR VERSION> SDK/sample code/LaTeX2AI/LaTeX2AI.sln` with Visual Studio, select the build type (*Release* or *Debug*) and the target (`x64`) and compile the solution.
   It might be necessary to change the *Windows SDK* version for `LaTeX2AI` and the *.NET Framework* version for `LaTeX2AIForms` to match the ones available on your system.
1. The compiled binaries are located at `Adobe Illustrator <YOUR VERSION> SDK/sample code/output/win/x64/<Debug/Release>`.
   This directory has to be set as the Plugin directory in Illustrator.
   The relevant files created, i.e. the ones needed to run LaTeX2AI are `LaTeX2AI.aip` and `LaTeX2AIForms.exe`.
   The executable `LaTeX2AIForms.exe` contains the UI for LaTeX2AI and should only be called from within the plugin itself.


# How to use LaTeX2AI
LaTeX2AI uses the *Placed Art* feature of Illustrator to embed LaTeX labels (compiled to `.pdf`) inside an Illustrator document.

The plugin adds four buttons to the main toolbar:

- ![Create / Edit](/l2a/resources/create_light.png?raw=true "Create / Edit") **Create / Edit**: Either create a new LaTeX2AI label or edit an existing one in the document.
- ![Redo items](/l2a/resources/redo_light.png?raw=true "Redo labels") **Redo LaTeX2AI labels**: This allows for the LaTeX recompilation and / or boundary box reset of all existing LaTeX2AI labels.
- ![LaTeX2AI options](/l2a/resources/options_light.png?raw=true "LaTeX2AI options") **LaTeX2AI options**: Open a form where the global LaTeX2AI options can be set. Also the LaTeX header can be opened in an external application.
- ![Save document as PDF](/l2a/resources/save_as_pdf_light.png?raw=true "Save document as PDF") **Save as PDF**: Save the current `.ai` document as a `.pdf` document with the same name. The LaTeX2AI labels are included into the created `.pdf` document.

Additionally, double clicking on a LaTeX2AI label enabled the editing mode.

## Create / Edit a LaTeX2AI label
The following form appears when creating or editing LaTeX2AI labels:

![Create / Edit](/doc/create-edit.png?raw=true "The LaTeX2aI Create / Edit form")

### Placement
This option defines how a label behaves when it its size changes.
Take for example the well-known formula `$\sum_{k=0}^{\infty}\frac{x^k}{k!}$` which is placed inside a rectangle:

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

### Stretch behavior
This option controls how a LaTeX2AI text label behaves when the size of the boundary box changes in Illustrator:

- **stretch** (default): The label always stretches to the boundary box dimensions.
- **remain original size**: Only the boundary box stretches, the label will remain in the correct size.

The following Figure illustrates the different options:

![Stretched labels](/doc/stretched_labels.png?raw=true)

Please note that the **remain original size** option leads to issues if the item is bend (see the red boxes in the previous Figure).

Every time a label changes the boundary box is reset so that it matches the size of the actual places PDF file.
With the **Redo items** button the boundary box of all labels can be redone.
It is generally advisable to redo all boundary boxes before exporting the Illustrator document, to ensure that the font size will be correct for each label.

## File structure
LaTeX2AI requires a certain file structure.
It is assumed that all Illustrator files in the same directory use the same LaTeX header `LaTeX2AI_header.tex` (if no one exits in the directory, it will be created the first time it is needed).
This header can be edited to include packages and macros needed for the labels (LaTeX2AI supports `\input` commands in the header, also recursively).
A sub directory of that directory will be created with the name `LaTeX2AI` which stores the `.pdf` files for the labels of all Illustrator documents in that directory.


# License & How to cite
LaTeX2AI is under the MIT license, see [./LICENSE](LICENSE).
If you use LaTeX2AI to create figures for your work, please acknowledge it with a link to the GitHub repository.
For example:

- Sketches in this work have been created using the Adobe Illustrator plug-in LaTeX2AI ([https://github.com/stoani89/LaTeX2AI](https://github.com/stoani89/LaTeX2AI)).

## Work that uses LaTeX2AI
1. Steinbrecher, I., Mayr, M., Grill, M.J., Kremheller, J., Meier, C., Popp, A.: A mortar-type finite element approach for embedding 1D beams into 3D solid volumes, Computational Mechanics, 66(6):1377-1398, [Open Access](https://doi.org/10.1007/s00466-020-01907-0)
2. Steinbrecher, I., Humer, A., Vu-Quoc, L.: On the numerical modeling of sliding beams: A comparison of different approaches, Journal of Sound and Vibration, 408:270-290, [Open Access](https://doi.org/10.1016/j.jsv.2017.07.010) (LaTeX2AI not credited)


# Contributing

If you are interested in contributing to LaTeX2AI, we welcome your collaboration.
For general questions, feature request and bug reports please open an [issue](https://github.com/stoani89/LaTeX2AI/issues).

If you contribute actual code, fork the repository and make the changes in a feature branch.
Depending on the topic and amount of changes you also might want to open an [issue](https://github.com/stoani89/LaTeX2AI/issues).
To merge your changes into the main LaTeX2AI repository, create a pull request to the `develop` branch (this branch will be merged into `master` with the next release).
A few things to keep in mind:
- Compile a debug build and run the framework tests in Illustrator ![Framework tests](/l2a/resources/testing_light.png?raw=true "Framework tests").
  You might also consider adding tests for your changes (`./l2a/src/tests`).
- Run the `python3` script `check_license.py` to ensure that all added source files have the correct license header.
- LaTeX2AI uses `clang-format` to format the C++ code.
  Make sure to apply clang format to the changed source files (with the Visual Studio solution in the repository this should happen automatically).
- Add a short description of your changes to the [Changelog](#changelog) in [./README.md](README.md).
- Feel free to add yourself to the [./CONTRIBUTORS](CONTRIBUTORS) file.


# Changelog
- **pre-release**
  - Bug fixes:
    - Fix bug when saving a PDF for an unsaved AI document.
  - Other:
    - Improve output message for Illustrator errors.
- **v0.0.5**
  - Other:
    - Support Illustrator 2017
- **v0.0.4**
  - Bug fixes:
    - Fix error when editing item with missing PDF file.
    - Fix error with included headers containing relative paths.
    - Clean temp directory at startup.
    - Fix bugs when working with locked and or hidden layers / groups.
  - Other:
    - Create automated build script.
    - Support Illustrator CC 2018.
- **v0.0.3**
  - New features:
    - Allow to redo boundary box from the create / edit form.
    - Allow to redo the latex label from the create / edit form.
  - Bug fixes:
    - Fix bug with whitespaces in `system` calls.
  - Other:
    - Add contributing guidelines.
    - Change the default placement options.
    - Rename environment variable `ILLUSTRATOR_PATH` to `ILLUSTRATOR_EXE`.
- **v0.0.2**
  - New features:
    - Allow for `\input` commands in LaTeX headers.
    - Add option to edit LaTeX header in the settings form.
  - Bug fixes:
    - Replace hardcoded path to python executable with environment variable `PYTHON_EXE`.
    - Replace hardcoded path to Illustrator executable with environment variable `ILLUSTRATOR_PATH`.
  - Other:
    - Add information how to cite.
    - Forms and the plugin are now compiled in the same directory; also add more detailed build information.
    - LaTeX2AI can now also be installed in the default Illustrator plug-in directory.
    - Support Illustrator 2021.
- **v0.0.1:** Initial release
