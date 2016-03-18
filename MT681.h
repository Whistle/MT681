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

#ifndef MT681_h
#define MT681_h

#include <stdint.h>

#define SML_MESSAGE_SIZE	0x144

#define ACTIVEPOWER         0xde
#define CONSUMPTION         0x96
#define TARIFF1             0xae
#define TARIFF2             0xc6


#define CRC                 (SML_MESSAGE_SIZE - 0x02)

class MT681
{
private:
    uint8_t littleEndian;

    uint16_t crcX25(const uint8_t * data, uint16_t data_length);
    uint16_t crcReflect(uint16_t data, uint8_t data_len);
    uint8_t isLittleEndian();
    void swapBytes(uint8_t *start, uint8_t size);
    void patchChecksum();

public:
    MT681();
    static uint8_t message[SML_MESSAGE_SIZE];

    void injectActivePower(uint32_t value);
    void injectConsumption(uint64_t value);
    uint16_t getMessageLength();
};

#endif
