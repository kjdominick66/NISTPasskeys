#!/bin/bash

make clean || exit 1
make || exit 1
make run