#include "serial.h"
#include "basic_io.h"

/*
 * Paraphrased from https://littleosbook.github.io/#the-serial-ports 
 * and https://wiki.osdev.org/Serial_Ports 
 * We need to send: 
 * - Speed used for sending data (bit rate/divisor)
 * - If error checking should be used
 * - Number of bits that represent a unit of data
 */

uint8_t init_serial(uint16_t port) 
{
    /* Disable interrupts */
    io_byte_out(port + 1, 0x00);
    
    /* 
     * Enable DLAB on the serial line (0x80).
     * This means expect the first highest 8 bits on the port
     * and then the lowest 8 bits after
     */
    io_byte_out(port + 3, 0x80);
    
    /* Set divisor to 3 (115200 / 3 = 38400 Hz) */
    io_byte_out(port + 0, 0x3);

    /* Disable interrupts (again I guess) */
    io_byte_out(port + 1, 0x00);

    /* 
     * Set datatype to 8 bits (bit 0/1 = 0b11) 
     * with 1 stop bit (bit 2 = 0b0)
     * and no parity (bit3/4/5 = 0b000) 
     */
    io_byte_out(port, 0b00011);

    /*
     * Configuring the buffers
     * bit 6/7 = 0b11   - how many bytes should be stored in the FIFO buffers
     * bit 5   = 0b0    - 0 = 16-byte large buffer, 1 = 64-byte
     * bit 4   = 0b0    - reserved
     * bit 3   = 0b0    - dma = direct memory access (???)
     * bit 2   = 0b1    - clear transmission FIFO buffer
     * bit 1   = 0b1    - clear receiver FIFO buffer
     * bit 0   = 0b1    - enable FIFO buffer
     */
    io_byte_out(port + 2, 0b11000111);

    /*
     * Configuring the modem
     * bit 6/7 = 0b00   - reserved
     * bit 5   = 0b0    - Enable autoflow control
     * bit 4   = 0b0    - Enable loopback mode (used for debugging)
     * bit 3   = 0b1    - Aux output 2, used to recieve interrupts
     * bit 2   = 0b0    - Aux output 1
     * bit 1   = 0b1    - Ready to transmit
     * bit 0   = 0b1    - Data terminal ready
     */
    io_byte_out(port + 4, 0x0B);

    /*
     * Re-configuring the modem to add loopback
     */
    io_byte_out(port + 4, 0x1E);

    /*
     * Test out serial chip
     */
    io_byte_out(port, 0xAE);
    if(io_byte_in(port) != 0xAE)
        return 1;

    /*
     * Awesome, if our serial chip works
     * Reconfigure our modem to work normally
     */
    io_byte_out(port + 4, 0x0F);
    return 0;
}

uint8_t is_transmit_empty(uint16_t port) 
{
   return io_byte_in(port + 5) & 0x20;
}

uint8_t write_serial(uint16_t port, uint8_t data)
{
    while(is_transmit_empty(port) == 0);
    io_byte_out(port, data);
    return 0;
}

int serial_received(uint16_t port) {
   return io_byte_in(port + 5) & 1;
}

uint8_t read_serial(uint16_t port)
{
    while (serial_received(port) == 0);
    return io_byte_in(port);
}