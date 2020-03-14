/*
*********************************************************************************************************
*                                               MODULE
*
* Note(s) : (1) This definition header file is protected from multiple pre-processor inclusion
*               through use of the definition module present pre-processor macro definition.
*********************************************************************************************************
*/

#ifndef __AT_TASK_H
#define __AT_TASK_H

/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include "at_def.h"

/*
*********************************************************************************************************
*									            DEFINES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*									           DATA TYPES
*********************************************************************************************************
*/

/**
 * @brief This struct will contain all the at task control functions.
 */

typedef void (*at_task_function_t)(void *arg_list);

/**
 * @brief This struct will contain all the at task control functions.
 */

struct at_task_function_arguement_list_package_s {
	void *mq_unit_ptr;

	void *message;

	void *err;
};

/**
 * @brief This struct will contain all the at task control functions.
 */

struct at_task_control_s {
	struct {
		struct {
			errno_t(*init)(struct at_task_os_s **task_os,
						   struct at_message_queue_s *message_queue);

			errno_t(*destroy)(struct at_task_os_s **task_os);
		}configuration;

		struct {
			at_task_size_t(*highest_priority)(struct at_task_os_s *task_os);
		}inquire;

		void (*core)(struct at_task_os_s *task_os,
					 void *arg_list);
	}os;

	struct {
		struct {
			errno_t(*init)(struct at_task_os_s *task_os,
						   struct at_task_s **task,
						   char *name,
						   void *func,
						   at_task_size_t priority,
						   bool join_message_queue,
						   void *hook,
						   enum at_task_option_e opt);

			errno_t(*destroy)(struct at_task_os_s *task_os,
							  struct at_task_s **task);

			errno_t(*suspend)(struct at_task_os_s *task_os,
							  struct at_task_s *task);

			errno_t(*rusume)(struct at_task_os_s *task_os,
							 struct at_task_s *task);
		}configuration;
	}task;
};

/*
*********************************************************************************************************
*								            FUNCTION PROTOTYPES
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
											  struct at_message_queue_s *message_queue);

/**
 * @brief This function will destroy the at task os.
 *
 * @param void
 *
 * @return void
 */

errno_t at_task_control_os_configuration_destroy(struct at_task_os_s **task_os);

/**
 * @brief This function will manage the priority of the os.
 *
 * @param void
 *
 * @return void
 */

void at_task_control_os_priority_management(struct at_task_os_s *task_os);

/**
 * @brief This function will schedule the tasks of the os.
 *
 * @param void
 *
 * @return void
 */

void at_task_control_os_core(struct at_task_os_s *task_os,
							 void *arg_list);

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
												bool join_message_queue,
												void *hook,
												enum at_task_option_e opt);

/**
 * @brief This function will destroy the task of at task os.
 *
 * @param void
 *
 * @return void
 */

errno_t at_task_control_task_configuration_destroy(struct at_task_os_s *task_os,
												   struct at_task_s **task);

/**
 * @brief This function will suspend the task of the at task os.
 *
 * @param void
 *
 * @return void
 */

errno_t at_task_control_task_configuration_suspend(struct at_task_os_s *task_os,
												   struct at_task_s *task);

/**
 * @brief This function will resume the task of the at task os.
 *
 * @param void
 *
 * @return void
 */

errno_t at_task_control_task_configuration_resume(struct at_task_os_s *task_os,
												  struct at_task_s *task);

/*
*********************************************************************************************************
*                                       EXTERN GLOBAL VARIABLES
*********************************************************************************************************
*/

/**
 * @brief This struct is the body of at task.
 */

extern struct at_task_s *at_task_os_idle_task;

/**
 * @brief This struct will contain all the at task control functions.
 */

extern struct at_task_control_s at_task_ctrl;

/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif // !__AT_TASK_H