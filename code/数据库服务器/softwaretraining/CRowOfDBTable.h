//------------------------------------------------------------
// 版权声明：本程序属于数据库管理的一部分
//           王**  版权所有
//
// 文件名称：CRowOfDBTable.h
// 包含类 CRowOftblAllIPositonInfo 分别封装数据库中的每一个表格的每行对象
// 开发作者：王**
// 创建日期：2019-06-20
// 模块版本：1.0
//------------------------------------------------------------
// 修改日期      版本              作者            备注
//------------------------------------------------------------
// 2019-06-20  1.0              王**             原创
//------------------------------------------------------------
#ifndef CROWOFDBTABLE_H_
#define CROWOFDBTABLE_H_
#include<string>
#include <iostream>
#include"define_variable.h"
#define OTL_ODBC_ALTERNATE_RPC
#if !defined(_WIN32) && !defined(_WIN64)
#define OTL_ODBC
#else 
#define OTL_ODBC_POSTGRESQL // required with PG ODBC on Windows
#endif

// #define OTL_ODBC_UNIX // uncomment this line if UnixODBC is used
#define OTL_STL // Turn on STL features
#ifndef OTL_ANSI_CPP
#define OTL_ANSI_CPP // Turn on ANSI C++ typecasts
#endif
#include "../otl/otlv4.h"

using namespace std;
//------------------------------------------------------------
// 类名称：  CRowOftblAllIPositonInfo
// 类描述：  该类是数据库表格 tbl_AllPositionInfo 的每一列的封装
//------------------------------------------------------------
class CRowOftblAllIPositonInfo {

public:
  int date;
  string company;
  string workplaceprovince;
  string workplacecity;
  string diploma;
  short number;
  string supplement;
  string positiontype;
  string keyword;
  int experience;
  int salary;
  //无参构造函数 设置此函数的目的在于可声明不初始化的类对象
  CRowOftblAllIPositonInfo(){}
  // default constructor
  CRowOftblAllIPositonInfo(int _date, string _company, string _workplaceprovince, string _workplacecity
  , string _diploma, short _number,string _supplement, string _positiontype, string _keyword, int _experience,int _salary) :
    date(_date), company(_company), workplaceprovince(_workplaceprovince), workplacecity(_workplacecity), diploma(_diploma), 
    number(_number), supplement(_supplement), positiontype(_positiontype), keyword(_keyword), experience(_experience),salary(_salary)
  {
  }
  //--------------------------------------------------------------------------------------
  // 函数名称： Initialization
  // 函数描述：对未初始化的对象进行初始化
  // 其他说明：此函数只要是为了允许列表对象可以无参构造并初始化，可大大提高CDataParse::DataParse性能
  //--------------------------------------------------------------------------------------
  void Initialization(int _date, string _company, string _workplaceprovince, string _workplacecity
    , string _diploma, short _number, string _supplement, string _positiontype, string _keyword, int _experience,int _salary);
  
  // destructor
  ~CRowOftblAllIPositonInfo() {}

  // copy constructor
  CRowOftblAllIPositonInfo(const CRowOftblAllIPositonInfo& arow) 
  {
    this->date = arow.date;
    this->company = arow.company;
    this->workplaceprovince = arow.workplaceprovince;
    this->workplacecity = arow.workplacecity;
    this->diploma = arow.diploma;
    this->number = arow.number;
    this->supplement = arow.supplement;
    this->positiontype = arow.positiontype;
    this->keyword = arow.keyword;
    this->experience = arow.experience;
    this->salary = arow.salary;
  }

  // assignment operator
  CRowOftblAllIPositonInfo& operator=(const CRowOftblAllIPositonInfo& arow)
  {
    this->date = arow.date;
    this->company = arow.company;
    this->workplaceprovince = arow.workplaceprovince;
    this->workplacecity = arow.workplacecity;
    this->diploma = arow.diploma;
    this->number = arow.number;
    this->supplement = arow.supplement;
    this->positiontype = arow.positiontype;
    this->keyword = arow.keyword;
    this->experience = arow.experience;
    this->salary = arow.salary;
    return *this;
  }

  // overload function
  // redefined operator>> for reading row& from otl_stream
  friend otl_stream& operator>>(otl_stream& s, CRowOftblAllIPositonInfo& row)
  {
    s >> row.company >> row.workplaceprovince >> row.workplacecity >> row.diploma >> row.number >> row.date >> row.supplement >> row.positiontype >> row.keyword >> row.experience >> row.salary;
    return s;
  }

  // redefined operator<< for writing row& into otl_stream
  friend otl_stream& operator<<(otl_stream& s, CRowOftblAllIPositonInfo& row)
  {
    s << row.company << row.workplaceprovince << row.workplacecity << row.diploma << row.number << row.date << row.supplement << row.positiontype << row.keyword << row.experience<< row.salary;
    return s;
  }

#ifdef DEBUG_
  friend ostream& operator<<(ostream& s, const CRowOftblAllIPositonInfo& row)
  {
    s << "workplaceprovince: " << row.workplaceprovince << ";workplacecity: " << row.workplacecity << ";diploma: " << row.diploma << ";number: " << row.number << ";date: " << row.date << ";positiontype: " << row.positiontype << ";keyword: " << row.keyword << ";experience: " << row.experience << ";salary: " << row.salary;
    return s;
  }

#endif

};
//------------------------------------------------------------
// 类名称：  CRowOfCompanyRecuitInfo
// 类描述：  该类是数据库表格 tbl_CompanyRecuitInfo 的每一列的封装
//------------------------------------------------------------
class CRowOfCompanyRecuitInfo {

public:
  int date;
  
  string workplaceprovince;
  string workplacecity;
  string diploma;
  short number; 
  string positiontype;
  string keyword;
  int experience;
  int salary;
  //无参构造函数 设置此函数的目的在于可声明不初始化的类对象
  CRowOfCompanyRecuitInfo() {}
  // default constructor
  CRowOfCompanyRecuitInfo(int _date,  string _workplaceprovince, string _workplacecity
    , string _diploma, short _number,  string _positiontype, string _keyword, int _experience, int _salary) :
    date(_date),  workplaceprovince(_workplaceprovince), workplacecity(_workplacecity), diploma(_diploma),
    number(_number),  positiontype(_positiontype), keyword(_keyword), experience(_experience), salary(_salary)
  {
  }
  
  // destructor
  ~CRowOfCompanyRecuitInfo() {}

  // copy constructor
  CRowOfCompanyRecuitInfo(const CRowOfCompanyRecuitInfo& arow)
  {
    this->date = arow.date;
    this->workplaceprovince = arow.workplaceprovince;
    this->workplacecity = arow.workplacecity;
    this->diploma = arow.diploma;
    this->number = arow.number;
    this->positiontype = arow.positiontype;
    this->keyword = arow.keyword;
    this->experience = arow.experience;
    this->salary = arow.salary;
  }

  // assignment operator
  CRowOfCompanyRecuitInfo& operator=(const CRowOfCompanyRecuitInfo& arow)
  {
    this->date = arow.date;
    this->workplaceprovince = arow.workplaceprovince;
    this->workplacecity = arow.workplacecity;
    this->diploma = arow.diploma;
    this->number = arow.number;
    this->positiontype = arow.positiontype;
    this->keyword = arow.keyword;
    this->experience = arow.experience;
    this->salary = arow.salary;
    return *this;
  }

  // overload function
  // redefined operator>> for reading row& from otl_stream
  friend otl_stream& operator>>(otl_stream& s, CRowOfCompanyRecuitInfo& row)
  {
    s >> row.workplaceprovince >> row.workplacecity >> row.diploma >> row.number >> row.date >> row.positiontype >> row.keyword >> row.experience >> row.salary;
    return s;
  }
  // redefined operator<< for writing row& into otl_stream
  friend otl_stream& operator<<(otl_stream& s, CRowOfCompanyRecuitInfo& row)
  {
    s << row.workplaceprovince << row.workplacecity << row.diploma << row.number << row.date << row.positiontype << row.keyword << row.experience << row.salary;
    return s;
  }
#ifdef DEBUG_
  friend ostream& operator<<(ostream& s, const CRowOfCompanyRecuitInfo& row)
  {
    s << "workplaceprovince: " << row.workplaceprovince << ";workplacecity: " << row.workplacecity << ";diploma: " << row.diploma << ";number: " << row.number << ";date: " << row.date << ";positiontype: " << row.positiontype << ";keyword: " << row.keyword << ";experience: " << row.experience << ";salary: " << row.salary;
    return s;
  }

#endif
};
#endif
