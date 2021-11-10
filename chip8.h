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

    void Initialize();
    void FillFont();

    union {
        char Memory[MEMORY_CAPACITY];
        struct {
            uint8_t V[16], Font[16 * 5], DelayTimer, SoundTimer, StackPointer, InternalDisplay[(SCREEN_WIDTH * SCREEN_HEIGHT) / 8];
            uint16_t Stack[16], I, PC, Keyboard;
            uint8_t _padding[SYSTEM_MEMORY_PADDING], Rom[ROM_SIZE];
        };
    };
    
    uint8_t Display[SCREEN_WIDTH * SCREEN_HEIGHT];

private:
    // 0NNN
    void op_sys_addr(uint16_t addr);
    // 1NNN
    void op_jp_addr(uint16_t addr);
    // 00E0
    void op_cls();
    // 5XY0
    void op_se_vx_vy(uint16_t instruction);
    // 6XKK
    void op_ld_vx(uint16_t instruction);
    // ANNN
    void op_ld_i(uint16_t instruction);
    // BNNN
    void op_jp_v0(uint16_t instruction);
    // 7XKK
    void op_add(uint16_t instruction);
    // CXKK
    void op_rnd_vx(uint16_t instruction);
    // DXYN
    void op_drw_vx_vy_n(uint16_t instruction);
    // EX9E
    void op_skp_vx(uint16_t instruction);
    // EX9E
    void op_sknp_vx(uint16_t instruction);
    // FX07
    void op_ld_vx_dt(uint16_t instruction);
    // FX0A
    void op_ld_vx_k(uint16_t instruction);
    // FX15
    void op_ld_dt_vx(uint16_t instruction);
    // FX18
    void op_ld_st_vx(uint16_t instruction);
    // FX1E
    void op_add_i_vx(uint16_t instruction);
    // FX29
    void op_ld_f_vx(uint16_t instruction);
    // 3XKK
    void op_skip_vx_nn(uint16_t instruction);
    // 4XKK
    void op_skip_vx_not_nn(uint16_t instruction);
    // 8XY0
    void op_store_vy_vx(uint16_t instruction);
    // 8XY1
    void op_or_vx_vy(uint16_t instruction);
    // 8XY2
    void op_and_vx_vy(uint16_t instruction);
    // 8XY3
    void op_xor_vx_vy(uint16_t instruction);
    // 8XY4
    void op_add_vx_vy(uint16_t instruction);
    // 8XY5
    void op_sub_vx_vy(uint16_t instruction);
    // 8XY6
    void op_shr_vx_vy(uint16_t instruction);
    // 8XY7
    void op_subn_vx_vy(uint16_t instruction);
    // 8XYE
    void op_shl_vx_vy(uint16_t instruction);
    // 9XY0
    void op_sne_vx_vy(uint16_t instruction);

    //FX55
    void op_ld_i_vx(uint16_t instruction);
    //FX65
    void op_ld_vx_i(uint16_t instruction);

    void op_unimplemented(uint16_t instruction);



};