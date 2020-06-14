//#define uint32_t unsigned int
#define STACK uint32_t
#define U32 uint32_t
#define U8 unsigned char
#define BOOL char
#define NULL (void*)0
#define TRUE 0
#define FALUSE -1
typedef void (*TASK_ENTRY) (void *);

/*for task control block*/
typedef struct TCB_STR {
	STACK *stack_ptr;
	//TICK delay;
	U8 *name;
	//LIST list;
	U8 prio;
	//BOOL state;
} TCB;

typedef struct cpu_context_st {
    uint32_t          mepc;
    uint32_t          mstatus;
    union { uint32_t  x1,  ra; };
    union { uint32_t  x3,  gp; };
    union { uint32_t  x4,  tp; };
    union { uint32_t  x5,  t0; };
    union { uint32_t  x6,  t1; };
    union { uint32_t  x7,  t2; };
    union { uint32_t  x8,  s0, fp; };
    union { uint32_t  x9,  s1; };
    union { uint32_t x10,  a0; };
    union { uint32_t x11,  a1; };
    union { uint32_t x12,  a2; };
    union { uint32_t x13,  a3; };
    union { uint32_t x14,  a4; };
    union { uint32_t x15,  a5; };
    union { uint32_t x16,  a6; };
    union { uint32_t x17,  a7; };
    union { uint32_t x18,  s2; };
    union { uint32_t x19,  s3; };
    union { uint32_t x20,  s4; };
    union { uint32_t x21,  s5; };
    union { uint32_t x22,  s6; };
    union { uint32_t x23,  s7; };
    union { uint32_t x24,  s8; };
    union { uint32_t x25,  s9; };
    union { uint32_t x26, s10; };
    union { uint32_t x27, s11; };
    union { uint32_t x28,  t3; };
    union { uint32_t x29,  t4; };
    union { uint32_t x30,  t5; };
    union { uint32_t x31,  t6; };
} cpu_context_t;

void *stack_init(STACK *stack_base, uint32_t stack_size, TASK_ENTRY entry, void *arg, void *task_exit)
{
    STACK *stk;
    cpu_context_t *regs = 0;

    stk = (uint32_t *)&stack_base[stack_size];
    stk = (uint32_t *)((uint32_t)(stk) & 0xFFFFFFFC);

    stk  -= (sizeof(cpu_context_t)/sizeof(uint32_t));

    regs = (cpu_context_t*) stk;

    for(int i=1; i<(sizeof(cpu_context_t)/sizeof(uint32_t)); i++) {
        *(stk + i) = 0xACEADD00 | ((i / 10) << 4) | (i % 10);
    }

    //uint32_t gp = 0;
    volatile uint32_t gp;
    asm("mv %0, gp":"=r"(gp));
    //__ASM__ __VOLATILE__ ("mv %0, gp":"=r"(gp));

    regs->gp        = (uint32_t)gp;           // global pointer
    regs->a0        = (uint32_t)arg;          // argument
    regs->ra        = (uint32_t)task_exit;         // return address
    regs->mstatus   = (uint32_t)0x00001880;   // return to machine mode and enable interrupt
    regs->mepc      = (uint32_t)entry;        // task entry

    return (STACK*)stk;
}

void *_memset(void *dest, int c, int n)
{
    char *p = dest;
    while (n-- > 0) {
        *(char*)dest++ = c;
    }
    return p;
}

void task_exit()
{
	//os_printf("Task return\n");
	//while (1)
	//	os_delay(100);
	sifive_uart_putchar('o');
}

void task_ilde()
{
	sifive_uart_init();
	sifive_test_init();
	sifive_uart_putchar('w');
	sifive_uart_putchar('o');
	sifive_test_poweroff();

	;
}

uint32_t task_create(TCB *tcb, U8 *name, TASK_ENTRY fun, void *arg, STACK *stack,
	       U32 stack_size, U8 prio, BOOL state)
{
#if 0
	if (tcb == NULL || fun == NULL || stack == NULL) {
		OS_LOG("Task error")
		    return NO_TCB;
	}
#endif
	_memset(stack, 0, stack_size);
	tcb->stack_ptr = stack_init(stack, stack_size, fun, arg, task_exit);
	tcb->name = name;
	tcb->prio = prio;
	//tcb->state = state;

	//prio_ready_queue_insert_tail(tcb);

	return TRUE;
}

TCB *new_task;
TCB *old_task;
TCB idle_tcb;
#define IDLE_STACK_SIZE 1024
U32 idle_stack[IDLE_STACK_SIZE];

extern void start_schedule(void);

/*Idle task*/
void idle_task(void *arg)
{
	for (;;) {
		/*do low power action or low priority thing, then schedule*/
		//schedule();
		sifive_uart_putchar('o');
	}
}
