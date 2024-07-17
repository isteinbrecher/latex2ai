# -*- coding: utf-8 -*-
# -----------------------------------------------------------------------------
# MIT License
#
# Copyright (c) 2020-2024 Ivo Steinbrecher
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
Create a release of LaTeX2AI. This requires that the compiled Windows binary are put in the
release_WIN directory
"""

import os
import sys
import subprocess
import glob
import re

from build import get_git_tag_or_hash, clean_repository

if __name__ == "__main__":
    # Execution part of script

    repo_dir = os.path.dirname(os.path.dirname(os.path.realpath(__file__)))

    # Get the tag or git hash
    git_sha, git_identifier = get_git_tag_or_hash(repo_dir)

    # Check if this is a clean repository
    is_clean_repo = clean_repository(repo_dir)
    if not is_clean_repo:
        if input("Repository is not clean. Do You Want To Continue? ").lower() != "y":
            sys.exit(1)

    # Check if the Windows binaries exist
    directory = os.path.join(repo_dir, "scripts/release_files/WIN")
    pattern = "LaTeX2AI*.aip"
    full_pattern = os.path.join(directory, pattern)
    windows_release_files = glob.glob(full_pattern)
    if not len(windows_release_files) == 1:
        print(
            f"Expected exactly 1 matching Windows binary, found {len(windows_release_files)}."
        )
        sys.exit(1)
    windows_release_file = windows_release_files[0]

    # Get the tag or git hash from the windows binary
    match = re.search(r"LaTeX2AI_(.*?)\.aip", windows_release_file)
    if match:
        windows_git_identifier = match.group(1)
    else:
        print("Could not extract the windows git identifier")
        sys.exit(1)

    # Check if the windows identifier matches with the current one
    if not git_identifier == windows_git_identifier:
        print(
            f'The git identifier for Windows "{git_identifier}" does not match the current repository "{windows_git_identifier}"'
        )
        sys.exit(1)

    # Sign the UI folder
    subprocess.call(
        [os.path.join(repo_dir, "scripts/ui_signing/sign_ui_folder.sh")],
        # stdout=open(os.devnull, "wb"),
    )
    print("Signed the UI folder")

    # Build the release version on macOS
    subprocess.call(
        [os.path.join(repo_dir, "scripts/compile_mac.sh")],
        # stdout=open(os.devnull, "wb"),
    )
    print("Build the release version on macOS")

    # Create the combined release zip
    directory = os.path.join(repo_dir, "scripts/release_files/")
    final_zip_name = f"../LaTeX2AI_{git_identifier}.zip"
    subprocess.call(
        ["zip", "-r", final_zip_name, "."],
        cwd=directory,
    )
    print("Created the final zip file")
