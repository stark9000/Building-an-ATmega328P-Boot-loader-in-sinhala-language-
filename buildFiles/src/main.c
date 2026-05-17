/*
 * ATmega328P Bootloader
 * Compatible with Arduino IDE (STK500v1 / Avrdude)
 *
 * Application : 0x0000 - 0x7BFF (31,744 bytes)
 * Bootloader  : 0x7C00 - 0x7FFF (1,024  bytes)
 * HFUSE = 0xDA  LOCK = 0xEF  UART = 115200 8N1
 */
 
#include <avr/io.h>
#include <avr/boot.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <stdint.h>
 
#define BOOT_START          0x7C00
#define PAGE_SIZE           128
#define F_CPU               16000000UL
#define BAUD                115200UL
#define UBRR_VAL            ((F_CPU / (16UL * BAUD)) - 1)
 
#define STK_OK              0x10
#define STK_INSYNC          0x14
#define STK_NOSYNC          0x15
#define STK_CRC_EOP         0x20
#define STK_GET_SYNC        0x30
#define STK_GET_PARAMETER   0x41
#define STK_SET_DEVICE      0x42
#define STK_SET_DEVICE_EXT  0x45
#define STK_ENTER_PROGMODE  0x50
#define STK_LEAVE_PROGMODE  0x51
#define STK_LOAD_ADDRESS    0x55
#define STK_PROG_PAGE       0x64
#define STK_READ_PAGE       0x74
#define STK_READ_SIGN       0x75
#define SIGNATURE_0         0x1E
#define SIGNATURE_1         0x95
#define SIGNATURE_2         0x0F
 
static uint8_t  page_buffer[PAGE_SIZE];
static uint16_t address = 0;
 
/* ── UART ── */
void uart_init(void) {
    UBRR0H = (uint8_t)(UBRR_VAL >> 8);
    UBRR0L = (uint8_t)(UBRR_VAL);
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}
void uart_send(uint8_t b) {
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = b;
}
uint8_t uart_receive(void) {
    while (!(UCSR0A & (1 << RXC0)));
    return UDR0;
}
 
/* ── STK500v1 helper ── */
void get_sync(void) {
    uint8_t eop = uart_receive();
    if (eop != STK_CRC_EOP) uart_send(STK_NOSYNC);
}
 
/* ── Flash self-programming ── */
void write_flash_page(uint16_t word_addr, uint8_t *data, uint16_t len) {
    if (word_addr >= (BOOT_START / 2)) return;
    uint32_t byte_addr = (uint32_t)word_addr * 2;
    boot_page_erase(byte_addr);  boot_spm_busy_wait();
    for (uint16_t i = 0; i < len; i += 2) {
        uint16_t word = data[i] | ((uint16_t)data[i+1] << 8);
        boot_page_fill(byte_addr + i, word);
    }
    boot_page_write(byte_addr);  boot_spm_busy_wait();
    boot_rww_enable();
}
 
/* ── Jump to application ── */
void jump_to_app(void) {
    wdt_enable(WDTO_15MS);
    while(1);
}
 
/* ── Main ── */
int main(void) {
    uint8_t mcusr = MCUSR;
    MCUSR = 0;
    if (mcusr & (1 << WDRF)) {
        wdt_disable();
        ((void (*)(void))0)();   // jump to 0x0000
    }
    wdt_enable(WDTO_1S);
    uart_init();
    while (1) {
        uint8_t cmd = uart_receive();
        switch (cmd) {
            case STK_GET_SYNC:
                wdt_disable();
                get_sync();
                uart_send(STK_INSYNC); uart_send(STK_OK);
                break;
            case STK_GET_PARAMETER: {
                uint8_t p = uart_receive(); get_sync();
                uart_send(STK_INSYNC);
                if      (p == 0x80) uart_send(0x02);
                else if (p == 0x81) uart_send(0x01);
                else                uart_send(0x00);
                uart_send(STK_OK); break; }
            case STK_SET_DEVICE:
                for (uint8_t i=0;i<20;i++) uart_receive();
                get_sync();
                uart_send(STK_INSYNC); uart_send(STK_OK); break;
            case STK_SET_DEVICE_EXT:
                for (uint8_t i=0;i<5;i++) uart_receive();
                get_sync();
                uart_send(STK_INSYNC); uart_send(STK_OK); break;
            case STK_ENTER_PROGMODE:
                get_sync();
                uart_send(STK_INSYNC); uart_send(STK_OK); break;
            case STK_LOAD_ADDRESS: {
                uint16_t lo = uart_receive();
                uint16_t hi = uart_receive();
                address = (hi << 8) | lo;
                get_sync();
                uart_send(STK_INSYNC); uart_send(STK_OK); break; }
            case STK_PROG_PAGE: {
                uint16_t len  = ((uint16_t)uart_receive()<<8)|uart_receive();
                uint8_t  type = uart_receive();
                for (uint16_t i=0;i<len;i++) page_buffer[i]=uart_receive();
                get_sync();
                if (type=='F') write_flash_page(address,page_buffer,len);
                uart_send(STK_INSYNC); uart_send(STK_OK); break; }
            case STK_READ_PAGE: {
                uint16_t len  = ((uint16_t)uart_receive()<<8)|uart_receive();
                uint8_t  type = uart_receive(); get_sync();
                uart_send(STK_INSYNC);
                if (type=='F')
                    for (uint16_t i=0;i<len;i++)
                        uart_send(pgm_read_byte((uint32_t)(address*2)+i));
                uart_send(STK_OK); break; }
            case STK_READ_SIGN:
                get_sync();
                uart_send(STK_INSYNC);
                uart_send(SIGNATURE_0);
                uart_send(SIGNATURE_1);
                uart_send(SIGNATURE_2);
                uart_send(STK_OK); break;
            case STK_LEAVE_PROGMODE:
                get_sync();
                uart_send(STK_INSYNC); uart_send(STK_OK);
                jump_to_app(); break;
            default:
                get_sync();
                uart_send(STK_INSYNC); uart_send(STK_OK); break;
        }
    }
    return 0;
}
