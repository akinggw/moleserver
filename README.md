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

#### 安装和编译
首先我们要搭建好编译环境，推荐使用ubuntu server,然后安装好lamp环境，gcc,g++,makefile等等都要安装好，外部库主要使用了libmysql和libjson,如果编译时缺其它库就安装相应的库，另外代码是在codeblocks下编辑的，编译直接用makefile。

1. 首先下载下代码
git clone https://gitee.com/akinggw/moleserver.git
2. 在服务器上安装最新版本的mysql，建立新的数据库moleweb，然后导入moleweb.sql
3. 配置服务器，在bin/configs/server_config.ini
[LoginServer]
ipaddress=127.0.0.1
port=3333
[database]
ipaddress=127.0.0.1
username=root
userpwd=123456
dbname=moleweb
port=3306
4. 游戏服务器和登录服务器的配置都在这里，另外这里只配置登录服务器，但游戏服务器数据库的配置也使用这个配置，如果您要在另外的服务器启动游戏服务器，需要拷贝这个配置
5. 游戏服务器的配置在mysql数据库中，具体在mol_gameroom表中，这里配置游戏服务器，添加一台就是一台
6. 开始启动，登录服务器的启动./login_server
7. 服务器的启动，./game_server 1 带参数，后面跟ID值，这个ID值就是mol_gameroom的ID,mol_gameroom表会实时更新这台游戏服务器的信息，以便于在网站上监控
8. 然后用浏览器打开websocket_demo.html，可以通过修改IP和端口来连接登录服务器和游戏服务器进入相应的消息测试。
9. 后端运行启动方式:nohup ./login_server &
10. 编译代码直接在主目录直接执行make就可以了，如果要编译成debug版，只需要make debug=1就可以了，另外代码使用了codeblocks进行编辑和管理，但并没有使用codeblocks进行编译，而直接使用了makefile.
11. 网站后台管理采用YznCMS,YznCMS基于thinkphp写成，二次开发简单快捷，具体安装方法在CMS目录下的readme文件中，后台初始用户名和密码都为admin。因此在使用本框架之前最好先安装好LAMP环境。

#### 联系作者
欢迎加QQ群交流:131296225
email:akinggw@126.com

[截图预览]  
![输入图片说明](https://gitee.com/akinggw/moleserver/raw/master/screen/net1.png)
![输入图片说明](https://gitee.com/akinggw/moleserver/raw/master/screen/net2.png)
![输入图片说明](https://gitee.com/akinggw/moleserver/raw/master/screen/net3.png)

