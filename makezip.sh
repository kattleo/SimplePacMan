#!/bin/bash

version=`grep "ASCII GFX v" README.txt | sed -e 's/.*v\([0-9\.]*\).*/\1/'`
echo creating zip archive of ASCII GFX $version
mkdir ASCII-GFX-$version
cp -rL README* COPYING main.cpp gfx examples screens CMakeLists.txt CMakeModules ASCII-GFX.cmake Doxyfile *.wav *.sh ASCII-GFX-$version/
zip -r ASCII-GFX-$version.zip ASCII-GFX-$version -x \*/.svn/\*
rm -rf ASCII-GFX-$version
