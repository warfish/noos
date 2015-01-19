//
// GDB stub implementation
//

#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include <arch/dbg.h>

#include <noos/serio.h>

//////////////////////////////////////////////////////////////////////////////////////////////////

#define GDB_SERIAL_PORT     SIO_COM1
#define GDB_RX_BUFFER_SIZE  256

static char g_rx_buffer[GDB_RX_BUFFER_SIZE] = {0};

static char g_hex_chars[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

static char gdb_recv_char(void)
{
    return serio_read(GDB_SERIAL_PORT);
}

static void gdb_send_char(char c)
{
    serio_write(GDB_SERIAL_PORT, c);
}

static int gdb_has_char(void)
{
    return serio_poll(GDB_SERIAL_PORT);
}

static int recv_packet(void)
{
    uint8_t checksum;
    char* pdata;
    int len;

    while (1)
    {
        // wait until packet start symbol
        while ('$' != gdb_recv_char())
            ;

        // recieve packet data
    _retry:

        checksum = 0;
        pdata = g_rx_buffer;
        len = 0;

        while (1)
        {
            char c = gdb_recv_char();
            if (c == '$') {
                // another packet begun? retry
                goto _retry;
            } else if (c == '#') {
                // packet ends
                break;
            } else {
                // packet data
                *pdata++ = c;
                len++;
                checksum += c;
            }
        }

        // verify checksum
        char checksum_chars[3] = {0};
        checksum_chars[0] = gdb_recv_char();
        checksum_chars[1] = gdb_recv_char();
        
        if (checksum != strtol(checksum_chars, NULL, 16)) {
            // send nack and retry
            gdb_send_char('-');
            continue;
        }

        // done
        return len;
    }
}

static void send_packet(const char* data)
{
    uint8_t checksum = 0;

    // Since we might have to send this packet several times we will precalculate checksum now
    const char* ptr = data;
    while(*ptr != '\0') {
        checksum += *ptr++;
    }

    char checksum_chars[3] = {0};
    checksum_chars[0] = g_hex_chars[(checksum >> 4) & 0xF];
    checksum_chars[1] = g_hex_chars[checksum & 0xF];

_resend:

    // start packet
    gdb_send_char('$');

    // send data chars
    ptr = data;
    while(*ptr != '\0') {
        gdb_send_char(*ptr++);
    }

    // send packet end and checksum
    gdb_send_char('#');
    gdb_send_char(checksum_chars[0]);
    gdb_send_char(checksum_chars[1]);

    // wait for reply
    int timeout = 1000;
    do 
    {
        char r = gdb_recv_char();
        if (r == '+') {
            // ack
            break;
        } else if (r == '-') {
            // resend
            goto _resend;
        } else {
            // something else? we can't do anything with that, wait for next char
            continue;
        }
    } while (timeout-- > 0);
}

static void reply_not_supported(void)
{
    send_packet("-");
}

static void reply_success(void)
{
    send_packet("OK");
}

// CPU exception callback
static void gdb_exception_handler(int vecnum, struct cpu_context* ctx)
{
    while(1) {
        recv_packet();
        switch(g_rx_buffer[0])
        {
        case '!':
            reply_not_supported();
            break;

        case '?':
        case 'q':
        case 'g':
        case 'G':
        case 'm':
        case 'M':
        case 'c':
        case 'd':
        case 'k':

        default:
            reply_not_supported();
            break;
        }
    }   
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void gdb_start_stub(void)
{
    dbg_install_exception_handler(gdb_exception_handler);
    __asm__ ("int $3");

}
//////////////////////////////////////////////////////////////////////////////////////////////////

