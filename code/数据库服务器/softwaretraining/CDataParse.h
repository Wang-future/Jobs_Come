//------------------------------------------------------------
// 版权声明：本程序模块属于数据库模块的一部分
//           王**  版权所有
//
// 文件名称：CDataParse.h
// 模块名称：数据库模块
// 模块描述：实现数据库管理
// 每一个文件版权许可及作者信息后，都要对文件内容进行注释说明。
// 文件说明：含有CDataParse类（对爬虫服务器传过来的数据进行接收并传送给数据库管理类）
// 开发作者：王**
// 创建日期：2019-06-01
// 模块版本：1.0
//------------------------------------------------------------
// 修改日期      版本              作者            备注
//------------------------------------------------------------
// 2019-06-01  1.0              王**             原创
//------------------------------------------------------------
#ifndef CDATAPARSE_H_
#define CDATAPARSE_H_
#include<string>
#include"define_variable.h"
#include"CRowOfDBTable.h"
//------------------------------------------------------------
// 类名称：  CDataParse
// 类描述：  对json格式的数据进行解析
//饿汉模式实现的单例类  可被多线程访问
//------------------------------------------------------------

class CDataParse {
public:
  //--------------------------------------------------------------------------------------
  // 函数名称： DataParse
  // 函数描述： 将数据进行解析
  // 其他说明：
  // 参数说明：第一个参数为本机套接字传传过来的数据

  // 返回结果： bool 
  //相关说明
  //参数不可以为 NULL；
  //--------------------------------------------------------------------------------------
  bool DataParse(CRowOftblAllIPositonInfo& obj,const std::string& info);
  //--------------------------------------------------------------------------------------
  // 函数名称： DataCheck
  // 函数描述： 将解析后的数据进行修正，并做合法性验证
  // 其他说明：
  // 参数说明：各个参数分别为提取出来的属性

  // 返回结果： bool 
  //相关说明
  //参数不可以为 NULL；
  //--------------------------------------------------------------------------------------
  bool DataCheck(int &_date, string &_company, string &_workplace, string &_diploma,
    short &_number, string &_supplement, string &_positiontype, string &_keyword, int &_experience, int &_salary);
#ifdef DEBUG_
  // 调试函数 创建json对象进行调试
  std::string CreateJson();
#endif
  //--------------------------------------------------------------------------------------
  // 函数名称： get_instance
  // 函数描述：得到CDataParse对象唯一实例
  // 参数说明：返回该对象指针
  // 返回结果： pointer  调用结果
  //--------------------------------------------------------------------------------------
  static CDataParse* get_instance();
private:
  static CDataParse* instance;
  CDataParse() {}
  ~CDataParse() { delete get_instance(); }
};
#endif
