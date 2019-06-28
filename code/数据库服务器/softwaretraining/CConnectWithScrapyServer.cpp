#include "stdafx.h"
#include"CConnectWithScrapyServer.h"
#include<string>
#include <winsock2.h>
#include"CDBManage.h"
#include<iostream>
#include"define_variable.h"
#include"windows.h"
#pragma comment(lib,"ws2_32.lib") 


using namespace std;
extern string g_failed_reason;
int CConnectWithScrapyServer::MyStartService(const std::string ip, const std::string port)
{
  WORD sVersion;
  WSADATA sWsa;
  int nRes;
  sVersion = MAKEWORD(1, 1);
  nRes = WSAStartup(sVersion, &sWsa);
  if (nRes != 0)
  {
    printf("Server:%d", WSAGetLastError());
    return 0;
  }
  // 创建
  SOCKET socket_Server = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_Server == INVALID_SOCKET)
  {
    printf("Server:%d", WSAGetLastError());
    return 0;
  }
  // 绑定
  SOCKADDR_IN addr_Server;
  addr_Server.sin_family = AF_INET;
  addr_Server.sin_port = htons(stoi(port));  //注意要保证端口没有被占用
  addr_Server.sin_addr.S_un.S_addr = inet_addr(ip.c_str());
  nRes = bind(socket_Server, (SOCKADDR*)&addr_Server, sizeof(SOCKADDR));
  if (nRes != 0)
  {
    printf("Server:%d", WSAGetLastError());
    return 0;
  }
  // 监听
  listen(socket_Server, 5);  // 后一个参数表示允许同时连接的人数
  printf("listening...\n\n");
  while (1)
  {
    static int times = 0;
    SOCKADDR addr_Client;
    int len = sizeof(addr_Client);
    SOCKET socket_Client = accept(socket_Server, &addr_Client, &len);
    if (socket_Client != INVALID_SOCKET)
    {
      printf("New Connection:%s\n\n", inet_ntoa(((SOCKADDR_IN*)&addr_Client)->sin_addr));
      // 发送
     // char szTmp[] = "Server:This is Server!\n\n";
      //send(socket_Client, szTmp, strlen(szTmp) + 1, 0);
      //如果有客户端连接 就一直接收客户端发送的数据 直到客户端断开
      while (true)
      {
        // 接收
        char revBuf[1024] = { 0 };
        int i_recv = recv(socket_Client, revBuf, sizeof(revBuf), 0);
        if (i_recv < 0)
        {
          CDBManage* instance = CDBManage::get_instance();
          instance->FailedRecord("recv from" + (string)inet_ntoa(((SOCKADDR_IN*)&addr_Client)->sin_addr) + " error!");
          send(socket_Client, "fail", 15, 0);
          closesocket(socket_Client);
          break;
        }
        //对方断开连接
        if (0 == i_recv)
        {
          closesocket(socket_Client);
          break;
        }
        if ((revBuf[0] != '{')||(revBuf[1023] != '\0'))
        {
          //此种情况证明数据过长 导致了分包 这里直接丢弃不合法数据 也可以自行编码多段发送的逻辑
          CDBManage* instance = CDBManage::get_instance();
          g_failed_reason = "发送的数据过长 导致了分包！服务器逻辑将数据直接丢弃!";
          revBuf[1023] = '\0';
          string temp = revBuf;
          instance->FailedRecord(temp);
          if (send(socket_Client, "long data", 15, 0) < 0)
          {
            CDBManage* instance = CDBManage::get_instance();
            instance->FailedRecord("Server can not send data to " + (string)inet_ntoa(((SOCKADDR_IN*)&addr_Client)->sin_addr) + "!");
            closesocket(socket_Client);
            break;
          }
         continue;
        }    
#ifdef DEBUG_
        printf("%s\n", revBuf);
#endif
        //将接收到的数据转化为string类型
        string str_data = revBuf;
        //有可能传过来的字符串 用json封装使用的'',而不是"";做处理
        for (int i = 0; i < str_data.size(); ++i)
        {
          if (str_data[i] == '\'')
            str_data[i] = '\"';
        }
        //将数据插入并判断是否成功并记录
        CDBManage* temp = CDBManage::get_instance();
        bool b_insert = temp->DataInsert(str_data);
        //插入失败
        if (!b_insert)
        {
          ++times;
          cout << "\n第" + std::to_string(times++) + "条错误记录出现，记录在" + PATH_OF_THE_FILE_TO_RECORD_THE_FAILED_INSERRT + ",请前往查看。\n";
        }
        if (send(socket_Client, "success", 15, 0) < 0)
        {
          CDBManage* instance = CDBManage::get_instance();
          instance->FailedRecord("Server can not send data to " + (string)inet_ntoa(((SOCKADDR_IN*)&addr_Client)->sin_addr) + "!");
          closesocket(socket_Client);
        }
      }
    }
  }
  // 关闭
  closesocket(socket_Server);
  WSACleanup();
  return 0;
 }
