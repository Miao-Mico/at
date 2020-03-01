/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include "at_task.h"

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

	struct {
		struct at_task_s *ready;
		struct at_task_s *suspend;
	}list_table[AT_TASK_CFG_PRIORITY_MAX];

	struct at_task_s *running_task_ptr;

	at_task_size_t priority;
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
	at_task_size_t name_len;

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
 * @brief This struct is the body of at task os.
 */

struct at_task_os_s at_task_os;

/**
 * @brief This struct is the body of at task.
 */

struct at_task_s *at_task_os_idle_task;

/**
 * @brief This struct is the body of at task os.
 */

struct at_task_data_structure_package_s *at_task_os_task_list_package =
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

errno_t at_task_control_os_configuration_init(void)
{
	errno_t err = 0;

	for (size_t cnt = 0; cnt < AT_TASK_CFG_PRIORITY_MAX; cnt++) {							/* Initialize the ready lists */
		if (at_task_os_task_list_package->configuration.
			init(&at_task_os.list_table[cnt].ready)) {
			return 1;
		}

		if (at_task_os_task_list_package->configuration.
			init(&at_task_os.list_table[cnt].suspend)) {
			return 2;
		}
	}

	#if (AT_TASK_CFG_IDLE_TASK_EN)

	if (at_task_control_task_configuration_init(&at_task_os_idle_task,						/* Initialize the idle task */
												"at task os.idle task",
												at_task_os_control_idle_task_function,
												AT_TASK_CFG_IDLE_TASK_PRIORITY,
												NULL,
												NULL,
												0)) {
		return 3;
	}

	#endif // (AT_TASK_CFG_IDLE_TASK_EN)

	at_task_os.os_running = true;

	return 0;
}

/**
 * @brief This function will destroy the at task os.
 *
 * @param void
 *
 * @return void
 */

errno_t at_task_control_os_configuration_destroy(void)
{
	void
		*group_priority = NULL,
		*list_priority = NULL,
		*task_priority_list = NULL;

	at_task_size_t size_priority_list = 0;

	for (size_t cnt = 0; cnt < AT_TASK_CFG_PRIORITY_MAX; cnt++) {							/* Destroy the at task list group of the priorities,total have _PRIORITY_MAX */
		group_priority = 																	/* Get the list group of the priority */
			((void **)at_task_os.list_table + cnt * AT_TASK_CFG_LIST_STACK_COUNT);

		for (size_t ct = 0; ct < AT_TASK_CFG_LIST_STACK_COUNT; ct++) {						/* Destroy the lists of this group,total have _LIST_STACK_COUNT */
			list_priority = *((void **)group_priority + ct);								/* Get the one list of the group */

			size_priority_list = (at_task_size_t)at_task_os_task_list_package->capacity.
				size(list_priority);														/* Get the size of the list */

			for (size_t c = 0; c < size_priority_list; c++) {								/* Destroy the tasks in the list,total have size_priority_list */
				task_priority_list = (void **)at_task_os_task_list_package->element_access.
					top(list_priority);
				at_task_control_task_configuration_destroy(&(struct at_task_s *)task_priority_list);

				at_task_os_task_list_package->modifiers.pop(list_priority);
			}
		}
																							/* TODO: put it into the above for loop */
		at_task_os_task_list_package->configuration.										/* Destroy the stack */
			destroy(&at_task_os.list_table[cnt].ready);
		at_task_os_task_list_package->configuration.
			destroy(&at_task_os.list_table[cnt].suspend);
	}

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
at_task_control_os_inquire_highest_priority(void)
{
	return at_task_os.priority;
}

/**
 * @brief This function will switch the tasks of the os to the ready list tail.
 *
 * @param void
 *
 * @return void
 */

void at_task_control_os_task_switcher(at_task_size_t priority,
									  struct at_task_s *task)
{
	at_task_os_task_list_package->modifiers.
		pop(at_task_os.list_table[priority].ready);

	at_task_os_task_list_package->modifiers.
		push(at_task_os.list_table[priority].ready, task);
}

/**
 * @brief This function will manage the priority of the os.
 *
 * @param void
 *
 * @return void
 */

void at_task_control_os_priority_management(void)
{
	if (AT_TASK_CFG_PRIORITY_MAX <= ++at_task_os.priority) {
		at_task_os.priority = 0;
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

void at_task_control_os_scheduler(void)
{
	if (false == at_task_os.os_running) {                        /* If task os is running */
		return;
	}

	at_task_size_t priority = 0;
	struct at_task_s *task_ready = NULL;

	priority = at_task_control_os_inquire_highest_priority();   /* Get the highest priority */

	task_ready = at_task_os_task_list_package->element_access.	/* Get the top task block of this priority ready list */
		top(at_task_os.list_table[priority].ready);

	if (NULL == task_ready) {
		goto PRIORITY_MANAGE;									/* Manage the priority */
	}

	if (at_task_os.running_task_ptr == task_ready) {            /* If the task ready is the task running */
		goto PRIORITY_MANAGE;									/* Manage the priority */
	}

	at_task_os.running_task_ptr = task_ready;

	at_task_control_os_task_switcher(priority, task_ready);		/* Switch the task block to the tail of the ready list */

PRIORITY_MANAGE:

	at_task_control_os_priority_management();					/* Manage the priority */
}

/**
 * @brief This function will schedule the tasks of the os.
 *
 * @param void
 *
 * @return void
 */

void at_task_control_os_core(void)
{
	if (false == at_task_os.os_running) {
		return;
	}

	at_task_control_os_scheduler();

	if (NULL != at_task_os.running_task_ptr) {
		at_task_os.running_task_ptr->function(at_task_os.running_task_ptr);			/* Run the function of the task */
	}
}

/**
 * @brief This function will initialize the task of the at task os.
 *
 * @param void
 *
 * @return void
 */

errno_t at_task_control_task_configuration_init(struct at_task_s **task,
												char *name,
												void *func,
												at_task_size_t priority,
												void *arg_list,
												void *hook,
												enum at_task_option_e opt)
{
	assert(task);
	assert(name);
	assert(func);

	if (NULL == ((*task) = calloc(1, sizeof(struct at_task_s)))) {
		return 1;
	}

	switch (opt) {
		case AT_TASK_OPTION_RUN:
			(*task)->info.status = AT_TASK_STATUS_READY;											/* Set the ready status of the task */

			at_task_os_task_list_package->modifiers.push(at_task_os.list_table[priority].ready,		/* Push the task address into the ready list of this priority */
														 *task);
			break;
		case AT_TASK_OPTION_SUSPEND:
			(*task)->info.status = AT_TASK_STATUS_SUSPEND;											/* Set the ready status of the task */

			at_task_os_task_list_package->modifiers.push(at_task_os.list_table[priority].suspend,	/* Push the task address into the suspend list of this priority */
														 *task);
			break;
		default:
			break;
	}

	(*task)->info.name = name;
	(*task)->info.name_len = 0;
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

errno_t at_task_control_task_configuration_destroy(struct at_task_s **task)
{
	assert(task);
	assert(*task);

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
at_task_control_task_configuration_suspend(struct at_task_s *task)
{
	assert(task);

	task->info.status = AT_TASK_STATUS_SUSPEND;

	void *task_addr = NULL;

	if (NULL == (task_addr = at_task_os_task_list_package->element_access.
				 top(at_task_os.list_table[task->info.priority].ready))) {
		return 1;
	}

	at_task_os_task_list_package->modifiers.
		push(at_task_os.list_table[task->info.priority].suspend, task_addr);
	at_task_os_task_list_package->modifiers.
		pop(at_task_os.list_table[task->info.priority].ready);

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
at_task_control_task_configuration_resume(struct at_task_s *task)
{
	assert(task);

	if (AT_TASK_STATUS_READY == task->info.status) {
		return 1;
	}

	task->info.status = AT_TASK_STATUS_READY;

	void *task_addr = NULL;

	if (NULL == (task_addr = at_task_os_task_list_package->element_access.
				 top(at_task_os.list_table[task->info.priority].suspend))) {
		return 2;
	}

	at_task_os_task_list_package->modifiers.
		push(at_task_os.list_table[task->info.priority].ready, task_addr);
	at_task_os_task_list_package->modifiers.
		pop(at_task_os.list_table[task->info.priority].suspend);

	return 0;
}

/**
 * @brief This function will be call when the at task os idle task is running.
 *
 * @param void
 *
 * @return void
 */

void at_task_os_control_idle_task_function(void *arg)
{
	printf("at task os.idle task \r\n");

	//assert(task);

	//task->info.status = AT_TASK_STATUS_SUSPEND;
}