#include "includes.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* The time between cycles of the 'check' task - which depends on whether the
check task has detected an error or not. */
#define mainCHECK_DELAY_NO_ERROR ((TickType_t)5000 / portTICK_PERIOD_MS)
#define mainCHECK_DELAY_ERROR ((TickType_t)500 / portTICK_PERIOD_MS)

/* Task priorities. */
#define mainSEM_TEST_PRIORITY (tskIDLE_PRIORITY + 1)
#define mainBLOCK_Q_PRIORITY (tskIDLE_PRIORITY + 2)
#define mainCHECK_TASK_PRIORITY (tskIDLE_PRIORITY + 3)
#define mainFLASH_TASK_PRIORITY (tskIDLE_PRIORITY + 2)
#define mainECHO_TASK_PRIORITY (tskIDLE_PRIORITY + 1)
#define mainINTEGER_TASK_PRIORITY (tskIDLE_PRIORITY)
#define mainGEN_QUEUE_TASK_PRIORITY (tskIDLE_PRIORITY)

void main_task(void *pvParameters);

HOST infoHost; // Information interaction with Marlin
MENU infoMenu; // Menu structure

void Hardware_GenericInit(void)
{
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  Delay_init(F_CPUM);
  OS_TimerInit(9999, F_CPUM - 1); // System clock timer, cycle 10ms

#ifdef DISABLE_DEBUG
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE); //disable JTAG & SWD
#endif

#ifdef DISABLE_JTAG
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
#endif

  XPT2046_Init();
  W25Qxx_Init();
  LCD_Init();
  readStoredPara();
  LCD_RefreshDirection(); //refresh display direction after reading settings
  scanUpdates();
  SD_DeInit();

#if LCD_ENCODER_SUPPORT
  LCD_EncoderInit();
#endif

#ifdef PS_ON_PIN
  PS_ON_Init();
#endif

#ifdef FIL_RUNOUT_PIN
  FIL_Runout_Init();
#endif

  if (readStoredPara() == false) // Read settings parameter
  {
    TSC_Calibration();
    storePara();
  }
  GUI_RestoreColorDefault();
  infoMenuSelect();
}

int main(void)
{

  SCB->VTOR = VECT_TAB_FLASH;

  Hardware_GenericInit();

  /* Create the 'echo' task, which is also defined within this file. */
  //  xTaskCreate(main_task, "Echo", configMINIMAL_STACK_SIZE, NULL, mainECHO_TASK_PRIORITY, NULL);
  // vTaskStartScheduler();
  for (;;)
  {
    (*infoMenu.menu[infoMenu.cur])();
  }
  /* Will only get here if there was insufficient memory to create the idle
    task.  The idle task is created within vTaskStartScheduler(). */
  for (;;)
    ;
}

void main_task(void *pvParameters)
{
  for (;;)
  {
    (*infoMenu.menu[infoMenu.cur])();
  }
}

void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
{
  /* This function will get called if a task overflows its stack.   If the
	parameters are corrupt then inspect pxCurrentTCB to find which was the
	offending task. */

  (void)pxTask;
  (void)pcTaskName;

  for (;;)
    ;
}