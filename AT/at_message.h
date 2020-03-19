/*
*********************************************************************************************************
*                                               MODULE
*
* Note(s) : (1) This definition header file is protected from multiple pre-processor inclusion
*               through use of the definition module present pre-processor macro definition.
*********************************************************************************************************
*/

#ifndef __AT_MESSAGE_H
#define __AT_MESSAGE_H

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
 * @brief This type is the message queue exchange function typedef.
 */

typedef errno_t(*at_message_queue_exchange_ftp)(struct at_message_queue_s *message_queue,
												struct at_message_queue_message_package_s *message_package);

/**
 * @brief This type is the at message queue structure typedef.
 */

typedef struct at_message_queue_s
*at_message_queue_stp,
**at_message_queue_stpp;

/**
 * @brief This type is the message pool control structure.
 */

struct at_message_pool_control_s {
	struct {
		/* @brief This function will initialize the message pool.                                       */
		errno_t(*init)(struct at_message_pool_s **message_pool,
					   struct at_data_structure_control_package_s *transmit_control,
					   struct at_data_structure_control_package_s *feedback_control);

		/* @brief This function will destroy the message pool.                                          */
		errno_t(*destroy)(struct at_message_pool_s **message);
	}configuration;

	struct {
		void *(*transmit)(struct at_message_pool_s *message);

		void *(*feedback)(struct at_message_pool_s *message);
	}element_access;

	struct {
		/* @brief This function will deposit the message into the at message pool for transmit.         */
		errno_t(*deposit)(struct at_message_pool_s *message,
						  at_size_t count,
						  struct at_message_transmit_unit_s *unit,
						  ...);

		/* @brief This function will load the message from the at message pool for transmit.            */
		struct at_message_transmit_group_s(*load)(struct at_message_pool_s *message);
	}transmit;

	struct {
		/* @brief This function will deposit the message into the at message pool for feedback.         */
		errno_t(*deposit)(struct at_message_pool_s *message,
						  void *str, at_size_t len);

		/* @brief This function will load the message from the at message pool for feedback.            */
		void *(*load)(struct at_message_pool_s *message);
	}feedback;
};

/**
 * @brief This type is the message queue control structure.
 */

struct at_message_queue_control_s {
	struct {
		/* @brief This function will initialize the message queue.                                      */
		errno_t(*init)(struct at_message_queue_s **message_queue,
					   at_message_queue_exchange_ftp exchange,
					   struct at_data_structure_control_package_s *manager_control,
					   struct at_data_structure_control_package_s *queue_control);

		/* @brief This function will initialize the message queue.                                      */
		errno_t(*destroy)(struct at_message_queue_s **message_queue);
	}configuration;

	struct {
		/* @brief This function will join the caller into the message queue.                            */
		at_size_t(*join)(struct at_message_queue_s *message_queue);

		/* @brief This function will quit the caller from the message queue.                            */
		errno_t(*quit)(struct at_message_queue_s *message_queue,
					   at_size_t who_are_you);
	}membership;

	struct {
		/* @brief This function will publish a message into the message queue.                          */
		errno_t(*publish)(struct at_message_queue_s *message_queue,
						  void *message,
						  at_size_t subscriber,
						  at_size_t publisher);

		/* @brief This function will subscribe a message into the message queue.                        */
		struct at_message_queue_message_package_s
		(*subscribe)(struct at_message_queue_s *message_queue,
					 at_size_t who_am_i);
	}communication;
};

/**
 * @brief This type is the message queue unit structure.
 */

struct at_message_queue_unit_s {
	at_size_t id;

	at_message_queue_stp mq_ptr;
};

/**
 * @brief This type is the at message queue message package structure.
 */

struct at_message_queue_message_package_s {
	void *message;

	at_size_t publisher;
	at_size_t subscriber;
};

/*
*********************************************************************************************************
*								            FUNCTION PROTOTYPES
*********************************************************************************************************
*/

/**
 * @brief This function will initialize the message pool.
 *
 * @param void
 *
 * @return void
 */

errno_t at_message_pool_control_configuration_init(struct at_message_pool_s **message_pool,
												   struct at_data_structure_control_package_s *transmit_control,
												   struct at_data_structure_control_package_s *feedback_control);

/**
 * @brief This function will destroy the message pool.
 *
 * @param void
 *
 * @return void
 */

errno_t at_message_pool_control_configuration_destroy(struct at_message_pool_s **message_pool);

/**
 * @brief This function will deposit the message into the at message pool for transmit.
 *
 * @param void
 *
 * @return void
 */

errno_t at_message_pool_control_transmit_deposit(struct at_message_pool_s *message_pool,
												 at_size_t count,
												 struct at_message_transmit_unit_s *unit,
												 ...);

/**
 * @brief This function will load the message from the at message pool for transmit.
 *
 * @param void
 *
 * @return void
 */

struct at_message_transmit_group_s
	at_message_pool_control_transmit_load(struct at_message_pool_s *message_pool);

/**
 * @brief This function will deposit the message into the at message pool for feedback.
 *
 * @param void
 *
 * @return void
 */

errno_t at_message_pool_control_feedback_deposit(struct at_message_pool_s *message_pool,
												 void *str, at_size_t len);

/**
 * @brief This function will load the message from the at message pool for feedback.
 *
 * @param void
 *
 * @return void
 */

void *at_message_pool_control_feedback_load(struct at_message_pool_s *message_pool);

/**
 * @brief This function will initialize the message queue.
 *
 * @param void
 *
 * @return void
 */

errno_t at_message_queue_control_configuration_init(struct at_message_queue_s **message_queue,
													at_message_queue_exchange_ftp exchange,
													struct at_data_structure_control_package_s *manager_control,
													struct at_data_structure_control_package_s *queue_control);

/**
 * @brief This function will destroy the message queue.
 *
 * @param void
 *
 * @return void
 */

errno_t at_message_queue_control_configuration_destroy(struct at_message_queue_s **message_queue);

/**
 * @brief This function will let the caller join into the message queue.
 *
 * @param void
 *
 * @return void
 */

at_size_t at_message_queue_control_membership_join(struct at_message_queue_s *message_queue);

/**
 * @brief This function will let the caller quit into the message queue.
 *
 * @param void
 *
 * @return void
 */

errno_t at_message_queue_control_membership_quit(struct at_message_queue_s *message_queue,
												 at_size_t who_are_you);

/**
 * @brief This function will publish a message into the message queue.
 *
 * @param void
 *
 * @return void
 */

errno_t
at_message_queue_control_communication_publish(struct at_message_queue_s *message_queue,
											   void *message,
											   at_size_t subscriber,
											   at_size_t publisher);

/**
 * @brief This function will subscribe a message from the message queue.
 *
 * @param void
 *
 * @return void
 */

struct at_message_queue_message_package_s
	at_message_queue_control_communication_subscribe(struct at_message_queue_s *message_queue,
													 at_size_t who_am_i);

/*
*********************************************************************************************************
*                                       EXTERN GLOBAL VARIABLES
*********************************************************************************************************
*/

/**
 * @brief This struct is the body of at message pool control struct.
 */

extern struct at_message_pool_control_s at_message_pool_ctrl;

/**
 * @brief This struct is the body of at message queue control struct.
 */

extern struct at_message_queue_control_s at_message_queue_ctrl;

/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif // !__AT_MESSAGE_H