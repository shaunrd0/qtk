#!/usr/bin/env bash
# $1: Input path to an image to use for generating icons.
# Helper script to generate qtk's icons for OSX given a path to an input PNG.
sudo apt install icnsutils imagemagick

convert "$1" -resize 32x32 kilroy_32.png
convert "$1" -resize 16x16 kilroy_16.png
convert "$1" -resize 48x48 kilroy_48.png
convert "$1" -resize 128x128 kilroy_128.png
convert "$1" -resize 256x256 kilroy_256.png
# For the application bundle in OSX.
png2icns png2icns kilroy.icns kilroy_*.png

# For the splash image in DMG installers.
convert "$1" -resize 800x600! kilroy_splash.png
