/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include "at.h"

#include "at_device.h"
#include "at_message.h"
#include "at_task.h"

/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

/* Configure    the max of levels allows when the transmit generate.									*/
#define AT_CFG_TRANSMIT_GENERATE_LEVELS_MAX									4u

/* Configure    the max of pieces each level allows when the transmit generate.							*/
#define AT_CFG_TRANSMIT_GENERATE_PIECES_EACH_LEVEL_MAX						4u

/* Configure    the max of pieces each level allows when the transmit generate.							*/
#define AT_CFG_TRANSMIT_GENERATE_PIECES_MAX																\
	(AT_CFG_TRANSMIT_GENERATE_LEVELS_MAX * AT_CFG_TRANSMIT_GENERATE_PIECES_EACH_LEVEL_MAX)

/* Configure    the max of pieces each level allows when the transmit generate.							*/
#define AT_CFG_TRANSMIT_GENERATE_STRING_LENGTH_MAX							100u

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
 * @brief This struct will contain all the universal vector functions address.
 */

struct at_info_s {
	char *feedback_sample_tail;

	errno_t multi_level_transmit_error;
};

/**
 * @brief This struct will contain all the universal vector functions address.
 */

struct at_exception_s {
	void (*error)(void);
};

/**
 * @brief This struct will contain all the universal vector functions address.
 */

struct at_s {
	bool switch_status;

	struct at_info_s info;

	struct at_exception_s exception;

	struct at_message_queue_unit_s message_queue_unit;

	struct at_device_package_s *device_package_ptr;

	struct at_task_os_s *task_os_ptr;

	struct at_message_pool_s *message_pool_ptr;
};

/**
 * @brief This struct will contain all the information from the format analysis.
 */

struct at_transmit_format_analysis_package_s {
	at_size_t level;
	at_size_t total_count;
	at_size_t *info;
	at_size_t *info_answer;
};

/**
 * @brief This struct will contain all the information from the format analysis.
 */

struct at_multi_level_transmit_request_package_s {
	struct at_message_transmit_group_s *msg_grp;

	void *device_ptr;

	errno_t(*send)(void *device, void *message, at_size_t len);
};

/**
 * @brief This struct will contain all the information from the format analysis.
 */

struct at_multi_level_transmit_inquire_package_s {
	size_t hash;

	size_t error;
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

struct at_control_s at_ctrl = {
	.configuration.init = at_control_configuration_init,
	.configuration.destroy = at_control_configuration_destroy,
	.configuration.retarget = at_control_configuration_retarget,

	.transmit.single_level_send = at_control_transmit_single_level_send,
	.transmit.multi_level.generate = at_control_transmit_multi_level_generate,
	.transmit.multi_level.send = at_control_transmit_multi_level_send,
	.transmit.multi_level.error = at_control_transmit_multi_level_error,

	.task_os_tick = at_control_task_os_tick,
	.device_interrupt = at_control_device_interrupt
};

struct at_task_s *at_task_os_multi_level_transmit_task = NULL;

/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

/**
 * @brief This function will retarget the device_package_ptr of at struct to the device_package_ptr specified.
 *
 * @param void
 *
 * @return void
 */

struct at_transmit_format_analysis_package_s
	at_control_transmit_format_analysis(const char *format);

/**
 * @brief This function will be called when the multi level transmit task run.
 *
 * @param void
 *
 * @return void
 */

void at_control_transmit_multi_level_send_task(void *arg_list);

/**
 * @brief This function will be called when the multi level transmit generate.
 *
 * @param void
 *
 * @return void
 */

static inline struct at_message_transmit_unit_s
at_control_transmit_multi_level_generate_core(struct at_transmit_format_analysis_package_s format_package,
											  char *ist_package[AT_CFG_TRANSMIT_GENERATE_PIECES_MAX],
											  at_size_t current_level);

/*
*********************************************************************************************************
*                                            FUNCTIONS
*********************************************************************************************************
*/

/**
 * @brief This function will initialize the at struct.
 *
 * @param void
 *
 * @return void
 */

errno_t at_control_configuration_init(struct at_s **at,
									  struct at_device_package_s *device_package_ptr)
{
	assert(at);
	assert(device_package_ptr);

	if (NULL == device_package_ptr ||														/* If the device_package_ptr is NULL */
		NULL == ((*at) = calloc(1, sizeof(struct at_s)))) {
		return 1;
	}

	if (at_message_pool_ctrl.configuration
		.init(&(*at)->message_pool_ptr,														/* Initialize the at message pool */
			  NULL,
			  NULL)) {
		return 2;
	}

	if (at_message_queue_ctrl.configuration
		.init(&(*at)->message_queue_unit.mq_ptr,											/* Initialize the at message queue */
			  NULL,
			  NULL,
			  NULL)) {
		return 3;
	}

	if (0 == ((*at)->message_queue_unit.id
			  = at_message_queue_ctrl.membership
			  .join((*at)->message_queue_unit.mq_ptr))) {									/* Join the at message queue */
		return 4;
	}

	if (at_task_ctrl.os.configuration
		.init(&(*at)->task_os_ptr,
			  (*at)->message_queue_unit.mq_ptr)) {												/* Initialize the at task os */
		return 5;
	}

	if (at_task_ctrl.task.configuration
		.init((*at)->task_os_ptr,															/* Initialize the multi level transmit task */
			  &at_task_os_multi_level_transmit_task,
			  "at task os.multi level transmit task",
			  at_control_transmit_multi_level_send_task,
			  0,																			/* Set Max priority */
			  NULL,
			  0)) {
		return 6;
	}

	if (at_control_configuration_retarget(*at, NULL, NULL, device_package_ptr)) {			/* Target the at to the device_package_ptr */
		return 7;
	}

	(*at)->switch_status = true;

	return 0;
}

/**
 * @brief This function will destroy the at struct.
 *
 * @param void
 *
 * @return void
 */

errno_t at_control_configuration_destroy(struct at_s **at)
{
	assert(at);
	assert(*at);

	if (at_task_ctrl.os.configuration
		.destroy(&(*at)->task_os_ptr)) {													/* Destroy the at tack os */
		return 2;
	}

	if (at_message_pool_ctrl.configuration
		.destroy(&(*at)->message_pool_ptr)) {												/* Destroy the at message pool */
		return 1;
	}

	if (at_message_queue_ctrl.configuration
		.destroy(&(*at)->message_queue_unit.mq_ptr)) {										/* Destroy the at message queue */
		return 1;
	}

	if ((*at)->device_package_ptr->configuration
		.demount((*at)->device_package_ptr)) {												/* Demount the device */
		return 3;
	}

	free(*at);

	(*at) = NULL;

	return 0;
}

/**
 * @brief This function will retarget the device_package_ptr of at struct to the device_package_ptr specified.
 *
 * @param void
 *
 * @return void
 */

errno_t at_control_configuration_retarget(struct at_s *at,
										  void *device,
										  void *arg_list,
										  struct at_device_package_s *device_package)
{
	assert(at);

	void *devide_package_cpy = NULL;

	if (NULL == device_package &&
		NULL == (device_package = at->device_package_ptr)) {								/* If not appoint the device_package pack */
		return 1;
	}

	if (!device_package->verify.package(device_package)) {									/* If device package is not valid */
		return 2;
	}

	if (NULL != at->device_package_ptr &&
		device_package->device_pack_id ==
		at->device_package_ptr->device_pack_id) {											/* If device_pack is the same as the device_pack of at */
		goto CONFIG_DEVICE_PART;
	} else {
		free(at->device_package_ptr);														/* Deallocate the outdated device package */
	}

	if (NULL == (devide_package_cpy =
				 calloc(1, sizeof(struct at_device_package_s))))							/* Allocate the space for device package copy */
	{
		return -1;
	}

	memcpy(devide_package_cpy, device_package,
		   sizeof(struct at_device_package_s));												/* Copy a new device package */

	at->device_package_ptr = devide_package_cpy;

CONFIG_DEVICE_PART:

	if (NULL != device &&
		NULL != arg_list) {
		errno_t err = 0;

		if (err = (errno_t)device_package->configuration.
			mount(device_package, arg_list)) {
			return err;
		}
	}

	return 0;
}

/**
 * @brief This function will generate then send the message through the device of at.
 *
 * @param void
 *
 * @return void
 */

errno_t at_control_transmit_single_level_send(struct at_s *at,
											  at_size_t count, char *ist, ...)
{
	assert(at);
	assert(ist);

	va_list va_ptr;

	void *ist_pack[AT_CFG_TRANSMIT_GENERATE_PIECES_EACH_LEVEL_MAX] = { NULL };
	char format[] = { '1', ':', (char)count + '0' };

	va_start(va_ptr, ist);

	at_size_t cnt = 0;

	do {
		ist_pack[cnt] = ist;
	} while (++cnt < count &&
			 NULL != (ist = va_arg(va_ptr, void *)));

	va_end(va_ptr);

	at_control_transmit_multi_level_generate(at, format,
											 ist_pack[0], ist_pack[1],
											 ist_pack[2], ist_pack[2]);

	at_control_transmit_multi_level_send(at);

	return 0;
}

/**
 * @brief This function will generate the at instruction through the format string.
 *
 * @param void
 *
 * @return void
 */

errno_t at_control_transmit_multi_level_generate(struct at_s *at,
												 char *format, char *ist, ...)
{
	assert(at);
	assert(format);
	assert(ist);

	va_list va_ptr;

	struct at_transmit_format_analysis_package_s
		format_package = { 0 };

	struct at_message_transmit_unit_s
		transmit_unit[AT_CFG_TRANSMIT_GENERATE_LEVELS_MAX] = { 0 };

	char
		*ist_package[AT_CFG_TRANSMIT_GENERATE_PIECES_MAX] = { NULL };						/* Allow MAX _TRANSMIT_GENERATE_PIECES_MAX pieces */

	if (AT_CFG_TRANSMIT_LEVEL_MAX < (format_package
									 = at_control_transmit_format_analysis(format))
		.level) {
		return 1;
	}

	va_start(va_ptr, ist);

	at_size_t cnt = 0;
	do {
		ist_package[cnt] = ist;																/* Store the ist fragment into the ist package */
	} while (++cnt < format_package.total_count &&											/* Limit the va_arg() by the format_package */
			 NULL != (ist = va_arg(va_ptr, char *)));										/* Always be true,because the ist must not be NULL,
																								if va_arg() and the count perform well. */

	va_end(va_ptr);

	for (size_t level = 0; level < format_package.level; level++) {							/* Merge the fragments into a whole with sprintf() */
		transmit_unit[level]
			= at_control_transmit_multi_level_generate_core(format_package,
															ist_package,
															level);
	}

	if (at_message_pool_ctrl.transmit.
		deposit(at->message_pool_ptr, format_package.level,									/* Deposit the string of each level into the message pool */
				&transmit_unit[0], &transmit_unit[1],
				&transmit_unit[2], &transmit_unit[3])) {
		return 3;
	}

	return 0;
}

/**
 * @brief This function will send the message load from the transmit message pool.
 *
 * @param void
 *
 * @return void
 */

errno_t at_control_transmit_multi_level_send(struct at_s *at)
{
	assert(at);

	static at_size_t level_last = 0;														/* Record the level last called, used to judge if reload the msg.
																								Depend on we won't resend the msg of lesser level normally. */

	static struct at_message_transmit_group_s msg = { 0 };

	if (0u == (msg = at_message_pool_ctrl.transmit
			   .load(at->message_pool_ptr))
		.count) {
		return 1;
	}

	if (NULL == msg.units[0].string.ist ||													/* Send the instruction of level 0 directly */
		0 == at->device_package_ptr->transmit
		.send(at->device_package_ptr->device_ptr,
			  msg.units[0].string.ist,
			  msg.units[0].length.ist)) {
		return 2;
	}

	if (1 < msg.count) {																	/* Publish the instruction that above level 0 to multi level transmit task */
		struct at_multi_level_transmit_request_package_s
			*request_package = NULL;

		if (NULL == (request_package														/* Allocate the request package */
					 = calloc(1, sizeof(struct at_multi_level_transmit_request_package_s)))) {
			return  -1;
		}

		request_package->device_ptr = at->device_package_ptr->device_ptr;
		request_package->msg_grp = &msg;
		request_package->send = at->device_package_ptr->transmit.send;

		if (at_message_queue_ctrl.communication
			.publish(at->message_queue_unit.mq_ptr,											/* Publish the request package to task */
					 request_package,
					 3,																		/* The message queue id of the multi level transmit level is 3 */
					 1)) {
			return 3;
		}
	}

	return 0;
}

/**
 * @brief This function will inquire the error code during the multi level sending.
 *
 * @param void
 *
 * @return void
 */

errno_t at_control_transmit_multi_level_error(struct at_s *at)
{
	assert(at);

	static struct at_message_queue_message_package_s message_package;
	static struct at_multi_level_transmit_inquire_package_s *inquire_message;

	if (NULL == (message_package = at_message_queue_ctrl.communication
				 .subscribe(at->message_queue_unit.mq_ptr, 1)).message) {
		return 1;
	}

	inquire_message = message_package.message;

																							/* TODO:Distinguish which instruction */

	return inquire_message->error;
}

/**
 * @brief This function will handle the soft logic of the at task os.
 *
 * @param void
 *
 * @return void
 */

extern inline void
at_control_task_os_tick(struct at_s *at)
{
	assert(at);

	at_task_ctrl.os.core(at->task_os_ptr, at->message_pool_ptr);
}

/**
 * @brief This function need be place under the hardware interrupt point of the device.
 *
 * @param void
 *
 * @return void
 */

extern inline void
at_control_device_interrupt(struct at_s *at)
{
	assert(at);

	struct at_device_package_interrupt_return_s
		interrupt_return = { 0 };

	static char string[100] = { 0 };
	static at_size_t count = 0;

	if (NULL == (interrupt_return = (at->device_package_ptr
									 ->interrupt(at->device_package_ptr->device_ptr))).string) {
		return;
	}

	/* These below are dependent on the situation that each AT instruction feedbacks with "\n" */

	char template[] = "\n";

	if (NULL == memcpy(string + count, interrupt_return.string, interrupt_return.count)) {
		return;
	}

	count += interrupt_return.count;

	if (NULL == strstr(string, template)) {
		goto NOT_FIT;
	}

	at_message_pool_ctrl.feedback.
		deposit(at->message_pool_ptr, string, count + 1);									/* Deposit the string into the feedback memory pool */

	memset(string, '\0', count);															/* Clean the static string and count */
	count = 0;

NOT_FIT:

	return;
}

/**
 * @brief This function will be called when the multi level transmit task run.
 *
 * @param void
 *
 * @return void
 */

void at_control_transmit_multi_level_send_task(void *arg_list)
{
	static struct at_multi_level_transmit_request_package_s
		*request_package = NULL;

	struct at_task_function_arguement_list_package_s
		*arg_list_package = arg_list;

	static struct at_message_queue_unit_s
		*mq_unit_outward;

	char
		*feedback_message = NULL;

	#if (AT_CFG_DEBUG_EN)

	printf("at task os.multi level transmit task.enter\r\n");

	#endif // (AT_CFG_DEBUG_EN)

	static at_size_t count = 0xff;
	if (NULL != request_package
		&& request_package->msg_grp->count > count) {
		#if (AT_CFG_DEBUG_EN)

		printf("at task os.multi level transmit task.send:instruction is \"%s\" answer is \"%s\"\r\n",
			   request_package->msg_grp->units[count].string.ist,
			   request_package->msg_grp->units[count].string.asw);

		#endif // (AT_CFG_DEBUG_EN)

		static float search_substring_rate = 0.0;
		static errno_t device_send_error = 0;

		if ('\0' != *request_package->msg_grp->units[count].string.asw) {
			static struct search_substring_package_s search_substring_package;

			if (NULL == (feedback_message = at_message_queue_ctrl.communication
						 .subscribe(arg_list_package->mq_unit->mq_ptr,						/* Subscribe the feedback message from the message queue host by task os */
									arg_list_package->mq_unit->id).message)) {
				return;
			}

			#if (AT_CFG_DEBUG_EN)

			printf("at task os.multi level transmit task.message:\"%s\" \r\n", feedback_message);

			#endif // (AT_CFG_DEBUG_EN)

			search_substring_package.str.string = feedback_message;
			search_substring_package.str.length = strlen(feedback_message);
			search_substring_package.substr.string = request_package->msg_grp->units[count].string.asw;
			search_substring_package.substr.length = request_package->msg_grp->units[count].length.asw;

			if (1.0f > (search_substring_rate
						= search_substring_control(AT_CFG_TRANSMIT_SEARCH_SUBSTRING_ALGORITHM,
												   search_substring_package))) {			/* Verify the message string,if not match */
				static at_size_t verify_count = 0;

				if (AT_CFG_TRANSMIT_VERIFY_EXPIRE_COUNT_MAX <= verify_count) {				/* Allow MAX _TRANSMIT_VERIFY_OVERDUE_TIME_MAX times,then timeout */
					verify_count = 0;

					device_send_error = 0;
					goto FAIL;
				} else {
					verify_count++;

					goto EXIT;
				}
			}
		}

		device_send_error = request_package													/* If the device send code is not 0,it's correct */
			->send(request_package->device_ptr,												/* Send the instruction to the device */
				   request_package->msg_grp->units[count].string.ist,
				   request_package->msg_grp->units[count].length.ist);

		if (1.0f == search_substring_rate) {
			static struct at_multi_level_transmit_inquire_package_s *inquire_package_ptr;

		FAIL:

			if (NULL == (inquire_package_ptr
						 = calloc(1,														/* Allocate a inquire package */
								  sizeof(struct at_multi_level_transmit_inquire_package_s)))) {
				while (true) {
				}
			}

			inquire_package_ptr->error = device_send_error;									/* Set the error of the inquire package */

			at_message_queue_ctrl.communication
				.publish(mq_unit_outward->mq_ptr,											/* Publish the inquire package to the message queue */
						 inquire_package_ptr,
						 1, 3);
		}

	EXIT:

		count++;

		return;
	} else {
		if (NULL == (mq_unit_outward
					 = at_task_ctrl.task.message_queue.outward
					 .join(arg_list_package->task_os,										/* Join the message queue hosted by outward */
						   arg_list_package->task))) {
			return;
		}

		if (NULL == (request_package = (at_message_queue_ctrl.communication
										.subscribe(mq_unit_outward->mq_ptr,					/* Subscribe the message from outward message queue host by at */
												   mq_unit_outward->id)).message)) {
			return;
		}

		count = 1;
	}
}

/**
 * @brief This function will analysis the transmit format string.
 *
 * @param void
 *
 * @return void
 */

struct at_transmit_format_analysis_package_s
	at_control_transmit_format_analysis(const char *format)
{
	assert(format);

	/*										EXPLNATION
	 *
	 *							char *format_template = "#2:1:@2";
	 *
	 *		+ This means: this at transmit instruction group template has two levels,
	 *			it's first level have 1 piece,
	 *			the second level have 2 pieces and it have to pend the answer from the feedback.
	 *
	 *		+ So,the format rules are these below:
	 *			- '#' + 'how much levels	= this instruction contains how much levels.
	 *			- '@' + 'how much pieces 	= this level of instruction has to pend answer,
	 *											this level of answer contains how much pieces.
	 *			- ':' + 'how much pieces 	= this level of instruction contains how much pieces.
	 */

	struct at_transmit_format_analysis_package_s
		format_package = { 0 };

	at_size_t
		symbol = 0,
		level = 0;
	bool get_level = false;

	while (format[symbol] != '\0') {
		if (format[symbol] == '#') {
			format_package.level = format[symbol + 1] - '0';								/* Set the next the value of symbol as the level */

			get_level = true;

			if (NULL == (format_package.info =
						 calloc(format_package.level, sizeof(at_size_t)))
				|| NULL == (format_package.info_answer =
							calloc(format_package.level, sizeof(at_size_t)))) {
				format_package.level = 255;

				goto EXIT;
			}
		}

		if (format[symbol] == ':') {
			if (!get_level) {
				format_package.level = 254;

				goto EXIT;
			} else {
				*(format_package.info + level) = format[symbol + 1] - '0';					/* Set the next the value of symbol as the level information */
				format_package.total_count += *(format_package.info + level);

				if ('@' == format[symbol + 2]) {
					*(format_package.info_answer + level) = format[symbol + 3] - '0';					/* Set the next the value of symbol as the level information */
					format_package.total_count += *(format_package.info_answer + level);
				}

				level++;
			}
		}

		symbol++;
	}

EXIT:

	return format_package;
}

/**
 * @brief This function will be called when the multi level transmit generate.
 *
 * @param void
 *
 * @return void
 */

static inline struct at_message_transmit_unit_s
at_control_transmit_multi_level_generate_core(struct at_transmit_format_analysis_package_s format_package,
											  char *ist_package[AT_CFG_TRANSMIT_GENERATE_PIECES_MAX],
											  at_size_t current_level)
{
	static struct at_message_transmit_unit_s
		transmit_unit = { 0 };

	char format_template[] = "%s%s%s%s";

	char
		format_string_ist[(AT_CFG_TRANSMIT_GENERATE_PIECES_EACH_LEVEL_MAX << 1) + 1] = { 0 },
		format_string_asw[(AT_CFG_TRANSMIT_GENERATE_PIECES_EACH_LEVEL_MAX << 1) + 1] = { 0 },
		*pieces_ist[AT_CFG_TRANSMIT_GENERATE_PIECES_EACH_LEVEL_MAX] = { NULL },
		*pieces_asw[AT_CFG_TRANSMIT_GENERATE_PIECES_EACH_LEVEL_MAX] = { NULL };

	at_size_t
		count = 0;
	static	at_size_t
		shift = 0;

	transmit_unit.length.ist = *(format_package.info + current_level);						/* Assign the length part of the transmit unit */
	transmit_unit.length.asw = *(format_package.info_answer + current_level);

	if (NULL == (transmit_unit.string.ist 													/* Allocate the string part of the transmit unit */
				 = calloc(1, AT_CFG_TRANSMIT_GENERATE_STRING_LENGTH_MAX))					/* Allow MAX _TRANSMIT_GENERATE_STRING_LENGTH_MAX pieces each level */
		|| NULL == (transmit_unit.string.asw
					= calloc(1, AT_CFG_TRANSMIT_GENERATE_STRING_LENGTH_MAX))) {
		return (struct at_message_transmit_unit_s) { (void *)-1 };
	}

	if (AT_CFG_TRANSMIT_GENERATE_PIECES_EACH_LEVEL_MAX < transmit_unit.length.ist) {
		return (struct at_message_transmit_unit_s) { (void *)1 };
	}

	if (0 == current_level) {
		shift = 0;																			/* Reset the shift point of the ist package */
	}

	do {
		pieces_ist[count] = ist_package[shift + count];
	} while (transmit_unit.length.ist > ++count);

	shift += transmit_unit.length.ist;														/* Increase the shift point of the ist package */
	count = 0;

	do {
		pieces_asw[count] = ist_package[shift + count];
	} while (transmit_unit.length.asw > ++count);

	if (NULL == (memcpy(&format_string_ist, 												/* Clip the format_template into the needed format string */
						&format_template,
						transmit_unit.length.ist << 1))
		|| NULL == (memcpy(&format_string_asw,
						   &format_template,
						   transmit_unit.length.asw << 1))) {
		return (struct at_message_transmit_unit_s) { (void *)2 };
	}

	transmit_unit.length.ist																/* Assign the length of the transmit_unit.string */
		= sprintf_s(transmit_unit.string.ist,												/* Format the instruction string into the transmit_unit.string */
					AT_CFG_TRANSMIT_GENERATE_STRING_LENGTH_MAX, format_string_ist,
					pieces_ist[0], pieces_ist[1], pieces_ist[2], pieces_ist[3]);			/* Allow MAX _PIECES_EACH_LEVEL_MAX pieces each level */
	transmit_unit.length.asw
		= sprintf_s(transmit_unit.string.asw,
					AT_CFG_TRANSMIT_GENERATE_STRING_LENGTH_MAX, format_string_asw,
					pieces_asw[0], pieces_asw[1], pieces_asw[2], pieces_asw[3]);

	return transmit_unit;
}