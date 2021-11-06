#include <cstdint>

constexpr int SCREEN_WIDTH = 64;
constexpr int SCREEN_HEIGHT = 32;
constexpr int MEMORY_CAPACITY = 4096;

constexpr uint16_t PC_INCREMENT = 16;
constexpr uint16_t MEMORY_START = 0x000;
constexpr uint16_t MEMORY_END = 0xFFF;
constexpr uint16_t PROGRAM_START = 0x200;
constexpr uint16_t PROGRAM_START_ETI_660 = 0x600;

class chip8
{
public:

    chip8();

protected:
    
    // called 60 times per second
    void Update(uint16_t newState);

    void Initialize();
    void FillFont();

    union
    {
        uint8_t Memory[MEMORY_CAPACITY] {0};
        struct
        {
            uint8_t V[16], Font[16 * 5], DelayTimer, SoundTimer, StackPointer;
            bool Display[SCREEN_HEIGHT * SCREEN_WIDTH];
            uint16_t Stack[16], I, PC, Keyboard;
        };
    };

private:
    // only a 12 bit value but will pack into 16 bit for ease
    void op_jp_addr(uint16_t addr);
    void op_cls();
    void op_ld_vx(uint16_t instruction);
    void op_ld_i(uint16_t instruction);
    void op_add(uint16_t instruction);
    void op_drw_vx_vy_n(uint16_t instruction);

    // need to think abouty this a bit more, how do we properly xor when we need to manipulate individual bits?
    // might make sense to move over to binary array for sake of ease
    uint16_t get_display_index(char x, char y);


};