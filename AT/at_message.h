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
 * @brief This type is the at message queue exchange function typedef.
 */

typedef errno_t(*at_message_queue_exchange_ftp)(struct at_message_queue_s *message_queue,
												struct at_message_queue_message_package_s *message_package);

/**
 * @brief This type is the at message structure.
 */

typedef struct at_message_queue_s
*at_message_queue_stp,
**at_message_queue_stpp;

/**
 * @brief This struct is the body of at message control struct.
 */

struct at_message_pool_control_s {
	struct {
		errno_t(*init)(struct at_message_pool_s **message);

		errno_t(*destroy)(struct at_message_pool_s **message);
	}configuration;

	struct {
		void *(*transmit)(struct at_message_pool_s *message);

		void *(*feedback)(struct at_message_pool_s *message);
	}element_access;

	struct {
		errno_t(*deposit)(struct at_message_pool_s *message,
						  at_size_t count,
						  struct at_message_transmit_unit_s *unit,
						  ...);

		struct at_message_transmit_group_s(*load)(struct at_message_pool_s *message);
	}transmit;

	struct {
		errno_t(*deposit)(struct at_message_pool_s *message,
						  void *str, at_size_t len);

		void *(*load)(struct at_message_pool_s *message);
	}feedback;
};

/**
 * @brief This struct is the body of at message queue control struct.
 */

struct at_message_queue_control_s {
	struct {
		errno_t(*init)(struct at_message_queue_s **message_queue,
					   at_message_queue_exchange_ftp exchange,
					   struct at_data_structure_control_package_s *manager_control,
					   struct at_data_structure_control_package_s *queue_control);

		errno_t(*destroy)(struct at_message_queue_s **message_queue);
	}configuration;

	struct {
		at_size_t(*join)(struct at_message_queue_s *message_queue);

		errno_t(*quit)(struct at_message_queue_s *message_queue,
					   at_size_t who_are_you);
	}membership;

	struct {
		errno_t(*publish)(struct at_message_queue_s *message_queue,
						  void *message,
						  at_size_t subscriber,
						  at_size_t publisher);

		struct at_message_queue_message_package_s
		(*subscribe)(struct at_message_queue_s *message_queue,
					 at_size_t who_am_i);
	}communication;
};

/**
 * @brief This struct will contain all the at task control functions.
 */

struct at_message_queue_unit_s {
	at_size_t id;

	at_message_queue_stp mq_ptr;
};

/**
 * @brief This type is the at message queue message package structure.
 */

struct at_message_queue_queue_package_s {
	at_size_t id;

	void *queue;
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
 * @brief This function will initialize the at message pool.
 *
 * @param void
 *
 * @return void
 */

errno_t at_message_pool_control_configuration_init(struct at_message_pool_s **message);

/**
 * @brief This function will destroy the at message pool.
 *
 * @param void
 *
 * @return void
 */

errno_t at_message_pool_control_configuration_destroy(struct at_message_pool_s **message);

/**
 * @brief This function will deposit the at message into the at message pool for transmit.
 *
 * @param void
 *
 * @return void
 */

errno_t at_message_pool_control_transmit_deposit(struct at_message_pool_s *message,
												 at_size_t count,
												 struct at_message_transmit_unit_s *unit,
												 ...);

/**
 * @brief This function will load the at message from the at message pool for transmit.
 *
 * @param void
 *
 * @return void
 */

struct at_message_transmit_group_s at_message_pool_control_transmit_load(struct at_message_pool_s *message);

/**
 * @brief This function will deposit the at message into the at message pool for feedback.
 *
 * @param void
 *
 * @return void
 */

errno_t at_message_pool_control_feedback_deposit(struct at_message_pool_s *message,
												 void *str,
												 at_size_t len);

/**
 * @brief This function will load the at message from the at message pool for feedback.
 *
 * @param void
 *
 * @return void
 */

void *at_message_pool_control_feedback_load(struct at_message_pool_s *message);

/**
 * @brief This function will publish a message into the message queue pool.
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
 * @brief This function will publish a message into the message queue pool.
 *
 * @param void
 *
 * @return void
 */

errno_t at_message_queue_control_configuration_destroy(struct at_message_queue_s **message_queue);

/**
 * @brief This function will make a queue for the caller,then join it into the message queue pool.
 *
 * @param void
 *
 * @return void
 */

at_size_t at_message_queue_control_membership_join(struct at_message_queue_s *message_queue);

/**
 * @brief This function will quit from message queue pool.
 *
 * @param void
 *
 * @return void
 */

errno_t at_message_queue_control_membership_quit(struct at_message_queue_s *message_queue,
												 at_size_t who_are_you);

/**
 * @brief This function will publish a message into the message queue pool.
 *
 * @param void
 *
 * @return void
 */

errno_t at_message_queue_control_communication_publish(struct at_message_queue_s *message_queue,
													   void *message,
													   at_size_t subscriber,
													   at_size_t publisher);

/**
 * @brief This function will subscribe a message from the message queue pool.
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
 * @brief This struct is the body of at message control struct.
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