/*
*********************************************************************************************************
*                                               MODULE
*
* Note(s) : (1) This definition header file is protected from multiple pre-processor inclusion
*               through use of the definition module present pre-processor macro definition.
*********************************************************************************************************
*/

#ifndef __AT_H
#define __AT_H

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
 * @brief This struct will contain all the at control functions.
 */

struct at_control_s {
	struct {
		errno_t(*init)(struct at_s **at,
					   struct at_device_package_s *device_package);

		errno_t(*destroy)(struct at_s **at);

		errno_t(*retarget)(struct at_s *at,
						   void *device,
						   void *arg_list,
						   struct at_device_package_s *device_package);
	}configuration;

	struct {
		errno_t(*single_level_send)(struct at_s *at,
									at_size_t count, char *ist, ...);

		struct {
			errno_t(*generate)(struct at_s *at,
							   char *format, char *ist, ...);

			errno_t(*send)(struct at_s *at,
						   at_size_t level,
						   char *promise_msg,
						   at_size_t len);
		} multi_level;
	}transmit;

	void (*task_os_tick)(struct at_s *at);

	void (*device_interrupt)(struct at_s *at);
};

/*
*********************************************************************************************************
*								            FUNCTION PROTOTYPES
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
									  struct at_device_package_s *device_package);

/**
 * @brief This function will destroy the at struct.
 *
 * @param void
 *
 * @return void
 */

errno_t at_control_configuration_destroy(struct at_s **at);

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
										  struct at_device_package_s *device_package);

/**
 * @brief This function will generate then send the message through the device of at.
 *
 * @param void
 *
 * @return void
 */

errno_t at_control_transmit_single_level_send(struct at_s *at,
											  at_size_t count, char *ist, ...);

/**
 * @brief This function will generate the at instruction through the format string.
 *
 * @param void
 *
 * @return void
 */

errno_t at_control_transmit_multi_level_generate(struct at_s *at,
												 char *format, char *ist, ...);

/**
 * @brief This function will send the message load from the transmit message pool.
 *
 * @param void
 *
 * @return void
 */

errno_t at_control_transmit_multi_level_send(struct at_s *at,
											 at_size_t level,
											 char *promise_msg,
											 at_size_t len);

/**
 * @brief This function will handle the soft logic of the at task os.
 *
 * @param void
 *
 * @return void
 */

void at_control_task_os_tick(struct at_s *at);

/**
 * @brief This function need be place under the hardware interrupt point of the device.
 *
 * @param void
 *
 * @return void
 */

void at_control_device_interrupt(struct at_s *at);

/*
*********************************************************************************************************
*                                       EXTERN GLOBAL VARIABLES
*********************************************************************************************************
*/

#if (AT_CFG_INTERGRATED_STRUCTURE_MODE_EN)

/**
 * @brief This struct will contain all the universal vector functions address.
 */

struct at_control_s at_ctrl;

#endif // (AT_CFG_INTERGRATED_STRUCTURE_MODE_EN)

/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif // !__AT_H