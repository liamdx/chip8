#include <stack>

constexpr int SCREEN_WIDTH = 64;
constexpr int SCREEN_HEIGHT = 32;
constexpr int MEMORY_CAPACITY = 4096;

#define MEMORY_START 0x000
#define MEMORY_END 0xFFF
#define PROGRAM_START = 0x200
#define PROGRAM_START_ETI_660 = 0x600

class chip8
{
public:

    chip8();

protected:
    
    void Fetch();
    void Decode();
    void Execute();


    void UpdateInput(unsigned short newState);
    void FillFont();

    union
    {
        unsigned char Memory[MEMORY_CAPACITY];
        unsigned char V[16], Display[(SCREEN_HEIGHT * SCREEN_WIDTH) / 8], Font[16 * 5], DelayTimer, SoundTimer, StackPointer;
        unsigned short Stack[16], I, PC, Keyboard;
    };

    //char* m_Framebuffer = new char[SCREEN_WIDTH * SCREEN_HEIGHT]; // screen
    //char* m_Ram = new char[4096]; // chip8 memory
    //unsigned short m_PC; // program counter
    //unsigned short m_IndexRegister; // "I" register
    //std::stack<unsigned short> m_Stack; // stack for 16 bit addresses
    //char m_DelayTimer; // 8bit delay timer
    //char m_SoundTimer; // 8bit sound timer
    //char* m_VariableRegisters = new char[16]; // general purpose registers

    // font for chip8 (load in first 512 bytes of m_Ram)
    //0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    //0x20, 0x60, 0x20, 0x20, 0x70, // 1
    //0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    //0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    //0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    //0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    //0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    //0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    //0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    //0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    //0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    //0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    //0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    //0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    //0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    //0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};