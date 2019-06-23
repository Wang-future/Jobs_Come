
#ifndef CCONNECTWITHSCRAPYSERVER_H_
#define CCONNECTWITHSCRAPYSERVER_H_
#include<string>
class CConnectWithScrapyServer {
public:
  //--------------------------------------------------------------------------------------
  // 函数名称： MyStartService
  // 函数描述：开启服务器服务，
  //
  // 其他说明：
  // 参数说明：int p_iMode[in] 模式类型，参考Mode Type:KZMQ_MT_REQREP、
  //                              KZMQ_MT_PUBSUB、KZMQ_MT_PULLPUSH、
  //                              KZMQ_MT_XPUBSUB   
  //           int p_iPort[in] 端口，对协议类型为KZMQ_PT_INPROC时无效
  // 返回结果： int  调用结果
  //            0    调用成功
  //            非0  为具体错误代码
  //填写相关说明
  //对类成员函数而言：函数调用期间对象是否需要保持引用参数，是否会释放这些参数；
  //如果函数分配了空间，需要由调用者释放；
  //参数是否可以为 NULL；
  //是否存在函数使用的性能隐忧（performance implications）；
  //如果函数是可重入的（re-entrant），其同步前提（synchronization assumptions）是什么？
  //--------------------------------------------------------------------------------------

  int MyStartService(const std::string ip, const std::string port);
private:
};
#endif CCONNECTWITHSCRAPYSERVER_
