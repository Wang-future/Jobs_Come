#include <winsock2.h>//此头文件需要放在第一行 否则与<windows.h>会出现冲突 可见链接
#include <iostream>
#include <string>
#include "../json/json.h"
#include"CDataParse.h"
#include"define_variable.h"
#include"CConnectWithScrapyServer.h"
#include "stdafx.h"
#include<string>
#include<fstream>

#include"CDBManage.h"
#include<thread>
#include<time.h>
#include<windows.h>
#pragma comment(lib,"ws2_32.lib") 
//--------------------------------------------------------------------------------------
// 函数名称： Thread_Fun_Call_DistributionData
// 函数描述： 此函数在一定时间就调用，函数内调用数据库管理的DistributionData函数，作用具体见DistributionData函数说明
//
// 其他说明
// 此函数有单独的线程负责调用
//--------------------------------------------------------------------------------------
void Thread_Fun_Call_DistributionData()
{
  int time_to_sleep = 1000 * 60 * 60;
  while (true)
  {
    time_t tt = time(NULL);//这句返回的只是一个时间cuo
    tm* t = new tm;  
    localtime_s(t,&tt);
    if ((t->tm_hour >= BEGINTIME_TO_DISTRIBUTE_DATA) && (t->tm_hour <= ENDTIME_TO_DISTRIBUTE_DATA))
    {
      CDBManage* pdb_instance = CDBManage::get_instance();
      pdb_instance->DistributionData();
      Sleep(time_to_sleep*20);//休眠20个小时
    }
    else
    {
      Sleep(time_to_sleep);
    }
  }
}

int main(void)
{
  CConnectWithScrapyServer server;
  //开启服务
  if (0 == server.MyStartService(DB_SERVER_IP, DB_SERVER_PORT))
  {
    cout << "无法开启服务器服务";
    exit(0);
  };
  std::thread thread_call(Thread_Fun_Call_DistributionData);
  thread_call.join();
  return 0;
}