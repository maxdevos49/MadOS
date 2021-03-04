
void legacy_loader()
{

    //Serial Debugging is outputted to stdio on host machine
    init_serial();

    //Initialize the tty interface for the screen character output
    if(init_tty(VESA))
    {
        serial_printf("[ERROR] TTY initialization failed. Halting Progress.");

        while(1);
        __builtin_unreachable();
    }

    BOOT_HEADER boot_header;

    //TODO Populate cpu information

    //TODO Populate gdt information

    //TODO Populate idt information

    //TODO Populate memory map information

    //TODO Populate framebuffer information

    //TODO Load elf kernel

    //TODO Start elf kenrel

    //Should never reach here
    while(1);
    __builtin_unreachable();
}