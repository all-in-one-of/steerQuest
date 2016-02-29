#!/bin/bash

git clone https://github.com/google/protobuf
git checkout tags/v2.5.0
./configure --prefix=/usr
make
make install
