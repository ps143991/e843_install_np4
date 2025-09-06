#!/bin/bash

echo "Cleaning build install"
rm -rf build install
echo "Preparing build files"
cmake -B build -DCMAKE_INSTALL_PREFIX=./install
echo "Installing"
make -C build install -j15

