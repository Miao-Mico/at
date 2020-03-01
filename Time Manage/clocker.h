/*
*********************************************************************************************************
*                                               MODULE
*
* Note(s) : (1) This definition header file is protected from multiple pre-processor inclusion
*               through use of the definition module present pre-processor macro definition.
*********************************************************************************************************
*/

#ifndef __CLOCKER_H
#define __CLOCKER_H

/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include "time_manage_def.h"

/*
*********************************************************************************************************
*									            DEFINES
*********************************************************************************************************
*/

/* Configure        if enable hardware timer.															*/
#define CLOCKER_CFG_HARDWARE_TIMER_EN											0u

/* Configure    if enable integrated structure.                                                         */
#define CLOCKER_CFG_INTERGRATED_STRUCTURE_MODE_EN					            1u

/* Configure    if enable debug.                                                                        */
#define CLOCKER_CFG_DEBUG_EN					                                1u

/*
*********************************************************************************************************
*									           DATA TYPES
*********************************************************************************************************
*/

/**
 * @brief This typedef will control the clocker.
 */

struct clocker_control_s {
	struct {
		errno_t(*init)(struct clocker_s **clocker,
					   time_manage_size_t clocker_freq,
					   struct time_manage_timer_package_s *timer_package);

        errno_t(*destroy)(struct clocker_s **clocker);

		errno_t(*calibrate)(struct clocker_s *clocker);

		errno_t(*change_time_sources)(struct clocker_s *clocker,
									  void *timer,
									  struct time_manage_timer_package_s *timer_package);
	}configuration;

	struct {
		time_manage_size_t(*timer_freq)(struct clocker_s *clocker);

		time_manage_size_t(*clcoker_freq)(struct clocker_s *clocker);

		struct time_manage_visual_time_frame_s(*visual_time_frame_type_record)(struct clocker_s *clocker);
	}inquire;

	errno_t(*start)(struct clocker_s *clocker);

	time_manage_size_t(*stop)(struct clocker_s *clocker);

	#if (CLOCKER_CFG_HARDWARE_TIMER_EN)

	time_manage_timer_counter_t counter;

	#endif // (CLOCKER_CFG_HARDWARE_TIMER_EN)
};

/*
*********************************************************************************************************
*								            FUNCTION PROTOTYPES
*********************************************************************************************************
*/

/**
 * @brief This function will initialize the clocker.
 *
 * @param void
 *
 * @return void
 */

errno_t cloker_control_configuration_init(struct clocker_s **clocker,
										  time_manage_size_t clocker_freq,
										  struct time_manage_timer_package_s *timer_package);

/**
 * @brief This function will destroy the clocker.
 *
 * @param void
 *
 * @return void
 */

errno_t cloker_control_configuration_destroy(struct clocker_s **clocker);

/**
 * @brief This function will calibrate the clocker to perform better.
 *
 * @param void
 *
 * @return void
 */

errno_t cloker_control_configuration_calibrate(struct clocker_s *clocker);

/**
 * @brief This function will change the timer of clocker.
 *
 * @param void
 *
 * @return void
 */

errno_t cloker_control_configuration_change_timer_sources(struct clocker_s *clocker,
														  void *timer,
														  struct time_manage_timer_package_s *timer_package);

/**
 * @brief This function will inquire the frequency of the timer.
 *
 * @param void
 *
 * @return void
 */

time_manage_size_t cloker_control_inquire_timer_freq(struct clocker_s *clocker);

/**
 * @brief This function will inquire the frequency of the clocker.
 *
 * @param void
 *
 * @return void
 */

time_manage_size_t cloker_control_inquire_clocker_freq(struct clocker_s *clocker);

/**
 * @brief This function will inquire the record of clocker with the type of visual time frame.
 *
 * @param void
 *
 * @return void
 */

struct time_manage_visual_time_frame_s
	cloker_control_inquire_visual_time_frame_type_record(struct clocker_s *clocker);

/**
 * @brief This function will change the timer of clocker.
 *
 * @param void
 *
 * @return void
 */

errno_t cloker_control_start(struct clocker_s *clocker);

/**
 * @brief This function will change the timer of clocker.
 *
 * @param void
 *
 * @return void
 */

time_manage_size_t cloker_control_stop(struct clocker_s *clocker);

#if (CLOCKER_CFG_HARDWARE_TIMER_EN)

/**
 * @brief This function will change the timer of clocker.
 *
 * @param void
 *
 * @return void
 */

void cloker_control_hardware_timer_counter(void *clocker);

#endif // (CLOCKER_CFG_HARDWARE_TIMER_EN)

/*
*********************************************************************************************************
*                                       EXTERN GLOBAL VARIABLES
*********************************************************************************************************
*/

#if (CLOCKER_CFG_INTERGRATED_STRUCTURE_MODE_EN)

struct clocker_control_s clocker_ctrl;

#endif // (CLOCKER_CFG_INTERGRATED_STRUCTURE_MODE_EN)

/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif // !__CLOCKER_H