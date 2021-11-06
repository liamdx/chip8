#include "chip8.h"
#include <vector>
#include <iostream>
chip8::chip8() 
{
	Initialize();
}

void chip8::Update(uint16_t newState)
{
	Keyboard = newState;
	uint16_t opcode = 1;

	switch(opcode)
	{
		// do the op
	}

	PC += 0xF0;
}


void chip8::Initialize()
{
	PC = PROGRAM_START;
	FillFont();
}

void chip8::FillFont()
{
	//0
	Font[0] = 0xF0;
	Font[1] = 0x90;
	Font[2] = 0x90;
	Font[3] = 0x90;
	Font[4] = 0xF0;
	//1
	Font[5] = 0x20;
	Font[6] = 0x60;
	Font[7] = 0x20;
	Font[8] = 0x20;
	Font[9] = 0x70;
	//2
	Font[10] = 0xF0;
	Font[11] = 0x10;
	Font[12] = 0xF0;
	Font[13] = 0x80;
	Font[14] = 0xF0;
	//3
	Font[15] = 0xF0;
	Font[16] = 0x10;
	Font[17] = 0xF0;
	Font[18] = 0x10;
	Font[19] = 0xF0;
	//4
	Font[20] = 0x90;
	Font[21] = 0x90;
	Font[22] = 0xF0;
	Font[23] = 0x10;
	Font[24] = 0x10;
	//5
	Font[25] = 0xF0;
	Font[26] = 0x80;
	Font[27] = 0xF0;
	Font[28] = 0x10;
	Font[29] = 0xF0;
	//6
	Font[30] = 0xF0;
	Font[31] = 0x80;
	Font[32] = 0xF0;
	Font[33] = 0x90;
	Font[34] = 0xF0;
	//7
	Font[35] = 0xF0;
	Font[36] = 0x10;
	Font[37] = 0x20;
	Font[38] = 0x40;
	Font[39] = 0x40;
	//8
	Font[40] = 0xF0;
	Font[41] = 0x90;
	Font[42] = 0xF0;
	Font[43] = 0x90;
	Font[44] = 0xF0;
	//9
	Font[45] = 0xF0;
	Font[46] = 0x90;
	Font[47] = 0xF0;
	Font[48] = 0x10;
	Font[49] = 0xF0;
	//A
	Font[50] = 0xF0;
	Font[51] = 0x90;
	Font[52] = 0xF0;
	Font[53] = 0x90;
	Font[54] = 0x90;
	//B
	Font[55] = 0xE0;
	Font[56] = 0x90;
	Font[57] = 0xE0;
	Font[58] = 0x90;
	Font[59] = 0xE0;
	//C
	Font[60] = 0xF0;
	Font[61] = 0x80;
	Font[62] = 0x80;
	Font[63] = 0x80;
	Font[64] = 0xF0;
	//D
	Font[65] = 0xE0;
	Font[66] = 0x90;
	Font[67] = 0x90;
	Font[68] = 0x90;
	Font[69] = 0xE0;
	//E
	Font[70] = 0xF0;
	Font[71] = 0x80;
	Font[72] = 0xF0;
	Font[73] = 0x80;
	Font[74] = 0xF0;
	//F
	Font[75] = 0xF0;
	Font[76] = 0x80;
	Font[77] = 0xF0;
	Font[78] = 0x80;
	Font[79] = 0x80;	
}

void chip8::op_jp_addr(uint16_t addr)
{
	uint16_t nnn = addr & 0xFFF;
	PC = nnn;
}

void chip8::op_cls()
{
	int limit = (SCREEN_HEIGHT * SCREEN_WIDTH) / 8;
	for (int i = 0; i < limit; i++)
	{
		Display[i] = 0x00;
	}
}

void chip8::op_ld_vx(uint16_t instruction)
{
	uint8_t lower_byte = (instruction & 0xFF);
	uint8_t upper_byte = (instruction >> 8);
	auto x = (upper_byte & 15);

	V[x] = lower_byte;
}

void chip8::op_ld_i(uint16_t instruction)
{
	uint16_t nnn = instruction & 0xFFF;
	I = nnn;
}

void chip8::op_add(uint16_t instruction)
{
	uint8_t lower_byte = (instruction & 0xFF);
	uint8_t upper_byte = (instruction >> 8);
	auto x = (upper_byte & 15);

	V[x] += lower_byte;
}

void chip8::op_drw_vx_vy_n(uint16_t instruction)
{
	// Draw a sprite at position VX, VY with N bytes of sprite data starting at the address stored in I
	// Set VF to 01 if any set pixels are changed to unset, and 00 otherwise
	// 
	// A sprite is a group of bytes which are a binary representation of the desired picture.Chip -
	//	8 sprites may be up to 15 bytes, for a possible sprite size of 8x15.

	uint16_t num_bytes = instruction & 0xF;
	// get the starting pixel location from the Vx registers
	uint16_t vx = instruction & 0xF00;
	uint16_t vy = instruction & 0xF0;
	char x_coord = V[vx];
	char y_coord = V[vy];

	// temporary vector to store the sprite data
	std::vector<uint8_t> sprite;

	for (uint16_t i = 0; i < num_bytes; i++)
	{
		// ..sprite data starting at the address stored in I
		uint16_t current_location = I + i;
		sprite.emplace_back(Memory[current_location]);
	}

	for (int row = 0; row < sprite.size(); row++)
	{

	}

}

uint16_t chip8::get_display_index(char x, char y)
{

	// (0, 0) = 0	(63, 0) = 63

	// total num bools = screenw * screenh = 64 * 32 = 2048	
	// last index 2047
	// (0, 31) = 2047 - 64	= ((x + 1) * (y + 1)) - 1;
	// screen_width * y = 64 * 31 = 1984 + 31 = 2015
	// (63, 31)

	if (x >= SCREEN_WIDTH)
	{
		std::cerr << "X bigger than screen width" << std::endl;
	}

	if (y >= SCREEN_HEIGHT)
	{
		std::cerr << "Y bigger than screen height" << std::endl;
	}


	return ((x + 1) * (y + 1)) - 1;
}
