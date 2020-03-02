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
 * @brief This struct is the body of at message control struct.
 */

struct at_message_control_s {
	struct {
		errno_t(*init)(void);

		errno_t(*destroy)(void);
	}configuration;

	struct {
		errno_t(*deposit)(at_size_t cnt,
						  void *msg, at_size_t len, ...);

        struct at_message_transmit_group_s(*load)(void);
	}transmit;

	struct {
		errno_t(*deposit)(void *msg, at_size_t len);

		void *(*load)(void);
	}feedback;
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

errno_t at_message_control_configuration_init(void);

/**
 * @brief This function will destroy the at message pool.
 *
 * @param void
 *
 * @return void
 */

errno_t at_message_control_configuration_destroy(void);

/**
 * @brief This function will deposit the at message into the at message pool for transmit.
 *
 * @param void
 *
 * @return void
 */

errno_t at_message_control_transmit_deposit(at_size_t cnt,
											void *msg, at_size_t len, ...);

/**
 * @brief This function will load the at message from the at message pool for transmit.
 *
 * @param void
 *
 * @return void
 */

struct at_message_transmit_group_s at_message_control_transmit_load(void);

/**
 * @brief This function will deposit the at message into the at message pool for feedback.
 *
 * @param void
 *
 * @return void
 */

errno_t at_message_control_feedback_deposit(void *msg,
											at_size_t len);

/**
 * @brief This function will load the at message from the at message pool for feedback.
 *
 * @param void
 *
 * @return void
 */

void *at_message_control_feedback_load(void);

/*
*********************************************************************************************************
*                                       EXTERN GLOBAL VARIABLES
*********************************************************************************************************
*/

/**
 * @brief This struct is the body of at message control struct.
 */

extern struct at_message_control_s at_message_ctrl;

/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif // !__AT_MESSAGE_H