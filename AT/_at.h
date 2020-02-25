#ifndef __AT_INSTRUCTION_H
#define __AT_INSTRUCTION_H

#include "etalib.h"

#define AT_CACHE_AMT							1			// 缓存数
#define AT_CACHE_LEVEL_AMT						2			// 缓存级数

#define AT_END_WITH_STRING_LEN					5			// 结束符长度
#define AT_REPLAY_JUDGEMENT_LEN					5

#define AT_FEEDBACK_STACK_CFG_MAX_STACK_DEPTH								10		// AT Feedback Stack Config - 最大堆栈深度

#define AT_FEEDBACK_VERIFY_THREAD_CFG_AMOUNT								5		// AT Feedback Verify Thread Config - 可配置的线程数目，且各线程优先级可为 [0 - AT_FEEDBACK_PRIORITY_AMOUNT-1]
#define AT_FEEDBACK_VERIFY_THREAD_CFG_MAX_RULE_LENGTH						100		// AT Feedback Verify Thread Config - rule 最大长度
#define AT_FEEDBACK_VERIFY_THREAD_CFG_ONE_INTERUPT_ONE_THREAD_MODE			false	// AT Feedback Verify Thread Config - 一次中断只进行一个进程，还是从最高优先级进行到最低优先级

#define AT_FEEDBACK_VERIFY_THREAD_MULTI_TRANSMIT_PRIORITY					0
#define AT_FEEDBACK_VERIFY_THREAD_IDLE_PRIORITY								AT_FEEDBACK_VERIFY_THREAD_CFG_AMOUNT - 1		// 最低优先级

typedef struct AT_Feedback_Verify_Thread_Module *AT_FEEDBACK_VERIFY_THREAD_MODULE_TYPEDEF_PTR,** AT_FEEDBACK_VERIFY_THREAD_MODULE_TYPEDEF_PPTR;

typedef struct AT_Configration_Control
{
	bool (*mount_device)(USART_TypeDef* usart_t);
	void (*transmit_end_with)(short level, char* transmit_end_with, ...);
}AT_CONFIG_CTRL_TYPEDEF;

typedef struct AT_Transmit_Control
{
	void (*single_level)(short param_amt, char* ist, ...);
	struct AT_Multi_Transmit
	{
		void (*generate)(char* param_info, char* ist, ...);
		void (*transmit)(short level);
	} multi_level;
} AT_TRANSMIT_CTRL_TYPEDEF, * AT_TRANSMIT_CTRL_TYPEDEF_PTR;

typedef struct AT_Feedback_Control
{
	struct
	{
		bool (*request)(short priority_of_thread,short rule_amount,char* rule,...);	// 请求
		int (*inquire)(short priority_of_thread,bool unlock_result);				// 查询
		void (*unlock_result)(short priority_of_thread);							// 解锁结果，即查询成功即擦除
		
		void (*set_verify_pass_threshold_value)(short priority_of_thread,float value);	// 设置阈值
		void (*set_callback)(short priority_of_thread,bool pass,bool fail,...);		// 设置阈值
		AT_FEEDBACK_VERIFY_THREAD_MODULE_TYPEDEF_PTR (*get_thread)(short priority_of_thread);
	}verify_thread;
	
	bool (*init_thread)(AT_FEEDBACK_VERIFY_THREAD_MODULE_TYPEDEF_PPTR module_t,
		char* name, void (*verify_pass_callback)(void),void (*verify_fail_callback)(void));
	void (*free_thread)(AT_FEEDBACK_VERIFY_THREAD_MODULE_TYPEDEF_PPTR module_t);
	void (*config_thread)(short cfg_groups_amount,AT_FEEDBACK_VERIFY_THREAD_MODULE_TYPEDEF_PTR module,short priority_module,...);
	void (*set_priority)(uint8_t priority);

	void (*software_handle)(void);
	void (*hardware_irqn)(void);
}AT_FEEDBACK_CTRL_TYPEDEF;

typedef struct AT_Control
{
	AT_CONFIG_CTRL_TYPEDEF config;

	AT_TRANSMIT_CTRL_TYPEDEF taransmit;
	AT_FEEDBACK_CTRL_TYPEDEF feedback;
}AT_CONTROL_TYPEDEF,*AT_CONTROL_TYPEDEF_PTR;

extern AT_CONTROL_TYPEDEF at_ctrl;

void at_feedback_hardware_idle_irqn(void);

#endif // __AT_INSTRUCTION_H
