#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <process.h>

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

void main_bsp_init(void);
size_t __stdcall main_hardware_layer(PVOID pM);
void main_software_layer(void);

size_t get_rand_delay_time(void);

at_stp at = NULL;

char *controller_file_name[4];
char *peripheral_file_name[4];

struct clocker_s *clocker = NULL;

struct at_device_package_s *at_windows_controller_package;
struct at_device_package_s *at_windows_peripheral_package;

size_t thread_id_hardware_layer = 0;
HANDLE thread_handle_hardware_layer = 0;

void main(void)
{
	main_bsp_init();

	clocker_ctrl.start(clocker);

	WaitForSingleObject(thread_handle_hardware_layer, INFINITE);							/* Wait the hardware layer run at least once */

	while (false) {
		main_software_layer();
	}

	printf("have cost: %lldus \r\n", clocker_ctrl.stop(clocker));

	at_windows_peripheral_package->configuration.demount(at_windows_peripheral_package);

	at_ctrl.configuration.destroy(&at);

	_endthreadex(thread_id_hardware_layer);

	return;
}

void main_bsp_init(void)																	/* Board Support Package and AT Initialization */
{
	errno_t error = 0;

	error += at_device_package_packer.
		windows_file_stream(&at_windows_peripheral_package,
							peripheral_file_name);											/* Pack the package as the windows file stream */

	error += clocker_ctrl.configuration.init(&clocker,										/* Initialize the clocker */
											 MAIN_CFG_CLOCKER_FREQUENCY,
											 &time_mamage_windows_timer_package);

	thread_handle_hardware_layer =
		(HANDLE)_beginthreadex(NULL, 0, main_hardware_layer, 								/* Begin the hardware layer simulation thread */
							   NULL, 0, &thread_id_hardware_layer);

	error += at_device_package_packer.
		windows_file_stream(&at_windows_controller_package,
							controller_file_name);											/* Pack the package as the windows file stream */

	error += at_ctrl.configuration.init(&at, 												/* Initialize the at */
										at_windows_controller_package);

	if (error) {
		while (true);
	}
}

size_t __stdcall main_hardware_layer(PVOID pM)												/* Simulate the peripheral device random communications */
{
	printf("hardware thread operate\r\n");

	DWORD time_delay = get_rand_delay_time();												/* Get random delay time */

	Sleep(time_delay);																		/* Sleep for a while */

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

char *controller_file_name[4] = {
	MAIN_CFG_CONTROLLER_SEND_TO_FILENAME,"a+",
	MAIN_CFG_CONTROLLER_RECEIVE_FROM_FILENAME,"a+"
};

char *peripheral_file_name[4] = {
	MAIN_CFG_PERIPHERAL_DEVICE_SEND_TO_FILENAME,"a+",
	MAIN_CFG_PERIPHERAL_DEVICE_RECEIVE_FROM_FILENAME,"a+"
};