#! /bin/bash
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

# Get the path of the current script
SCRIPT_DIR="$(dirname "$0")"

# Remove an existing version
BUILD_PATH=${SCRIPT_DIR}/../../output/mac/${L2A_BUILD_TYPE}/LaTeX2AI.aip
rm -rf ${BUILD_PATH}

# Build the version of LaTeX2AI
xcodebuild -project ${SCRIPT_DIR}/../LaTeX2AI.xcodeproj -scheme "LaTeX2AI" -configuration "${L2A_BUILD_TYPE}" -destination "generic/platform=macOS" clean build

# Move to the correct folder
FINAL_PATH_DIR=${SCRIPT_DIR}/release_files/macOS
mkdir -p ${FINAL_PATH_DIR}
FINAL_PATH=${FINAL_PATH_DIR}/LaTeX2AI_${L2A_GIT_IDENTIFIER}_${L2A_BUILD_TYPE}.aip
rm -rf ${FINAL_PATH}
mv ${BUILD_PATH} ${FINAL_PATH}
