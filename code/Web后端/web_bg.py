# _*_ coding:utf-8 _*_
__author__ = 'Patrick'

import socket
import threading
import sys
import os

import base64
import hashlib
import struct

# ====== config ======

#web后端ip 这里的ip改动 需要同时配置改动前端相关代码绑定的ip+port
HOST = 'localhost'
#web后端端口
PORT = 9000
MAGIC_STRING = '258EAFA5-E914-47DA-95CA-C5AB0DC85B11'
HANDSHAKE_STRING = "HTTP/1.1 101 Switching Protocols\r\n" \
                   "Upgrade:websocket\r\n" \
                   "Connection: Upgrade\r\n" \
                   "Sec-WebSocket-Accept: {1}\r\n" \
                   "WebSocket-Location: ws://{2}/chat\r\n" \
                   "WebSocket-Protocol:chat\r\n\r\n"


class Th(threading.Thread):
    def __init__(self, connection, ):
        threading.Thread.__init__(self)
        self.con = connection

    def run(self):
        while True:
            str=self.recv_data(1024)
            if not str:
                continue
            #str为从前端获取到的用户输入的文本，编码应该是utf-8 可以打印长度自行确认下
            #print("\n字符长度:")
            #print(len(str))
            else:
                # put you code here
                # according to the input of user
                # Generate visualized images
                # and send the path of images to the html
                # the following is one example
                 send_str="img/show1.jpg"
                 self.con.send(self.write_msg(send_str))

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
