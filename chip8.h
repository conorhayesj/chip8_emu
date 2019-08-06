class chip8 
{
    public:
        chip8();
        ~chip8();

        void init();
        bool loadMem(const char *filename);
        void emulate();

    private:
        unsigned short pc;
        unsigned short sp;
        unsigned short ir;
        unsigned short opcode;

        unsigned short stack[16];
        unsigned char memory[4096];
        unsigned char V[16];
        unsigned char gfx[64 * 32];
        unsigned char key[16];
        
        bool drawFlag;

        unsigned char delay_timer;
        unsigned char sound_timer;
};