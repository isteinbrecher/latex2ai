# -*- coding: utf-8 -*-
# -----------------------------------------------------------------------------
# MIT License
#
# Copyright (c) 2020 Ivo Steinbrecher (https://github.com/stoani89/LaTeX2AI)
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
Create the header containing version information of the plugin.
"""

# Import python modules.
import subprocess
import os
from check_license import get_license_text, license_to_source

# Get the git sha of the repository.
process = subprocess.Popen(['git', 'rev-parse', 'HEAD'], stdout=subprocess.PIPE)
out, _err = process.communicate()
git_sha = out.decode('UTF-8').strip()

# Create the headers.
license_text = get_license_text()
license_c = license_to_source(license_text, 'c')
license_tex = license_to_source(license_text, 'tex')

version_lines = [
    '\n',
    '// Automatic generated header with version information.',
    '#ifndef VERSION_H_',
    '#define VERSION_H_',
    '#define L2A_VERSION_GIT_SHA_HEAD_ "{}"'.format(git_sha),
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
dir_path = 'src/auto_generated'
os.makedirs(dir_path, exist_ok=True)

# The script is caled form the base repository directory.
with open(os.path.join(dir_path, 'version.h'), 'w') as version_header:
    version_header.write(license_c + '\n'.join(version_lines))
with open(os.path.join(dir_path, 'license.h'), 'w') as license_header:
    license_header.write(license_c + '\n'.join(license_lines))
