#!/usr/bin/env bash
# $1: Input path to an image to use for generating icons.
# Helper script to generate qtk's icons for Windows given a path to an input PNG.
sudo apt install imagemagick

# For the NSIS (un)installer executable shortcut icon.
convert "$1" -resize 256x256,128x128,64x64,48x48,32x32,16x16 -background transparent -colors 256 kilroy.ico

# For the NSIS installer splash screen.
convert "$1" -resize 164x314! -background white -flatten kilroy_splash.bmp
