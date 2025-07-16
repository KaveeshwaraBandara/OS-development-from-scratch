void printf(char* str){
    unsigned int* videoMemory = (unsigned int*)0xb8000;

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

extern "C" void kernelMain(void* multiboot_structure, unsigned int magic) {
    printf("Kavee\n");

    while (1);
}