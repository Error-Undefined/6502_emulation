//Header file describing all the instructions in the CPU

#ifndef INS_H
#define INS_H

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

  //TAX
  INS_TAX_IMP = 0xAA, 
  //TAY
  INS_TAY_IMP = 0xA8,
  //TXA
  INS_TXA_IMP = 0x8A,
  //TYA
  INS_TYA_IMP = 0x98,

  //TSX
  INS_TSX_IMP = 0xBA,
  //TXS
  INS_TXS_IMP = 0x9A,
  //PHA
  INS_PHA_IMP = 0x48,
  //PHP
  INS_PHP_IMP = 0x08,
  //PLA
  INS_PLA_IMP = 0x68,
  //PLP
  INS_PLP_IMP = 0x28,

  //AND
  INS_AND_IM = 0x29,
  INS_AND_ZP = 0x25,
  INS_AND_ZP_X = 0x35,
  INS_AND_ABS = 0x2D,
  INS_AND_ABS_X = 0x3D,
  INS_AND_ABS_Y = 0x39,
  INS_AND_IND_X = 0x21,
  INS_AND_IND_Y = 0x31,

  //BIT
  INS_BIT_ZP = 0x24,
  INS_BIT_ABS = 0x2C,

  //EOR
  INS_EOR_IM = 0x49,
  INS_EOR_ZP = 0x45,
  INS_EOR_ZP_X = 0x55,
  INS_EOR_ABS = 0x4D,
  INS_EOR_ABS_X = 0x5D,
  INS_EOR_ABS_Y = 0x59,
  INS_EOR_IND_X = 0x41,
  INS_EOR_IND_Y = 0x51,

  //ORA
  INS_ORA_IM = 0x09,
  INS_ORA_ZP = 0x05,
  INS_ORA_ZP_X = 0x15,
  INS_ORA_ABS = 0x0D,
  INS_ORA_ABS_X = 0x1D,
  INS_ORA_ABS_Y = 0x19,
  INS_ORA_IND_X = 0x01,
  INS_ORA_IND_Y = 0x11,

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

typedef enum Instruction Instruction_Type;

#endif
