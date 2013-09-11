/*
 *
 * Corsiva_12
 *
 * created with FontCreator
 * written by F. Maximilian Thiele
 *
 * http://www.apetech.de/fontCreator
 * me@apetech.de
 *
 * File Name           : corsiva_12.h
 * Date                : 29.01.2005
 * Font size in bytes  : 5690
 * Font width          : 10
 * Font height         : 11
 * Font first char     : 32
 * Font last char      : 128
 * Font used chars     : 96
 *
 * The font data are defined as
 *
 * struct _FONT_ {
 *     uint16_t   font_Size_in_Bytes_over_all_included_Size_it_self;
 *     uint8_t    font_Width_in_Pixel_for_fixed_drawing;
 *     uint8_t    font_Height_in_Pixel_for_all_characters;
 *     unit8_t    font_First_Char;
 *     uint8_t    font_Char_Count;
 *
 *     uint8_t    font_Char_Widths[font_Last_Char - font_First_Char +1];
 *                  // for each character the separate width in pixels,
 *                  // characters < 128 have an implicit virtual right empty row
 *
 *     uint8_t    font_data[];
 *                  // bit field of all characters
 */

#include <inttypes.h>
#include <avr/pgmspace.h>

#ifndef CORSIVA_12_H
#define CORSIVA_12_H

#define CORSIVA_12_WIDTH 10
#define CORSIVA_12_HEIGHT 11

static const uint8_t Corsiva_12[] PROGMEM = {
    0x16, 0x3A, // size
    0x0A, // width
    0x0B, // height
    0x20, // first char
    0x60, // char count
    
    // char widths
    0x03, 0x02, 0x02, 0x06, 0x05, 0x07, 0x09, 0x01, 0x03, 0x04, 
    0x02, 0x05, 0x02, 0x03, 0x01, 0x05, 0x05, 0x04, 0x05, 0x04, 
    0x05, 0x06, 0x05, 0x05, 0x05, 0x05, 0x02, 0x03, 0x05, 0x05, 
    0x05, 0x04, 0x07, 0x07, 0x07, 0x06, 0x08, 0x07, 0x07, 0x07, 
    0x0A, 0x05, 0x06, 0x09, 0x07, 0x09, 0x0A, 0x06, 0x07, 0x09, 
    0x09, 0x06, 0x08, 0x08, 0x08, 0x0B, 0x09, 0x09, 0x07, 0x03, 
    0x03, 0x04, 0x03, 0x06, 0x02, 0x04, 0x04, 0x04, 0x06, 0x04, 
    0x06, 0x06, 0x04, 0x03, 0x04, 0x06, 0x03, 0x06, 0x04, 0x04, 
    0x05, 0x05, 0x04, 0x04, 0x04, 0x04, 0x06, 0x08, 0x07, 0x05, 
    0x06, 0x03, 0x01, 0x04, 0x05, 0x06, 
    
    // font data
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 32
    0x80, 0x3E, 0x00, 0x00, // 33
    0x08, 0x06, 0x00, 0x00, // 34
    0xA0, 0x78, 0xAE, 0x78, 0x2E, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 35
    0x80, 0x8C, 0xF2, 0xBF, 0x62, 0x00, 0x20, 0x00, 0x00, 0x00, // 36
    0x1C, 0x92, 0x7E, 0x0C, 0xE2, 0x90, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 37
    0x70, 0xC8, 0xA8, 0xB8, 0x6C, 0xBE, 0x02, 0xC2, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, // 38
    0x08, 0x00, // 39
    0xE0, 0x18, 0x06, 0x60, 0x80, 0x00, // 40
    0x00, 0x00, 0xC3, 0x3C, 0x40, 0x20, 0x00, 0x00, // 41
    0x0C, 0x0F, 0x00, 0x00, // 42
    0x10, 0x10, 0x7C, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, // 43
    0x00, 0x80, 0x40, 0x20, // 44
    0x20, 0x20, 0x20, 0x00, 0x00, 0x00, // 45
    0x80, 0x00, // 46
    0x00, 0xC0, 0x30, 0x0C, 0x03, 0x60, 0x00, 0x00, 0x00, 0x00, // 47
    0x78, 0x84, 0x82, 0x42, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, // 48
    0x80, 0x88, 0xFC, 0x86, 0x00, 0x00, 0x00, 0x00, // 49
    0xC0, 0xC4, 0xA2, 0x92, 0x4E, 0x00, 0x00, 0x00, 0x00, 0x00, // 50
    0xC0, 0x80, 0xDA, 0x76, 0x00, 0x00, 0x00, 0x00, // 51
    0x20, 0x30, 0xA8, 0x7E, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, // 52
    0x80, 0x80, 0x84, 0x4A, 0x32, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 53
    0x70, 0xAC, 0x94, 0x72, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, // 54
    0xC4, 0x32, 0x0A, 0x06, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, // 55
    0xE0, 0xAC, 0x92, 0x6A, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, // 56
    0x80, 0x9C, 0x52, 0x72, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, // 57
    0x80, 0x08, 0x00, 0x00, // 58
    0x00, 0x80, 0x08, 0x40, 0x20, 0x00, // 59
    0x30, 0x30, 0x48, 0x48, 0x48, 0x00, 0x00, 0x00, 0x00, 0x00, // 60
    0x28, 0x28, 0x28, 0x28, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, // 61
    0x48, 0x48, 0x48, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, // 62
    0x80, 0x36, 0x12, 0x0C, 0x00, 0x00, 0x00, 0x00, // 63
    0x70, 0xCC, 0xB4, 0xAA, 0xBA, 0x62, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 64
    0x00, 0x80, 0x4C, 0x32, 0x2E, 0xFE, 0x82, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 65
    0x38, 0x24, 0x86, 0xFA, 0x8E, 0x8A, 0x76, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 66
    0xF0, 0x8C, 0x04, 0x02, 0x82, 0x0C, 0x00, 0x20, 0x20, 0x20, 0x00, 0x00, // 67
    0x38, 0x4C, 0x86, 0xEA, 0x9A, 0x82, 0x46, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 68
    0x38, 0x84, 0xE2, 0x9E, 0x92, 0xBA, 0xC6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 69
    0x1C, 0x86, 0xE2, 0x9E, 0x12, 0x3A, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 70
    0x70, 0x8C, 0x84, 0x82, 0xD2, 0x32, 0x16, 0x00, 0x80, 0x80, 0xC0, 0x20, 0x00, 0x00, // 71
    0xC0, 0x8C, 0xC2, 0x3A, 0x16, 0x90, 0xF8, 0x86, 0x01, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 72
    0x80, 0xC0, 0xFA, 0x06, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, // 73
    0x00, 0x00, 0x0C, 0xC2, 0x3A, 0x06, 0x60, 0x40, 0x40, 0x20, 0x00, 0x00, // 74
    0x8C, 0x82, 0xF2, 0x1E, 0x78, 0x88, 0x06, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x40, 0x40, 0x40, // 75
    0x80, 0xC0, 0xB8, 0x84, 0x82, 0x82, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 76
    0x80, 0x9C, 0x62, 0x1E, 0xF8, 0x60, 0x90, 0xFC, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 77
    0x80, 0x0C, 0xE2, 0x12, 0x0E, 0xF0, 0x70, 0x0C, 0x02, 0x02, 0x20, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 78
    0x78, 0xC4, 0x82, 0x82, 0x46, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 79
    0x38, 0xCC, 0x86, 0xFA, 0x92, 0x12, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 80
    0x78, 0xC4, 0x82, 0x82, 0x42, 0x26, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x60, 0xC0, 0x80, 0x80, 0x40, // 81
    0x38, 0xA4, 0x86, 0xFA, 0x32, 0xD2, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x40, 0x40, // 82
    0x80, 0x00, 0x0C, 0x92, 0xE2, 0x0E, 0x00, 0x20, 0x20, 0x20, 0x00, 0x00, // 83
    0x1C, 0x86, 0x82, 0xF2, 0x8E, 0x02, 0x02, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 84
    0x0C, 0x7A, 0x86, 0x80, 0x80, 0x40, 0x30, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 85
    0x0C, 0x06, 0xFE, 0x42, 0x20, 0x10, 0x0C, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 86
    0x0C, 0x06, 0xFE, 0x32, 0x08, 0x04, 0xFE, 0x20, 0x10, 0x08, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 87
    0x00, 0x80, 0x40, 0x22, 0x1E, 0xF4, 0x82, 0x01, 0x01, 0x20, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 88
    0x1C, 0x04, 0x82, 0xC6, 0xBC, 0x8C, 0x02, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 89
    0x80, 0xC0, 0xA6, 0x92, 0x8A, 0x86, 0xC2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 90
    0x00, 0xF0, 0x0F, 0xC0, 0xA0, 0x00, // 91
    0x07, 0xFC, 0x80, 0x00, 0x00, 0x60, // 92
    0x00, 0x00, 0xF1, 0x0F, 0x80, 0xE0, 0x00, 0x00, // 93
    0x04, 0x02, 0x04, 0x00, 0x00, 0x00, // 94
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, // 95
    0x01, 0x02, 0x00, 0x00, // 96
    0xE0, 0x90, 0xC8, 0xB8, 0x00, 0x00, 0x00, 0x00, // 97
    0xE0, 0x9E, 0x49, 0x39, 0x00, 0x00, 0x00, 0x00, // 98
    0xE0, 0x90, 0x88, 0x48, 0x00, 0x00, 0x00, 0x00, // 99
    0xE0, 0x90, 0x48, 0xF8, 0x4E, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 100
    0xF0, 0xB0, 0xA8, 0x58, 0x00, 0x00, 0x00, 0x00, // 101
    0x00, 0x90, 0x78, 0x16, 0x01, 0x01, 0x40, 0x20, 0x00, 0x00, 0x00, 0x00, // 102
    0x00, 0xE0, 0x90, 0x88, 0xE8, 0x18, 0xE0, 0x80, 0x80, 0xC0, 0x60, 0x00, // 103
    0xE0, 0x1E, 0xD1, 0xB9, 0x00, 0x00, 0x00, 0x00, // 104
    0xE0, 0x38, 0x02, 0x00, 0x00, 0x00, // 105
    0x00, 0x90, 0x78, 0x02, 0x80, 0x60, 0x00, 0x00, // 106
    0xE0, 0x3E, 0xE9, 0x19, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x40, 0x40, // 107
    0xE0, 0x9C, 0x42, 0x00, 0x00, 0x00, // 108
    0xC8, 0x38, 0xD0, 0x38, 0xF0, 0x98, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 109
    0xE8, 0x18, 0xD0, 0xB8, 0x00, 0x00, 0x00, 0x00, // 110
    0xF0, 0x88, 0xC8, 0x38, 0x00, 0x00, 0x00, 0x00, // 111
    0x88, 0xF8, 0x90, 0x48, 0x38, 0xE0, 0x80, 0x00, 0x00, 0x00, // 112
    0xE0, 0x90, 0x88, 0xE8, 0x18, 0x00, 0x80, 0xC0, 0xA0, 0x00, // 113
    0xC8, 0x38, 0x10, 0x08, 0x00, 0x00, 0x00, 0x00, // 114
    0xC0, 0x98, 0xE8, 0x08, 0x00, 0x00, 0x00, 0x00, // 115
    0xE8, 0x9C, 0x88, 0x40, 0x00, 0x00, 0x00, 0x00, // 116
    0xE8, 0x98, 0xE0, 0x98, 0x00, 0x00, 0x00, 0x00, // 117
    0x10, 0x08, 0xF8, 0x88, 0x60, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 118
    0x10, 0x08, 0xF8, 0x88, 0x40, 0xF8, 0x40, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 119
    0x80, 0xD0, 0x58, 0xF0, 0x10, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x40, 0x60, // 120
    0x10, 0x18, 0xF0, 0xC0, 0x38, 0x80, 0x40, 0x20, 0x00, 0x00, // 121
    0x80, 0xD8, 0xA8, 0x98, 0xD8, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 122
    0xA0, 0xD8, 0x07, 0xE0, 0x80, 0x00, // 123
    0xFF, 0xE0, // 124
    0x00, 0x00, 0xD1, 0x2F, 0x80, 0xC0, 0x20, 0x00, // 125
    0x08, 0x08, 0x18, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, // 126
    0xFF, 0x81, 0x81, 0x81, 0x81, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 // 127
    
};

#endif
