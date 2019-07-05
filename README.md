# Jobs_Come、
#一个爬取招聘网站数据进行就业分析的系统，辅助大家更好的选择行业、工作地点等
#系统分为web端，数据库服务器与爬取端三部分
#数据库服务器部署说明：数据库使用C++编写（window环境，非linux环境），使用mysql数据库。在define_variable.h 配置好ip+port，即可运行接收爬虫端传送的数据
#爬取端是基于scrapy框架所写的，python语言。在代码中设置HOST、PORT主机为数据库服务器的ip+port，运行，就可以爬取数据并传送进数据库
#web端 配置nginx 并将前端代码置于 nginx下的html文件夹；运行web后端代码（配置好后端代码的host与port）并更改前端代码中的websocket绑定的ip+port。
