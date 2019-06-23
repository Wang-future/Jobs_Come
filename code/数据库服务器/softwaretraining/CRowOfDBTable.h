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
  //无参构造函数 设置此函数的目的在于可声明不初始化的类对象
  CRowOftblAllIPositonInfo(){}
  // default constructor
  CRowOftblAllIPositonInfo(int _date, string _company, string _workplaceprovince, string _workplacecity
  , string _diploma, short _number,string _supplement, string _positiontype, string _keyword, int _experience) :
    date(_date), company(_company), workplaceprovince(_workplaceprovince), workplacecity(_workplacecity), diploma(_diploma), 
    number(_number), supplement(_supplement), positiontype(_positiontype), keyword(_keyword), experience(_experience)
  {
  }
  //--------------------------------------------------------------------------------------
  // 函数名称： Initialization
  // 函数描述：对未初始化的对象进行初始化
  // 其他说明：此函数只要是为了允许列表对象可以无参构造并初始化，可大大提高CDataParse::DataParse性能
  //--------------------------------------------------------------------------------------
  void Initialization(int _date, string _company, string _workplaceprovince, string _workplacecity
    , string _diploma, short _number, string _supplement, string _positiontype, string _keyword, int _experience);
  
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
    return *this;
  }

  // overload function
  // redefined operator>> for reading row& from otl_stream
  otl_stream& operator>>(otl_stream& s)
  {
    s >> this->date >> this->company>> workplaceprovince>> workplacecity>> diploma>> number>> supplement>> positiontype>> keyword>> experience;
    return s;
  }

  // redefined operator<< for writing row& into otl_stream
  otl_stream& operator<<(otl_stream& s)
  {
    s <<this->date << this->company << workplaceprovince << workplacecity << diploma << number << supplement << positiontype << keyword << experience;
    return s;
  }

  

};
//------------------------------------------------------------
// 类名称：  CRowOftblAllIPositonInfo
// 类描述：  该类是数据库表格 tbl_AllPositionInfo 的每一列的封装
//------------------------------------------------------------

#endif
