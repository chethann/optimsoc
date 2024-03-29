/* Copyright (c) 2012-2014 by the author(s)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * Author(s):
 *   Stefan Wallentowitz <stefan.wallentowitz@tum.de>
 */

#ifndef OPTIMSOC_BAREMETAL_H_
#define OPTIMSOC_BAREMETAL_H_

// Internal defines
#define OPTIMSOC_NA_BASE        0xe0000000
#define OPTIMSOC_NA_CONF        OPTIMSOC_NA_BASE + 0x00000
#define OPTIMSOC_NA_CONF_TILEID OPTIMSOC_NA_CONF + 0x0
#define OPTIMSOC_NA_CONF_XDIM   OPTIMSOC_NA_CONF + 0x4
#define OPTIMSOC_NA_CONF_YDIM   OPTIMSOC_NA_CONF + 0x8
#define OPTIMSOC_NA_CONF_COREBASE OPTIMSOC_NA_CONF + 0x10
#define OPTIMSOC_NA_CONF_NUMCORES OPTIMSOC_NA_CONF + 0x14

#define OPTIMSOC_NA_CONF_MODS     OPTIMSOC_NA_CONF + 0xc
#define OPTIMSOC_NA_CONF_MPSIMPLE 0x1
#define OPTIMSOC_NA_CONF_DMA      0x2

#define OPTIMSOC_MPSIMPLE       OPTIMSOC_NA_BASE + 0x100000
#define OPTIMSOC_MPSIMPLE_SEND  OPTIMSOC_MPSIMPLE + 0x0
#define OPTIMSOC_MPSIMPLE_RECV  OPTIMSOC_MPSIMPLE + 0x0

#define OPTIMSOC_MPSIMPLE_STATUS_WAITING OPTIMSOC_MPSIMPLE + 0x18

#define OPTIMSOC_DEST_MSB 31
#define OPTIMSOC_DEST_LSB 27
#define OPTIMSOC_CLASS_MSB 26
#define OPTIMSOC_CLASS_LSB 24
#define OPTIMSOC_CLASS_NUM 8
#define OPTIMSOC_SRC_MSB 23
#define OPTIMSOC_SRC_LSB 19

#include <stdint.h>
#include <stdlib.h>

#include <spr-defs.h>
#include <or1k-support.h>

/**
 * \defgroup libbaremetal Baremetal library
 */

/**
 * \defgroup tracing OpTiMSoC tracing
 *
 * @{
 */

/**
 * The instruction sequence issued as a trace event
 *
 * \param id The identifier of the trace (static value)
 * \param v  The value of the trace event
 */
#define OPTIMSOC_TRACE(id,v)                     \
        asm("l.addi\tr3,%0,0": :"r" (v) : "r3"); \
        asm("l.nop %0": :"K" (id));

/**
 * Define a section for the software tracing
 *
 * This initializes a section with a given id and sets a name, that can be
 * used for display or similar.
 *
 * \param id Section identifier
 * \param name Name of this section
 */
extern void optimsoc_trace_definesection(int id, char* name);

extern void optimsoc_trace_defineglobalsection(int id, char* name);

/**
 * Trace a section enter
 *
 * Enter a section and trace this to the debug system.
 *
 * \param id Section entered
 */
extern void optimsoc_trace_section(int id);

/**
 * Trace a kernel enter
 *
 * On entering the kernel trace this to the debug system.
 */
extern void optimsoc_trace_kernelsection(void);

/**
 * @}
 */

/**
 * \defgroup system OpTiMSoC system functions
 *
 * @{
 */

// TODO: gets removed (to libbaremetal)
typedef struct optimsoc_conf {

} optimsoc_conf;

extern void optimsoc_init(optimsoc_conf *config);

/**
 * Get the tile identifier
 *
 * \ingroup system
 * \return Identifier of this tile
 */
static inline unsigned int optimsoc_get_tileid(void) {
    return REG32(OPTIMSOC_NA_BASE);
}

/**
 * Get the core id, relative in this tile
 *
 * \ingroup system
 * \return relative core identifier
 */
static inline unsigned int optimsoc_get_relcoreid(void) {
    return or1k_mfspr(SPR_COREID);
}

static inline unsigned int optimsoc_get_tilenumcores(void) {
    return or1k_mfspr(SPR_NUMCORES);
}

static inline unsigned int optimsoc_get_abscoreid(void) {
    return REG32(OPTIMSOC_NA_CONF_COREBASE) + optimsoc_get_relcoreid();
}

uint32_t optimsoc_get_domain_coreid();
uint32_t optimsoc_get_domain_numcores();

/**
 * Get the number of compute tiles
 *
 * \ingroup system
 * \return Number of compute tiles
 */
extern int optimsoc_ctnum(void);

/**
 * Generate rank of this compute tile in all compute tiles
 *
 * \ingroup system
 * This gives the rank in the set of compute tiles.
 * For example in a system where a compute tile is at position 0 and
 * one at position 3, they will get this output
 *  tile 0 -> ctrank 0
 *  tile 3 -> ctrank 1
 *
 * \return rank of this tile
 */
extern int optimsoc_ctrank(void);

/**
 * Generate rank of given compute tile in all compute tiles
 *
 * \ingroup system
 * This gives the rank in the set of compute tiles.
 * For example in a system where a compute tile is at position 0 and
 * one at position 3, they will get this output
 *  tile 0 -> ctrank 0
 *  tile 3 -> ctrank 1
 *
 * \param tile Tile to look up
 * \return rank of this tile
 */
extern int optimsoc_tilerank(unsigned int tile);

/**
 * Get the tile that has the given rank
 *
 * This is the reverse of optimsoc_tilerank and generates the tile
 * identifier for the given rank.
 *
 * \param rank The rank to lookup
 */
extern int optimsoc_ranktile(unsigned int rank);

/**
 * \defgroup userio User I/O
 * \ingroup libbaremetal
 * @{
 */

extern int lcd_set(unsigned int row,unsigned int col,char c);
extern void uart_printf(const char *fmt, ...);

/**
 * @}
 */

extern uint32_t optimsoc_noc_maxpacketsize();
extern uint32_t optimsoc_has_hostlink();
extern uint32_t optimsoc_hostlink();
extern uint32_t optimsoc_has_uart();
extern uint32_t optimsoc_uarttile();
extern uint32_t optimsoc_uart_lcd_enable();
extern uint32_t optimsoc_compute_tile_num();
extern uint32_t optimsoc_compute_tile_id(uint32_t);

/**
 * @}
 */
// TODO: Remove
extern void uart_printf(const char *fmt, ...);

/**
 * \defgroup utility Utility functions
 *
 * Utility functions
 *
 * @{
 */

/**
 * extract bits between MSB and LSB (including)
 *
 *  \code
 *  return x[MSB:LSB];
 *  \endcode
 *
 * \param x Value to extract from
 * \param msb MSB of extraction
 * \param lsb LSB of extraction
 * \return extracted value (aligned to lsb=0)
 *
 */
static inline unsigned int extract_bits(uint32_t x, uint32_t msb,
                                           uint32_t lsb) {
    return ((x>>lsb) & ~(~0 << (msb-lsb+1)));
}

/**
 * Set bits in variable between MSB and LSB (including)

 *  \code
 *  x[MSB:LSB] = v;
 *  \endcode
 *
 * \param x Pointer to value where to set
 * \param v Value to set
 * \param msb MSB of part to set
 * \param lsb LSB of part to set
 */
// FIXME move to optimsoc.c
static inline void set_bits(uint32_t *x, uint32_t v, uint32_t msb,
                              uint32_t lsb) {
    if(msb != 31) {
    *x = (((~0 << (msb+1) | ~(~0 << lsb))&(*x)) | ((v & ~(~0<<(msb-lsb+1))) << lsb));
    } else {
    /* only the last 5 bits from the shift left operand are used -> can not shift 32 bits */
    *x = (((~(~0 << lsb))&(*x)) | ((v & ~(~0<<(msb-lsb+1))) << lsb));
    }
}

/**
 * @}
 */


/**
 * \defgroup sync Support for atomic operations and synchronization
 * \ingroup libbaremetal
 * @{
 */

/**
 * This is a convenience function that disables interrupts
 *
 * The external interrupts and timer interrupts are disabled.
 * optimsoc_critical_end can be used to enable those interrupts again. When
 * calling the latter function the status of the external interrupts and the
 * tick timer before entering this function needs to be restored. Therefore
 * this function returns this value.
 *
 * \code
 * uint32_t restore = optimsoc_critical_begin();
 * // Critical section code
 * optimsoc_critical_end(restore);
 * \endcode
 *
 * \note This only disables the interrupts but does neither manipulate the
 * interrupt mask vector (what you would not expect) and also does not actually
 * stop the timer (what you might expect)
 *
 * \return The current status of enabled interrupts and tick timer
 */
extern uint32_t optimsoc_critical_begin(void);

/**
 * Leave critical section
 *
 * End a critical section and restore interrupt enables. Read the documentation
 * of optimsoc_critical_begin for more details.
 *
 * \param restore Restore vector of interrupt and tick timer enable
 */
extern void optimsoc_critical_end(uint32_t restore);

/**
 * The mutex data type
 *
 * The mutex data type which is actually hidden on purpose.
 */
typedef uint64_t mutex_t;

/**
 * Initialize mutex
 *
 * Initializes a mutex so that you can lock and unlock it later.
 *
 * \param mutex Mutex to initialize
 */
extern void optimsoc_mutex_init(mutex_t *mutex);

/**
 * Lock mutex
 *
 * Lock a mutex
 *
 * \param mutex Mutex to lock
 */
extern void optimsoc_mutex_lock(mutex_t *mutex);

/**
 * Unlock mutex
 *
 * Unlock a mutex
 *
 * \param mutex Mutex to unlock
 */
extern void optimsoc_mutex_unlock(mutex_t *mutex);

/**
 * @}
 */


/**
 * \defgroup dma Direct Memory Access support
 * \ingroup libbaremetal
 * @{
 */

/**
 * OpTiMSoC dma success code
 *
 * The OpTiMSoC dma success code indicated the success of an operation.
 */
typedef enum {
    DMA_SUCCESS = 0,            /*!< Successful operation */
    DMA_ERR_NOTINITIALIZED = 1, /*!< Driver not initialized */
    DMA_ERR_NOSLOT = 2          /*!< No slot available */
} dma_success_t;

/**
 * DMA transfer handle
 *
 * The DMA transfer handle is used when calling the DMA functions
 */
typedef uint32_t dma_transfer_handle_t;

/**
 * The direction of a DMA transfer
 *
 * The direction of a DMA transfer is either from local to a remote tile or
 * vice versa
 */
typedef enum {
    LOCAL2REMOTE=0, /*!< Transfer data from local to remote */
    REMOTE2LOCAL=1  /*!< Transfer data from remore to local */
} dma_direction_t;

/**
 * Initialize DMA driver
 *
 * Initialize the DMA driver. Necessary before calling it the first time
 */
extern void dma_init(void);

/**
 * Allocate a DMA transfer slot and get handle
 *
 * This function allocates a DMA slot. DMA transfers are handled asynchronously
 * and each of the ongoing transfers is controlled by one slot in the DMA
 * controller. You therefore need to allocate a slot before starting transfers.
 *
 * \param[out] The handle of this slot
 * \return Success code
 */
extern dma_success_t dma_alloc(dma_transfer_handle_t *id);

/**
 * Initiate a DMA transfer
 *
 * This function initiates a DMA transfer between the local address in this tile
 * to remote_tile address remote of size. The direction of the transfer is
 * determined by dir. The slot id will be used for this transfer.
 *
 * \param local Local address
 * \param remote_tile Remote tile
 * \param remote Remote address
 * \param size Size of the transfer
 * \param dir Direction of the transfer
 * \param id Handle of the slot as allocated by dma_alloc
 * \return Success code
 */
extern dma_success_t dma_transfer(void* local,
                                    uint32_t remote_tile,
                                    void* remote,
                                    size_t size,
                                    dma_direction_t dir,
                                    dma_transfer_handle_t id);

/**
 * Blocking wait for DMA transfer
 *
 * Wait for a DMA transfer to finish and block.
 *
 * \param id The handle of the transfer slot
 * \return Success code
 */
extern dma_success_t dma_wait(dma_transfer_handle_t id);

/**
 * @}
 */

/**
 * \defgroup mp Message passing support
 * \ingroup libbaremetal
 * @{
 */

/**
 * Initialize simple message passing environment
 */
extern void optimsoc_mp_simple_init(void);

/**
 * Send a message
 *
 * Sends a message of size from buf.
 *
 * \param size Size of the message in (word-sized) flits
 * \param buf Message buffer containint size flits
 */
extern void optimsoc_mp_simple_send(unsigned int size, uint32_t* buf);

/**
 * Add a handler for a class of incoming messages
 *
 * A message header contains a class. This class field can be used to mix
 * different kinds of message services. For each class you are using in your
 * system you need to add a class handler. As there is no default class handler
 * all remaining classes are dropped.
 *
 * \param class Class to register
 * \param hnd Function pointer to handler for this class
 */
extern void optimsoc_mp_simple_addhandler(unsigned int class,
                                               void (*hnd)(unsigned int*,int));

/**
 * @}
 */

#endif
