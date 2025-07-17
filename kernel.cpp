#include "types.h"
#include "gdt.h"

void printf(char* str){
    uint16_t* videoMemory = (uint16_t*)0xb8000;

    for(int i = 0; str[i] != '\0'; i++) {
        videoMemory[i] = (videoMemory[i] & 0xFF00) | str[i];
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

    GlobalDescriptorTable gdt;

    while (1);
}