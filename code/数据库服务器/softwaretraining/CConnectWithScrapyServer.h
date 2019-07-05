//------------------------------------------------------------
// 版权声明：本程序模块属于数据库模块的一部分
//           王**  版权所有
//
// 文件名称：CConnectWithScrapyServer.h
// 模块名称：数据库模块
// 模块描述：实现数据库与爬虫端的连接
// 每一个文件版权许可及作者信息后，都要对文件内容进行注释说明。
// 文件说明：含有CConnectWithScrapyServer类（开启服务）
// 开发作者：王**
// 创建日期：2019-06-01
// 模块版本：1.0
//------------------------------------------------------------
// 修改日期      版本              作者            备注
//------------------------------------------------------------
// 2019-06-01  1.0              王**             原创
//------------------------------------------------------------
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
  //            0    调用失败
  //            非0  开启成功
  //--------------------------------------------------------------------------------------

  int MyStartService(const std::string ip, const std::string port);
private:
};
#endif CCONNECTWITHSCRAPYSERVER_
