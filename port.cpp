#include "port.h"

Port::Port(uint16_t portNumber) : portNumber(portNumber) {
    this->portNumber = portNumber;
};
Port::~Port() {};

Port8Bit::Port8Bit(uint16_t portNumber) : Port(portNumber) {}
Port8Bit::~Port8Bit() {}

uint8_t Port8Bit::Read() {
    uint8_t result;
    asm volatile("inb %1, %0" : "=a"(result) : "Nd"(portNumber));
    return result;
}

void Port8Bit::Write(uint8_t data) {
    asm volatile("outb %0, %1" : : "a"(data), "Nd"(portNumber));
}

Port8BitSlow::Port8BitSlow(uint16_t portNumber) : Port8Bit(portNumber) {}
Port8BitSlow::~Port8BitSlow() {}    
void Port8BitSlow::Write(uint8_t data) {
    asm volatile("outb %0, %1" : : "a"(data), "Nd"(portNumber));
    for (int i = 0; i < 1000; i++) {
        asm volatile("nop");
    }
}
Port16Bit::Port16Bit(uint16_t portNumber) : Port(portNumber) {}
Port16Bit::~Port16Bit() {}
uint16_t Port16Bit::Read() {
    uint16_t result;
    asm volatile("inw %1, %0" : "=a"(result) : "Nd"(portNumber));
    return result;
}
void Port16Bit::Write(uint16_t data) {
    asm volatile("outw %0, %1" : : "a"(data), "Nd"(portNumber));
}
Port32Bit::Port32Bit(uint16_t portNumber) : Port(portNumber) {}
Port32Bit::~Port32Bit() {}
uint32_t Port32Bit::Read() {
    uint32_t result;
    asm volatile("inl %1, %0" : "=a"(result) : "Nd"(portNumber));
    return result;
}
void Port32Bit::Write(uint32_t data) {
    asm volatile("outl %0, %1" : : "a"(data),
                    "Nd"(portNumber));              
}
