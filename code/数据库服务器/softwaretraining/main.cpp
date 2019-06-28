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
//���Ժ��� ����ʱɾ��
std::string createJson()
{
  std::string jsonStr;
  Json::Value root, lang, mail;
  Json::StreamWriterBuilder writerBuilder;
  std::ostringstream os;

  root["Name"] = "Liming";
  root["Age"] = 26;

  lang[0] = "C++";
  lang[1] = "Java";
  root["Language"] = lang;

  mail["Netease"] = "lmshao@163.com";
  mail["Hotmail"] = "liming.shao@hotmail.com";
  root["E-mail"] = mail;

  std::unique_ptr<Json::StreamWriter> jsonWriter(writerBuilder.newStreamWriter());
  jsonWriter->write(root, &os);
  jsonStr = os.str();

  std::cout << "Json:\n" << jsonStr << std::endl;
  return jsonStr;
}

//���Ժ��� ����ʱɾ��
bool parseJson(const std::string &info)
{
  if (info.empty())
    return false;

  bool res;
  JSONCPP_STRING errs;
  Json::Value root, lang, mail;
  Json::CharReaderBuilder readerBuilder;

  std::unique_ptr<Json::CharReader> const jsonReader(readerBuilder.newCharReader());
  res = jsonReader->parse(info.c_str(), info.c_str() + info.length(), &root, &errs);
  if (!res || !errs.empty()) {
    std::cout << "parseJson err. " << errs << std::endl;
  }

  std::cout << "Name: " << root["Name"].asString() << std::endl;
  std::cout << "Age: " << root["Age"].asInt() << std::endl;

  lang = root["Language"];
  std::cout << "Language: ";
  for (int i = 0; i < lang.size(); ++i) {
    std::cout << lang[i] << " ";
  }
  std::cout << std::endl;

  mail = root["E-mail"];
  std::cout << "Netease: " << mail["Netease"].asString() << std::endl;
  std::cout << "Hotmail: " << mail["Hotmail"].asString() << std::endl;

  return true;
}

//--------------------------------------------------------------------------------------
// �������ƣ� Thread_Fun_Call_DistributionData
// ���������� �˺�����һ��ʱ��͵��ã������ڵ������ݿ�����DistributionData���������þ����DistributionData����˵��
//
// ����˵����
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
  server.MyStartService(DB_SERVER_IP, DB_SERVER_PORT);
 /*CDBManage* my = CDBManage::get_instance();
  CDataParse* myd = CDataParse::get_instance();
  string l = myd->CreateJson();
  cout << l;*/
 /* CConnectWithScrapyServer server;
  server.MyStartService(DB_SERVER_IP, DB_SERVER_PORT);*/
  //my->DataInsert(myd->CreateJson());
 /* std::thread thread_call(Thread_Fun_Call_DistributionData);
  thread_call.join();
  */
 
  system("pause");
}