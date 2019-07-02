# Jobs_Come、
#数据库服务器部署说明：在define_variable.h 配置好ip+port
#爬取端在代码中设置HOST、PORT主机为数据库服务器的ip+port，运行，就可以爬取数据并传送进数据库
#web端 配置nginx 并将前端代码置于 nginx下的html文件夹；运行web后端代码（配置好host与port）并更改前端代码中的套接字ip+port
