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

#define AT_TASK_CFG_LIST_STACK_COUNT										2u

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

	struct at_message_queue_unit_s mq_unit;

	struct at_message_queue_unit_s mq_unit_outward;

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

	struct at_message_queue_unit_s mq_unit;

	struct at_message_queue_unit_s mq_unit_outward;

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
	.task.message_queue.task_os.join = at_task_control_task_message_queue_task_os_join,
	.task.message_queue.task_os.quit = at_task_control_task_message_queue_task_os_quit,
	.task.message_queue.outward.join = at_task_control_task_message_queue_outward_join,
	.task.message_queue.outward.quit = at_task_control_task_message_queue_outward_quit,
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

/**
 * @brief This function will join the message queue.
 *
 * @param void
 *
 * @return void
 */

errno_t
at_task_control_task_message_queue_join_core(at_message_queue_stp mseeage_queue,
											 struct at_message_queue_unit_s *message_queue_unit);

/**
 * @brief This function will quit the message queue.
 *
 * @param void
 *
 * @return void
 */

errno_t
at_task_control_task_message_queue_quit_core(at_message_queue_stp mseeage_queue,
											 struct at_message_queue_unit_s *message_queue_unit);

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

errno_t at_task_control_os_configuration_init(struct at_task_os_s **task_os,
											  struct at_message_queue_s *message_queue)
{
	assert(task_os);
	assert(message_queue);

	errno_t err = 0;

	if (NULL == ((*task_os) = calloc(1, sizeof(struct at_task_os_s)))) {
		return -1;
	}

	if (0u == ((*task_os)->mq_unit_outward.id
			   = at_message_queue_ctrl.membership
			   .join(((*task_os)->mq_unit_outward.mq_ptr = message_queue)))) {				/* Join the message queue with outside */
		return 4;
	}

	if (at_message_queue_ctrl.configuration
		.init(&(*task_os)->mq_unit.mq_ptr,													/* Initialize the message queue of the at task os */
			  NULL, NULL, NULL)) {
		return 4;
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

	if (at_message_queue_ctrl.configuration
		.destroy(&(*task_os)->mq_unit.mq_ptr)) {											/* Destroy the message queue */
		return 1;
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
		static bool highest_priority = false;

		if (false == highest_priority
			&& task_os->priority_max == task_os->running_task_ptr->info.priority) {			/* First run should be the highest priority task */
			static char *message = NULL;

			if (NULL == (message
						 = at_message_pool_ctrl.feedback
						 .load(arg_list))) {												/* Load the feedback message when at the priority max */
				return;
			}

			if (at_message_queue_ctrl.communication
				.publish(task_os->mq_unit.mq_ptr,											/* publish the feedback message to message queue */
						 message,
						 2, 																/* todo:set a post cast code */
						 task_os->mq_unit.id)) {
				return;
			}

			highest_priority = true;
		}

		if (false == highest_priority) {
			return;
		}

		struct at_task_function_arguement_list_package_s
			arg_list_package = {
			.task_os = task_os,
			.task = task_os->running_task_ptr,
			.mq_unit = &task_os->running_task_ptr->mq_unit
		};

		task_os->running_task_ptr->function(&arg_list_package);								/* Run the function of the task */

		if ((at_task_size_t)AT_TASK_CFG_PRIORITY_MAX - 1 ==									/* When the idle task */
			task_os->running_task_ptr->info.priority) {
			highest_priority = false;
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

			if (at_task_os_task_list_package->modifiers
				.insert(task_os->task_list[priority].ready, *task)) {						/* Push the task address into the ready list of this priority */
				return 2;
			}
			break;
		case AT_TASK_OPTION_SUSPEND:
			(*task)->info.status = AT_TASK_STATUS_SUSPEND;									/* Set the ready status of the task */

			if (at_task_os_task_list_package->modifiers
				.insert(task_os->task_list[priority].suspend, *task)) {						/* Push the task address into the suspend list of this priority */
				return 3;
			}
			break;
		default:
			break;
	}

	if (NULL == at_task_control_task_message_queue_task_os_join(task_os,
																*task)) {					/* Join the message queue */
		return 4;
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

	if (NULL == (task_addr
				 = at_task_os_task_list_package->element_access
				 .at(task_os->task_list[task->info.priority].ready))) {						/* Access the pointer task from the ready list */
		return 1;
	}

	if (at_task_os_task_list_package->modifiers
		.insert(task_os->task_list[task->info.priority].suspend, task_addr)) {				/* Insert the task pointer into the suspend list */
		return 3;
	}

	if (at_task_os_task_list_package->modifiers
		.delete(task_os->task_list[task->info.priority].ready)) {							/* Delete the task pointer from the ready list */
		return 4;
	}

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

	if (NULL == (task_addr
				 = at_task_os_task_list_package->element_access
				 .at(task_os->task_list[task->info.priority].suspend))) {					/* Access the pointer task from the suspend list */
		return 2;
	}

	if (at_task_os_task_list_package->modifiers
		.insert(task_os->task_list[task->info.priority].ready, task_addr)) {				/* Insert the task pointer into the ready list */
		return 3;
	}

	if (at_task_os_task_list_package->modifiers
		.delete(task_os->task_list[task->info.priority].suspend)) {							/* Delete the task pointer from the suspend list */
		return 4;
	}

	return 0;
}

/**
 * @brief This function will join the message queue hosted by at task os.
 *
 * @param void
 *
 * @return void
 */

struct at_message_queue_unit_s
	*at_task_control_task_message_queue_task_os_join(struct at_task_os_s *task_os,
													 struct at_task_s *task)
{
	assert(task_os);
	assert(task);

	if (at_task_control_task_message_queue_join_core(task_os->mq_unit.mq_ptr,
													 &task->mq_unit)) {
		return NULL;
	}

	return &task->mq_unit;
}

/**
 * @brief This function will quit the message queue hosted by at task os.
 *
 * @param void
 *
 * @return void
 */

errno_t at_task_control_task_message_queue_task_os_quit(struct at_task_os_s *task_os,
														struct at_task_s *task)
{
	assert(task_os);
	assert(task);

	return at_task_control_task_message_queue_quit_core(task_os->mq_unit.mq_ptr,
														&task->mq_unit);
}

/**
 * @brief This function will join the message queue hosted by at task os.
 *
 * @param void
 *
 * @return void
 */

struct at_message_queue_unit_s
	*at_task_control_task_message_queue_outward_join(struct at_task_os_s *task_os,
													 struct at_task_s *task)
{
	assert(task_os);
	assert(task);

	if (at_task_control_task_message_queue_join_core(task_os->mq_unit_outward.mq_ptr,
													 &task->mq_unit_outward)) {
		return NULL;
	}

	return &task->mq_unit_outward;
}

/**
 * @brief This function will quit the message queue hosted by at task os.
 *
 * @param void
 *
 * @return void
 */

errno_t at_task_control_task_message_queue_outward_quit(struct at_task_os_s *task_os,
														struct at_task_s *task)
{
	assert(task_os);
	assert(task);

	return at_task_control_task_message_queue_quit_core(task_os->mq_unit_outward.mq_ptr,
														&task->mq_unit_outward);
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
	struct at_task_function_arguement_list_package_s
		*arg_list_package = arg_list;

	char *message = NULL;

	printf("at task os.idle task.enter\r\n");

	if (NULL == (message
				 = at_message_queue_ctrl.communication
				 .subscribe(arg_list_package->mq_unit->mq_ptr,								/* Subscribe the message from the mesasge queue */
							arg_list_package->mq_unit->id).message)) {
		return;
	}

	printf("at task os.idle task.message:\"%s\" \r\n", message);
}

/**
 * @brief This function will join the message queue.
 *
 * @param void
 *
 * @return void
 */

extern inline errno_t
at_task_control_task_message_queue_join_core(at_message_queue_stp mseeage_queue,
											 struct at_message_queue_unit_s *message_queue_unit)
{
	assert(mseeage_queue);
	assert(message_queue_unit);

	if (0u != message_queue_unit->id) {
		goto EXIT;
	}

	if (0u == (message_queue_unit->id
			   = at_message_queue_ctrl.membership
			   .join(mseeage_queue))) {														/* Join the message queue */
		return 2;
	}

	message_queue_unit->mq_ptr = mseeage_queue;

EXIT:

	return 0;
}

/**
 * @brief This function will quit the message queue.
 *
 * @param void
 *
 * @return void
 */

extern inline errno_t
at_task_control_task_message_queue_quit_core(at_message_queue_stp mseeage_queue,
											 struct at_message_queue_unit_s *message_queue_unit)
{
	assert(mseeage_queue);
	assert(message_queue_unit);

	if (0u != message_queue_unit->id
		&& at_message_queue_ctrl.membership
		.quit(mseeage_queue,																/* Quit the message queue */
			  message_queue_unit->id)) {
		return 1;
	}

	message_queue_unit->mq_ptr = NULL;

	return 0;
}