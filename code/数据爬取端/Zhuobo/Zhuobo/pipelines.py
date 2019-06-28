# -*- coding: utf-8 -*-

# Define your item pipelines here
#
# Don't forget to add your pipeline to the ITEM_PIPELINES setting
# See: https://doc.scrapy.org/en/latest/topics/item-pipeline.html
# wang:第一步 导入相应包
import os
import json
from socket import *
import time
#wang:第一步结束
import json
import codecs


class ZhuoboPipeline(object):
    def __init__(self):

        # wang:第二步 创建(如果不存在)，或者直接打开追加日志文件 存放错误记录
        self.filename = open("F:/logs_of_scrapyserver/logs.txt", "a")
        # wang:第二步结束

        # wang:第三步 进行连接初始化
        self.HOST = 'localhost'  # 因为我是在同一台机器上运行，所以是localhost，不同主机的话，此处需要改成服务器地址
        self.PORT = 8880  # 主机端口
        self.BUFSIZ = 1024  # 缓冲区
        self.ADDR = (self.HOST, self.PORT)
        self.tcpCliSock = socket(AF_INET, SOCK_STREAM)  # 客户端套接字

        connect = self.tcpCliSock.connect((self.HOST, self.PORT))
        # wang:第三步结束
    def process_item(self, item, spider):
        # wang:第四步 进行数据包装与传送
        text = json.dumps(dict(item), ensure_ascii=False)
        i_timessend = 0;
        while True:
            try:
                self.tcpCliSock.send(text.encode("ANSI"))  # 发送数据
                print("\n发送成功\n")
                break
            except:
                if i_timessend >= 3:
                    self.filename.write(("Send " + text + " failed!\nTime:" + time.strftime('%Y-%m-%d %H:%M:%S',
                                                                                            time.localtime(
                                                                                                time.time()))))
                    print("程序终止！原因：向数据库服务器三次发送数据失败！相关数据已记录在错误日志，路径为：" + "F:\logs_of_scrapyserver\logs.txt")
                    self.tcpCliSock.close()
                    exit()
                i_timessend += 1;
        print("\n开始等待数据反馈\n")
        recvdata = self.tcpCliSock.recv(15)
        if not recvdata:  # 对方断开连接
            print("对方断开连接！")
            self.tcpCliSock.close()
        if recvdata[0:7] != b'success':
            if recvdata[0:13] == b'data too long':
                self.filename.write((
                                            "Send " + text + " Warning:the data your send is too long(more than 1024字符)!\nTime:" + time.strftime(
                                        '%Y-%m-%d %H:%M:%S',
                                        time.localtime(time.time()))))
            else:
                print("主机无法接收，主动断开连接！")
                self.tcpCliSock.close()
        # wang:第四步 数据传送结束
        return item

    def close_spider(self, spider):
        self.filename.close()







