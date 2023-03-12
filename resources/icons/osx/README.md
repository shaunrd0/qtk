
```bash
sudo apt install icnsutils
```

```bash
convert icon.png -resize 32x32 kilroy_32.png
convert icon.png -resize 16x16 kilroy_16.png
convert icon.png -resize 48x48 kilroy_48.png
convert icon.png -resize 128x128 kilroy_128.png
convert icon.png -resize 256x256 kilroy_256.png
png2icns png2icns kilroy.icns kilroy_*.png
```
