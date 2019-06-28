# -*- coding: utf-8 -*-

# Define your item pipelines here
#
# Don't forget to add your pipeline to the ITEM_PIPELINES setting
# See: https://doc.scrapy.org/en/latest/topics/item-pipeline.html

#from pymongo import MongoClient

#client = MongoClient()
#collection = client["tencent"]["hr1"]
import re
import time, datetime
import cpca
# wang:第一步 导入相应包
import json
from socket import *
import time
#wang:第一步结束
class RencaiPipeline(object):
    def __init__(self):
        # wang:第二步 创建(如果不存在)，或者直接打开追加日志文件 存放错误记录
        self.filename = open("F:/logs_of_scrapyserver/logs.txt", "a")
        # wang:第二步结束

        # wang:第三步 进行连接初始化
        self.HOST = 'localhost'  # 因为我是在同一台机器上运行，所以是localhost，不同主机的话，此处需要改成服务器地址
        self.PORT = 8880 #主机端口
        self.BUFSIZ = 1024 #缓冲区
        self.ADDR = (self.HOST, self.PORT)
        self.tcpCliSock = socket(AF_INET, SOCK_STREAM)  # 客户端套接字

        connect = self.tcpCliSock.connect((self.HOST, self.PORT))
        # wang:第三步结束
    def process_item(self, item, spider):
        if spider.name == 'hr1':
            # 处理文凭数据
            if item["DIPLOMA"] == "本科以上":
                item["DIPLOMA"] = "本科"
            elif item["DIPLOMA"] == "大专以上":
                item["DIPLOMA"] = "大专"
            elif item["DIPLOMA"] == "硕士以上":
                item["DIPLOMA"] = "硕士"
            elif item["DIPLOMA"] == "博士以上":
                item["DIPLOMA"] = "博士"
            elif item["DIPLOMA"] == "中专以上" or item["DIPLOMA"] == "高中以上" or item["DIPLOMA"] == "初中以上":
                item["DIPLOMA"] = "大专以下"
            else:
                item["DIPLOMA"] = "不限"

            # 处理关键字数据
            item["KEYWORD"] = '-'.join(item["KEYWORD"])

            # 处理招聘人数
            item["NUMBER"] = re.findall(r"\d", item["NUMBER"])
            item["NUMBER"] = ''.join(item["NUMBER"])
            if item["NUMBER"] == "":
                item["NUMBER"] = "0"
            item["NUMBER"] = int(item["NUMBER"])

            # 处理日期
            item["DATE"] = "2019-"+item["DATE"]+" 00:00:00"
            timeArray = time.strptime(item["DATE"], "%Y-%m-%d %H:%M:%S")
            timeStamp = int(time.mktime(timeArray))
            item["DATE"] = timeStamp

            # 处理工作经验
            item["EXPERIENCE"] = int(item["EXPERIENCE"])

            # 处理薪水
            if item["SALARY"] == "面议" or item["SALARY"] == "不限":
                item["SALARY"] = 0
            else:
                item["SALARY"] = re.findall(r"\d+.?\d*-", item["SALARY"])
                item["SALARY"] = [x.replace("-", "") for x in item["SALARY"]]
                item["SALARY"] = ''.join(item["SALARY"])
                item["SALARY"] = float(item["SALARY"])*1000
                item["SALARY"] = int(item["SALARY"])

            # 处理工作地点
            location_str = [item["WORKPLACE"]]
            df = cpca.transform(location_str)
            province = (df[0:1])['省'][0]
            city = (df[0:1])['市'][0]
            #print(df)
            item["WORKPLACE"] = province + city

            # 处理职位信息
            item["SUPPLEMENT"] = ''.join(item["SUPPLEMENT"])
            item["SUPPLEMENT"] = item["SUPPLEMENT"][:500]

            #将字典转化成json数据
            json_obj1 = json.dumps(item, ensure_ascii=False)
            print(json_obj1)
            print("\n")
            # print(item)
            # collection.insert(item)
            return item

        if spider.name == 'hr2':
            item["POSITIONTYPE"] = item["POSITIONTYPE"].replace("\n", "")
            item["POSITIONTYPE"] = item["POSITIONTYPE"].replace("\r", "")
            item["POSITIONTYPE"] = item["POSITIONTYPE"].replace("\t", "")
            item["POSITIONTYPE"] = item["POSITIONTYPE"].replace(" ", "")
            # 处理文凭数据
            if item["DIPLOMA"] == "本科及以上" or item["DIPLOMA"] == "统招本科":
                item["DIPLOMA"] = "本科"
            elif item["DIPLOMA"] == "大专及以上":
                item["DIPLOMA"] = "大专"
            elif item["DIPLOMA"] == "硕士及以上":
                item["DIPLOMA"] = "硕士"
            elif item["DIPLOMA"] == "博士及以上":
                item["DIPLOMA"] = "博士"
            elif item["DIPLOMA"] == "中专/中技及以上" or item["DIPLOMA"] == "高中及以上" or item["DIPLOMA"] == "初中及以上":
                item["DIPLOMA"] = "大专以下"
            elif item["DIPLOMA"] == "学历不限":
                item["DIPLOMA"] = "不限"
            else:
                item["DIPLOMA"] = "不限"

            # # 处理关键字数据
            # item["KEYWORD"] = '-'.join(item["KEYWORD"])
            #
            # # 处理招聘人数
            # # 猎聘网没有招聘人数
            #
            # 处理日期
            item["DATE"] = item["DATE"].replace("年", "-")
            item["DATE"] = item["DATE"].replace("月", "-")
            item["DATE"] = item["DATE"].replace("日", "")
            item["DATE"] = item["DATE"] + " 00:00:00"
            timeArray = time.strptime(item["DATE"], "%Y-%m-%d %H:%M:%S")
            timeStamp = int(time.mktime(timeArray))
            item["DATE"] = timeStamp

            # 处理工作经验
            item["EXPERIENCE"] = re.findall(r"\d", item["EXPERIENCE"])
            item["EXPERIENCE"] = ''.join(item["EXPERIENCE"])
            if item["EXPERIENCE"] == "":
                item["EXPERIENCE"] = "0"
            item["EXPERIENCE"] = int(item["EXPERIENCE"])

            # 处理薪水
            if item["SALARY"] == "面议" or item["SALARY"] == "不限":
                item["SALARY"] = 0
            else:
                item["SALARY"] = re.findall(r"\d+.?\d*-", item["SALARY"])
                item["SALARY"] = [x.replace("-", "") for x in item["SALARY"]]
                item["SALARY"] = ''.join(item["SALARY"])
                item["SALARY"] = float(item["SALARY"]) * 10000/12
                item["SALARY"] = int(item["SALARY"])

            # 处理工作地点
            location_str = [item["WORKPLACE"]]
            df = cpca.transform(location_str)
            province = (df[0:1])['省'][0]
            city = (df[0:1])['市'][0]
            # print(df)
            item["WORKPLACE"] = province + city

            # 处理职位信息
            item["SUPPLEMENT"] = ''.join(item["SUPPLEMENT"])
            item["SUPPLEMENT"] = item["SUPPLEMENT"][:500]

            # 将字典转化成json数据
            json_obj2 = json.dumps(item, ensure_ascii=False)
            print(json_obj2)
            print("\n")

            # print(item)
            # collection.insert(item)
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
