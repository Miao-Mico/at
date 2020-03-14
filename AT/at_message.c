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
 * @brief This type is the at message structure.
 */

struct at_message_pool_s {
	void *transmit_queue;

	void *feedback_stack;
};

/**
 * @brief This type is the at message structure.
 */

struct at_message_queue_s {
	struct {
		at_size_t cnt_routers;
		at_size_t seed_who_am_i;
	}info;

	struct at_data_structure_package_s queue_manager_package;

	struct at_data_structure_control_package_s *queue_control_ptr;

	at_message_queue_exchange_ftp exchange_ptr;
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

struct at_data_structure_control_package_s *at_message_transmit_list_queue_package =
	&AT_MESSAGE_CFG_USER_DEFINED_TRANSMIT_MESSAGE_POOL_DATA_STRUCTURE;

/**
 * @brief This struct will contain all the at task list stack control functions.
 */

struct at_data_structure_control_package_s *at_message_feedback_list_stack_package =
	&AT_MESSAGE_CFG_USER_DEFINED_FEEDBACK_MESSAGE_POOL_DATA_STRUCTURE;

/**
 * @brief This struct is the body of at message control struct.
 */

struct at_message_pool_control_s at_message_pool_ctrl = {
	.configuration.init = at_message_pool_control_configuration_init,
	.configuration.destroy = at_message_pool_control_configuration_destroy,

	.transmit.deposit = at_message_pool_control_transmit_deposit,
	.transmit.load = at_message_pool_control_transmit_load,
	.feedback.deposit = at_message_pool_control_feedback_deposit,
	.feedback.load = at_message_pool_control_feedback_load,
};

/**
 * @brief This struct is the body of at message queue control struct.
 */

struct at_message_queue_control_s at_message_queue_ctrl = {
	.configuration.init = at_message_queue_control_configuration_init,
	.configuration.destroy = at_message_queue_control_configuration_destroy,

	.membership.join = at_message_queue_control_membership_join,
	.membership.quit = at_message_queue_control_membership_quit,

	.communication.publish = at_message_queue_control_communication_publish,
	.communication.subscribe = at_message_queue_control_communication_subscribe
};

/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

/**
 * @brief This function will publish a message into the message queue pool.
 *
 * @param void
 *
 * @return void
 */

errno_t at_message_queue_control_exchange(struct at_message_queue_s *message_queue,
										  struct at_message_queue_message_package_s *message_package);

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

errno_t at_message_pool_control_configuration_init(struct at_message_pool_s **message)
{
	assert(message);

	if (NULL == ((*message) = calloc(1, sizeof(struct at_message_pool_s)))) {
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

errno_t at_message_pool_control_configuration_destroy(struct at_message_pool_s **message)
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

errno_t at_message_pool_control_transmit_deposit(struct at_message_pool_s *message,
												 at_size_t count,
												 struct at_message_transmit_unit_s *unit,
												 ...)
{
	assert(AT_CFG_TRANSMIT_LEVEL_MAX >= count);

	struct at_message_transmit_group_s *msg_grp = NULL;

	if (NULL == (msg_grp = calloc(1, sizeof(struct at_message_transmit_group_s)))) {		/* Allocate the message group */
		return -1;
	}

	msg_grp->count = count;

	va_list va_ptr;

	at_size_t cnt = 0;

	va_start(va_ptr, unit);

	do {
		if (NULL == memcpy(&msg_grp->units[cnt++], 											/* Copy the string list and the length list */
						   unit,
						   sizeof(struct at_message_transmit_unit_s))) {
			return 1;
		}
	} while (cnt < count
			 && (unit = va_arg(va_ptr, struct at_message_transmit_unit_s *)));

	va_end(va_ptr);

	if (at_message_transmit_list_queue_package->modifiers.
		insert(message->transmit_queue, msg_grp)) {											/* Push the str group into the queue */
		return 3;
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

struct at_message_transmit_group_s at_message_pool_control_transmit_load(struct at_message_pool_s *message)
{
	struct at_message_transmit_group_s
		msg_grp = { 0 },
		*msg_grp_ptr = NULL;

	if (NULL == (msg_grp_ptr = at_message_transmit_list_queue_package->element_access
				 .at(message->transmit_queue))) {
		return (struct at_message_transmit_group_s) { 0 };
	}

	msg_grp = *msg_grp_ptr;

	if (at_message_transmit_list_queue_package->modifiers.
		delete(message->transmit_queue)) {
		return (struct at_message_transmit_group_s) { 0 };
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

errno_t at_message_pool_control_feedback_deposit(struct at_message_pool_s *message,
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
		insert(message->feedback_stack, msg_cpy)) {
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

void *at_message_pool_control_feedback_load(struct at_message_pool_s *message)
{
	void *str = NULL;

	if (NULL == (str = at_message_feedback_list_stack_package->element_access
				 .at(message->feedback_stack))) {
		return NULL;
	}

	if (at_message_feedback_list_stack_package->modifiers.
		delete(message->feedback_stack)) {
		return NULL;
	}

	return str;
}

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
													struct at_data_structure_control_package_s *queue_control)
{
	assert(message_queue);

	#ifdef __AT_DATA_STRUCTURE_TEMPLATE_H

	if (NULL == manager_control) {
		manager_control = &at_red_black_tree_control_package;
	}

	if (NULL == queue_control) {
		queue_control = &at_list_queue_control_package;
	}

	#else

	assert(manager_control);
	assert(queue_control);

	#endif // __AT_DATA_STRUCTURE_TEMPLATE_H

	if (NULL == ((*message_queue) = calloc(1, sizeof(struct at_message_queue_s)))) {
		return -1;
	}

	if (manager_control->configuration
		.init(&(*message_queue)->queue_manager_package.data_structure_ptr)) {
		return 1;
	}

	if (NULL == exchange) {
		exchange = at_message_queue_control_exchange;
	}

	(*message_queue)->exchange_ptr = exchange;
	(*message_queue)->queue_manager_package.control_ptr = manager_control;
	(*message_queue)->queue_control_ptr = queue_control;
	(*message_queue)->exchange_ptr = exchange;

	return 0;
}

/**
 * @brief This function will publish a message into the message queue pool.
 *
 * @param void
 *
 * @return void
 */

errno_t at_message_queue_control_configuration_destroy(struct at_message_queue_s **message_queue)
{
	assert(message_queue);
	assert(*message_queue);

	free((*message_queue));

	(*message_queue) = NULL;

	return 0;
}

/**
 * @brief This function will publish a message into the message queue pool.
 *
 * @param void
 *
 * @return void
 */

errno_t at_message_queue_control_exchange(struct at_message_queue_s *message_queue,
										  struct at_message_queue_message_package_s *message_package)
{
	assert(message_queue);

	void *queue = NULL;

	if (NULL == (queue = message_queue->queue_manager_package.control_ptr->lookup
				 .search(message_queue->queue_manager_package.data_structure_ptr,
						 message_package->subscriber))) {
		return 1;
	}

	if (message_queue->queue_control_ptr->modifiers
		.insert(queue,
				message_package)) {
		return 2;
	}

	return 0;
}

/**
 * @brief This function will make a queue for the caller,then join it into the message queue pool.
 *
 * @param void
 *
 * @return void
 */

at_size_t at_message_queue_control_membership_join(struct at_message_queue_s *message_queue)
{
	assert(message_queue);

	struct at_message_queue_queue_package_s message_package = { 0 };

	if (message_queue->queue_control_ptr->configuration										/* Initialize the queue */
		.init(&message_package.queue)) {
		return 1;
	}

	message_package.id = ++message_queue->info.seed_who_am_i;

	printf("at message queue.membership.join.queue %d :%p\r\n", message_package.id, message_package.queue);

	if (message_queue->queue_manager_package.control_ptr->modifiers							/* Push the queue into the manage pool */
		.insert(message_queue->queue_manager_package.data_structure_ptr,
				&message_package)) {
		return 2;
	}

	message_queue->info.cnt_routers++;														/* Increase the router count */

	return message_package.id;
}

/**
 * @brief This function will quit from message queue pool.
 *
 * @param void
 *
 * @return void
 */

errno_t at_message_queue_control_membership_quit(struct at_message_queue_s *message_queue,
												 at_size_t who_are_you)
{
	assert(message_queue);

	void *queue = NULL;

	if (NULL == (queue = message_queue->queue_manager_package.control_ptr->lookup
				 .search(message_queue->queue_manager_package.data_structure_ptr,			/* Search the queue via the key that who are you */
						 who_are_you))) {
		return 1;
	}

	if (message_queue->queue_manager_package.control_ptr->modifiers							/* Push the queue into the manage pool */
		.delete(message_queue->queue_manager_package.data_structure_ptr,
				queue, 0)) {
		return 2;
	}

	if (message_queue->queue_control_ptr->configuration										/* Destroy the queue */
		.destroy(&queue)) {
		return 3;
	}

	return 0;
}

/**
 * @brief This function will publish a message into the message queue pool.
 *
 * @param void
 *
 * @return void
 */

extern inline errno_t
at_message_queue_control_communication_publish(struct at_message_queue_s *message_queue,
											   void *message,
											   at_size_t subscriber,
											   at_size_t publisher)
{
	assert(message_queue);
	assert(message);

	struct at_message_queue_message_package_s
		*message_package_ptr = NULL;

	if (NULL == (message_package_ptr = calloc(1, sizeof(struct at_message_queue_message_package_s)))) {
		return -1;
	}

	message_package_ptr->message = message;
	message_package_ptr->publisher = publisher;
	message_package_ptr->subscriber = subscriber;

	return message_queue->exchange_ptr(message_queue,										/* Exchange the message into the queue */
									   message_package_ptr);
}

/**
 * @brief This function will subscribe a message from the message queue pool.
 *
 * @param void
 *
 * @return void
 */

struct at_message_queue_message_package_s
	at_message_queue_control_communication_subscribe(struct at_message_queue_s *message_queue,
													 at_size_t who_am_i)
{
	assert(message_queue);
	assert(0 <= who_am_i);

	void *queue = NULL;
	struct at_message_queue_message_package_s
		*message_package_ptr = NULL;

	static struct at_message_queue_message_package_s
		message_package = { 0 };

	if (NULL == (queue = message_queue->queue_manager_package.control_ptr->lookup
				 .search(message_queue->queue_manager_package.data_structure_ptr,
						 who_am_i))) {
		return (struct at_message_queue_message_package_s) { NULL };
	}

	if (NULL == (message_package_ptr = message_queue->queue_control_ptr->element_access
				 .at(queue))) {																/* Access the message element in the queue */
		return (struct at_message_queue_message_package_s) { NULL };
	}

	if (NULL == memcpy(&message_package, message_package_ptr, sizeof(struct at_message_queue_message_package_s))) {
		return (struct at_message_queue_message_package_s) { NULL };
	}

	if (message_queue->queue_control_ptr->modifiers
		.delete(queue)) {
		return (struct at_message_queue_message_package_s) { NULL };
	}

	return message_package;
}