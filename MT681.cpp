/*
    Library that simulates the behavior of an ISKRA MT681 smart meter
    Copyright (C) 2016 Dominic Prass

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 2
    of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include <stdio.h>

#include "MT681.h"

uint8_t MT681::message[SML_MESSAGE_SIZE] = {
    0x1b, 0x1b, 0x1b, 0x1b, 0x01, 0x01, 0x01, 0x01, 0x76, 0x05, 0x00, 0x01, 0x35, 0x52,
    0x62, 0x00, 0x62, 0x00, 0x72, 0x63, 0x01, 0x01, 0x76, 0x01, 0x01, 0x05, 0x00, 0x00,
    0x67, 0x1c, 0x0b, 0x06, 0x49, 0x53, 0x4b, 0x01, 0x09, 0x77, 0x10, 0x34, 0x9d, 0x01,
    0x01, 0x63, 0x93, 0xfc, 0x00, 0x76, 0x05, 0x00, 0x01, 0x35, 0x53, 0x62, 0x00, 0x62,
    0x00, 0x72, 0x63, 0x07, 0x01, 0x77, 0x01, 0x0b, 0x06, 0x49, 0x53, 0x4b, 0x01, 0x09,
    0x77, 0x10, 0x34, 0x9d, 0x07, 0x01, 0x00, 0x62, 0x0a, 0xff, 0xff, 0x72, 0x62, 0x01,
    0x65, 0x00, 0x00, 0xf8, 0x1c, 0x77, 0x77, 0x07, 0x81, 0x81, 0xc7, 0x82, 0x03, 0xff,
    0x01, 0x01, 0x01, 0x01, 0x04, 0x49, 0x53, 0x4b, 0x01, 0x77, 0x07, 0x01, 0x00, 0x00,
    0x00, 0x09, 0xff, 0x01, 0x01, 0x01, 0x01, 0x0b, 0x06, 0x49, 0x53, 0x4b, 0x01, 0x09,
    0x77, 0x10, 0x34, 0x9d, 0x01, 0x77, 0x07, 0x01, 0x00, 0x01, 0x08, 0x00, 0xff, 0x65,
    0x00, 0x00, 0x01, 0x80, 0x01, 0x62, 0x1e, 0x52, 0xff, 0x59, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x01, 0x77, 0x07, 0x01, 0x00, 0x01, 0x08, 0x01, 0xff, 0x01,
    0x01, 0x62, 0x1e, 0x52, 0xff, 0x59, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x77, 0x07, 0x01, 0x00, 0x01, 0x08, 0x02, 0xff, 0x01, 0x01, 0x62, 0x1e, 0x52,
    0xff, 0x59, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x77, 0x07, 0x01,
    0x00, 0x10, 0x07, 0x00, 0xff, 0x01, 0x01, 0x62, 0x1b, 0x52, 0x00, 0x55, 0x00, 0x00,
    0x00, 0x00, 0x01, 0x77, 0x07, 0x81, 0x81, 0xc7, 0x82, 0x05, 0xff, 0x01, 0x01, 0x01,
    0x01, 0x83, 0x02, 0x39, 0x96, 0x85, 0x68, 0x4f, 0x5e, 0x0e, 0x54, 0xce, 0x29, 0x52,
    0xb9, 0x34, 0x5e, 0xde, 0xe1, 0x02, 0x00, 0xca, 0xda, 0x2d, 0x84, 0x76, 0xf3, 0xfc,
    0x50, 0xe4, 0x53, 0xd9, 0x88, 0x79, 0x8f, 0x3f, 0xcb, 0x52, 0xe0, 0x53, 0x3b, 0xd3,
    0xab, 0xdc, 0xd2, 0xed, 0x6b, 0x05, 0x64, 0x0c, 0x25, 0x01, 0x01, 0x01, 0x63, 0x35,
    0x80, 0x00, 0x76, 0x05, 0x00, 0x01, 0x35, 0x54, 0x62, 0x00, 0x62, 0x00, 0x72, 0x63,
    0x02, 0x01, 0x71, 0x01, 0x63, 0xe8, 0x41, 0x00, 0x1b, 0x1b, 0x1b, 0x1b, 0x1a, 0x00,
    0x7e, 0xc3
};

MT681::MT681() {
    littleEndian = MT681::isLittleEndian();
}

uint8_t MT681::isLittleEndian() {
    union {
        uint16_t i;
        uint8_t c[sizeof(i)];
    } u = {0xCAFE};

    return u.c[0] == 0xFE;
}

uint16_t MT681::crcReflect(uint16_t data, uint8_t data_len) {
    uint16_t ret;
    uint8_t i;

    ret = data & 0x01;
    for (i = 1; i < data_len; i++) {
        data >>= 1;
        ret = (ret << 1) | (data & 0x01);
    }
    return ret;
}

void MT681::swapBytes(uint8_t *start, uint8_t size) {
    uint8_t *lo = start;
    uint8_t *hi = start + size - 1;

    //XOR swap algorithm
    for (;lo < hi;lo++, hi--) {
        if (lo != hi) {
            *lo ^= *hi;
            *hi ^= *lo;
            *lo ^= *hi;
        }
    }
}

void MT681::injectActivePower(uint32_t value) {
    uint32_t * activePower = (uint32_t *) &MT681::message[ACTIVEPOWER];
    *activePower = value;

    if(littleEndian) {
        MT681::swapBytes(&MT681::message[ACTIVEPOWER], sizeof(value));
    }

    MT681::patchChecksum();
}

void MT681::injectConsumption(uint64_t value) {
    uint64_t * consumption = (uint64_t *) &MT681::message[CONSUMPTION];
    *consumption = value;

    consumption = (uint64_t *) &MT681::message[TARIFF1];
    *consumption = value;

    if(littleEndian) {
        MT681::swapBytes(&MT681::message[CONSUMPTION], sizeof(value));
        MT681::swapBytes(&MT681::message[TARIFF1], sizeof(value));
    }

    MT681::patchChecksum();
}

void MT681::patchChecksum() {
    uint16_t value;
    uint16_t * crc = (uint16_t *) &MT681::message[CRC];

    value = crcX25(MT681::message, SML_MESSAGE_SIZE - 2);
    *crc = value;
    if(!littleEndian) {
        MT681::swapBytes(&MT681::message[CRC], sizeof(value));
    }
}

uint16_t MT681::crcX25(const uint8_t * data, uint16_t data_len) {
    const uint8_t *d = (const uint8_t *)data;
    uint8_t i,c;
    uint16_t bit;
    uint16_t crc = 0x84cf;

    while (data_len--) {
        c = MT681::crcReflect(*d++, 8);
        for (i = 0; i < 8; i++) {
            bit = crc & 0x8000;
            crc = (crc << 1) | ((c >> (7 - i)) & 0x01);
            if (bit) {
                crc ^= 0x1021;
            }
        }
        crc &= 0xffff;
    }
    crc &= 0xffff;

    for (i = 0; i < 16; i++) {
        bit = crc & 0x8000;
        crc = (crc << 1) | 0x00;
        if (bit) {
            crc ^= 0x1021;
        }
    }
    crc = MT681::crcReflect(crc, 16);
    return (crc ^ 0xffff) & 0xffff;
}

uint16_t MT681::getMessageLength() {
    return SML_MESSAGE_SIZE;
}
