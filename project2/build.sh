#!/bin/bash

mkdir build
cd build
cmake ..
cmake --build . --config Release --target all -- -j 4