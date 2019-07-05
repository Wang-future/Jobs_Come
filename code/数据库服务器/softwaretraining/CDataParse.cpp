#include"CDataParse.h"
#include <string>
#include<iostream>
#include "../json/json.h"
#include"define_variable.h"
using namespace std;
#ifdef DEBUG_
extern string g_failed_reason;
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
bool CDataParse::DataCheck(int &_date, string &_company, string &_workplace, string &_diploma,
  short &_number, string &_supplement, string &_positiontype, string &_keyword, int &_experience, int &_salary)
{
  //�������� ����ֻ����˶�company��������company �� ���ܺ���'('')'��
  for (int i = 0; i < _company.size(); ++i)
  {
    if (_company[i] == '(' || _company[i] == ')' || _company[i] == '��' || _company[i] == '��')
      _company[i] = '_';
  }
  // data �ж��߼�

  //_workplace �ַ��������Խ����ж�
  string::size_type pos = _workplace.find('-');
  if ((pos == string::npos) || (pos == 0) || (pos == _workplace.size() - 1))
  {
    g_failed_reason = "�ص�����" + _workplace + "���Ϸ�";
    return false;
  }
  //��string���Խ��г��ȣ������ֽ��д�С���
  if (_company.size() > 60)
  {
    g_failed_reason = "��˾����" + _company + "�����涨��60�ַ���";
    return false;
  }
  // ��ȡ�ص���
  string workplace_province = _workplace.substr(0, pos);
  string workplace_city = _workplace.substr(pos + 1, _workplace.size() - pos - 1);
  if (workplace_province.size() > 30)
  {
    g_failed_reason = "�ص�����" + workplace_province + "�����涨��30�ַ���";
    return false;
  }
  if (workplace_province.size() > 30)
  {
    g_failed_reason = "�ص�����" + workplace_province + "�����涨��30�ַ���";
    return false;
  }
  if (_diploma.size() > 16)
  {
    g_failed_reason = "�ص�����" + workplace_city + "�����涨��30�ַ���";
    return false;
  }
  if (_number<0)
  {
    g_failed_reason = "��Ƹ������" + std::to_string(_number) + " Ӧ�ò�С��0��";
    return false;
  }
  if (_date<0)
  {
    g_failed_reason = "��Ƹ���ڣ�" + std::to_string(_date) + " Ӧ�ò�С��0��";
    return false;
  }
  if (_supplement.size() > 1000)
  {
    g_failed_reason = "ְλ˵����" + _supplement + "�����涨��1000�ַ���";
    return false;
  }
  if (_positiontype.size() > 30)
  {
    g_failed_reason = "ְλ���ƣ�" + _positiontype + "�����涨��30�ַ���";
    return false;
  }
  if (_keyword.size() > 50)
  {
    g_failed_reason = "ְλ���ƣ�" + _keyword + "�����涨��50�ַ���";
    return false;
  }
  if (_experience<0)
  {
    g_failed_reason = "���飺" + std::to_string(_experience) + " Ӧ�ò�С��0��";
    return false;
  }
  if (_salary<0)
  {
    g_failed_reason = "нˮ��" + std::to_string(_salary) + " Ӧ�ò�С��0��";
    return false;
  }
 
  return true;
}
//ע�������info ����ֻ��WORKPLACE���ԣ���Ҫ������WORKPLACE_PROVINCE,WORKPLACE_CITY
bool CDataParse::DataParse(CRowOftblAllIPositonInfo& obj,const std::string& info)
{
  if (info.empty())
  {
    g_failed_reason = "����Ϊ�ա�";
    return false;
  }
    

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
    g_failed_reason = "parseJson err.";
    return false;
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
  SALARY = root["SALARY"];
  //��WORKPLACE��ֳ� ʡ����
  
  
  
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
  int date = DATE.asInt();
  string companyname = COMPANY.asString();
  string workplace = WORKPLACE.asString();
  string diploma = DIPLOMA.asString();
  short number = static_cast<short>(NUMBER.asInt());
  string supplement = SUPPLEMENT.asString();
  string positiontype = POSITIONTYPE.asString();
  string keyword = KEYWORD.asString();
  int experience = EXPERIENCE.asInt();
  int salary = SALARY.asInt();
  bool b_datacheck=DataCheck(date, companyname, workplace, diploma,number, supplement, positiontype, keyword,experience, salary);
  //���ݲ��Ϸ�
  if (!b_datacheck)
    return false;
  
  string::size_type pos = workplace.find('-');
  string workplace_province = workplace.substr(0, pos);
  string workplace_city = workplace.substr(pos + 1, workplace.size() - pos - 1);
  //�����ʼ�� NUMBER.asInt() Ӧ��ǿ��ת��Ϊshort���� ��������warning
  obj.Initialization(date, companyname, workplace_province, workplace_city, diploma, number, supplement, positiontype, keyword, experience, salary);
  return true;
}