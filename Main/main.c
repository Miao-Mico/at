#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <process.h>

#include "at.h"
#include "at_device.h"
#include "time_manage.h"
#include "clocker.h"

#define MAIN_CFG_CONTROLLER_SEND_TO_FILENAME												\
	"./Text/peripheral_device_receive.txt"
#define MAIN_CFG_CONTROLLER_RECEIVE_FROM_FILENAME											\
	"./Text/peripheral_device_send.txt"

#define MAIN_CFG_PERIPHERAL_DEVICE_SEND_TO_FILENAME											\
	"./Text/controller_receive.txt"
#define MAIN_CFG_PERIPHERAL_DEVICE_RECEIVE_FROM_FILENAME									\
	"./Text/controller_send.txt"

#define MAIN_CFG_CLOCKER_FREQUENCY															\
	1000000

void main_bsp_init(void);
size_t __stdcall main_hardware_layer(PVOID pM);
void main_software_layer(void);

size_t get_rand_delay_time(void);

at_stp at = NULL;

char *controller_file_name[2];
char *peripheral_file_name[2];

struct clocker_s *clocker = NULL;

struct at_device_package_s *at_windows_controller_package;
struct at_device_package_s *at_windows_peripheral_package;

size_t thread_id_hardware_layer = 0;
HANDLE thread_handle_hardware_layer = 0;

struct at_task_s *at_task_os_test_task = NULL;

void test_task_function(void *arg);

void main(void)
{
	main_bsp_init();

	clocker_ctrl.start(clocker);

	at_task_ctrl.os.configuration.init();

	size_t cnt = 0;

	at_task_ctrl.task.configuration.init(&at_task_os_test_task,				/* Initialize the test task */
										 "at task os.test task",
										 test_task_function,
										 0,
										 NULL,
										 NULL,
										 1);

	at_device_ctrl.package_operator.transmit.send(at_windows_peripheral_package,
												  "at device control.transmit.send\r\n",
												  sizeof("at device control.transmit.send\r\n"));

	printf("Cost %llu microsec\r\n", clocker_ctrl.stop(clocker));

	while (true) {
		at_task_ctrl.os.core();

		if (15 == cnt) {
			at_task_ctrl.task.configuration.rusume(at_task_os_test_task);
			printf("Resume \r\n");
		}

		if (55 == cnt) {
			at_task_ctrl.task.configuration.suspend(at_task_os_test_task);
			printf("Suspend \r\n");
		}

		if (100 < cnt) {
			break;
		}

		cnt++;
	}

	at_task_ctrl.os.configuration.destroy();

	at_device_ctrl.package_operator.configuration.demount(at_windows_peripheral_package);

	WaitForSingleObject(thread_handle_hardware_layer, INFINITE);

	while (0) {
		main_software_layer();
	}

	_endthreadex(thread_id_hardware_layer);

	return;
}

void main_bsp_init(void)																/* Board Support Package and AT Initialization */
{
	at_device_ctrl.configuration.init.
		windows_package(&at_windows_peripheral_package);								/* Initialize the at peripheral device package as the windows i/o file stream */

	at_device_ctrl.package_operator.configuration.										/* Mount the at peripheral device package to the windows i/o file stream */
		mount(at_windows_peripheral_package, controller_file_name);

	clocker_ctrl.configuration.init(&clocker,											/* Initialize the clocker */
									MAIN_CFG_CLOCKER_FREQUENCY,
									&time_mamage_windows_timer_package);

	thread_handle_hardware_layer =
		(HANDLE)_beginthreadex(NULL, 0, main_hardware_layer, 							/* Begin the hardware layer simulation thread */
							   NULL, 0, &thread_id_hardware_layer);
}

size_t __stdcall main_hardware_layer(PVOID pM)											/* Simulate the peripheral device random communications */
{
	printf("hardware thread operate\r\n");

	DWORD time_delay = get_rand_delay_time();											/* Get random delay time */

	Sleep(time_delay);																	/* Sleep for a while */

	printf("have sleep %ld ms\r\n", time_delay);

	return 0;
}

void main_software_layer(void)															/* Simulate the soft logic of controller device or sever */
{
}

size_t get_rand_delay_time(void)
{
	LARGE_INTEGER seed = { 0 };

	QueryPerformanceCounter(&seed);														/* Generate the seed by the windows counter */

	srand((size_t)seed.QuadPart);

	return (rand() % 10 + 1);															/* Get random time between 1 and 10 */
}

void test_task_function(void *arg)
{
	printf("at task os.test task\r\n");
}

char *controller_file_name[2] = {
	MAIN_CFG_CONTROLLER_SEND_TO_FILENAME,
	MAIN_CFG_CONTROLLER_RECEIVE_FROM_FILENAME
};

char *peripheral_file_name[2] = {
	MAIN_CFG_PERIPHERAL_DEVICE_SEND_TO_FILENAME,
	MAIN_CFG_PERIPHERAL_DEVICE_RECEIVE_FROM_FILENAME
};