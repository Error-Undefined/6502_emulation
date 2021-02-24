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
  INS_LDX_ABS_Y = 0xBE,

  // LDY
  INS_LDY_IM = 0xA0,
  INS_LDY_ZP = 0xA4,
  INS_LDY_ZP_X = 0xB4,
  INS_LDY_ABS = 0xAC,
  INS_LDY_ABS_X = 0xBC,

  // STA
  INS_STA_ZP = 0x85,
  INS_STA_ZP_X = 0x95,
  INS_STA_ABS = 0x8D,
  INS_STA_ABS_X = 0x9D,
  INS_STA_ABS_Y = 0x99,
  INS_STA_IND_X = 0x81,
  INS_STA_IND_Y = 0x91,

  //STX
  INS_STX_ZP = 0x86,
  INS_STX_ZP_Y = 0x96,
  INS_STX_ABS = 0x8E,

  //STY
  INS_STY_ZP = 0x84,
  INS_STY_ZP_X = 0x94,
  INS_STY_ABS = 0x8C,

  //JMP
  INS_JMP_ABS = 0x4C,
  INS_JMP_IND = 0x6C,

  //JSR
  INS_JSR_ABS = 0x20,

  //RTS
  INS_RTS_IMP = 0x60,

  // NOP
  INS_NOP = 0xEA
};

#endif