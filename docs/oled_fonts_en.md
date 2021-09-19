# About

Here's a brief description about how to draw images/animations on OLED screen and how to change their appearances.

**NOTE:** I'm currently rewriting source codes, so this is a tentative description. The content will be probably modified again.

---

## What's OLED fonts?

It's a [Helix](https://github.com/MakotoKurauchi/helix) style graphical font that have **32 horizontal blocks**.

For example, `layers.c` has **32 x 4 blocks**, and `game/invader.c` has **32 x 7 blocks** respectively.

And each block has **horizontal 6 pixels and vertical 8 pixels**.

The terms such as "block", "row", and "column" are illustrated below:

![](https://github.com/snagimmo/qmk_shooter/blob/main/images/fonts.png)

**NOTE:**

The number of horizontal blocks of OLED font is actually **NOT** fixed to 32. It may be possible to change this number by rewriting `font_block.c` as follows:

---

## The number of OLED font-blocks

As for the number of horizontal/vertical blocks of the OLED display, the value is updated and kept with the `static` variable in `font_block.c`.

And the number of pixels to skip is called `offset`.

```c
static uint8_t hrzn_blocks = 0;
static uint8_t vert_blocks = 0;
static uint8_t offset = 0;
```

If you use the display SSD1306 of size **128 x 32 px**, the following calculations for `offset` result in **2px** in both portrait and landscape orientation.

```c
void set_rotation(uint8_t isVertical) {
    if (isVertical) {
        // vertical
        // 32px / font-width 6
        hrzn_blocks = OLED_DISPLAY_HEIGHT / OLED_FONT_WIDTH;  // 5 blocks (128 x 32)
        // remainder is 2
        offset      = OLED_DISPLAY_HEIGHT % OLED_FONT_WIDTH;  // 2px (128 x 32)
        // 128px / font-height 8
        vert_blocks = OLED_DISPLAY_WIDTH  / OLED_FONT_HEIGHT; // 16 blocks (128 x 32)
    } else {
        // horizontal
        // 128px / font-width 6
        hrzn_blocks = OLED_DISPLAY_WIDTH  / OLED_FONT_WIDTH;  // 21 blocks (128 x 32)
        // remainder is 2
        offset      = OLED_DISPLAY_WIDTH  % OLED_FONT_WIDTH;  // 2px (128 x 32)
        // 32px / font-height 8
        vert_blocks = OLED_DISPLAY_HEIGHT / OLED_FONT_HEIGHT; // 4 blocks (128 x 32)
    }
}
```
---

## How does `write_font_blocks()` work?

The example of the implementation written in my `keymap.c` is like:

```c
write_font_blocks(layer_char, 2, 5, 0x01, 0);
```

Inside the `write_font_blocks()`, each parameter has the following meaning:

- 1st param is the pointer to a hex array (OLED font array, `const unsigned char*` type)
- 2nd is the number of **rows** in the fonts you want to read
- 3rd is the number of **columns** in the fonts you want to read
- 4th is the **read-index(rIdx)** on OLED font (i.e. `game/invader.c`, `layers.c`
- 5th is the **write-index(wIdx)** on OLED buffer (your OLED screen)

In short, the example above means,

> "Read OLED fonts of **2** rows and **5** columns starting from **0x01** of **`layer_char[]`,** and then write them from **0** (top-left) of OLED screen."

**NOTE:**

To distinguish `read-index` from `write-index`, only `read-index` is expressed as hexadecimal (internally the same, though.)

---

## How to change appearances

Graphical fonts are placed in `.c` source files (e.g. `game/invader.c` or `layers.c`) as a hex array:

```c
const unsigned char invader_font[] PROGMEM {
     0x00, ...
};
```

And you can modify this with the tool below:

- [helix/FontConverter at master · MakotoKurauchi/helix](https://github.com/MakotoKurauchi/helix/tree/master/FontConverter)

This tool can convert a Bitmap image into a hex array (and vice versa). I recommend that you should redirect its standard output to an intermediate file (e.g. `font.txt`).

After that, you should replace the hex array `THIS_IS_FONTNAME[]` in `YOUR_FONTFILE.c` with the tool's output. And then you get your own graphical font.

```c
const unsigned char THIS_IS_FONTNAME[] PROGMEM {
    // Replace these hex numbers with the output of the tool
    0x00, 0x80, 0xE0, ...
};
```

As mentioned above, a Helix-type OLED font normally has **32blocks** horizontally.

The hex arrays such as `layer_char[]` and `invader_font[]` are called within the function `write_font_blocks()` in `keymap.c` / `drawing.c` , so please modify the name of them on yourself if you want to rewrite codes.

I placed some bitmap images in `qmk_shooter/images` as reference, so let's try editing them with paint software, and then converting them with the converter tool.

By the way, be careful **NOT** to fatten a firmware's size so much (especially with QMK Firmware ver 0.14+).

For more details, see the official docs: [QMK firmware Docs](https://github.com/qmk/qmk_firmware).
