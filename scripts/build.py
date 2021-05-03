# -*- coding: utf-8 -*-
# -----------------------------------------------------------------------------
# MIT License
#
# Copyright (c) 2020 Ivo Steinbrecher
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
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
Automatically build LaTeX2AI and create a arcive with the executables.
"""

# Import python modules.
import sys
import os
import shutil
import subprocess
from zipfile import ZipFile
from create_headers import get_git_sha


def get_git_tag_or_hash():
    """
    Return the commit, if the commit has a tag, return the tag.
    """

    # Get current sha.
    git_sha = get_git_sha()

    # Get all tags and their sha.
    process = subprocess.Popen(['git', 'show-ref', '--tags'],
        stdout=subprocess.PIPE)
    out, _err = process.communicate()
    tags = out.decode('UTF-8').strip().split('\n')
    tags = [line.split(' ') for line in tags]
    for sha, tag in tags:
        if sha == git_sha:
            # This commit has a tag, return the tag.
            return tag.split('/')[-1]
    return git_sha[:7]
   

if __name__ == '__main__':
    """Execution part of script"""
    
    # Get base directory.
    file_path = os.path.realpath(os.path.join(os.getcwd(), __file__))
    script_dir = os.path.dirname(file_path)
    base_dir = os.path.dirname(script_dir)
    
    # Remove output directory.
    output_dir = os.path.join(base_dir, '..\output')
    try:
        shutil.rmtree(output_dir)
    except OSError as e:
        print("Error: %s - %s." % (e.filename, e.strerror))
        sys.exit(1)

    # Get git information.
    git_info = get_git_tag_or_hash()

    # Build LaTeX2AI.
    build_types = [
        'Release',
        'Debug'
        ]
    for build_type in build_types:
        os.chdir(script_dir)
        os.environ['LaTeX2AI_build_type'] = build_type
        subprocess.call(['compile_solution.bat'])

        # Get information about the build.
        split_path = os.path.normpath(base_dir).split(os.sep)
        illustrator_version = split_path[-3]
        version_dir = {
            'Adobe Illustrator CS6 SDK': 'IllustratorCS6',
            'Adobe Illustrator CC 2018 SDK': 'IllustratorCC2018',
            'Adobe Illustrator 2021 SDK': 'Illustrator2021'
            }

        # Compress the executables.
        executable_dir = os.path.realpath(
            os.path.join(output_dir, 'win', 'x64', build_type))
        os.chdir(executable_dir)
        executables = [
            'LaTeX2AI.aip',
            'LaTeX2AIForms.exe'
            ]
        zip_name = ('LaTeX2AI_' + git_info + '_' + build_type.lower() + '_'
            + version_dir[illustrator_version] + '.zip')
        with ZipFile(zip_name, mode='w') as zf:
            for f in executables:
                zf.write(f)
