#include "chip8.h"

chip8 myChip8;
void setupGraphics();
void setupInput();


int main()
{
    setupGraphics();
    setupInput();

    myChip8.init();
    myChip8.loadMem("something");

    for(;;)
    {
        myChip8.emulate();
    }


    return 0;
}

void setupGraphics()
{
    ;
}

void setupInput()
{
    ;
}