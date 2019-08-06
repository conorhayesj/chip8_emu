#include "chip8.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

chip8::chip8()
{
}

chip8::~chip8()
{
}

void chip8::init()
{
    // Initialize memory and registers
    pc = 0x200;
    opcode = 0;
    ir = 0;
    sp = 0;
}

void chip8::emulate()
{
    // Fetch opcode
    opcode = memory[pc] << 8 | memory[pc + 1];
    // Decode opcode
    switch(opcode & 0xF000) // Get first hex value (eg: 00e0 becomes 0000, ANNN becomes A000)
    {
        case 0x0000: // More than 1 opcode starts with 0, so additional switch is required to check last 4 bits.
            switch(opcode & 0x000F)
            {
                case 0x0000: // This corresponds to 0x00E0 -> Clear the screen.
                //Clear the screen.
                break;

                case 0x000E: // 0x00EE -> Return from subroutine.
                break;

                default:
                    std::cout << "Unknown opcode : " << opcode << std::endl;
            }
        break;

        case 0x1000: // 0x1NNN -> Jump to address NNN.
            pc = opcode & 0x0FFF;
        break;

        case 0x2000: // 0x2NNN -> Call subroutine at NNN.
            stack[sp] = pc;
            ++sp;
            pc = opcode & 0x0FFF;
        break;

        case 0x3000: // 0x3XNN -> Skips the next instruction if VX equals NN.
            if(V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
                pc += 4;
            else
                pc += 2;
        break;

        case 0x4000: // 0x4XNN -> Skips the next instruction if VX doesn't equal NN.
            if(V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF))
                pc += 4;
            else
                pc += 2;
        break;

        case 0x5000: // 0x5XY0 -> Skips the next instruction if VX equals VY.
            if(V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4])
                pc += 4;
            else
                pc += 2;
        break;

        case 0x6000: // 0x6XNN -> Sets VX to NN.
            V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
            pc += 2;
        break;

        case 0x7000: // 0x7XNN -> Adds NN to VX (Carry flag is not changed).
            V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
            pc += 2;
        break;

        case 0x8000:
            switch(opcode & 0x000F)
            {
                case 0x0000: // 0x8XY0 -> Sets VX to the value of VY.
                    V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                break;

                case 0x0001: // 0x8XY1 -> Sets VX to VX OR VY.
                    V[(opcode & 0x0F00) >> 8] |= V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                break;

                case 0x0002: // 0x8XY2 -> Sets VX to VX AND VY.
                    V[(opcode & 0x0F00) >> 8] &= V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                break;

                case 0x0003: // 0x8XY3 -> Sets VX to VX XOR VY.
                    V[(opcode & 0x0F00) >> 8] ^= V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                break;

                case 0x0004: // 0x8XY4 -> Adds VY to VX. VF is 1 when there's a carry, otherwise 0.
                    
                break;

                case 0x0005: // 0x8XY5 -> VY is subtracted from VX. VF is 0 when there's a borrow, otherwise 1.
                break;

                case 0x0006: // 0x8XY6 -> Stores the LSB of VX in VF and then shifts VX to the right by 1.
                break;

                case 0x0007: // 0x8XY7 -> Sets VX to VY minus VX. VF is 0 when there's a borrow, otherwise 1.
                break;

                case 0x000E: // 0x8XYE -> Stores the MSB of VX in VF then shifts VX to the left by 1.
                break;

                default:
                    std::cout << "Unknown opcode : " << opcode << std::endl;
            }
        break;

        case 0x9000: // 0x9XY0 -> Skips the next instruction if VX doesn't equal VY.
        break;

        case 0xA000: // 0xANNN -> Sets ir to the address of NNN
            ir = opcode & 0x0FFF;
            pc += 2;
        break;

        case 0xB000: // 0xBNNN -> Jumps to the address of NNN plus V0.
        break;

        case 0xC000: // 0xCXNN -> Sets VX to the result of a bitwise AND on a random number (0-255) and NN.
        break;

        case 0xD000: // 0xDXYN -> Draws a sprite at (VX,VY) with a width of 8 and a height of N.
        break;

        case 0xE000:
            switch(opcode & 0x000F)
            {
                case 0x000E: // 0xEX9E -> Skips the next instruction if the key stored in VX is pressed.
                break;

                case 0x0001: // 0xEXA1 -> Skips the next instruction if the key stored in VX isn't pressed.
                break;

                default:
                    std::cout << "Unknown opcode : " << opcode << std::endl;
            }
        break;

        case 0xF000:
            switch(opcode & 0x00FF)
            {
                case 0x0007: // 0xFX07 -> Sets VX to the value of the delay timer.
                break;

                case 0x000A: // 0xFX0A -> A key press is awaited and then stored in VX (Blocking operation).
                break;

                case 0x0015: // 0xFX15 -> Sets the delay timer to VX.
                break;

                case 0x0018: // 0xFX18 -> Sets the sound timer to VX.
                break;

                case 0x001E: // 0xFX1E -> Adds VX to IR.
                break;

                case 0x0029: // 0xFX29 -> Sets IR to the location of the sprite for the character in VX.
                break;

                case 0x0033: // 0xFX33 -> Take the decimal of VX and place the hundreds digit in memory at location IR, the tens in IR+1, and ones in IR+2.
                break;

                case 0x0055: // 0xFX55 -> Stores V0 to VX in memory starting at address IR.
                break;

                case 0x0065: // 0xFX65 -> Fills V0 to VX with values from memory starting at address IR.
                break;

                default:
                    std::cout << "Unknown opcode : " << opcode << std::endl;
            }
        break;        

    }
    // Execute opcode
    // Update timers
}

bool chip8::loadMem(const char *filename)
{
    FILE * file = fopen(filename, "rb");
    if(file == NULL)
    {
        std::cout << "File error!" << std::endl;
        return false;
    }
    
    fseek(file, 0, SEEK_END);
    long fSize = ftell(file);
    rewind(file);
    std::cout << "File size: " << (int)fSize << std::endl;
    char * buff = (char*)malloc(sizeof(char) * fSize);
    if(buff == NULL)
    {
        std::cout << "Memory error" << std::endl;
        return false;
    }
    // Load file into memory, starting at 512
    for(int i = 0; i < fSize; ++i)
    {
        memory[i+512] = buff[i];
    }
    return true;
}