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

#include "at_device.h"
#include "at_task.h"

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
		void (*init)(struct at_s **at, struct at_device_package_s *device_pack);

		void (*destroy)(struct at_s **at);

		void (*retarget)(struct at_s *at, void *device);

		void (*transmit_tail)(short level, char *transmit_end_with, ...);
	}configuration;

	struct {
		void (*single_level)(short param_amt, char *ist, ...);

		struct {
			void (*generate)(char *param_info, char *ist, ...);
			void (*transmit)(short level);
		} multi_level;
	}transmit;

	struct {
		void (*set_priority)(short priority);

		void (*software_handle)(struct at_s *at);
		void (*hardware_irqn)(struct at_s *at);

		bool (*init_thread)(at_feedback_thread_stpp thread,
							char *name, void (*verify_pass_callback)(void), void (*verify_fail_callback)(void));
		void (*free_thread)(at_feedback_thread_stpp thread);
		void (*config_thread)(short cfg_groups_amount, at_feedback_thread_stpp thread, short priority_module, ...);

		bool (*request)(short priority_of_thread, short rule_amount, char *rule, ...);	// 请求
		int (*inquire)(short priority_of_thread, bool unlock_result);				// 查询
		void (*unlock_result)(short priority_of_thread);							// 解锁结果，即查询成功即擦除

		void (*set_verify_pass_threshold_value)(short priority_of_thread, float value);	// 设置阈值
		void (*set_callback)(short priority_of_thread, bool pass, bool fail, ...);		// 设置阈值
		at_feedback_thread_stp(*get_thread)(short priority_of_thread);
	}feedback;
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

bool at_control_configuration_init(struct at_s **at,
								  struct at_device_package_s *device_pack);

/**
 * @brief This function will destroy the at struct.
 *
 * @param void
 *
 * @return void
 */

void at_control_configuration_destroy(struct at_s **at);

/**
 * @brief This function will retarget the device_package of at struct to the device_package specified.
 *
 * @param void
 *
 * @return void
 */

void at_control_configuration_retarget(struct at_s *at,
									  void *device,
									  void *arg_list,
									  struct at_device_package_s *device_package);

/**
 * @brief This function will set the specified tail of the at transmit.
 *
 * @param void
 *
 * @return void
 */

void at_control_configuration_transmit_tail(struct at_s *at,
										   short level,
										   char *tail, ...);

/**
 * @brief This function will generate and transmit the instruction according the giving specified parameter.
 *
 * @param void
 *
 * @return void
 */

void at_control_transmit_single_level_transmit(struct at_s *at,
											   short param_amt, char *ist, ...);

/**
 * @brief This function will generate the instruction according the giving specified parameter.
 *
 * @param void
 *
 * @return void
 */

void at_control_transmit_multi_level_generate(struct at_s *at,
											  char *param_info, char *ist, ...);

/**
 * @brief This function will transmit the instruction
 *			who is the string stored in the specified level of the at transmit buffer.
 *
 * @param void
 *
 * @return void
 */

void at_control_transmit_multi_level_transmit(struct at_s *at,
											  short level);

void at_control_feedback_set_priority(short priority);

void at_control_feedback_software_handle(void);

void at_control_feedback_hardware_irqn(void);

/**
 * @brief This function will initialize the feedback thread.
 *
 * @param void
 *
 * @return void
 */

bool at_control_feedback_init_thread(struct at_s *at,
									 struct at_feedback_thread_s **thread,
									 char *name, void (*verify_pass_callback)(void), void (*verify_fail_callback)(void));

/**
 * @brief This function will free the feedback thread.
 *
 * @param void
 *
 * @return void
 */

void at_control_feedback_free_thread(struct at_s *at,
									 struct at_feedback_thread_s **thread);

/**
 * @brief This function will config the feedback thread.
 *
 * @param void
 *
 * @return void
 */

void at_control_feedback_config_thread(struct at_s *at,
									   short cfg_groups_amount,
									   struct at_feedback_thread_s *thread,
									   short priority_module, ...);

/**
 * @brief This function will request the specified thread verify the rule.
 *
 * @param void
 *
 * @return void
 */

bool at_control_feedback_request(struct at_s *at,
								 short priority_of_thread,
								 short rule_amount,
								 char *rule, ...);	// 请求

/**
 * @brief This function will inquire the specified thread the result of verification.
 *
 * @param void
 *
 * @return void
 */

int at_control_feedback_inquire(struct at_s *at,
								short priority_of_thread, bool unlock_result);				// 查询

void at_control_feedback_unlock_result(short priority_of_thread);							// 解锁结果，即查询成功即擦除

void at_control_feedback_set_verify_pass_threshold_value(short priority_of_thread,
														 float value);	// 设置阈值

void at_control_feedback_set_callback(short priority_of_thread,
									  bool pass, bool fail, ...);		// 设置阈值

at_feedback_thread_stp at_control_feedback_get_thread(short priority_of_thread);

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