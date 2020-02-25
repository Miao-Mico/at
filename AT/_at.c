#include "at.h"

#include "usart.h"
#include "dma.h"

#include "stack.h"
#include "queue.h"


#define DEBUG_MODE					1

#define AT_FUNCTION_SWITCH_ASSERT		1

typedef struct AT_Transmit_Cache
{
	short cache_amt;
	short cache_level_amt;
	short crt_ist;
	short cache_level[AT_CACHE_AMT];
	char cache[AT_CACHE_AMT][AT_CACHE_LEVEL_AMT][100];
}AT_TRANSMIT_CACHE_TYPEEF,*AT_TRANSMIT_CACHE_TYPEEF_PTR;

typedef struct AT_Feedback_Verify_Thread_Module
{
	struct AT_Verify_Instruction_Feedback_Info
	{
		char* thread_name;											// 方便 log
		float verify_pass_threshold_value;							// 阈值
		
		bool switch_status;											// 开关
		int verify_pass;											// 是否验证通过
		bool result_locked;											// 
		char (*verify_rule)[AT_FEEDBACK_VERIFY_THREAD_CFG_MAX_RULE_LENGTH];	// 验证规则
		short verify_amount;
	}info;
	
	void (*process)(struct AT_Feedback_Verify_Thread_Module* module_t);							
	void (*verify_pass_callback)(void);			// 回调函数
	void (*verify_fail_callback)(void);			// 回调函数
}AT_FEEDBACK_VERIFY_THREAD_MODULE_TYPEDEF;

typedef struct AT_Device
{
	USART_TypeDef* usart_at;
}AT_DEVICE_TYPEDEF;

typedef struct AT_Info
{
	bool switch_status;
	
	char transmit_end_with[AT_CACHE_LEVEL_AMT][AT_END_WITH_STRING_LEN];
} AT_INFO_TYPEDEF;

typedef struct AT_FeedBack
{
	short handle_priority;
	short amount_inited_thread;							// 初始化的线程
	short amount_max_thread;							// 最大线程数
	
	AT_FEEDBACK_VERIFY_THREAD_MODULE_TYPEDEF_PTR thread_ptr[AT_FEEDBACK_VERIFY_THREAD_CFG_AMOUNT];
	bool thread_actived[AT_FEEDBACK_VERIFY_THREAD_CFG_AMOUNT];
} AT_FEEDBACK_TYPEDEF, * AT_FEEDBACK_TYPEDEF_PTR;

typedef struct AT_Exception
{
	void (*hardware_not_ready)(void);
	void (*at_config_error)(void);
	void (*at_feedback_verify_thread_config_error)(void);
	void (*at_feedback_verify_thread_process_error)(void);
}AT_EXCEPTION_TYPEDEF;

typedef struct AT
{
	AT_INFO_TYPEDEF info;
	
	AT_DEVICE_TYPEDEF device;
	
	AT_TRANSMIT_CACHE_TYPEEF transmit_cache;
	
	AT_FEEDBACK_TYPEDEF feedback;

	AT_EXCEPTION_TYPEDEF exception;
}AT_TYPEDEF;


void at_feedback_verify_thread_process(AT_FEEDBACK_VERIFY_THREAD_MODULE_TYPEDEF_PTR module_t);
void at_feedback_verify_thread_idle_pass_callback(void);
void at_feedback_verify_thread_idle_fail_callback(void);

void hardware_not_ready(void);
void at_config_error(void);
void at_feedback_verify_thread_init_error(void);
void at_feedback_verify_thread_config_error(void);
void at_feedback_verify_thread_process_error(void);

AT_TYPEDEF at = 
{
	{
		0
	},
	{
		0
	},
	{
		AT_CACHE_AMT,
		AT_CACHE_LEVEL_AMT,
		AT_CACHE_AMT,
		{},
		{},
//		at_ist_cache_deposit,
//		at_ist_cache_load
	},
	{
		0
	},
	{
		hardware_not_ready,
		at_config_error,
		at_feedback_verify_thread_config_error,
		at_feedback_verify_thread_process_error
	}
};

#define UART_TRANSMIT(format, ...) printf(format, ##__VA_ARGS__)

AT_FEEDBACK_VERIFY_THREAD_MODULE_TYPEDEF_PTR at_feedback_verify_thread_multi_level_transmit = NULL;
AT_FEEDBACK_VERIFY_THREAD_MODULE_TYPEDEF_PTR at_feedback_verify_thread_idle = NULL;

CHAIN_STACK_TYPEDEF_PTR at_feedback_stack = NULL; 								// AT 链栈

bool at_config_mount(USART_TypeDef* usart_t)
{
	if(NULL == usart_t)	// 如果传入串口不对
	{
		at.exception.hardware_not_ready();

		return false;
	}
	
	at.feedback.amount_max_thread = AT_FEEDBACK_VERIFY_THREAD_CFG_AMOUNT;
	
	chain_stack_ctrl.init(&at_feedback_stack,AT_FEEDBACK_STACK_CFG_MAX_STACK_DEPTH);				// 初始化堆栈

	at_ctrl.feedback.init_thread(&at_feedback_verify_thread_multi_level_transmit,
		"multi transmit",at_feedback_verify_thread_idle_pass_callback,at_feedback_verify_thread_idle_fail_callback);
	at_ctrl.feedback.init_thread(&at_feedback_verify_thread_idle,
		"idle",at_feedback_verify_thread_idle_pass_callback,at_feedback_verify_thread_idle_fail_callback);

	at_ctrl.feedback.config_thread(2,																	// 设置AT Feedback Thread 优先级及其结构体
		at_feedback_verify_thread_multi_level_transmit,AT_FEEDBACK_VERIFY_THREAD_MULTI_TRANSMIT_PRIORITY,
		at_feedback_verify_thread_idle,AT_FEEDBACK_VERIFY_THREAD_IDLE_PRIORITY);
	
	at_ctrl.feedback.verify_thread.request(AT_FEEDBACK_VERIFY_THREAD_IDLE_PRIORITY,1," ");				// 请求空闲任务
	
	at.device.usart_at = usart_t;
	
//	usart.fill_irqn_info(usart_t, 0, USART_IT_RXNE, at.feedback.hardware_irqn);						// 配置
	
	at.info.switch_status = true;
	
	return true;
}

void at_config_transmit_end_with(short level, char* transmit_end_with, ...)
{	// 配置 AT 指令后面跟随的字符，例如 at + Instruction + [ \r\n ]
	va_list va_ptr;

	char* current_level_info = { 0 };

	if (level < 0 || level > AT_CACHE_LEVEL_AMT)
	{
		at.exception.at_config_error();

		return;
	}

	current_level_info = transmit_end_with;

	va_start(va_ptr, transmit_end_with);

	for (size_t cnt = 0; cnt < level; cnt++)
	{
		for (size_t ct = 0; current_level_info[ct] == '\0' || ct < AT_END_WITH_STRING_LEN; ct++)
			at.info.transmit_end_with[cnt][ct] = current_level_info[ct];

		current_level_info = (char*)va_arg(va_ptr, char*);
	}

	va_end(va_ptr);
}

short* at_param_info_analysis(const char* param_info)
{	// 解析生成多级指令时的传入控制字符
	short
		info_cnt = 0,
		result_cnt = 0,
		*result = (short*)calloc(param_info[0] + 1 - '0', sizeof(short));

	if (result == NULL)
		return result;

	while (param_info[info_cnt] != '\0')
	{
		if (param_info[info_cnt] == ':')
			result[result_cnt++] = param_info[info_cnt - 1] - '0';

		info_cnt++;
	}

	result[result_cnt] = param_info[info_cnt - 1] - '0';

	return result;
}

void at_ist_cache_deposit(char(**ist_pack)[100]) // 存入 cache
{	// 往 AT 指令缓存 存入
	short
		* crt_cache = &at.transmit_cache.crt_ist;

	if (*crt_cache < at.transmit_cache.cache_amt - 1)
		(*crt_cache)++;
	else
		(*crt_cache) = 0;

	*ist_pack = at.transmit_cache.cache[(*crt_cache)];
}

void at_ist_cache_load(short which_cache, short which_level, char** cache_pack) // 读取 cache
{	// 从 AT 指令缓存 读出
	short
		cache_num = which_cache,
		level_num = which_level;

	if (which_cache < 0) // 取当前命令往前隔 which_cache 个的缓存
	{
		cache_num = at.transmit_cache.crt_ist + which_cache;
		level_num = at.transmit_cache.cache_level[cache_num];

		if (cache_num < 0)
			cache_num += at.transmit_cache.cache_amt;
	}

	*cache_pack = at.transmit_cache.cache[cache_num][level_num];
}

// param_info = " param_level:ist_param_amt_level_1:ist_param_amt_level_2:... ";
// at_instruction_transmit("2:1:1","+MSG:","**","123456789");
// at.info.transmit_end_with = "\r\n";
// This Tamplate Will output like :
//				Send <- "AT+MSG**\r\n"
//				Somewhere Need To Input,
//				Send <- "123456789"
//	Here AT Instruction End , Send With Twice .

void at_instruction_generate(char* param_info, char* ist, ...)
{	// AT 生成指令
	char
		* crt_pack[20] = { "AT" },
		tail[100] = { 0 },
		format[10][100] = { "%s" }, // "HEAD" ... 
		(*pack)[100] = NULL;

	short
		amount_param = 0,
		*level_info = { 0 };

	va_list va_ptr = { 0 };

	at_ist_cache_deposit(&pack);

	level_info = at_param_info_analysis(param_info); // 获取各级参数信息

	for (short cnt = 1; cnt < level_info[0] + 1; cnt++) // 获取参数总个数
		amount_param += level_info[cnt];

	va_start(va_ptr, ist);

	crt_pack[1] = ist;

	for (short cnt = 2; cnt < amount_param + 2 - 1; cnt++) // 获取全部可变参数，去掉第一个 ist
		crt_pack[cnt] = (char*)va_arg(va_ptr, char*);

	for (short cnt = 0, amt_pack = 0, amt_tail = 0; cnt < level_info[0]; cnt++) // 生成各级指令
	{
		for (short ct = 0; ct < level_info[cnt + 1]; ct++) // 生成本级格式控制符
			strcat(format[cnt], "%s");

		for (short ct = 0; crt_pack[level_info[cnt + 1] + amt_tail][ct] != '\0'; ct++)
			tail[ct] = crt_pack[level_info[cnt + 1] + amt_tail][ct];

		strcat(tail, at.info.transmit_end_with[cnt]);
		crt_pack[level_info[cnt + 1] + amt_tail] = tail;

		sprintf(pack[cnt], format[cnt],
			crt_pack[amt_pack + 0], crt_pack[amt_pack + 1], crt_pack[amt_pack + 2],
			crt_pack[amt_pack + 3], crt_pack[amt_pack + 4], crt_pack[amt_pack + 5],
			crt_pack[amt_pack + 6], crt_pack[amt_pack + 7], crt_pack[amt_pack + 8]); // 一级命令最多传入 8 个参数

		if (cnt == 0) // 从 ist 位开始计数，补到 ist 位即 crt_pack[1]
			amt_pack += 1;

		amt_pack += amt_tail += level_info[cnt + 1];

		memset(tail, '\0', strlen(tail));
	}
}

void at_instruction_transmit(short level)
{	// AT 从指令缓存中选择一级并开始传输
	if(!at.info.switch_status)
		return;

//	USART_TypeDef*
//		USART_temp = usart.printf.fput;
//	
//	if (usart.printf.fput != at.device.usart_at)
//		usart.printf.redirect_to(at.device.usart_at);

//	UART_TRANSMIT("%s", at.transmit_cache.cache[at.transmit_cache.crt_ist][level]);
//	
//	usart.printf.fput = USART_temp;
	
	dma.write_cache(&at.transmit_cache.cache[at.transmit_cache.crt_ist][level],
		strlen(at.transmit_cache.cache[at.transmit_cache.crt_ist][level]),at.device.usart_at);
	
	dma.transfer(at.device.usart_at);
	
//	usart.send.send_data((const int16_t*)at_cache.cache[at_cache.crt_ist][level],strlen(at_cache.cache[at_cache.crt_ist][level]), at.info.usart_at);
}

void at_single_level_instruction_transmit(short param_amt, char* ist, ...)
{	// AT 生成一级指令并开始传输
	va_list va_ptr = { 0 };

	char
		param_info[] = { '1', ':', param_amt + '0' },
		* param[10] = { 0 };

	va_start(va_ptr, ist);

	param[0] = ist;

	for (size_t cnt = 1; cnt < param_amt; cnt++)
		param[cnt] = (char*)va_arg(va_ptr, char*);

	at_ctrl.taransmit.multi_level.generate(param_info, param[0], param[1], param[2], param[3], param[4],
		param[5], param[6], param[7], param[8], param[9]);

	at_ctrl.taransmit.multi_level.transmit(0);

	va_end(va_ptr);
}


void at_feedback_hardware_idle_irqn(void)							// 中断服务函数
{	// AT Feedback - 中断服务函数
	char data_pack[100] = {0};
	
	dma.read_cache(data_pack,USART1);
	
	#ifdef DEBUG_MODE
	
	printf("hard irqn get data : %s \r\n",data_pack);
	
	#endif
	
	chain_stack_ctrl.push(at_feedback_stack,data_pack,strlen(data_pack));			// 把完整语句写入栈
	
}

void at_feedback_hardware_irqn(void)							// 中断服务函数
{	// AT Feedback - 中断服务函数
	static char 
		crt_num = 0;
	static char
		data_pack[100] = {0};
	
	static bool knock_on = false;
	
	data_pack[crt_num] = at.device.usart_at->DR; //记录
	
	switch(data_pack[crt_num++])
	{
		case '\r':
			knock_on = true;
			return;
		case '\n':
			if(knock_on == true)
				break;
			
			return;
		default:
			knock_on = false;
			return;
	}
	
	#ifdef DEBUG_MODE
	
	printf("hard irqn get data : %s \r\n",data_pack);
	
	#endif
	
	chain_stack_ctrl.push(at_feedback_stack,data_pack,strlen(data_pack));			// 把完整语句写入栈
	
	for (; crt_num > 0; crt_num--)													// 清空数据和当前位数
		data_pack[crt_num] = 0;
	
	knock_on = false;
}

bool at_feedback_verify_thread_init(AT_FEEDBACK_VERIFY_THREAD_MODULE_TYPEDEF_PPTR thread,
	char* name, void (*verify_pass_callback)(void),void (*verify_fail_callback)(void))
{
	if( NULL == thread ||
		(NULL == verify_pass_callback && NULL == verify_fail_callback) ||
		at.feedback.amount_inited_thread >= at.feedback.amount_max_thread)			// 超过最大线程
	{
		at_feedback_verify_thread_init_error();

		return false;
	}
	
	char (*verify_rule_ptr)[AT_FEEDBACK_VERIFY_THREAD_CFG_MAX_RULE_LENGTH] = (char(*)[100])calloc(1,sizeof(char(*)[100]));
	AT_FEEDBACK_VERIFY_THREAD_MODULE_TYPEDEF_PTR
		thread_new = (AT_FEEDBACK_VERIFY_THREAD_MODULE_TYPEDEF_PTR)calloc(1,sizeof(AT_FEEDBACK_VERIFY_THREAD_MODULE_TYPEDEF));

	if( NULL == thread_new &&
		NULL == verify_rule_ptr)
	{
		at_feedback_verify_thread_init_error();

		return false;
	}

	thread_new->info.thread_name = name;
	thread_new->info.switch_status = false;
	thread_new->info.verify_pass_threshold_value = 1.0f;

	thread_new->info.verify_rule = verify_rule_ptr;

	thread_new->process = at_feedback_verify_thread_process;

	thread_new->verify_pass_callback = verify_pass_callback;
	thread_new->verify_fail_callback = verify_fail_callback;

	*thread = thread_new;

	at.feedback.amount_inited_thread++;
	
	return true;
}

void at_feedback_verify_thread_free(AT_FEEDBACK_VERIFY_THREAD_MODULE_TYPEDEF_PPTR thread)
{	// AT Feedback Verify Thread - 释放进程
	free((*thread)->info.verify_rule);

	free(*thread);
}

void at_feedback_verify_thread_config(short cfg_groups_amount,AT_FEEDBACK_VERIFY_THREAD_MODULE_TYPEDEF_PTR thread,short priority_module,...)
{	// AT Feedback Verify Thread - 设置优先级对应的处理函数
	va_list va_ptr;
	
	if(NULL == thread)
		return;
	
	short 
		priority_temp = priority_module;

	if (cfg_groups_amount >= AT_FEEDBACK_VERIFY_THREAD_CFG_AMOUNT)
	{
		at.exception.at_feedback_verify_thread_config_error();

		return;
	}
	
	va_start(va_ptr,priority_module);
	
	for (size_t i = 0; i < cfg_groups_amount; i++)
	{
		at.feedback.thread_ptr[priority_temp] = thread;
		
		if(cfg_groups_amount - 1 == i)
			break;
		
		thread = (AT_FEEDBACK_VERIFY_THREAD_MODULE_TYPEDEF_PTR)va_arg(va_ptr,int);
		priority_temp = (short)va_arg(va_ptr,int);									// 获取可变参数
	}

	va_end(va_ptr);
}

void at_feedback_verify_thread_set_callback(short priority_of_thread,bool pass,bool fail,...)
{
	if(NULL == priority_of_thread || false == pass == fail)
		return ;
	
	AT_FEEDBACK_VERIFY_THREAD_MODULE_TYPEDEF_PTR
		module_ptr = at.feedback.thread_ptr[priority_of_thread];
	
	short 
		config_amount = pass + fail,
		mode[2] = {pass,fail + 10};

	va_list va_ptr;
	
	if(config_amount == 0)
		return;

	va_start(va_ptr,fail);
	
	for (size_t i = 0; i < config_amount; i++)
	{
		switch (mode[i])
		{
			case 1:
				module_ptr->verify_pass_callback = (void(*)(void))va_arg(va_ptr,int);
				break;
			case 11:
				module_ptr->verify_fail_callback = (void(*)(void))va_arg(va_ptr,int);
				break;
			default:
				break;
		}
	}
	
	va_end(va_ptr);
}

static inline void at_feedback_set_priority(uint8_t priority)
{	// AT Feedback - 设置当前为何优先级 
	if(at.feedback.handle_priority <= priority)								// 当前优先级小于或等于设定值
		return;

	at.feedback.handle_priority = priority;									// 设置优先级
}

void at_feedback_verify_thread_garbage_collection(short thread_priority_in_service)
{	// 回收堆栈空间
	static bool is_thread_in_service[AT_FEEDBACK_VERIFY_THREAD_CFG_AMOUNT] = {0};
	static int this_msg_loop_cnt = 0;
	static int 
		stack_depth = 0,
		stack_depth_prew = 0;
	
	if( stack_depth_prew != (stack_depth = chain_stack_ctrl.get_stack_info.current_depth(at_feedback_stack)) ||
		0 == stack_depth)
	{
		stack_depth_prew = stack_depth;		// 记录本次
		
		return;								// 退出
	}
	
	is_thread_in_service[thread_priority_in_service] = true;
	
	for(size_t i = 0; i < AT_FEEDBACK_VERIFY_THREAD_CFG_AMOUNT; i++)
		this_msg_loop_cnt += is_thread_in_service[thread_priority_in_service];
	
	if(this_msg_loop_cnt >= at.feedback.amount_inited_thread)
	{
		#ifdef DEBUG_MODE
		
		char data[100] = {0};
		
		chain_stack_ctrl.get(at_feedback_stack,data);
		
		printf("delete depth %d rule : %s\r\n",stack_depth,data);
		
		#endif
		
		chain_stack_ctrl.delete(at_feedback_stack);
		
		for (size_t i = 0; i < AT_FEEDBACK_VERIFY_THREAD_CFG_AMOUNT; i++)
		{
			is_thread_in_service[thread_priority_in_service] = false;
		}
	}
}

void at_feedback_software_handle_process(void)			
{	// AT Feedback - 优先级处理函数
	static bool threads_running = false;
	
	for (short thread_priority = 0; thread_priority < AT_FEEDBACK_VERIFY_THREAD_CFG_AMOUNT; thread_priority++)
	{
		if(	true == threads_running)
			at_feedback_verify_thread_garbage_collection(thread_priority);
		
		if( at.feedback.handle_priority > thread_priority							||	// 当前优先级较小的
			at.feedback.thread_ptr[thread_priority] == NULL						 	||	// 未初始化的
			at.feedback.thread_ptr[thread_priority]->info.switch_status == false 	||	// 未打开的
			at.feedback.thread_ptr[thread_priority]->info.verify_amount <= 0 	 	||	// 无判断规则的
			at.feedback.thread_ptr[thread_priority]->info.verify_rule[0][0] == ' ' 	||	// 未赋值规则的
			0 )
		{
			threads_running = false;
			
			continue;
		}
		
		at_feedback_verify_thread_process(at.feedback.thread_ptr[thread_priority]);		// 执行当前优先级线程的进程
		
		threads_running = true;														// 有线程在跑
		
		if(AT_FEEDBACK_VERIFY_THREAD_CFG_ONE_INTERUPT_ONE_THREAD_MODE == true)			// 是不是一次中断事件，只进行一个判断线程
			return;																		// 是，退出，无垃圾回收
		else
			continue;																	// 否，继续
	}
}

float data_verify_percent(char* input, char* rule,int sizeof_rule)     // 数据正确率
{	// 验证数据，返回小数[0.0-1.0]
    float 
		percent = 0;
    int 
        correct_num = 0;

	if(*rule == '\0')
		return true;

    for (size_t cnt = 0; cnt < sizeof_rule; cnt++)
    {
        if (input[cnt] == rule[cnt])
        {
            correct_num++;
        }
    }

    percent = correct_num / (float)sizeof_rule;

    return percent;
}

bool at_feedback_verify_thread_request(short priority_of_thread,short rule_amount,char* rule,...)
{	// AT Feedback Verify Thread - module 请求
	if(rule_amount == 0 || rule == NULL)
		return false;
	
	va_list va_ptr;

	AT_FEEDBACK_VERIFY_THREAD_MODULE_TYPEDEF_PTR
		module_ptr = at.feedback.thread_ptr[priority_of_thread];

	va_start(va_ptr,rule);
	
	char* rule_ptr = rule;

	for (size_t i = 0; i < rule_amount; i++)
	{
		#ifdef DEBUG_MODE
		
		printf("get rule : %s \r\n",rule_ptr);
		
		#endif
		
		memset(module_ptr->info.verify_rule,' ',
			strlen(module_ptr->info.verify_rule[i]));					// 清空验证规则

		convert_char_ptr_to_array(strlen(rule_ptr),rule_ptr,
			module_ptr->info.verify_rule[i]);							// 填充验证规则

		if(rule_amount - 1 != i)
			rule_ptr = (char*)va_arg(va_ptr,char*);
	}
	
	at_feedback_set_priority(priority_of_thread);						// 设置优先级
	
	module_ptr->info.verify_amount = rule_amount;
	module_ptr->info.verify_pass = false;							// 置位验证通过标志位
	module_ptr->info.result_locked = true;							// 确认锁定验证结果
	
	module_ptr->info.switch_status = true;							// 打开开关，读取成功时，会自动关掉
	
	va_end(va_ptr);

	return true;
}

void at_feedback_verify_thread_process(AT_FEEDBACK_VERIFY_THREAD_MODULE_TYPEDEF_PTR thread)
{	// AT Feedback Verify Thread - module 进程
	
	char data[100] = {0};
	
	float perc = 0;
	
	if(true != chain_stack_ctrl.get_stack_info.is_empty(at_feedback_stack))		// 队列有数据
		chain_stack_ctrl.get(at_feedback_stack,data);							// 从队列读取

	if (!thread->info.switch_status)
	{
		at.exception.at_feedback_verify_thread_process_error();

		return;
	}
	
	if( '\0' == data[0] || 					// 非字符串时
		' ' == data[0]	)
		return;
	
	#ifdef DEBUG_MODE
	
	printf("get data from stack : [ %s ] \r\n",data);
		
	#endif
	
	for (size_t i = 0; i < thread->info.verify_amount; i++)
	{
		if((perc = data_verify_percent(data,thread->info.verify_rule[i],strlen(thread->info.verify_rule[i]))) >= 
			thread->info.verify_pass_threshold_value)
		{
			memset(thread->info.verify_rule,' ',
				strlen(thread->info.verify_rule[i]));					// 清空验证规则
			
			thread->info.verify_amount --;								// 减少一个条件记录
			
			thread->info.verify_pass |= 0x00000001 << i;				// 相应位置写入 1
		}
	}
	
	if(0 < (unsigned int)thread->info.verify_pass)
	{
		#ifdef DEBUG_MODE
		
//		printf("[ thread name:%s | %s %d]\r\n",thread->info.thread_name,data,chain_stack_ctrl.get_stack_info.current_depth(at_feedback_stack));
		
		#endif

		thread->verify_pass_callback();

		chain_stack_ctrl.delete(at_feedback_stack);						// 验证成功，从队列删掉
	}
	else
	{
		#ifdef DEBUG_MODE
		
		printf("\r\n\r\n perc[ %s ]:%f \r\n\r\n",thread->info.verify_rule[0],perc);
				
		#endif

		thread->verify_fail_callback();
		
		thread->info.verify_pass = 0x00000000;
	}
	
	(void)perc;
}

int at_feedback_verify_thread_inquire(short priority_of_thread,bool unlock_result)
{	// AT Feedback Verify Thread - module 查询
	AT_FEEDBACK_VERIFY_THREAD_MODULE_TYPEDEF_PTR
		module_ptr = at.feedback.thread_ptr[priority_of_thread];
	
	unsigned int verify = module_ptr->info.verify_pass;
	
	if( 0 < verify &&
		false == module_ptr->info.result_locked)
	{
		#ifdef DEBUG_MODE
		
		printf("inquire:%d \r\n",verify);
				
		#endif
		
		module_ptr->info.switch_status = false;						// 自动关闭开关
		module_ptr->info.verify_pass = 0x00000000;					// 置位标志位
	}

	return verify;
}

void at_feedback_verify_thread_unlock_result(short priority_of_thread)
{
	AT_FEEDBACK_VERIFY_THREAD_MODULE_TYPEDEF_PTR
		module_ptr = at.feedback.thread_ptr[priority_of_thread];
	
	module_ptr->info.result_locked = false;
}

static inline void set_verify_pass_threshold_value(short priority_of_thread,float value)
{	
	AT_FEEDBACK_VERIFY_THREAD_MODULE_TYPEDEF_PTR
		thread_ptr = at.feedback.thread_ptr[priority_of_thread];
	
	if(NULL != thread_ptr)
		thread_ptr->info.verify_pass_threshold_value = value;
}

void at_feedback_verify_thread_idle_pass_callback(void)
{
	printf("\r\n pass :%x \r\n\r\n",at_ctrl.feedback.verify_thread.inquire(4,false));
	
	at_ctrl.feedback.set_priority(AT_FEEDBACK_VERIFY_THREAD_IDLE_PRIORITY);		// 恢复优先级
}

void at_feedback_verify_thread_idle_fail_callback(void)
{
	at_ctrl.feedback.set_priority(AT_FEEDBACK_VERIFY_THREAD_IDLE_PRIORITY);		// 恢复优先级
}

void at_feedback_verify_thread_init_error(void)
{
	while(1);
}

void hardware_not_ready(void)
{
	while(1);
}

void at_config_error(void)
{
	while(1);
}

void at_feedback_verify_thread_config_error(void)
{
	while(1);
}

void at_feedback_verify_thread_process_error(void)
{
	while(1);
}

AT_CONTROL_TYPEDEF at_ctrl = 
{
	{
		at_config_mount,
		at_config_transmit_end_with,
	},
	{
		at_single_level_instruction_transmit,
		{
			at_instruction_generate,
			at_instruction_transmit
		}
	},
	{
		{
			at_feedback_verify_thread_request,
			at_feedback_verify_thread_inquire,
			at_feedback_verify_thread_unlock_result,
			set_verify_pass_threshold_value,
			at_feedback_verify_thread_set_callback
		},
		
		at_feedback_verify_thread_init,
		at_feedback_verify_thread_free,
		at_feedback_verify_thread_config,
		at_feedback_set_priority,
		at_feedback_software_handle_process,
		at_feedback_hardware_irqn
	},
};
