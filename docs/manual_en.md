# About this

A simple shooter game like Space Invaders running on QMK firmware.

This program made simple animations possible by drawing different fonts in each 8x6 pixel field on every frame, which results in the smooth movement of beams and characters.


# Environment

## Required

- DIY mechanical keyboard
    - It must support QMK firmware.
- OLED display (128x32 px & vertical only)
    - **128 x 32** px only. Other sizes are **NOT SUPPORTED**.
    - It must be **vertical**, and a horizontal style is **NOT SUPPORTED**.
    - This program is basically made for the **SSD1306**.
- [QMK Firmware](https://github.com/qmk/qmk_firmware) ver 0.14+
    - This program has been developed & tested with **QMK firmware 0.14.15**.
    - If your firmware is still ver 0.13 or earlier, please update it.

## Recommended

- [Claw44](https://github.com/yfuku/claw44) (split mechanical keyboard)
    - It's not necessary needed, but you can more easily get started with it.


# Install/Build

See the documentations within `qmk_shooter/docs` for more details.

1. Claw44 users
- All you have to do is to apply `diff.patch` to your `qmk_firmware` directory.
- See `qmk_shooter/docs/guide_claw_en.md`.

2. Other keyboard users
- Sorry to bother you, but you have to **manually** modify several files.
- See `qmk_shooter/docs/guide_other_en.md`.


# How to play

You can press the keys to control the player-character for example:

- `KC_LEFT` ("←"): Move the player to the left
- `KC_RGHT` ("→"): Move the player to the right
- `KC_SPC` ("Space"): Fire a beam
    - Firing is limited up to **3 times** by default.
- `GM_INV` (Unique key): Start/exit the game

**NOTE:** `KC_` is a prefix of keycodes for QMK firmware to recognize which key is pressed by users.

The game mode can be toggled with the `GM_INV` key, and Enter(`KC_ENT`) & Windows(`KC_LGUI`) keys don't trigger any event while the game mode is on.

If you'd rather use your own key bindings than these, of course you can configure customized keycodes with QMK firmware.

The goal of this game is to beat all the enemies. Some stages and some kinds of enemies are waiting for you.


# How to modify appearances

Graphical fonts are set in the file `game/invader.c` as a hex array:

```c
const unsigned char invader_font[] PROGMEM { 0x00, ...};
```

And you can modify this with the tool below:

- [helix/FontConverter at master · MakotoKurauchi/helix](https://github.com/MakotoKurauchi/helix/tree/master/FontConverter)

This tool can convert a Bitmap image into a hex array (and vice versa). I recommend that you should redirect its standard output to an intermediate file (i.e. `font.txt`).

After that, you should replace the hex array `THIS_IS_FONTNAME[]` in `YOUR_FONTFILE.c` with the intermediate file. And then you get your own graphical font (Helix-type).

```c
const unsigned char THIS_IS_FONTNAME[] PROGMEM {
    // Replace these hex numbers with the output of the tool
    0x00, 0x80, 0xE0, ...
};
```

The hex arrays such as `layer_char[]` and `invader_font[]` are called within the function `write_font_blocks()` in `keymap.c` / `drawing.c` , so please modify its name on yourself if you want to rewrite codes.

I placed some bitmap images in `qmk_shooter/images` as reference, so let's try editing them with paint software, and then converting them with the converter tool.

By the way, be careful NOT to fatten a firmware's size so much (especially with QMK Firmware ver 0.14+).

For more details, see the official docs: [QMK firmware Docs](https://github.com/qmk/qmk_firmware).


# Thanks

- `rndgen.c`
- `rndgen.h`

These are written with reference to [edrosten/8bit_rng](https://github.com/edrosten/8bit_rng) by Edward Rosten.
