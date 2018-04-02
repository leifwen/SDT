//
//  ALG_BASE64.cpp
//  SDT
//
//  Created by Leif Wen on 21/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
#include "ALG_BASE64.h"
//------------------------------------------------------------------------------------------//
#ifdef ALG_BASE64_h
//#define LOGPRINT_ENABLE
#include "SYS_Log.h"
//------------------------------------------------------------------------------------------//
static const char e0[256] = {
 'A',  'A',  'A',  'A',  'B',  'B',  'B',  'B',  'C',  'C',
 'C',  'C',  'D',  'D',  'D',  'D',  'E',  'E',  'E',  'E',
 'F',  'F',  'F',  'F',  'G',  'G',  'G',  'G',  'H',  'H',
 'H',  'H',  'I',  'I',  'I',  'I',  'J',  'J',  'J',  'J',
 'K',  'K',  'K',  'K',  'L',  'L',  'L',  'L',  'M',  'M',
 'M',  'M',  'N',  'N',  'N',  'N',  'O',  'O',  'O',  'O',
 'P',  'P',  'P',  'P',  'Q',  'Q',  'Q',  'Q',  'R',  'R',
 'R',  'R',  'S',  'S',  'S',  'S',  'T',  'T',  'T',  'T',
 'U',  'U',  'U',  'U',  'V',  'V',  'V',  'V',  'W',  'W',
 'W',  'W',  'X',  'X',  'X',  'X',  'Y',  'Y',  'Y',  'Y',
 'Z',  'Z',  'Z',  'Z',  'a',  'a',  'a',  'a',  'b',  'b',
 'b',  'b',  'c',  'c',  'c',  'c',  'd',  'd',  'd',  'd',
 'e',  'e',  'e',  'e',  'f',  'f',  'f',  'f',  'g',  'g',
 'g',  'g',  'h',  'h',  'h',  'h',  'i',  'i',  'i',  'i',
 'j',  'j',  'j',  'j',  'k',  'k',  'k',  'k',  'l',  'l',
 'l',  'l',  'm',  'm',  'm',  'm',  'n',  'n',  'n',  'n',
 'o',  'o',  'o',  'o',  'p',  'p',  'p',  'p',  'q',  'q',
 'q',  'q',  'r',  'r',  'r',  'r',  's',  's',  's',  's',
 't',  't',  't',  't',  'u',  'u',  'u',  'u',  'v',  'v',
 'v',  'v',  'w',  'w',  'w',  'w',  'x',  'x',  'x',  'x',
 'y',  'y',  'y',  'y',  'z',  'z',  'z',  'z',  '0',  '0',
 '0',  '0',  '1',  '1',  '1',  '1',  '2',  '2',  '2',  '2',
 '3',  '3',  '3',  '3',  '4',  '4',  '4',  '4',  '5',  '5',
 '5',  '5',  '6',  '6',  '6',  '6',  '7',  '7',  '7',  '7',
 '8',  '8',  '8',  '8',  '9',  '9',  '9',  '9',  '+',  '+',
 '+',  '+',  '/',  '/',  '/',  '/'
};
static const char e1[256] = {
 'A',  'B',  'C',  'D',  'E',  'F',  'G',  'H',  'I',  'J',
 'K',  'L',  'M',  'N',  'O',  'P',  'Q',  'R',  'S',  'T',
 'U',  'V',  'W',  'X',  'Y',  'Z',  'a',  'b',  'c',  'd',
 'e',  'f',  'g',  'h',  'i',  'j',  'k',  'l',  'm',  'n',
 'o',  'p',  'q',  'r',  's',  't',  'u',  'v',  'w',  'x',
 'y',  'z',  '0',  '1',  '2',  '3',  '4',  '5',  '6',  '7',
 '8',  '9',  '+',  '/',  'A',  'B',  'C',  'D',  'E',  'F',
 'G',  'H',  'I',  'J',  'K',  'L',  'M',  'N',  'O',  'P',
 'Q',  'R',  'S',  'T',  'U',  'V',  'W',  'X',  'Y',  'Z',
 'a',  'b',  'c',  'd',  'e',  'f',  'g',  'h',  'i',  'j',
 'k',  'l',  'm',  'n',  'o',  'p',  'q',  'r',  's',  't',
 'u',  'v',  'w',  'x',  'y',  'z',  '0',  '1',  '2',  '3',
 '4',  '5',  '6',  '7',  '8',  '9',  '+',  '/',  'A',  'B',
 'C',  'D',  'E',  'F',  'G',  'H',  'I',  'J',  'K',  'L',
 'M',  'N',  'O',  'P',  'Q',  'R',  'S',  'T',  'U',  'V',
 'W',  'X',  'Y',  'Z',  'a',  'b',  'c',  'd',  'e',  'f',
 'g',  'h',  'i',  'j',  'k',  'l',  'm',  'n',  'o',  'p',
 'q',  'r',  's',  't',  'u',  'v',  'w',  'x',  'y',  'z',
 '0',  '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8',  '9',
 '+',  '/',  'A',  'B',  'C',  'D',  'E',  'F',  'G',  'H',
 'I',  'J',  'K',  'L',  'M',  'N',  'O',  'P',  'Q',  'R',
 'S',  'T',  'U',  'V',  'W',  'X',  'Y',  'Z',  'a',  'b',
 'c',  'd',  'e',  'f',  'g',  'h',  'i',  'j',  'k',  'l',
 'm',  'n',  'o',  'p',  'q',  'r',  's',  't',  'u',  'v',
 'w',  'x',  'y',  'z',  '0',  '1',  '2',  '3',  '4',  '5',
 '6',  '7',  '8',  '9',  '+',  '/'
};
static const uint32 d0[256] = {
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,	0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff,	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,	0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff,	0x01ffffff, 0x000000f8, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x000000fc,
	0x000000d0, 0x000000d4, 0x000000d8, 0x000000dc, 0x000000e0, 0x000000e4,	0x000000e8, 0x000000ec,
	0x000000f0, 0x000000f4, 0x01ffffff, 0x01ffffff,	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x00000000,	0x00000004, 0x00000008, 0x0000000c, 0x00000010, 0x00000014, 0x00000018,
	0x0000001c, 0x00000020, 0x00000024, 0x00000028, 0x0000002c, 0x00000030,	0x00000034, 0x00000038,
	0x0000003c, 0x00000040, 0x00000044, 0x00000048,	0x0000004c, 0x00000050, 0x00000054, 0x00000058,
	0x0000005c, 0x00000060,	0x00000064, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x00000068, 0x0000006c, 0x00000070, 0x00000074, 0x00000078,	0x0000007c, 0x00000080,
	0x00000084, 0x00000088, 0x0000008c, 0x00000090, 0x00000094, 0x00000098, 0x0000009c, 0x000000a0,
	0x000000a4, 0x000000a8, 0x000000ac, 0x000000b0, 0x000000b4, 0x000000b8, 0x000000bc, 0x000000c0,
	0x000000c4, 0x000000c8, 0x000000cc, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff
};
static const uint32 d1[256] = {
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x0000e003, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x0000f003,
	0x00004003, 0x00005003, 0x00006003, 0x00007003, 0x00008003, 0x00009003, 0x0000a003, 0x0000b003,
	0x0000c003, 0x0000d003, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x00000000, 0x00001000, 0x00002000, 0x00003000, 0x00004000, 0x00005000, 0x00006000,
	0x00007000, 0x00008000, 0x00009000, 0x0000a000, 0x0000b000, 0x0000c000, 0x0000d000, 0x0000e000,
	0x0000f000, 0x00000001, 0x00001001, 0x00002001, 0x00003001, 0x00004001, 0x00005001, 0x00006001,
	0x00007001, 0x00008001, 0x00009001, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x0000a001, 0x0000b001, 0x0000c001, 0x0000d001, 0x0000e001, 0x0000f001, 0x00000002,
	0x00001002, 0x00002002, 0x00003002, 0x00004002, 0x00005002, 0x00006002, 0x00007002, 0x00008002,
	0x00009002, 0x0000a002, 0x0000b002, 0x0000c002, 0x0000d002, 0x0000e002, 0x0000f002, 0x00000003,
	0x00001003, 0x00002003, 0x00003003, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff
};
static const uint32 d2[256] = {
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x00800f00, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x00c00f00,
	0x00000d00, 0x00400d00, 0x00800d00, 0x00c00d00, 0x00000e00, 0x00400e00, 0x00800e00, 0x00c00e00,
	0x00000f00, 0x00400f00, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x00000000, 0x00400000, 0x00800000, 0x00c00000, 0x00000100, 0x00400100, 0x00800100,
	0x00c00100, 0x00000200, 0x00400200, 0x00800200, 0x00c00200, 0x00000300, 0x00400300, 0x00800300,
	0x00c00300, 0x00000400, 0x00400400, 0x00800400, 0x00c00400, 0x00000500, 0x00400500, 0x00800500,
	0x00c00500, 0x00000600, 0x00400600, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x00800600, 0x00c00600, 0x00000700, 0x00400700, 0x00800700, 0x00c00700, 0x00000800,
	0x00400800, 0x00800800, 0x00c00800, 0x00000900, 0x00400900, 0x00800900, 0x00c00900, 0x00000a00,
	0x00400a00, 0x00800a00, 0x00c00a00, 0x00000b00, 0x00400b00, 0x00800b00, 0x00c00b00, 0x00000c00,
	0x00400c00, 0x00800c00, 0x00c00c00, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff
};
static const uint32 d3[256] = {
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x003e0000, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x003f0000,
	0x00340000, 0x00350000, 0x00360000, 0x00370000, 0x00380000, 0x00390000, 0x003a0000, 0x003b0000,
	0x003c0000, 0x003d0000, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x00000000, 0x00010000, 0x00020000, 0x00030000, 0x00040000, 0x00050000, 0x00060000,
	0x00070000, 0x00080000, 0x00090000, 0x000a0000, 0x000b0000, 0x000c0000, 0x000d0000, 0x000e0000,
	0x000f0000, 0x00100000, 0x00110000, 0x00120000, 0x00130000, 0x00140000, 0x00150000, 0x00160000,
	0x00170000, 0x00180000, 0x00190000, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x001a0000, 0x001b0000, 0x001c0000, 0x001d0000, 0x001e0000, 0x001f0000, 0x00200000,
	0x00210000, 0x00220000, 0x00230000, 0x00240000, 0x00250000, 0x00260000, 0x00270000, 0x00280000,
	0x00290000, 0x002a0000, 0x002b0000, 0x002c0000, 0x002d0000, 0x002e0000, 0x002f0000, 0x00300000,
	0x00310000, 0x00320000, 0x00330000, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
void ALG_B64_Init(ALG_BASE64_CTX* ctx,uint32 cfg){
	ctx->next_in = nullptr;
	ctx->avail_in = 0;
	ctx->total_in = 0;
	
	ctx->next_out = nullptr;
	ctx->avail_out = 0;
	ctx->total_out = 0;
	
	ctx->status = 0;
	
	ctx->cfg = cfg;
	ctx->outMinSize = B_ChkFLAG32(cfg,ALG_BASE64::CFG_NL) ? 5 : 4;
	ctx->in64 = 0;
	ctx->unDoNum = 0;
}
//------------------------------------------------------------------------------------------//
bool32 ALG_B64_ReInit(ALG_BASE64_CTX* ctx){
	ctx->in64 = 0;
	ctx->unDoNum = 0;
	return G_TRUE;
}
//------------------------------------------------------------------------------------------//
bool32 ALG_B64_Release(ALG_BASE64_CTX* ctx){
	return(ALG_B64_ReInit(ctx));
}
//------------------------------------------------------------------------------------------//
bool32 ALG_B64_EnCalc(ALG_BASE64_CTX* ctx,uint8*& _out,uint8*& _in){
	uint8	t1, t2, t3;
	
	while (ctx->avail_in > 2){
		if (ctx->avail_out < ctx->outMinSize)
			return BASE64_NOMEM;
		t1 = *_in++;
		t2 = *_in++;
		t3 = *_in++;
		
		ctx->total_in += 3;
		ctx->avail_in -= 3;
		
		*_out++ = e0[t1];
		*_out++ = e1[((t1 & 0x03) << 4) | ((t2 >> 4) & 0x0F)];
		*_out++ = e1[((t2 & 0x0F) << 2) | ((t3 >> 6) & 0x03)];
		*_out++ = e1[t3];
		
		ctx->total_out += 4;
		ctx->avail_out -= 4;
		
		if (B_ChkFLAG32(ctx->cfg,ALG_BASE64::CFG_NL)){
			ctx->in64 += 4;
			if (ctx->in64 == 64){
				*_out++ = '\n';
				ctx->in64 = 0;
				++ ctx->total_out;
				-- ctx->avail_out;
			}
		}
	};
	return BASE64_OK;
}
//------------------------------------------------------------------------------------------//
bool32 ALG_B64_EnUpdate(ALG_BASE64_CTX* ctx,uint8* _out,const uint64& outSize,const uint8* data,const uint64& length){
	bool32	err = BASE64_OK;
	uint64	total_in,avail_in;
	uint8*	buf = ctx->buf;

	ctx->next_in = (uint8*)data;
	ctx->avail_in = length;
	ctx->next_out = _out;
	ctx->avail_out = outSize;
	
	while((ctx->unDoNum > 0) && (ctx->avail_in > 0)){
		if (ctx->unDoNum < 3){
			ctx->buf[ctx->unDoNum++] = *ctx->next_in++;
			++ ctx->total_in;
			-- ctx->avail_in;
		}
		if (ctx->unDoNum == 3){
			ctx->unDoNum = 0;
			total_in = ctx->total_in;
			avail_in = ctx->avail_in;
			ctx->avail_in = 3;
			err = ALG_B64_EnCalc(ctx, ctx->next_out, buf);
			ctx->total_in = total_in;
			ctx->avail_in = avail_in;
			if (err == BASE64_NOMEM){
				ctx->unDoNum = 3;
				return BASE64_NOMEM;
			}
			break;
		}
	};
	if (ctx->avail_in == 0)
		return BASE64_OK;
	
	if (ctx->avail_in > 2)
		err = ALG_B64_EnCalc(ctx, ctx->next_out, ctx->next_in);
	ctx->unDoNum = 0;
	switch(ctx->avail_in){
		case 0:;
			ctx->unDoNum = 0;
			break;
		case 1:;
			ctx->buf[0] = *ctx->next_in++;
			ctx->unDoNum = 1;
			break;
		case 2:;
			ctx->buf[0] = *ctx->next_in++;
			ctx->buf[1] = *ctx->next_in++;
			ctx->unDoNum = 2;
			break;
		case 3:;
			ctx->buf[0] = *ctx->next_in++;
			ctx->buf[1] = *ctx->next_in++;
			ctx->buf[2] = *ctx->next_in++;
			ctx->unDoNum = 3;
			break;
		default:;
	}
	ctx->total_in += ctx->unDoNum;
	ctx->avail_in -= ctx->unDoNum;
	return(err);
}
//------------------------------------------------------------------------------------------//
bool32 ALG_B64_EnFinal(ALG_BASE64_CTX* ctx,uint8* _out,const uint64& outSize){

	ctx->avail_in = 0;
	ctx->next_out = _out;
	ctx->avail_out = outSize;

	if (ctx->avail_out < ctx->outMinSize)
		return BASE64_NOMEM;

	switch (ctx->unDoNum) {
		case 0:
			if (B_ChkFLAG32(ctx->cfg,ALG_BASE64::CFG_NL) && (ctx->in64 < 64)){
				*ctx->next_out = '\n';
				++ ctx->total_out;
				-- ctx->avail_out;
			}
			break;
		case 1:
			*ctx->next_out++ = e0[ctx->buf[0]];
			*ctx->next_out++ = e1[(ctx->buf[0] & 0x03) << 4];
			*ctx->next_out++ = '=';
			*ctx->next_out++ = '=';
			ctx->total_out += 4;
			ctx->avail_out -= 4;
			if (B_ChkFLAG32(ctx->cfg,ALG_BASE64::CFG_NL)){
				*ctx->next_out = '\n';
				++ ctx->total_out;
				-- ctx->avail_out;
			}
			break;
		case 2:;
		default:
			*ctx->next_out++ = e0[ctx->buf[0]];
			*ctx->next_out++ = e1[((ctx->buf[0] & 0x03) << 4) | ((ctx->buf[1] >> 4) & 0x0F)];
			*ctx->next_out++ = e1[(ctx->buf[1] & 0x0F) << 2];
			*ctx->next_out++ = '=';
			ctx->total_out += 4;
			ctx->avail_out -= 4;
			if (B_ChkFLAG32(ctx->cfg,ALG_BASE64::CFG_NL)){
				*ctx->next_out = '\n';
				++ ctx->total_out;
				-- ctx->avail_out;
			}
	}
	return BASE64_FINISH;
}
//------------------------------------------------------------------------------------------//
bool32 ALG_B64_DeCalc(ALG_BASE64_CTX* ctx,uint8*& _out,uint8*& _in){
	uint32	x;
	uint8	x2,x3;

	while (ctx->avail_in > 3){
		if (*_in == '\n'){
			++ ctx->total_in;
			-- ctx->avail_in;
			++ _in;
			continue;
		}

		x  = d0[*_in++];
		x |= d1[*_in++];
		x2 = *_in++;
		x3 = *_in++;
		
		if (x2 == '='){
			if (ctx->avail_out < 1){
				_in -= 4;
				return BASE64_NOMEM;
			}
			*_out++ = (uint8)x;
			++ ctx->total_out;
			-- ctx->avail_out;
		}
		else if (x3 == '='){
			if (ctx->avail_out < 2){
				_in -= 4;
				return BASE64_NOMEM;
			}

			x |= d2[x2];
			*_out++ = (uint8)x;
			*_out++ = (uint8)(x >> 8);
			ctx->total_out += 2;
			ctx->avail_out -= 2;
		}
		else{
			if (ctx->avail_out < 3){
				_in -= 4;
				return BASE64_NOMEM;
			}
			x |= d2[x2];
			x |= d3[x3];
			*_out++ = (uint8)x;
			*_out++ = (uint8)(x >> 8);
			*_out++ = (uint8)(x >> 16);
			ctx->total_out += 3;
			ctx->avail_out -= 3;
		}
		ctx->total_in += 4;
		ctx->avail_in -= 4;
	}
	if ((ctx->avail_in > 0) && (*_in == '\n')){
		++ ctx->total_in;
		-- ctx->avail_in;
		++ _in;
	}
	return BASE64_OK;
}
//------------------------------------------------------------------------------------------//
bool32 ALG_B64_DeUpdate(ALG_BASE64_CTX* ctx,uint8* _out,const uint64& outSize,const uint8* data,const uint64& length){
	bool32	err = BASE64_OK;
	uint64	total_in,avail_in;
	uint8*	buf = ctx->buf;
	
	ctx->next_in = (uint8*)data;
	ctx->avail_in = length;
	ctx->next_out = _out;
	ctx->avail_out = outSize;
	
	while((ctx->unDoNum > 0) && (ctx->avail_in > 0)){
		if (ctx->unDoNum < 4){
			ctx->buf[ctx->unDoNum++] = *ctx->next_in++;
			++ ctx->total_in;
			-- ctx->avail_in;
		}
		if (ctx->unDoNum == 4){
			ctx->unDoNum = 0;
			total_in = ctx->total_in;
			avail_in = ctx->avail_in;
			ctx->avail_in = 4;
			err = ALG_B64_DeCalc(ctx, ctx->next_out, buf);
			ctx->total_in = total_in;
			ctx->avail_in = avail_in;
			if (err == BASE64_NOMEM){
				ctx->unDoNum = 4;
				return BASE64_NOMEM;
			}
			break;
		}
	};
	if (ctx->avail_in == 0)
		return BASE64_OK;
	
	if (ctx->avail_in > 3)
		err = ALG_B64_DeCalc(ctx, ctx->next_out, ctx->next_in);
	ctx->unDoNum = 0;
	switch(ctx->avail_in){
		case 0:;
			ctx->unDoNum = 0;
			break;
		case 1:;
			ctx->buf[0] = *ctx->next_in++;
			ctx->unDoNum = 1;
			break;
		case 2:;
			ctx->buf[0] = *ctx->next_in++;
			ctx->buf[1] = *ctx->next_in++;
			ctx->unDoNum = 2;
			break;
		case 3:;
			ctx->buf[0] = *ctx->next_in++;
			ctx->buf[1] = *ctx->next_in++;
			ctx->buf[2] = *ctx->next_in++;
			ctx->unDoNum = 3;
			break;
		case 4:;
			ctx->buf[0] = *ctx->next_in++;
			ctx->buf[1] = *ctx->next_in++;
			ctx->buf[2] = *ctx->next_in++;
			ctx->buf[3] = *ctx->next_in++;
			ctx->unDoNum = 4;
			break;
		default:;
	}
	ctx->total_in += ctx->unDoNum;
	ctx->avail_in -= ctx->unDoNum;
	return(err);
}
//------------------------------------------------------------------------------------------//
bool32 ALG_B64_DeFinal(ALG_BASE64_CTX* ctx,uint8* _out,const uint64& outSize){
	bool32 err = BASE64_OK;
	uint64	total_in;
	uint8*	buf = ctx->buf;
	
	ctx->avail_in = 0;
	ctx->next_out = _out;
	ctx->avail_out = outSize;
	
	while(ctx->unDoNum > 1){
		if (ctx->unDoNum < 4)
			ctx->buf[ctx->unDoNum++] = '=';
		if (ctx->unDoNum == 4){
			ctx->unDoNum = 0;
			total_in = ctx->total_in;
			ctx->avail_in = 4;
			err = ALG_B64_DeCalc(ctx, ctx->next_out, buf);
			ctx->total_in = total_in;
			ctx->avail_in = 0;
			if (err == BASE64_NOMEM){
				ctx->unDoNum = 4;
				return BASE64_NOMEM;
			}
			break;
		}
	};
	return BASE64_FINISH;
};
//------------------------------------------------------------------------------------------//
bool32 ALG_B64_Update(ALG_BASE64_CTX* ctx,uint8* _out,const uint64& outSize,const uint8* data,const uint64& length){
	if (B_ChkFLAG32(ctx->cfg, ALG_BASE64::CFG_Encode))
		return(ALG_B64_EnUpdate(ctx, _out, outSize, data, length));
	return(ALG_B64_DeUpdate(ctx, _out, outSize, data, length));
}
//------------------------------------------------------------------------------------------//
bool32 ALG_B64_Final(ALG_BASE64_CTX* ctx,uint8* _out,const uint64& outSize){
	if (B_ChkFLAG32(ctx->cfg, ALG_BASE64::CFG_Encode))
		return(ALG_B64_EnFinal(ctx, _out, outSize));
	return(ALG_B64_DeFinal(ctx, _out, outSize));
}
//------------------------------------------------------------------------------------------//
static inline void ALG_B64_Init(void* ctx,uint32 cfg,const void* p){
	ALG_B64_Init((ALG_BASE64_CTX*)ctx,cfg);
	ELog(((ALG_BASE64_CTX*)ctx)->dstf << "Init():   "
		 << (B_ChkFLAG32(((ALG_BASE64_CTX*)ctx)->cfg, ALG_BASE64::CFG_Encode) ? "Encode" : "Decode"));
};
//------------------------------------------------------------------------------------------//
static inline bool32 ALG_B64_Update(void* ctx,uint8* _out,const uint64& outSize,const uint8* data,const uint64& length){
	ELog(((ALG_BASE64_CTX*)ctx)->dstf << "Update(): "
		 << (B_ChkFLAG32(((ALG_BASE64_CTX*)ctx)->cfg, ALG_BASE64::CFG_Encode) ? "Encode" : "Decode") << "    ,len=" << length);
	return(ALG_B64_Update((ALG_BASE64_CTX*)ctx, _out, outSize, data, length));
}
//------------------------------------------------------------------------------------------//
static inline bool32 ALG_B64_Final(void* ctx,uint8* _out,const uint64& outSize,const uint8* data,const uint64& length){
	ELog(((ALG_BASE64_CTX*)ctx)->dstf << "Final():  "
		 << (B_ChkFLAG32(((ALG_BASE64_CTX*)ctx)->cfg, ALG_BASE64::CFG_Encode) ? "Encode" : "Decode"));
	return(ALG_B64_Final((ALG_BASE64_CTX*)ctx,_out,outSize));
};
//------------------------------------------------------------------------------------------//
static inline bool32 ALG_B64_Release(void* ctx){
	ELog(((ALG_BASE64_CTX*)ctx)->dstf << "Release()");
	ALG_B64_Release((ALG_BASE64_CTX*)ctx);
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
static inline bool32 ALG_B64_ReInit(void* ctx){
	ELog(((ALG_AES_CTX*)ctx)->dstf << "ReInit()");
	ALG_B64_ReInit((ALG_BASE64_CTX*)ctx);
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
ALG_BASE64::ALG_BASE64(void) : DSTF_DIR(){
	cgCTX.Init		= ALG_B64_Init;
	cgCTX.Update	= ALG_B64_Update;
	cgCTX.Final		= ALG_B64_Final;
	cgCTX.Release	= ALG_B64_Release;
	cgCTX.ReInit	= ALG_B64_ReInit;
	TNFP::SetSelfName("B64");
}
//------------------------------------------------------------------------------------------//
ALG_BASE64::~ALG_BASE64(void){
	ALG_B64_Release(&cgCTX);
}
//------------------------------------------------------------------------------------------//
#endif
