#ifndef __OPCODES_H__
#define __OPCODES_H__

typedef struct Chip8 Chip8;

void chip8_op_00e0(Chip8 *const chip8);
void chip8_op_00ee(Chip8 *const chip8);
void chip8_op_1nnn(Chip8 *const chip8);
void chip8_op_2nnn(Chip8 *const chip8);
void chip8_op_3xkk(Chip8 *const chip8);
void chip8_op_4xkk(Chip8 *const chip8);
void chip8_op_5xy0(Chip8 *const chip8);
void chip8_op_6xkk(Chip8 *const chip8);
void chip8_op_7xkk(Chip8 *const chip8);
void chip8_op_8xy0(Chip8 *const chip8);
void chip8_op_8xy1(Chip8 *const chip8);
void chip8_op_8xy2(Chip8 *const chip8);
void chip8_op_8xy3(Chip8 *const chip8);
void chip8_op_8xy4(Chip8 *const chip8);
void chip8_op_8xy5(Chip8 *const chip8);
void chip8_op_8xy6(Chip8 *const chip8);
void chip8_op_8xy7(Chip8 *const chip8);
void chip8_op_8xye(Chip8 *const chip8);					 
void chip8_op_9xy0(Chip8 *const chip8);						 
void chip8_op_annn(Chip8 *const chip8);						 
void chip8_op_bnnn(Chip8 *const chip8);						 
void chip8_op_cxkk(Chip8 *const chip8);						 
void chip8_op_dxyn(Chip8 *const chip8);						 
void chip8_op_ex9e(Chip8 *const chip8);						 
void chip8_op_exa1(Chip8 *const chip8);						 
void chip8_op_fx07(Chip8 *const chip8);						 
void chip8_op_fx0a(Chip8 *const chip8);						 
void chip8_op_fx15(Chip8 *const chip8);						 
void chip8_op_fx18(Chip8 *const chip8);						 
void chip8_op_fx1e(Chip8 *const chip8);						 
void chip8_op_fx29(Chip8 *const chip8);					 
void chip8_op_fx33(Chip8 *const chip8);						 
void chip8_op_fx55(Chip8 *const chip8);					 
void chip8_op_fx65(Chip8 *const chip8);

#endif
