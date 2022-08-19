#include <cstdio>
#include <cstdint>
#include <string>

#include <pico/stdlib.h>

#include <hardware/regs/addressmap.h>

typedef union RP2040GpioStatus {
    struct {
        uint32_t __reserved0 : 8;
        uint32_t outfromperi : 1;
        uint32_t outtopad    : 1;
        uint32_t __reserved1 : 2;
        uint32_t oefromperi  : 1;
        uint32_t oetopad     : 1;
        uint32_t __reserved2 : 3;
        uint32_t infrompad   : 1;
        uint32_t __reserved3 : 1;
        uint32_t intoperi    : 1;
        uint32_t __reserved4 : 4;
        uint32_t irqfrompad  : 1;
        uint32_t __reserved5 : 1;
        uint32_t irqtoproc   : 1;
        uint32_t __reserved6 : 5;

    };
    uint32_t _value;
} RP2040GpioStatus;

typedef union RP2040GpioControl {
    struct {
        uint32_t funcsel       : 5; /* type RW, reset 0x1f */
        uint32_t __reserved4   : 3;
        uint32_t outover       : 2; /* type RW, reset 0x0 */
        uint32_t __reserved3   : 2;
        uint32_t oeover        : 2; /* type RW, reset 0x0 */
        uint32_t __reserved2   : 2;
        uint32_t inover        : 2; /* type RW, reset 0x0 */
        uint32_t __reserved1   : 10; 
        uint32_t irqover       : 2; /* type RW, reset 0x0 */
        uint32_t __reserved0   : 2; 
    };
    uint32_t _value;
} RP2040GpioControl;


std::string to_string(const RP2040GpioStatus& s)
{
    return std::string("{.irqtoproc = ") + std::to_string(s.irqtoproc) 
        + std::string(", .irqfrompad = ") + std::to_string(s.irqfrompad) 
        + std::string(", .intoperi = ") + std::to_string(s.intoperi) 
        + std::string(", .infrompad = ") + std::to_string(s.infrompad) 
        + std::string(", .oetopad = ") + std::to_string(s.oetopad) 
        + std::string(", .oefromperi = ") + std::to_string(s.oefromperi) 
        + std::string(", .outtopad = ") + std::to_string(s.outtopad) 
        + std::string(", .outfromperi = ") + std::to_string(s.outfromperi) 
        + std::string("}");
}

std::string to_string(const RP2040GpioControl& c)
{
    return std::string("{.irqover = ") + std::to_string(c.irqover) 
        + std::string(", .inover = ") + std::to_string(c.inover) 
        + std::string(", .oeover = ") + std::to_string(c.oeover) 
        + std::string(", .outover = ") + std::to_string(c.outover) 
        + std::string(", .funcsel = ") + std::to_string(c.funcsel) 
        + std::string("}");}

void dump_gpio_registers(uint32_t pin)
{
    RP2040GpioStatus status = *(RP2040GpioStatus *)(IO_BANK0_BASE + 0x08 * pin);
    RP2040GpioControl ctrl = *(RP2040GpioControl *)(IO_BANK0_BASE + 0x08 * pin + 0x04);
    uint32_t intr0 = *(uint32_t *)(IO_BANK0_BASE + 0xfc + 0x04* (pin / 8));
    uint32_t proc0_inte = *(uint32_t *)(IO_BANK0_BASE + 0x100 + 0x04* (pin / 8));
    uint32_t proc0_intf = *(uint32_t *)(IO_BANK0_BASE + 0x110 + 0x04* (pin / 8));
    uint32_t proc0_ints = *(uint32_t *)(IO_BANK0_BASE + 0x120 + 0x04* (pin / 8));
    uint32_t proc1_inte = *(uint32_t *)(IO_BANK0_BASE + 0x130 + 0x04* (pin / 8));
    uint32_t proc1_intf = *(uint32_t *)(IO_BANK0_BASE + 0x140 + 0x04* (pin / 8));
    uint32_t proc1_ints = *(uint32_t *)(IO_BANK0_BASE + 0x150 + 0x04* (pin / 8));
    uint32_t dormant_wake_inte = *(uint32_t *)(IO_BANK0_BASE + 0x160 + 0x04* (pin / 8));
    uint32_t dormant_wake_intf = *(uint32_t *)(IO_BANK0_BASE + 0x170 + 0x04* (pin / 8));
    uint32_t dormant_wake_ints = *(uint32_t *)(IO_BANK0_BASE + 0x180 + 0x04* (pin / 8));
    printf("Status of GPIO(%ld)\n", pin);
    printf("     status: 0x%08lx  %s\n", status._value, to_string(status).c_str());
    printf("    control: 0x%08lx  %s\n", ctrl._value, to_string(ctrl).c_str());
    printf("      intr0: 0x%08lx\n", intr0);
    printf(" proc0 inte: 0x%08lx\n", proc0_inte);
    printf(" proc0 intf: 0x%08lx\n", proc0_intf);
    printf(" proc0 ints: 0x%08lx\n", proc0_ints);
    printf(" proc1 inte: 0x%08lx\n", proc1_inte);
    printf(" proc1 intf: 0x%08lx\n", proc1_intf);
    printf(" proc1 ints: 0x%08lx\n", proc1_ints);
    printf(" dr wk inte: 0x%08lx\n", dormant_wake_inte);
    printf(" dr wk intf: 0x%08lx\n", dormant_wake_intf);
    printf(" dr wk ints: 0x%08lx\n", dormant_wake_ints);
}

int main()
{
    stdio_init_all();
    
    puts("/----------------------------------\\");
    puts("| Raspberry PICO - GPIO OUTPUT TEST |");
    puts("\\-----------------------------------/");

    dump_gpio_registers(7);
    gpio_init(7);
    puts("GPIO initialized");
    dump_gpio_registers(7);
    gpio_set_dir(7, true);
    puts("GPIO set dir");
    dump_gpio_registers(7);
    gpio_set_outover(7, true);
    puts("GPIO set outover");
    dump_gpio_registers(7);

    while (true)
    {
    }
}