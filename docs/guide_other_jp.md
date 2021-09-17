# 他のキーボード用ガイド

Claw44以外のキーボードでこのゲームを動かす為の説明です。

この説明は、[QMK Firmware](https://github.com/qmk/qmk_firmware) や [QMK CLI](https://docs.qmk.fm/#/ja/cli?id=qmk-cli) が既にインストールされていることを前提としています。

まだの方は[こちら](https://docs.qmk.fm/#/ja/newbs_getting_started)を読みながら QMK Firmware 等のセットアップをして下さい。

また、お使いの環境で上手く動作しない場合は、お手数ですが **自分で書き直してリファクタリング** してください。

**注意:**

書き換えあるいは上書きする前に、全てのファイルを **必ず** バックアップしてください。

この説明を読んで書き換え・上書き等を実行された場合に生じた問題・失敗・トラブルなどに関して私は **一切** 責任を負う事が出来ません。ご了承ください。


# 概要

1. 以下のファイル/ディレクトリをお使いのキーボードの `keymaps/shooter` ディレクトリにコピーする必要があります。

- `qmk_shooter/game`
- `qmk_shooter/font_block.c`
- `qmk_shooter/font_block.h`
    - 今回の説明では`git clone`でリポジトリごとクローンしますが、最終的にはリポジトリ内で上記3つのファイル/ディレクトリのみ残していれば動作します。

1. また、以下のファイルをコピーした上で書き換えたり、上書きしたりする必要があります。

- `YOUR_KEYBOARD/keymaps/YOUR_KEYMAP/keymap.c`
- `YOUR_KEYBOARD/keymaps/YOUR_KEYMAP/rules.mk`
- `YOUR_KEYBOARD/keymaps/YOUR_KEYMAP/config.h`
    - 概要1で残した3つのファイル/ディレクトリ以外に、これら3つのファイルも必要になります。
    - 別ディレクトリにコピーした上で作業を進めますので、**指示に従って頂ければ** 元ファイルに手を加える必要はありません。


# ファイル書き換えによる導入方法

Claw44以外のキーボードを使っている場合や、自分でキーマップを書き換えたり上書きしたりしたい場合は、以下の説明に従って導入して下さい。

少々説明が長いかもしれませんが、ご容赦ください。

参考用に `git diff` の結果の一部を掲載しておきますので、それを見ながら `keymap.c` を各自で修正してください。

---

## 1. インストールの準備

## 1-1. リポジトリをクローンする

まず、下記コマンドを実行して`YOUR_KEYBOARD`ディレクトリに移動します。

```bash
cd $(qmk config setup.home | cut -d "=" -f2)/YOUR_KEYBOARD/keymaps
```

`YOUR_KEYBOARD` とは、例えば次のようなものです。

- [Helix](https://github.com/MakotoKurauchi/helix) の場合: `helix`
- [Corne Keyboard](https://github.com/foostan/crkbd/) の場合: `crkbd`

その後、このリポジトリを `keymaps/shooter` ディレクトリにクローンします。

```bash
# SSH
git clone git@github.com:snagimmo/qmk_shooter.git ./shooter

# HTTPS
git clone https://github.com/snagimmo/qmk_shooter.git ./shooter
```

---

## 1-2. YOUR_KEYMAPディレクトリのコピー

以下のコマンドを実行します。

```bash
# YOUR_KEYMAP ディレクトリ内のファイルを全てコピー
cp -rvf ./YOUR_KEYMAP/* ./shooter
```

**注意:**

`diff.patch` はディレクトリには絶対に適用しないでください。Claw44ユーザ用のパッチファイルです。

---

## 1-3. 準備完了

この時点で、`qmk_firmware/keyboards/YOUR_KEYBOARD` ディレクトリは、以下のような構造になっているはずです。

```bash
YOUR_KEYBOARD
# いくつかのファイル/ディレクトリ省略
├── keymaps
│   ├── YOUR_KEYMAP
│   └── shooter # ここに追加
│       ├── .git # 後で削除しても大丈夫です
│       ├── docs # 後で削除しても大丈夫です
│       ├── game
│       │   ├── drawing.c
│       │   ├── drawing.h
│       │   ├── game.c
│       │   ├── game.h
│       │   ├── init.h
│       │   ├── invader.c
│       │   ├── rndgen.c
│       │   └── rndgen.h
│       ├── images # 後で削除しても大丈夫です
│       ├── config.h
│       ├── diff.patch # 後で削除しても大丈夫です
│       ├── font_block.c
│       ├── font_block.h
│       ├── keymap.c
│       ├── layers.c # 後で削除しても大丈夫です
│       ├── LICENSE.md # 後で削除しても大丈夫です
│       ├── README.md # 後で削除しても大丈夫です
│       └── rules.mk
# いくつかのファイル/ディレクトリ省略
└── rules.mk
```

## 2. keymap.cの修正

まず `YOUR_KEYBOARD/keymaps/shooter/keymap.c` (これは `YOUR_KEYMAP/keymap.c` ではないので注意して下さい) を修正してみましょう。

### 2-1. インクルードディレクティブの追加

以下のディレクティブを `keymap.c` ファイルに追加します。

下にあるのはClaw44 の `oled` (デフォルトの OLED キーマップ) と `shooter` キーマップの間で `git diff` した結果です。

```diff
@@ -16,6 +16,13 @@ along with this program.  If not, see <http://www.gnu.org/licenses/>.
 #include QMK_KEYBOARD_H
 #include <stdio.h>

+#include "font_block.h"
+#include "game/game.h"
```

---

### 2-2. 独自キーコードの追加

次に、`enum` オブジェクトに `GM_INV` というキーコードを追加します。

```diff
@@ -28,6 +35,10 @@ enum layer_number {
     _ADJUST,
 };

+enum custom_keycodes {
+    GM_INV = SAFE_RANGE
+};
+
```

独自キーコードを追加する場合、キーコード同士がお互いに干渉しないよう `SAFE_RANGE` マクロの利用が推奨されています。

詳しくは、公式ドキュメントをご覧ください。

- [機能のカスタマイズ](https://docs.qmk.fm/#/custom_quantum_functions?id=defining-a-new-keycode)

---

### 2-3. キーマップの変更

前節で追加したキーコードを `keymaps[]` 配列に追加します。

以下の例では、`GM_INV` というキーコードがそれです。

もちろん、キーマップ内の他の場所に設定することもできます。

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

### 2-4. キーコードの動作変更

次に、`process_record_user()`の中の`switch case`ブロックを変更します。

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

これとは別に独自のキーバインディングを使いたい場合は，もちろん自由に変更することができます。

ただし、それぞれ呼び出されている関数の名前を変更しないようにして下さい。

また、これらの`case`文と同じものが既に `process_record_user()` 内に存在する場合は、自分で書き直さなければならないかもしれません。

---

### 2-5. ゲーム処理の追加

以下の関数群をご覧ください。

これはゲーム処理の **コア部分** です。

```diff
 void oled_task_user(void) {
     if (is_keyboard_master()) {
-        render_layer_state();
-        oled_write_ln(read_keylog(), false);
-        oled_write_ln(read_keylogs(), false);
+        if (!isGamingMode()) {
+            render_layer_state();
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

ここでは、`isGamingMode()`が値を返すことで、ゲーミングモードがONかOFFかをチェックしていることが重要です。

上の例では、

- 通常の処理ブロックは、`oled_task_user()`の`if (!isGamingMode())`ブロックの中に含まれています。

- 一方、ゲーム処理ブロックは、`matrix_scan_user()`の`if (isGamingMode())`ブロックに含まれています。

これら2つの`xxx_user()`関数とモードチェッカーによって、`game_main()`関数はループし続けます。

もしもこれらの関数の中に重要な処理が既に存在している場合は、**ご自分で** 修正をお願いいたします。

---

### 2-6. OLEDディスプレイの垂直化(必要な場合)

必要であれば、`oled_init_user()` 関数を修正してキーボードのディスプレイを縦長にします。

```diff
 oled_rotation_t oled_init_user(oled_rotation_t rotation) {
     if (!is_keyboard_master()) return OLED_ROTATION_180;
-    return rotation;
+    set_rotation(1); // vertical (main display)
+    return OLED_ROTATION_270;
 }
```

---

### 2-7. 表示関連の関数を全て停止する

この節で `keymap.c` の改造は終了です。

お使いのキーボードの `keymap.c` に、OLEDディスプレイへの描画を行う関数がある場合は、**必ず** それら全てをコメントアウトするか、削除して下さい。

私のソースコードはデフォルトとは全く異なる描画方法を採用していますので、そうしないと機能しません。

上手くいかない場合、ソースコード全体を **自分で** 書き直したり、リファクタリングしたりしてください(お手数ですが)。

---

#### `xxx_ln()`や`xxx_P()`関数を使わない理由は？

大多数のQMKキーボードの表示処理では、デフォルトで `oled_write_ln_P()`、`oled_write_P()`、`oled_write_ln()` などが使われています。

これに対し私のコードでは、プロセスを表示する関数は `pgm_read_byte()`や`oled_write_raw_byte()` を使用しています。

バッファへの書き込みを **バイト単位** で行っているので、フォントやアニメーションを描きやすくなっています(個人的な意見ですが)。

キャラクターやビームのピクセルアート風アニメーションを実装したいと強く思っていた事もあり、`xxx_ln()`や`xxx_P()`関数は採用しませんでした。

他に良い方法があるのかも知れませんが、ひとまず暫定的なものとして採用しています。


---

## 3. rules.mkの修正

それでは、ソースコードを `YOUR_KEYBOARD/keymaps/shooter/rules.mk` に追加してみましょう (これは `YOUR_KEYMAP/rules.mk` ではないので注意して下さい)。

このファイルには、QMKがソースコードをビルドするために使う情報が格納されています。

このファイル修正がないと、恐らくQMKのビルドタスクは上手く機能しないと思います。

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

何度も言いますが、うまくいかない場合は自分で書き換える必要があるかもしれません。

**注意:**

QMKファームウェアがVer 0.13以前の場合、`OLED_DRIVER_ENABLE` マクロが存在する可能性があります。

その場合は、(念のため)QMKファームウェアとQMK CLIのバックアップ完了後、それぞれアップデートしてください。

**注意:**

`LTO_ENABLE` (Link Time Optimization) マクロ を`rules.mk`に追加することで、コンパイル後のファームウェアサイズを大幅に減らすことができます。

例えば、私のファームウェアサイズはこのオプションなしでは最大値の約 **97%** になりますが、このオプションをつけると **89%** になります。

個人的にはこのオプション利用を強くお勧めします。


---

## 4. config.hの修正

最後に、 `YOUR_KEYBOARD/keymaps/shooter/config.h` をご覧ください (これは `YOUR_KEYMAP/config.h` ではないので注意して下さい)。

OLEDディスプレイに関連するシンボル定義がある場合は、**必ず**コメントアウトするか削除してください。

例えばもし `config.h` が下記の通りでしたら、ただ1行コメントアウトするだけです。

```diff
@@ -19,4 +19,4 @@ along with this program.  If not, see <http://www.gnu.org/licenses/>.
 #pragma once

 #define MASTER_LEFT
-#define OLED_FONT_H "keyboards/claw44/lib/glcdfont.c"
+// #define OLED_FONT_H "keyboards/claw44/lib/glcdfont.c"
```


---

## 5. ビルドと書き込み(フラッシュ)

さて、あとは `.hex` ファイルをビルドしてフラッシュするだけです。

ご存知とは思いますが、次のようなコマンドでビルドやフラッシュが行えます。

```bash
# ビルドのみ
make YOUR_KEYBOARD:shooter

# ビルドとフラッシュ
make YOUR_KEYBOARD:shooter:avrdude
```

またはこれでも出来ます。

```bash
# ProMicro用のビルドとフラッシュ
qmk Flash -kb YOUR_KEYBOARD -km shooter

# Elite-C用のビルドとフラッシュ(未検証)
qmk flash -kb YOUR_KEYBOARD -km shooter -bl dfu
```

WindowsまたはMacOSでは、[**QMK Toolbox**](https://docs.qmk.fm/#/ja/newbs_flashing?id=qmk-toolbox-%E3%82%92%E4%BD%BF%E3%81%A3%E3%81%A6%E3%82%AD%E3%83%BC%E3%83%9C%E3%83%BC%E3%83%89%E3%81%AB%E6%9B%B8%E3%81%8D%E8%BE%BC%E3%82%80)も使用できます。

なお、ビルドやフラッシュに関する詳細は下記公式ドキュメントをご覧ください。

- [QMK公式](https://docs.qmk.fm/#/ja/)
- [QMK環境の構築](https://docs.qmk.fm/#/ja/newbs_getting_started)
- [ファームウェアを書き込む](https://docs.qmk.fm/#/ja/newbs_flashing)

または、お持ちの各キーボードのドキュメント/ビルドガイドに従ってください。

---

## 6. ゲーム開始

ビルドと書き込みが終わった後、エラーがなければゲームの準備は完了です。

あとは、`GM_INV`キーを押して、`gaming_mode`変数をトグルさせれば、ゲームが始まります。

お待たせしました。長い説明で申し訳なかったです。
