#include <winsock2.h>//��ͷ�ļ���Ҫ���ڵ�һ�� ������<windows.h>����ֳ�ͻ �ɼ�����
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
// �������ƣ� Thread_Fun_Call_DistributionData
// ���������� �˺�����һ��ʱ��͵��ã������ڵ������ݿ�����DistributionData���������þ����DistributionData����˵��
//
// ����˵��
// �˺����е������̸߳������
//--------------------------------------------------------------------------------------
void Thread_Fun_Call_DistributionData()
{
  int time_to_sleep = 1000 * 60 * 60;
  while (true)
  {
    time_t tt = time(NULL);//��䷵�ص�ֻ��һ��ʱ��cuo
    tm* t = new tm;  
    localtime_s(t,&tt);
    if ((t->tm_hour >= BEGINTIME_TO_DISTRIBUTE_DATA) && (t->tm_hour <= ENDTIME_TO_DISTRIBUTE_DATA))
    {
      CDBManage* pdb_instance = CDBManage::get_instance();
      pdb_instance->DistributionData();
      Sleep(time_to_sleep*20);//����20��Сʱ
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
  //��������
  if (0 == server.MyStartService(DB_SERVER_IP, DB_SERVER_PORT))
  {
    cout << "�޷���������������";
    exit(0);
  };
  std::thread thread_call(Thread_Fun_Call_DistributionData);
  thread_call.join();
  return 0;
}