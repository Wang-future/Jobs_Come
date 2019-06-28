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
//测试函数 发布时删除
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

//测试函数 发布时删除
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
// 函数名称： Thread_Fun_Call_DistributionData
// 函数描述： 此函数在一定时间就调用，函数内调用数据库管理的DistributionData函数，作用具体见DistributionData函数说明
//
// 其他说明：
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