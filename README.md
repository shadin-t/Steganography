# Steganography - Data Hiding in Images

This project implements LSB-based steganography in C to hide and extract secret messages within BMP image files. The message is encoded into the least significant bits of pixel data, making it invisible to the naked eye.

Developed during the C programming module at Emertxe.

## Features

- Hide any text message inside a .bmp image file
- Extract hidden messages from encoded images
- Simple CLI-based interface
- Works with uncompressed 24-bit BMP format

## Technologies Used

- C Programming
- Bitwise Operations
- File Handling (binary mode)
- Bitmap file format parsing


## Learning Outcomes

- Applied bit-level data embedding techniques
- Gained understanding of BMP image structure
- Practiced secure data manipulation using file pointers
