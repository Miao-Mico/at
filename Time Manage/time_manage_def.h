/*
*********************************************************************************************************
*                                               MODULE
*
* Note(s) : (1) This definition header file is protected from multiple pre-processor inclusion
*               through use of the definition module present pre-processor macro definition.
*********************************************************************************************************
*/

#ifndef __TIME_MANAGE_DEFINITION_H
#define __TIME_MANAGE_DEFINITION_H

/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <malloc.h>

#include "time_manage_cfg.h"

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
 * @brief This type is typedef of the time manage size type definition.
 */

typedef TIME_MANAGE_CFG_SIZE_TYPE time_manage_size_t;

/**
 * @brief This type is typedef of the time manage float type definition.
 */

typedef TIME_MANAGE_CFG_FLOAT_TYPE time_manage_float_t;

/**
 * @brief This typedef is the time manage hardware counter function.
 */

typedef void (*time_manage_timer_counter_t)(void *operator);

/**
 * @brief This type is the body of the time frame struct.
 */

typedef struct time_manage_visual_time_frame_s {
	time_manage_size_t year;
	time_manage_size_t month;
	time_manage_size_t week;
	time_manage_size_t day;
	time_manage_size_t hour;
	time_manage_size_t minue;
	time_manage_size_t second;
	time_manage_size_t millisec;
	time_manage_size_t microsec;
}time_manage_time_frame_st;

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

extern inline time_manage_size_t
time_manage_control_convert_timer_counter_tick_to_microsec(time_manage_float_t timer_ticks_per_microsec,
														   time_manage_size_t timer_freq,
														   time_manage_size_t counter_tick)
{
	assert(0 < timer_ticks_per_microsec || 0 < timer_freq);
	assert(0 < counter_tick);

	time_manage_size_t microsec = 0;

	if (0u == timer_ticks_per_microsec 
		&& 0u != timer_freq) {
		timer_ticks_per_microsec = 1000000.0f / timer_freq;
	}

	microsec = (time_manage_size_t)((time_manage_float_t)counter_tick * timer_ticks_per_microsec);

	return microsec;
}

/**
 * @brief This function will convert the microsecond value to the visual time frame struct.
 *
 * @param void
 *
 * @return void
 */

extern inline struct time_manage_visual_time_frame_s
time_manage_control_convert_microsec_to_visual_time_frame(time_manage_size_t microsec)
{
	assert(0 < microsec);

	struct time_manage_visual_time_frame_s time_frame = { 0 };

	return time_frame;
}

/**
 * @brief This function will convert the visual time frame struct to the microsecond value.
 *
 * @param void
 *
 * @return void
 */

extern inline time_manage_size_t
time_manage_control_convert_visual_time_frame_to_microsec(struct time_manage_visual_time_frame_s time_frame)
{
	assert(0 < time_frame.year ||
		   0 < time_frame.month ||
		   0 < time_frame.week ||
		   0 < time_frame.day ||
		   0 < time_frame.hour ||
		   0 < time_frame.minue ||
		   0 < time_frame.second ||
		   0 < time_frame.millisec ||
		   0 < time_frame.microsec);

	return 0u;
}

/*
*********************************************************************************************************
*                                       EXTERN GLOBAL VARIABLES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif // !__TIME_MANAGE_DEFINITION_H