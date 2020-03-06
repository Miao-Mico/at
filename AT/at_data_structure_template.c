/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include "at_data_structure_template.h"

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

/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

#ifdef __STACK_H

/**
 * @brief This function will initialize the ready list.
 *
 * @param void
 *
 * @return void
 */

errno_t at_list_stack_package_configration_init(void *arg);

/**
 * @brief This function will destroy the ready list stack.
 *
 * @param void
 *
 * @return void
 */

errno_t at_list_stack_package_configration_destroy(void *arg);

/**
 * @brief This function will push the data as the top one of the stack.
 *
 * @param void
 *
 * @return void
 */

at_task_size_t at_list_stack_package_capacity_size(void *arg);

/**
 * @brief This function will access the top element of the stack.
 *
 * @param void
 *
 * @return void
 */

void *at_list_stack_package_element_access_top(void *arg);

/**
 * @brief This function will push the data as the top one of the stack.
 *
 * @param void
 *
 * @return void
 */

errno_t at_list_stack_package_modifiers_push(void *arg, void *data);

/**
 * @brief This function will pop the top element of the stack.
 *
 * @param void
 *
 * @return void
 */

errno_t at_list_stack_package_modifiers_pop(void *arg);

#endif // __STACK_H

#ifdef __QUEUE_H

/**
 * @brief This function will initialize the ready list.
 *
 * @param void
 *
 * @return void
 */

errno_t at_list_queue_package_configration_init(void *arg);

/**
 * @brief This function will destroy the ready list stack.
 *
 * @param void
 *
 * @return void
 */

errno_t at_list_queue_package_configration_destroy(void *arg);

/**
 * @brief This function will push the data as the top one of the stack.
 *
 * @param void
 *
 * @return void
 */

at_task_size_t at_list_queue_package_capacity_size(void *arg);

/**
 * @brief This function will access the top element of the stack.
 *
 * @param void
 *
 * @return void
 */

void *at_list_queue_package_element_access_top(void *arg);

/**
 * @brief This function will push the data as the top one of the stack.
 *
 * @param void
 *
 * @return void
 */

errno_t at_list_queue_package_modifiers_push(void *arg, void *data);

/**
 * @brief This function will pop the top element of the stack.
 *
 * @param void
 *
 * @return void
 */

errno_t at_list_queue_package_modifiers_pop(void *arg);

#endif // __QUEUE_H

#ifdef __PRIORITY_QUEUE_H

/**
 * @brief This function will initialize the ready list.
 *
 * @param void
 *
 * @return void
 */

errno_t at_list_priority_queue_package_configration_init(void *arg);

/**
 * @brief This function will destroy the ready list stack.
 *
 * @param void
 *
 * @return void
 */

errno_t at_list_priority_queue_package_configration_destroy(void *arg);

/**
 * @brief This function will push the data as the top one of the stack.
 *
 * @param void
 *
 * @return void
 */

at_task_size_t at_list_priority_queue_package_capacity_size(void *arg);

/**
 * @brief This function will access the top element of the stack.
 *
 * @param void
 *
 * @return void
 */

void *at_list_priority_queue_package_element_access_top(void *arg);

/**
 * @brief This function will push the data as the top one of the stack.
 *
 * @param void
 *
 * @return void
 */

errno_t at_list_priority_queue_package_modifiers_push(void *arg, void *data);

/**
 * @brief This function will pop the top element of the stack.
 *
 * @param void
 *
 * @return void
 */

errno_t at_list_priority_queue_package_modifiers_pop(void *arg);

#endif // __PRIORITY_QUEUE_H

/*
*********************************************************************************************************
*					LOCAL GLOBAL VARIABLES & LOCAL FUNCTION PROTOTYPES INTERSECTION
*********************************************************************************************************
*/

#ifdef __STACK_H

/**
 * @brief This struct will contain all the at task list stack control functions.
 */

struct at_data_structure_package_s at_list_stack_package = {
	.configuration.init = (at_import_func_t)at_list_stack_package_configration_init,
	.configuration.destroy = (at_import_func_t)at_list_stack_package_configration_destroy,

	.capacity.size = (at_import_func_t)at_list_stack_package_capacity_size,

	.element_access.top = (at_import_func_t)at_list_stack_package_element_access_top,

	.modifiers.push = (at_import_func_t)at_list_stack_package_modifiers_push,
	.modifiers.pop = (at_import_func_t)at_list_stack_package_modifiers_pop,
};

#endif // __STACK_H

#ifdef __QUEUE_H

/**
 * @brief This struct will contain all the at task list stack control functions.
 */

struct at_data_structure_package_s at_list_queue_package = {
	.configuration.init = (at_import_func_t)at_list_queue_package_configration_init,
	.configuration.destroy = (at_import_func_t)at_list_queue_package_configration_destroy,

	.capacity.size = (at_import_func_t)at_list_queue_package_capacity_size,

	.element_access.top = (at_import_func_t)at_list_queue_package_element_access_top,

	.modifiers.push = (at_import_func_t)at_list_queue_package_modifiers_push,
	.modifiers.pop = (at_import_func_t)at_list_queue_package_modifiers_pop,
};

#endif // __QUEUE_H

#ifdef __PRIORITY_QUEUE_H

/**
 * @brief This struct will contain all the at task list stack control functions.
 */

struct at_data_structure_package_s at_list_priority_queue_package = {
	.configuration.init = (at_import_func_t)at_list_priority_queue_package_configration_init,
	.configuration.destroy = (at_import_func_t)at_list_priority_queue_package_configration_destroy,

	.capacity.size = (at_import_func_t)at_list_priority_queue_package_capacity_size,

	.element_access.top = (at_import_func_t)at_list_priority_queue_package_element_access_top,

	.modifiers.push = (at_import_func_t)at_list_priority_queue_package_modifiers_push,
	.modifiers.pop = (at_import_func_t)at_list_priority_queue_package_modifiers_pop,
};

#endif // __PRIORITY_QUEUE_H

/*
*********************************************************************************************************
*                                            FUNCTIONS
*********************************************************************************************************
*/

#ifdef __STACK_H

/**
 * @brief This function will initialize the ready list.
 *
 * @param void
 *
 * @return void
 */

static inline errno_t
at_list_stack_package_configration_init(void *arg)
{
	stack_ctrl.configuration.init((stack_stpp)arg,                       /* Initialize the data structure */
								  FORWARD_LIST,
								  sizeof(void *),
								  NULL, NULL);

	return 0;
}

/**
 * @brief This function will destroy the ready list stack.
 *
 * @param void
 *
 * @return void
 */

static inline errno_t
at_list_stack_package_configration_destroy(void *arg)
{
	stack_ctrl.configuration.destroy((stack_stpp)arg);                   /* Destroy the data structure */

	return 0;
}

/**
 * @brief This function will push the data as the top one of the stack.
 *
 * @param void
 *
 * @return void
 */

static inline at_task_size_t
at_list_stack_package_capacity_size(void *arg)
{
	assert(arg);

	#if (AT_TASK_DATA_STRUCTURE_CFG_DEBUG_EN)

	printf("ready list %p .push:%p\r\n",
		   arg, data);

	#endif // (AT_TASK_DATA_STRUCTURE_CFG_DEBUG_EN)

	return (at_task_size_t)stack_ctrl.capacity.size((stack_stp)arg);
}

/**
 * @brief This function will access the top element of the stack.
 *
 * @param void
 *
 * @return void
 */

static inline void
*at_list_stack_package_element_access_top(void *arg)
{
	assert(arg);

	void *data = NULL;

	if (NULL == (data = stack_ctrl.element_access.top((stack_stp)arg))) {
		return NULL;
	}

	#if (AT_TASK_DATA_STRUCTURE_CFG_DEBUG_EN)

	printf("ready list %p .top:%p \r\n",
		   arg, *(void **)data);

	#endif // (AT_TASK_DATA_STRUCTURE_CFG_DEBUG_EN)

	return (*(void **)data);
}

/**
 * @brief This function will push the data as the top one of the stack.
 *
 * @param void
 *
 * @return void
 */

static inline errno_t
at_list_stack_package_modifiers_push(void *arg, void *data)
{
	assert(arg);

	#if (AT_TASK_DATA_STRUCTURE_CFG_DEBUG_EN)

	printf("ready list %p .push:%p\r\n",
		   arg, data);

	#endif // (AT_TASK_DATA_STRUCTURE_CFG_DEBUG_EN)

	stack_ctrl.modifiers.push((stack_stp)arg,
							  &data);

	return 0;
}

/**
 * @brief This function will pop the top element of the stack.
 *
 * @param void
 *
 * @return void
 */

static inline errno_t
at_list_stack_package_modifiers_pop(void *arg)
{
	assert(arg);

	stack_ctrl.modifiers.pop(arg);

	return 0;
}

#endif // __STACK_H

#ifdef __QUEUE_H

/**
 * @brief This function will initialize the ready list.
 *
 * @param void
 *
 * @return void
 */

static inline errno_t
at_list_queue_package_configration_init(void *arg)
{
	queue_ctrl.configuration.init((queue_stp *)arg,                       /* Initialize the data structure */
								  FORWARD_LIST,
								  sizeof(void *),
								  NULL, NULL);

	return 0;
}

/**
 * @brief This function will destroy the ready list stack.
 *
 * @param void
 *
 * @return void
 */

static inline errno_t
at_list_queue_package_configration_destroy(void *arg)
{
	queue_ctrl.configuration.destroy((queue_stp *)arg);                   /* Destroy the data structure */

	return 0;
}

/**
 * @brief This function will push the data as the top one of the stack.
 *
 * @param void
 *
 * @return void
 */

static inline at_task_size_t
at_list_queue_package_capacity_size(void *arg)
{
	assert(arg);

	#if (AT_TASK_DATA_STRUCTURE_CFG_DEBUG_EN)

	printf("ready list %p .push:%p\r\n",
		   arg, data);

	#endif // (AT_TASK_DATA_STRUCTURE_CFG_DEBUG_EN)

	return (at_task_size_t)queue_ctrl.capacity.size((queue_stp)arg);
}

/**
 * @brief This function will access the top element of the stack.
 *
 * @param void
 *
 * @return void
 */

static inline void
*at_list_queue_package_element_access_top(void *arg)
{
	assert(arg);

	void *data = NULL;

	if (NULL == (data = queue_ctrl.element_access.front((queue_stp)arg))) {
		return NULL;
	}

	#if (AT_TASK_DATA_STRUCTURE_CFG_DEBUG_EN)

	printf("ready list %p .top:%p \r\n",
		   arg, *(void **)data);

	#endif // (AT_TASK_DATA_STRUCTURE_CFG_DEBUG_EN)

	return (*(void **)data);
}

/**
 * @brief This function will push the data as the top one of the stack.
 *
 * @param void
 *
 * @return void
 */

static inline errno_t
at_list_queue_package_modifiers_push(void *arg, void *data)
{
	assert(arg);

	#if (AT_TASK_DATA_STRUCTURE_CFG_DEBUG_EN)

	printf("ready list %p .push:%p\r\n",
		   arg, data);

	#endif // (AT_TASK_DATA_STRUCTURE_CFG_DEBUG_EN)

	queue_ctrl.modifiers.push((queue_stp)arg,
							  &data);

	return 0;
}

/**
 * @brief This function will pop the top element of the stack.
 *
 * @param void
 *
 * @return void
 */

static inline errno_t
at_list_queue_package_modifiers_pop(void *arg)
{
	assert(arg);

	queue_ctrl.modifiers.pop((queue_stp)arg);

	return 0;
}

#endif // __QUEUE_H

#ifdef __PRIORITY_QUEUE_H

/**
 * @brief This function will initialize the ready list.
 *
 * @param void
 *
 * @return void
 */

static inline errno_t
at_list_priority_queue_package_configration_init(void *arg)
{
	priority_queue_ctrl.configuration.init((priority_queue_stp *)arg,                       /* Initialize the data structure */
										   FORWARD_LIST,
										   sizeof(void *),
                                           NULL,
										   NULL, NULL);

	return 0;
}

/**
 * @brief This function will destroy the ready list stack.
 *
 * @param void
 *
 * @return void
 */

static inline errno_t
at_list_priority_queue_package_configration_destroy(void *arg)
{
	priority_queue_ctrl.configuration.destroy((priority_queue_stp *)arg);                   /* Destroy the data structure */

	return 0;
}

/**
 * @brief This function will push the data as the top one of the stack.
 *
 * @param void
 *
 * @return void
 */

static inline at_task_size_t
at_list_priority_queue_package_capacity_size(void *arg)
{
	assert(arg);

	#if (AT_TASK_DATA_STRUCTURE_CFG_DEBUG_EN)

	printf("ready list %p .push:%p\r\n",
		   arg, data);

	#endif // (AT_TASK_DATA_STRUCTURE_CFG_DEBUG_EN)

	return (at_task_size_t)priority_queue_ctrl.capacity.size((priority_queue_stp)arg);
}

/**
 * @brief This function will access the top element of the stack.
 *
 * @param void
 *
 * @return void
 */

static inline void
*at_list_priority_queue_package_element_access_top(void *arg)
{
	assert(arg);

	void *data = NULL;

	if (NULL == (data = priority_queue_ctrl.element_access.top((priority_queue_stp)arg))) {
		return NULL;
	}

	#if (AT_TASK_DATA_STRUCTURE_CFG_DEBUG_EN)

	printf("ready list %p .top:%p \r\n",
		   arg, *(void **)data);

	#endif // (AT_TASK_DATA_STRUCTURE_CFG_DEBUG_EN)

	return (*(void **)data);
}

/**
 * @brief This function will push the data as the top one of the stack.
 *
 * @param void
 *
 * @return void
 */

static inline errno_t
at_list_priority_queue_package_modifiers_push(void *arg, void *data)
{
	assert(arg);

	#if (AT_TASK_DATA_STRUCTURE_CFG_DEBUG_EN)

	printf("ready list %p .push:%p\r\n",
		   arg, data);

	#endif // (AT_TASK_DATA_STRUCTURE_CFG_DEBUG_EN)

	priority_queue_ctrl.modifiers.push((priority_queue_stp)arg,
									   &data);

	return 0;
}

/**
 * @brief This function will pop the top element of the stack.
 *
 * @param void
 *
 * @return void
 */

static inline errno_t
at_list_priority_queue_package_modifiers_pop(void *arg)
{
	assert(arg);

	priority_queue_ctrl.modifiers.pop((priority_queue_stp)arg);

	return 0;
}

#endif // __PRIORITY_QUEUE_H