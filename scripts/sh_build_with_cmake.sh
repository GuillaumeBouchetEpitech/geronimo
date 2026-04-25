#!/bin/bash


INITIAL_CWD="$PWD"

#
#

echo ""
echo "#"
echo "# WEB_WASM BUILD"
echo "#"
echo ""

cd "$INITIAL_CWD" || exit 1

emcmake cmake -B "./cmake-build.release.wasm"
cd "./cmake-build.release.wasm" || exit 1
cmake --build . --config Release --parallel 5

#
#

echo ""
echo "#"
echo "# NATIVE BUILD"
echo "#"
echo ""

cd "$INITIAL_CWD" || exit 1

# cmake -B "./cmake-build.release.native" -D'CHECK_CLANG_TIDY:BOOL=TRUE'
cmake -B "./cmake-build.release.native" -D'CHECK_CLANG_TIDY:BOOL=FALSE'
cd "./cmake-build.release.native" || exit 1
cmake --build . --config Release --parallel 5

#
#

echo ""
echo "#"
echo "# TEST-BED (WASM)"
echo "#"
echo ""

cd "$INITIAL_CWD/samples/test-bed" || exit 1

emcmake cmake -B "./cmake-build.release.wasm"
cd "./cmake-build.release.wasm" || exit 1
cmake --build . --config Release --parallel 5

#
#

echo ""
echo "#"
echo "# TEST-BED (NATIVE)"
echo "#"
echo ""

cd "$INITIAL_CWD/samples/test-bed" || exit 1

cmake -B "./cmake-build.release.native"
cd "./cmake-build.release.native" || exit 1
cmake --build . --config Release --parallel 5

#
#

echo ""
echo "#"
echo "# PRINT"
echo "#"
echo ""

cd "$INITIAL_CWD" || exit 1

tree -hD ./lib/wasm
tree -hD ./lib/native
tree -hD ./samples/test-bed/bin
tree -hD ./samples/test-bed/dist

echo ""
echo "#"
echo "# DONE!"
echo "#"
echo ""

