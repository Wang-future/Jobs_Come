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

  // ����
  SOCKET socket_Server = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_Server == INVALID_SOCKET)
  {
    printf("Server:%d", WSAGetLastError());
    return 0;
  }

  // ��
  SOCKADDR_IN addr_Server;
  addr_Server.sin_family = AF_INET;
  addr_Server.sin_port = htons(stoi(port));  //ע��Ҫ��֤�˿�û�б�ռ��
  addr_Server.sin_addr.S_un.S_addr = inet_addr(ip.c_str());
  nRes = bind(socket_Server, (SOCKADDR*)&addr_Server, sizeof(SOCKADDR));
  if (nRes != 0)
  {
    printf("Server:%d", WSAGetLastError());
    return 0;
  }

  // ����
  listen(socket_Server, 5);  // ��һ��������ʾ����ͬʱ���ӵ�����
  printf("listening...\n\n");
  while (1)
  {
    static int times = 0;
    SOCKADDR addr_Client;
    int len = sizeof(addr_Client);
    SOCKET socket_Client = accept(socket_Server, &addr_Client, &len);
    if (socket_Client != INVALID_SOCKET)
    {
      //printf("New Connection:%s\n\n", inet_ntoa(((SOCKADDR_IN*)&addr_Client)->sin_addr));
      // ����
     // char szTmp[] = "Server:This is Server!\n\n";
      //send(socket_Client, szTmp, strlen(szTmp) + 1, 0);
      //����пͻ������� ��һֱ���տͻ��˷��͵����� ֱ���ͻ��˶Ͽ�
      while(true)
      { 
      // ����
      char revBuf[1024] = { 0 };
      int i_recv = recv(socket_Client, revBuf, sizeof(revBuf), 0);
      if (i_recv < 0)
      {
        CDBManage* instance = CDBManage::get_instance();
        instance->FailedRecord("recv from" + (string)inet_ntoa(((SOCKADDR_IN*)&addr_Client)->sin_addr )+ " error!");
      }
        //�Է��Ͽ�������
      if (0 == i_recv)
        break;
      
      //�����յ�������ת��Ϊstring���ͣ���
#ifdef DEBUG_
      printf("%s\n", revBuf);
#endif
      string str_data = revBuf;
      
      for (int i = 0; i < str_data.size(); ++i)
      {
        if (str_data[i] == '\'')
          str_data[i] = '\"';
      }
      std::cout << str_data;
      CDBManage* temp = CDBManage::get_instance();
      bool b_insert=temp->DataInsert(str_data);
      //����ʧ��
      if (!b_insert)
      {
        ++times;
        cout << "\n��" + std::to_string(times) + "�������¼���֣���¼��" + PATH_OF_THE_FILE_TO_RECORD_THE_FAILED_INSERRT + ",��ǰ���鿴��\n";
      }
    }

    if(send(socket_Client, "success", 8,0)<0)
    {
      cout << "send chucuo!";
      
    }
    }
  }

  // �ر�
  closesocket(socket_Server);
  WSACleanup();


  return 0;

 }
