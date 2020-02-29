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

struct at_transmit_s {
	short cache_amt;
	short cache_level_amt;
	short crt_ist;
	short cache_level[AT_CFG_CACHE_AMT];
	char cache[AT_CFG_CACHE_AMT][AT_CFG_CACHE_LEVEL_AMT][100];
};

/**
 * @brief This struct will contain all the universal vector functions address.
 */

struct at_feedback_thread_s {
	struct at_feedback_thread_information_s {
		char *thread_name;													// 方便 log
		float verify_pass_threshold_value;									// 阈值

		bool switch_status;													// 开关
		int verify_pass;													// 是否验证通过
		bool result_locked;													//
		char(*verify_rule)[AT_CFG_FEEDBACK_VERIFY_THREAD_MAX_RULE_LENGTH];	// 验证规则
		short verify_amount;
	}info;

	void (*process)(struct AT_Feedback_Verify_Thread_Module *module_t);
	void (*verify_pass_callback)(void);										// 回调函数
	void (*verify_fail_callback)(void);										// 回调函数
};

/**
 * @brief This struct will contain all the universal vector functions address.
 */

struct at_feedback_s {
	short handle_priority;
	struct at_feedback_thread_s *handle_thread;

	short amount_inited_thread;							// 初始化的线程
	short amount_max_thread;							// 最大线程数

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

	struct at_device_package_s *device_package;

	struct at_transmit_s transmit;

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
//
//struct at_feedback_thread_s *at_feedback_verify_thread_multi_level_transmit = NULL;
//
//struct at_feedback_thread_s *at_feedback_verify_thread_idle = NULL;
//
//struct at_control_s at_ctrl = {
//	{
//		at_control_configuration_init,
//	},
//	{
//		NULL,
//	},
//	{
//		NULL,
//	}
//};
//
///*
//*********************************************************************************************************
//*                                      LOCAL FUNCTION PROTOTYPES
//*********************************************************************************************************
//*/
//
///**
// * @brief This function will deposit the instruction in the transmit cache.
// *
// * @param void
// *
// * @return void
// */
//
//void at_control_transmit_deposit_cache(struct at_s *at,
//									   char(**ist_pack)[100]);
//
///**
// * @brief This function will load the instruction in the transmit cache.
// *
// * @param void
// *
// * @return void
// */
//
//void at_control_transmit_load_cache(struct at_s *at,
//									short which_cache, short which_level, char **cache_pack);
//
///**
// * @brief This function will analysis the specified parameter.
// *
// * @param void
// *
// * @return void
// */
//
//short *at_control_transmit_param_info_analysis(const char *param_info);
//
///**
// * @brief This function will schedule the feedback thread.
// *
// * @param void
// *
// * @return void
// */
//
//void at_control_feedback_thread_scheduler(struct at_s *at);
//
///**
// * @brief This function will operate the specified thread verify the rule.
// *
// * @param void
// *
// * @return void
// */
//
//void at_control_feedback_thread_operator(struct at_s *at,
//										 struct at_feedback_thread_s *thread);
//
///**
// * @brief This function will do something seems the garbage collector will do.
// *
// * @param void
// *
// * @return void
// */
//
//void at_control_feedback_thread_garbage_collection(struct at_s *at,
//												   short thread_priority_in_service);
//
///**
// * @brief This function will verify the string which left hand side with the one right hand side.
// *
// * @param void
// *
// * @return void
// */
//
//float data_verify_percent(char *lhs, char *rhs, int sizeof_rule);     // 数据正确率;
///**
// * @brief This function will callback when the idle thread verify pass.
// *
// * @param void
// *
// * @return void
// */
//
//void at_control_feedback_verify_thread_idle_pass_callback(void);
//
///**
// * @brief This function will callback when the idle thread verify fail.
// *
// * @param void
// *
// * @return void
// */
//
//void at_control_feedback_verify_thread_idle_fail_callback(void);
//
///*
//*********************************************************************************************************
//*                                            FUNCTIONS
//*********************************************************************************************************
//*/
//
///**
// * @brief This function will initialize the at struct.
// *
// * @param void
// *
// * @return void
// */
//
//bool at_control_configuration_init(struct at_s **at,
//								  struct at_device_package_s *device_package)
//{
//	assert(at);
//	assert(device_package);
//
//	if (NULL == device_package ||																					/* If the device_package is not valid */
//		NULL == ((*at) = calloc(1, sizeof(struct at_s)))) {
//		//(*at)->exception.hardware_not_ready();
//
//		return false;
//	}
//
//	//stack_ctrl.configuration.init(&at_feedback_stack, FORWARD_LIST, 											/* Initialize the stack */
//	//							  AT_CFG_FEEDBACK_STACK_MAX_STACK_DEPTH,
//	//							  NULL, NULL);
//
//	at_control_feedback_init_thread(*at,																		/* Initialize the multi transmit thread */
//									&at_feedback_verify_thread_multi_level_transmit, "multi transmit",
//									at_control_feedback_verify_thread_idle_pass_callback,
//									at_control_feedback_verify_thread_idle_fail_callback);
//
//	at_control_feedback_init_thread(*at,																		/* Initialize the idle thread */
//									&at_feedback_verify_thread_idle, "idle",
//									at_control_feedback_verify_thread_idle_pass_callback,
//									at_control_feedback_verify_thread_idle_fail_callback);
//
//	at_control_feedback_config_thread(*at, 2,																	/* Config the thread */
//									  at_feedback_verify_thread_multi_level_transmit,
//									  AT_CFG_FEEDBACK_VERIFY_THREAD_MULTI_TRANSMIT_PRIORITY,
//									  at_feedback_verify_thread_idle,
//									  AT_CFG_FEEDBACK_VERIFY_THREAD_IDLE_PRIORITY);
//
//	at_control_feedback_request(*at, AT_CFG_FEEDBACK_VERIFY_THREAD_IDLE_PRIORITY, 1, " ");						/* Request the idle */
//
//	at_control_configuration_retarget(*at, at_device_ctrl.inquire.												/* Target the at to the device_package */
//									 package_device(device_package), NULL, device_package);
//
//	(*at)->feedback.amount_max_thread = AT_CFG_FEEDBACK_VERIFY_THREAD_AMOUNT;
//
//	(*at)->info.switch_status = true;
//
//	//(*at)->info.transmit_end_with[0] = "\r\n";
//
//	return true;
//}
//
///**
// * @brief This function will destroy the at struct.
// *
// * @param void
// *
// * @return void
// */
//
//void at_control_configuration_destroy(struct at_s **at)
//{
//	assert(at);
//	assert(*at);
//
//	free(*at);
//}
//
///**
// * @brief This function will retarget the device_package of at struct to the device_package specified.
// *
// * @param void
// *
// * @return void
// */
//
//void at_control_configuration_retarget(struct at_s *at,
//									  void *device,
//									  void *arg_list,
//									  struct at_device_package_s *device_package)
//{
//	assert(at);
//	assert(device);
//
//	if (NULL == device_package &&
//		NULL == (device_package = at->device_package)) {			/* If not appoint the device_package pack */
//		return;
//	}
//
//	if (NULL != at_device_ctrl.package_operator.verify.package &&	/* If device package is not valid */
//		!at_device_ctrl.package_operator.verify.package(device_package)) {
//		return;
//	}
//
//	if (device_package != at->device_package) {						/* If device_pack is not the same with the device_pack of at */
//		memcpy(at->device_package, device_package, at_device_ctrl.inquire.package_size());
//	}
//
//	at_device_ctrl.package_operator.configuration.mount(device_package,
//													   arg_list);
//}
//
///**
// * @brief This function will set the specified tail of the at transmit.
// *
// * @param void
// *
// * @return void
// */
//
//void at_control_configuration_transmit_tail(struct at_s *at,
//										   short level,
//										   char *tail, ...)
//{
//	va_list va_ptr;
//
//	char *current_level_info = { 0 };
//
//	if (level < 0 || level > AT_CFG_CACHE_LEVEL_AMT) {
//		//at.exception.at_config_error();
//
//		return;
//	}
//
//	current_level_info = tail;
//
//	va_start(va_ptr, tail);
//
//	for (size_t cnt = 0; cnt < level; cnt++) {
//		for (size_t ct = 0; current_level_info[ct] == '\0' || ct < AT_CFG_END_WITH_STRING_LEN; ct++)
//			at->info.transmit_end_with[cnt][ct] = current_level_info[ct];
//
//		current_level_info = (char *)va_arg(va_ptr, char *);
//	}
//
//	va_end(va_ptr);
//}
//
//// param_info = " param_level:ist_param_amt_level_1:ist_param_amt_level_2:... ";
//// at_control_transmit_multi_level_transmit("2:1:1","+MSG:","**","123456789");
//// at.info.transmit_end_with = "\r\n";
//// This template will output like :
////				Send <- "AT+MSG**\r\n"
////				Somewhere Need To Input,
////				Send <- "123456789"
////	Here AT Instruction End , Send With Twice .
//
///**
// * @brief This function will generate the instruction according the giving specified parameter.
// *
// * @param void
// *
// * @return void
// */
//
//void at_control_transmit_multi_level_generate(struct at_s *at,
//											  char *param_info, char *ist, ...)
//{
//	char
//		*crt_pack[20] = { "AT" },
//		tail[100] = { 0 },
//		format[10][100] = { "%s" }, // "HEAD" ...
//		(*pack)[100] = NULL;
//
//	short
//		amount_param = 0,
//		*level_info = { 0 };
//
//	va_list va_ptr = { 0 };
//
//	at_control_transmit_deposit_cache(at, &pack);
//
//	level_info = at_control_transmit_param_info_analysis(param_info); // 获取各级参数信息
//
//	for (short cnt = 1; cnt < level_info[0] + 1; cnt++) // 获取参数总个数
//		amount_param += level_info[cnt];
//
//	va_start(va_ptr, ist);
//
//	crt_pack[1] = ist;
//
//	for (short cnt = 2; cnt < amount_param + 2 - 1; cnt++) // 获取全部可变参数，去掉第一个 ist
//		crt_pack[cnt] = (char *)va_arg(va_ptr, char *);
//
//	for (short cnt = 0, amt_pack = 0, amt_tail = 0; cnt < level_info[0]; cnt++) // 生成各级指令
//	{
//		//for (short ct = 0; ct < level_info[cnt + 1]; ct++) // 生成本级格式控制符
//		//	strcat(format[cnt], "%s");
//
//		//for (short ct = 0; crt_pack[level_info[cnt + 1] + amt_tail][ct] != '\0'; ct++)
//		//	tail[ct] = crt_pack[level_info[cnt + 1] + amt_tail][ct];
//
//		//strcat(tail, at->info.transmit_end_with[cnt]);
//		//crt_pack[level_info[cnt + 1] + amt_tail] = tail;
//
//		//sprintf(pack[cnt], format[cnt],
//		//		crt_pack[amt_pack + 0], crt_pack[amt_pack + 1], crt_pack[amt_pack + 2],
//		//		crt_pack[amt_pack + 3], crt_pack[amt_pack + 4], crt_pack[amt_pack + 5],
//		//		crt_pack[amt_pack + 6], crt_pack[amt_pack + 7], crt_pack[amt_pack + 8]);
//
//		if (cnt == 0) // 从 ist 位开始计数，补到 ist 位即 crt_pack[1]
//			amt_pack += 1;
//
//		amt_pack += amt_tail += level_info[cnt + 1];
//
//		memset(tail, '\0', strlen(tail));
//	}
//}
//
///**
// * @brief This function will transmit the instruction
// *			who is the string stored in the specified level of the at transmit buffer.
// *
// * @param void
// *
// * @return void
// */
//
//void at_control_transmit_multi_level_transmit(struct at_s *at,
//											  short level)
//{
//	if (!at->info.switch_status)
//		return;
//
//	void *cache = NULL;
//
//	at_control_transmit_load_cache(at, 1, 0, &cache);
//
//	at_device_control_package_operator_transmit_send(at->device_package,		/* Send the data to the device */
//													 cache, strlen(cache));
//}
//
///**
// * @brief This function will generate and transmit the instruction according the giving specified parameter.
// *
// * @param void
// *
// * @return void
// */
//
//void at_control_transmit_single_level_transmit(struct at_s *at,
//											   short param_amt, char *ist, ...)
//{
//	va_list va_ptr = { 0 };
//
//	char
//		param_info[] = { '1', ':', param_amt + '0' },
//		*param[10] = { 0 };
//
//	va_start(va_ptr, ist);
//
//	param[0] = ist;
//
//	for (size_t cnt = 1; cnt < param_amt; cnt++)
//		param[cnt] = (char *)va_arg(va_ptr, char *);
//
//	at_control_transmit_multi_level_generate(at, param_info,
//											 param[0], param[1], param[2], param[3],
//											 param[4], param[5], param[6], param[7],
//											 param[8], param[9]);
//
//	at_control_transmit_multi_level_transmit(at, 0);
//
//	va_end(va_ptr);
//}
//
///**
// * @brief This function will initialize the feedback thread.
// *
// * @param void
// *
// * @return void
// */
//
//bool at_control_feedback_init_thread(struct at_s *at,
//									 struct at_feedback_thread_s **thread,
//									 char *name, void (*verify_pass_callback)(void),
//									 void (*verify_fail_callback)(void))
//{
//	if (NULL == thread ||
//		(NULL == verify_pass_callback && NULL == verify_fail_callback) ||
//		at->feedback.amount_inited_thread >= at->feedback.amount_max_thread)			// 超过最大线程
//	{
//		//at_feedback_verify_thread_init_error();
//
//		return false;
//	}
//
//	char(*verify_rule_ptr)[AT_CFG_FEEDBACK_VERIFY_THREAD_MAX_RULE_LENGTH] = (char(*)[100])calloc(1, sizeof(char(*)[100]));
//
//	struct at_feedback_thread_s *
//		thread_new = (struct at_feedback_thread_s *)calloc(1, sizeof(struct at_feedback_thread_s));
//
//	if (NULL == thread_new ||
//		NULL == verify_rule_ptr) {
//		//at_feedback_verify_thread_init_error();
//
//		return false;
//	}
//
//	thread_new->info.thread_name = name;
//	thread_new->info.switch_status = false;
//	thread_new->info.verify_pass_threshold_value = 1.0f;
//
//	thread_new->info.verify_rule = verify_rule_ptr;
//
//	thread_new->process = at_control_feedback_thread_operator;
//
//	thread_new->verify_pass_callback = verify_pass_callback;
//	thread_new->verify_fail_callback = verify_fail_callback;
//
//	*thread = thread_new;
//
//	at->feedback.amount_inited_thread++;
//
//	return true;
//}
//
///**
// * @brief This function will free the feedback thread.
// *
// * @param void
// *
// * @return void
// */
//
//void at_control_feedback_free_thread(struct at_s *at,
//									 struct at_feedback_thread_s **thread)
//{	// AT Feedback Verify Thread - 释放进程
//	free((*thread)->info.verify_rule);
//
//	free(*thread);
//}
//
///**
// * @brief This function will config the feedback thread.
// *
// * @param void
// *
// * @return void
// */
//
//void at_control_feedback_config_thread(struct at_s *at,
//									   short cfg_groups_amount,
//									   struct at_feedback_thread_s *thread,
//									   short priority_module, ...)
//{	// AT Feedback Verify Thread - 设置优先级对应的处理函数
//	va_list va_ptr;
//
//	if (NULL == thread)
//		return;
//
//	short
//		priority_temp = priority_module;
//
//	if (cfg_groups_amount >= AT_CFG_FEEDBACK_VERIFY_THREAD_AMOUNT) {
//		//at->exception.at_feedback_verify_thread_config_error();
//
//		return;
//	}
//
//	va_start(va_ptr, priority_module);
//
//	for (size_t i = 0; i < cfg_groups_amount; i++) {
//		at->feedback.thread_ptr[priority_temp] = thread;
//
//		if (cfg_groups_amount - 1 == i)
//			break;
//
//		thread = (struct at_feedback_thread_s *)va_arg(va_ptr, int);
//		priority_temp = (short)va_arg(va_ptr, int);									// 获取可变参数
//	}
//
//	va_end(va_ptr);
//}
//
///**
// * @brief This function will config the callback of feedback thread.
// *
// * @param void
// *
// * @return void
// */
//
//void at_control_feedback_set_thread_callback(struct at_s *at,
//											 short priority_of_thread,
//											 bool pass, bool fail, ...)
//{
//	if (NULL == priority_of_thread || false == pass == fail)
//		return;
//
//	struct at_feedback_thread_s *
//		module_ptr = at->feedback.thread_ptr[priority_of_thread];
//
//	short
//		config_amount = pass + fail,
//		mode[2] = { pass,fail + 10 };
//
//	va_list va_ptr;
//
//	if (config_amount == 0)
//		return;
//
//	va_start(va_ptr, fail);
//
//	for (size_t i = 0; i < config_amount; i++) {
//		switch (mode[i]) {
//			case 1:
//				module_ptr->verify_pass_callback = (void(*)(void))va_arg(va_ptr, int);
//				break;
//			case 11:
//				module_ptr->verify_fail_callback = (void(*)(void))va_arg(va_ptr, int);
//				break;
//			default:
//				break;
//		}
//	}
//
//	va_end(va_ptr);
//}
//
///**
// * @brief This function will request the specified thread verify the rule.
// *
// * @param void
// *
// * @return void
// */
//
//bool at_control_feedback_request(struct at_s *at,
//								 struct at_feedback_thread_s *thread,
//								 at_size_t rule_amount,
//								 char *rule, ...)
//{
//	assert(thread);
//
//	if (rule_amount == 0 || rule == NULL)
//		return false;
//
//	va_list va_ptr;
//
//	va_start(va_ptr, rule);
//
//	char *rule_ptr = rule;
//
//	for (size_t i = 0; i < rule_amount; i++) {
//		#ifdef AT_CFG_DEBUG_EN
//
//		printf("get rule : %s \r\n", rule_ptr);
//
//		#endif
//
//		memset(thread->info.verify_rule, ' ',
//			   strlen(thread->info.verify_rule[i]));					// 清空验证规则
//
//		memcpy(thread->info.verify_rule[i], rule_ptr, strlen(rule_ptr));
//
//		if (rule_amount - 1 != i)
//			rule_ptr = (char *)va_arg(va_ptr, char *);
//	}
//
//	thread->info.verify_amount = rule_amount;
//	thread->info.verify_pass = false;							// 置位验证通过标志位
//	thread->info.result_locked = true;							// 确认锁定验证结果
//
//	thread->info.switch_status = true;							// 打开开关，读取成功时，会自动关掉
//
//	va_end(va_ptr);
//
//	return true;
//}
//
///**
// * @brief This function will inquire the specified thread the result of verification.
// *
// * @param void
// *
// * @return void
// */
//
//int at_control_feedback_inquire(struct at_s *at,
//								struct at_feedback_thread_s *thread,
//								short priority_of_thread, bool unlock_result)
//{
//	unsigned int verify = thread->info.verify_pass;
//
//	if (0 < verify &&
//		false == thread->info.result_locked) {
//		#ifdef AT_CFG_DEBUG_EN
//
//		printf("inquire:%d \r\n", verify);
//
//		#endif
//
//		thread->info.switch_status = false;						// 自动关闭开关
//		thread->info.verify_pass = 0x00000000;					// 置位标志位
//	}
//
//	return verify;
//}
//
///**
// * @brief This function will set the priority of feedback.
// *
// * @param void
// *
// * @return void
// */
//
//static inline void at_control_feedback_set_priority(struct at_s *at,
//													short priority)
//{	// AT Feedback - 设置当前为何优先级
//	if (at->feedback.handle_priority <= priority)								// 当前优先级小于或等于设定值
//		return;
//
//	at->feedback.handle_priority = priority;									// 设置优先级
//}
//
///**
// * @brief This function will unlock the specified thread the result of verification.
// *
// * @param void
// *
// * @return void
// */
//
//void at_control_feedback_unlock_result(struct at_s *at,
//									   short priority_of_thread)
//{
//	struct at_feedback_thread_s *
//		module_ptr = at->feedback.thread_ptr[priority_of_thread];
//
//	module_ptr->info.result_locked = false;
//}
//
///**
// * @brief This function will set the verify pass threshold value of specified thread.
// *
// * @param void
// *
// * @return void
// */
//
//static inline void
//at_control_feedback_set_verify_pass_threshold_value(struct at_s *at,
//													short priority_of_thread, float value)
//{
//	struct at_feedback_thread_s *
//		thread_ptr = at->feedback.thread_ptr[priority_of_thread];
//
//	if (NULL != thread_ptr)
//		thread_ptr->info.verify_pass_threshold_value = value;
//}
//
///**
// * @brief This function will analysis the specified parameter.
// *
// * @param void
// *
// * @return void
// */
//
//short *at_control_transmit_param_info_analysis(const char *param_info)
//{	// 解析生成多级指令时的传入控制字符
//	short
//		info_cnt = 0,
//		result_cnt = 0,
//		*result = (short *)calloc(param_info[0] + 1 - '0', sizeof(short));
//
//	if (result == NULL)
//		return result;
//
//	while (param_info[info_cnt] != '\0') {
//		if (param_info[info_cnt] == ':')
//			result[result_cnt++] = param_info[info_cnt - 1] - '0';
//
//		info_cnt++;
//	}
//
//	result[result_cnt] = param_info[info_cnt - 1] - '0';
//
//	return result;
//}
//
///**
// * @brief This function will deposit the instruction in the transmit cache.
// *
// * @param void
// *
// * @return void
// */
//
//void at_control_transmit_deposit_cache(struct at_s *at,
//									   char(**ist_pack)[100]) // 存入 cache
//{	// 往 AT 指令缓存 存入
//	short
//		*crt_cache = &at->transmit.crt_ist;
//
//	if (*crt_cache < at->transmit.cache_amt - 1)
//		(*crt_cache)++;
//	else
//		(*crt_cache) = 0;
//
//	*ist_pack = at->transmit.cache[(*crt_cache)];
//}
//
///**
// * @brief This function will load the instruction in the transmit cache.
// *
// * @param void
// *
// * @return void
// */
//
//void at_control_transmit_load_cache(struct at_s *at,
//									short which_cache, short which_level,
//									char **cache_pack) // 读取 cache
//{	// 从 AT 指令缓存 读出
//	short
//		cache_num = which_cache,
//		level_num = which_level;
//
//	if (which_cache < 0) // 取当前命令往前隔 which_cache 个的缓存
//	{
//		cache_num = at->transmit.crt_ist + which_cache;
//		level_num = at->transmit.cache_level[cache_num];
//
//		if (cache_num < 0)
//			cache_num += at->transmit.cache_amt;
//	}
//
//	*cache_pack = at->transmit.cache[cache_num][level_num];
//}
//
///**
// * @brief This function will handle all the hardware interrupt of the device_package.
// *
// * @param void
// *
// * @return void
// */
//
//void at_feedback_hardware_irqn(struct at_s *at)
//{
//	static char
//		crt_num = 0;
//	static char
//		data_pack[100] = { 0 };
//
//	//data_pack[crt_num] = at->device_package->interrupt(); //记录
//
//	#ifdef AT_CFG_DEBUG_EN
//
//	printf("hard irqn get data : %s \r\n", data_pack);
//
//	#endif
//
//	//stack_ctrl.modifiers.push(at_feedback_stack, data_pack);			// 把完整语句写入栈
//
//	for (; crt_num > 0; crt_num--)														// 清空数据和当前位数
//		data_pack[crt_num] = 0;
//}
//
///**
// * @brief This function will schedule the feedback thread.
// *
// * @param void
// *
// * @return void
// */
//
//void at_control_feedback_thread_scheduler(struct at_s *at)
//{	// AT Feedback - 优先级处理函数
//	static bool threads_running = false;
//
//	for (short thread_priority = 0; thread_priority < AT_CFG_FEEDBACK_VERIFY_THREAD_AMOUNT; thread_priority++) {
//		if (true == threads_running)
//			at_control_feedback_thread_garbage_collection(at, thread_priority);
//
//		if (at->feedback.handle_priority > thread_priority ||	// 当前优先级较小的
//			at->feedback.thread_ptr[thread_priority] == NULL ||	// 未初始化的
//			at->feedback.thread_ptr[thread_priority]->info.switch_status == false ||	// 未打开的
//			at->feedback.thread_ptr[thread_priority]->info.verify_amount <= 0 ||	// 无判断规则的
//			at->feedback.thread_ptr[thread_priority]->info.verify_rule[0][0] == ' ' ||	// 未赋值规则的
//			0) {
//			threads_running = false;
//
//			continue;
//		}
//
//		at_control_feedback_thread_operator(at, at->feedback.thread_ptr[thread_priority]);		// 执行当前优先级线程的进程
//
//		threads_running = true;														// 有线程在跑
//
//		if (AT_CFG_FEEDBACK_VERIFY_THREAD_ONE_INTERUPT_ONE_THREAD_MODE == true)			// 是不是一次中断事件，只进行一个判断线程
//			return;																		// 是，退出，无垃圾回收
//		else
//			continue;																	// 否，继续
//	}
//}
//
///**
// * @brief This function will operate the specified thread verify the rule.
// *
// * @param void
// *
// * @return void
// */
//
//void at_control_feedback_thread_operator(struct at_s *at,
//										 struct at_feedback_thread_s *thread)
//{	// AT Feedback Verify Thread - module 进程
//	char data[100] = { 0 };
//
//	float perc = 0;
//
//	//if (true != stack_ctrl.capacity.empty(at_feedback_stack))					// 队列有数据
//	//	stack_ctrl.element_access.top(at_feedback_stack);							// 从队列读取
//
//	if (!thread->info.switch_status) {
//		//at->exception.at_feedback_verify_thread_process_error();
//
//		return;
//	}
//
//	if ('\0' == data[0] || 					// 非字符串时
//		' ' == data[0])
//		return;
//
//	#ifdef AT_CFG_DEBUG_EN
//
//	printf("get data from stack : [ %s ] \r\n", data);
//
//	#endif
//
//	for (size_t i = 0; i < thread->info.verify_amount; i++) {
//		if ((perc = data_verify_percent(data, thread->info.verify_rule[i], strlen(thread->info.verify_rule[i]))) >=
//			thread->info.verify_pass_threshold_value) {
//			memset(thread->info.verify_rule, ' ',
//				   strlen(thread->info.verify_rule[i]));					// 清空验证规则
//
//			thread->info.verify_amount--;								// 减少一个条件记录
//
//			thread->info.verify_pass |= 0x00000001 << i;				// 相应位置写入 1
//		}
//	}
//
//	if (0 < (unsigned int)thread->info.verify_pass) {
//		#ifdef AT_CFG_DEBUG_EN
//
////		printf("[ thread name:%s | %s %d]\r\n",thread->info.thread_name,data,chain_stack_ctrl.get_stack_info.current_depth(at_feedback_stack));
//
//		#endif
//
//		thread->verify_pass_callback();
//
//		//stack_ctrl.modifiers.pop(at_feedback_stack);						// 验证成功，从队列删掉
//	} else {
//		#ifdef AT_CFG_DEBUG_EN
//
//		printf("\r\n\r\n perc[ %s ]:%f \r\n\r\n", thread->info.verify_rule[0], perc);
//
//		#endif
//
//		thread->verify_fail_callback();
//
//		thread->info.verify_pass = 0x00000000;
//	}
//
//	(void)perc;
//}
//
///**
// * @brief This function will do something seems the garbage collector will do.
// *
// * @param void
// *
// * @return void
// */
//
//void at_control_feedback_thread_garbage_collection(struct at_s *at,
//												   short thread_priority_in_service)
//{	// 回收堆栈空间
//	static bool is_thread_in_service[AT_CFG_FEEDBACK_VERIFY_THREAD_AMOUNT] = { 0 };
//	static int this_msg_loop_cnt = 0;
//	static int
//		stack_depth = 0,
//		stack_depth_prew = 0;
//
//	//if (stack_depth_prew != (stack_depth = stack_ctrl.capacity.size(at_feedback_stack)) ||
//	//	0 == stack_depth) {
//	//	stack_depth_prew = stack_depth;		// 记录本次
//
//	//	return;								// 退出
//	//}
//
//	is_thread_in_service[thread_priority_in_service] = true;
//
//	for (size_t i = 0; i < AT_CFG_FEEDBACK_VERIFY_THREAD_AMOUNT; i++)
//		this_msg_loop_cnt += is_thread_in_service[thread_priority_in_service];
//
//	if (this_msg_loop_cnt >= at->feedback.amount_inited_thread) {
//		#ifdef AT_CFG_DEBUG_EN
//
//		char *data = NULL;
//
//		//data = stack_ctrl.element_access.top(at_feedback_stack);
//
//		printf("delete depth %d rule : %s\r\n", stack_depth, data);
//
//		#endif
//
//		//stack_ctrl.modifiers.pop(at_feedback_stack);
//
//		for (size_t i = 0; i < AT_CFG_FEEDBACK_VERIFY_THREAD_AMOUNT; i++) {
//			is_thread_in_service[thread_priority_in_service] = false;
//		}
//	}
//}
//
///**
// * @brief This function will verify the string which left hand side with the one right hand side.
// *
// * @param void
// *
// * @return void
// */
//
//float data_verify_percent(char *lhs, char *rhs, int sizeof_rule)     // 数据正确率
//{	// 验证数据，返回小数[0.0-1.0]
//	float
//		percent = 0;
//	int
//		correct_num = 0;
//
//	if (*rhs == '\0')
//		return true;
//
//	for (size_t cnt = 0; cnt < sizeof_rule; cnt++) {
//		if (lhs[cnt] == rhs[cnt]) {
//			correct_num++;
//		}
//	}
//
//	percent = correct_num / (float)sizeof_rule;
//
//	return percent;
//}
//
///**
// * @brief This function will callback when the idle thread verify pass.
// *
// * @param void
// *
// * @return void
// */
//
//void at_control_feedback_verify_thread_idle_pass_callback(void)
//{
//	printf("\r\n pass :%x \r\n\r\n", at_ctrl.feedback.inquire(4, false));
//
//	at_ctrl.feedback.set_priority(AT_CFG_FEEDBACK_VERIFY_THREAD_IDLE_PRIORITY);		// 恢复优先级
//}
//
///**
// * @brief This function will callback when the idle thread verify fail.
// *
// * @param void
// *
// * @return void
// */
//
//void at_control_feedback_verify_thread_idle_fail_callback(void)
//{
//	at_ctrl.feedback.set_priority(AT_CFG_FEEDBACK_VERIFY_THREAD_IDLE_PRIORITY);		// 恢复优先级
//}

//void at_feedback_verify_thread_init_error(void)
//{
//	while (1);
//}

void hardware_not_ready(void)
{
	while (1);
}

void at_config_error(void)
{
	while (1);
}

void at_feedback_verify_thread_config_error(void)
{
	while (1);
}

void at_feedback_verify_thread_process_error(void)
{
	while (1);
}