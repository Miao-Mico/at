/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include "at.h"

#include "at_device.h"
#include "at_task.h"

/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/

/**
 * @brief This struct will contain all the universal vector functions address.
 */

struct at_information_s {
	bool switch_status;

	char transmit_end_with[AT_CFG_CACHE_LEVEL_AMT][AT_CFG_END_WITH_STRING_LEN];
};

/**
 * @brief This struct will contain all the universal vector functions address.
 */

struct at_transmit_s {
	short cache_amt;
	short cache_level_amt;
	short crt_ist;
	short cache_level[AT_CFG_CACHE_AMT];
	char cache[AT_CFG_CACHE_AMT][AT_CFG_CACHE_LEVEL_AMT][100];
};

/**
 * @brief This struct will contain all the universal vector functions address.
 */

struct at_feedback_thread_s {
	struct at_feedback_thread_information_s {
		char *thread_name;													// 方便 log
		float verify_pass_threshold_value;									// 阈值

		bool switch_status;													// 开关
		int verify_pass;													// 是否验证通过
		bool result_locked;													//
		char(*verify_rule)[AT_CFG_FEEDBACK_VERIFY_THREAD_MAX_RULE_LENGTH];	// 验证规则
		short verify_amount;
	}info;

	void (*process)(struct AT_Feedback_Verify_Thread_Module *module_t);
	void (*verify_pass_callback)(void);										// 回调函数
	void (*verify_fail_callback)(void);										// 回调函数
};

/**
 * @brief This struct will contain all the universal vector functions address.
 */

struct at_feedback_s {
	short handle_priority;
	struct at_feedback_thread_s *handle_thread;

	short amount_inited_thread;							// 初始化的线程
	short amount_max_thread;							// 最大线程数

	struct at_feedback_thread_s *thread_ptr[AT_CFG_FEEDBACK_VERIFY_THREAD_AMOUNT];
	bool thread_actived[AT_CFG_FEEDBACK_VERIFY_THREAD_AMOUNT];
};

/**
 * @brief This struct will contain all the universal vector functions address.
 */

struct at_exception_s {
	void (*error)(void);
};

/**
 * @brief This struct will contain all the universal vector functions address.
 */

struct at_s {
	struct at_information_s info;

	struct at_device_package_s *device_package;

	struct at_transmit_s transmit;

	struct at_feedback_s feedback;

	struct at_exception_s exception;
};

/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

struct at_control_s at_ctrl = {
	.configuration.init = at_control_configuration_init,
	.configuration.destroy = at_control_configuration_destroy,
	.configuration.retarget = at_control_configuration_retarget
};

struct at_task_s *at_task_os_multi_level_transmit_task = NULL;

/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

/**
 * @brief This function will be called when the multi level transmit task run.
 *
 * @param void
 *
 * @return void
 */

void at_control_at_task_os_multi_level_transmit_task_function(void);

/*
*********************************************************************************************************
*                                            FUNCTIONS
*********************************************************************************************************
*/

/**
 * @brief This function will initialize the at struct.
 *
 * @param void
 *
 * @return void
 */

errno_t at_control_configuration_init(struct at_s **at,
									  struct at_device_package_s *device_package)
{
	assert(at);
	assert(device_package);

	if (NULL == device_package ||																				/* If the device_package is NULL */
		NULL == ((*at) = calloc(1, sizeof(struct at_s)))) {
		//(*at)->exception.hardware_not_ready();

		return 1;
	}

	at_task_ctrl.os.configuration.init();																		/* Initialize the at task os */

	at_task_ctrl.task.configuration.init(&at_task_os_multi_level_transmit_task,									/* Initialize the multi level transmit task */
										 "at task os.multi level transmit task",
										 at_control_at_task_os_multi_level_transmit_task_function,
										 0,
										 NULL,
										 NULL,
										 0);

	if (at_control_configuration_retarget(*at, NULL, NULL, device_package)) {									/* Target the at to the device_package */
		return 2;
	}

	(*at)->info.switch_status = true;

	return 0;
}

/**
 * @brief This function will destroy the at struct.
 *
 * @param void
 *
 * @return void
 */

errno_t at_control_configuration_destroy(struct at_s **at)
{
	assert(at);
	assert(*at);

	if (at_task_ctrl.os.configuration.destroy()) {											/* Destroy the at tack os */
		return 1;
	}

	free(*at);

	(*at) = NULL;

	return 0;
}

/**
 * @brief This function will retarget the device_package of at struct to the device_package specified.
 *
 * @param void
 *
 * @return void
 */

errno_t at_control_configuration_retarget(struct at_s *at,
										  void *device,
										  void *arg_list,
										  struct at_device_package_s *device_package)
{
	assert(at);

	void *devide_package_cpy = NULL;

	if (NULL == device_package &&
		NULL == (device_package = at->device_package)) {			/* If not appoint the device_package pack */
		return 1;
	}

	if (!device_package->verify.package(device_package)) {			/* If device package is not valid */
		return 2;
	}

	if (NULL != at->device_package &&
		device_package->device_pack_id ==
		at->device_package->device_pack_id) {						/* If device_pack is the same as the device_pack of at */
		goto CONFIG_DEVICE_PART;
	} else {
		free(at->device_package);									/* Deallocate the outdated device package */
	}

	if (NULL == (devide_package_cpy =
				 calloc(1, sizeof(struct at_device_package_s))))	/* Allocate the space for device package copy */
	{
		return -1;
	}

	memcpy(devide_package_cpy, device_package,
		   sizeof(struct at_device_package_s));						/* Copy a new device package */

	at->device_package = devide_package_cpy;

CONFIG_DEVICE_PART:

	if (NULL != device &&
		NULL != arg_list) {
		errno_t err = 0;

		if (err = (errno_t)device_package->configuration.
			mount(device_package, arg_list)) {
			return err;
		}
	}

	return 0;
}

/**
 * @brief This function will be called when the multi level transmit task run.
 *
 * @param void
 *
 * @return void
 */

void at_control_at_task_os_multi_level_transmit_task_function(void)
{
	printf("at task os.multi level transmit task\r\n");
}