; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Stack_Size      EQU     0x00000200

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size       EQU     0x00000100

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit


                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       DCD     __initial_sp              ; Top of Stack
                DCD     Reset_Handler             ; Reset Handler
                DCD     NMI_Handler               ; NMI Handler
                DCD     HardFault_Handler         ; Hard Fault Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     SVC_Handler               ; SVCall Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     PendSV_Handler            ; PendSV Handler
                DCD     SysTick_Handler           ; SysTick Handler
                DCD     0                         ; Reserved
                DCD     WDT_Handler               ; WDT Handler
                DCD     RTC_Handler               ; RTC Handler
                DCD     UART0_Handler             ; UART 0 Handler
                DCD     UART1_Handler             ; UART 1 Handler
                DCD     UART2_Handler             ; UART 2 Handler
                DCD     UART3_Handler             ; UART 3 Handler
                DCD     UART4_Handler             ; UART 4 Handler
                DCD     UART5_Handler             ; UART 5 Handler
                DCD     SPI_Handler               ; SPI Handler
                DCD     CFLASH_Handler            ; Code Flash Handler
                DCD     DFLASH_Handler            ; Data Flash Handler
                DCD     TIMER0_Handler            ; Timer 0 Handler
                DCD     TIMER1_Handler            ; Timer 1 Handler
                DCD     TIMER2_Handler            ; Timer 2 Handler
                DCD     TIMER3_Handler            ; Timer 3 Handler
                DCD     TIMER4_Handler            ; TIMER 4 handler
                DCD     EXT0_Handler              ; EXT 0 Handler
                DCD     EXT1_Handler              ; EXt 1 Handler
                DCD     EXT2_Handler              ; EXT 2 Handler
                DCD     EXT3_Handler              ; EXT 3 Handler
                DCD     LCD_Handler               ; LCD Handler
                DCD     ADC_Handler               ; ADC Handler
                DCD     GPIOA_Handler             ; GPIO A Handler
                DCD     GPIOB_Handler             ; GPIO B Handler
                DCD     GPIOC_Handler             ; GPIO C Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     SINT_Handler              ; Reserved
__Vectors_End

__Vectors_Size         EQU     __Vectors_End - __Vectors
                AREA    |.text|, CODE, READONLY


; Reset Handler

Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  __main
                IMPORT  SystemInit

                LDR     R0, =SystemInit
                BLX     R0
                LDR     R0, =__main
                BX      R0
                ENDP


; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     	PROC
                	EXPORT  NMI_Handler               [WEAK]
                	B       .
                	ENDP
HardFault_Handler	PROC
                	EXPORT  HardFault_Handler         [WEAK]
                	B       .
                	ENDP
SVC_Handler     	PROC
                	EXPORT  SVC_Handler               [WEAK]
                	B       .
                	ENDP
PendSV_Handler  	PROC
                	EXPORT  PendSV_Handler            [WEAK]
                	B       .
                	ENDP
SysTick_Handler 	PROC
                	EXPORT  SysTick_Handler            [WEAK]
                	B       .
                	ENDP
Default_Handler 	PROC
                	EXPORT RTC_Handler                [WEAK]
                	EXPORT WDT_Handler                [WEAK]
                	EXPORT UART0_Handler              [WEAK]
                	EXPORT UART1_Handler              [WEAK]
                	EXPORT UART2_Handler              [WEAK]
                	EXPORT UART3_Handler              [WEAK]
                	EXPORT UART4_Handler              [WEAK]
                	EXPORT UART5_Handler              [WEAK]
                	EXPORT SPI_Handler                [WEAK]
                	EXPORT CFLASH_Handler             [WEAK]
                	EXPORT DFLASH_Handler             [WEAK]
                	EXPORT TIMER0_Handler             [WEAK]
                	EXPORT TIMER1_Handler             [WEAK]
                	EXPORT TIMER2_Handler             [WEAK]
                	EXPORT TIMER3_Handler             [WEAK]
                	EXPORT TIMER4_Handler             [WEAK]
                	EXPORT EXT0_Handler               [WEAK]
                	EXPORT EXT1_Handler               [WEAK]
                	EXPORT EXT2_Handler               [WEAK]
                	EXPORT EXT3_Handler               [WEAK]
                	EXPORT LCD_Handler                [WEAK]
                	EXPORT ADC_Handler                [WEAK]
                	EXPORT GPIOA_Handler              [WEAK]
                	EXPORT GPIOB_Handler              [WEAK]
                	EXPORT GPIOC_Handler              [WEAK]
                	EXPORT DUMMY_Handler              [WEAK]
                  EXPORT SINT_Handler               [WEAK]

RTC_Handler
WDT_Handler
RTC_Handlder
UART0_Handler
UART1_Handler
UART2_Handler
UART3_Handler
UART4_Handler
UART5_Handler
SPI_Handler
CFLASH_Handler
DFLASH_Handler
TIMER0_Handler
TIMER1_Handler
TIMER2_Handler
TIMER3_Handler
TIMER4_Handler
EXT0_Handler
EXT1_Handler
EXT2_Handler
EXT3_Handler
LCD_Handler
ADC_Handler
GPIOA_Handler
GPIOB_Handler
GPIOC_Handler
SINT_Handler
DUMMY_Handler
                B       .
                ENDP

                ALIGN


; User Initial Stack & Heap

                IF      :DEF:__MICROLIB

                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit

                ELSE

                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap
__user_initial_stackheap

                LDR     R0, =  Heap_Mem
                LDR     R1, =(Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR

                ALIGN

                ENDIF


                END

