/*
*********************************************************************************************************
*                                               MODULE
*
* Note(s) : (1) This definition header file is protected from multiple pre-processor inclusion
*               through use of the definition module present pre-processor macro definition.
*********************************************************************************************************
*/

#ifndef __AT_DEVICE_H
#define __AT_DEVICE_H

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
 * @brief This struct will contain all the universal vector functions address.
 */

struct at_device_control_s {
	struct {
		errno_t(*init_package)(struct at_device_package_s **package);

		errno_t(*deinit_package)(struct at_device_package_s *package);

		errno_t(*destroy_package)(struct at_device_package_s **package);
	}configuration;

	struct {
		void *(*package_device)(struct at_device_package_s *package);

		at_size_t(*package_size)(void);
	}inquire;

	struct {
		struct {
			errno_t(*mount)(struct at_device_package_s *package,
							void *arg_list);

			errno_t(*demount)(struct at_device_package_s *package);
		}configuration;

		struct {
			errno_t(*send)(struct at_device_package_s *package,
						   void *data, size_t len);

			void *(*receive)(struct at_device_package_s *package,
							 size_t len);
		}transmit;

		struct {
			bool (*device)(struct at_device_package_s *package,
						   void *device);

			bool (*package)(struct at_device_package_s *package);
		}verify;
	}package_operator;
};

/*
*********************************************************************************************************
*								            FUNCTION PROTOTYPES
*********************************************************************************************************
*/

/**
 * @brief This function will deinitialize the i/o stream of the windows.
 *
 * @param void
 *
 * @return void
 */

errno_t at_device_control_configuration_deinit_package(struct at_device_package_s *package);

/**
 * @brief This function will destroy the i/o stream of the windows.
 *
 * @param void
 *
 * @return void
 */

errno_t at_device_control_configuration_destroy_package(struct at_device_package_s **package);

/**
 * @brief This function will inquire the device of the device package.
 *
 * @param void
 *
 * @return void
 */

void *at_device_control_inquire_package_device(struct at_device_package_s *package);

/**
 * @brief This function will inquire the size of the data package.
 *
 * @param void
 *
 * @return void
 */

at_size_t at_device_control_inquire_package_size(void);

/**
 * @brief This function will mount the device to the package.
 *
 * @param void
 *
 * @return void
 */

errno_t at_device_control_package_operator_configuration_mount(struct at_device_package_s *package,
															   void *arg_list);

/**
 * @brief This function will demount the device from the package.
 *
 * @param void
 *
 * @return void
 */

errno_t at_device_control_package_operator_configuration_demount(struct at_device_package_s *package);

/**
 * @brief This function will control the send/output of the device i/o.
 *
 * @param void
 *
 * @return void
 */

errno_t at_device_control_package_operator_transmit_send(struct at_device_package_s *package,
														 void *data, size_t len);

/**
 * @brief This function will control the receive/input of the device i/o.
 *
 * @param void
 *
 * @return void
 */

void *at_device_control_package_operator_transmit_receive(struct at_device_package_s *package,
														  size_t len);

/**
 * @brief This function will verify if the device is valid.
 *
 * @param void
 *
 * @return void
 */

bool at_device_control_package_operator_verify_device(struct at_device_package_s *package,
													  void *device);

/**
 * @brief This function will verify if the device package is valid.
 *
 * @param void
 *
 * @return void
 */

bool at_device_control_package_operator_verify_package(struct at_device_package_s *package);

/*
*********************************************************************************************************
*                                       EXTERN GLOBAL VARIABLES
*********************************************************************************************************
*/

/**
 * @brief This variables will obtain the operator of the at device.
 */

struct at_device_control_s at_device_ctrl;

/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif // !__AT_DEVICE_H