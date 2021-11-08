#include <cstdint>
#include <vector>
#include <string>

constexpr int SCREEN_WIDTH = 64;
constexpr int SCREEN_HEIGHT = 32;
constexpr int MEMORY_CAPACITY = 4096;
constexpr int SYSTEM_MEMORY_PADDING = 118;
constexpr int ROM_SIZE = 3584;

constexpr uint16_t PC_INCREMENT = 16;
constexpr uint16_t MEMORY_START = 0x000;
constexpr uint16_t MEMORY_END = 0xFFF;
constexpr uint16_t PROGRAM_START = 0x200;
constexpr uint16_t PROGRAM_START_ETI_660 = 0x600;

class chip8
{
public:

    chip8();
    std::vector<bool> GetPixels();
    void LoadRom(std::vector<uint8_t> rom);
    void Update(uint16_t newState);

    std::vector<uint8_t> LoadRomStream(std::string path);
protected:
    
    uint16_t FetchOpcode();
    void HandleOpcode(uint16_t opcode);
    
    // called 60 times per second
    

    void Initialize();
    void FillFont();

    union
    {
    public:
        uint8_t Memory[MEMORY_CAPACITY];
        struct
        {
            uint8_t V[16], Font[16 * 5], DelayTimer, SoundTimer, StackPointer, Display[(SCREEN_WIDTH * SCREEN_HEIGHT) / 8];
            uint16_t Stack[16], I, PC, Keyboard;
            uint8_t _padding[SYSTEM_MEMORY_PADDING], Rom[ROM_SIZE];
        };
    };

private:
    // 1NNN
    void op_jp_addr(uint16_t addr);
    // 00E0
    void op_cls();
    // 6XKK
    void op_ld_vx(uint16_t instruction);
    // ANNN
    void op_ld_i(uint16_t instruction);
    // 7XKK
    void op_add(uint16_t instruction);
    // DXYN
    void op_drw_vx_vy_n(uint16_t instruction);

    void op_unimplemented();

    // need to think abouty this a bit more, how do we properly xor when we need to manipulate individual bits?
    // might make sense to move over to binary array for sake of ease
    uint16_t get_display_index(char x, char y);


};