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
		errno_t(*init)(struct at_message_s **message);

		errno_t(*destroy)(struct at_message_s **message);
	}configuration;

    struct {
		void *(*transmit)(struct at_message_s *message);

		void *(*feedback)(struct at_message_s *message);
    }element_access;

	struct {
		errno_t(*deposit)(struct at_message_s *message,
                          at_size_t cnt,
						  void *str, at_size_t len, ...);

        struct at_message_transmit_group_s(*load)(struct at_message_s *message);
	}transmit;

	struct {
		errno_t(*deposit)(struct at_message_s *message,
                          void *str, at_size_t len);

		void *(*load)(struct at_message_s *message);
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

errno_t at_message_control_configuration_init(struct at_message_s **message);

/**
 * @brief This function will destroy the at message pool.
 *
 * @param void
 *
 * @return void
 */

errno_t at_message_control_configuration_destroy(struct at_message_s **message);

/**
 * @brief This function will deposit the at message into the at message pool for transmit.
 *
 * @param void
 *
 * @return void
 */

errno_t at_message_control_transmit_deposit(struct at_message_s *message,
                                            at_size_t cnt,
											void *str, at_size_t len, ...);

/**
 * @brief This function will load the at message from the at message pool for transmit.
 *
 * @param void
 *
 * @return void
 */

struct at_message_transmit_group_s at_message_control_transmit_load(struct at_message_s *message);

/**
 * @brief This function will deposit the at message into the at message pool for feedback.
 *
 * @param void
 *
 * @return void
 */

errno_t at_message_control_feedback_deposit(struct at_message_s *message,
                                            void *str,
											at_size_t len);

/**
 * @brief This function will load the at message from the at message pool for feedback.
 *
 * @param void
 *
 * @return void
 */

void *at_message_control_feedback_load(struct at_message_s *message);

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