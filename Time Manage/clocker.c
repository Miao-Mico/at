/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include "clocker.h"

#include "GeneralHashFunctions.h"

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

/**
 * @brief This type is the body of the clocker struct.
 */

struct clocker_s {
	struct {
		bool switch_status;

		time_manage_size_t record;

		time_manage_size_t timer_freq;
		time_manage_size_t clocker_freq;
		time_manage_size_t timer_func_cost;
		time_manage_float_t timer_tick_per_record;
	}info;

	struct time_manage_timer_package_s *timer_package;
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

#if (CLOCKER_CFG_INTERGRATED_STRUCTURE_MODE_EN)

struct clocker_control_s clocker_ctrl =
{
	{
		cloker_control_configuration_init,
		cloker_control_configuration_destroy,
		cloker_control_configuration_calibrate,
		cloker_control_configuration_change_timer_sources
	},
	{
		cloker_control_inquire_timer_freq,
		cloker_control_inquire_clocker_freq,
		cloker_control_inquire_visual_time_frame_type_record,
	},
	cloker_control_start,
	cloker_control_stop,

	#if (CLOCKER_CFG_HARDWARE_TIMER_EN)

	cloker_control_hardware_timer_counter

	#endif // (CLOCKER_CFG_HARDWARE_TIMER_EN)
};

#endif // (CLOCKER_CFG_INTERGRATED_STRUCTURE_MODE_EN)

/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            FUNCTIONS
*********************************************************************************************************
*/

/**
 * @brief This function will initialize the clocker.
 *
 * @param void
 *
 * @return void
 */

void cloker_control_configuration_init(struct clocker_s **clocker,
									  time_manage_size_t clocker_freq,
									  struct time_manage_timer_package_s *timer_package)
{
	assert(clocker);
	assert(timer_package);

	if (NULL == (*clocker = calloc(1, sizeof(struct clocker_s))) ||
		false == timer_package->verify_package(timer_package)) {
		return;
	}

	(*clocker)->info.switch_status = false;

	(*clocker)->info.record = 0u;
	(*clocker)->info.timer_freq =
		timer_package->inquire.timer_frequency(timer_package->timer_ptr);
	(*clocker)->info.clocker_freq = clocker_freq;
	(*clocker)->info.timer_func_cost = 0u;
	(*clocker)->info.timer_tick_per_record =
		(time_manage_float_t)clocker_freq / (*clocker)->info.timer_freq;

	(*clocker)->timer_package = timer_package;
}

/**
 * @brief This function will destroy the clocker.
 *
 * @param void
 *
 * @return void
 */

void cloker_control_configuration_destroy(struct clocker_s **clocker)
{
	assert(clocker);
	assert((*clocker));

	free((*clocker));

	(*clocker) = NULL;
}

/**
 * @brief This function will calibrate the clocker to perform better.
 *
 * @param void
 *
 * @return void
 */

void cloker_control_configuration_calibrate(struct clocker_s *clocker)
{
	assert(clocker);

	/** 						Explanation
	 *
	 *				inquire_point
	 *
	 *	----|-----------!-------|-----------!-------|------------------|-------->
	 *						time_func_cost ------>	0
	 *
	 */

	clocker->info.timer_func_cost = clocker->timer_package->inquire.	/* Get the interpolation of call the timer counter twice */
		timer_counter(clocker->timer_package->timer_ptr);
	clocker->info.timer_func_cost = clocker->timer_package->inquire.
		timer_counter(clocker->timer_package->timer_ptr) -
		clocker->info.timer_func_cost;
}

/**
 * @brief This function will change the timer of clocker.
 *
 * @param void
 *
 * @return void
 */

void cloker_control_configuration_change_timer_sources(struct clocker_s *clocker,
													  void *timer,
													  struct time_manage_timer_package_s *timer_package)
{
	assert(clocker);

	if (NULL == timer_package &&
		NULL == clocker->timer_package &&
		NULL == timer) {
		return;
	}

	if (timer_package != clocker->timer_package) {
		memcpy(clocker->timer_package, timer_package,
			   sizeof(struct time_manage_timer_package_s));
	}

	clocker->timer_package->timer_ptr = timer;
}

/**
 * @brief This function will inquire the frequency of the timer.
 *
 * @param void
 *
 * @return void
 */

extern inline time_manage_size_t
cloker_control_inquire_timer_freq(struct clocker_s *clocker)
{
	assert(clocker);

	return clocker->info.timer_freq;
}

/**
 * @brief This function will inquire the frequency of the clocker.
 *
 * @param void
 *
 * @return void
 */

extern inline time_manage_size_t
cloker_control_inquire_clocker_freq(struct clocker_s *clocker)
{
	assert(clocker);

	return clocker->info.clocker_freq;
}

/**
 * @brief This function will inquire the record of clocker with the type of visual time frame.
 *
 * @param void
 *
 * @return void
 */

struct time_manage_visual_time_frame_s cloker_control_inquire_visual_time_frame_type_record(struct clocker_s *clocker)
{
	assert(clocker);

	struct time_manage_visual_time_frame_s time_frame = { 0 };

	return time_frame;
}

/**
 * @brief This function will change the timer of clocker.
 *
 * @param void
 *
 * @return void
 */

void cloker_control_start(struct clocker_s *clocker)
{
	assert(clocker);

	clocker->info.record =
		clocker->timer_package->inquire.
		timer_counter(clocker->timer_package->timer_ptr);       /* Inquire the counter of the timer,
																	Assign it to the record_microsec */

	//char hash_string[40] = { 0 };
	//long long hash = 0;

	//sprintf_s(hash_string, sizeof(hash_string), "this time is %llu",
	//		  clocker->timer_package->inquire.timer_counter(clocker->timer_package->timer_ptr));

	//hash = RSHash(hash_string, sizeof(hash_string));

	//sprintf_s(hash_string, sizeof(hash_string), "time is %llu",
	//		  clocker->timer_package->inquire.timer_counter(clocker->timer_package->timer_ptr));

	//hash = RSHash(hash_string, sizeof(hash_string));

	clocker->info.switch_status = true;
}

/**
 * @brief This function will change the timer of clocker.
 *
 * @param void
 *
 * @return void
 */

time_manage_size_t cloker_control_stop(struct clocker_s *clocker)
{
	assert(clocker);

	time_manage_size_t tick_current =
		clocker->timer_package->inquire.
		timer_counter(clocker->timer_package->timer_ptr);							/* Inquire the tick of the timer counter */

	/** 						Explanation
	 *
	 *						tick_current [tick_current < record_microsec]
	 *				tick_current [tick_current > record_microsec]
	 *		record_microsec
	 *
	 *	--|---------!---$----|-----#---!--------|------------------|-------->
	 *					timer_freq	------>		0
	 *
	 * 	 	$		{***} -> tick_current - record_microsec
	 *		#		{**************} -> timer_freq - (record_microsec - tick_current)
	 */

	if (tick_current > clocker->info.record) {										/* If the tick_current > record_microsec,TAG:$ */
		clocker->info.record = tick_current - clocker->info.record;
	} else {																		/* If the tick_current > record_microsec,TAG:# */
		clocker->info.record = clocker->info.timer_freq -
			clocker->info.record - tick_current;
	}

	clocker->info.record =															/* Convert the time counter tick to the microsec */
		time_manage_control_convert_timer_counter_tick_to_microsec(clocker->info.timer_tick_per_record,
																   0, clocker->info.record);

	clocker->info.switch_status = false;

	return clocker->info.record;
}

#if (CLOCKER_CFG_HARDWARE_TIMER_EN)

/**
 * @brief This function will change the timer of clocker.
 *
 * @param void
 *
 * @return void
 */

extern inline void cloker_control_hardware_timer_counter(void *clocker)
{
	assert(clocker);

	struct clocker_s *_clocker = clocker;

	_clocker->info.record += _clocker->info.timer_tick_per_record;
}

#endif // (CLOCKER_CFG_HARDWARE_TIMER_EN)