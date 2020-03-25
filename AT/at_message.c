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
 * @brief This type is the at message pool structure.
 */

struct at_message_pool_s {
	struct at_data_structure_package_s transmit;

	struct at_data_structure_package_s feedback;
};

/**
 * @brief This type is the at message queue structure.
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

/**
 * @brief This type is the message queue message package structure.
 */

struct at_message_queue_queue_package_s {
	at_size_t id;

	void *queue;
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
 * @brief This struct is the body of at message pool control struct.
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
 * @brief This function will exchange messages into the message queues.
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
 * @brief This function will initialize the message pool.
 *
 * @param void
 *
 * @return void
 */

errno_t at_message_pool_control_configuration_init(struct at_message_pool_s **message_pool,
												   struct at_data_structure_control_package_s *transmit_control,
												   struct at_data_structure_control_package_s *feedback_control)
{
	assert(message_pool);

	#ifdef __AT_DATA_STRUCTURE_TEMPLATE_H													/* If contain the file */

	if (NULL == transmit_control) {
		transmit_control = &at_list_queue_control_package;
	}

	if (NULL == feedback_control) {
		feedback_control = &at_list_stack_control_package;
	}

	#else

	assert(transmit_control);
	assert(feedback_control);

	#endif // __AT_DATA_STRUCTURE_TEMPLATE_H

	if (NULL == ((*message_pool) = calloc(1, sizeof(struct at_message_pool_s)))) {			/* Allocate the message pool body */
		return -1;
	}

	if (transmit_control->configuration
		.init(&(*message_pool)->transmit.data_structure_ptr)) {								/* Initialize the transmit part of the message pool */
		return 1;
	}

	if (feedback_control->configuration
		.init(&(*message_pool)->feedback.data_structure_ptr)) {								/* Initialize the feedback part of the message pool */
		return 2;
	}

	(*message_pool)->transmit.control_ptr = transmit_control;
	(*message_pool)->feedback.control_ptr = feedback_control;

	return 0;
}

/**
 * @brief This function will destroy the message pool.
 *
 * @param void
 *
 * @return void
 */

errno_t at_message_pool_control_configuration_destroy(struct at_message_pool_s **message_pool)
{
	assert(message_pool);
	assert(*message_pool);

	if ((*message_pool)->transmit.control_ptr->configuration
		.destroy(&(*message_pool)->transmit.data_structure_ptr)) {
		return 1;
	}

	if ((*message_pool)->feedback.control_ptr->configuration
		.destroy(&(*message_pool)->feedback.data_structure_ptr)) {
		return 2;
	}

	return 0;
}

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
												 ...)
{
	assert(AT_CFG_TRANSMIT_LEVEL_MAX >= count);

	struct at_message_transmit_group_s *msg_grp = NULL;

	if (NULL == (msg_grp = calloc(1, sizeof(struct at_message_transmit_group_s)))) {		/* Allocate the message group */
		return -1;
	}

	msg_grp->count = count;

	va_list va_ptr;

	va_start(va_ptr, unit);

	at_size_t cnt = 0;
	do {
		if (NULL == memcpy(&msg_grp->units[cnt++], 											/* Copy the string list and the length list */
						   unit,
						   sizeof(struct at_message_transmit_unit_s))) {
			return 1;
		}
	} while (cnt < count
			 && (unit = va_arg(va_ptr, struct at_message_transmit_unit_s *)));

	va_end(va_ptr);

	if (message_pool->transmit.control_ptr->modifiers
		.insert(message_pool->transmit.data_structure_ptr, msg_grp)) {						/* Insert the address of the message group into the queue */
		return 3;
	}

	return 0;
}

/**
 * @brief This function will load the message from the at message pool for transmit.
 *
 * @param void
 *
 * @return void
 */

struct at_message_transmit_group_s
	at_message_pool_control_transmit_load(struct at_message_pool_s *message_pool)
{
	struct at_message_transmit_group_s
		msg_grp = { 0 },
		*msg_grp_ptr = NULL;

	if (NULL == (msg_grp_ptr
				 = message_pool->transmit.control_ptr->element_access
				 .at(message_pool->transmit.data_structure_ptr))) {							/* Access the message from the message pool */
		return (struct at_message_transmit_group_s) { 0 };
	}

	msg_grp = *msg_grp_ptr;

	if (message_pool->transmit.control_ptr->modifiers
		.delete(message_pool->transmit.data_structure_ptr)) {								/* Delete the message from the message pool */
		return (struct at_message_transmit_group_s) { 0 };
	}

	return msg_grp;
}

/**
 * @brief This function will deposit the message into the at message pool for feedback.
 *
 * @param void
 *
 * @return void
 */

errno_t at_message_pool_control_feedback_deposit(struct at_message_pool_s *message_pool,
												 void *str, at_size_t len)
{
	void *msg_cpy = NULL;

	if (NULL == (msg_cpy = calloc(1, len))) {
		return -1;
	}

	if (!memcpy(msg_cpy, str, len)) {														/* Copy the message */
		return 1;
	}

	if (message_pool->feedback.control_ptr->modifiers
		.insert(message_pool->feedback.data_structure_ptr, msg_cpy)) {						/* Insert the address of message into the message pool,
																								it will have no limit on the length of the message */
		return 2;
	}

	return 0;
}

/**
 * @brief This function will load the message from the at message pool for feedback.
 *
 * @param void
 *
 * @return void
 */

void *at_message_pool_control_feedback_load(struct at_message_pool_s *message_pool)
{
	void *str = NULL;

	if (NULL == (str
				 = message_pool->feedback.control_ptr->element_access
				 .at(message_pool->feedback.data_structure_ptr))) {							/* Access the message from the message pool */
		return NULL;
	}

	if (message_pool->feedback.control_ptr->modifiers
		.delete(message_pool->feedback.data_structure_ptr)) {								/* Delete the message from the message pool */
		return NULL;
	}

	return str;
}

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

	if (NULL == ((*message_queue)
				 = calloc(1, sizeof(struct at_message_queue_s)))) {							/* Allocate the message queue */
		return -1;
	}

	if (manager_control->configuration
		.init(&(*message_queue)->queue_manager_package.data_structure_ptr)) {				/* Initialize the queue manager part of the message queue */
		return 1;
	}

	if (NULL == exchange) {
		exchange = at_message_queue_control_exchange;										/* Default the exchange function of the message queue */
	}

	(*message_queue)->exchange_ptr = exchange;
	(*message_queue)->queue_manager_package.control_ptr = manager_control;
	(*message_queue)->queue_control_ptr = queue_control;
	(*message_queue)->exchange_ptr = exchange;

	return 0;
}

/**
 * @brief This function will destroy the message queue.
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
 * @brief This function will exchange messages into the message queue.
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
 * @brief This function will let the caller join into the message queue.
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
 * @brief This function will let the caller quit into the message queue.
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

	if (message_queue->queue_manager_package.control_ptr->modifiers		
		.delete(message_queue->queue_manager_package.data_structure_ptr,					/* Delete the queue from the manager data structure */
				NULL, who_are_you)) {
		return 2;
	}

	if (message_queue->queue_control_ptr->configuration										/* Destroy the queue */
		.destroy(&queue)) {
		return 3;
	}

	return 0;
}

/**
 * @brief This function will publish a message into the message queue.
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
 * @brief This function will subscribe a message from the message queue.
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
				 .search(message_queue->queue_manager_package.data_structure_ptr,			/* Search and get the queue the subscriber have */
						 who_am_i))) {
		return (struct at_message_queue_message_package_s) { NULL };
	}

	if (NULL == (message_package_ptr = message_queue->queue_control_ptr->element_access
				 .at(queue))) {																/* Access the message element in the queue */
		return (struct at_message_queue_message_package_s) { NULL };
	}

	if (NULL == memcpy(&message_package, message_package_ptr,
					   sizeof(struct at_message_queue_message_package_s))) {
		return (struct at_message_queue_message_package_s) { NULL };
	}

	if (message_queue->queue_control_ptr->modifiers
		.delete(queue)) {
		return (struct at_message_queue_message_package_s) { NULL };
	}

	return message_package;
}