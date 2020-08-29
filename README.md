# LaTeX2AI
LaTeX2AI is a plugin for Adobe Illustrator CS6 that enables the use of LaTeX items in an Illustrator document.

## Requirements
A LaTeX compiler must be installed on the system such as [TeX Live](https://www.tug.org/texlive) or [MiKTeX](https://miktex.org) as well as [ghostscript](https://www.ghostscript.com).

## Build from source

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