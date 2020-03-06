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

struct at_information_s {
	bool switch_status;

	void *multi_level_transmit_ptr;
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
	struct at_information_s info;

	struct at_device_package_s *device_package;

	struct at_task_os_s *task_os;

	struct at_message_s *message;

	struct at_exception_s exception;
};

/**
 * @brief This struct will contain all the information from the format analysis.
 */

struct at_transmit_format_analysis_package_s {
	at_size_t level;
	at_size_t total_count;
	at_size_t *info;
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
 * @brief This function will retarget the device_package of at struct to the device_package specified.
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

void at_control_at_task_os_multi_level_transmit_task_function(void *arg_list);

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
									  struct at_device_package_s *device_package)
{
	assert(at);
	assert(device_package);

	if (NULL == device_package ||															/* If the device_package is NULL */
		NULL == ((*at) = calloc(1, sizeof(struct at_s)))) {
		//(*at)->exception.hardware_not_ready();

		return 1;
	}

	at_message_ctrl.configuration.init(&(*at)->message);									/* Initialize the at message */

	at_task_ctrl.os.configuration.init(&(*at)->task_os);									/* Initialize the at task os */

	at_task_ctrl.task.configuration.init((*at)->task_os,									/* Initialize the multi level transmit task */
										 &at_task_os_multi_level_transmit_task,
										 "at task os.multi level transmit task",
										 at_control_at_task_os_multi_level_transmit_task_function,
										 0,													/* Set Max priority */
										 NULL,
										 NULL,
										 0);

	if (at_control_configuration_retarget(*at, NULL, NULL, device_package)) {				/* Target the at to the device_package */
		return 2;
	}

	(*at)->info.switch_status = true;

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

	if (at_message_ctrl.configuration.destroy(&(*at)->message)) {							/* Destroy the at message */
		return 1;
	}

	if (at_task_ctrl.os.configuration.destroy(&(*at)->task_os)) {							/* Destroy the at tack os */
		return 2;
	}

	free(*at);

	(*at) = NULL;

	return 0;
}

/**
 * @brief This function will retarget the device_package of at struct to the device_package specified.
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
		NULL == (device_package = at->device_package)) {									/* If not appoint the device_package pack */
		return 1;
	}

	if (!device_package->verify.package(device_package)) {									/* If device package is not valid */
		return 2;
	}

	if (NULL != at->device_package &&
		device_package->device_pack_id ==
		at->device_package->device_pack_id) {												/* If device_pack is the same as the device_pack of at */
		goto CONFIG_DEVICE_PART;
	} else {
		free(at->device_package);															/* Deallocate the outdated device package */
	}

	if (NULL == (devide_package_cpy =
				 calloc(1, sizeof(struct at_device_package_s))))							/* Allocate the space for device package copy */
	{
		return -1;
	}

	memcpy(devide_package_cpy, device_package,
		   sizeof(struct at_device_package_s));												/* Copy a new device package */

	at->device_package = devide_package_cpy;

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

	va_start(va_ptr, ist);

	struct at_transmit_format_analysis_package_s
		format_package = at_control_transmit_format_analysis(format);

	if (AT_CFG_TRANSMIT_LEVEL_MAX < format_package.level) {
		return 1;
	}

	char
		*ist_package[AT_CFG_TRANSMIT_GENERATE_PIECES_MAX] = { NULL },
		string_package[AT_CFG_TRANSMIT_GENERATE_LEVELS_MAX]									/* Allow MAX _TRANSMIT_GENERATE_LEVELS_MAX level */
		[AT_CFG_TRANSMIT_GENERATE_STRING_LENGTH_MAX] = { 0 };								/* Allow MAX _TRANSMIT_GENERATE_STRING_LENGTH_MAX length */

	at_size_t cnt = 0;

	do {
		ist_package[cnt] = ist;																/* Store the ist fragment into the ist package */
	} while (++cnt < format_package.total_count &&											/* Limit the va_arg() by the format_package */
			 NULL != (ist = va_arg(va_ptr, char *)));										/* Always be true,because the ist must not be NULL,
																								if va_arg() and the count perform well. */

	va_end(va_ptr);

	for (size_t level = 0; level < format_package.level; level++) {							/* Merge the fragments into a whole with sprintf() */
		at_size_t
			pieces = *(format_package.info + level),
			count = 0;
		char
			format_string[AT_CFG_TRANSMIT_GENERATE_PIECES_EACH_LEVEL_MAX * sizeof("%s")] = { 0 },
			*ist_pieces[AT_CFG_TRANSMIT_GENERATE_PIECES_EACH_LEVEL_MAX] = { NULL };

		static at_size_t
			shift = 0;

		if (AT_CFG_TRANSMIT_GENERATE_PIECES_EACH_LEVEL_MAX < pieces) {
			return 2;
		}

		do {
			ist_pieces[count] = ist_package[shift + count];
			strcat(format_string, "%s");
		} while (pieces > ++count);

		shift += pieces;																	/* Increase the shift point of the ist package */

		sprintf_s(string_package[level],
				  AT_CFG_TRANSMIT_GENERATE_STRING_LENGTH_MAX, format_string,
				  ist_pieces[0], ist_pieces[1], ist_pieces[2], ist_pieces[3]);				/* Allow MAX _PIECES_EACH_LEVEL_MAX pieces each level */
	}

	if (at_message_ctrl.transmit.
		deposit(at->message, format_package.level,											/* Deposit the string of each level into the message pool */
				string_package[0], strlen(string_package[0]),
				string_package[1], strlen(string_package[1]),
				string_package[2], strlen(string_package[2]),
				string_package[3], strlen(string_package[3]))) {
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

errno_t at_control_transmit_multi_level_send(struct at_s *at,
											 at_size_t level)
{
	assert(at);

	static at_size_t level_last = 0;														/* Record the level last called, used to judge if reload the msg.
																								Depend on we won't resend the msg of lesser level normally. */

	static struct at_message_transmit_group_s msg = { 0 };

	if (level_last >= level) { 																/* If the level is lesser than the level last called. */
		if (0u == (msg = at_message_ctrl.transmit.load(at->message)).count) {
			return 1;
		}
	}

	if (NULL == msg.pool[level] ||
		at->device_package->transmit.
		send(at->device_package->device_ptr, msg.pool[level], strlen(msg.pool[level]))) {
		return 2;
	}

	return 0;
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

	at_task_ctrl.os.core(at->task_os, at->message);
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
		*interrupt_return = NULL;

	static char string[100] = { 0 };
	static at_size_t count = 0;

	if (NULL == (interrupt_return = (at->device_package->
									 interrupt(at->device_package->device_ptr)))->string) {
		return;
	}

	/* These below are dependent on the situation that each AT instruction feedbacks with "\n" */

	char template[] = "\n";

	memcpy(string + count, interrupt_return->string, interrupt_return->count);
	count += interrupt_return->count;

	if (NULL == strstr(string, template)) {
		goto NOT_FIT;
	}

	at_message_ctrl.feedback.
		deposit(at->message, string, count+1);												/* Deposit the string into the feedback memory pool */

	memset(string, '\0', count);															/* Clean the static string and count */
	count = 0;

NOT_FIT:

	return;
}

/**
 * @brief This function will retarget the device_package of at struct to the device_package specified.
 *
 * @param void
 *
 * @return void
 */

struct at_transmit_format_analysis_package_s
	at_control_transmit_format_analysis(const char *format)
{
	assert(format);

	/*							EXPLNATION
	 *
	 *				char *format_template = "#2:1:2";
	 *
	 *			+ This means: this at transmit instruction group template has two levels,
	 *				it's first level have 1 piece,and the second level have 2 pieces.
	 *
	 *			+ So,the format rules are these below:
	 *				- '#' + 'how much levels'
	 *				- ':' + 'how much pieces this level contains'
	 */

	struct at_transmit_format_analysis_package_s
		format_package = { 0 };

	at_size_t
		symbol = 0,
		level = 0;
	bool get_level = false;

	while (format[symbol] != '\0') {
		if (format[symbol] == '#') {
			format_package.level = format[symbol + 1] - '0';										/* Set the next the value of symbol as the level */

			get_level = true;

			if (NULL == (format_package.info =
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
				*(format_package.info + level) = format[symbol + 1] - '0';			/* Set the next the value of symbol as the level information */
				format_package.total_count += *(format_package.info + level);

				level++;
			}
		}

		symbol++;
	}

EXIT:

	return format_package;
}

/**
 * @brief This function will be called when the multi level transmit task run.
 *
 * @param void
 *
 * @return void
 */

void at_control_at_task_os_multi_level_transmit_task_function(void *arg_list)
{
	printf("at task os.multi level transmit task.message:\"%s\" \r\n", (char *)arg_list);


}