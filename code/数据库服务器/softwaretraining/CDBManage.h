#ifndef CDBMANAGE_H_
#define CDBMANAGE_H_
#include<string>
//------------------------------------------------------------
// �����ƣ�  CDBManage
// ��������  �����ݿ���й���
//����ģʽʵ�ֵĵ�����  �ɱ����̷߳���
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