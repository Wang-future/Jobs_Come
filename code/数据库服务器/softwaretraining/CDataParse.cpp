#include"CDataParse.h"
#include <string>
#include<iostream>
#include "../json/json.h"
#include"define_variable.h"
using namespace std;
#ifdef DEBUG_
CDataParse* CDataParse::instance=new CDataParse;
CDataParse* CDataParse::get_instance()
{
  return CDataParse::instance;
}

string CDataParse::CreateJson()
{
  std::string jsonStr;
  Json::Value COMPANY, WORKPLACE_PROVINCE, WORKPLACE_CITY, DIPLOMA, NUMBER, DATE, SUPPLEMENT, POSITIONTYPE, KEYWORD, EXPERIENCE;
  Json::Value root;
  Json::StreamWriterBuilder writerBuilder;
  std::ostringstream os;
  short int a = 10;
  root["DATE"] = 1000033;
  root["COMPANY"] = "COMPANY";
  root["WORKPLACE_PROVINCE"] = "GUANG";
  root["WORKPLACE_CITY"] = "SHEN";
  root["DIPLOMA"] = "DIMA";
  root["NUMBER"] = a;
  root["SUPPLEMENT"] = "SUPPLEMENT";
  root["POSITIONTYPE"] = "POSITIONTYPE";
  root["KEYWORD"] = "KEYWORD";
  root["EXPERIENCE"] =365;
  

 

  std::unique_ptr<Json::StreamWriter> jsonWriter(writerBuilder.newStreamWriter());
  jsonWriter->write(root, &os);
  jsonStr = os.str();

  //std::cout << "Json:\n" << jsonStr << std::endl;
  return jsonStr;
}
#endif
bool CDataParse::DataParse(CRowOftblAllIPositonInfo& obj,const std::string& info)
{
  if (info.empty())
    return false;

  bool res;
  JSONCPP_STRING errs;
  //COMPANY等分别代表一条数据的多个属性
  Json::Value COMPANY, WORKPLACE_PROVINCE,WORKPLACE_CITY, DIPLOMA, NUMBER, DATE, SUPPLEMENT, POSITIONTYPE, KEYWORD, EXPERIENCE, EDUCATIONLEVEL;
  Json::Value root;
  Json::CharReaderBuilder readerBuilder;

  std::unique_ptr<Json::CharReader> const jsonReader(readerBuilder.newCharReader());
  res = jsonReader->parse(info.c_str(), info.c_str() + info.length(), &root, &errs);
  if (!res || !errs.empty()) {
    std::cout << "parseJson err. " << errs << std::endl;
  }
  //提取root中的各个属性
  COMPANY = root["COMPANY"];
  WORKPLACE_PROVINCE = root["WORKPLACE_PROVINCE"];
  WORKPLACE_CITY = root["WORKPLACE_CITY"];
  DIPLOMA = root["DIPLOMA"];
  NUMBER = root["NUMBER"];
  DATE = root["DATE"];
  SUPPLEMENT = root["SUPPLEMENT"];
  POSITIONTYPE = root["POSITIONTYPE"];
  KEYWORD = root["KEYWORD"];
  EXPERIENCE = root["EXPERIENCE"];
  EDUCATIONLEVEL = root["EDUCATIONLEVEL"];
#ifdef DEBUG_
  std::cout << "COMPANY: " << COMPANY.asString() << std::endl;
  std::cout << "WORKPLACE_PROVINCE: " << WORKPLACE_PROVINCE.asString() << std::endl;
  std::cout << "WORKPLACE_CITY: " << WORKPLACE_CITY.asString() << std::endl;
  std::cout << "DIPLOMA: " << DIPLOMA.asString() << std::endl;
  std::cout << "NUMBER: " << NUMBER.asInt() << std::endl;
  std::cout << "DATE: " << DATE.asInt() << std::endl;
  std::cout << "SUPPLEMENT: " << SUPPLEMENT.asString() << std::endl;
  std::cout << "POSITIONTYPE: " << POSITIONTYPE.asString() << std::endl;
  std::cout << "KEYWORD: " << KEYWORD.asString() << std::endl;
  std::cout << "EXPERIENCE: " << EXPERIENCE.asInt() << std::endl;
  
#endif
  //这里初始化 NUMBER.asInt() 应该强制转换为short类型 否则会出现warning
  obj.Initialization(DATE.asInt(), COMPANY.asString(), WORKPLACE_PROVINCE.asString(), WORKPLACE_CITY.asString(),
    DIPLOMA.asString(), static_cast<short>(NUMBER.asInt()), SUPPLEMENT.asString(), POSITIONTYPE.asString(), KEYWORD.asString(),
    EXPERIENCE.asInt());
  return true;
}