/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include "at_message.h"

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

/**
 * @brief This struct is the body of at message.
 */

struct at_message_s {
	void *transmit_queue;

	void *feedback_stack;
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
 * @brief This struct is the body of at message.
 */

struct at_message_s at_message;

/**
 * @brief This struct will contain all the at task list stack control functions.
 */

struct at_data_structure_package_s *at_message_transmit_list_queue_package =
	&AT_MESSAGE_CFG_USER_DEFINED_TRANSMIT_MESSAGE_POOL_DATA_STRUCTURE;

/**
 * @brief This struct will contain all the at task list stack control functions.
 */

struct at_data_structure_package_s *at_message_feedback_list_stack_package =
	&AT_MESSAGE_CFG_USER_DEFINED_FEEDBACK_MESSAGE_POOL_DATA_STRUCTURE;

/**
 * @brief This struct is the body of at message control struct.
 */

struct at_message_control_s at_message_ctrl = {
	.configuration.init = at_message_control_configuration_init,
	.configuration.destroy = at_message_control_configuration_destroy,

	.transmit.deposit = at_message_control_transmit_deposit,
	.transmit.load = at_message_control_transmit_load,
	.feedback.deposit = at_message_control_feedback_deposit,
	.feedback.load = at_message_control_feedback_load,
};

/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*					LOCAL GLOBAL VARIABLES & LOCAL FUNCTION PROTOTYPES INTERSECTION
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            FUNCTIONS
*********************************************************************************************************
*/

/**
 * @brief This function will initialize the at message pool.
 *
 * @param void
 *
 * @return void
 */

errno_t at_message_control_configuration_init(void)
{
	if (at_message_transmit_list_queue_package->configuration.
		init(&at_message.transmit_queue)) {
		return 1;
	}

	if (at_message_feedback_list_stack_package->configuration.
		init(&at_message.feedback_stack)) {
		return 2;
	}

	return 0;
}

/**
 * @brief This function will destroy the at message pool.
 *
 * @param void
 *
 * @return void
 */

errno_t at_message_control_configuration_destroy(void)
{
	if (at_message_transmit_list_queue_package->configuration.
		destroy(&at_message.transmit_queue)) {
		return 1;
	}

	if (at_message_feedback_list_stack_package->configuration.
		destroy(&at_message.feedback_stack)) {
		return 2;
	}

	return 0;
}

/**
 * @brief This function will deposit the at message into the at message pool for transmit.
 *
 * @param void
 *
 * @return void
 */

errno_t at_message_control_transmit_deposit(at_size_t cnt, void *msg, at_size_t len, ...)
{
	assert(AT_CFG_TRANSMIT_LEVEL_MAX >= cnt);

	struct at_message_transmit_group_s *msg_grp = NULL;

	if (NULL == (msg_grp = calloc(1, sizeof(struct at_message_transmit_group_s)))) {		/* Allocate the message group */
		return -1;
	}

	va_list va_ptr;

	va_start(va_ptr, len);

	char *msg_cpy = NULL;

	msg_grp->count = cnt;

	at_size_t ct = 0;

	do {
		if (NULL == (msg_cpy = calloc(1, len))) {
			return -2;
		}
		if (!memcpy(msg_cpy, msg, len)) {
			return 1;
		}

		msg_grp->pool[ct++] = msg_cpy;
	} while (ct < cnt && 
			 (msg = va_arg(va_ptr, void *),
			 len = va_arg(va_ptr, size_t)));
	
	va_end(va_ptr);

	if (at_message_transmit_list_queue_package->modifiers.
		push(at_message.transmit_queue, msg_grp)) {											/* Push the msg group into the queue */
		return 2;
	}

	return 0;
}

/**
 * @brief This function will load the at message from the at message pool for transmit.
 *
 * @param void
 *
 * @return void
 */

struct at_message_transmit_group_s at_message_control_transmit_load(void)
{
	struct at_message_transmit_group_s *msg_grp = NULL;

	if (NULL == (msg_grp = at_message_transmit_list_queue_package->element_access.
				 top(at_message.transmit_queue))) {
		return (struct at_message_transmit_group_s) { 0, NULL, NULL, NULL };
	}

	if (at_message_transmit_list_queue_package->modifiers.
		pop(at_message.transmit_queue)) {
		return (struct at_message_transmit_group_s) { 0, NULL, NULL, NULL };
	}

	return *msg_grp;
}

/**
 * @brief This function will deposit the at message into the at message pool for feedback.
 *
 * @param void
 *
 * @return void
 */

errno_t at_message_control_feedback_deposit(void *msg, at_size_t len)
{
	void *msg_cpy = NULL;

	if (NULL == (msg_cpy = calloc(1, len))) {
		return -1;
	}

	if (!memcpy(msg_cpy, msg, len)) {
		return 1;
	}

	if (at_message_feedback_list_stack_package->modifiers.
		push(at_message.feedback_stack, msg_cpy)) {
		return 2;
	}

	return 0;
}

/**
 * @brief This function will load the at message from the at message pool for feedback.
 *
 * @param void
 *
 * @return void
 */

void *at_message_control_feedback_load(void)
{
	void *msg = NULL;

	if (NULL == (msg = at_message_feedback_list_stack_package->element_access.
				 top(at_message.feedback_stack))) {
		return NULL;
	}

	if (at_message_feedback_list_stack_package->modifiers.
		pop(at_message.feedback_stack)) {
		return NULL;
	}

	return msg;
}