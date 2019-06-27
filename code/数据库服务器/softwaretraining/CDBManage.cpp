#include"CDBManage.h"
#include"CDataParse.h"
#include"define_variable.h"
#include"CRowOfDBTable.h"
#include<fstream>
#include<vector>
#include<time.h>
//#define OTL_ODBC_ALTERNATE_RPC
#define OTL_ODBC_MYSQL
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

#include"..\otl\otlv4.h"
using namespace std;
string g_failed_reason = "";
otl_connect db; // connect object
CDBManage* CDBManage::instance = new CDBManage;

static CRowOftblAllIPositonInfo g_crow;
void CDBManage::ConnectToDB()
{
  otl_connect::otl_initialize(); // initialize ODBC environment
  try {

    //db.rlogon("UID=sa;PWD=wang654321.;DSN=shixun3"); // connect to ODBC
    db.rlogon("UID=root;PWD=wang654321.;DSN=jobs_come_new_32");
#ifdef DEBUG_
    cout << "connect to jobs_come database success!";
#endif
  }
  catch (otl_exception& p) { // intercept OTL exceptions
    cerr << p.msg << endl; // print out error message
    cerr << p.stm_text << endl; // print out SQL that caused the error
    cerr << p.var_info << endl; // print out the variable that caused the error
  }

 // db.logoff(); // disconnect from ODBC      
  return ;
}
CDBManage*  CDBManage::get_instance()
{
  return CDBManage::instance;
}
void CDBManage::ReleaseInstance()
{
  delete CDBManage::get_instance();
}
bool CDBManage::CreatTable(CRowOftblAllIPositonInfo& obj)
{
  bool b_createtable = false;
  string sql = "CREATE TABLE " + obj.company +
    "(ID INT PRIMARY KEY NOT NULL AUTO_INCREMENT, "
    "WORKPLACEPROVINCE VARCHAR(8) NOT NULL,"
    "WORKPLACECITY VARCHAR(8) NOT NULL,"
    "DIPLOMA VARCHAR(5) NOT NULL,"
    "NUMBER SMALLINT NOT NULL,"
    "DATE INT NOT NULL,"
    "POSITIONTYPE VARCHAR(20) NOT NULL,"
    "KEYWORD VARCHAR(20) NOT NULL,"
    "EXPERIENCE INT NOT NULL,"
    "SALARY INT NOT NULL)";
  bool b_insertinto_createdtable = false;
  
  try {
    otl_cursor::direct_exec(db, // PostgreSQL 8.1 and higher, the buffer can be > 1
      sql.c_str());
    db.commit();
    b_createtable = true;
    otl_stream o(1, // PostgreSQL 8.1 and higher, the buffer can be > 1
      "insert into testcreatedtable values(:f1<int>,:f2<char[31]>)",
      // INSERT statement
      db // connect object
    );
    o << NULL << obj.company;
    b_insertinto_createdtable = true;
  }
  // �쳣Ҫ��������лع�
  catch (otl_exception& p) { // intercept OTL exceptions
    cerr << p.msg << endl; // print out error message
    cerr << p.stm_text << endl; // print out SQL that caused the error
    cerr << p.var_info << endl; // print out the variable that caused the error
    if (!b_createtable)
    {
      //������񲻳ɹ�
      g_failed_reason = "�������" + obj.company + "���ɹ�";
    }
    if (b_createtable&&(!b_insertinto_createdtable))//�������ɹ� ���ǲ��벻�ɹ�
    {
      string sql3 = "DROP TABLE " + obj.company;
      otl_cursor::direct_exec
      (
        db,
        sql3.c_str(),
        otl_exception::disabled // disable OTL exceptions
      ); // drop table
      db.commit();
      g_failed_reason = "�������" + obj.company + "�ɹ������ǽ�����¼��testcreatedtable���ִ����ѽ������ı��ɾ����";
    }
    return false;
  }
  return true;
}
bool CDBManage::DataDuplicateChecking(CRowOftblAllIPositonInfo& obj)
{
  
  if (!IsTheTableExisted(obj.company))//�����ڴ˱�
  {   
    if (!CreatTable(obj))
    {
      //�����ɹ� 
      return false;
    }
    else
    {
      //����ɹ������������� �������¼��
      return true;
    }
  }
  else//���ڸñ�� �������ݲ���
  {
    string sql = "select WORKPLACEPROVINCE,WORKPLACECITY,DIPLOMA,NUMBER,DATE,POSITIONTYPE,KEYWORD,EXPERIENCE,SALARY from " + obj.company + " where DATE between :begintime<int> and :endtime<int>";
    otl_stream i(150, // buffer size may be > 1
      sql.c_str(),
      db // connect object
    );

    i << obj.date - INTERVAL_TO_JUDGE_THE_POSITON_IS_DUPLICATED << obj.date + INTERVAL_TO_JUDGE_THE_POSITON_IS_DUPLICATED;
    vector<CRowOfCompanyRecuitInfo> v; // vector of CRowOfCompanyRecuitInfos
    copy(otl_input_iterator<CRowOfCompanyRecuitInfo, ptrdiff_t>(i),
      otl_input_iterator<CRowOfCompanyRecuitInfo, ptrdiff_t>(),
      back_inserter(v));
    // ���ظ����ݽ���ɸѡ
    for (std::vector<CRowOfCompanyRecuitInfo>::iterator iter = v.begin(); iter != v.end(); ++iter)
    {
      if ((iter->workplaceprovince == obj.workplaceprovince) && (iter->workplacecity == obj.workplacecity))
      {
        if (iter->positiontype == obj.positiontype)
        {
          //���������������if ���ж������ظ� 
          g_failed_reason = "�����ظ���";
          return false;
        }
      }
    }
#ifdef DEBUG_
    cout << "Size=" << static_cast<int>(v.size()) << endl;
    // send the vector to cout
    copy(v.begin(), v.end(), ostream_iterator<CRowOfCompanyRecuitInfo>(cout, "\n"));
#endif
    return true;
  }
   
}
bool CDBManage::DataInsert(const std::string str_data)
{
  CDataParse* instance = CDataParse::get_instance();
  bool b_dataparse=instance->DataParse(g_crow, str_data);
  if (!b_dataparse)
  {
    g_failed_reason = "�������ݲ��Ϸ�,�����޷�������" ;
    //��¼���������
    FailedRecord(str_data);
    return false;
  }
  bool b_datapduplicatechecking = DataDuplicateChecking(g_crow);
  if (!b_datapduplicatechecking)
  {
     //��¼���������
    FailedRecord(str_data);
    return false;
  }
  bool insert_into_AllPositionInfo = false;
  bool insert_into_companytable = false;
  try {
    
    otl_stream o(1, // PostgreSQL 8.1 and higher, the buffer can be > 1
      "insert into testtbl_AllPositionInfo values(:ID<int>,\
      :COMPONY<char[16]>,\
      :WORKPLACEPROVINCE<char[9]>,\
      :WORKPLACECITY<char[9]>,\
      :DIPLOMA<char[6]>,\
      :NUMBER<short>,\
      :DATE<int>,\
      :SUPPLEMENT<char[1001]>,\
      :POSITIONTYPE<char[21]>,\
      :KEYWORD<char[21]>,\
      :EXPERIENCE<int>,\
      :SALARY<INT>)",
      // INSERT statement
      db // connect object
    );
    o <<NULL<< g_crow.company << g_crow.workplaceprovince << g_crow.workplacecity << g_crow.diploma << g_crow.number << g_crow.date << g_crow.supplement << g_crow.positiontype << g_crow.keyword << g_crow.experience<<g_crow.salary;
    insert_into_AllPositionInfo = true;

   
    string sql_insert_companytable = "insert into " + g_crow.company + " values(:ID<int>,:WORKPLACEPROVINCE<char[9]>,:WORKPLACECITY<char[9]>,:DIPLOMA<char[6]>,:NUMBER<short>,:DATE<int>,:POSITIONTYPE<char[21]>,\
    :KEYWORD<char[21]>,:EXPERIENCE<int>,:SALARY<int>)";
    otl_stream o1(1, // PostgreSQL 8.1 and higher, the buffer can be > 1
      sql_insert_companytable.c_str(),
      // INSERT statement
      db // connect object
    );
    o1 << NULL << g_crow.workplaceprovince << g_crow.workplacecity << g_crow.diploma << g_crow.number << g_crow.date << g_crow.positiontype << g_crow.keyword << g_crow.experience << g_crow.salary;
    insert_into_companytable = true;
  }
  catch (otl_exception& p) { // intercept OTL exceptions
    cerr << p.msg << endl; // print out error message
    cerr << p.stm_text << endl; // print out SQL that caused the error
    cerr << p.var_info << endl; // print out the variable that caused the error
    char * buff = (char*)p.msg;
    g_failed_reason.assign(buff);
    if (insert_into_AllPositionInfo)
    {
      //�����һ�����ɹ� 
      if (!insert_into_companytable)
      {
        //���빫˾���ɹ�
        //���ع� ��������¼
        g_failed_reason = "�����һ�����ɹ� �����Ӧ��˾���ɹ� û�лع�";
      }

    }
   //��¼���������
    FailedRecord(str_data);
    return false;
  }
  
 
  //db.logoff(); // disconnect from ODBC
  return true;
}
bool  CDBManage::DistributionData()
{
  cout << "in DistributionData";
  return true;
}
bool CDBManage::IsTheTableExisted(const string & tablename)
{
  otl_stream i(50, // buffer size may be > 1
    "select 1 from testCreatedTable "
    "where TABLENAME = (:f12<char[31]>)"
    "limit 1",
    db // connect object
  );
  i << tablename;
  int i_retvalue;
  i >> i_retvalue;
  if (i_retvalue == 1)
    return true;
  //û�������
  return false;
}
void CDBManage::FailedRecord(const string & record)
{
  
  SYSTEMTIME st = { 0 };
  GetLocalTime(&st);
  printf("%d-%02d-%02d %02d:%02d:%02d\n",
    st.wYear,
    st.wMonth,
    st.wDay,
    st.wHour,
    st.wMinute,
    st.wSecond);
  string str_data = record + "\nʱ�䣺" + std::to_string(st.wYear) + "-" + std::to_string(st.wMonth) + "-" + std::to_string(st.wDay) + "-" + std::to_string(st.wHour)+  "\nʧ��ԭ��:"+ g_failed_reason+"\n";
  ofstream file(PATH_OF_THE_FILE_TO_RECORD_THE_FAILED_INSERRT, ios::app);
  if (file.is_open())
  {
    file << str_data;
    file.close();
  }
}
