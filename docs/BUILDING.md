# Building BetterFoil

## Prerequisites
- **devkitPro** (devkitA64 + libnx), installed at `C:\devkitpro` on Windows.
- Switch portlibs (installed via `dkp-pacman`/`pacman` in the devkitPro MSYS2):
  ```
  switch-pkg-config switch-curl switch-mbedtls
  switch-sdl2 switch-sdl2_mixer switch-sdl2_gfx switch-sdl2_image switch-sdl2_ttf
  switch-opusfile switch-libopus switch-libmodplug switch-mpg123 switch-libvorbisidec switch-libogg
  switch-libwebp switch-libpng switch-libjpeg-turbo switch-freetype
  switch-zlib switch-bzip2 switch-libzstd switch-glm
  ```
  (`switch-zlib` provides `libminizip.a`; the zstd package is `switch-libzstd`.)

## ⚠️ The path must NOT contain spaces
The devkitPro `make` rules break on spaces in the path (e.g. a `-f <dir>/Makefile` with a
space splits into bad targets). The Windows user profile `C:\Users\Luan Bogo\...` contains a
space, and **directory junctions do not help** — `make` resolves the junction back to the real
spaced path. Therefore the project lives at a space-free path:

**`C:\dev\BetterFoil`** (MSYS2: `/c/dev/BetterFoil`)

## Build
From the devkitPro MSYS2 environment, in a space-free path:
```sh
cd /c/dev/BetterFoil
make -j4
```
Or, driven from any shell on Windows:
```sh
"C:/devkitpro/msys2/usr/bin/bash.exe" -lc 'cd /c/dev/BetterFoil && make -j4'
```
Output: **`BetterFoil.nro`** (~12 MB) in the project root.

## Install for testing
Copy `BetterFoil.nro` to the SD card under `sdmc:/switch/` and launch it from the hbmenu
(Homebrew Menu) on the CFW console.

## Clean
```sh
make clean
```
