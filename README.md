<div align="center">

<img src="https://readme-typing-svg.demolab.com?font=Fira+Code&weight=700&size=28&pause=1000&color=2E75B6&center=true&vCenter=true&width=700&lines=ATmega328P+Bootloader;Written+in+Sinhala+%F0%9F%87%B1%F0%9F%87%B0;From+Scratch+%E2%80%94+Step+by+Step" alt="Typing SVG" />

<br/>

# 🔧 ATmega328P Bootloader — සිංහලෙන්
### *Building an Optiboot-style UART Bootloader from Scratch*

<br/>

[![Language](https://img.shields.io/badge/Language-C%20%2F%20AVR-blue?style=for-the-badge&logo=c&logoColor=white)](https://gcc.gnu.org/wiki/avr-gcc)
[![Platform](https://img.shields.io/badge/Platform-ATmega328P-orange?style=for-the-badge&logo=arduino&logoColor=white)](https://www.microchip.com/en-us/product/atmega328p)
[![IDE](https://img.shields.io/badge/Tools-Arduino%20IDE%201.8.x-teal?style=for-the-badge&logo=arduino&logoColor=white)](https://www.arduino.cc/en/software)
[![OS](https://img.shields.io/badge/OS-Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white)](https://www.microsoft.com/windows)
[![License](https://img.shields.io/badge/License-MIT-green?style=for-the-badge)](LICENSE)
[![Tutorial](https://img.shields.io/badge/Tutorial-Sinhala%20%F0%9F%87%B1%F0%9F%87%B0-yellow?style=for-the-badge)](ATmega328P_Bootloader_Sinhala.docx)

<br/>

> **සිංහල භාෂාවෙන් ලියන ලද පළමු ATmega328P Bootloader නිබන්ධය**
> *The first ATmega328P Bootloader tutorial written entirely in Sinhala*

<br/>

---

</div>

## 📖 මෙය කුමක්ද? / What is This?

මෙය **Arduino Uno (ATmega328P)** සඳහා සම්පූර්ණයෙන්ම සිංහල භාෂාවෙන් ලියන ලද **Bootloader නිබන්ධයකි**. Optiboot ශෛලියේ UART bootloader එකක් ශුන්‍යයෙන් (from scratch) ගොඩනඟන ආකාරය — **සෑම පියවරක්ම** ATmega328P Datasheet සමග පැහැදිලි කරයි.

This is a **complete Bootloader tutorial** written in Sinhala for the **Arduino Uno (ATmega328P)**. It explains how to build an Optiboot-style UART bootloader from scratch — with **every step backed by the ATmega328P datasheet**.

<br/>

---

## ✨ විශේෂාංග / Features

| | Feature |
|---|---|
| 🇱🇰 | **Written in Sinhala** — possibly the first embedded systems tutorial of its kind |
| 📖 | **Datasheet-backed** — every register, every bit, referenced to the official ATmega328P datasheet |
| 🔨 | **Working code** — compiles clean, fits in 856 bytes (under the 1024 byte limit) |
| 🪟 | **Windows-friendly** — simple `.bat` scripts, no complex build systems |
| 🎓 | **Beginner-friendly** — explains concepts from first principles |
| ⚡ | **115200 baud UART** — STK500v1 protocol, compatible with Arduino IDE & Avrdude |

<br/>

---

## 📐 Bootloader Memory Layout

```
ATmega328P Flash — 32KB Total
┌──────────────────────────────────┐  ← 0x0000
│                                  │
│        APPLICATION SECTION       │
│          (31,744 bytes)          │
│       Your sketch goes here      │
│                                  │
├──────────────────────────────────┤  ← 0x7C00
│        BOOTLOADER SECTION        │
│          (1,024 bytes)           │
│        Our code lives here       │
└──────────────────────────────────┘  ← 0x7FFF
```

<br/>

---

## 📚 පරිච්ඡේද / Chapters

<details>
<summary><b>📗 Chapter 1 — How the ATmega328P Boots</b></summary>

- Program Counter (PC) and what it does
- Flash memory map — 0x0000 to 0x7FFF
- BOOTRST fuse — how the CPU decides where to start
- USBasp vs Arduino IDE upload — two scenarios explained
- Every startup flow, step by step

</details>

<details>
<summary><b>📗 Chapter 2 — Bootloader Section & Fuses</b></summary>

- BOOTSZ fuse — 4 possible bootloader sizes
- Why we choose 512 words (1024 bytes) at 0x7C00
- HFUSE = 0xDA — exactly which bits and why
- Lock bits = 0xEF — protecting our bootloader
- The 3 avrdude commands to set everything up

</details>

<details>
<summary><b>📗 Chapter 3 — Toolchain Setup (Windows)</b></summary>

- Arduino IDE 1.8.x tool paths
- `build.bat` — compiles main.c → bootloader.hex
- `flash.bat` — sets fuses, flashes, sets lock bits
- The one linker flag that places code at 0x7C00
- How to verify the output .hex file

</details>

<details>
<summary><b>📗 Chapter 4 — UART From Scratch</b></summary>

- How UART sends a byte (start/data/stop bits)
- Baud rate math — UBRR formula from the datasheet
- UBRR0, UCSR0B, UCSR0C — 3 registers explained bit by bit
- `uart_init()`, `uart_send()`, `uart_receive()` functions

</details>

<details>
<summary><b>📗 Chapter 5 — STK500v1 Protocol</b></summary>

- What Avrdude actually sends over the wire
- Command structure — every packet format
- All 10 commands we handle — GET_SYNC to LEAVE_PROGMODE
- The complete upload sequence from IDE to chip

</details>

<details>
<summary><b>📗 Chapter 6 — Flash Self-Programming</b></summary>

- Why you must write in 128-byte pages
- 3-step process — Erase → Fill buffer → Write
- SPM instruction and `<avr/boot.h>` macros
- RWW vs NRWW sections
- Timing — ~7.4ms per page

</details>

<details>
<summary><b>📗 Chapter 7 — Watchdog Timer & Safe Jump</b></summary>

- What the watchdog is and why it has its own oscillator
- 1-second upload window using WDTO_1S
- MCUSR register — detecting watchdog reset (WDRF)
- Why `goto 0x0000` is wrong — the watchdog reset trick
- `jump_to_app()` — the safe way to hand off to the sketch

</details>

<details>
<summary><b>📗 Chapter 8 — Complete Bootloader</b></summary>

- Full `main.c` — every piece assembled
- Build → verify → flash → test walkthrough
- Build output: **856 bytes** (fits comfortably in 1024 bytes ✅)
- Testing with Arduino IDE Blink sketch

</details>

<br/>

---

## 🚀 ඉක්මන් ආරම්භය / Quick Start

### Prerequisites
- Arduino IDE 1.8.x installed
- USBasp programmer
- ATmega328P board (Arduino Uno or compatible)

### 1. Clone the repo
```bash
git clone https://github.com/stark9000/Building-an-ATmega328P-Boot-loader-in-sinhala-language-.git
cd Building-an-ATmega328P-Boot-loader-in-sinhala-language-
```

### 2. Build
```batch
build.bat
```
Expected output:
```
[1/4] Compiling...
[2/4] Creating .hex file...
[3/4] Checking binary size...
Program: 856 bytes (2.6% Full) ✅
[4/4] Done! Output: build\bootloader.hex
```

### 3. Flash (connect USBasp first)
```batch
flash.bat
```
This will:
- Set fuses → `HFUSE = 0xDA`
- Flash the bootloader
- Set lock bits → `0xEF`

### 4. Test
Open Arduino IDE → Select **Arduino Uno** → Select COM port → Upload any sketch 🎉

<br/>

---

## 📁 Repository Structure

```
📦 Building-an-ATmega328P-Boot-loader-in-sinhala-language-
 ┣ 📂 buildFiles/
 ┃ ┣ 📄 main.c              ← Bootloader source code
 ┃ ┣ 📄 build.bat           ← Build script (Windows)
 ┃ ┗ 📄 flash.bat           ← Flash script (USBasp)
 ┣ 📂 edited pictures with numbers/
 ┃ ┗ 🖼️  Tutorial diagrams
 ┣ 📂 original pictures/
 ┃ ┗ 🖼️  Original reference images
 ┣ 📄 ATmega328P_Bootloader_Sinhala.docx  ← Full tutorial (Sinhala)
 ┗ 📄 README.md
```

<br/>

---

## ⚙️ Technical Specifications

| Parameter | Value |
|-----------|-------|
| **Target MCU** | ATmega328P |
| **CPU Clock** | 16 MHz |
| **Bootloader Size** | 856 bytes (limit: 1024 bytes) |
| **Boot Section Start** | `0x7C00` |
| **Protocol** | STK500v1 |
| **Baud Rate** | 115200 |
| **Frame Format** | 8N1 |
| **HFUSE** | `0xDA` |
| **Lock Byte** | `0xEF` |
| **Compatible With** | Arduino IDE, Avrdude |

<br/>

---

## 📋 Fuse Reference

| Fuse | Value | Meaning |
|------|-------|---------|
| `HFUSE` | `0xDA` | BOOTRST=active, BOOTSZ=512 words |
| `LOCK` | `0xEF` | App section cannot overwrite bootloader |

```bash
# Set fuses
avrdude -c usbasp -p m328p -U hfuse:w:0xDA:m

# Flash bootloader
avrdude -c usbasp -p m328p -U flash:w:bootloader.hex:i

# Set lock bits (do this LAST)
avrdude -c usbasp -p m328p -U lock:w:0xEF:m
```

<br/>

---

## 🙏 ස්තූතිය / Acknowledgements

- **Microchip Technology** — ATmega328P Datasheet
- **Optiboot Project** — inspiration for protocol and design decisions
- **Arduino Team** — AVR toolchain bundled with Arduino IDE

<br/>

---

<div align="center">

**සිංහල තාක්ෂණ ප්‍රජාව සඳහා සාදන ලදී** 🇱🇰

*Made for the Sinhala tech community*

<br/>

⭐ **If this helped you, please star the repo!** ⭐

<br/>

[![GitHub stars](https://img.shields.io/github/stars/stark9000/Building-an-ATmega328P-Boot-loader-in-sinhala-language-?style=social)](https://github.com/stark9000/Building-an-ATmega328P-Boot-loader-in-sinhala-language-/stargazers)
[![GitHub forks](https://img.shields.io/github/forks/stark9000/Building-an-ATmega328P-Boot-loader-in-sinhala-language-?style=social)](https://github.com/stark9000/Building-an-ATmega328P-Boot-loader-in-sinhala-language-/network/members)

</div>
