#include "gdt.h"

GlobalDescriptorTable::GlobalDescriptorTable()
    : nullSegmentSelector(0, 0, 0),
      unusedSegmentSelector(0, 0, 0),
      codeSegmentSelector(0, 64*1024*1024, 0x9A), // Code segment
      dataSegmentSelector(0, 64*1024*1024, 0x92)  // Data segment
{
    struct {
        uint16_t limit;
        uint32_t base;
    } __attribute__((packed)) gdtDescriptor;

    gdtDescriptor.limit = sizeof(GlobalDescriptorTable) - 1;
    gdtDescriptor.base = (uint32_t)this;

    asm volatile("lgdt %0" : : "m"(gdtDescriptor));
}

GlobalDescriptorTable::~GlobalDescriptorTable() {}

uint16_t GlobalDescriptorTable::DataSegmentSelector() const {
    return (uint8_t*)&dataSegmentSelector - (uint8_t*)this;
}

uint16_t GlobalDescriptorTable::CodeSegmentSelector() const {
    return (uint8_t*)&codeSegmentSelector - (uint8_t*)this;
}

GlobalDescriptorTable::SegmentDescriptor::SegmentDescriptor(uint32_t base, uint32_t limit, uint8_t access)
{
    uint8_t* target = (uint8_t*)this;

    if (limit <= 65536) {
        target[6] = 0x40;
    } else {
        if ((limit & 0xFFF) != 0xFFF) {
            limit = (limit >> 12) - 1;
        } else {
            limit = (limit >> 12);
        }
        target[6] = 0xC0;
    }

    // Set limit
    target[0] = limit & 0xFF;
    target[1] = (limit >> 8) & 0xFF;
    target[6] |= (limit >> 16) & 0x0F;

    // Set base
    target[2] = base & 0xFF;
    target[3] = (base >> 8) & 0xFF;
    target[4] = (base >> 16) & 0xFF;
    target[7] = (base >> 24) & 0xFF;

    // Access byte
    target[5] = access;
}

uint32_t GlobalDescriptorTable::SegmentDescriptor::Base() const {
    uint8_t* target = (uint8_t*)this;
    uint32_t result = target[7];
    result = (result << 8) | target[4];
    result = (result << 8) | target[3];
    result = (result << 8) | target[2];
    return result;
}

uint32_t GlobalDescriptorTable::SegmentDescriptor::Limit() const {
    uint8_t* target = (uint8_t*)this;
    uint32_t result = target[6] & 0x0F;
    result = (result << 8) | target[1];
    result = (result << 8) | target[0];

    if (target[6] & 0xC0) {
        return (result << 12) | 0xFFF;
    } else {
        return result;
    }
}
