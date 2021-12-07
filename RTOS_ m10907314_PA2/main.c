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
static void task4(void* p_arg);

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
static OS_STK Task4_STK[TASK_STACKSIZE];

#define TASK1_ID 1 //定義task1的id、priority、arrival、execution、period
#define TASK1_PRIORITY 1
#define TASK1_ARRIVAL 0
#define TASK1_EXECUTION 2
#define TASK1_PERIOD 8
INT32U TASK1_aperiodic = 0; //定義非週期的工作數量
float TASK1_serversize = 0; //定義使用率
static INT32U TASK1_arrival[] = { 0 }; //依序定義工作的arrival
static INT32U TASK1_execution[] = { 0 }; //依序定義工作的execution
static INT32U TASK1_deadline[] = { 0 }; //依序定義工作的deadline

#define TASK2_ID 2 //定義task2的id、priority、arrival、execution、period
#define TASK2_PRIORITY 2
#define TASK2_ARRIVAL 0
#define TASK2_EXECUTION 3
#define TASK2_PERIOD 10
INT32U TASK2_aperiodic = 0; //定義非週期的工作數量
float TASK2_serversize = 0; //定義使用率
static INT32U TASK2_arrival[] = { 0 }; //依序定義工作的arrival
static INT32U TASK2_execution[] = { 0 }; //依序定義工作的execution
static INT32U TASK2_deadline[] = { 0 }; //依序定義工作的deadline

#define TASK3_ID 3 //定義task3的id、priority、arrival、execution、period
#define TASK3_PRIORITY 3
#define TASK3_ARRIVAL 0
#define TASK3_EXECUTION 5
#define TASK3_PERIOD 20
INT32U TASK3_aperiodic = 0; //定義非週期的工作數量
float TASK3_serversize = 0; //定義使用率
static INT32U TASK3_arrival[] = { 0 }; //依序定義工作的arrival
static INT32U TASK3_execution[] = { 0 }; //依序定義工作的execution
static INT32U TASK3_deadline[] = { 0 }; //依序定義工作的deadline

#define TASK4_ID 4 //定義task4的id、priority、arrival、execution、period
#define TASK4_PRIORITY 4
#define TASK4_ARRIVAL 0
#define TASK4_EXECUTION 0
#define TASK4_PERIOD 0
INT32U TASK4_aperiodic = 2; //定義非週期的工作數量
float TASK4_serversize = 0.2; //定義使用率
static INT32U TASK4_arrival[] = { 12,14 }; //依序定義工作的arrival
static INT32U TASK4_execution[] = { 3,2 }; //依序定義工作的execution
static INT32U TASK4_deadline[] = { 28,39 }; //依序定義工作的deadline

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
    Jack_OSTaskCreateExt(task1,
        0u,
        &Task1_STK[TASK_STACKSIZE - 1u],
        TASK1_PRIORITY,
        TASK1_ID,
        &Task1_STK[0u],
        TASK_STACKSIZE,
        0u,
        (OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR),
        TASK1_ARRIVAL,
        TASK1_EXECUTION,
        TASK1_PERIOD,
        TASK1_aperiodic,
        TASK1_serversize,
        &TASK1_arrival,
        &TASK1_execution,
        &TASK1_deadline);
#endif // TASK1_ID


#ifdef TASK2_ID //假如task2有被定義的話 就創建task2工作
    Jack_OSTaskCreateExt(task2,
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
        TASK2_PERIOD,
        TASK2_aperiodic,
        TASK2_serversize,
        &TASK2_arrival,
        &TASK2_execution,
        &TASK2_deadline);
#endif // TASK2_ID

#ifdef TASK3_ID //假如task3有被定義的話 就創建task3工作
    Jack_OSTaskCreateExt(task3,
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
        TASK3_PERIOD,
        TASK3_aperiodic,
        TASK3_serversize,
        &TASK3_arrival,
        &TASK3_execution,
        &TASK3_deadline);
#endif // TASK3_ID

#ifdef TASK4_ID //假如task4有被定義的話 就創建task4工作
    Jack_OSTaskCreateExt(task4,
        0,
        &Task4_STK[TASK_STACKSIZE - 1u],
        TASK4_PRIORITY,
        TASK4_ID,
        &Task4_STK[0u],
        TASK_STACKSIZE,
        0u,
        (OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR),
        TASK4_ARRIVAL,
        TASK4_EXECUTION,
        TASK4_PERIOD,
        TASK4_aperiodic,
        TASK4_serversize,
        &TASK4_arrival,
        &TASK4_execution,
        &TASK4_deadline);
#endif // TASK4_ID

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

void task1(void* p_arg) //task1的執行程式
{
    (void)p_arg;

    while (1)
    {
        //假如task開始執行時 還沒到達arrival time那就執行 OSTimeDly休息相差的時間
        if (OSTCBCur->arrival > OSTimeGet())
        {
            OSTimeDly(OSTCBCur->arrival - OSTimeGet());
        }
        else
        {
            while (1) {

                //begin_ready_time是 task變為ready準備能執行的時間點
                //現在時間點 - task變為ready的時間點 < task的反應時間的話 就持續卡在while內
                while (OSTimeGet() - OSTCBCur->begin_ready_time < OSTCBCur->response) {
                    //Do something
                }

                //休息剩餘的時間(週期時間-反應時間)
                OSTimeDly(OSTCBCur->period - OSTCBCur->response);
            }
        }
    }
}

void task2(void* p_arg) //task2的執行程式
{
    (void)p_arg;

    while (1)
    {
        //假如task開始執行時 還沒到達arrival time那就執行 OSTimeDly休息相差的時間
        if (OSTCBCur->arrival > OSTimeGet())
        {
            OSTimeDly(OSTCBCur->arrival - OSTimeGet());
        }
        else
        {
            while (1) {

                //begin_ready_time是 task變為ready準備能執行的時間點
                //現在時間點 - task變為ready的時間點 < task的反應時間的話 就持續卡在while內
                while (OSTimeGet() - OSTCBCur->begin_ready_time < OSTCBCur->response) {
                    //Do something
                }
                //休息剩餘的時間(週期時間-反應時間)
                OSTimeDly(OSTCBCur->period - OSTCBCur->response);
            }
        }
    }
}

void task3(void* p_arg) //task3的執行程式
{
    (void)p_arg;

    while (1)
    {
        //假如task開始執行時 還沒到達arrival time那就執行 OSTimeDly休息相差的時間
        if (OSTCBCur->arrival > OSTimeGet())
        {
            OSTimeDly(OSTCBCur->arrival - OSTimeGet());
        }
        else
        {
            while (1) {

                //begin_ready_time是 task變為ready準備能執行的時間點
                //現在時間點 - task變為ready的時間點 < task的反應時間的話 就持續卡在while內
                while (OSTimeGet() - OSTCBCur->begin_ready_time < OSTCBCur->response) {
                    //Do something
                }

                //休息剩餘的時間(週期時間-反應時間)
                OSTimeDly(OSTCBCur->period - OSTCBCur->response);
            }
        }
    }
}

void task4(void* p_arg) //task4的執行程式
{
    (void)p_arg;

    while (1)
    {
        //假如task開始執行時 還沒到達arrival time那就執行 OSTimeDly休息相差的時間
        if (OSTCBCur->arrival > OSTimeGet())
        {
            OSTimeDly(OSTCBCur->arrival - OSTimeGet());
        }
        else
        {
            while (1) {

                //begin_ready_time是 task變為ready準備能執行的時間點
                //現在時間點 - task變為ready的時間點 < task的反應時間的話 就持續卡在while內
                while (OSTimeGet() - OSTCBCur->begin_ready_time < OSTCBCur->response) {
                    //Do something
                }

                //休息剩餘的時間(週期時間-反應時間)
                OSTimeDly(OSTCBCur->period - OSTCBCur->response);
            }
        }
    }
}

