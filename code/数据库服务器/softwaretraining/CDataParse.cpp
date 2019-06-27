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
bool CDataParse::DataCheck(int _date, string _company, string _workplace, string _diploma,
  short _number, string _supplement, string _positiontype, string _keyword, int _experience, int _salary)
{
  // data �ж��߼�
  if (_date < 0)
    return false;
  //_workplace �ж��߼�
  string::size_type pos = _workplace.find('-');
  if ((pos == string::npos)||(pos==0)||(pos== _workplace.size()-1))
    return false;
 /* string::size_type pos1 = _workplace.find('ʡ');
  if ((pos1 == string::npos)|| (pos1!= pos-1))
    return false;
  string::size_type pos2 = _workplace.find('��');
  if ((pos2== string::npos) || (pos2<pos))
    return false;*/
  return true;
}
//ע�������info ����ֻ��WORKPLACE���ԣ���Ҫ������WORKPLACE_PROVINCE,WORKPLACE_CITY
bool CDataParse::DataParse(CRowOftblAllIPositonInfo& obj,const std::string& info)
{
  if (info.empty())
    return false;

  bool res;
  JSONCPP_STRING errs;
  //COMPANY�ȷֱ����һ�����ݵĶ������
  Json::Value COMPANY, WORKPLACE,WORKPLACE_PROVINCE,WORKPLACE_CITY, DIPLOMA, NUMBER, DATE, SUPPLEMENT, POSITIONTYPE, KEYWORD, EXPERIENCE, EDUCATIONLEVEL,SALARY;
  Json::Value root;
  Json::CharReaderBuilder readerBuilder;

  std::unique_ptr<Json::CharReader> const jsonReader(readerBuilder.newCharReader());
  res = jsonReader->parse(info.c_str(), info.c_str() + info.length(), &root, &errs);
  if (!res || !errs.empty()) {
    std::cout << "parseJson err. " << errs << std::endl;
  }
  //��ȡroot�еĸ�������
  COMPANY = root["COMPANY"];
  WORKPLACE = root["WORKPLACE"];
  DIPLOMA = root["DIPLOMA"];
  NUMBER = root["NUMBER"];
  DATE = root["DATE"];
  SUPPLEMENT = root["SUPPLEMENT"];
  POSITIONTYPE = root["POSITIONTYPE"];
  KEYWORD = root["KEYWORD"];
  EXPERIENCE = root["EXPERIENCE"];
  EDUCATIONLEVEL = root["EDUCATIONLEVEL"];
  SALARY = root["SALARY"];
#ifdef DEBUG_
  std::cout << "COMPANY: " << COMPANY.asString() << std::endl;
  std::cout << "WORKPLACE_PROVINCE: " << WORKPLACE.asString() << std::endl;
  std::cout << "DIPLOMA: " << DIPLOMA.asString() << std::endl;
  std::cout << "NUMBER: " << NUMBER.asInt() << std::endl;
  std::cout << "DATE: " << DATE.asInt() << std::endl;
  std::cout << "SUPPLEMENT: " << SUPPLEMENT.asString() << std::endl;
  std::cout << "POSITIONTYPE: " << POSITIONTYPE.asString() << std::endl;
  std::cout << "KEYWORD: " << KEYWORD.asString() << std::endl;
  std::cout << "EXPERIENCE: " << EXPERIENCE.asInt() << std::endl;
  std::cout << "SALARY: " << SALARY.asInt() << std::endl;
#endif
  bool b_datacheck=DataCheck(DATE.asInt(), COMPANY.asString(), WORKPLACE.asString(),DIPLOMA.asString(), 
    static_cast<short>(NUMBER.asInt()), SUPPLEMENT.asString(), POSITIONTYPE.asString(), KEYWORD.asString(),
    EXPERIENCE.asInt(), SALARY.asInt());
  //���ݲ��Ϸ�
  if (!b_datacheck)
    return false;
  //��WORKPLACE��ֳ� ʡ����
  string str_temp = WORKPLACE.asString();
  string::size_type pos = str_temp.find('-');
  string workplace_province =str_temp.substr(0, pos);
  string workplace_city = str_temp.substr(pos+1, str_temp.size()-pos-1);
  //�����ʼ�� NUMBER.asInt() Ӧ��ǿ��ת��Ϊshort���� ��������warning
  obj.Initialization(DATE.asInt(), COMPANY.asString(), workplace_province, workplace_city,
    DIPLOMA.asString(), static_cast<short>(NUMBER.asInt()), SUPPLEMENT.asString(), POSITIONTYPE.asString(), KEYWORD.asString(),
    EXPERIENCE.asInt(),SALARY.asInt());
  return true;
}