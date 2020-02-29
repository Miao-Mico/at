/*
*********************************************************************************************************
*                                               MODULE
*
* Note(s) : (1) This definition header file is protected from multiple pre-processor inclusion
*               through use of the definition module present pre-processor macro definition.
*********************************************************************************************************
*/

#ifndef __TIME_MANAGE_H
#define __TIME_MANAGE_H

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

/*
*********************************************************************************************************
*									           DATA TYPES
*********************************************************************************************************
*/

/**
 * @brief This type is the body of the time frame struct.
 */

struct time_manage_timer_package_s {
	void *timer_ptr;

	struct {
		void (*init)(void *timer, void *arg);
		void (*deinit)(void *timer, void *arg);
	}configuration;

	struct {
		time_manage_size_t(*timer_frequency)(void *timer);

		time_manage_size_t(*timer_counter)(void *timer);
	}inquire;

	bool (*verify_package)(struct time_manage_timer_package_s *timer_package);
};

/*
*********************************************************************************************************
*								            FUNCTION PROTOTYPES
*********************************************************************************************************
*/

/**
 * @brief This function will convert the timer counter tick to the microsecond value.
 *
 * @param void
 *
 * @return void
 */

time_manage_size_t
time_manage_control_convert_timer_counter_tick_to_microsec(time_manage_float_t timer_ticks_per_microsec,
														   time_manage_size_t timer_freq,
														   time_manage_size_t counter_tick);

/**
 * @brief This function will convert the microsecond value to the visual time frame struct.
 *
 * @param void
 *
 * @return void
 */

struct time_manage_visual_time_frame_s
	time_manage_control_convert_microsec_to_visual_time_frame(time_manage_size_t timer);

/**
 * @brief This function will convert the visual time frame struct to the microsecond value.
 *
 * @param void
 *
 * @return void
 */

time_manage_size_t
time_manage_control_convert_visual_time_frame_to_microsec(struct time_manage_visual_time_frame_s time_frame);

/*
*********************************************************************************************************
*                                       EXTERN GLOBAL VARIABLES
*********************************************************************************************************
*/

#ifdef _WIN32

/**
 * @brief This variables will obtain the operator of the windows timer.
 */

struct time_manage_timer_package_s time_mamage_windows_timer_package;

#endif // _WIN32

/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif // !__TIME_MANAGE_H