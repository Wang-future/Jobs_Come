# -*- coding: utf-8 -*-

# Define your item pipelines here
#
# Don't forget to add your pipeline to the ITEM_PIPELINES setting
# See: https://doc.scrapy.org/en/latest/topics/item-pipeline.html
# wang:第一步 导入相应包
#import simplejson
import json
from socket import *
import time
#wang:第一步结束
import json
class ZhuoboPipeline(object):
    def __init__(self):
        a='a';
    # self.filename = open("zhuobo.json", "wb")
    # wang:第二步 进行连接初始化
    # self.filename = open("F:/logs_of_scrapyserver/logs.txt", "a")
    # self.HOST = 'localhost'  # 因为我是在同一台机器上运行，所以是localhost，不同主机的话，此处需要改成服务器地址
    # self.PORT = 8880  # 主机端口
    # self.BUFSIZ = 1024  # 缓冲区
    # self.ADDR = (self.HOST, self.PORT)
    # self.tcpCliSock = socket(AF_INET, SOCK_STREAM)  # 客户端套接字
    # i_try_to_connect = 0
    # while self.tcpCliSock.connect((self.HOST, self.PORT)) < 0:  # 发起连接
    #     i_try_to_connect += 1
    #     if i_try_to_connect == 10:
    #         print("已经尝试与数据库服务器主机连接10次，均出错！程序仍在继续尝试连接，建议终止程序运行，检查数据库服务器状态！")

    # wang:第二步结束
    def process_item(self, item, spider):
        text = json.dumps(dict(item), ensure_ascii=False) + ",\n"
        # self.filename.write(text.encode("utf-8"))
        print(text.encode("utf-8"))
        # wang:第三步 进行数据包装与传送
        # text = simplejson.dumps(item, ensure_ascii=False)
        # # self.filename.write(text.encode("utf-8"))
        # i_timessend = 0;
        # while True:
        #     try:
        #         self.tcpCliSock.send(text.encode("ANSI"))  # 发送数据
        #         break
        #     except:
        #         if i_timessend >= 3:
        #             self.filename.write(("Send " + text + " failed!\nTime:" + time.strftime('%Y-%m-%d %H:%M:%S',
        #                                                                                     time.localtime(
        #                                                                                         time.time()))).encode(
        #                 "utf-8"))
        #             print("程序终止！原因：向数据库服务器三次发送数据失败！相关数据已记录在错误日志，路径为：" + "F:\logs_of_scrapyserver\logs.txt")
        #             self.tcpCliSock.close()
        #             exit()
        #         i_timessend += 1;
        # while True:
        #     recvdata = self.tcpCliSock.recv(8)
        #     if not recvdata:  # 对方断开连接
        #         print("对方断开连接！")
        #         self.tcpCliSock.close()
        #         break
        #
        #     if recvdata != 'success\00':
        #         print("主机无法接收，主动断开连接！")
        #         self.tcpCliSock.close()
        #         break
        # wang:第三步 数据传送结束
        return item
    def close_spider(self, spider):
        self.filename.close()









