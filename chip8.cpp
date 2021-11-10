#include "chip8.h"
#include <iostream>
#include <iterator>
#include <algorithm>
#include <fstream>

chip8::chip8() 
{	
	Initialize();
}

std::vector<bool> chip8::GetPixels()
{
	std::vector<bool> pixels;

	for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)
	{
		if (Display[i] > 0)
		{
			pixels.emplace_back(true);
		}
		else
		{
			pixels.emplace_back(false);
		}
	}

	return pixels;
}

void chip8::LoadRom(std::vector<uint8_t> rom)
{
	// allows this function to be identical if we change to a super-chip8 impl
	uint16_t available_ram = MEMORY_END - MEMORY_START;

	// check the rom will fit in memory
	if (available_ram < rom.size())
	{
		std::cerr << "Desired Rom is too big for the chip8 available memory" << std::endl;
		return;
	}

	// fill the memory with the rom
	for (uint16_t i = 0; i < rom.size(); i++)
	{
		Rom[i] = rom[i];
	}
}

uint16_t chip8::FetchOpcode()
{
	// fetch next 2 bytes and combine in to a 16 bit instruction
	uint8_t byte1 = Memory[PC];
	uint8_t byte2 = Memory[PC + 1];

	// combine
	uint16_t op = 0;
	// assign first byte
	op = byte1;
	// bit shift 8 places to the left
	op <<= 8;
	// assign byte2 to the last 8 bits
	op |= byte2;

	return op;
}

void chip8::HandleOpcode(uint16_t opcode)
{
	// mask the first nibble to get a rough idea
	// switch on the first nibble

	
	uint8_t first_byte = (opcode >> 8);
	uint8_t second_byte = (opcode & 0xFF);

	uint8_t n1 = first_byte & 0xF0;
	n1 = n1 >> 4;
	uint8_t n2 = first_byte & 0x0F;
	uint8_t n3 = second_byte & 0xF0;
	n3 = n3 >> 4;
	uint8_t n4 = second_byte & 0x0F;

 	switch (n1)
	{
		
		case 0x0:
			if (opcode == 0x00E0)
			{
				op_cls();
			}
			else if (opcode == 0x00EE)
			{
				op_ret();
			}
			else
			{
				op_sys_addr(opcode);
			}
			
			break;
		case 0x1:
			op_jp_addr(opcode);
			break;
		case 0x2:
			op_call_addr(opcode);
		case 0x3:
			op_skip_vx_nn(opcode);
			break;
		case 0x4:
			op_skip_vx_not_nn(opcode);
			break;
		case 0x5:
			if (n4 == 0)
			{
				op_se_vx_vy(opcode);
			}
			break;
		case 0x6:
			op_ld_vx(opcode);
			break;
		case 0xA:
			op_ld_i(opcode);
			break;
		case 0xB:
			op_jp_v0(opcode);
			break;
		case 0x7:
			op_add(opcode);
			break;
		case 0x9:
			op_sne_vx_vy(opcode);
			break;
		case 0xD:
			op_drw_vx_vy_n(opcode);
			break;
		default: 
			op_unimplemented(opcode);
			break;
	}
}

void chip8::Update(uint16_t newState)
{
	Keyboard = newState;

	// disgusting hack, at 60hz this is approximately 700 ips
	for (int i = 0; i < 12; i++)
	{
		uint16_t opcode = FetchOpcode();
		PC += 2;
		if (PC > 4095)
		{
			PC = PROGRAM_START;
		}
		// increment program counter
		
		HandleOpcode(opcode);
		
	}
}

std::vector<uint8_t> chip8::LoadRomStream(std::string path)
{
	std::ifstream rom_stream = std::ifstream(path, std::ios::binary);

	std::vector<char> buffer(std::istreambuf_iterator<char>(rom_stream), {});
	std::vector<uint8_t> converted_rom;

	// this might be sus
	for (int i = 0; i < buffer.size(); i++)
	{
		converted_rom.emplace_back((uint32_t)buffer[i]);
	}

	return converted_rom;

}


void chip8::Initialize()
{
	for (auto i = 0; i < MEMORY_CAPACITY; i++)
	{
		Memory[i] = 0x00;
	}
	for (auto i = 0; i < 16; i++)
	{
		V[i] = 0x00;
	}
	FillFont();
	DelayTimer = 0x00;
	SoundTimer = 0x00;
	StackPointer = 0x00;

	for (auto i = 0; i < (SCREEN_HEIGHT * SCREEN_WIDTH) / 8; i++)
	{
		Display[i] = 0X00;
	}

	for (auto i = 0; i < 16; i++)
	{
		Stack[i] = 0x0000;
	}
	I = 0x0000;
	PC = PROGRAM_START;
	Keyboard = 0x0000;
	for (auto i = 0; i < SYSTEM_MEMORY_PADDING; i++)
	{
		_padding[i] = 0xFF;
	}
	for (auto i = 0; i < ROM_SIZE; i++)
	{
		Rom[i] = 0x00;
	}
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

void chip8::op_sys_addr(uint16_t addr)
{
	uint16_t nnn = addr & 0xFFF;
	PC = nnn;
}

void chip8::op_ret()
{
	PC = Stack[0];
	StackPointer--;
}

// I SUSPECT YOU ARE THE PROBLEM
void chip8::op_jp_addr(uint16_t addr)
{
	uint16_t nnn = addr & 0xFFF;
	PC = nnn;
}

void chip8::op_call_addr(uint16_t addr)
{
	uint16_t nnn = addr & 0xFFF;
	StackPointer++;
	// shift stack to the right
	for (int i = 16; i > 0; i--)
	{
		if (i == 1)
		{
			Stack[0] = nnn;
		}
		else
		{
			Stack[i - 1] = Stack[i - 2];
		}
	}
	PC = nnn;
}

void chip8::op_cls()
{
	int limit = (SCREEN_HEIGHT * SCREEN_WIDTH);
	for (int i = 0; i < limit; i++)
	{
		Display[i] = 0x00;
	}
}

void chip8::op_se_vx_vy(uint16_t instruction)
{
	uint8_t first_byte = (instruction >> 8);
	uint8_t second_byte = (instruction & 0xFF);

	uint8_t x = first_byte & 0x0F;
	uint8_t y = second_byte & 0xF0;

	if (V[x] == V[y])
	{
		PC += 2;
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

void chip8::op_jp_v0(uint16_t instruction)
{
	uint16_t nnn = instruction & 0xFFF;
	PC = nnn + V[0x0];
}

void chip8::op_add(uint16_t instruction)
{
	uint8_t lower_byte = (instruction & 0xFF);
	uint8_t upper_byte = (instruction >> 8);
	auto x = (upper_byte & 15);

	V[x] += lower_byte;
}

void chip8::op_rnd_vx(uint16_t instruction)
{
	uint8_t lower_byte = (instruction & 0xFF);
	uint8_t upper_byte = (instruction >> 8);
	auto x = (upper_byte & 15);

	uint8_t rnd = std::rand();
	V[x] = rnd & lower_byte;

}

void chip8::op_drw_vx_vy_n(uint16_t instruction)
{
	// Draw a sprite at position VX, VY with N bytes of sprite data starting at the address stored in I
	// Set VF to 01 if any set pixels are changed to unset, and 00 otherwise
	// 
	// A sprite is a group of bytes which are a binary representation of the desired picture.Chip -
	//	8 sprites may be up to 15 bytes, for a possible sprite size of 8x15.

	uint8_t first_byte = (instruction >> 8);
	uint8_t second_byte = (instruction & 0xFF);

	uint8_t vx = first_byte & 0x0F;
	uint8_t vy = second_byte & 0xF0;
	vy = vy >> 4;
	uint8_t num_bytes = second_byte & 0x0F;

	char x_coord = V[vx];
	char y_coord = V[vy];
	V[0xF] = 0x00;

	// temporary vector to store the sprite data
	std::vector<uint8_t> sprite;

	for (uint16_t i = 0; i < num_bytes; i++)
	{
		// ..sprite data starting at the address stored in I
		uint16_t current_location = I + i;
		sprite.emplace_back(Memory[current_location]);
	}

	for (uint8_t row = 0; row < sprite.size(); row++)
	{	
		uint8_t current_row = sprite[row];
		for (uint8_t bit = 8; bit > 0; bit--)
		{
			int final_x = x_coord + (7 - (bit - 1));
			int final_y = y_coord + row;

			int final_byte_index = final_x + (SCREEN_WIDTH * final_y);

			bool pixel_currently_active = Display[final_byte_index] > 0;
			bool new_pixel_value = (current_row & (1 << bit -1)) >> bit -1 ;

			Display[final_byte_index] ^= new_pixel_value;

			if (pixel_currently_active && !new_pixel_value)
			{
				V[0xF] = 0x01;
			}
		}
	}	
}

void chip8::op_skp_vx(uint16_t instruction)
{
	uint8_t first_byte = (instruction >> 8);

	uint8_t x = first_byte & 15;

	if ((V[x] & Keyboard) == V[x])
	{
		PC += 2;
	}
}

void chip8::op_sknp_vx(uint16_t instruction)
{
	uint8_t first_byte = (instruction >> 8);

	uint8_t x = first_byte & 15;

	if ((V[x] & Keyboard) != V[x])
	{
		PC += 2;
	}
}

void chip8::op_ld_vx_dt(uint16_t instruction)
{
	uint8_t first_byte = (instruction >> 8);

	uint8_t x = first_byte & 15;
	V[x] = DelayTimer;
}

void chip8::op_ld_vx_k(uint16_t instruction)
{
	std::cerr << "op FX0A unimplemented!" << std::endl;
}

void chip8::op_ld_dt_vx(uint16_t instruction)
{
	uint8_t first_byte = (instruction >> 8);

	uint8_t x = first_byte & 15;
	DelayTimer = V[x];
}

void chip8::op_ld_st_vx(uint16_t instruction)
{
	uint8_t first_byte = (instruction >> 8);

	uint8_t x = first_byte & 15;
	SoundTimer = V[x];
}

void chip8::op_add_i_vx(uint16_t instruction)
{
	uint8_t first_byte = (instruction >> 8);

	uint8_t x = first_byte & 15;
	I += V[x];
}

void chip8::op_ld_f_vx(uint16_t instruction)
{
	uint8_t first_byte = (instruction >> 8);

	uint8_t x = first_byte & 15;
	I = FONT_START + (V[x] * 5);
}

void chip8::op_ld_b_vx(uint16_t instruction)
{
	std::cerr << "op FX33 unimplemented!" << std::endl;
}

void chip8::op_skip_vx_nn(uint16_t instruction)
{
	uint8_t first_byte = (instruction >> 8);
	uint8_t second_byte = (instruction & 0xFF);

	uint8_t x = first_byte & 15;
	if (V[x] == second_byte)
	{
		PC += 2;
	}
}

void chip8::op_skip_vx_not_nn(uint16_t instruction)
{
	uint8_t first_byte = (instruction >> 8);
	uint8_t second_byte = (instruction & 0xFF);

	uint8_t x = first_byte & 15;
	if (V[x] != second_byte)
	{
		PC += 2;
	}
}

void chip8::op_store_vy_vx(uint16_t instruction)
{
	uint8_t first_byte = (instruction >> 8);
	uint8_t second_byte = (instruction & 0xFF);

	uint8_t x = first_byte & 15;
	uint8_t y = second_byte & 15;

	V[x] = V[y];
}

void chip8::op_or_vx_vy(uint16_t instruction)
{
	uint8_t first_byte = (instruction >> 8);
	uint8_t second_byte = (instruction & 0xFF);

	uint8_t x = first_byte & 15;
	uint8_t y = second_byte & 15;

	V[x] |= V[y];
}

void chip8::op_and_vx_vy(uint16_t instruction)
{
	uint8_t first_byte = (instruction >> 8);
	uint8_t second_byte = (instruction & 0xFF);

	uint8_t x = first_byte & 15;
	uint8_t y = second_byte & 15;

	V[x]&= V[y];
}

void chip8::op_xor_vx_vy(uint16_t instruction)
{
	uint8_t first_byte = (instruction >> 8);
	uint8_t second_byte = (instruction & 0xFF);

	uint8_t x = first_byte & 15;
	uint8_t y = second_byte & 15;

	V[x] ^= V[y];
}

void chip8::op_add_vx_vy(uint16_t instruction)
{
	uint8_t first_byte = (instruction >> 8);
	uint8_t second_byte = (instruction & 0xFF);

	uint8_t x = first_byte & 15;
	uint8_t y = second_byte & 15;

	V[x] += V[y];
}

void chip8::op_sub_vx_vy(uint16_t instruction)
{
	uint8_t first_byte = (instruction >> 8);
	uint8_t second_byte = (instruction & 0xFF);

	uint8_t x = first_byte & 15;
	uint8_t y = second_byte & 15;

	V[x] -= V[y];
}

void chip8::op_shr_vx_vy(uint16_t instruction)
{
	// least significant bit
	// x &= -x;
	uint8_t first_byte = (instruction >> 8);
	uint8_t x = first_byte & 15;

	uint8_t x_lsb = V[x] & -V[x];
	
	V[0xF] = 0;

	if (x_lsb == 1)
	{
		V[0xF] = 1;
	}

	V[x] = V[x] / 2;
}

void chip8::op_subn_vx_vy(uint16_t instruction)
{
	uint8_t first_byte = (instruction >> 8);
	uint8_t second_byte = (instruction & 0xFF);

	uint8_t x = first_byte & 15;
	uint8_t y = second_byte & 15;

	if (V[y] > V[x])
	{
		V[0xF] = 1;
	}
	else
	{
		V[0xF] = 0;
	}

	V[x] = V[y] - V[x];
}

void chip8::op_shl_vx_vy(uint16_t instruction)
{
	uint8_t first_byte = (instruction >> 8);
	uint8_t x = first_byte & 15;

	bool msb_true = (V[x] & 0x80) == 0x80;
	
	V[0xF] = 0;
	if (msb_true)
	{
		V[0xF] = 1;
	}

}

void chip8::op_sne_vx_vy(uint16_t instruction)
{
	uint8_t first_byte = (instruction >> 8);
	uint8_t second_byte = (instruction & 0xFF);

	uint8_t x = first_byte & 15;
	uint8_t y = second_byte & 15;

	if (V[x] != V[y])
	{
		PC += 2;
	}
}

void chip8::op_ld_i_vx(uint16_t instruction)
{
	uint8_t first_byte = (instruction >> 8);

	uint8_t x = first_byte & 15;

	for (int i = 0; i < x; i++)
	{
		Memory[I + i] = V[i];
	}
}

void chip8::op_ld_vx_i(uint16_t instruction)
{
	uint8_t first_byte = (instruction >> 8);

	uint8_t x = first_byte & 15;

	for (int i = 0; i < x; i++)
	{
		V[i] = Memory[I + i];
	}
}

void chip8::op_unimplemented(uint16_t instruction)
{
	std::cout << "opcode unimplemented : " << std::hex << instruction << std::endl;
}