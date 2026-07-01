<div align="center">

<img width="100%" src="https://capsule-render.vercel.app/api?type=waving&color=0:0d1117,30:0a3d62,60:1a1a2e,100:16213e&height=260&section=header&text=Image%20Steganography&fontSize=52&fontColor=00d4ff&fontAlignY=45&desc=LSB%20Encoding%20%26%20Decoding%20of%20Secret%20Data%20in%20BMP%20Images%20—%20Pure%20C&descSize=18&descColor=a0cfee&descAlignY=65&animation=fadeIn&stroke=00d4ff&strokeWidth=1"/>

<img src="https://readme-typing-svg.demolab.com?font=Fira+Code&weight=700&size=19&duration=2500&pause=900&color=00D4FF&center=true&vCenter=true&width=680&lines=🔐+Hide+Secret+Files+Inside+BMP+Images;🖼️+LSB+—+Least+Significant+Bit+Encoding;🔍+Magic+String+Verification+for+Security;↩️+Decoder+Recovers+Original+Secret+Text;⚡+Built+in+Pure+C+—+No+External+Libraries" alt="Typing SVG" />

<br/>

![Language](https://img.shields.io/badge/Language-C-00599C?style=for-the-badge&logo=c&logoColor=white)
![Technique](https://img.shields.io/badge/Technique-LSB%20Steganography-00599C?style=for-the-badge&logo=c&logoColor=white)
![Build](https://img.shields.io/badge/Build-GCC-orange?style=for-the-badge&logo=gnu&logoColor=white)
![Status](https://img.shields.io/badge/Status-Complete-brightgreen?style=for-the-badge)

</div>

---

## 📌 Overview

**Image Steganography using LSB Encoding & Decoding** is a command-line tool written entirely in **C** that hides a secret text file inside a **BMP image** by replacing the **Least Significant Bit (LSB)** of each pixel byte with one bit of the secret message — without any visible change to the image.

The human eye cannot detect a 1-bit change in a pixel's colour value. This makes LSB steganography one of the most effective techniques for **covert data hiding** — and this project implements it from scratch using **bitwise operations**, **BMP file parsing**, **structs**, and **file I/O**.

Two modules are included:
- 🔐 **Encoder** — hides the secret file inside the image
- 🔓 **Decoder** — extracts and reconstructs the original secret file

---

## ✨ Features

| Feature | Description |
|---------|-------------|
| 🔐 **LSB Encoding** | Hides secret data bit-by-bit into image pixel bytes |
| 🔓 **LSB Decoding** | Extracts and reconstructs the original secret file |
| 🖼️ **BMP Parsing** | Reads and writes BMP file headers and pixel data correctly |
| 🧩 **Magic String** | Embeds a secret signature for safe identification during decode |
| 📏 **Size Embedding** | Stores secret file size inside image before the actual data |
| ✅ **File Validation** | Validates `.bmp` source and secret file extensions before processing |
| 🎯 **Zero Visual Change** | Output image looks identical to original — no quality loss |
| 🖥️ **CLI Driven** | All operations via command-line arguments |

---

## 📁 Project Structure

```
📁 image-steganography/
│
├── 📄 main.c              → Entry point, argument parsing, encode/decode dispatch
├── 📄 encode.c            → LSB encoding logic — hide secret into BMP
├── 📄 encode.h            → Encoder function declarations
├── 📄 decode.c            → LSB decoding logic — extract secret from BMP
├── 📄 decode.h            → Decoder function declarations
├── 📄 common.c            → Shared BMP read/write, magic string, bit operations
├── 📄 common.h            → Common structs, constants, shared declarations
├── 📄 utils.c             → File extension validation, argument checking
├── 📄 utils.h             → Utility declarations
├── 📄 Makefile            → Build automation
└── 📄 README.md           → You are here
```

---

## 🚀 Getting Started

### Prerequisites

```bash
# Verify GCC is installed
gcc --version

# Install on Ubuntu / Debian
sudo apt install gcc build-essential

# Install on Fedora / RHEL
sudo dnf install gcc make
```

### Build

```bash
# Clone the repository
git clone https://github.com/mithunhnnithun2119-ops/image-steganography.git
cd image-steganography

# Build with Makefile
make

# Or compile manually
gcc main.c encode.c decode.c common.c utils.c -o stego
```

---

## 🖥️ Usage

### Syntax

```bash
# Encoding — hide secret.txt inside source.bmp → output.bmp
./stego -e source.bmp secret.txt output.bmp

# Decoding — extract secret from stego.bmp → recovered.txt
./stego -d stego.bmp recovered.txt
```

### Options

| Option | Description | Example |
|--------|-------------|---------|
| `-e` | Encode — hide secret file into BMP | `./stego -e cover.bmp msg.txt stego.bmp` |
| `-d` | Decode — extract secret from BMP | `./stego -d stego.bmp output.txt` |
| `--help` | Show usage guide | `./stego --help` |

---

## 🎬 Demo

### Encoding a Secret File

```bash
$ ./stego -e cover.bmp secret.txt stego_output.bmp

✅ Source Image     : cover.bmp        (1920 x 1080 px)
✅ Secret File      : secret.txt       (2.3 KB)
✅ Magic String     : Embedded
✅ Bits Encoded     : 18,432
✅ Output Image     : stego_output.bmp

🔐 Encoding complete — secret hidden successfully!
```

### Decoding the Secret File

```bash
$ ./stego -d stego_output.bmp recovered.txt

✅ Stego Image      : stego_output.bmp
✅ Magic String     : Verified ✔
✅ Secret Size      : 2.3 KB
✅ Output File      : recovered.txt

🔓 Decoding complete — secret recovered successfully!
```

### Invalid File Handling

```bash
$ ./stego -e photo.jpg secret.txt out.bmp

❌ Error: Source image must be a .bmp file. Got: "photo.jpg"
```

### Magic String Mismatch

```bash
$ ./stego -d random.bmp output.txt

❌ Error: Magic string not found — this image contains no hidden data.
```

---

## 🧠 Technical Deep Dive

### What Is LSB Steganography?

Every pixel in a BMP image is stored as **RGB bytes** — each byte holds a value from `0` to `255`. Changing the **last 1 bit** of a byte changes its value by at most `1` — completely invisible to the human eye.

LSB steganography exploits this by **replacing the LSB** of each pixel byte with one bit of the secret message:

```
Original pixel byte :   1 1 0 1 0 1 1 0   →  214
Secret bit          :                   1
After encoding      :   1 1 0 1 0 1 1 1   →  215

Visible difference  :   NONE  (1 out of 255)
```

To hide 1 byte of secret data, we modify **8 pixel bytes** — one LSB per pixel byte.

### BMP File Structure

```
┌──────────────────────────────────────────────────┐
│              BMP FILE LAYOUT                     │
├────────────┬─────────────────────────────────────┤
│  Bytes     │  Content                            │
├────────────┼─────────────────────────────────────┤
│  0–1       │  Signature : "BM"                   │
│  2–5       │  File size (4 bytes)                │
│  6–9       │  Reserved  (4 bytes)                │
│  10–13     │  Pixel data offset                  │
├────────────┼─────────────────────────────────────┤
│  14–53     │  DIB Header (image info)            │
│            │  Width, Height, Bit depth, etc.     │
├────────────┼─────────────────────────────────────┤
│  offset+   │  Pixel Data  ← SECRET HIDDEN HERE  │
│            │  RGB bytes, row by row              │
└────────────┴─────────────────────────────────────┘
```

### Data Structures

```c
/* BMP file header — 14 bytes */
typedef struct {
    unsigned char  signature[2];    /* "BM"               */
    unsigned int   file_size;       /* Total file size     */
    unsigned int   reserved;        /* Always 0            */
    unsigned int   data_offset;     /* Pixel data offset   */
} BMPHeader;

/* BMP info header — 40 bytes */
typedef struct {
    unsigned int   header_size;     /* Always 40           */
    int            width;           /* Image width px      */
    int            height;          /* Image height px     */
    unsigned short color_planes;    /* Always 1            */
    unsigned short bits_per_pixel;  /* 24 for RGB          */
    unsigned int   compression;     /* 0 = no compression  */
    unsigned int   image_size;      /* Raw pixel data size */
} BMPInfoHeader;

/* Encode config — holds all operation parameters */
typedef struct {
    FILE          *src_image;       /* Source BMP file     */
    FILE          *secret_file;     /* Secret text file    */
    FILE          *stego_image;     /* Output BMP file     */
    char          *magic_string;    /* Verification string */
    unsigned int   secret_size;     /* Secret file size    */
} EncodeInfo;
```

### Encoding Logic — Bit by Bit

```c
/*
 * Encode one byte of secret data into 8 pixel bytes.
 * Replace LSB of each pixel byte with one secret bit.
 */
int encode_byte_to_lsb(char data, FILE *src, FILE *dest) {
    unsigned char pixel;

    for (int bit = 7; bit >= 0; bit--) {
        fread(&pixel, 1, 1, src);           /* Read one pixel byte   */
        pixel = (pixel & 0xFE) |            /* Clear LSB             */
                ((data >> bit) & 0x01);     /* Set LSB = secret bit  */
        fwrite(&pixel, 1, 1, dest);         /* Write modified pixel  */
    }
    return SUCCESS;
}
```

### Magic String Verification

```c
/*
 * Before encoding: embed magic string into image first.
 * Before decoding: read and verify magic string exists.
 * If magic string missing → image has no hidden data.
 */
#define MAGIC_STRING  "MHN_STEGO"
#define MAGIC_LEN      9

int encode_magic_string(FILE *src, FILE *dest) {
    for (int i = 0; i < MAGIC_LEN; i++)
        encode_byte_to_lsb(MAGIC_STRING[i], src, dest);
    return SUCCESS;
}

int verify_magic_string(FILE *stego) {
    char buf[MAGIC_LEN + 1];
    for (int i = 0; i < MAGIC_LEN; i++)
        buf[i] = decode_byte_from_lsb(stego);
    buf[MAGIC_LEN] = '\0';
    return (strcmp(buf, MAGIC_STRING) == 0);
}
```

### Decoding Logic — Reconstruct Secret

```c
/*
 * Decode one byte from 8 pixel bytes.
 * Extract the LSB of each pixel byte and reassemble.
 */
char decode_byte_from_lsb(FILE *stego) {
    unsigned char pixel;
    char          data = 0;

    for (int bit = 7; bit >= 0; bit--) {
        fread(&pixel, 1, 1, stego);         /* Read pixel byte        */
        data |= (pixel & 0x01) << bit;      /* Extract LSB, shift in  */
    }
    return data;
}
```

---

## ⚙️ Key Concepts Applied

| Concept | Application |
|---------|-------------|
| `Bitwise Operations` | LSB clear (`& 0xFE`), set (`\| bit`), extract (`& 0x01`) |
| `File I/O` (`fread`/`fwrite`/`fseek`) | Binary BMP read, pixel-by-pixel write |
| `Structs` | `BMPHeader`, `BMPInfoHeader`, `EncodeInfo` data models |
| `Pointers` | File handles, buffer passing, struct access |
| `Magic String` | Covert signature embedded before data for decoder verification |
| `Size Encoding` | Secret file size stored in image before payload for correct decode length |
| `File Validation` | Extension check on both source BMP and secret file |
| `Modular Programming` | Encode, decode, common, utils separated into clean modules |

---

## 🧪 Challenges & Solutions

```
❌ Challenge 1: Encoding data without visibly altering the image
✅ Solution   : Used LSB substitution — only the last bit of each pixel
                byte is modified, changing its value by at most 1 out
                of 255, which is completely imperceptible to the eye.

❌ Challenge 2: Knowing where secret data ends during decoding
✅ Solution   : Encoded the secret file size as a 32-bit integer into
                the image before the actual payload. The decoder reads
                this size first, then extracts exactly that many bytes.

❌ Challenge 3: Verifying that an image actually contains hidden data
✅ Solution   : Embedded a known magic string "MHN_STEGO" at the start
                of the encoded region. The decoder checks for it first;
                if missing, it reports no hidden data found.

❌ Challenge 4: BMP header offset varying across different BMP files
✅ Solution   : Read the pixel data offset from the BMP file header
                (bytes 10–13) dynamically using fseek(), instead of
                assuming a fixed offset value.

❌ Challenge 5: Capacity check — secret file too large for image
✅ Solution   : Before encoding, calculated max capacity as
                (image pixel bytes / 8) and compared against secret
                file size — rejected if secret was too large.
```

---

## 📈 Future Enhancements

- [ ] 🖼️ Support for PNG and JPEG formats via format detection
- [ ] 🔑 AES encryption of secret data before embedding
- [ ] 📊 Multi-bit LSB (2-bit, 4-bit) encoding for higher capacity
- [ ] 📂 Support for hiding any file type (PDF, image, binary)
- [ ] 🔬 Steganalysis resistance improvements
- [ ] 🖥️ GUI frontend using GTK or SDL

---

## 🎓 Learning Outcomes

Building this project gave me hands-on experience in:

- **Bitwise programming** — mastering `&`, `|`, `>>`, `<<` for real data manipulation
- **Binary file I/O** — working below text level with raw bytes via `fread`/`fwrite`
- **BMP format internals** — parsing a real binary file format from specification
- **Security concepts** — understanding covert channels and data hiding techniques
- **Struct-based design** — clean modelling of BMP headers and encode config
- **Modular C architecture** — encode, decode, common, utils as independent modules

---

## 👤 Author

<div align="center">

| Field | Details |
|-------|---------|
| **Name** | Mithun H N |
| **Degree** | B.E. ECE — Visvesvaraya Technological University (CGPA: 8.13) |
| **Training** | Advanced Embedded Systems — Emertxe, Bangalore |
| **Certification** | Skill India / NSDC — ELE/Q1501 (Embedded Engineer) |
| **LinkedIn** | [linkedin.com/in/mithun-hn](https://www.linkedin.com/in/mithun-hn) |
| **GitHub** | [github.com/mithunhnnithun2119-ops](https://github.com/mithunhnnithun2119-ops) |
| **Email** | mithunhnmithun2119@gmail.com |

</div>

---

<div align="center">

<img width="100%" src="https://capsule-render.vercel.app/api?type=waving&color=0:16213e,50:1a1a2e,100:0d1117&height=120&section=footer&animation=fadeIn"/>

*"The best hidden message is the one that looks like it was never there."* ⚡

![Visitor Count](https://komarev.com/ghpvc/?username=mithunhnnithun2119-ops&color=00d4ff&style=for-the-badge&label=PROFILE+VIEWS)

⭐ **Star this repo if it helped you!** ⭐

</div>
