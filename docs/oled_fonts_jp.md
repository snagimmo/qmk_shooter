# このドキュメントについて

OLEDディスプレイに画像やアニメーションを描く方法と、ゲームの見た目を変える方法について簡単に説明します。

※現在ソースコードを書き換えていますので、これは暫定的な説明です。恐らくまた内容に変更があると思います。

---

## OLEDフォントとは？

これは[Helix](https://github.com/MakotoKurauchi/helix)スタイルのグラフィカルなフォントのことで、 **水平方向に32のブロック** を持っています。

例えば、`layers.c`は **32 x 4ブロック** 、`game/invader.c`は **32 x 7ブロック** となっています。

また、各ブロックは **水平方向に6ピクセル、垂直方向に8ピクセル** の大きさです。

ここで「ブロック」、「行(row)」、「列(column)」などの用語に関して、以下に図示しておきます。このようなイメージです。

![](https://github.com/snagimmo/qmk_shooter/blob/main/images/fonts.png)

**注意:**

OLEDフォントの横方向のブロック数は、実は **32に固定されていません** ので、`font_block.c` の該当部分を書き換えることで変更可能です。

現在こちらの公式APIに関数が実装されている事が分かったのでソースコードを書き換え中です。少々お待ち下さい。

---

## OLEDフォントのブロック数について

OLEDディスプレイの水平/垂直方向のブロック数に関しては、`font_block.c` 内の `static` 変数で値を更新・保持しています。

また、描画時にスキップする余りピクセル数を `offset` として計算しています。

```c
static uint8_t hrzn_blocks = 0; // 水平方向のブロック数
static uint8_t vert_blocks = 0; // 垂直方向のブロック数
static uint8_t offset = 0;
```

サイズ **128 x 32 px** のSSD1306に関しては、下記の計算で `offset` は縦向き・横向きどちらの場合も **2px** になります。

```c
void set_rotation(uint8_t isVertical) {
    if (isVertical) {
        // 縦向き
        // 32px / フォント幅6 = ブロック数5
        hrzn_blocks = OLED_DISPLAY_HEIGHT / OLED_FONT_WIDTH;  // 5 blocks (128 x 32)
        // 剰余がオフセット2
        offset      = OLED_DISPLAY_HEIGHT % OLED_FONT_WIDTH;  // 2px (128 x 32)
        // 128px / フォント高さ8 = ブロック数16
        vert_blocks = OLED_DISPLAY_WIDTH  / OLED_FONT_HEIGHT; // 16 blocks (128 x 32)
    } else {
        // 横向き
        // 128px / フォント幅6 = ブロック数21
        hrzn_blocks = OLED_DISPLAY_WIDTH  / OLED_FONT_WIDTH;  // 21 blocks (128 x 32)
        // 剰余がオフセット2
        offset      = OLED_DISPLAY_WIDTH  % OLED_FONT_WIDTH;  // 2px (128 x 32)
        // 32px / フォント高さ8 = ブロック数4
        vert_blocks = OLED_DISPLAY_HEIGHT / OLED_FONT_HEIGHT; // 4 blocks (128 x 32)
    }
}
```

---

## 関数 `write_font_blocks()` の仕様について

これは私の `keymap.c` から引用してきた関数呼び出しの実例です。

```c
write_font_blocks(layer_char, 2, 5, 0x01, 0);
```

`write_font_blocks()`の中で、各パラメータは以下のような意味を持ちます。

- 1番目のパラメータは、16進数の配列（OLEDフォント配列）の先頭要素へのポインタです。 `const unsigned char*` 型です。
- 2番目は、読み込みたいフォントの **行数(row)** です。
- 3番目は、読み込みたいフォントの **列数(column)** です。
- 4番目は、OLEDフォント(例: `game/invader.c`, `layers.c`)からの **読み込みインデックス(read-index, rIdx)** です。
- 5番目は、OLEDバッファへの **書き込みインデックス(write-index, wIdx)** です。

要するに、上記の例は次のような意味になります。

> **`layer_char[]`** の **0x01** から **2**行 x **5**列のOLEDフォントを読み込み、OLEDディスプレイの **0**(一番左上の位置)から書き込む。

**注意:**

読み込みインデックスと書き込みインデックスを区別するために、 **読み込みインデックスのみ** を16進数で表現しています(内部的には同じですが)。

---

## 見た目の変更

OLED上で表示されるグラフィカルなフォントは、`.c` ソースファイル (例:`game/invader.c` や `layers.c`) 内に16進数の配列として実装されています。

```c
const unsigned char invader_font[] PROGMEM {
    0x00, ...
};
```

このグラフィカルフォント(配列)は以下のツールで改造することができます。

- [helix/FontConverter at master - MakotoKurauchi/helix](https://github.com/MakotoKurauchi/helix/tree/master/FontConverter)

このツールを使うと、ビットマップ画像(`.bmp`)を16進数の配列に変換することができます(その逆も可)。標準出力に対して出力結果が渡されるので、中間ファイルとして`.txt`などにしておくのがオススメです。

その後、別途用意した `YOUR_FONTFILE.c` 内の16進数配列 `THIS_IS_FONTNAME[]` をさきほど出力した16進数配列で置き換えることにより、独自のグラフィカルフォント(Helix系)を作ることが出来ます。

```c
const unsigned char THIS_IS_FONTNAME[] PROGMEM {
    // ここをFontConverterの出力結果で置き換える
    0x00, 0x80, 0xE0, ...
};
```

前述のように、HelixタイプのOLEDフォントは通常 **32ブロックの横幅** になっています。

このプログラムで読み込んでいるフォント配列 `layer_char[]` や `invader_font[]` は主に `keymap.c` や `drawing.c` の `write_font_blocks()` 関数の中で呼び出されます。コード中の配列名を変更する場合にはお手数ですが自力で書き換えて下さい。

参考用画像として `qmk_shooter/images` 内にいくつかビットマップ画像を置いておいたので、そちらをコピーしてペイントソフト等で編集後、上記ツールで変換してみて下さい。

また、特にQMKファームウェアのバージョンが0.14以上の場合、ファームウェアのサイズをあまり大きくしないように注意してください。

詳しくは、公式ドキュメント([QMK Firmware Docs](https://github.com/qmk/qmk_firmware))をご覧ください。
