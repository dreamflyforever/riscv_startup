extern char _bss_start;
extern char _bss_end;
extern char _memory_end;

#define uint32_t unsigned int
int main();
int trap_handler();

enum {
	HART0_CLOCK_FREQ           = 0x00010000,
	UART0_CLOCK_FREQ           = 0x00011000,
	UART0_BAUD_RATE            = 0x00011100,
	NS16550A_UART0_CTRL_ADDR   = 0x00030000,
	RISCV_HTIF_BASE_ADDR       = 0x00050000,
	SIFIVE_CLINT_CTRL_ADDR     = 0x55550000,
	SIFIVE_CLIC_CRTL_ADDR      = 0x55550001,
	SIFIVE_TEST_CTRL_ADDR      = 0x55550002,
	SIFIVE_UART0_CTRL_ADDR     = 0x55550010,
	SIFIVE_GPIO0_CTRL_ADDR     = 0x55550020,
	SIFIVE_SPI0_CTRL_ADDR      = 0x55550030,
};

enum {
	/* UART Registers */
	UART_REG_TXFIFO = 0,
	UART_REG_RXFIFO = 1,
	UART_REG_TXCTRL = 2,
	UART_REG_RXCTRL = 3,
	UART_REG_IE     = 4,
	UART_REG_IP     = 5,
	UART_REG_DIV    = 6,

	/* TXCTRL register */
	UART_TXEN       = 1,
	UART_TXSTOP     = 2,

	/* RXCTRL register */
	UART_RXEN       = 1,

	/* IP register */
	UART_IP_TXWM    = 1,
	UART_IP_RXWM    = 2
};

typedef struct auxval {
	unsigned long key;
	unsigned long val;
} auxval_t;

static volatile int *uart;

auxval_t __auxv[] = {
	{ UART0_CLOCK_FREQ,         32000000   },
	{ UART0_BAUD_RATE,          115200     },
	{ SIFIVE_UART0_CTRL_ADDR,   0x10013000 },
	{ SIFIVE_TEST_CTRL_ADDR,    0x100000   },
	{ 0, 0 }
};

static volatile uint32_t *test;

enum {
	SIFIVE_TEST_FAIL = 0x3333,
	SIFIVE_TEST_PASS = 0x5555,
};

unsigned long getauxval(unsigned long key)
{
	auxval_t *auxv = __auxv;
	while(auxv->key) {
		if (auxv->key == key) {
			return auxv->val;
		}
		auxv++;
	}
	return 0;
}

static void sifive_test_init()
{
	test = (uint32_t *)(void *)getauxval(SIFIVE_TEST_CTRL_ADDR);
}

static void sifive_test_poweroff()
{
	*test = SIFIVE_TEST_PASS;
	while (1) {
		asm volatile("");
	}
}


static void sifive_uart_init()
{
	uart = (int *)(void *)getauxval(SIFIVE_UART0_CTRL_ADDR);
	uint32_t uart_freq = getauxval(UART0_CLOCK_FREQ);
	uint32_t baud_rate = getauxval(UART0_BAUD_RATE);
	uint32_t divisor = uart_freq / baud_rate - 1;
	uart[UART_REG_DIV] = divisor;
	uart[UART_REG_TXCTRL] = UART_TXEN;
	uart[UART_REG_RXCTRL] = UART_RXEN;
	uart[UART_REG_IE] = 0;
}

static int sifive_uart_putchar(int ch)
{
	while (uart[UART_REG_TXFIFO] < 0);
	return uart[UART_REG_TXFIFO] = ch & 0xff;
}

__attribute__((noreturn)) void _main()
{
	main();
	__builtin_unreachable();
}

#include "support.c"
int main()
{
	int i = 1 ;
	sifive_uart_init();
	sifive_test_init();
	sifive_uart_putchar('h');
	sifive_uart_putchar('e');
	/*Create idle task */
	task_create(&idle_tcb, (U8 *) "idle_task", idle_task, NULL, idle_stack,
		    IDLE_STACK_SIZE, 31, 1);

	sifive_uart_putchar('l');
	new_task = &idle_tcb;
	sifive_uart_putchar('r');
	start_schedule();
	sifive_uart_putchar('e');
	sifive_test_poweroff();
	return 0;
}

int trap_handler()
{
	return 0;
}
