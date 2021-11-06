#include <stack>

constexpr int SCREEN_WIDTH = 64;
constexpr int SCREEN_HEIGHT = 32;
constexpr int MEMORY_CAPACITY = 4096;

constexpr unsigned short PC_INCREMENT = 16;
constexpr unsigned short MEMORY_START = 0x000;
constexpr unsigned short MEMORY_END = 0xFFF;
constexpr unsigned short PROGRAM_START = 0x200;
constexpr unsigned short PROGRAM_START_ETI_660 = 0x600;

class chip8
{
public:

    chip8();

protected:
    
    // called 60 times per second
    void Update(unsigned short newState);

    void Initialize();
    void FillFont();

    union
    {
        unsigned char Memory[MEMORY_CAPACITY] {0};
        struct
        {
            unsigned char V[16], Display[(SCREEN_HEIGHT * SCREEN_WIDTH) / 8], Font[16 * 5], DelayTimer, SoundTimer, StackPointer;
            unsigned short Stack[16], I, PC, Keyboard;
        };
    };

private:
    // only a 12 bit value but will pack into 16 bit for ease
    void op_jp_addr(unsigned short addr);
    void op_cls();
    void op_ld_vx(unsigned short instruction);
    void op_ld_i(unsigned short instruction);
    void op_add(unsigned short instruction);
    void op_drw_vx_vy_n(unsigned short instruction);

    // need to think abouty this a bit more, how do we properly xor when we need to manipulate individual bits?
    // might make sense to move over to binary array for sake of ease
    unsigned short get_display_index(char x, char y);


};