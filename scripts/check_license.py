# -*- coding: utf-8 -*-
# -----------------------------------------------------------------------------
# MIT License
#
# Copyright (c) 2020-2025 Ivo Steinbrecher
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
# -----------------------------------------------------------------------------

"""
Check if the source files in the repository have the correct header.
"""

# Import python modules.
import os
import subprocess


def get_repository_dir():
    """
    Get the root directory of this repository.
    """

    script_path = os.path.realpath(__file__)
    root_dir = os.path.dirname(os.path.dirname(script_path))
    return root_dir


def get_license_text():
    """
    Return the license text as a string.
    """

    license_path = os.path.join(get_repository_dir(), "LICENSE")
    with open(license_path) as license_file:
        return license_file.read().strip()


def get_all_source_files():
    """
    Get all source files that should be checked for license headers.
    """

    # Get the files in the git repository.
    repo_dir = get_repository_dir()
    process = subprocess.Popen(
        ["git", "ls-files"], stdout=subprocess.PIPE, cwd=repo_dir
    )
    out, _err = process.communicate()
    files = out.decode("UTF-8").strip().split("\n")

    source_line_endings = [".py", ".cs", ".cpp", ".h", ".bat", ".sh", ".tex"]
    source_ending_types = {
        ".py": "py",
        ".cs": "c",
        ".cpp": "c",
        ".h": "c",
        ".bat": "bat",
        ".sh": "sh",
        ".tex": "tex",
    }
    source_files = {"py": [], "c": [], "bat": [], "sh": [], "tex": []}

    skip_folders = ["scripts/pipl"]

    for file in files:
        extension = os.path.splitext(file)[1]
        for skip_folder in skip_folders:
            if file.startswith(skip_folder):
                break
        else:
            if extension not in source_line_endings:
                pass
            else:
                source_files[source_ending_types[extension]].append(
                    os.path.join(repo_dir, file)
                )
    return source_files


def license_to_source(license_text, source_type):
    """
    Convert the license text to a text that can be written to source code.
    """

    header = None
    start_line = "-" * 77
    if source_type == "py":
        header = "# -*- coding: utf-8 -*-"
        comment = "#"
    elif source_type == "c" or source_type == "js":
        comment = "//"
    elif source_type == "bat":
        comment = "@REM"
    elif source_type == "sh":
        header = "#! /bin/bash"
        comment = "#"
    elif source_type == "tex":
        comment = "%"
    else:
        raise ValueError("Wrong extension!")

    source = []
    if header is not None:
        source.append(header)
    source.append(comment + " " + start_line)
    for line in license_text.split("\n"):
        if len(line) > 0:
            source.append(comment + " " + line)
        else:
            source.append(comment + line)
    source.append(comment + " " + start_line)
    return "\n".join(source)


def check_license():
    """
    Check the license for all source files.
    """

    license_text = get_license_text()
    source_files = get_all_source_files()

    skip_list = ["resources/Win/Resource.h"]

    for key in source_files:
        header = license_to_source(license_text, key)
        for file in source_files[key]:
            for skip in skip_list:
                if file.endswith(skip):
                    break
            else:
                with open(file) as source_file:
                    source_text = source_file.read()
                    if not source_text.startswith(header):
                        print("Wrong header in: {}".format(file))


if __name__ == "__main__":
    """
    Execution part of script.
    """

    check_license()
