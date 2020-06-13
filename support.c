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

void *stack_init(STACK *stack_base, uint32_t stack_size, TASK_ENTRY entry, void *arg, void *task_exit)
{
    STACK *stk;
    register int *gp asm("x3");
    uint32_t temp = (uint32_t)(stack_base + stack_size);

    temp &= 0xFFFFFFFCUL;

    stk = (uint32_t *)temp;

    *(--stk) = (uint32_t)entry;                   /* PC            */
    *(--stk) = (uint32_t)0xbbbbbbbbL;             /* X15           */
    *(--stk) = (uint32_t)0xaaaaaaaaL;             /* X14           */
    *(--stk) = (uint32_t)0x99999999L;             /* X13           */
    *(--stk) = (uint32_t)0x88888888L;             /* X12           */
    *(--stk) = (uint32_t)0x77777777L;             /* X11           */
    *(--stk) = (uint32_t)arg;                     /* X10           */
    *(--stk) = (uint32_t)0x66666666L;             /* X9            */
    *(--stk) = (uint32_t)0x55555555L;             /* X8            */
    *(--stk) = (uint32_t)0x44444444L;             /* X7            */
    *(--stk) = (uint32_t)0x33333333L;             /* X6            */
    *(--stk) = (uint32_t)0x22222222L;             /* X5            */
    *(--stk) = (uint32_t)0x11111111L;             /* X4            */
    *(--stk) = (uint32_t)gp;                      /* X3            */
    *(--stk) = (uint32_t)task_exit;    		  /* X1            */

    return stk;	
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
