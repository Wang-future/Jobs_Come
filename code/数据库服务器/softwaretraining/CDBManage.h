#ifndef CDBMANAGE_H_
#define CDBMANAGE_H_
#include<string>
//------------------------------------------------------------
// 类名称：  CDBManage
// 类描述：  对数据库进行管理
//饿汉模式实现的单例类  可被多线程访问
//------------------------------------------------------------

class CDBManage {
public:
  static CDBManage * get_instance();
  void ReleaseInstance();
  bool DataInsert(const std::string str_data);
  bool DistributionData();
private:
  void ConnectToDB();
  static CDBManage* instance;
  CDBManage() 
  { 
    ConnectToDB(); 
  }
};
#endif