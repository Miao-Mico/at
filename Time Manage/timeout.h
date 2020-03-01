/*
*********************************************************************************************************
*                                               MODULE
*
* Note(s) : (1) This definition header file is protected from multiple pre-processor inclusion
*               through use of the definition module present pre-processor macro definition.
*********************************************************************************************************
*/

#ifndef __TIMEOUT_H
#define __TIMEOUT_H

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
 * @brief This type will control the timeout.
 */

struct timeout_control_s {
	struct {
		errno_t(*init)(struct timeout_s **timeout,
					   time_manage_size_t timeout_freq,
					   struct time_manage_timer_package_s *timer_package);

		errno_t(*destroy)(struct timeout_s **timeout);

		errno_t(*calibrate)(struct timeout_s *timeout);

		errno_t(*change_time_sources)(struct timeout_s *timeout,
									  void *timer,
									  struct time_manage_timer_package_s *timer_package);
	}configuration;

	void (*set)(struct timeout_s *timeout,
				time_manage_size_t sec, time_manage_size_t msec, time_manage_size_t usec);

	bool (*inquire)(struct timeout_s *timeout);
};

/*
*********************************************************************************************************
*								            FUNCTION PROTOTYPES
*********************************************************************************************************
*/

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

#endif // !__TIMEOUT_H