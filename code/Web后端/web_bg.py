# _*_ coding:utf-8 _*_
__author__ = 'Patrick'

import socket
import threading
import sys
import os
import time
import base64
import hashlib
import struct
from pyecharts import options as opts
from pyecharts.render import make_snapshot
from pyecharts.charts import Pie
from pyecharts.charts import Bar
from snapshot_phantomjs import snapshot
import pymysql
import json
import pymysql.cursors
import shutil
from datetime import datetime
# ====== config ======
config = {
                    'host': '110.64.87.175',
                    'user': 'cyx',
                    'passwd': 'cyx123456',
                    'db': 'jobs_come'
                }
#web后端ip 这里的ip改动 需要同时配置改动前端相关代码绑定的ip+port
HOST = '110.64.86.209'
#web后端端口
PORT = 9000
MAGIC_STRING = '258EAFA5-E914-47DA-95CA-C5AB0DC85B11'
HANDSHAKE_STRING = "HTTP/1.1 101 Switching Protocols\r\n" \
                   "Upgrade:websocket\r\n" \
                   "Connection: Upgrade\r\n" \
                   "Sec-WebSocket-Accept: {1}\r\n" \
                   "WebSocket-Location: ws://{2}/chat\r\n" \
                   "WebSocket-Protocol:chat\r\n\r\n"
#--------------------------------------------------------------------------------------
# 函数名称： Open
# 函数描述：通过参数指定通信模式、通信协议，打开特定ip、端口的对象
#
# 其他说明：
# 参数说明：int p_iMode[in] 模式类型，参考Mode Type:KZMQ_MT_REQREP、
#                              KZMQ_MT_PUBSUB、KZMQ_MT_PULLPUSH、
#                              KZMQ_MT_XPUBSUB   
#           int p_iPort[in] 端口，对协议类型为KZMQ_PT_INPROC时无效
# 返回结果： int  调用结果
#            0    调用成功
#            非0  为具体错误代码
#填写相关说明
#对类成员函数而言：函数调用期间对象是否需要保持引用参数，是否会释放这些参数；
#如果函数分配了空间，需要由调用者释放；
#参数是否可以为 NULL；
#是否存在函数使用的性能隐忧（performance implications）；
#如果函数是可重入的（re-entrant），其同步前提（synchronization assumptions）是什么？
#--------------------------------------------------------------------------------------

#------------------------------------------------------------
# 类名称：  RWlock
# 类描述：  自实现读写锁
# 该类的实例可被多线程访问
#------------------------------------------------------------
class RWlock(object):
    def __init__(self):
        self._lock = threading.Lock()
        self._extra = threading.Lock()
        self.read_num = 0

    def read_acquire(self):
        with self._extra:
            self.read_num += 1
            if self.read_num == 1:
                self._lock.acquire()

    def read_release(self):
        with self._extra:
            self.read_num -= 1
            if self.read_num == 0:
                self._lock.release()

    def write_acquire(self):
        self._lock.acquire()

    def write_release(self):
        self._lock.release()
# 多个全局变量
g_d_positontype_to_date={}
g_d_positontype_to_times={}
lock_date=RWlock()
lock_times=RWlock()
class Th(threading.Thread):
    def __init__(self, connection, ):
        threading.Thread.__init__(self)
        self.con = connection

    def run(self):
        while True:
            userin=self.recv_data(1024)
            if not userin:
                continue
            #str为从前端获取到的用户输入的文本，编码应该是utf-8 可以打印长度自行确认下
            #print("\n字符长度:")
            #print(len(str))
            else:
                lock_date.read_acquire()
                if userin in g_d_positontype_to_date:
                    # 不为空 可更改记录并直接发送 图片
                    lock_date.read_release()
                    lock_times.write_acquire()
                    # 更新键值的访问次数
                    g_d_positontype_to_times[userin]+=1
                    lock_times.write_release()
                    self.con.send(self.write_msg(("./img/"+userin+"1.png")))
                    self.con.send(self.write_msg(("./img/"+userin+"2.png")))
                    self.con.send(self.write_msg(("./img/"+userin+"3.png")))
                    self.con.send(self.write_msg(("./img/"+userin+"4.png")))
                else:
                    lock_date.read_release()
                    con = pymysql.connect(**config)
                    mycursor = con.cursor()

                    sql1 = [
                        "Select sum(NUMBER) from testtbl_AllPositionInfo where date between 1551369600 and 1554048000 and positiontype like '%" + userin + "%'",
                        "Select sum(NUMBER) from testtbl_AllPositionInfo where date between 1554048000 and 1556640000 and positiontype like '%" + userin + "%'",
                        "Select sum(NUMBER) from testtbl_AllPositionInfo where date between 1556640000 and 1559318400 and positiontype like '%" + userin + "%'",
                        "Select sum(NUMBER) from testtbl_AllPositionInfo where date between 1559318400 and 1561910400 and positiontype like '%" + userin + "%'"]
                    sql2 = "Select workplacecity, avg(salary) as avg_salary from testtbl_AllPositionInfo where date between 1559318400 and 1561910400 and salary>0 and positiontype like '%" + userin + "%' group by workplacecity order by avg_salary desc;"
                    sql3 = [
                        "Select avg(salary) as avg_salary from testtbl_AllPositionInfo where date between 1551369600 and 1554048000 and salary >0 and positiontype like '%" + userin + "%'",
                        "Select avg(salary) as avg_salary from testtbl_AllPositionInfo where date between 1554048000 and 1556640000 and salary >0 and positiontype like '%" + userin + "%'",
                        "Select avg(salary) as avg_salary from testtbl_AllPositionInfo where date between 1556640000 and 1559318400 and salary >0 and positiontype like '%" + userin + "%'",
                        "Select avg(salary) as avg_salary from testtbl_AllPositionInfo where date between 1559318400 and 1561910400 and salary >0 and positiontype like '%" + userin + "%'"]
                    sql4 = "Select workplacecity,sum(NUMBER) as total_number from testtbl_AllPositionInfo where date between 1559318400 and 1561910400 and positiontype like '%" + userin + "%' group by workplacecity order by total_number desc;"

                    tmonth = ["3月", "4月", "5月", "6月"]
                    t1number = []
                    for i in sql1:
                        mycursor.execute(i)
                        data = mycursor.fetchall()
                        for x in data:
                            t1number.append(int('%d' % x))
                    def pie_rich_label1() -> Pie:
                        c = (
                            Pie()
                                .add(
                                "",
                                [list(z) for z in zip(tmonth, t1number)],
                                radius=["40%", "55%"],
                                label_opts=opts.LabelOpts(
                                    position="outside",
                                    formatter="{a|招聘人数}{abg|}\n{hr|}\n {b|{b}: }{c}  {per|{d}%}  ",
                                    background_color="#eee",
                                    border_color="#aaa",
                                    border_width=1,
                                    border_radius=4,
                                    rich={
                                        "a": {"color": "#999", "lineHeight": 22, "align": "center"},
                                        "abg": {
                                            "backgroundColor": "#e3e3e3",
                                            "width": "100%",
                                            "align": "right",
                                            "height": 22,
                                            "borderRadius": [4, 4, 0, 0],
                                        },
                                        "hr": {
                                            "borderColor": "#aaa",
                                            "width": "100%",
                                            "borderWidth": 0.5,
                                            "height": 0,
                                        },
                                        "b": {"fontSize": 16, "lineHeight": 33},
                                        "per": {
                                            "color": "#eee",
                                            "backgroundColor": "#334455",
                                            "padding": [2, 4],
                                            "borderRadius": 2,
                                        },
                                    },
                                ),
                            )
                                .set_global_opts(title_opts=opts.TitleOpts(title=userin + "近几月招聘需求"))
                        )
                        return c

                    make_snapshot(snapshot, pie_rich_label1().render(),"./img/"+userin+"1"+".png")

                    t2base = []
                    t2salary = []
                    mycursor.execute(sql2)
                    data = mycursor.fetchall()
                    for i in data:
                        for s in i:
                            if (type(s) == str):
                                t2base.append(s)
                            else:
                                t2salary.append(int('%d' % s))
                    bar2 = (
                        Bar()
                            .add_xaxis(t2base)
                            .add_yaxis(userin, t2salary)
                            .set_global_opts(title_opts=opts.TitleOpts(title=userin + "各城市工资对比（近一月）"))
                    )
                    make_snapshot(snapshot, bar2.render(),"./img/"+userin+"2"+".png")


                    t3salary = []
                    for i in sql3:
                        mycursor.execute(i)
                        data = mycursor.fetchall()
                        for x in data:
                            t3salary.append(int('%d' % x))
                    print(t3salary)
                    bar3 = (
                        Bar()
                            .add_xaxis(tmonth)
                            .add_yaxis(userin, t3salary)
                            .set_global_opts(title_opts=opts.TitleOpts(title=userin + "近几月平均工资"))
                    )
                    make_snapshot(snapshot, bar3.render(), "./img/"+userin+"3"+".png")

                    t4base = []
                    t4number = []
                    mycursor.execute(sql4)
                    data = mycursor.fetchall()
                    for i in data:
                        for s in i:
                            if (type(s) == str):
                                t4base.append(s)
                            else:
                                t4number.append(int('%d' % s))

                    def pie_rich_label4() -> Pie:
                        c = (
                            Pie()
                                .add(
                                "",
                                [list(z) for z in zip(t4base, t4number)],
                                radius=["40%", "55%"],
                                label_opts=opts.LabelOpts(
                                    position="outside",
                                    formatter="{a|招聘人数}{abg|}\n{hr|}\n {b|{b}: }{c}  {per|{d}%}  ",
                                    background_color="#eee",
                                    border_color="#aaa",
                                    border_width=1,
                                    border_radius=4,
                                    rich={
                                        "a": {"color": "#999", "lineHeight": 22, "align": "center"},
                                        "abg": {
                                            "backgroundColor": "#e3e3e3",
                                            "width": "100%",
                                            "align": "right",
                                            "height": 22,
                                            "borderRadius": [4, 4, 0, 0],
                                        },
                                        "hr": {
                                            "borderColor": "#aaa",
                                            "width": "100%",
                                            "borderWidth": 0.5,
                                            "height": 0,
                                        },
                                        "b": {"fontSize": 16, "lineHeight": 33},
                                        "per": {
                                            "color": "#eee",
                                            "backgroundColor": "#334455",
                                            "padding": [2, 4],
                                            "borderRadius": 2,
                                        },
                                    },
                                ),
                            )
                                .set_global_opts(title_opts=opts.TitleOpts(title=userin + "各城市招聘需求（近一月）"))
                        )
                        return c
                    make_snapshot(snapshot, pie_rich_label4().render(), "./img/"+userin+"4"+".png")
                    strtime=datetime.now().strftime('%Y-%m-%d')
                    self.con.send(self.write_msg(("{\"imgpath\":\"./img/java1.png\",\"date\":\""+strtime+"\"}")))
                    self.con.send(self.write_msg(("{\"imgpath\":\"./img/java2.png\",\"date\":\"" + strtime + "\"}")))
                    self.con.send(self.write_msg(("{\"imgpath\":\"./img/java3.png\",\"date\":\"" + strtime + "\"}")))
                    self.con.send(self.write_msg(("{\"imgpath\":\"./img/java4.png\",\"date\":\"" + strtime + "\"}")))
                    # self.con.send(self.write_msg(("./img/java2.png")))
                    # self.con.send(self.write_msg(("./img/java3.png")))
                    # self.con.send(self.write_msg(("./img/java4.png")))
                    # 表格更新
                    lock_times.write_acquire()
                    # 插入键值
                    g_d_positontype_to_times[userin]= 1
                    lock_times.write_release()
                    lock_date.write_acquire()
                    # 增加g_d_positontype_to_date
                    g_d_positontype_to_date[userin]=int(time.time())
                    lock_date.write_release()
            self.con.close()




    def recv_data(self, num):
        try:
            all_data = self.con.recv(num)
            #print((all_data.decode()))
            if not len(all_data):
                #print("empty data")
                return False
        except:
            return False

        else:
            code_len = all_data[1] & 127
            if code_len == 126:
                masks = all_data[4:8]
                data = all_data[8:]
            elif code_len == 127:
                masks = all_data[10:14]
                data = all_data[14:]
            else:
                masks = all_data[2:6]
                data = all_data[6:]
            raw_str = ""
            i = 0
            #print(len(data))
            for d in data:
                raw_str += chr(d ^ masks[i % 4])
                i += 1
            return raw_str

    # send data
    def send_data(self, data):
        if data:
            data = str(data)
        else:
            return False
        token = "\x81"
        length = len(data)
        print(length)
        if length < 126:
            token += struct.pack("B", length).decode()
        elif length <= 0xFFFF:
            token += struct.pack("!BH", 126, length).decode()
        else:
            token += struct.pack("!BQ", 127, length).decode()
        # struct为Python中处理二进制数的模块，二进制流为C，或网络流的形式。
        # data = '%s%s' % (token, data)
        #self.con.send(data.encode())
        #print(len(data))
        # print(data)
        return True

    def write_msg(self,message):
        data = struct.pack('B', 129)  # 写入第一个字节，10000001

        # 写入包长度
        msg_len = len(message)
        if msg_len <= 125:
            data += struct.pack('B', msg_len)
        elif msg_len <= (2 ** 16 - 1):
            data += struct.pack('!BH', 126, msg_len)
        elif msg_len <= (2 ** 64 - 1):
            data += struct.pack('!BQ', 127, msg_len)
        else:

            return

        data += bytes(message, encoding='utf-8')  # 写入消息内容

        return data

    # handshake
def handshake(con):
    headers = {}
    shake = con.recv(1024)

    if not len(shake):
        return False

    header, data = shake.split(('\r\n\r\n').encode(), 1)
    for line in header.split(('\r\n').encode())[1:]:
        key, val = line.split(': '.encode(), 1)
        headers[key] = val

    if 'Sec-WebSocket-Key'.encode() not in headers:
        print('This socket is not websocket, client close.')
        con.close()
        return False

    sec_key = headers['Sec-WebSocket-Key'.encode()]
    res_key = base64.b64encode(hashlib.sha1((sec_key.decode() + MAGIC_STRING).encode()).digest())

    str_handshake = HANDSHAKE_STRING.replace('{1}', res_key.decode()).replace('{2}',(HOST + ':' + str(PORT)))
    print(str_handshake)
    con.send(str_handshake.encode())
    return True


def new_service():
    """start a service socket and listen
    when coms a connection, start a new thread to handle it"""

    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        sock.bind((HOST, PORT))
        sock.listen(1000)
        # 链接队列大小
        print("bind "+str(PORT)+",ready to use")
    except:
        print("Server is already running,quit")
        sys.exit()

    while True:
        connection, address = sock.accept()
        # 返回元组（socket,add），accept调用时会进入waite状态
        print("Got connection from ", address)
        if handshake(connection):
            print("handshake success")
            try:
                t = Th(connection)
                t.start()
                print('new thread for client ...\n')
            except:
                print('start new thread error')
                connection.close()


if __name__ == '__main__':
    new_service()
