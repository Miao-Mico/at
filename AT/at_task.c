/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include "at_task.h"

#include "at_device.h"

#include "at_message.h"

/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

#if (AT_TASK_CFG_IDLE_TASK_EN)

#define AT_TASK_CFG_IDLE_TASK_PRIORITY                                      AT_TASK_CFG_PRIORITY_MAX - 1

#endif // (AT_TASK_CFG_IDLE_TASK_EN)

#define AT_TASK_CFG_LIST_STACK_COUNT											2u

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
 * @brief This struct is the body of at task os.
 */

struct at_task_os_s {
	bool os_running;

	at_task_size_t priority_max;

	at_task_size_t priority;

	struct {
		struct at_task_s *ready;
		struct at_task_s *suspend;
	}task_list[AT_TASK_CFG_PRIORITY_MAX];

	struct at_task_s *running_task_ptr;
};

/**
 * @brief This type will contain all the at task status enums.
 */

enum at_task_status_e {
	AT_TASK_STATUS_RUNNIBG,
	AT_TASK_STATUS_READY,
	AT_TASK_STATUS_SUSPEND,
};

/**
 * @brief This type will contain all the at task status enums.
 */

enum at_task_option_e {
	AT_TASK_OPTION_RUN,
	AT_TASK_OPTION_SUSPEND,
};

/**
 * @brief This struct is the body of at task information.
 */

struct at_task_info_s {
	enum at_task_status_e status;

	char *name;

	at_task_size_t priority;
};

/**
 * @brief This struct is the body of at task.
 */

struct at_task_s {
	struct at_task_info_s info;

	at_task_function_t function;

	void (*hook)(void);
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

/**
 * @brief This struct is the body of at task.
 */

struct at_task_s *at_task_os_idle_task;

/**
 * @brief This struct is the body of at task os.
 */

struct at_data_structure_control_package_s *at_task_os_task_list_package =
	&AT_TASK_CFG_USER_DEFINED_READY_LIST_DATA_STRUCTURE;

/**
 * @brief This struct will contain all the at task control functions.
 */

struct at_task_control_s at_task_ctrl = {
	.os.configuration.init = at_task_control_os_configuration_init,
	.os.configuration.destroy = at_task_control_os_configuration_destroy,
	.os.core = at_task_control_os_core,

	.task.configuration.init = at_task_control_task_configuration_init,
	.task.configuration.destroy = at_task_control_task_configuration_destroy,
	.task.configuration.suspend = at_task_control_task_configuration_suspend,
	.task.configuration.rusume = at_task_control_task_configuration_resume,
};

/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

/**
 * @brief This function will be call when the at task os idle task is running.
 *
 * @param void
 *
 * @return void
 */

void at_task_os_control_idle_task_function(void *arg);

/*
*********************************************************************************************************
*                                            FUNCTIONS
*********************************************************************************************************
*/

/**
 * @brief This function will initialize the at task os.
 *
 * @param void
 *
 * @return void
 */

errno_t at_task_control_os_configuration_init(struct at_task_os_s **task_os)
{
	assert(task_os);

	errno_t err = 0;

	if (NULL == ((*task_os) = calloc(1, sizeof(struct at_task_os_s)))) {
		return -1;
	}

	for (size_t cnt = 0; cnt < AT_TASK_CFG_PRIORITY_MAX; cnt++) {							/* Initialize the ready lists */
		if (at_task_os_task_list_package->configuration
			.init(&(*task_os)->task_list[cnt].ready)) {
			return 1;
		}

		if (at_task_os_task_list_package->configuration
			.init(&(*task_os)->task_list[cnt].suspend)) {
			return 2;
		}
	}

	#if (AT_TASK_CFG_IDLE_TASK_EN)

	if (at_task_control_task_configuration_init((*task_os),									/* Initialize the idle task */
												&at_task_os_idle_task,
												"at task os.idle task",
												at_task_os_control_idle_task_function,
												AT_TASK_CFG_IDLE_TASK_PRIORITY,
												NULL,
												NULL,
												0)) {
		return 3;
	}

	#endif // (AT_TASK_CFG_IDLE_TASK_EN)

	(*task_os)->priority_max = (at_task_size_t)AT_TASK_CFG_PRIORITY_MAX - 1;

	(*task_os)->os_running = true;

	return 0;
}

/**
 * @brief This function will destroy the at task os.
 *
 * @param void
 *
 * @return void
 */

errno_t at_task_control_os_configuration_destroy(struct at_task_os_s **task_os)
{
	assert(task_os);
	assert(*task_os);

	void
		*group_priority = NULL,
		*list_priority = NULL,
		*task_priority_list = NULL;

	at_task_size_t size_priority_list = 0;

	for (size_t cnt = 0; cnt < AT_TASK_CFG_PRIORITY_MAX; cnt++) {							/* Destroy the at task list group of the priorities,total have _PRIORITY_MAX */
		group_priority = 																	/* Get the list group of the priority */
			((void **)(*task_os)->task_list + cnt * AT_TASK_CFG_LIST_STACK_COUNT);

		for (size_t ct = 0; ct < AT_TASK_CFG_LIST_STACK_COUNT; ct++) {						/* Destroy the lists of this group,total have _LIST_STACK_COUNT */
			list_priority = *((void **)group_priority + ct);								/* Get the one list of the group */

			size_priority_list = (at_task_size_t)at_task_os_task_list_package->capacity
				.size(list_priority);														/* Get the size of the list */

			for (size_t c = 0; c < size_priority_list; c++) {								/* Destroy the tasks in the list,total have size_priority_list */
				task_priority_list = (void **)at_task_os_task_list_package->element_access
					.at(list_priority);
				at_task_control_task_configuration_destroy(*task_os,
														   &(struct at_task_s *)task_priority_list);

				at_task_os_task_list_package->modifiers.delete(list_priority);
			}
		}
																							/* TODO: put it into the above for loop */
		at_task_os_task_list_package->configuration.										/* Destroy the stack */
			destroy(&(*task_os)->task_list[cnt].ready);
		at_task_os_task_list_package->configuration.
			destroy(&(*task_os)->task_list[cnt].suspend);
	}

	(*task_os) = NULL;

	return 0;
}

/**
 * @brief This function will inquire the highest priority of the at task os.
 *
 * @param void
 *
 * @return void
 */

static inline at_task_size_t
at_task_control_os_inquire_highest_priority(struct at_task_os_s *task_os)
{
	assert(task_os);

	return task_os->priority;
}

/**
 * @brief This function will switch the tasks of the os to the ready list tail.
 *
 * @param void
 *
 * @return void
 */

void at_task_control_os_task_switcher(struct at_task_os_s *task_os,
									  at_task_size_t priority,
									  struct at_task_s *task)
{
	assert(task_os);
	assert(task);

	at_task_os_task_list_package->modifiers
		.delete(task_os->task_list[priority].ready);

	at_task_os_task_list_package->modifiers
		.insert(task_os->task_list[priority].ready, task);
}

/**
 * @brief This function will manage the priority of the os.
 *
 * @param void
 *
 * @return void
 */

void at_task_control_os_priority_management(struct at_task_os_s *task_os)
{
	assert(task_os);

	if (AT_TASK_CFG_PRIORITY_MAX <= ++task_os->priority) {
		task_os->priority = 0;
	}
}

/**
 * @brief This function will schedule the tasks of the os.
 *
 * @param void
 *
 * @return void
 * @note if there is lower priority task exist the task wouldn't be scheduled,
 *			so the suspend of running task will be likely be delayed.
 */

void at_task_control_os_scheduler(struct at_task_os_s *task_os)
{
	assert(task_os);

	if (false == task_os->os_running) {														/* If task os is running */
		return;
	}

	at_task_size_t priority = 0;
	struct at_task_s *task_ready = NULL;

	priority = at_task_control_os_inquire_highest_priority(task_os);						/* Get the highest priority */

	task_ready = at_task_os_task_list_package->element_access								/* Get the top task block of this priority ready list */
		.at(task_os->task_list[priority].ready);

	if (NULL == task_ready) {
		goto PRIORITY_MANAGE;																/* Manage the priority */
	}

	if (task_os->running_task_ptr == task_ready) {											/* If the task ready is the task running */
		goto PRIORITY_MANAGE;																/* Manage the priority */
	}

	task_os->running_task_ptr = task_ready;

	at_task_control_os_task_switcher(task_os, priority, task_ready);						/* Switch the task block to the tail of the ready list */

PRIORITY_MANAGE:

	at_task_control_os_priority_management(task_os);										/* Manage the priority */
}

/**
 * @brief This function will schedule the tasks of the os.
 *
 * @param void
 *
 * @return void
 */

void at_task_control_os_core(struct at_task_os_s *task_os, void *arg_list)
{
	assert(task_os);

	if (false == task_os->os_running) {
		return;
	}

	at_task_control_os_scheduler(task_os);

	if (NULL != task_os->running_task_ptr) {												/* Valid task */
		static char *msg = NULL;
		static bool msg_load = false;
		static bool highest_priority = false;

		if (false == msg_load &&
			NULL == (msg = at_message_ctrl.feedback.load(arg_list))) {						/* Load the feedback message when at the priority max */
			return;
		}

		msg_load = true;

		if (task_os->priority_max == task_os->running_task_ptr->info.priority) {			/* First run should be the highest priority task */
			highest_priority = true;
		}

		if (false == highest_priority) {
			return;
		}

		task_os->running_task_ptr->function(msg);											/* Run the function of the task */

		if ((at_task_size_t)AT_TASK_CFG_PRIORITY_MAX - 1 ==									/* When the idle task */
			task_os->running_task_ptr->info.priority) {
			highest_priority = false;
			msg_load = false;
		}
	}
}

/**
 * @brief This function will initialize the task of the at task os.
 *
 * @param void
 *
 * @return void
 */

errno_t at_task_control_task_configuration_init(struct at_task_os_s *task_os,
												struct at_task_s **task,
												char *name,
												void *func,
												at_task_size_t priority,
												void *arg_list,
												void *hook,
												enum at_task_option_e opt)
{
	assert(task_os);
	assert(task);
	assert(name);
	assert(func);

	if (NULL == ((*task) = calloc(1, sizeof(struct at_task_s)))) {
		return 1;
	}

	switch (opt) {
		case AT_TASK_OPTION_RUN:
			(*task)->info.status = AT_TASK_STATUS_READY;									/* Set the ready status of the task */

			at_task_os_task_list_package->modifiers
				.insert(task_os->task_list[priority].ready, *task);							/* Push the task address into the ready list of this priority */
			break;
		case AT_TASK_OPTION_SUSPEND:
			(*task)->info.status = AT_TASK_STATUS_SUSPEND;									/* Set the ready status of the task */

			at_task_os_task_list_package->modifiers
				.insert(task_os->task_list[priority].suspend, *task);						/* Push the task address into the suspend list of this priority */
			break;
		default:
			break;
	}

	if (task_os->priority_max > priority) {													/* Record the highest priority of the task os */
		task_os->priority_max = priority;
	}

	(*task)->info.name = name;
	(*task)->info.priority = priority;

	(*task)->function = func;

	(*task)->hook = hook;

	return 0;
}

/**
 * @brief This function will destroy the task of at task os.
 *
 * @param void
 *
 * @return void
 */

errno_t at_task_control_task_configuration_destroy(struct at_task_os_s *task_os,
												   struct at_task_s **task)
{
	assert(task_os);
	assert(task);
	assert(*task);

	if (task_os->priority_max = (*task)->info.priority) {									/* Record the highest priority of the task os */
		task_os->priority_max--;
	}

	free(*task);

	*task = NULL;

	return 0;
}

/**
 * @brief This function will suspend the task of the at task os.
 *
 * @param void
 *
 * @return void
 */

extern inline errno_t
at_task_control_task_configuration_suspend(struct at_task_os_s *task_os,
										   struct at_task_s *task)
{
	assert(task_os);
	assert(task);

	task->info.status = AT_TASK_STATUS_SUSPEND;

	void *task_addr = NULL;

	if (NULL == (task_addr = at_task_os_task_list_package->element_access
				 .at(task_os->task_list[task->info.priority].ready))) {
		return 1;
	}

	at_task_os_task_list_package->modifiers
		.insert(task_os->task_list[task->info.priority].suspend, task_addr);
	at_task_os_task_list_package->modifiers
		.delete(task_os->task_list[task->info.priority].ready);

	return 0;
}

/**
 * @brief This function will resume the task of the at task os.
 *
 * @param void
 *
 * @return void
 */

extern inline errno_t
at_task_control_task_configuration_resume(struct at_task_os_s *task_os,
										  struct at_task_s *task)
{
	assert(task_os);
	assert(task);

	if (AT_TASK_STATUS_READY == task->info.status) {
		return 1;
	}

	task->info.status = AT_TASK_STATUS_READY;

	void *task_addr = NULL;

	if (NULL == (task_addr = at_task_os_task_list_package->element_access
				 .at(task_os->task_list[task->info.priority].suspend))) {
		return 2;
	}

	at_task_os_task_list_package->modifiers
		.insert(task_os->task_list[task->info.priority].ready, task_addr);
	at_task_os_task_list_package->modifiers
		.delete(task_os->task_list[task->info.priority].suspend);

	return 0;
}

/**
 * @brief This function will be call when the at task os idle task is running.
 *
 * @param void
 *
 * @return void
 */

void at_task_os_control_idle_task_function(void *arg_list)
{
	printf("at task os.idle task.message:\"%s\" \r\n", (char *)arg_list);
}