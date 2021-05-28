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
Create the header containing version information of the plugin.
"""

# Import python modules.
import sys
import subprocess
import os
from check_license import (get_license_text, license_to_source,
    get_repository_dir)


def get_git_sha():
    """
    Return the git sha of the repository.
    """

    # Get the git sha of the repository.
    process = subprocess.Popen(['git', 'rev-parse', 'HEAD'],
        stdout=subprocess.PIPE)
    out, _err = process.communicate()
    return out.decode('UTF-8').strip()


def create_cpp_headers():
    """
    Create the headers for the c++ files.
    """

    # Create the headers.
    license_text = get_license_text()
    license_c = license_to_source(license_text, 'c')
    license_tex = license_to_source(license_text, 'tex')

    version_lines = [
        '\n',
        '// Automatic generated header with version information.',
        '#ifndef VERSION_H_',
        '#define VERSION_H_',
        '#define L2A_VERSION_GIT_SHA_HEAD_ "{}"'.format(get_git_sha()),
        '#endif',
        ''
        ]

    license_lines = [
        '\n',
        '// Automatic generated header with license information.',
        '#ifndef LICENSE_H_',
        '#define LICENSE_H_',
        'namespace L2A',
        '{',
        '  namespace LICENSE',
        '  {',
        '    static const char* tex_license_ ='
        ]
    for line in license_tex.split('\n'):
        license_lines.append('      "{}\\n"'.format(line.replace('"', '\\"')))
    license_lines[-1] = license_lines[-1] + ';'
    license_lines.extend([
        '  }',
        '}',
        '#endif',
        ''])

    # If it does not exist, create the directory for the header.
    dir_path = os.path.join(get_repository_dir(), 'l2a/src/auto_generated')
    os.makedirs(dir_path, exist_ok=True)

    # The script is caled form the base repository directory.
    with open(os.path.join(dir_path, 'version.h'), 'w') as version_header:
        version_header.write(license_c + '\n'.join(version_lines))
    with open(os.path.join(dir_path, 'license.h'), 'w') as license_header:
        license_header.write(license_c + '\n'.join(license_lines))


def create_cs_headers():
    """
    Create the headers for the c# files.
    """

    # Create the header.
    license_text = get_license_text()
    license_c = license_to_source(license_text, 'c')
    version_lines = [
        '\n',
        '// Automatic generated header with version information.',
        '// This header will be overwritten at each build!',
        'namespace L2A',
        '{',
        '    class Constants',
        '    {',
        '        public const string l2a_version_git_sha_head_ = "{}";'.format(
            get_git_sha()),
        '    }',
        '}',
        ''
        ]

    # If it does not exist, create the directory for the header.
    dir_path = os.path.join(get_repository_dir(),
        'forms/src/auto_generated')
    os.makedirs(dir_path, exist_ok=True)

    # The script is caled form the base repository directory.
    with open(os.path.join(dir_path, 'version.cs'), 'w') as version_header:
        version_header.write(license_c + '\n'.join(version_lines))


if __name__ == '__main__':
    """Execution part of script"""

    if len(sys.argv) != 2:
        raise ValueError('Wrong number of system arguments.')

    # Change working directory to script directory.
    os.chdir(os.path.dirname(__file__))

    if sys.argv[1] == 'cpp':
        create_cpp_headers()
    elif sys.argv[1] == 'cs':
        create_cs_headers()
    else:
        raise ValueError('Got unexpected comandline argument.')
