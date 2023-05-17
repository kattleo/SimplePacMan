#!/bin/bash
echo -n "lines of code: "
find . \( -name \*.h -o -name \*.cpp \) -exec cat {} \; | wc -l
