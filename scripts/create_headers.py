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
Create the header containing constants for c++ and JS.
"""

# Import python modules.
import sys
import subprocess
import os
import shutil
from check_license import get_license_text, license_to_source, get_repository_dir


def get_git_sha(repo=None):
    """
    Return the git sha of the repository.
    """

    # Get the git sha of the repository.
    process = subprocess.Popen(
        ["git", "rev-parse", "HEAD"], stdout=subprocess.PIPE, cwd=repo
    )
    out, _err = process.communicate()
    return out.decode("UTF-8").strip()


def create_cpp_version_headers(dir_path, license_c):
    """
    Create the header containing the version definitions.
    """

    version_lines = [
        "\n",
        "// Automatic generated header with version information.",
        "#ifndef VERSION_H_",
        "#define VERSION_H_",
        '#define L2A_VERSION_GIT_SHA_HEAD_ "{}"'.format(get_git_sha()),
        "#endif",
        "",
    ]

    # The script is called form the base repository directory.
    with open(os.path.join(dir_path, "version.h"), "w") as version_header:
        version_header.write(license_c + "\n".join(version_lines))


def create_cpp_tex_headers(dir_path, license_c):
    """
    Create the header containing the TeX definitions.
    """

    def load_tex_code(file_name):
        """Load the TeX code from the templates."""
        tex_path = os.path.join("..", "tex", file_name)
        with open(tex_path) as tex_file:
            return (tex_file.read().strip()).replace("\\", "\\\\").replace('"', '\\"')

    # Get the LaTeX codes.
    header_code = load_tex_code("LaTeX2AI_header.tex")
    item_code = load_tex_code("LaTeX2AI_item.tex")

    tex_lines = [
        "\n",
        "// Automatic generated header with TeX code.",
        "#ifndef TEX_H_",
        "#define TEX_H_",
        "",
        "#define L2A_LATEX_HEADER_ \\",
        '  "' + '\\n"\\\n  "'.join(header_code.split("\n")) + '"',
        "",
        "#define L2A_LATEX_ITEM_ \\",
        '  "' + '\\n"\\\n  "'.join(item_code.split("\n")) + '"',
        "#endif",
        "",
    ]

    # The script is called form the base repository directory.
    with open(os.path.join(dir_path, "tex.h"), "w") as tex_header:
        tex_header.write(license_c + "\n".join(tex_lines))


def create_cpp_headers():
    """
    Create the C++ headers.
    """

    # Get the license text.
    license_text = get_license_text()
    license_c = license_to_source(license_text, "c")

    # Clean the directory for the automatic generated headers.
    dir_path = os.path.join(get_repository_dir(), "src/auto_generated")
    if os.path.exists(dir_path):
        shutil.rmtree(dir_path)
    os.makedirs(dir_path)

    create_cpp_version_headers(dir_path, license_c)
    create_cpp_tex_headers(dir_path, license_c)


def create_js_headers():
    """
    Create the headers for the java script files.
    """

    # Create the header.
    license_text = get_license_text()
    license_c = license_to_source(license_text, "js")
    version_lines = [
        "",
        "",
        "// Automatic generated header with version information.",
        "// This header will be overwritten at each build!",
        "function get_git_sha() {",
        f'    return "{get_git_sha()}"',
        "}",
        "",
    ]

    # If it does not exist, create the directory for the header.
    dir_path = os.path.join(get_repository_dir(), "ui/js")
    os.makedirs(dir_path, exist_ok=True)

    # The script is caled form the base repository directory.
    with open(os.path.join(dir_path, "auto_generated.js"), "w") as version_header:
        version_header.write(license_c + "\n".join(version_lines))


if __name__ == "__main__":
    """Execution part of script"""

    if len(sys.argv) != 1:
        raise ValueError("Wrong number of system arguments.")

    # Change working directory to script directory.
    os.chdir(os.path.dirname(__file__))

    create_cpp_headers()
    create_js_headers()
