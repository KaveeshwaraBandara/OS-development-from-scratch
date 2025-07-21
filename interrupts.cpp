#include "interrupts.h"

void printf(char* str);

InterruptManager::GateDescriptor InterruptManager::interruptDescriptorTable[256];   

void InterruptManager::SetInterruptDescriptorTableEntry(
    uint8_t interruptNumber, 
    uint16_t gdt_CodeSegmentSelectorOffset, 
    void (*handler)(), 
    uint8_t DescriptorPrivilegeLevel, 
    uint8_t DescriptorType
) {
    GateDescriptor& entry = interruptDescriptorTable[interruptNumber];
    
    entry.handlerAddressLowBits = (uint32_t)handler & 0xFFFF;
    entry.gdt_codeSegmentSelector = gdt_CodeSegmentSelectorOffset;
    entry.reserved = 0;
    entry.access = DescriptorType | (DescriptorPrivilegeLevel << 5) | 0x80; // Present bit set
    entry.handlerAddressHighBits = ((uint32_t)handler >> 16) & 0xFFFF;
}

InterruptManager::InterruptManager(GlobalDescriptorTable* gdt)
    : picMasterCommand(0x20), picMasterData(0x21), 
      picSlaveCommand(0xA0), picSlaveData(0xA1) {
    // Initialize the interrupt descriptor table
    for (int i = 0; i < 256; i++) {
        SetInterruptDescriptorTableEntry(i, gdt->CodeSegmentSelector(), IgnoreInterruptRequest, 0, 0x8E);
    }
    SetInterruptDescriptorTableEntry(0x20, gdt->CodeSegmentSelector(), HandleInterruptRequest0x00, 0, 0xE);
    SetInterruptDescriptorTableEntry(0x21, gdt->CodeSegmentSelector(), HandleInterruptRequest0x01, 0, 0xE);

    // Initialize the PICs
    picMasterCommand.Write(0x11); // Initialize PIC master
    picSlaveCommand.Write(0x11);  // Initialize PIC slave
    picMasterData.Write(0x20);     // Set master PIC vector offset
    picSlaveData.Write(0x28);      // Set slave PIC vector offset
    picMasterData.Write(0x04);     // Tell master PIC that there is a slave
    picSlaveData.Write(0x02);      // Tell slave PIC its cascade identity
    picMasterData.Write(0x01);     // Set master PIC to 8086 mode
    picSlaveData.Write(0x01);      // Set slave PIC to 8086 mode
    picMasterData.Write(0x0);      // Clear master PIC mask
    picSlaveData.Write(0x0);       // Clear slave PIC mask
    
    InterruptDescriptorTablePointer idtPtr;
    idtPtr.size = 256*sizeof(GateDescriptor) - 1;
    idtPtr.base = (uint32_t)interruptDescriptorTable;
    asm volatile("lidt %0" : : "m"(idtPtr)); // Load the IDT
}
InterruptManager::~InterruptManager() {
    // Destructor logic if needed
}

void InterruptManager::Activate() {
    asm volatile("sti"); // Enable interrupts
}

uint32_t InterruptManager::handleInterrupt(uint32_t interruptNumber, uint32_t esp){
    printf("Interrupt handled: ");
    return esp;
}

void InterruptManager::IgnoreInterruptRequest() {
    printf("Interrupt ignored\n");
}

void InterruptManager::HandleInterruptRequest0x00() {
    printf("Timer Interrupt (IRQ0)\n");
    // Acknowledge PIC if needed
}

void InterruptManager::HandleInterruptRequest0x01() {
    printf("Keyboard Interrupt (IRQ1)\n");
    // Acknowledge PIC if needed
}
