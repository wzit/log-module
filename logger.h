#ifndef LOGGER_H_
#define LOGGER_H_

#include <syslog.h>

/* 
 * 日志模块初始化. 日志默认级别是LOG_INFO, 默认打印到标准错误.
 */
int log_init();

/* 日志模块卸载 */
void log_destroy();

/* 
 * 设置日志打印级别, 分为以下几个级别:
 * DEBUG, INFO, WARN, ERR. 默认级别是INFO
 *
 * 注意务必检查返回值: 0, 成功; 否则, 失败
 */
int log_set_level(const char *pLogLevel);

/* 
 * 设置日志打印根路径, 以及日志文件名称
 *
 * 假设base_path为/home/bob, filename_prefix为bob_log, 则日志打印路径为/home/bob/log/bob_log.log 
 * 默认打印到标准错误. 
 *
 * 注意: 
 * 1. 目前只能在日志模块初始化时设置, 不支持在运行时更改路径!
 * 2. 务必检查返回值: 0, 成功; 否则, 失败
 */
int log_set_prefix(const char *base_path, const char *filename_prefix);

/* 
 * 设置日志自动切分周期, 默认按天切分.
 *
 * 支持按天(D), 小时(H), 分钟(M)切分; 禁用切分(NONE) 
 *
 * 注意务必检查返回值: 0, 成功; 否则, 失败
 */
int log_set_rotate_cycle(const char *pRotateCycle);

/* 
 * 设置日志打印到磁盘文件是否使用内存缓冲. 
 * 启用缓冲可极大提高性能, 默认不启用缓冲. 
 *
 * 注意务必检查返回值: 0, 成功; 否则, 失败
 */
int log_set_cache(const bool bLogCache);

/* 
 * 强制刷新日志缓冲到日志文件
 *
 * 注意务必检查返回值: 0, 成功; 否则, 失败
 */
int log_force_sync();

/* 
 * 打印日志方法, 不同级别参考<sys/syslog.h> 
 *
 * 由于syslog级别太多, 本模块进行了精简, 删除了Alert, Crit, Emerg, Notice等级别. 
 * 如果需要, 可自行添加
 */
#define logError   logErrorEx
#define logWarning logWarningEx
#define logInfo    logInfoEx
#define logDebug   logDebugEx


/* --- 以下方法为日志模块方法具体实现, 不推荐直接使用 --- */

extern int log_level;

#define logErrorEx(format, ...) if(log_level>=LOG_ERR)\
	logEx(LOG_ERR, "ERROR", true, __FILE__, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)

#define logWarningEx(format, ...) if(log_level>=LOG_WARNING)\
	logEx(LOG_WARNING, "WARNING", true, __FILE__, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)

#define logInfoEx(format, ...) if(log_level>=LOG_INFO)\
	logEx(LOG_INFO, "INFO", false, __FILE__, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)

#define logDebugEx(format, ...) if(log_level==LOG_DEBUG)\
	logEx(LOG_DEBUG, "DEBUG", true, __FILE__, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)

int logEx(int priority, const char *caption, const bool need_sync, 
                const char *file_name, const char *func_name, int line_number, const char *format, ...);

#endif

/* 
 * vim: ts=8 sw=8 expandtab fenc=utf-8
 */
