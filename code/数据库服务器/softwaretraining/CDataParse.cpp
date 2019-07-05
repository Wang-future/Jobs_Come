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
  //数据修正 这里只完成了对company的修正（company 名 不能含有'('')'）
  for (int i = 0; i < _company.size(); ++i)
  {
    if (_company[i] == '(' || _company[i] == ')' || _company[i] == '（' || _company[i] == '）')
      _company[i] = '_';
  }
  // data 判断逻辑

  //_workplace 字符串合理性进行判断
  string::size_type pos = _workplace.find('-');
  if ((pos == string::npos) || (pos == 0) || (pos == _workplace.size() - 1))
  {
    g_failed_reason = "地点名：" + _workplace + "不合法";
    return false;
  }
  //对string属性进行长度，对数字进行大小检查
  if (_company.size() > 60)
  {
    g_failed_reason = "公司名：" + _company + "超过规定的60字符。";
    return false;
  }
  // 提取地点名
  string workplace_province = _workplace.substr(0, pos);
  string workplace_city = _workplace.substr(pos + 1, _workplace.size() - pos - 1);
  if (workplace_province.size() > 30)
  {
    g_failed_reason = "地点名：" + workplace_province + "超过规定的30字符。";
    return false;
  }
  if (workplace_province.size() > 30)
  {
    g_failed_reason = "地点名：" + workplace_province + "超过规定的30字符。";
    return false;
  }
  if (_diploma.size() > 16)
  {
    g_failed_reason = "地点名：" + workplace_city + "超过规定的30字符。";
    return false;
  }
  if (_number<0)
  {
    g_failed_reason = "招聘人数：" + std::to_string(_number) + " 应该不小于0。";
    return false;
  }
  if (_date<0)
  {
    g_failed_reason = "招聘日期：" + std::to_string(_date) + " 应该不小于0。";
    return false;
  }
  if (_supplement.size() > 1000)
  {
    g_failed_reason = "职位说明：" + _supplement + "超过规定的1000字符。";
    return false;
  }
  if (_positiontype.size() > 30)
  {
    g_failed_reason = "职位名称：" + _positiontype + "超过规定的30字符。";
    return false;
  }
  if (_keyword.size() > 50)
  {
    g_failed_reason = "职位名称：" + _keyword + "超过规定的50字符。";
    return false;
  }
  if (_experience<0)
  {
    g_failed_reason = "经验：" + std::to_string(_experience) + " 应该不小于0。";
    return false;
  }
  if (_salary<0)
  {
    g_failed_reason = "薪水：" + std::to_string(_salary) + " 应该不小于0。";
    return false;
  }
 
  return true;
}
//注意这里的info 里面只有WORKPLACE属性，需要解析成WORKPLACE_PROVINCE,WORKPLACE_CITY
bool CDataParse::DataParse(CRowOftblAllIPositonInfo& obj,const std::string& info)
{
  if (info.empty())
  {
    g_failed_reason = "数据为空。";
    return false;
  }
    

  bool res;
  JSONCPP_STRING errs;
  //COMPANY等分别代表一条数据的多个属性
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
  //提取root中的各个属性
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
  //将WORKPLACE拆分成 省—市
  
  
  
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
  //数据不合法
  if (!b_datacheck)
    return false;
  
  string::size_type pos = workplace.find('-');
  string workplace_province = workplace.substr(0, pos);
  string workplace_city = workplace.substr(pos + 1, workplace.size() - pos - 1);
  //这里初始化 NUMBER.asInt() 应该强制转换为short类型 否则会出现warning
  obj.Initialization(date, companyname, workplace_province, workplace_city, diploma, number, supplement, positiontype, keyword, experience, salary);
  return true;
}