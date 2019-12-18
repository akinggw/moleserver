# moleserver

#### 介绍
一个C++编写的基于websocket协议的游戏服务器框架，运行于linux上，底层主要基于epoll，性能强劲，登录验证，注册，游戏框架基本全部做好，您只需要添加游戏逻辑就可以了。
已经主要实现以下功能:
1. 用户注册，验证，登录，更改信息；
2. 游戏服务器可以通过网站进行监控，配置和管理；
3. 游戏服务器已经实现了登录验证，桌子管理，掉线管理，掉线重回管理，定时器等等框架该有的功能都有了；
4. 游戏逻辑框架，游戏中该用到的接口，如果进入房间，离开房间，掉线处理，掉线重回后处理，定时器处理等等都已实现；
5. 提供一个与html5相连的例子供大家学习参考，也可以参考我的另外一个基于windows的项目：
https://gitee.com/akinggw/Mole2dConfrontationplatform

6. 这个linux项目只作为一个通用的游戏服务器框架，不再接入具体的实际项目相关的需求。
7. 网站后台管理功能，已经实现对玩家，游戏，服务器的管理和监控
8. 机器人功能，在数据库配置好机器人后，机器人就可以使用了 
9. 玩家无限制分成代理分销，充值，退款等功能

#### 系列教程

- [MoleServer游戏服务器框架使用教程（一） ](https://blog.csdn.net/akinggw/article/details/102985982)
- [MoleServer游戏服务器框架使用教程（二） ](https://blog.csdn.net/akinggw/article/details/102986758)
- [MoleServer游戏服务器框架使用教程（三） ](https://blog.csdn.net/akinggw/article/details/102987997)
- [MoleServer游戏服务器框架使用教程（四） ](https://blog.csdn.net/akinggw/article/details/102988586)

#### 安装和编译
首先我们要搭建好编译环境，推荐使用ubuntu server,然后安装好lamp环境，gcc,g++,makefile等等都要安装好，外部库主要使用了libmysql和libjson,如果编译时缺其它库就安装相应的库，另外代码是在codeblocks下编辑的，编译直接用makefile。

编译环境的安装，以下以ubuntu server为例:
1. sudo apt-get update
2. sudo apt-get upgrade
3. sudo apt-get dist-upgrade
4. sudo apt-get install build-essential
5. sudo apt-get install libmysqlclient-dev
6. sudo apt-get install libjsoncpp-dev
7. sudo apt-get install libhiredis-dev

lamp环境的安装，以下以ubuntu server为例:
1. sudo apt install apache2 -y
2. sudo apt install mysql-server mysql-client
3. sudo apt install php7.2-mysql php7.2-curl php7.2-json php7.2-cgi php7.2 libapache2-mod-php7.2 php7.2-gd 
4. sudo apt install redis-server (可选)

测试地址:
1. 后台测试地址：http://182.61.5.226/index.php/admin/index/login.html 账户密码admin admin
2. 代理手机测试地址：http://cms.o4.com/agentuser/index/index.html
3. 游戏测试地址: http://akinggw.gitee.io_moleserver

其它说明:
1. 首先下载下代码
git clone https://gitee.com/akinggw/moleserver.git
2. 在服务器上安装最新版本的mysql，建立新的数据库moleweb，然后导入moleweb.sql
3. 配置数据库，在bin/configs/database.ini
[database]
ipaddress=127.0.0.1
username=root
userpwd=123456
dbname=moleweb
port=3306
4. 游戏服务器和登录服务器都使用这个配置，其它的配置都在网站后台进行配置。
5. 游戏服务器的配置在mysql数据库中，具体在mol_gameroom表中，这里配置游戏服务器，添加一台就是一台
6. 开始启动，账号服务器的启动./account_server 1，带参数，后面跟ID值，这个ID值就是mol_accountserver的ID,mol_accountserver表会实时更新这台游戏服务器的信息，以便于在网站上监控.如果缺少libmysqlclient和libjson的动态库，可以通过sudo apt install libmysqlclient-dev libjsoncpp-dev来进行安装
7. 游戏服务器的启动，./game_server 1 带参数，后面跟ID值，这个ID值就是mol_gameroom的ID,mol_gameroom表会实时更新这台游戏服务器的信息，以便于在网站上监控
8. 框架带一个完整的游戏示例，在websocket_demo目录下，安装方法是将这个websocket_demo目录拷贝到网站目录下，然后像打开网页一样打开它就可以了，登录地址和端口的修改在websocket_demo/src/wxhh_mainframe.js里
9. 后端运行启动方式:setsid ./account_server 1
10. 编译代码直接在主目录直接执行make就可以了，如果要编译成debug版，只需要make debug=1就可以了，另外代码使用了codeblocks进行编辑和管理，但并没有使用codeblocks进行编译，而直接使用了makefile.
11. 网站后台管理采用YznCMS,YznCMS基于thinkphp写成，二次开发简单快捷，具体安装方法在CMS目录下的readme文件中，后台初始用户名和密码都为admin，网站的配置说明在CMS目录下。因此在使用本框架之前最好先安装好LAMP环境。
12. 有关于xredis的使用方法，请参考：https://gitee.com/0xsky/xredis

#### 联系作者
欢迎加QQ群交流:131296225
email:akinggw@126.com

[截图预览]  
![输入图片说明](https://gitee.com/akinggw/moleserver/raw/master/screen/net1.jpg)
![输入图片说明](https://gitee.com/akinggw/moleserver/raw/master/screen/net2.jpg)
![输入图片说明](https://gitee.com/akinggw/moleserver/raw/master/screen/net4.jpg)
![输入图片说明](https://gitee.com/akinggw/moleserver/raw/master/screen/screen66.jpg)
![输入图片说明](https://gitee.com/akinggw/moleserver/raw/master/screen/screen111.jpg)
![输入图片说明](https://gitee.com/akinggw/moleserver/raw/master/screen/screen201.jpg)
![输入图片说明](https://gitee.com/akinggw/moleserver/raw/master/screen/screen202.jpg)

[游戏测试]  
 [demo](http://akinggw.gitee.io/moleserver)   

