//
// Created by boran on 25.09.2016.
//

#include "../include/gdt.h"

// set a descriptor in the GDT
void gdt_set_gate(uint32 num, uint64 base, uint64 limit, uint8 access, uint8 detail) {

    // base address
    gdt[num].base_low = (uint16) (base & 0xFFFF);
    gdt[num].base_middle = (uint8) ((base >> 16) & 0xFF);
    gdt[num].base_high = (uint8) ((base >> 24) & 0xFF);

    // descriptor limits
    gdt[num].limit_low = (uint16) (limit & 0xFFFF);
    gdt[num].detail = (uint8) ((limit >> 16) & 0x0F);

    //detail and access
    gdt[num].detail |= (detail & 0xF0);
    gdt[num].access = access;
}

/* Should be called by main. This will setup the special GDT
*  pointer, set up the first 3 entries in our GDT, and then
*  finally call gdt_flush() in the gdt.asm File in order
*  to tell the processor where the new GDT is and update the
*  new segment registers */
void gdt_install()
{
    /* Setup the GDT pointer and limit */
    gdtPtr.limit = (sizeof(struct gdt_entry) * 3) - 1;
    gdtPtr.base = (uint32) &gdt;

    /* Our NULL descriptor */
    gdt_set_gate(0, 0, 0, 0, 0);

    /* The second entry is our Code Segment. The base address
    *  is 0, the limit is 4GBytes, it uses 4KByte granularity,
    *  uses 32-bit opcodes, and is a Code Segment descriptor.
    *  Please check the table above in the tutorial in order
    *  to see exactly what each value means */
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

    /* The third entry is our Data Segment. It's EXACTLY the
    *  same as our code segment, but the descriptor type in
    *  this entry's access byte says it's a Data Segment */
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    /* Flush out the old GDT and install the new changes! */
    gdt_flush();
}