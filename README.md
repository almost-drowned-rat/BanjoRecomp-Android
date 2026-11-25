<p align="center">
  <img src="assets/icon_256.png" alt="Banjo: Recompiled for Android" width="160">
</p>

# Banjo: Recompiled — Android

The Android version of [Banjo: Recompiled](https://github.com/BanjoRecomp/BanjoRecomp): Banjo-Kazooie, statically
recompiled, running natively on Android with touch buttons, full controller support and RetroArch shaders. Made
for Android handhelds with a built-in controller and for phones. Tested on the **AYN Odin 2 Portal** (Snapdragon 8
Gen 2, 1920×1080, Android 13).

> **This is not a project of the Banjo: Recompiled team**, and it was made with AI assistance (see below), which
> the Banjo: Recompiled team does not use and does not accept in contributions. Please don't report problems of
> this Android version to them: use this repository's [Issues](../../issues).

This repository is a fork of the main project: branch `main` follows upstream, branch `android` (the default) is
the Android port. Its Android layer comes from this account's
[Donkey Kong 64: Recompiled Android port](https://github.com/almost-drowned-rat/Donkey-Kong-64-Recompiled-Android)
(DK64: Recompiled is built the same way as Banjo: Recompiled). The main project's README is kept as
[README-UPSTREAM.md](README-UPSTREAM.md).

> ⚠️ **No game files.** The app contains only this project's code. You need your own legally obtained ROM of
> **Banjo-Kazooie (USA) 1.0** (NTSC-U, not Rev 1), sha1 `1fe1632098865f639e22c11b9a81ee8f29c75d7a`.

## Install

1. Download the `.apk` from the [Releases](../../releases) page and open it on the device (allow installing apps
   from your browser or file manager when Android asks).
2. Start **Banjo: Recompiled** and choose **Load ROM**: Android's file picker opens; pick your ROM. It is checked,
   copied into the app and the game starts.

Updates install over the previous version and keep your saves. To get them automatically, add this repository to
[Obtainium](https://github.com/ImranR98/Obtainium) (`https://github.com/almost-drowned-rat/BanjoRecomp-Android`).

Needs Android 8.0 or newer, a 64-bit (arm64) device and a GPU with Vulkan. App ID
`io.github.almostdrownedrat.banjo` — it installs next to other Banjo apps.

## Controls

- **Controller**: works out of the box (built-in handheld controls or a Bluetooth / USB pad); set it up in the
  game's settings (Controls).
- **Touch buttons** (N64 layout: stick, A, B, Z, L, R, START, C buttons; D-pad hidden by default). **Automatic**
  hides them while a controller is used and a touch brings them back; **Always** and **Off** are the other modes.
- **Back** (the back gesture, the system bar's back button or a handheld's own Back key) opens the menu over the
  game; Back again closes it.

Holding A to jump higher works once Bottles has taught it (the first molehill in Spiral Mountain), as on the N64.

## Menu

One menu for everything — the game's settings and the Android ones — laid out the same in all of these N64 Android
ports: the sections in a column on the left, the settings as cards on the right, grouped under headings and marked
in the game's colour (blue, as its start screen).

- **Controller**: D-pad / stick move, **A** select (on a value **A** takes it, left / right change it, **A** or **B**
  lets go), **B** one level back (from the section column it closes the menu), **Y** back to the default,
  **L / R** the section. The hints are at the bottom right.
- **Touch**: tap a card, swipe to scroll; on a value a tap on its left third steps back, on the rest forward.

The sections:

- **General** — Back to game, Exit game; Note & Jinjo Saving; Background Input Mode
- **Camera** — camera mode and sensitivity; the invert options
- **Graphics** — picture (with the cutscene aspect ratio), quality, frame rate (with the FPS readout) and the GPU
  driver; graphics changes are applied with **Apply** at the bottom. On Snapdragon (Adreno) GPUs Turnip draws the
  whole frame at once, which avoids tile glitches.
- **Shaders**, **Audio**
- **Controls** — Button mapping, deadzone, rumble; the touch buttons (mode, move and resize, size, single buttons,
  opacity, stick sensitivity, reset)
- **Mods** — Manage mods and Find mods (before the game starts): Find mods lists the mods of
  [Thunderstore](https://thunderstore.io/c/banjo-recompiled/) with their pictures and installs them (and what they
  need) with one tap (the app uses the internet only for this). In the game, **Install / switch mods (restart)** restarts the
  app to the start screen, where mods are installed and turned on or off.

There are no window mode options (always full screen).

### Shaders and GPU driver

- **Shaders** — RetroArch shaders (librashader). None are shipped: **Import shader pack (.zip)** takes a slang
  shader pack such as [libretro/slang-shaders](https://github.com/libretro/slang-shaders)' "Download ZIP"; pick a
  preset by folder and change its parameters (kept per preset). **Shader Resolution**: **Original** (the N64's
  240 lines, as in RetroArch — the look the shaders were made for and by far the lightest), **2x** or **Full**.
- **Graphics > Driver > GPU Driver (Turnip)** — pick one of the installed drivers, **Install driver (.zip)** or
  **Use system driver**; the app restarts.

## Fixes for Android

- **Code mods** (`.nrm`, e.g. the Asset Expansion Pak, translations): Android doesn't let the app make a hooked code
  page executable again; the hooked pages are swapped for anonymous copies, patches that cross a page boundary are
  handled and the instruction cache is flushed — Rival's fix from
  [AurelioB/N64ModernRuntime#1](https://github.com/AurelioB/N64ModernRuntime/pull/1).
- **GPUs without dual source blending** (every Mali GPU): the renderer takes the same fallbacks as on Qualcomm's
  driver instead of failing every alpha blended draw (white or missing textures).
- Audio crackling and pitch jumps fixed; real full screen.

## Known issues

- Not tested on a Mali GPU or with every mod yet.
- With the system driver, Android's screen recording can make the GPU hang after a moment.

Report problems on this repository's [Issues](../../issues) page, with the device, Android version and GPU driver.

## Building

On Linux: decompress your ROM and generate the game code as described in [BUILDING.md](BUILDING.md) (N64Recomp,
RSPRecomp, the MIPS patches — with clang 21 or newer add `-Wno-error=incompatible-pointer-types`), build
`lib/rt64/src/tools/file_to_c` for the host, apply the patches in [android/patches](android/patches) to the
submodules (`git -C <submodule> apply`), then build the APK with
`./gradlew assembleRelease -PHOST_FILE_TO_C=<path to file_to_c>` in `android/` (NDK, JDK 21).

## AI assistance

The changes in this Android version were made with AI assistance ([Claude Code](https://claude.com/claude-code)).
Planning, testing on the device and the decision what gets released are by the repository owner. Use at your own
risk. This applies only to this version's own changes, not to the code of the original projects.

## Credits and license

- [BanjoRecomp/BanjoRecomp](https://github.com/BanjoRecomp/BanjoRecomp) — Banjo: Recompiled (see
  [README-UPSTREAM.md](README-UPSTREAM.md) for its team and credits)
- [deivid22srk/dk64-recomp-android](https://github.com/deivid22srk/dk64-recomp-android) — the Android layer this
  version's DK64 port started from (NDK build, SDLActivity integration, SAF file bridge, libadrenotools / Turnip,
  lifecycle)
- Rival — the code mod fix ([AurelioB/N64ModernRuntime#1](https://github.com/AurelioB/N64ModernRuntime/pull/1),
  for [AurelioB/BanjoRecomp-Android#15](https://github.com/AurelioB/BanjoRecomp-Android/issues/15))
- [ogdanimal/Goemon64Recomp-Android](https://github.com/ogdanimal/Goemon64Recomp-Android) — found the Mali cause
  (dual source blending)
- [N64Recomp](https://github.com/N64Recomp/N64Recomp) and [N64ModernRuntime](https://github.com/N64Recomp/N64ModernRuntime)
  by Mr-Wiseguy, [RecompFrontend](https://github.com/N64Recomp/RecompFrontend)
- [RT64](https://github.com/rt64/rt64) by Dario (DarioSamo), with [plume](https://github.com/renderbag/plume)
- [Banjo-Kazooie decompilation](https://gitlab.com/banjo.decomp/banjo-kazooie)
- [librashader](https://github.com/SnowflakePowered/librashader) by SnowflakePowered (MPL-2.0 / GPL-3.0)
- [libadrenotools](https://github.com/bylaws/libadrenotools) by bylaws — custom Vulkan drivers
- [SDL2](https://www.libsdl.org/), [RmlUi](https://github.com/mikke89/RmlUi) and the other libraries in the
  submodules

License: GPL-3.0, see [COPYING](COPYING). The source code of all changes is in this repository.

*Fan project, not affiliated with Nintendo, Rare or Microsoft. Banjo-Kazooie is © Microsoft / Rare.*
