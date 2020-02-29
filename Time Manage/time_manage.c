/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include "time_manage.h"

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

/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

#ifdef _WIN32

/**
 * @brief This function will inquire the frequency of the specified timer.
 *
 * @param void
 *
 * @return void
 */

time_manage_size_t time_manage_windows_timer_inquire_timer_frequency(void *timer);

/**
 * @brief This function will inquire the counter of the specified timer.
 *
 * @param void
 *
 * @return void
 */

time_manage_size_t time_manage_windows_timer_inquire_timer_counter(void *timer);

/**
 * @brief This function will verify if the windows timer package is valid.
 *
 * @param void
 *
 * @return void
 */

bool time_manage_windows_timer_verify_package(struct time_manage_timer_package_s *timer_package);

#endif // _WIN32

/*
*********************************************************************************************************
*					LOCAL GLOBAL VARIABLES & LOCAL FUNCTION PROTOTYPES INTERSECTION
*********************************************************************************************************
*/

#ifdef _WIN32

/**
 * @brief This variables will obtain the operator of the windows timer.
 */

struct time_manage_timer_package_s time_mamage_windows_timer_package = {
	.inquire.timer_frequency = time_manage_windows_timer_inquire_timer_frequency,
	.inquire.timer_counter = time_manage_windows_timer_inquire_timer_counter,
	.verify_package = time_manage_windows_timer_verify_package
};

#endif // _WIN32

/*
*********************************************************************************************************
*                                            FUNCTIONS
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
														   time_manage_size_t counter_tick)
{
	assert(0 < timer_ticks_per_microsec || 0 < timer_freq);
	assert(0 < counter_tick);

	time_manage_size_t microsec = 0;

	if (0 == timer_ticks_per_microsec) {
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

struct time_manage_visual_time_frame_s
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

time_manage_size_t
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

#ifdef _WIN32

/**
 * @brief This function will inquire the frequency of the specified timer.
 *
 * @param void
 *
 * @return void
 */

extern inline time_manage_size_t
time_manage_windows_timer_inquire_timer_frequency(void *timer)
{
	LARGE_INTEGER frequency = { 0 };

	if (QueryPerformanceFrequency(&frequency)) {
		return frequency.QuadPart;
	}

	return 0u;
}

/**
 * @brief This function will inquire the counter of the specified timer.
 *
 * @param void
 *
 * @return void
 */

extern inline time_manage_size_t
time_manage_windows_timer_inquire_timer_counter(void *timer)
{
	LARGE_INTEGER counter = { 0 };

	if (QueryPerformanceCounter(&counter)) {
		return counter.QuadPart;
	}

	return 0u;
}

/**
 * @brief This function will verify if the windows timer package is valid.
 *
 * @param void
 *
 * @return void
 */

extern inline bool
time_manage_windows_timer_verify_package(struct time_manage_timer_package_s *timer_package)
{
	assert(timer_package);

	if (NULL == timer_package->inquire.timer_frequency ||       /* Verify if the inquire part of the timer package is valid */
		NULL == timer_package->inquire.timer_counter) {
		goto FAIL;
	}

	return true;

FAIL:
	return false;
}

#endif // _WIN32