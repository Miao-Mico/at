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

errno_t at_message_control_configuration_init(struct at_message_s **message)
{
	assert(message);

	if (NULL == ((*message) = calloc(1, sizeof(struct at_message_s)))) {
		return -1;
	}

	if (at_message_transmit_list_queue_package->configuration.
		init(&(*message)->transmit_queue)) {
		return 1;
	}

	if (at_message_feedback_list_stack_package->configuration.
		init(&(*message)->feedback_stack)) {
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

errno_t at_message_control_configuration_destroy(struct at_message_s **message)
{
	assert(message);
	assert(*message);

	if (at_message_transmit_list_queue_package->configuration.
		destroy(&(*message)->transmit_queue)) {
		return 1;
	}

	if (at_message_feedback_list_stack_package->configuration.
		destroy(&(*message)->feedback_stack)) {
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

errno_t at_message_control_transmit_deposit(struct at_message_s *message,
											at_size_t cnt, void *str, at_size_t len, ...)
{
	assert(AT_CFG_TRANSMIT_LEVEL_MAX >= cnt);

	struct at_message_transmit_group_s *msg_grp = NULL;

	if (NULL == (msg_grp = calloc(1, sizeof(struct at_message_transmit_group_s)))) {		/* Allocate the message group */
		return -1;
	}

	msg_grp->count = cnt;

	va_list va_ptr;
	char *msg_cpy = NULL;
	at_size_t ct = 0;

	va_start(va_ptr, len);

	do {
		if (NULL == (msg_cpy = calloc(1, len + 1))) {										/* Retain a bit for '\0' */
			return -2;
		}
		if (!memcpy(msg_cpy, str, len)) {
			return 1;
		}

		msg_grp->pool[ct++] = msg_cpy;
	} while (ct < cnt &&
		(str = va_arg(va_ptr, void *),
		 len = va_arg(va_ptr, size_t)));

	va_end(va_ptr);

	if (at_message_transmit_list_queue_package->modifiers.
		push(message->transmit_queue, msg_grp)) {											/* Push the str group into the queue */
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

struct at_message_transmit_group_s at_message_control_transmit_load(struct at_message_s *message)
{
	struct at_message_transmit_group_s
		msg_grp = { 0 },
		*msg_grp_ptr = NULL;

	if (NULL == (msg_grp_ptr = at_message_transmit_list_queue_package->element_access.
				 top(message->transmit_queue))) {
		return (struct at_message_transmit_group_s) { 0, NULL, NULL, NULL };
	}

	msg_grp = *msg_grp_ptr;

	if (at_message_transmit_list_queue_package->modifiers.
		pop(message->transmit_queue)) {
		return (struct at_message_transmit_group_s) { 0, NULL, NULL, NULL };
	}

	return msg_grp;
}

/**
 * @brief This function will deposit the at message into the at message pool for feedback.
 *
 * @param void
 *
 * @return void
 */

errno_t at_message_control_feedback_deposit(struct at_message_s *message,
											void *str, at_size_t len)
{
	void *msg_cpy = NULL;

	if (NULL == (msg_cpy = calloc(1, len))) {
		return -1;
	}

	if (!memcpy(msg_cpy, str, len)) {
		return 1;
	}

	if (at_message_feedback_list_stack_package->modifiers.
		push(message->feedback_stack, msg_cpy)) {
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

void *at_message_control_feedback_load(struct at_message_s *message)
{
	void *str = NULL;

	if (NULL == (str = at_message_feedback_list_stack_package->element_access.
				 top(message->feedback_stack))) {
		return NULL;
	}

	if (at_message_feedback_list_stack_package->modifiers.
		pop(message->feedback_stack)) {
		return NULL;
	}

	return str;
}