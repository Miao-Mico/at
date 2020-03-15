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

errno_t at_list_stack_control_package_configration_init(void *data_structure);

/**
 * @brief This function will destroy the ready list stack.
 *
 * @param void
 *
 * @return void
 */

errno_t at_list_stack_control_package_configration_destroy(void *data_structure);

/**
 * @brief This function will push the data as the at one of the stack.
 *
 * @param void
 *
 * @return void
 */

at_size_t at_list_stack_control_package_capacity_size(void *data_structure);

/**
 * @brief This function will access the at element of the stack.
 *
 * @param void
 *
 * @return void
 */

void *at_list_stack_control_package_element_access_at(void *data_structure,
													  at_size_t position);

/**
 * @brief This function will push the data as the at one of the stack.
 *
 * @param void
 *
 * @return void
 */

errno_t at_list_stack_control_package_modifiers_insert(void *data_structure,
													   void *data,
													   at_size_t position);

/**
 * @brief This function will pop the at element of the stack.
 *
 * @param void
 *
 * @return void
 */

errno_t at_list_stack_control_package_modifiers_delete(void *data_structure,
													   void *data,
													   at_size_t position);

#endif // __STACK_H

#ifdef __QUEUE_H

/**
 * @brief This function will initialize the ready list.
 *
 * @param void
 *
 * @return void
 */

errno_t at_list_queue_control_package_configration_init(void *data_structure);

/**
 * @brief This function will destroy the ready list stack.
 *
 * @param void
 *
 * @return void
 */

errno_t at_list_queue_control_package_configration_destroy(void *data_structure);

/**
 * @brief This function will push the data as the at one of the stack.
 *
 * @param void
 *
 * @return void
 */

at_size_t at_list_queue_control_package_capacity_size(void *data_structure);

/**
 * @brief This function will access the at element of the stack.
 *
 * @param void
 *
 * @return void
 */

void *at_list_queue_control_package_element_access_at(void *data_structure,
													  at_size_t position);

/**
 * @brief This function will push the data as the at one of the stack.
 *
 * @param void
 *
 * @return void
 */

errno_t at_list_queue_control_package_modifiers_insert(void *data_structure,
													   void *data,
													   at_size_t position);

/**
 * @brief This function will pop the at element of the stack.
 *
 * @param void
 *
 * @return void
 */

errno_t at_list_queue_control_package_modifiers_delete(void *data_structure,
													   void *data,
													   at_size_t position);

#endif // __QUEUE_H

#ifdef __PRIORITY_QUEUE_H

/**
 * @brief This function will initialize the ready list.
 *
 * @param void
 *
 * @return void
 */

errno_t at_list_priority_queue_control_package_configration_init(void *data_structure);

/**
 * @brief This function will destroy the ready list stack.
 *
 * @param void
 *
 * @return void
 */

errno_t at_list_priority_queue_control_package_configration_destroy(void *data_structure);

/**
 * @brief This function will push the data as the at one of the stack.
 *
 * @param void
 *
 * @return void
 */

at_size_t at_list_priority_queue_control_package_capacity_size(void *data_structure);

/**
 * @brief This function will access the at element of the stack.
 *
 * @param void
 *
 * @return void
 */

void *at_list_priority_queue_control_package_element_access_at(void *data_structure,
															   at_size_t position);

/**
 * @brief This function will push the data as the at one of the stack.
 *
 * @param void
 *
 * @return void
 */

errno_t at_list_priority_queue_control_package_modifiers_insert(void *data_structure,
																void *data,
																at_size_t position);

/**
 * @brief This function will pop the at element of the stack.
 *
 * @param void
 *
 * @return void
 */

errno_t at_list_priority_queue_control_package_modifiers_delete(void *data_structure,
																void *data,
																at_size_t position);

#endif // __PRIORITY_QUEUE_H

#ifdef __RED_BLACK_TREE_H

/**
 * @brief This function will initialize the ready list.
 *
 * @param void
 *
 * @return void
 */

errno_t at_red_black_tree_control_package_configration_init(void **data_structure);

/**
 * @brief This function will destroy the ready list stack.
 *
 * @param void
 *
 * @return void
 */

errno_t at_red_black_tree_control_package_configration_destroy(void **data_structure);

/**
 * @brief This function will push the data as the at one of the stack.
 *
 * @param void
 *
 * @return void
 */

at_size_t at_red_black_tree_control_package_capacity_size(void *data_structure);

/**
 * @brief This function will access the at element of the stack.
 *
 * @param void
 *
 * @return void
 */

void *at_red_black_tree_control_package_lookup_find(void *data_structure,
													at_size_t position);

	 /**
	  * @brief This function will push the data as the at one of the stack.
	  *
	  * @param void
	  *
	  * @return void
	  */

errno_t at_red_black_tree_control_package_modifiers_insert(void *data_structure,
														   void *data,
														   at_size_t position);

/**
 * @brief This function will pop the at element of the stack.
 *
 * @param void
 *
 * @return void
 */

errno_t at_red_black_tree_control_package_modifiers_delete(void *data_structure,
														   void *data,
														   at_size_t position);

#endif // __RED_BLACK_TREE_H

/*
*********************************************************************************************************
*					LOCAL GLOBAL VARIABLES & LOCAL FUNCTION PROTOTYPES INTERSECTION
*********************************************************************************************************
*/

#ifdef __STACK_H

/**
 * @brief This struct will contain all the at task list stack control functions.
 */

struct at_data_structure_control_package_s at_list_stack_control_package = {
	.configuration.init = at_list_stack_control_package_configration_init,
	.configuration.destroy = at_list_stack_control_package_configration_destroy,

	.capacity.size = at_list_stack_control_package_capacity_size,

	.element_access.at = (at_import_func_t)at_list_stack_control_package_element_access_at,

	.modifiers.insert = at_list_stack_control_package_modifiers_insert,
	.modifiers.delete = at_list_stack_control_package_modifiers_delete,
};

#endif // __STACK_H

#ifdef __QUEUE_H

/**
 * @brief This struct will contain all the at task list stack control functions.
 */

struct at_data_structure_control_package_s at_list_queue_control_package = {
	.configuration.init = at_list_queue_control_package_configration_init,
	.configuration.destroy = at_list_queue_control_package_configration_destroy,

	.capacity.size = at_list_queue_control_package_capacity_size,

	.element_access.at = (at_import_func_t)at_list_queue_control_package_element_access_at,

	.modifiers.insert = at_list_queue_control_package_modifiers_insert,
	.modifiers.delete = at_list_queue_control_package_modifiers_delete,
};

#endif // __QUEUE_H

#ifdef __PRIORITY_QUEUE_H

/**
 * @brief This struct will contain all the at task list stack control functions.
 */

struct at_data_structure_control_package_s at_list_priority_queue_control_package = {
	.configuration.init = at_list_priority_queue_control_package_configration_init,
	.configuration.destroy = at_list_priority_queue_control_package_configration_destroy,

	.capacity.size = at_list_priority_queue_control_package_capacity_size,

	.element_access.at = (at_import_func_t)at_list_priority_queue_control_package_element_access_at,

	.modifiers.insert = at_list_priority_queue_control_package_modifiers_insert,
	.modifiers.delete = at_list_priority_queue_control_package_modifiers_delete,
};

#endif // __PRIORITY_QUEUE_H

#ifdef __RED_BLACK_TREE_H

/**
 * @brief This struct will contain all the at task list stack control functions.
 */

struct at_data_structure_control_package_s at_red_black_tree_control_package = {
	.configuration.init = at_red_black_tree_control_package_configration_init,
	.configuration.destroy = at_red_black_tree_control_package_configration_destroy,

	.capacity.size = at_red_black_tree_control_package_capacity_size,

	.lookup.search = at_red_black_tree_control_package_lookup_find,

	.modifiers.insert = at_red_black_tree_control_package_modifiers_insert,
	.modifiers.delete = at_red_black_tree_control_package_modifiers_delete,
};

#endif // __RED_BLACK_TREE_H

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
at_list_stack_control_package_configration_init(void *data_structure)
{
	stack_ctrl.configuration.init((stack_stpp)data_structure,                       /* Initialize the data structure */
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
at_list_stack_control_package_configration_destroy(void *data_structure)
{
	stack_ctrl.configuration.destroy((stack_stpp)data_structure);                   /* Destroy the data structure */

	return 0;
}

/**
 * @brief This function will push the data as the at one of the stack.
 *
 * @param void
 *
 * @return void
 */

static inline at_size_t
at_list_stack_control_package_capacity_size(void *data_structure)
{
	assert(data_structure);

	#if (AT_TASK_DATA_STRUCTURE_CFG_DEBUG_EN)

	printf("ready list %p .push:%p\r\n",
		   data_structure, data);

	#endif // (AT_TASK_DATA_STRUCTURE_CFG_DEBUG_EN)

	return (at_size_t)stack_ctrl.capacity.size((stack_stp)data_structure);
}

/**
 * @brief This function will access the at element of the stack.
 *
 * @param void
 *
 * @return void
 */

static inline void
*at_list_stack_control_package_element_access_at(void *data_structure,
												 at_size_t position)
{
	assert(data_structure);

	void *data = NULL;

	if (NULL == (data = stack_ctrl.element_access.top((stack_stp)data_structure))) {
		return NULL;
	}

	#if (AT_TASK_DATA_STRUCTURE_CFG_DEBUG_EN)

	printf("ready list %p .at:%p \r\n",
		   data_structure, *(void **)data);

	#endif // (AT_TASK_DATA_STRUCTURE_CFG_DEBUG_EN)

	return (*(void **)data);
}

/**
 * @brief This function will push the data as the at one of the stack.
 *
 * @param void
 *
 * @return void
 */

static inline errno_t
at_list_stack_control_package_modifiers_insert(void *data_structure,
											   void *data,
											   at_size_t position)
{
	assert(data_structure);
	assert(data);

	#if (AT_TASK_DATA_STRUCTURE_CFG_DEBUG_EN)

	printf("ready list %p .push:%p\r\n",
		   data_structure, data);

	#endif // (AT_TASK_DATA_STRUCTURE_CFG_DEBUG_EN)

	stack_ctrl.modifiers.push((stack_stp)data_structure,
							  &data);

	return 0;
}

/**
 * @brief This function will pop the at element of the stack.
 *
 * @param void
 *
 * @return void
 */

static inline errno_t
at_list_stack_control_package_modifiers_delete(void *data_structure,
											   void *data,
											   at_size_t position)
{
	assert(data_structure);

	stack_ctrl.modifiers.pop(data_structure);

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
at_list_queue_control_package_configration_init(void *data_structure)
{
	queue_ctrl.configuration.init((queue_stp *)data_structure,                       /* Initialize the data structure */
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
at_list_queue_control_package_configration_destroy(void *data_structure)
{
	queue_ctrl.configuration.destroy((queue_stp *)data_structure);                   /* Destroy the data structure */

	return 0;
}

/**
 * @brief This function will push the data as the at one of the stack.
 *
 * @param void
 *
 * @return void
 */

static inline at_size_t
at_list_queue_control_package_capacity_size(void *data_structure)
{
	assert(data_structure);

	#if (AT_TASK_DATA_STRUCTURE_CFG_DEBUG_EN)

	printf("ready list %p .push:%p\r\n",
		   data_structure, data);

	#endif // (AT_TASK_DATA_STRUCTURE_CFG_DEBUG_EN)

	return (at_size_t)queue_ctrl.capacity.size((queue_stp)data_structure);
}

/**
 * @brief This function will access the at element of the stack.
 *
 * @param void
 *
 * @return void
 */

static inline void
*at_list_queue_control_package_element_access_at(void *data_structure,
												 at_size_t position)
{
	assert(data_structure);

	void *data = NULL;

	if (NULL == (data = queue_ctrl.element_access.front((queue_stp)data_structure))) {
		return NULL;
	}

	#if (AT_TASK_DATA_STRUCTURE_CFG_DEBUG_EN)

	printf("ready list %p .at:%p \r\n",
		   data_structure, *(void **)data);

	#endif // (AT_TASK_DATA_STRUCTURE_CFG_DEBUG_EN)

	return (*(void **)data);
}

/**
 * @brief This function will push the data as the at one of the stack.
 *
 * @param void
 *
 * @return void
 */

static inline errno_t
at_list_queue_control_package_modifiers_insert(void *data_structure,
											   void *data,
											   at_size_t position)
{
	assert(data_structure);
	assert(data);

	#if (AT_TASK_DATA_STRUCTURE_CFG_DEBUG_EN)

	printf("ready list %p .push:%p\r\n",
		   data_structure, data);

	#endif // (AT_TASK_DATA_STRUCTURE_CFG_DEBUG_EN)

	queue_ctrl.modifiers.push((queue_stp)data_structure,
							  &data);

	return 0;
}

/**
 * @brief This function will pop the at element of the stack.
 *
 * @param void
 *
 * @return void
 */

static inline errno_t
at_list_queue_control_package_modifiers_delete(void *data_structure,
											   void *data,
											   at_size_t position)
{
	assert(data_structure);

	queue_ctrl.modifiers.pop((queue_stp)data_structure);

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
at_list_priority_queue_control_package_configration_init(void *data_structure)
{
	priority_queue_ctrl.configuration.init((priority_queue_stp *)data_structure,                       /* Initialize the data structure */
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
at_list_priority_queue_control_package_configration_destroy(void *data_structure)
{
	priority_queue_ctrl.configuration.destroy((priority_queue_stp *)data_structure);                   /* Destroy the data structure */

	return 0;
}

/**
 * @brief This function will push the data as the at one of the stack.
 *
 * @param void
 *
 * @return void
 */

static inline at_size_t
at_list_priority_queue_control_package_capacity_size(void *data_structure)
{
	assert(data_structure);

	#if (AT_TASK_DATA_STRUCTURE_CFG_DEBUG_EN)

	printf("ready list %p .push:%p\r\n",
		   data_structure, data);

	#endif // (AT_TASK_DATA_STRUCTURE_CFG_DEBUG_EN)

	return (at_size_t)priority_queue_ctrl.capacity.size((priority_queue_stp)data_structure);
}

/**
 * @brief This function will access the at element of the stack.
 *
 * @param void
 *
 * @return void
 */

static inline void
*at_list_priority_queue_control_package_element_access_at(void *data_structure,
														  at_size_t position)
{
	assert(data_structure);

	void *data = NULL;

	if (NULL == (data = priority_queue_ctrl.element_access
				 .top((priority_queue_stp)data_structure))) {
		return NULL;
	}

	#if (AT_TASK_DATA_STRUCTURE_CFG_DEBUG_EN)

	printf("ready list %p .at:%p \r\n",
		   data_structure, *(void **)data);

	#endif // (AT_TASK_DATA_STRUCTURE_CFG_DEBUG_EN)

	return (*(void **)data);
}

/**
 * @brief This function will push the data as the at one of the stack.
 *
 * @param void
 *
 * @return void
 */

static inline errno_t
at_list_priority_queue_control_package_modifiers_insert(void *data_structure,
														void *data,
														at_size_t position)
{
	assert(data_structure);

	#if (AT_TASK_DATA_STRUCTURE_CFG_DEBUG_EN)

	printf("ready list %p .push:%p\r\n",
		   data_structure, data);

	#endif // (AT_TASK_DATA_STRUCTURE_CFG_DEBUG_EN)

	priority_queue_ctrl.modifiers.push((priority_queue_stp)data_structure,
									   &data);

	return 0;
}

/**
 * @brief This function will pop the at element of the stack.
 *
 * @param void
 *
 * @return void
 */

static inline errno_t
at_list_priority_queue_control_package_modifiers_delete(void *data_structure,
														void *data,
														at_size_t position)
{
	assert(data_structure);

	priority_queue_ctrl.modifiers.pop((priority_queue_stp)data_structure);

	return 0;
}

#endif // __PRIORITY_QUEUE_H

#ifdef __RED_BLACK_TREE_H

/**
 * @brief This function will initialize the ready list.
 *
 * @param void
 *
 * @return void
 */

static inline errno_t
at_red_black_tree_control_package_configration_init(void **data_structure)
{
	red_black_tree_control_configuration_init((struct tree_family_s **)data_structure,
											  10,
											  NULL,
											  NULL);

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
at_red_black_tree_control_package_configration_destroy(void **data_structure)
{
	tree_family_control_configuration_destroy((struct tree_family_s **)data_structure);     /* Destroy the data structure */

	return 0;
}

/**
 * @brief This function will push the data as the at one of the stack.
 *
 * @param void
 *
 * @return void
 */

static inline at_size_t
at_red_black_tree_control_package_capacity_size(void *data_structure)
{
	assert(data_structure);

	return 0;
}

/**
 * @brief This function will access the at element of the stack.
 *
 * @param void
 *
 * @return void
 */

static inline void
*at_red_black_tree_control_package_lookup_find(void *data_structure,
											   at_size_t position)
{
	assert(data_structure);

	tree_family_search_node_return_st search_return =
		tree_family_control_search((struct tree_family_s *)data_structure,
		(void *)&position);

	if (NULL == search_return.node) {
		return NULL;
	}

	return *(((void **)search_return.content) + 1);
}

/**
 * @brief This function will push the data as the at one of the stack.
 *
 * @param void
 *
 * @return void
 */

static inline errno_t
at_red_black_tree_control_package_modifiers_insert(void *data_structure,
												   void *data,
												   at_size_t position)
{
	assert(data_structure);

	struct specific_data_template_s {
		size_t id;

		int mark;
		void *data;
	}specific_data = { 0 };

	specific_data.id = *(size_t *)data;
	specific_data.data = *((void **)data + 1);

	tree_family_control_insert((struct tree_family_s *)data_structure,
							   &specific_data);

	return 0;
}

/**
 * @brief This function will pop the at element of the stack.
 *
 * @param void
 *
 * @return void
 */

static inline errno_t
at_red_black_tree_control_package_modifiers_delete(void *data_structure,
												   void *data,
												   at_size_t position)
{
	assert(data_structure);

	tree_family_control_delete((struct tree_family_s *)data_structure,
		(void *)&position);

	return 0;
}

#endif // __RED_BLACK_TREE_H