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

typedef void (*at_task_function_t)(void *arg);

/**
 * @brief This struct will contain all the at task control functions.
 */

struct at_task_control_s {
	struct {
		struct {
			void (*init)(void);

			void (*destroy)(void);
		}configuration;

		struct {
			void (*highest_priority)(void);
		}inquire;

		void (*core)(void);
	}os;

	struct {
		struct {
			errno_t(*init)(struct at_task_s **task,
						   char *name,
						   void *func,
						   at_task_size_t priority,
						   void *arg_list,
						   void *hook,
						   enum at_task_option_e opt);

			errno_t(*destroy)(struct at_task_s **task);

			errno_t(*suspend)(struct at_task_s *task);

			errno_t(*rusume)(struct at_task_s *task);
		}configuration;

		struct {
			void (*priority)(struct at_task_s *task);
		}inquire;
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

void at_task_control_os_configuration_init(void);

/**
 * @brief This function will destroy the at task os.
 *
 * @param void
 *
 * @return void
 */

void at_task_control_os_configuration_destroy(void);

/**
 * @brief This function will manage the priority of the os.
 *
 * @param void
 *
 * @return void
 */

void at_task_control_os_priority_management(void);

/**
 * @brief This function will schedule the tasks of the os.
 *
 * @param void
 *
 * @return void
 */

void at_task_control_os_scheduler(void);

/**
 * @brief This function will schedule the tasks of the os.
 *
 * @param void
 *
 * @return void
 */

void at_task_control_os_core(void);

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
												enum at_task_option_e opt);

/**
 * @brief This function will destroy the task of at task os.
 *
 * @param void
 *
 * @return void
 */

errno_t at_task_control_task_configuration_destroy(struct at_task_s **task);

/**
 * @brief This function will suspend the task of the at task os.
 *
 * @param void
 *
 * @return void
 */

errno_t at_task_control_task_configuration_suspend(struct at_task_s *task);

/**
 * @brief This function will resume the task of the at task os.
 *
 * @param void
 *
 * @return void
 */

errno_t at_task_control_task_configuration_resume(struct at_task_s *task);

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