
#ifndef CCONNECTWITHSCRAPYSERVER_H_
#define CCONNECTWITHSCRAPYSERVER_H_
#include<string>
class CConnectWithScrapyServer {
public:
  //--------------------------------------------------------------------------------------
  // �������ƣ� MyStartService
  // ������������������������
  //
  // ����˵����
  // ����˵����int p_iMode[in] ģʽ���ͣ��ο�Mode Type:KZMQ_MT_REQREP��
  //                              KZMQ_MT_PUBSUB��KZMQ_MT_PULLPUSH��
  //                              KZMQ_MT_XPUBSUB   
  //           int p_iPort[in] �˿ڣ���Э������ΪKZMQ_PT_INPROCʱ��Ч
  // ���ؽ���� int  ���ý��
  //            0    ���óɹ�
  //            ��0  Ϊ����������
  //��д���˵��
  //�����Ա�������ԣ����������ڼ�����Ƿ���Ҫ�������ò������Ƿ���ͷ���Щ������
  //������������˿ռ䣬��Ҫ�ɵ������ͷţ�
  //�����Ƿ����Ϊ NULL��
  //�Ƿ���ں���ʹ�õ��������ǣ�performance implications����
  //��������ǿ�����ģ�re-entrant������ͬ��ǰ�ᣨsynchronization assumptions����ʲô��
  //--------------------------------------------------------------------------------------

  int MyStartService(const std::string ip, const std::string port);
private:
};
#endif CCONNECTWITHSCRAPYSERVER_
