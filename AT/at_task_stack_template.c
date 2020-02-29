/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include "at_task_stack_template.h"

#include "stack.h"

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

/**
 * @brief This function will initialize the ready list of the at task os as a stack.
 *
 * @param void
 *
 * @return void
 */

errno_t at_task_os_control_ready_list_init(void *arg);

/**
 * @brief This function will destroy the ready list of the at task os.
 *
 * @param void
 *
 * @return void
 */

errno_t at_task_os_control_ready_list_destroy(void *arg);

/**
 * @brief This function will push the data as the top one of the stack.
 *
 * @param void
 *
 * @return void
 */

errno_t at_task_os_control_ready_list_push(void *arg,
										   void *data);

/**
 * @brief This function will access the top element of the stack.
 *
 * @param void
 *
 * @return void
 */

void *at_task_os_control_ready_list_top(void *arg);

/**
 * @brief This function will pop the top element of the stack.
 *
 * @param void
 *
 * @return void
 */

errno_t at_task_os_control_ready_list_pop(void *arg);

/*
*********************************************************************************************************
*					LOCAL GLOBAL VARIABLES & LOCAL FUNCTION PROTOTYPES INTERSECTION
*********************************************************************************************************
*/

/**
 * @brief This struct will contain all the at task control functions.
 */

struct at_task_data_structure_package_s ready_list_stack_package = {
	.configuration.init = (at_import_func_t)at_task_os_control_ready_list_init,
	.configuration.destroy = (at_import_func_t)at_task_os_control_ready_list_destroy,

	.access.top = (at_import_func_t)at_task_os_control_ready_list_top,

	.modifiers.push = (at_import_func_t)at_task_os_control_ready_list_push,
	.modifiers.pop = (at_import_func_t)at_task_os_control_ready_list_pop,
};

/*
*********************************************************************************************************
*                                            FUNCTIONS
*********************************************************************************************************
*/

/**
 * @brief This function will initialize the ready list.
 *
 * @param void
 *
 * @return void
 */

static inline errno_t
at_task_os_control_ready_list_init(void *arg)
{
	stack_ctrl.configuration.init((struct stack_s **) arg,                       /* Initialize the data structure */
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
at_task_os_control_ready_list_destroy(void *arg)
{
	stack_ctrl.configuration.destroy((struct stack_s **) arg);                   /* Destroy the data structure */

	return 0;
}

/**
 * @brief This function will push the data as the top one of the stack.
 *
 * @param void
 *
 * @return void
 */

static inline errno_t
at_task_os_control_ready_list_push(void *arg, void *data)
{
	assert(arg);

    #if (AT_TASK_DATA_STRUCTURE_CFG_DEBUG_EN)

	printf("ready list %p .push:%p\r\n",
		   arg, data);

    #endif // (AT_TASK_DATA_STRUCTURE_CFG_DEBUG_EN)

	stack_ctrl.modifiers.push((struct stack_s *)arg,
							  &data);

	return 0;
}

/**
 * @brief This function will access the top element of the stack.
 *
 * @param void
 *
 * @return void
 */

static inline void
*at_task_os_control_ready_list_top(void *arg)
{
	assert(arg);

	void *data = NULL;

	if (NULL == (data = stack_ctrl.element_access.top((struct stack_s *)arg))) {
		return NULL;
	}

	#if (AT_TASK_DATA_STRUCTURE_CFG_DEBUG_EN)

	printf("ready list %p .top:%p \r\n",
		   arg, *(void **)data);

    #endif // (AT_TASK_DATA_STRUCTURE_CFG_DEBUG_EN)

	return (*(void **)data);
}

/**
 * @brief This function will pop the top element of the stack.
 *
 * @param void
 *
 * @return void
 */

static inline errno_t
at_task_os_control_ready_list_pop(void *arg)
{
	assert(arg);

	stack_ctrl.modifiers.pop(arg);

	return 0;
}