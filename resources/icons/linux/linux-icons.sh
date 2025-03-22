#!/usr/bin/env bash
# $1: Input path to an image to use for generating icons.
# Helper script to generate qtk's icons for Linux given a path to an input PNG.
sudo apt install imagemagick

convert "$1" -resize 256x256 kilroy-256.png
convert "$1" -resize 128x128 kilroy-128.png
convert "$1" -resize 64x64 kilroy-64.png
convert "$1" -resize 48x48 kilroy-48.png
convert "$1" -resize 32x32 kilroy-32.png
convert "$1" -resize 16x16 kilroy-16.png
