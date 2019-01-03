#include "gct301s.h"
#include "ntshell.h"
#include "ntopt.h"
#include "ntlibc.h"
#include "ntstdio.h"

ntstdio_t handle;

//-----------------------------------------------------------------------------
// Select UART to use
#define USE_UART    0

//-----------------------------------------------------------------------------
#if (USE_UART==0)
UART_TypeDef *uart = UART0;
#elif (USE_UART==1)
UART_TypeDef *uart = UART1;
#elif (USE_UART==2)
UART_TypeDef *uart = UART2;
#elif (USE_UART==3)
UART_TypeDef *uart = UART3;
#elif (USE_UART==4)
UART_TypeDef *uart = UART4;
#elif (USE_UART==5)
UART_TypeDef *uart = UART5;
#else
UART_TypeDef *uart = UART0;
#endif

typedef enum {
    BAUD_57600 = 0,
    BAUD_28800 = 1,
    BAUD_19200 = 2,
    BAUD_9600  = 5,
    BAUD_4800  = 11, 
} BAUD_ENUM;

volatile uint32_t SYSTICK = 0;

//-----------------------------------------------------------------------------
void SysTick_Handler(void)
{
    (void)SysTick->CTRL;        // read to clear flag
    SYSTICK++;
}

void delay_ms(uint32_t d)
{
    uint32_t end_tick = SYSTICK +  d;

    while (SYSTICK < end_tick) {
        __NOP();
    }
}

//-----------------------------------------------------------------------------
void GPIO_Init(void)
{
#if (USE_UART==0)
    GPIOC->ALTFUNCSET = BV(1) | BV(0);
#elif (USE_UART==1)
    GPIOC->ALTFUNCSET = BV(3) | BV(2);
#elif (USE_UART==2)
    GPIOC->ALTFUNCSET = BV(5) | BV(4);
#elif (USE_UART==3)
    GPIOA->ALTFUNCSET = BV(11) | BV(10);    // GPIOA
#elif (USE_UART==4)
    GPIOC->ALTFUNCSET = BV(7) | BV(6);
#elif (USE_UART==5)
    GPIOA->ALTFUNCSET = BV(7) | BV(6);      // GPIOA
#else
    GPIOC->ALTFUNCSET = BV(1) | BV(0);
#endif    
    
}

//-----------------------------------------------------------------------------
void DPLL_Init(void)
{
    // turn on DPLL
    SYSCTRL->DPLL_CTL = 0x1;
    while ((SYSCTRL->SYS_CLK_STA & 0x1) == 0);
}

//-----------------------------------------------------------------------------
void SER_Init(void)
{
    // uart frame settings
    uart->CLKEN = 0;
    while ((uart->CLKEN & 0x1) == 0x1);

    uart->BAUD = BAUD_57600; 
    
    uart->CLKEN = 1;
    while ((uart->CLKEN & 0x1) == 0);

    // enable trx
    uart->ENABLE = 0x3;
    while ((uart->ENABLE & 0x3) != 0x3);
}

int32_t SER_put(uint8_t c)
{
    while ((uart->STATE & 0x1) == 0);
    uart->DATA = c;
    return c;
}

int32_t SER_get(void)
{
    while ((uart->STATE & 0x2) == 0);
    return uart->DATA;
}

//-----------------------------------------------------------------------------
uint32_t dec_str_to_uint(char *str)
{
    int sum = 0;
    
    // discard whitespace
    while (*str == ' ') {
        str++;
    }
    
    while ((*str >= '0' && *str <= '9') && (*str != '\0')) {
        sum = (sum * 10) + (*str - '0');
        str++;
    }     
    return sum;
}

uint32_t hex_str_to_uint(char *str)
{
    uint32_t sum = 0;
    char c;

    // discard whitespace
    while (*str == ' ') {
        str++;
    }
    
    while (1) {
        c = *str++;
        if ( c >= '0' && c <= '9' ) {
            c = c - '0';
        }
        else if ( c >= 'A' && c <= 'F' ) {
            c = c - 'A' + 10;
        }
        else if ( c >= 'a' && c <= 'f' ) {
            c = c - 'a' + 10;
        }
        else {
            break;
        }
        sum = (sum * 16) + c;
    }     
    return sum;
}

//-----------------------------------------------------------------------------

#define UNUSED_VARIABLE(N)  do { (void)(N); } while (0)

char uart_getc(void)
{
    return (char) SER_get();
}

void uart_putc(char c)
{
    SER_put(c);
}

void uart_puts(const char *s)
{
    char *ptr = (char *)s;
    while (*ptr) {
        uart_putc(*ptr);
        ptr++;
    }   
    return;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

typedef int (*USRCMDFUNC)(int argc, char **argv);

static int usrcmd_ntopt_callback(int argc, char **argv, void *extobj);
static int usrcmd_help(int argc, char **argv);
static int usrcmd_info(int argc, char **argv);
static int usrcmd_mww(int argc, char **argv);
static int usrcmd_mrw(int argc, char **argv);

typedef struct {
    char *cmd;
    char *desc;
    USRCMDFUNC func;
} cmd_table_t;

static const cmd_table_t cmdlist[] = {
    { "help", "This is a description text string for help command.", usrcmd_help },
    { "info", "This is a description text string for info command.", usrcmd_info },
    { "mww", "Memory write word command.", usrcmd_mww },
    { "mrw", "Memory read word command.", usrcmd_mrw },
};

int usrcmd_execute(const char *text)
{
    return ntopt_parse(text, usrcmd_ntopt_callback, 0);
}

static int usrcmd_ntopt_callback(int argc, char **argv, void *extobj)
{
    if (argc == 0) {
        return 0;
    }
    const cmd_table_t *p = &cmdlist[0];
    for (int i = 0; i < sizeof(cmdlist) / sizeof(cmdlist[0]); i++) {
        if (ntlibc_strcmp((const char *)argv[0], p->cmd) == 0) {
            return p->func(argc, argv);
        }
        p++;
    }
    uart_puts("Unknown command found.\r\n");
    return 0;
}

static int usrcmd_help(int argc, char **argv)
{
    const cmd_table_t *p = &cmdlist[0];
    for (int i = 0; i < sizeof(cmdlist) / sizeof(cmdlist[0]); i++) {
        uart_puts(p->cmd);
        uart_puts("\t:");
        uart_puts(p->desc);
        uart_puts("\r\n");
        p++;
    }
    return 0;
}

static int usrcmd_info(int argc, char **argv)
{
    uart_puts("NTShell for GCT301S v0.0.0\r\n");
    return 0;
}

static int usrcmd_mww(int argc, char **argv)
{
    uart_puts("Memory write word\r\n");
    return 0;
}

static int usrcmd_mrw(int argc, char **argv)
{
    uint32_t addr;
    uint32_t data;
    int retval;
    
    uart_puts("Memory read word\r\n");
    if (argc != 2) {
        uart_puts("Missing address\r\n");
        retval = 1;
    }
    else {
        //addr = dec_str_to_uint(argv[1]);
        addr = hex_str_to_uint(argv[1]);
        addr &= ~(0x3); // word aligned
        data = M32(addr);
        ntstdio_printf(&handle, "addr = %08x data = 0x%08x\r\n", addr, data);
        retval = 0;
    }
    return retval;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

static int func_read(char *buf, int cnt, void *extobj)
{
  int i;
  UNUSED_VARIABLE(extobj);
  for (i = 0; i < cnt; i++) {
    buf[i] = uart_getc();
  }
  return cnt;
}

static int func_write(const char *buf, int cnt, void *extobj)
{
  int i;
  UNUSED_VARIABLE(extobj);
  for (i = 0; i < cnt; i++) {
    uart_putc(buf[i]);
  }
  return cnt;
}

static int func_callback(const char *text, void *extobj)
{
  ntshell_t *ntshell = (ntshell_t *)extobj;
  UNUSED_VARIABLE(ntshell);
  UNUSED_VARIABLE(extobj);

#if 0    
  
  if (ntlibc_strlen(text) > 0) {
    uart_puts("User input text:'");
    uart_puts(text);
    uart_puts("'\r\n");
  }
#else
  usrcmd_execute(text);
#endif
  
  return 0;
}

int main(void)
{
    ntshell_t ntshell;

    SYSCTRL->CLK_DIVSEL = 0x4;
    
    // initialize GPIO for TX0 & RX0
    GPIO_Init();
    
    // Enable DPLL for uart clock
    DPLL_Init();
    
    // Configure and enable uart
    SER_Init();
    
    // systick at 1000Hz
    SysTick_Config(4000L);  

    ntstdio_init(&handle, 0, (NTSTDIO_XI) uart_getc, (NTSTDIO_XO) uart_putc);
    
    ntshell_init(&ntshell, func_read, func_write, func_callback, (void *)&ntshell);
    uart_puts("\r\n");
    ntshell_set_prompt(&ntshell, "> ");
    
    ntshell_execute(&ntshell);
    
    return 0;
}
