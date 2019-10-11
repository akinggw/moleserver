# moleserver

#### 介绍
基于websocket的服务器。

#### 安装和编译
1. 首先下载下代码\n
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

#### 联系作者
有问题加我微信啊:mole2d

