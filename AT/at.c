/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include "at.h"

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
 * @brief This struct will contain all the universal vector functions address.
 */

struct at_information_s {
	bool switch_status;

	char transmit_end_with[AT_CFG_CACHE_LEVEL_AMT][AT_CFG_END_WITH_STRING_LEN];
};

/**
 * @brief This struct will contain all the universal vector functions address.
 */

struct at_device_package_s {
	void *device_ptr;

	void (*transmit)(void *device, void *data, size_t len);
	void (*receive)(void *device, void *data, size_t len);
	void (*interupt)(void *device, void *data, size_t len);
};

/**
 * @brief This struct will contain all the universal vector functions address.
 */

struct at_feedback_thread_s {
	struct at_feedback_thread_information_s {
		char *thread_name;													// ���� log
		float verify_pass_threshold_value;									// ��ֵ

		bool switch_status;													// ����
		int verify_pass;													// �Ƿ���֤ͨ��
		bool result_locked;													// 
		char(*verify_rule)[AT_CFG_FEEDBACK_VERIFY_THREAD_MAX_RULE_LENGTH];	// ��֤����
		short verify_amount;
	}info;

	void (*process)(struct AT_Feedback_Verify_Thread_Module *module_t);
	void (*verify_pass_callback)(void);										// �ص�����
	void (*verify_fail_callback)(void);										// �ص�����
};

/**
 * @brief This struct will contain all the universal vector functions address.
 */

struct at_feedback_s {
	short handle_priority;
	short amount_inited_thread;							// ��ʼ�����߳�
	short amount_max_thread;							// ����߳���

	struct at_feedback_thread_s *thread_ptr[AT_CFG_FEEDBACK_VERIFY_THREAD_AMOUNT];
	bool thread_actived[AT_CFG_FEEDBACK_VERIFY_THREAD_AMOUNT];
};

/**
 * @brief This struct will contain all the universal vector functions address.
 */

struct at_exception_s {
	void (*error)(void);
};

/**
 * @brief This struct will contain all the universal vector functions address.
 */

struct at_s {
    struct at_information_s info;

	struct at_device_package_s device;

	struct at_feedback_s feedback;

	struct at_exception_s exception;
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
