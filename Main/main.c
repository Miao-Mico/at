#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <process.h>
#include <time.h>

#include "at.h"
#include "at_device_template.h"
#include "time_manage.h"
#include "time_manage_timer_template.h"
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

size_t __stdcall main_hardware_layer(PVOID arg);
size_t __stdcall main_exit_control(PVOID arg);

void main_platform_support_package_init(void);
void main_software_layer(void);

size_t get_rand_delay_time(void);

at_stp at = NULL;

char *controller_file_name[4];
char *peripheral_file_name[4];

struct clocker_s *clocker = NULL;

struct at_device_package_s *at_windows_controller_package;
struct at_device_package_s *at_windows_peripheral_package;

size_t thread_id_exit_control = 0;
HANDLE thread_handle_exit_control = 0;

size_t thread_id_hardware_layer = 0;
HANDLE thread_handle_hardware_layer = 0;

bool main_thred_exit = false;

void main(void)
{
	main_platform_support_package_init();

	WaitForSingleObject(thread_handle_hardware_layer, INFINITE);							/* Wait the hardware layer run at least once */

	clocker_ctrl.start(clocker);

	if (at_ctrl.transmit.multi_level
		.generate(at, "#3:1:2:1@1",															/* Generate the multi level at instruction */
				  "level 1.\r\n",
				  "level 2.", "\r\n",
				  "level 3.\r\n", "FILE:__PERIPHERAL_DEVICE_SEND_TXT")) {
		return;
	}

	printf("have cost: %lldus \r\n", clocker_ctrl.stop(clocker));

	while (true) {
		main_software_layer();																/* Wait all the task run at least once */

		static size_t cnt = 0;

		if (50 < cnt++) {
			break;
		}
	}

	if (at_ctrl.transmit.multi_level
		.send(at)) {																		/* Send the multi level at instruction */
		return;
	}

	while (true) {
		main_software_layer();

		if (main_thred_exit) {
			break;
		}
	}

	at_windows_peripheral_package->configuration.demount(at_windows_peripheral_package);

	at_ctrl.configuration.destroy(&at);

	_endthreadex(thread_id_hardware_layer);

	return;
}

void main_platform_support_package_init(void)												/* Platform Support Package and AT Initialization */
{
	errno_t error = 0;

	error += at_device_package_packer.
		windows_file_stream(&at_windows_peripheral_package,
							peripheral_file_name);											/* Pack the package as the windows file stream */

	error += clocker_ctrl.configuration.init(&clocker,										/* Initialize the clocker */
											 MAIN_CFG_CLOCKER_FREQUENCY,
											 &time_mamage_windows_timer_package);

	error += at_device_package_packer.
		windows_file_stream(&at_windows_controller_package,
							controller_file_name);											/* Pack the package as the windows file stream */

	error += at_ctrl.configuration.init(&at, 												/* Initialize the at */
										at_windows_controller_package);

	thread_handle_exit_control =
		(HANDLE)_beginthreadex(NULL, 0, main_exit_control, 									/* Begin the exit control thread */
							   NULL, 0, &thread_id_exit_control);

	thread_handle_hardware_layer =
		(HANDLE)_beginthreadex(NULL, 0, main_hardware_layer, 								/* Begin the hardware layer simulation thread */
							   NULL, 0, &thread_id_hardware_layer);

	if (error) {
		while (true);
	}
}

size_t __stdcall main_exit_control(PVOID arg)												/* Control if the main thread exit the wile loop */
{
	static char scanf_string[100] = { 0 };

	scanf_string[99] = scanf("%s", scanf_string);

	if ('0' != scanf_string[0]) {															/* If have input,set main_thred_exit */
		main_thred_exit = true;
	}

	return 0;
}

size_t __stdcall main_hardware_layer(PVOID arg)												/* Simulate the peripheral device random communications */
{
	printf("hardware thread operate\r\n");

	DWORD time_delay = get_rand_delay_time();												/* Get random delay time */

	Sleep(time_delay);																		/* Sleep for a while */

	printf("have sleep %ld ms\r\n", time_delay);

	return 0;
}

void main_software_layer(void)																/* Simulate the soft logic of controller device or sever */
{
	at_ctrl.device_interrupt(at);															/* Device hardware interrupt */
	at_ctrl.task_os_tick(at);																/* Task os soft tick */
}

size_t get_rand_delay_time(void)
{
	LARGE_INTEGER seed = { 0 };

	QueryPerformanceCounter(&seed);															/* Generate the seed by the windows counter */

	srand((size_t)seed.QuadPart);

	return (rand() % 10 + 1);																/* Get random time between 1 and 10 */
}

char *controller_file_name[4] = {
	MAIN_CFG_CONTROLLER_SEND_TO_FILENAME,"a+",
	MAIN_CFG_CONTROLLER_RECEIVE_FROM_FILENAME,"a+"
};

char *peripheral_file_name[4] = {
	MAIN_CFG_PERIPHERAL_DEVICE_SEND_TO_FILENAME,"a+",
	MAIN_CFG_PERIPHERAL_DEVICE_RECEIVE_FROM_FILENAME,"a+"
};