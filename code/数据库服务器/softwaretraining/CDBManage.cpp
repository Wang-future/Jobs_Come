#include"CDBManage.h"
#include"CDataParse.h"
#include"define_variable.h"
#include"CRowOfDBTable.h"
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
otl_connect db; // connect object
CDBManage* CDBManage::instance = new CDBManage;

static CRowOftblAllIPositonInfo g_crow;
void CDBManage::ConnectToDB()
{
  otl_connect::otl_initialize(); // initialize ODBC environment
  try {

    //db.rlogon("UID=sa;PWD=wang654321.;DSN=shixun3"); // connect to ODBC
    db.rlogon("UID=root;PWD=wang654321.;DSN=jobs_come_new_32",1);
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
bool CDBManage::DataInsert(const std::string str_data)
{
  CDataParse* instance = CDataParse::get_instance();
  instance->DataParse(g_crow, str_data);
  try {
/*
    otl_cursor::direct_exec
    (
      db,
      "drop table test1",
      otl_exception::disabled // disable OTL exceptions
    ); // drop table
    db.commit();
    otl_cursor::direct_exec
    (
      db,
      "CREATE TABLE test1 (COMPONY VARCHAR(15) NOT NULL,\
      WORKPLACEPROVINCE VARCHAR(8) NOT NULL,\
      WORKPLACECITY VARCHAR(8) NOT NULL,\
      DIPLOMA VARCHAR(5) NOT NULL,\
      NUMBER SMALLINT NOT NULL,\
      DATE INT NOT NULL,\
      SUPPLEMENT VARCHAR(1000) NOT NULL,\
      POSITIONTYPE VARCHAR(20) NOT NULL,\
      KEYWORD VARCHAR(20) NOT NULL,\
      EXPERIENCE INT NOT NULL)"

  );  // create table
  db.commit(); // committing the create table statement to to the database
    otl_stream o(1, // PostgreSQL 8.1 and higher, the buffer can be > 1
      "insert into test1 values(:COMPONY<char[16]>,\
      :WORKPLACEPROVINCE<char[9]>,\
      :WORKPLACECITY<char[9]>,\
      :DIPLOMA<char[6]>,\
      :NUMBER<short>,\
      :DATE<int>,\
      :SUPPLEMENT<char[1001]>,\
      :POSITIONTYPE<char[21]>,\
      :KEYWORD<char[21]>,\
      :EXPERIENCE<int>)",
  // INSERT statement
  db // connect object
); 
    o << g_crow.company << g_crow.workplaceprovince << g_crow.workplacecity << g_crow.diploma << g_crow.number << g_crow.date <<g_crow.supplement<<g_crow.positiontype<<g_crow.keyword<<g_crow.experience;


*/
//otl_stream o(1, // PostgreSQL 8.1 and higher, the buffer can be > 1
//  "insert into test_tab values(:f1<int>,\
//      :f2<char[31]>)",
//  // INSERT statement
//  db // connect object
//);
//o << g_crow.date+100 << g_crow.company;
    //ºÃ±¾±¦±¦
    //otl_stream o(1, // PostgreSQL 8.1 and higher, the buffer can be > 1
    //  "insert into test1 values(:DATE<int>,:COMPONY<char[16]>,:WORKPLACEPROVINCE<char[9]>,:WORKPLACECITY<char[9]>,:DIPLOMA<char[6]>,\
    //:NUMBER<short>,:SUPPLEMENT<clob>,:POSITIONTYPE<char[21]>,:KEYWORD<char[21]>,:EXPERIENCE<int>)",
    //  // INSERT statement
    //  db // connect object
    //);
    //o << g_crow.date << g_crow.company << g_crow.workplaceprovince << g_crow.workplacecity << g_crow.diploma << g_crow.number
    //  << g_crow.supplement << g_crow.positiontype << g_crow.keyword << g_crow.experience;
    //otl_stream o(1, // PostgreSQL 8.1 and higher, the buffer can be > 1
    //  "insert into test2 values(:COMPONY<char[16]>,\
    //  :WORKPLACEPROVINCE<char[9]>,\
    //  :WORKPLACECITY<char[9]>,\
    //  :DIPLOMA<char[6]>,\
    //  :NUMBER<short>,\
    //  :DATE<int>,\
    //  :SUPPLEMENT<char[1001]>,\
    //  :POSITIONTYPE<char[21]>,\
    //  :KEYWORD<char[21]>,\
    //  :EXPERIENCE<int>)",
    //  // INSERT statement
    //  db // connect object
    //);
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
      :EXPERIENCE<int>)",
      // INSERT statement
      db // connect object
    );
    o <<NULL<< g_crow.company << g_crow.workplaceprovince << g_crow.workplacecity << g_crow.diploma << g_crow.number << g_crow.date << g_crow.supplement << g_crow.positiontype << g_crow.keyword << g_crow.experience;

  }
  catch (otl_exception& p) { // intercept OTL exceptions
    cerr << p.msg << endl; // print out error message
    cerr << p.stm_text << endl; // print out SQL that caused the error
    cerr << p.var_info << endl; // print out the variable that caused the error
  }
  
 
  //db.logoff(); // disconnect from ODBC
  return true;
}
bool  CDBManage::DistributionData()
{
  cout << "in DistributionData";
  return true;
}