# Guide for other keyboards

This is a guide on how to run the shooter game on keyboards other than Claw44.

These instructions assume that you've already installed [QMK Firmware](https://github.com/qmk/qmk_firmware) & [QMK CLI](https://docs.qmk.fm/#/cli?id=qmk-cli) in your environment.

If you haven't already done so, please read [here](https://docs.qmk.fm/#/ja/newbs_getting_started) and set up QMK Firmware, etc.

If these settings don't work in your environment, please **rewrite it yourself**.

**NOTE:**

You **MUST** backup all the related files before rewriting/overwriting them.

I'm **NOT** responsible for any mistakes, problems, failures, or troubles.


# Overview

1. You have to copy(clone) these direcory/files into `YOUR_KEYBOARD/keymaps/shooter` directory.

- `qmk_shooter/game`
- `qmk_shooter/font_block.c`
- `qmk_shooter/font_block.h`
    - In this guide, you will clone the whole repository with `git clone`, but in the end, it will work if you leave only the above three files/directories in the repository.

2. And you may have to rewrite/overwrite files below after copying them.

- `YOUR_KEYBOARD/keymaps/YOUR_KEYMAP/keymap.c`
- `YOUR_KEYBOARD/keymaps/YOUR_KEYMAP/rules.mk`
- `YOUR_KEYBOARD/keymaps/YOUR_KEYMAP/config.h`
    - You'll also need these three files in addition to the three files/directories you left in the Overview #1.
    - Copying them to another directory and working on them, so **as long as you follow the instructions**, you don't have to modify these original files.


# Manual Setup

If you use keyboard other than Claw44 or want to rewrite/overwrite your own keymaps, please follow these instruction.

The explanation might be too long, but please bear with it.

There are some partial results of the `git diff` for your reference, so please modify `keymap.c` on yourself with it.

---

## 1. Preparing for installation

## 1-1. Cloning this repo

First of all, run the command below and move to `YOUR_KEYBOARD` directory.

```bash
cd $(qmk config setup.home | cut -d "=" -f2)/YOUR_KEYBOARD/keymaps
```

For example, `YOUR_KEYBOARD` is:

- If using [Helix](https://github.com/MakotoKurauchi/helix): `helix`
- If using [Corne Keyboard](https://github.com/foostan/crkbd/): `crkbd`

After that, clone this repo into your `keymaps/shooter` directory.

```bash
# SSH
git clone git@github.com:snagimmo/qmk_shooter.git ./shooter

# HTTPS
git clone https://github.com/snagimmo/qmk_shooter.git ./shooter
```

---

## 1-2. Copying YOUR_KEYMAP directory

Run the commands below.

```bash
# Copy all the files in your YOUR_KEYMAP directory
cp -rvf ./YOUR_KEYMAP/* ./shooter
```

**NOTE:**

Never apply `diff.patch` to your directory in this case. That's the file only for users of Claw44.

---

## 1-3. Preparation is done

Now, your `qmk_firmware/keyboards/YOUR_KEYBOARD` directory should look like a structure as follows:

```bash
YOUR_KEYBOARD
# Some files/directories omitted
├── keymaps
│   ├── YOUR_KEYMAP
│   └── shooter # Added here
│       ├── .git # You can remove this later
│       ├── docs # You can remove this later
│       ├── game
│       │   ├── drawing.c
│       │   ├── drawing.h
│       │   ├── game.c
│       │   ├── game.h
│       │   ├── init.h
│       │   ├── invader.c
│       │   ├── rndgen.c
│       │   └── rndgen.h
│       ├── images # You can remove this later
│       ├── config.h
│       ├── diff.patch # You can remove this later
│       ├── font_block.c
│       ├── font_block.h
│       ├── keymap.c
│       ├── layers.c # You can remove this later
│       ├── LICENSE.md # You can remove this later
│       ├── README.md # You can remove this later
│       └── rules.mk
# Some files/directories omitted
└── rules.mk
```


## 2. Modifying keymap.c

Let's modify your file `YOUR_KEYBOARD/keymaps/shooter/keymap.c` (it's **NOT** `YOUR_KEYMAP/keymap.c`).

### 2-1. Adding include directives

Add directives below to your `keymap.c` file.

This is the result of `git diff` between Claw44's `oled` (default OLED keymap) and `shooter` keymap:

```diff
@@ -16,6 +16,13 @@ along with this program.  If not, see <http://www.gnu.org/licenses/>.
 #include QMK_KEYBOARD_H
 #include <stdio.h>

+#include "font_block.h"
+#include "game/game.h"
```

---

### 2-2. Adding a unique keycode

Next, you have to add the `GM_INV` keycode to `enum` object.

```diff
@@ -28,6 +35,10 @@ enum layer_number {
     _ADJUST,
 };

+enum custom_keycodes {
+    GM_INV = SAFE_RANGE
+};
+
```

The `SAFE_RANGE` macro is recommended here so that keycodes don't interfere with each other.

For more detail, see the docs:

- [Customizing Functionality](https://docs.qmk.fm/#/custom_quantum_functions?id=defining-a-new-keycode)

---

### 2-3. Modifying your keymap

Add keycodes that you added on the former section into your `keymaps[]` array.

In the example below, `GM_INV` keycode is that.

Of course you can put it elsewhere on your keymap.

```diff
@@ -54,7 +65,7 @@ const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
     //|--------+--------+--------+--------+--------+--------|   |--------+--------+--------+--------+--------+--------|
         KC_LPRN, KC_HASH, KC_DLR , KC_DQT , KC_QUOT, KC_TILD,     KC_LEFT, KC_DOWN,  KC_UP , KC_RGHT, KC_GRV , KC_RPRN,
     //|--------+--------+--------+--------+--------+--------|   |--------+--------+--------+--------+--------+--------|
-        _______, _______, _______, _______, KC_LCBR, KC_LBRC,     KC_RBRC, KC_RCBR, _______, _______, _______, _______,
+        _______, GM_INV , _______, _______, KC_LCBR, KC_LBRC,     KC_RBRC, KC_RCBR, _______, _______, _______, _______,
     //`--------+--------+--------+--------+--------+--------/   \--------+--------+--------+--------+--------+--------'
                           _______, _______, _______, _______,     _______, _______, _______, RESET
     //                  `--------+--------+--------+--------'   `--------+--------+--------+--------'
```

---

### 2-4. Changing keycodes' behavior

Next, modify the `switch case` blocks within the `process_record_user()`.

```diff
 bool process_record_user(uint16_t keycode, keyrecord_t *record) {
-    if (record->event.pressed) {
-        set_keylog(keycode, record);
+    switch (keycode) {
+        case GM_INV:
+            // Toggle gaming mode & clear OLED display
+            if (!record->event.pressed) {
+                toggleGamingMode();
+                if (isGamingMode()) {
+                    readMainTimer();
+                    initGame();
+                    startGame();
+                }
+                clear_display();
+            }
+            break;
+        case KC_LEFT:
+            if (record->event.pressed) {
+                if (isGamingMode()) {
+                    movePlayer(1); // 1 = isLeft
+                    return false;
+                }
+            }
+            break;
+        case KC_RIGHT:
+            if (record->event.pressed) {
+                if (isGamingMode()) {
+                    movePlayer(0); // 0 = isRight
+                    return false;
+                }
+            }
+            break;
+        case KC_SPC:
+            if (record->event.pressed) {
+                if (isGamingMode()) {
+                    firePlayerBeam();
+                    return false;
+                }
+            }
+            break;
+        case KC_ENT:
+        case KC_LGUI:
+            if (record->event.pressed) {
+                if (isGamingMode()) {
+                    return false;
+                }
+            }
+            break;
     }
     return true;
 }
```

If you'd rather use your own key bindings, of course you can modify its name as you like.

Needless to say, however, you should **NOT** change the name of each called functions.

Moreover, you may have to rewrite these `case`s yourself if they already exists within `process_record_user()`.

---

### 2-5. Adding a gaming processing

Please look the following functions.

It's the **CORE** of gaming processing.

```diff
 void oled_task_user(void) {
     if (is_keyboard_master()) {
-        render_layer_state();
-        oled_write_ln(read_keylog(), false);
-        oled_write_ln(read_keylogs(), false);
+        if (!isGamingMode()) {
+            /* render_layer_state(); */
+        }
     } else {
         render_logo();
     }
 }

+void matrix_scan_user(void) {
+    if (isGamingMode()) {
+        if (countMainTimer() > 0) {
+            game_main();
+        }
+    }
+}
+
```

It's important that `isGamingMode()` returns a value to check whether gaming mode is ON or OFF.

In the example above,

- the normal processing block is included within `if (!isGamingMode())` block of `oled_task_user()`,

- while gaming processing block is included within `if (isGamingMode())` block of `matrix_scan_user()`.

These two `xxx_user()` functions and the mode checker make `game_main()` function keep on looping.

If there are already unavoidable processes inside these functions, please rewrite them **yourself**.

---

### 2-6. Making the display vertical (if necessary)

If you need, modify the `oled_init_user()` function to make your keyboard's display vertical.

```diff
 oled_rotation_t oled_init_user(oled_rotation_t rotation) {
     if (!is_keyboard_master()) return OLED_ROTATION_180;
-    return rotation;
+    set_rotation(1); // vertical (main display)
+    return OLED_ROTATION_270;
 }
```

In the case you use a split keyboard and OLED screen on "non-master" side of it displays the keyboard's logo horizontally, you may have to do this.

---

### 2-7. Stopping all the displaying-related functions

Modification of `keymap.c` ends in this section.

If your keyboard's `keymap.c` has functions drawing pixels on OLED display, you **MUST** comment them all out or remove them all.

My source codes adopt a very different way of drawing than the default, so you have to do that.

If it doesn't work, please rewrite the whole source code **on yourself** (sorry for inconvenience).

---

#### Why not using `xxx_ln()` or `xxx_P()` functions?

By default, most QMK keyboards' displaying processes use `oled_write_ln_P()`, `oled_write_P()` or `oled_write_ln()` etc.

In contrast to that, in my codes displaying functions use `pgm_read_byte()` and `oled_write_raw_byte()`.

My codes do writing to buffers **per one byte**, and this make it easier to draw pics/animations (in my opinion).

I strongly wanted to implement pixel art animation of the characters/beams, that's why I didn't adopt `xxx_ln()` or `xxx_P()` functions.

Perhaps there may be another way, and it's only tentative.

---

### How to call the function `render_layer_state()`

If you want to call the function `render_layer_state()` properly, please follow these instructions.

First, include `layers.c` additionally as follows:

```diff
@@ -16,6 +16,13 @@ along with this program.  If not, see <http://www.gnu.org/licenses/>.
 #include QMK_KEYBOARD_H
 #include <stdio.h>

+#include "layers.c"
 #include "font_block.h"
 #include "game/game.h"
```

And then implement the function like:

```diff
+void render_layer_state(void) {
+    switch (get_highest_layer(layer_state)) {
+        case _QWERTY:
+            write_font_blocks(layer_char, 2, 5, 0x01, 0);
+            break;
+        case _RAISE:
+            write_font_blocks(layer_char, 2, 5, 0x06, 0);
+            break;
+        case _LOWER:
+            write_font_blocks(layer_char, 2, 5, 0x0b, 0);
+            break;
+        case _ADJUST:
+            write_font_blocks(layer_char, 2, 5, 0x10, 0);
+            break;
+        default:
+            write_font_blocks(layer_char, 2, 5, 0x15, 0);
+    }
+}
```

In the end, uncomment a line calling this function:

```diff
 void oled_task_user(void) {
     if (is_keyboard_master()) {
         if (!isGamingMode()) {
-            /* render_layer_state(); */
+            render_layer_state();
         }
     } else {
         render_logo();
     }
 }
```

After that, please make sure the whole code is correct.

By the way, a description about how `write_font_blocks()` works is on [docs/oled_fonts_en.md](https://github.com/snagimmo/qmk_shooter/blob/main/docs/oled_fonts_en.md).

---

## 3. Modifying rules.mk

Let's add your source codes to `YOUR_KEYBOARD/keymaps/shooter/rules.mk` (it's **NOT** `YOUR_KEYMAP/rules.mk`).

This file stores informations to build your source codes.

Without this section, your QMK's build task won't be completed.

```diff
@@ -1,2 +1,13 @@
 OLED_ENABLE = yes
 OLED_DRIVER = SSD1306
+OLED_GAMING = yes
+LTO_ENABLE = yes
+
+ifeq ($(strip $(OLED_ENABLE)), yes)
+  SRC += font_block.c
+endif
+
+ifeq ($(strip $(OLED_GAMING)), yes)
+  SRC += game/game.c
+  SRC += game/drawing.c
+  SRC += game/rndgen.c
+endif
```

As I've mentioned repeatedly, you may have to rewrite them yourself if it doesn't work.

**NOTE:**

Adding `LTO_ENABLE` (Link Time Optimization) to `rules.mk` can significantly reduce the compiled firmware size.

For example, my firmware size will be about **97%** of max without this option, but **89%** with the option.

I highly recommend it.


---

## 4. Modifying config.h

See your `YOUR_KEYBOARD/keymaps/shooter/config.h` (it's **NOT** `YOUR_KEYMAP/config.h`).

If there are symbol definitions related to OLED-displaying, you **MUST** comment it out or remove it.

In the case that your `config.h` is like this, just comment one line out.

```diff
@@ -19,4 +19,4 @@ along with this program.  If not, see <http://www.gnu.org/licenses/>.
 #pragma once

 #define MASTER_LEFT
-#define OLED_FONT_H "keyboards/claw44/lib/glcdfont.c"
+// #define OLED_FONT_H "keyboards/claw44/lib/glcdfont.c"
```


---

## 5. Build & Flash

Now let's build your keyboard's firmware in your local QMK environment.

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

or you should follow your keyboard's documentations/build-guide.


---

## 6. Let's play

If there are no errors after finishing the build & flash, the game is ready.

Now you can press the `GM_INV` key that toggles `gaming_mode` variable, and then the game starts.

Thanks for your patience and sorry for the long explanation.
