//Header file describing all the instructions in the CPU

#ifndef INS_H
#define INS_H

typedef enum Instruction Instruction_Type;

enum Instruction
{
  // LDA
  INS_LDA_IM = 0xA9,
  INS_LDA_ZP = 0xA5
};

#endif