#ifndef DEFINE_VARIABLE
#define DEFINE_VARIABLE
//------------------------------------------------------------
// 整个系统需要使用的一些宏
// 开发作者：王**
// 创建日期：2019-06-01
// 模块版本：1.0
//------------------------------------------------------------
// 修改日期      版本              作者            备注
//------------------------------------------------------------
// 2019-06-01  1.0              王**             原创
//------------------------------------------------------------
#define DEBUG_
#define DB_SERVER_IP ("192.168.137.1")
#define DB_SERVER_PORT ("8880")
#define BEGINTIME_TO_DISTRIBUTE_DATA 1
#define ENDTIME_TO_DISTRIBUTE_DATA 2
#define INTERVAL_TO_JUDGE_THE_POSITON_IS_DUPLICATED (86400*10)
#define PATH_OF_THE_FILE_TO_RECORD_THE_FAILED_INSERRT ("F:\\failed_log_of_jobs_come\\logs.txt")
#endif
