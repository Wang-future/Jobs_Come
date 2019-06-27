//------------------------------------------------------------
// ��Ȩ�������������������ݿ�����һ����
//           ��**  ��Ȩ����
//
// �ļ����ƣ�CRowOfDBTable.h
// ������ CRowOftblAllIPositonInfo �ֱ��װ���ݿ��е�ÿһ������ÿ�ж���
// �������ߣ���**
// �������ڣ�2019-06-20
// ģ��汾��1.0
//------------------------------------------------------------
// �޸�����      �汾              ����            ��ע
//------------------------------------------------------------
// 2019-06-20  1.0              ��**             ԭ��
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
// �����ƣ�  CRowOftblAllIPositonInfo
// ��������  ���������ݿ��� tbl_AllPositionInfo ��ÿһ�еķ�װ
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
  //�޲ι��캯�� ���ô˺�����Ŀ�����ڿ���������ʼ���������
  CRowOftblAllIPositonInfo(){}
  // default constructor
  CRowOftblAllIPositonInfo(int _date, string _company, string _workplaceprovince, string _workplacecity
  , string _diploma, short _number,string _supplement, string _positiontype, string _keyword, int _experience,int _salary) :
    date(_date), company(_company), workplaceprovince(_workplaceprovince), workplacecity(_workplacecity), diploma(_diploma), 
    number(_number), supplement(_supplement), positiontype(_positiontype), keyword(_keyword), experience(_experience),salary(_salary)
  {
  }
  //--------------------------------------------------------------------------------------
  // �������ƣ� Initialization
  // ������������δ��ʼ���Ķ�����г�ʼ��
  // ����˵�����˺���ֻҪ��Ϊ�������б��������޲ι��첢��ʼ�����ɴ�����CDataParse::DataParse����
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
// �����ƣ�  CRowOfCompanyRecuitInfo
// ��������  ���������ݿ��� tbl_CompanyRecuitInfo ��ÿһ�еķ�װ
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
  //�޲ι��캯�� ���ô˺�����Ŀ�����ڿ���������ʼ���������
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
