/*
*********************************************************************************************************
*                                            EXAMPLE CODE
*
*               This file is provided as an example on how to use Micrium products.
*
*               Please feel free to use any application code labeled as 'EXAMPLE CODE' in
*               your application products.  Example code may be used as is, in whole or in
*               part, or may be used as a reference only. This file can be modified as
*               required to meet the end-product requirements.
*
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*
*               You can find our product's user manual, API reference, release notes and
*               more information at https://doc.micrium.com.
*               You can contact us at www.micrium.com.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                              uC/OS-II
*                                            EXAMPLE CODE
*
* Filename : main.c
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  <cpu.h>
#include  <lib_mem.h>
#include  <os.h>

#include  "app_cfg.h"


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

static void task1(void* p_arg);
static void task2(void* p_arg);
static void task3(void* p_arg);
static void mywait(int tick);

/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

static  OS_STK  StartupTaskStk[APP_CFG_STARTUP_TASK_STK_SIZE];

#define TASK_STACKSIZE 2048

static OS_STK Task1_STK[TASK_STACKSIZE]; 
static OS_STK Task2_STK[TASK_STACKSIZE];
static OS_STK Task3_STK[TASK_STACKSIZE];

#define TASK1_ID 1 //定義task1的id、priority、arrival、execution、period
#define TASK1_PRIORITY 3
#define TASK1_ARRIVAL 2
#define TASK1_EXECUTION 6
#define TASK1_PERIOD 30

#define TASK2_ID 2 //定義task2的id、priority、arrival、execution、period
#define TASK2_PRIORITY 4
#define TASK2_ARRIVAL 0
#define TASK2_EXECUTION 7
#define TASK2_PERIOD 60

/*#define TASK3_ID 3 //定義task3的id、priority、arrival、execution、period
#define TASK3_PRIORITY 5
#define TASK3_ARRIVAL 0
#define TASK3_EXECUTION 7
#define TASK3_PERIOD 90*/

//R1的ID為1   R2的ID為2
#define R1_ID 1
#define R1_PRIO 1
#define R2_ID 2
#define R2_PRIO 2

OS_EVENT* R1;
OS_EVENT* R2;

/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  StartupTask(void* p_arg);


/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary initialization.
*
* Arguments   : none
*
* Returns     : none
*
* Notes       : none
*********************************************************************************************************
*/

int  main(void)
{
#if OS_TASK_NAME_EN > 0u
    CPU_INT08U  os_err;
#endif


    CPU_IntInit();

    Mem_Init();                                                 /* Initialize Memory Managment Module                   */
    CPU_IntDis();                                               /* Disable all Interrupts                               */
    CPU_Init();                                                 /* Initialize the uC/CPU services                       */

    OSInit();                                                   /* Initialize uC/OS-II                                  */

    /*OSTaskCreateExt( StartupTask,                               // Create the startup task
                     0,
                    &StartupTaskStk[APP_CFG_STARTUP_TASK_STK_SIZE - 1u],
                     APP_CFG_STARTUP_TASK_PRIO,
                     APP_CFG_STARTUP_TASK_PRIO,
                    &StartupTaskStk[0u],
                     APP_CFG_STARTUP_TASK_STK_SIZE,
                     0u,
                    (OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

#if OS_TASK_NAME_EN > 0u
    OSTaskNameSet(APP_CFG_STARTUP_TASK_PRIO,
        (INT8U*)"Startup Task",
        &os_err);
#endif*/

#ifdef TASK1_ID //假如task1有被定義的話 就創建task1工作
    OSTaskCreateExt(task1,
        0,
        &Task1_STK[TASK_STACKSIZE - 1u],
        TASK1_PRIORITY,
        TASK1_ID,
        &Task1_STK[0u],
        TASK_STACKSIZE,
        0u,
        (OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR),
        TASK1_ARRIVAL,
        TASK1_EXECUTION,
        TASK1_PERIOD);
#endif // TASK1_ID


#ifdef TASK2_ID //假如task2有被定義的話 就創建task2工作
    OSTaskCreateExt(task2,
        0,
        &Task2_STK[TASK_STACKSIZE - 1u],
        TASK2_PRIORITY,
        TASK2_ID,
        &Task2_STK[0u],
        TASK_STACKSIZE,
        0u,
        (OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR),
        TASK2_ARRIVAL,
        TASK2_EXECUTION,
        TASK2_PERIOD);
#endif // TASK2_ID

#ifdef TASK3_ID //假如task3有被定義的話 就創建task3工作
    OSTaskCreateExt(task3,
        0,
        &Task3_STK[TASK_STACKSIZE - 1u],
        TASK3_PRIORITY,
        TASK3_ID,
        &Task3_STK[0u],
        TASK_STACKSIZE,
        0u,
        (OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR),
        TASK3_ARRIVAL,
        TASK3_EXECUTION,
        TASK3_PERIOD);
#endif // TASK3_ID

    INT8U err;
    R1 = Jack_OSMutexCreate(R1_PRIO, &err, R1_ID);
    R2 = Jack_OSMutexCreate(R2_PRIO, &err, R2_ID);

    OSTimeSet(0); //重新歸零timetick

    OSStart();                                                  /* Start multitasking (i.e. give control to uC/OS-II)   */

    while (DEF_ON) {                                            /* Should Never Get Here.                               */
        ;
    }
}


/*
*********************************************************************************************************
*                                            STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
*
* Arguments   : p_arg   is the argument passed to 'StartupTask()' by 'OSTaskCreate()'.
*
* Returns     : none
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/


static  void  StartupTask(void* p_arg)
{
    (void)p_arg;

    OS_TRACE_INIT();                                            /* Initialize the uC/OS-II Trace recorder               */

#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&err);                               /* Compute CPU capacity with no task running            */
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();
#endif

    APP_TRACE_DBG(("uCOS-III is Running...\n\r"));

    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.       */
        OSTimeDlyHMSM(0u, 0u, 1u, 0u);
        APP_TRACE_DBG(("Time: %d\n\r", OSTimeGet()));
    }
}

void mywait(int tick)
{
#if OS_CRITICAL_METHOD == 3
    OS_CPU_SR cpu_sr = 0;
#endif

    int now, exit;
    OS_ENTER_CRITICAL();
    now = OSTCBCur->runtime; //now 為目前task執行多久的時間點
    exit = now + tick; //exit 為now加等待多久tick
    OS_EXIT_CRITICAL();
    while (1)
    {
        //OSTCBCur->runtime會在只有在當前task在執行時才會加時間
        //exit只要小於等於runtime時就會跳出迴圈
        if (exit <= OSTCBCur->runtime)
            break;
    }
}

void task1(void* p_arg) //task1的執行程式
{
    (void)p_arg;
    INT8U err;

    while (1)
    {
        //假如task開始執行時 還沒到達arrival time那就執行 OSTimeDly休息相差的時間
        if (OSTCBCur->arrival > OSTimeGet())
        {
            OSTimeDly(OSTCBCur->arrival - OSTimeGet());
        }
        else
        {
            while (1)
            {
                //第一小題的題目設定
                /*mywait(1);

                OSMutexPend(R1, 0, &err);
                mywait(3);

                OSMutexPost(R1);
                mywait(1);

                //printf("task 1 sleep %d\n", OSTCBCur->deadline - OSTimeGet());
                OSTimeDly(OSTCBCur->deadline - OSTimeGet());*/


                //第二小題的題目設定
                mywait(1);

                OSMutexPend(R1, 0, &err);
                mywait(2);

                OSMutexPend(R2, 0, &err);
                mywait(2);

                OSMutexPost(R2);
                OSMutexPost(R1);
                mywait(1);

                OSTimeDly(OSTCBCur->deadline - OSTimeGet());
            }
        }
    }
}

void task2(void* p_arg) //task2的執行程式
{
    (void)p_arg;
    INT8U err;

    while (1)
    {
        //假如task開始執行時 還沒到達arrival time那就執行 OSTimeDly休息相差的時間
        if (OSTCBCur->arrival > OSTimeGet())
        {
            OSTimeDly(OSTCBCur->arrival - OSTimeGet());
        }
        else
        {

            while (1)
            {
                //第一小題的題目設定
                /*mywait(3);

                OSTimeDly(OSTCBCur->deadline - OSTimeGet());*/

                //第二小題的題目設定
                mywait(1);

                OSMutexPend(R2, 0, &err);
                mywait(3);

                OSMutexPend(R1, 0, &err);
                mywait(2);

                OSMutexPost(R1);
                OSMutexPost(R2);
                mywait(1);

                OSTimeDly(OSTCBCur->deadline - OSTimeGet());

            }
        }
    }
}

void task3(void* p_arg) //task3的執行程式
{
    (void)p_arg;
    INT8U err;

    while (1)
    {
        //假如task開始執行時 還沒到達arrival time那就執行 OSTimeDly休息相差的時間
        if (OSTCBCur->arrival > OSTimeGet())
        {
            OSTimeDly(OSTCBCur->arrival - OSTimeGet());
        }
        else
        {

            while (1)
            {
                //第一小題的題目設定
                mywait(1);

                OSMutexPend(R2, 0, &err);
                mywait(5);

                OSMutexPost(R2);
                mywait(1);

                OSTimeDly(OSTCBCur->deadline - OSTimeGet());

            }
        }
    }
}

