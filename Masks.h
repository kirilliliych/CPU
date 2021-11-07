#ifndef MASKS_H
#define MASKS_H

const unsigned char RAM_MASK       = (unsigned char) 0b10000000;
const unsigned char REG_MASK       = (unsigned char) 0b01000000;
const unsigned char IMM_CONST_MASK = (unsigned char) 0b00100000;
const unsigned char ARGS_MASK      = (unsigned char) 0b11100000;
const unsigned char COMMAND_MASK   = (unsigned char) 0b00011111;

#endif