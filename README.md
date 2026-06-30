# BetterFoil

A NSP, NSZ, XCI, and XCZ installer for jailbroken (CFW / Atmosphère) Nintendo Switch
consoles — a fork of [Awoo Installer](https://github.com/Huntereb/Awoo-Installer)
(itself derived from [Adubbz Tinfoil](https://github.com/Adubbz/Tinfoil)) aimed at a
better UI/UX, a **stay-awake** install mode, and an optional Windows host server with
metadata/cover-art display.

> **Status:** early development (`0.1.0-dev`). This is a fork being rebranded and extended.
> See `docs/superpowers/specs/` for the design and the project plan.

## Goals (what makes it "Better")
- Cleaner, faster UI/UX on top of the existing Plutonium interface
- **Stay-awake mode**: keep installing with the screen off; any input wakes the screen
- Open-source and auditable (GPLv3)
- *(later)* A **Windows host server** that scans a games folder, reads metadata
  (name, size, cover) using your own `prod.keys`, and serves them over the network
- *(later)* Richer install of base game **+ updates + DLC**

## Inherited features (from Awoo / Tinfoil)
- Installs NSP/NSZ/XCI/XCZ and split files from the SD card
- Installs over LAN or USB (e.g. with [NS-USBloader](https://github.com/developersu/ns-usbloader))
- Installs over the internet by URL or Google Drive
- Verifies NCAs by header signature before installing
- Manages signature patches

## Keys
BetterFoil **does not** dump or bundle any console keys. Decryption-dependent features
require **your own legally obtained `prod.keys`**, supplied by you — same model as
`nsz`/`hactool`. You must legally own any content you install.

## Building
Requires **devkitPro** (devkitA64 + libnx) with the Switch portlibs. With the toolchain
installed and `DEVKITPRO` set, run `make` in this directory to produce `BetterFoil.nro`.

## License
GPLv3. This is a derivative work — see `LICENSE` and `Tinfoil.LICENSE`, which are preserved
from the upstream projects. The original Awoo/Tinfoil/Plutonium copyright notices remain in
place as required by the GPL.

## Thanks to
- **Huntereb & HookedBehemoth** for [Awoo Installer](https://github.com/Huntereb/Awoo-Installer), the base of this fork
- **Adubbz** and contributors for [Tinfoil](https://github.com/Adubbz/Tinfoil)
- **XorTroll** for [Plutonium](https://github.com/XorTroll/Plutonium) and [Goldleaf](https://github.com/XorTroll/Goldleaf)
- **blawar** and **nicoboss** for [NSZ](https://github.com/nicoboss/nsz) support
