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
    SOCKADDR addr_Client;
    int len = sizeof(addr_Client);
    SOCKET socket_Client = accept(socket_Server, &addr_Client, &len);
    if (socket_Client != INVALID_SOCKET)
    {
      //printf("New Connection:%s\n\n", inet_ntoa(((SOCKADDR_IN*)&addr_Client)->sin_addr));
      // 发送
     // char szTmp[] = "Server:This is Server!\n\n";
      //send(socket_Client, szTmp, strlen(szTmp) + 1, 0);

      // 接收
      char revBuf[1024] = { 0 };
      recv(socket_Client, revBuf, sizeof(revBuf), 0);
    
      //将接收到的数据转化为string类型，并
#ifdef DEBUG_
      printf("%s\n", revBuf);
#endif
      string str_data = revBuf;
      std::cout << str_data;
      CDBManage* temp = CDBManage::get_instance();
      temp->DataInsert(str_data);
     // printf("%s\n\n", revBuf);

    }

    // 关闭
    closesocket(socket_Client);
  }

  // 关闭
  closesocket(socket_Server);
  WSACleanup();


  return 0;

 }
