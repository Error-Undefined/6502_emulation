//Header file describing all the instructions in the CPU

#ifndef INS_H
#define INS_H

typedef enum Instruction Instruction_Type;

enum Instruction
{
  // LDA
  INS_LDA_IM = 0xA9,
  INS_LDA_ZP = 0xA5,
  INS_LDA_ZP_X = 0xB5,
  INS_LDA_ABS = 0xAD,
  INS_LDA_ABS_X = 0xBD,
  INS_LDA_ABS_Y = 0xB9,
  INS_LDA_IND_X = 0xA1,
  INS_LDA_IND_Y = 0xB1,

  // LDX
  INS_LDX_IM = 0xA2,
  INS_LDX_ZP = 0xA6,
  INS_LDX_ZP_Y = 0xB6,
  INS_LDX_ABS = 0xAE,
  INS_LDX_ABS_Y = 0xBE
};

#endif