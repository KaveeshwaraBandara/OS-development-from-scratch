#ifndef __GDT_H__
#define __GDT_H__

#include "types.h"

class GlobalDescriptorTable{
    public:
        class SegmentDescriptor{
            private:
            uint16_t limit_lo;
            uint16_t base_lo;
            uint8_t base_hi;
            uint8_t type;
            uint8_t flags_limit_hi;
            uint8_t base_vhi;

            public:
            SegmentDescriptor(uint32_t base, uint32_t limit, uint8_t type);
            uint32_t Base() const;
            uint32_t Limit() const;
        } __attribute__((packed));

        SegmentDescriptor nullSegmentSelector;
        SegmentDescriptor unusedSegmentSelector;
        SegmentDescriptor codeSegmentSelector;
        SegmentDescriptor dataSegmentSelector;

        public:
        GlobalDescriptorTable();
        ~GlobalDescriptorTable();

        uint16_t CodeSegmentSelector() const;
        uint16_t DataSegmentSelector() const;
};

#endif