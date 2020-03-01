/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include "time_manage_timer_template.h"

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