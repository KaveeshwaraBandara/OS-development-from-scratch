#include "types.h"
#include "gdt.h"

void printf(char* str){
    static uint16_t* videoMemory = (uint16_t*)0xb8000;

    static int cursorX = 0;
    static int cursorY = 0;

    for(int i = 0; str[i] != '\0'; i++) {
        switch(str[i]) {
            case '\n':
                cursorX = 0;
                cursorY++;
                if(cursorY >= 25) {
                    cursorY = 0;
                    for(int j = 0; j < 80*25; j++) {
                        videoMemory[j] = (videoMemory[j] & 0xFF00) | ' ';
                    }  
                }
                continue;
            case '\r':
                cursorX = 0;
                continue;
        }
        if(cursorX >= 80) {
            cursorX = 0;
            cursorY++;
            if(cursorY >= 25) {
                cursorY = 0;
                for(int j = 0; j < 80*25; j++) {
                    videoMemory[j] = (videoMemory[j] & 0xFF00) | ' ';
                }
            }
        }
        videoMemory[80*cursorY+cursorX] = (videoMemory[80*cursorY+cursorX] & 0xFF00) | str[i];
    
        cursorX++;
        if(cursorX >= 80) {
            cursorX = 0;
            cursorY++;
            if(cursorY >= 25) {
                cursorY = 0;
                for(int j = 0; j < 80*25; j++) {
                    videoMemory[j] = (videoMemory[j] & 0xFF00) | ' ';
                }  
            }
        }
    }     
}


typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors() {
    for (constructor* ctor = &start_ctors; ctor < &end_ctors; ++ctor) {
        (*ctor)();
    }
}

extern "C" void kernelMain(void* multiboot_structure, uint32_t magic) {
    printf("Kavee\n");
    printf("Nishu\n");

    GlobalDescriptorTable gdt;

    while (1);
}