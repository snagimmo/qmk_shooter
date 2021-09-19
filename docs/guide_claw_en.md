# Guide for Claw44

This is a guide on how to run the shooter game on your Claw44.

These instructions assume that you've already installed [QMK Firmware](https://github.com/qmk/qmk_firmware) & [QMK CLI](https://docs.qmk.fm/#/cli?id=qmk-cli) in your environment.

If you haven't already done so, please read [here](https://docs.qmk.fm/#/ja/newbs_getting_started) and set up QMK Firmware, etc.

If these settings don't work in your environment, please **rewrite it yourself**.

**NOTE:**

You **MUST** backup all the related files before rewriting/overwriting them.

I'm **NOT** responsible for any mistakes, problems, failures, or troubles.


# Overview

There's two setup ways to play this game. One is easy, and the other is a hassle.

If you are a Claw44's user, all you have to do is to apply a patch to your local directory.

However, if you want to rewrite/overwrite your `keymap.c`, see [docs/guide_other_en.md](https://github.com/snagimmo/qmk_shooter/blob/main/docs/guide_other_en.md).


# Easy Setup

This patching method is very easy to get started.

## 1. Cloning this repo

First, move into your `qmk_firmware/keyboards/claw44/keymaps` directory.

```bash
# Move into the directory
cd $(qmk config setup.home | cut -d "=" -f2)/keyboards/claw44/keymaps
```

Next, clone this repo there.

```bash
# SSH
git clone git@github.com:snagimmo/qmk_shooter.git ./shooter

# HTTPS
git clone https://github.com/snagimmo/qmk_shooter.git ./shooter
```

## 2. Copying the oled directory

Run the commands below.

```bash
# Duplicate your "oled" directory
cp -rvf ./oled/* ./shooter
```

## 3. Applying a patch to your directory

Just apply a patch file and then it automatically updates nesessary files within `shooter` (`oled` directory will be left as default).

```bash
# Move to the shooter directory
cd ./shooter
# Apply the patch file
patch -p5 < diff.patch
```

## 4. Finished

Now, your `qmk_firmware/keyboards/claw44` directory should look like a structure as follows:

```bash
./keyboards/claw44/
# Some files/directories omitted
├── keymaps
│   ├── default
│   ├── oled
│   ├── shooter # Added here
│   │   ├── .git # You can remove this later
│   │   ├── docs # You can remove this later
│   │   ├── game
│   │   │   ├── drawing.c
│   │   │   ├── drawing.h
│   │   │   ├── game.c
│   │   │   ├── game.h
│   │   │   ├── init.h
│   │   │   ├── invader.c
│   │   │   ├── rndgen.c
│   │   │   └── rndgen.h
│   │   ├── images # You can remove this later
│   │   ├── config.h
│   │   ├── diff.patch # You can remove this later
│   │   ├── font_block.c
│   │   ├── font_block.h
│   │   ├── keymap.c
│   │   ├── layers.c
│   │   ├── LICENSE.md # You can remove this later
│   │   ├── README.md # You can remove this later
│   │   └── rules.mk
│   └── via
├── lib
│   └── glcdfont.c
├── readme.md
├── rev1
└── rules.mk
```

## 5. Build & Flash

Now the only thing left to do is to build & flash `.hex` file.

For example, you can build and flash with the following commands:

```bash
# Build only
make claw44:shooter

# Build & Flash
make claw44:shooter:avrdude
```

or

```bash
# Build & Flash for ProMicro
qmk flash -kb claw44 -km shooter

# Build & Flash for Elite-C (Untested)
qmk flash -kb claw44 -km shooter -bl dfu
```

On Windows/MacOS, you can use the [**QMK Toolbox**](https://docs.qmk.fm/#/newbs_flashing?id=flashing-your-keyboard-with-qmk-toolbox) as well.

For more details, see the official docs:

- [QMK website](https://docs.qmk.fm/#/)
- [Setup](https://docs.qmk.fm/#/newbs_getting_started)
- [Flashing Firmware](https://docs.qmk.fm/#/newbs_flashing)
- [Claw44 Build guide](https://kbd.dailycraft.jp/claw44/buildguide/)


## 6. Let's play

If there are no errors after finishing the build & flash, the game is ready.

Now you can press the `GM_INV` key on the 2nd layer ("Z" key on `_RAISE`) that toggles `gaming_mode` variable, and then the game starts.


---

# Manual Setup

If you've failed/mistaken in the setup above or want to rewrite/overwrite keymaps, please follow the instruction with [docs/guide_other_en.md](https://github.com/snagimmo/qmk_shooter/blob/main/docs/guide_other_en.md).
