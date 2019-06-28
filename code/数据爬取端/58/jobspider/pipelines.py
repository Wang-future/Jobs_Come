# -*- coding: utf-8 -*-

# Define your item pipelines here
#
# Don't forget to add your pipeline to the ITEM_PIPELINES setting
# See: https://doc.scrapy.org/en/latest/topics/item-pipeline.html
import json
import pymysql
import pymongo
from scrapy.conf import settings
from twisted.enterprise import adbapi

# wang:第一步 导入相应包
import json
from socket import *
import time
#wang:第一步结束

class JobspiderPipeline(object):
    def __init__(self, dbpool):
        try:
           # host = settings["MONGODB_HOST"]
           # port = settings["MONGODB_PORT"]
           # dbname = settings["MONGODB_DBNAME"]
           # sheetname = settings["MONGODB_SHEETNAME"]
           # client = pymongo.MongoClient(host = host, port = port)
           # database = client[dbname]
           # self.post = database[sheetname]
           # 以上这些代码是连接mongodb用的代码
           self.dbpool = dbpool

           self.filename = open("58job.json", "ab")
           # dbpool = adbapi.ConnectionPool('')
        except Exception as e:
            print(e)
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
    @classmethod
    def from_settings(cls, settings):
        """
        数据库建立连接
        :param settings: 配置参数
        :return: 参数
        """
        adbparams = dict(
            host=settings['MYSQL_HOST'],
            db=settings['MYSQL_DBNAME'],
            user=settings['MYSQL_USER'],
            password=settings['MYSQL_PASSWORD'],
            charset='utf8',
            cursorclass=pymysql.cursors.DictCursor
        )
        dbpool = adbapi.ConnectionPool('pymysql', **adbparams)
        # 返回实例化参数
        return cls(dbpool)



    def process_item(self, item, spider):
        jsontext = json.dumps(dict(item), ensure_ascii=False) + ",\n"
        self.filename.write(jsontext.encode("utf-8"))
        # data = dict(item)
        # self.post.insert(data)
        query = self.dbpool.runInteraction(self.do_insert, item)  # 指定操作方法和操作数据
        # 添加异常处理
        query.addCallback(self.handle_error)  # 处理异常
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

    def do_insert(self, cursor, item):
        # insert_sql = "insert into positions(COMPONY,WORKPLACE,DIPLOMA,_NUMBER,_DATE,SUPPLEMENT,POSITIONTYPE,KEYWORD,EXPERIENCE) VALUES(%s,%s,%s,%s,%s,%s,%s,%s,%s)"
        # cursor.execute(insert_sql, (item['company_name'], item['location'], item['education'], item['number'],
        #                             item['time'], item['position_message'], item['position_name'], item['key'], item['experience']))
        insert_sql = "insert into positions58(COMPANY,WORKPLACE,DIPLOMA,_NUMBER,_DATE,SUPPLEMENT,POSITIONTYPE,KEYWORD,EXPERIENCE,SALARY) VALUES(%s,%s,%s,%s,%s,%s,%s,%s,%s,%s)"
        cursor.execute(insert_sql, (item['COMPANY'], item['WORKPLACE'], item['DIPLOMA'], item['NUMBER'],
                                    item['DATE'], item['SUPPLEMENT'], item['POSITIONTYPE'], item['KEYWORD'],
                                    item['EXPERIENCE'], item['SALARY']))

    # company_name = scrapy.Field()
    # position_name = scrapy.Field()
    # number = scrapy.Field()
    # location = scrapy.Field()
    # time = scrapy.Field()
    # key = scrapy.Field()
    # education = scrapy.Field()
    # experience = scrapy.Field()
    # position_message = scrapy.Field()

    def handle_error(self, failure):
        if failure:
            # 打印错误信息
            print(failure)

    def close_spider(self, spider):
        self.filename.close()
