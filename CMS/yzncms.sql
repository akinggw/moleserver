-- MySQL dump 10.13  Distrib 5.7.27, for Linux (x86_64)
--
-- Host: localhost    Database: yzncms
-- ------------------------------------------------------
-- Server version	5.7.27-0ubuntu0.18.04.1

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `yzn_addons`
--

DROP TABLE IF EXISTS `yzn_addons`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `yzn_addons` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT '主键',
  `name` varchar(40) NOT NULL COMMENT '插件名或标识',
  `title` varchar(20) NOT NULL DEFAULT '' COMMENT '中文名',
  `description` text COMMENT '插件描述',
  `config` text COMMENT '配置',
  `author` varchar(40) DEFAULT '' COMMENT '作者',
  `version` varchar(20) DEFAULT '' COMMENT '版本号',
  `create_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '安装时间',
  `has_adminlist` tinyint(1) unsigned NOT NULL DEFAULT '0' COMMENT '是否有后台列表',
  `status` tinyint(2) NOT NULL DEFAULT '0' COMMENT '状态',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=utf8 COMMENT='插件表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `yzn_addons`
--

LOCK TABLES `yzn_addons` WRITE;
/*!40000 ALTER TABLE `yzn_addons` DISABLE KEYS */;
/*!40000 ALTER TABLE `yzn_addons` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `yzn_admin`
--

DROP TABLE IF EXISTS `yzn_admin`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `yzn_admin` (
  `id` smallint(3) unsigned NOT NULL AUTO_INCREMENT COMMENT '用户ID',
  `username` varchar(20) DEFAULT NULL COMMENT '管理账号',
  `password` varchar(32) DEFAULT NULL COMMENT '管理密码',
  `roleid` tinyint(4) unsigned DEFAULT '0',
  `encrypt` varchar(6) DEFAULT NULL COMMENT '加密因子',
  `nickname` char(16) NOT NULL COMMENT '昵称',
  `last_login_time` int(10) unsigned DEFAULT '0' COMMENT '最后登录时间',
  `last_login_ip` bigint(20) unsigned DEFAULT '0' COMMENT '最后登录IP',
  `email` varchar(40) DEFAULT NULL,
  `status` tinyint(2) NOT NULL DEFAULT '0' COMMENT '状态',
  PRIMARY KEY (`id`),
  KEY `username` (`username`)
) ENGINE=MyISAM AUTO_INCREMENT=3 DEFAULT CHARSET=utf8 COMMENT='管理员表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `yzn_admin`
--

LOCK TABLES `yzn_admin` WRITE;
/*!40000 ALTER TABLE `yzn_admin` DISABLE KEYS */;
INSERT INTO `yzn_admin` VALUES (1,'admin','9724b5e6c56b95f5723009ef81961bfe',1,'Wo0bAa','御宅男',1573019703,2130706433,'530765310@qq.com',1),(2,'ken678','932e31f030b850a87702a86c0e16db16',2,'Sxq6dR','御宅男',1542781151,2130706433,'530765310@qq.com',1);
/*!40000 ALTER TABLE `yzn_admin` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `yzn_adminlog`
--

DROP TABLE IF EXISTS `yzn_adminlog`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `yzn_adminlog` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT COMMENT '日志ID',
  `status` tinyint(2) NOT NULL DEFAULT '0' COMMENT '状态',
  `uid` smallint(3) NOT NULL DEFAULT '0' COMMENT '操作者ID',
  `info` text COMMENT '说明',
  `create_time` int(10) unsigned NOT NULL DEFAULT '0',
  `ip` bigint(20) unsigned NOT NULL DEFAULT '0',
  `get` varchar(255) NOT NULL DEFAULT '',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=345 DEFAULT CHARSET=utf8 COMMENT='操作日志';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `yzn_adminlog`
--

LOCK TABLES `yzn_adminlog` WRITE;
/*!40000 ALTER TABLE `yzn_adminlog` DISABLE KEYS */;
INSERT INTO `yzn_adminlog` VALUES (1,0,0,'提示语:请先登陆',1569296928,2130706433,'/admin'),(2,1,1,'提示语:恭喜您，登陆成功',1569296942,2130706433,'/admin/index/login.html'),(3,1,1,'提示语:模块安装成功！一键清理缓存后生效！',1569297052,2130706433,'/admin/module/install.html?module=cms'),(4,1,1,'提示语:模块安装成功！一键清理缓存后生效！',1569297058,2130706433,'/admin/module/install.html?module=formguide'),(5,1,1,'提示语:模块安装成功！一键清理缓存后生效！',1569297064,2130706433,'/admin/module/install.html?module=member'),(6,1,1,'提示语:模块安装成功！一键清理缓存后生效！',1569297072,2130706433,'/admin/module/install.html?module=message'),(7,1,1,'提示语:模块安装成功！一键清理缓存后生效！',1569297076,2130706433,'/admin/module/install.html?module=pay'),(8,0,0,'提示语:请先登陆',1570793597,2130706433,'/admin'),(9,1,1,'提示语:恭喜您，登陆成功',1570793876,2130706433,'/admin/index/login.html'),(10,1,1,'提示语:清理缓存',1570793923,2130706433,'/admin/index/cache.html?type=all&_=1570793877720'),(11,1,1,'提示语:清理缓存',1570793926,2130706433,'/admin/index/cache.html?type=data&_=1570793877721'),(12,1,1,'提示语:清理缓存',1570793929,2130706433,'/admin/index/cache.html?type=template&_=1570793877722'),(13,1,0,'提示语:注销成功！',1570793935,2130706433,'/admin/index/logout.html'),(14,1,1,'提示语:恭喜您，登陆成功',1570793953,2130706433,'/admin/index/login.html'),(15,0,0,'提示语:请先登陆',1570794198,2130706433,'/admin'),(16,0,0,'提示语:验证码输入错误！',1570794217,2130706433,'/admin/index/login.html'),(17,0,0,'提示语:验证码输入错误！',1570794226,2130706433,'/admin/index/login.html'),(18,1,1,'提示语:恭喜您，登陆成功',1570794235,2130706433,'/admin/index/login.html'),(19,1,0,'提示语:注销成功！',1570795112,2130706433,'/admin/index/logout.html'),(20,1,1,'提示语:恭喜您，登陆成功',1570795129,2130706433,'/admin/index/login.html'),(21,1,1,'提示语:恭喜您，登陆成功',1570801069,2130706433,'/admin/index/login.html'),(22,0,0,'提示语:请先登陆',1570848716,2130706433,'/admin/'),(23,0,0,'提示语:验证码输入错误！',1570848746,2130706433,'/admin/index/login.html'),(24,0,0,'提示语:验证码输入错误！',1570848845,2130706433,'/admin/index/login.html'),(25,1,1,'提示语:恭喜您，登陆成功',1570849086,2130706433,'/admin/index/login.html'),(26,1,1,'提示语:模块卸载成功！一键清理缓存后生效！',1570850260,2130706433,'/admin/module/uninstall.html?module=cms'),(27,1,1,'提示语:清理缓存',1570850265,2130706433,'/admin/index/cache.html?type=all&_=1570849088791'),(28,1,1,'提示语:模块卸载成功！一键清理缓存后生效！',1570851374,2130706433,'/admin/module/uninstall.html?module=member'),(29,1,1,'提示语:清理缓存',1570851378,2130706433,'/admin/index/cache.html?type=all&_=1570850858183'),(30,1,1,'提示语:模块安装成功！一键清理缓存后生效！',1570851474,2130706433,'/admin/module/install.html?module=member'),(31,1,1,'提示语:清理缓存',1570851483,2130706433,'/admin/index/cache.html?type=all&_=1570851387371'),(32,1,1,'提示语:清理缓存',1570852322,2130706433,'/admin/index/cache.html?type=all&_=1570852316731'),(33,1,1,'提示语:模块卸载成功！一键清理缓存后生效！',1570852402,2130706433,'/admin/module/uninstall.html?module=member'),(34,1,1,'提示语:清理缓存',1570852406,2130706433,'/admin/index/cache.html?type=all&_=1570852390451'),(35,1,1,'提示语:清理缓存',1570852419,2130706433,'/admin/index/cache.html?type=all&_=1570852390452'),(36,1,1,'提示语:模块安装成功！一键清理缓存后生效！',1570852433,2130706433,'/admin/module/install.html?module=member'),(37,1,1,'提示语:清理缓存',1570852438,2130706433,'/admin/index/cache.html?type=all&_=1570852422942'),(38,1,1,'提示语:模块卸载成功！一键清理缓存后生效！',1570854448,2130706433,'/admin/module/uninstall.html?module=member'),(39,1,1,'提示语:清理缓存',1570854458,2130706433,'/admin/index/cache.html?type=all&_=1570852442973'),(40,1,1,'提示语:模块安装成功！一键清理缓存后生效！',1570854470,2130706433,'/admin/module/install.html?module=member'),(41,1,1,'提示语:清理缓存',1570854474,2130706433,'/admin/index/cache.html?type=all&_=1570854461618'),(42,1,1,'提示语:清理缓存',1570854667,2130706433,'/admin/index/cache.html?type=all&_=1570854665509'),(43,1,1,'提示语:清理缓存',1570855332,2130706433,'/admin/index/cache.html?type=all&_=1570854780627'),(44,1,1,'提示语:清理缓存',1570855358,2130706433,'/admin/index/cache.html?type=all&_=1570855352882'),(45,1,0,'提示语:注销成功！',1570857041,2130706433,'/admin/index/logout.html'),(46,1,1,'提示语:恭喜您，登陆成功',1570857052,2130706433,'/admin/index/login.html'),(47,1,0,'提示语:注销成功！',1570857433,2130706433,'/admin/index/logout.html'),(48,1,1,'提示语:恭喜您，登陆成功',1570857453,2130706433,'/admin/index/login.html'),(49,1,1,'提示语:恭喜您，登陆成功',1571013984,2130706433,'/admin/index/login.html'),(50,1,1,'提示语:模块安装成功！一键清理缓存后生效！',1571015399,2130706433,'/admin/module/install.html?module=gameserver'),(51,1,1,'提示语:清理缓存',1571015404,2130706433,'/admin/index/cache.html?type=all&_=1571015379334'),(52,0,1,'提示语:该模块未安装，无需卸载！',1571015840,2130706433,'/admin/module/uninstall.html?module=gameserver'),(53,0,0,'提示语:请先登陆',1571019181,2130706433,'/gameserver/gameserver/manage/menuid/144.html'),(54,1,0,'提示语:注销成功！',1571019190,2130706433,'/admin/index/logout.html'),(55,1,1,'提示语:恭喜您，登陆成功',1571019201,2130706433,'/admin/index/login.html'),(56,1,1,'提示语:操作成功！',1571019430,2130706433,'/admin/menu/setstate.html?id=3&status=0'),(57,1,1,'提示语:操作成功！',1571019430,2130706433,'/admin/menu/setstate.html?id=3&status=1'),(58,1,1,'提示语:清理缓存',1571019822,2130706433,'/admin/index/cache.html?type=all&_=1571019203487'),(59,1,1,'提示语:更新成功！',1571023790,2130706433,'/gameserver/gameserver/edit.html?id=1'),(60,1,1,'提示语:更新成功！',1571023804,2130706433,'/gameserver/gameserver/edit.html?id=1'),(61,1,1,'提示语:更新成功！',1571024298,2130706433,'/gameserver/gameserver/edit.html?id=1'),(62,0,1,'提示语:该服务器不存在！',1571024533,2130706433,'/gameserver/gameserver/edit.html?id=300001'),(63,0,1,'提示语:该服务器不存在！',1571024613,2130706433,'/gameserver/gameserver/edit.html?id=300001'),(64,1,1,'提示语:更新成功！',1571024790,2130706433,'/gameserver/gameserver/edit.html?id=1'),(65,0,0,'提示语:请先登陆',1571029369,2130706433,'/gameserver/gameserver/manage/menuid/144.html'),(66,1,1,'提示语:恭喜您，登陆成功',1571029380,2130706433,'/admin/index/login.html'),(67,1,1,'提示语:添加成功！',1571031859,2130706433,'/gameserver/gameserver/add.html'),(68,1,1,'提示语:更新成功！',1571032068,2130706433,'/gameserver/gameserver/edit.html?id=1'),(69,1,1,'提示语:更新成功！',1571032077,2130706433,'/gameserver/gameserver/edit.html?id=1'),(70,1,1,'提示语:删除成功！',1571032083,2130706433,'/gameserver/gameserver/delete.html?ids=2'),(71,0,1,'提示语:该会员不存在！',1571032767,2130706433,'/member/member/edit.html?id=undefined'),(72,0,1,'提示语:该会员不存在！',1571032795,2130706433,'/member/member/edit.html?id=undefined'),(73,1,1,'提示语:更新成功！',1571033545,2130706433,'/member/member/edit.html?id=2'),(74,1,1,'提示语:更新成功！',1571033560,2130706433,'/member/member/edit.html?id=2'),(75,1,1,'提示语:更新成功！',1571033667,2130706433,'/member/member/edit.html?id=2'),(76,1,1,'提示语:清理缓存',1571035443,2130706433,'/admin/index/cache.html?type=all&_=1571035112100'),(77,1,1,'提示语:清理缓存',1571035456,2130706433,'/admin/index/cache.html?type=template&_=1571035112101'),(78,1,1,'提示语:清理缓存',1571035460,2130706433,'/admin/index/cache.html?type=data&_=1571035112102'),(79,0,0,'提示语:请先登陆',1571035543,2130706433,'/member/member/manage/menuid/141.html'),(80,1,1,'提示语:恭喜您，登陆成功',1571035554,2130706433,'/admin/index/login.html'),(81,1,1,'提示语:清理缓存',1571035592,2130706433,'/admin/index/cache.html?type=all&_=1571035566138'),(82,1,1,'提示语:清理缓存',1571035652,2130706433,'/admin/index/cache.html?type=all&_=1571035601715'),(83,1,1,'提示语:清理缓存',1571035723,2130706433,'/admin/index/cache.html?type=all&_=1571035656808'),(84,0,0,'提示语:请先登陆',1571035959,2130706433,'/admin/index/index.html'),(85,1,1,'提示语:更新成功！',1571038319,2130706433,'/member/member/gamedata.html?id=2'),(86,1,1,'提示语:删除成功！',1571038357,2130706433,'/gameserver/gameserver/delete.html?ids=2'),(87,1,1,'提示语:删除成功！',1571038465,2130706433,'/gameserver/gameserver/delete.html?ids=2'),(88,1,0,'提示语:注销成功！',1571038640,2130706433,'/admin/index/logout.html'),(89,0,0,'提示语:验证码输入错误！',1571038650,2130706433,'/admin/index/login.html'),(90,0,0,'提示语:验证码输入错误！',1571038652,2130706433,'/admin/index/login.html'),(91,1,1,'提示语:恭喜您，登陆成功',1571038660,2130706433,'/admin/index/login.html'),(92,1,1,'提示语:恭喜您，登陆成功',1571041915,2130706433,'/admin/index/login.html'),(93,1,1,'提示语:模块安装成功！一键清理缓存后生效！',1571042080,2130706433,'/admin/module/install.html?module=gameserver'),(94,1,1,'提示语:模块安装成功！一键清理缓存后生效！',1571042085,2130706433,'/admin/module/install.html?module=games'),(95,1,1,'提示语:清理缓存',1571042091,2130706433,'/admin/index/cache.html?type=data&_=1571041916651'),(96,1,1,'提示语:清理缓存',1571042093,2130706433,'/admin/index/cache.html?type=all&_=1571041916652'),(97,0,1,'提示语:该模块未安装，无需卸载！',1571042267,2130706433,'/admin/module/uninstall.html?module=gameserver'),(98,1,1,'提示语:清理缓存',1571042276,2130706433,'/admin/index/cache.html?type=all&_=1571042240808'),(99,1,1,'提示语:模块安装成功！一键清理缓存后生效！',1571042488,2130706433,'/admin/module/install.html?module=gameserver'),(100,1,1,'提示语:清理缓存',1571042492,2130706433,'/admin/index/cache.html?type=all&_=1571042474058'),(101,1,1,'提示语:清理缓存',1571043109,2130706433,'/admin/index/cache.html?type=all&_=1571043058971'),(102,1,1,'提示语:更新成功！',1571044091,2130706433,'/games/games/edit.html?id=300001'),(103,1,1,'提示语:添加成功！',1571044344,2130706433,'/games/games/add.html'),(104,1,1,'提示语:更新成功！',1571044356,2130706433,'/games/games/edit.html?id=300002'),(105,1,1,'提示语:删除成功！',1571044373,2130706433,'/games/games/delete.html?ids=300002'),(106,1,1,'提示语:清理缓存',1571044417,2130706433,'/admin/index/cache.html?type=all&_=1571043651858'),(107,1,1,'提示语:清理缓存',1571045079,2130706433,'/admin/index/cache.html?type=data&_=1571045075189'),(108,1,1,'提示语:清理缓存',1571045083,2130706433,'/admin/index/cache.html?type=template&_=1571045075190'),(109,1,1,'提示语:清理缓存',1571045085,2130706433,'/admin/index/cache.html?type=all&_=1571045075191'),(110,1,0,'提示语:注销成功！',1571045088,2130706433,'/admin/index/logout.html'),(111,0,0,'提示语:请先登陆',1571045120,2130706433,'/admin/'),(112,1,1,'提示语:恭喜您，登陆成功',1571045319,2130706433,'/admin/index/login.html'),(113,0,0,'提示语:验证码输入错误！',1571105625,2130706433,'/admin/index/login.html'),(114,1,1,'提示语:恭喜您，登陆成功',1571105644,2130706433,'/admin/index/login.html'),(115,1,1,'提示语:更新成功！',1571105737,2130706433,'/member/member/edit.html?id=2'),(116,1,1,'提示语:恭喜您，登陆成功',1571187147,2130706433,'/admin/index/login.html'),(117,1,1,'提示语:模块卸载成功！一键清理缓存后生效！',1571187819,2130706433,'/admin/module/uninstall.html?module=member'),(118,1,1,'提示语:清理缓存',1571187825,2130706433,'/admin/index/cache.html?type=all&_=1571187149330'),(119,1,1,'提示语:模块安装成功！一键清理缓存后生效！',1571187845,2130706433,'/admin/module/install.html?module=member'),(120,1,1,'提示语:清理缓存',1571187852,2130706433,'/admin/index/cache.html?type=all&_=1571187849012'),(121,1,1,'提示语:模块卸载成功！一键清理缓存后生效！',1571188233,2130706433,'/admin/module/uninstall.html?module=member'),(122,1,1,'提示语:清理缓存',1571188237,2130706433,'/admin/index/cache.html?type=all&_=1571188220818'),(123,1,1,'提示语:模块安装成功！一键清理缓存后生效！',1571188251,2130706433,'/admin/module/install.html?module=member'),(124,1,1,'提示语:清理缓存',1571188255,2130706433,'/admin/index/cache.html?type=all&_=1571188242016'),(125,1,1,'提示语:模块卸载成功！一键清理缓存后生效！',1571188316,2130706433,'/admin/module/uninstall.html?module=member'),(126,1,1,'提示语:清理缓存',1571188319,2130706433,'/admin/index/cache.html?type=all&_=1571188259438'),(127,1,1,'提示语:模块安装成功！一键清理缓存后生效！',1571188329,2130706433,'/admin/module/install.html?module=member'),(128,1,1,'提示语:清理缓存',1571188334,2130706433,'/admin/index/cache.html?type=all&_=1571188322816'),(129,1,1,'提示语:解锁玩家成功！',1571189025,2130706433,'/member/member/resetstate.html'),(130,1,1,'提示语:更新成功！',1571189081,2130706433,'/member/member/gamedata.html?id=2'),(131,1,1,'提示语:解锁玩家成功！',1571189204,2130706433,'/member/member/resetstate.html'),(132,1,1,'提示语:清理缓存',1571189225,2130706433,'/admin/index/cache.html?type=all&_=1571188339686'),(133,1,1,'提示语:解锁玩家成功！',1571190184,2130706433,'/member/member/resetstate.html'),(134,0,1,'提示语:该页面不存在！',1571191704,2130706433,'/member/member/robotmanage/menuid/166.html'),(135,1,1,'提示语:解锁玩家成功！',1571191707,2130706433,'/member/member/resetstate.html'),(136,0,1,'提示语:该页面不存在！',1571191738,2130706433,'/member/member/robotmanage/menuid/166.html'),(137,0,1,'提示语:该页面不存在！',1571191783,2130706433,'/member/member/robotmanage/menuid/166.html'),(138,1,1,'提示语:清理缓存',1571191916,2130706433,'/admin/index/cache.html?type=all&_=1571191766790'),(139,1,1,'提示语:清理缓存',1571191991,2130706433,'/admin/index/cache.html?type=all&_=1571191933799'),(140,1,1,'提示语:清理缓存',1571191994,2130706433,'/admin/index/cache.html?type=data&_=1571191933800'),(141,1,1,'提示语:清理缓存',1571191996,2130706433,'/admin/index/cache.html?type=template&_=1571191933801'),(142,1,0,'提示语:注销成功！',1571191999,2130706433,'/admin/index/logout.html'),(143,1,1,'提示语:恭喜您，登陆成功',1571192023,2130706433,'/admin/index/login.html'),(144,1,1,'提示语:清理缓存',1571192170,2130706433,'/admin/index/cache.html?type=all&_=1571192025072'),(145,1,1,'提示语:模块卸载成功！一键清理缓存后生效！',1571192229,2130706433,'/admin/module/uninstall.html?module=member'),(146,1,1,'提示语:清理缓存',1571192232,2130706433,'/admin/index/cache.html?type=all&_=1571192025073'),(147,1,1,'提示语:模块安装成功！一键清理缓存后生效！',1571192244,2130706433,'/admin/module/install.html?module=member'),(148,1,1,'提示语:清理缓存',1571192249,2130706433,'/admin/index/cache.html?type=all&_=1571192235973'),(149,0,1,'提示语:该页面不存在！',1571192257,2130706433,'/member/member/add.html'),(150,1,1,'提示语:模块卸载成功！一键清理缓存后生效！',1571192663,2130706433,'/admin/module/uninstall.html?module=member'),(151,1,1,'提示语:清理缓存',1571192666,2130706433,'/admin/index/cache.html?type=all&_=1571192252660'),(152,1,1,'提示语:模块安装成功！一键清理缓存后生效！',1571192679,2130706433,'/admin/module/install.html?module=member'),(153,1,1,'提示语:清理缓存',1571192681,2130706433,'/admin/index/cache.html?type=all&_=1571192670258'),(154,1,1,'提示语:清理缓存',1571193106,2130706433,'/admin/index/cache.html?type=all&_=1571192684883'),(155,1,1,'提示语:模块卸载成功！一键清理缓存后生效！',1571193115,2130706433,'/admin/module/uninstall.html?module=member'),(156,1,1,'提示语:清理缓存',1571193118,2130706433,'/admin/index/cache.html?type=all&_=1571192684884'),(157,1,1,'提示语:模块安装成功！一键清理缓存后生效！',1571193129,2130706433,'/admin/module/install.html?module=member'),(158,1,1,'提示语:清理缓存',1571193132,2130706433,'/admin/index/cache.html?type=all&_=1571193121420'),(159,1,1,'提示语:清理缓存',1571195371,2130706433,'/admin/index/cache.html?type=all&_=1571195368968'),(160,1,1,'提示语:添加成功！',1571195615,2130706433,'/games/games/add.html'),(161,1,1,'提示语:添加成功！',1571195628,2130706433,'/games/games/add.html'),(162,1,1,'提示语:删除成功！',1571195634,2130706433,'/games/games/delete.html?ids=300003'),(163,0,0,'提示语:请先登陆',1571201477,2130706433,'/member/robotmanage/manage/menuid/181.html'),(164,1,0,'提示语:注销成功！',1571201481,2130706433,'/admin/index/logout.html'),(165,1,1,'提示语:恭喜您，登陆成功',1571201505,2130706433,'/admin/index/login.html'),(166,1,1,'提示语:添加成功！',1571204087,2130706433,'/member/robotmanage/add.html'),(167,1,1,'提示语:添加成功！',1571204569,2130706433,'/member/robotmanage/add.html'),(168,1,1,'提示语:添加成功！',1571204596,2130706433,'/member/robotmanage/add.html'),(169,1,1,'提示语:删除成功！',1571205457,2130706433,'/member/robotmanage/delete.html?ids=4'),(170,1,1,'提示语:删除成功！',1571205638,2130706433,'/member/robotmanage/delete.html'),(171,1,1,'提示语:删除成功！',1571205695,2130706433,'/member/robotmanage/delete.html?ids=5'),(172,1,1,'提示语:删除成功！',1571206033,2130706433,'/member/robotmanage/delete.html?ids=6'),(173,1,1,'提示语:删除成功！',1571206372,2130706433,'/member/robotmanage/delete.html'),(174,1,1,'提示语:添加成功！',1571207170,2130706433,'/member/robotmanage/add.html'),(175,1,1,'提示语:删除成功！',1571207250,2130706433,'/member/robotmanage/delete.html'),(176,1,1,'提示语:添加成功！',1571207257,2130706433,'/member/robotmanage/add.html'),(177,1,1,'提示语:删除成功！',1571207287,2130706433,'/member/robotmanage/delete.html'),(178,1,1,'提示语:添加成功！',1571207849,2130706433,'/member/robotmanage/add.html'),(179,0,1,'提示语:该页面不存在！',1571207900,2130706433,'/member/robotmanage/edit.html?id=22'),(180,1,1,'提示语:更新成功！',1571208321,2130706433,'/member/robotmanage/gamedata.html?id=22'),(181,1,1,'提示语:更新成功！',1571208349,2130706433,'/member/robotmanage/gamedata.html?id=23'),(182,1,1,'提示语:加钱成功！',1571209812,2130706433,'/member/robotmanage/updateandroidmoney.html'),(183,1,1,'提示语:删除日志成功！',1571210174,2130706433,'/admin/adminlog/deletelog.html'),(184,1,1,'提示语:模块卸载成功！一键清理缓存后生效！',1571210366,2130706433,'/admin/module/uninstall.html?module=member'),(185,1,1,'提示语:清理缓存',1571210370,2130706433,'/admin/index/cache.html?type=all&_=1571209594123'),(186,1,1,'提示语:模块安装成功！一键清理缓存后生效！',1571210387,2130706433,'/admin/module/install.html?module=member'),(187,1,1,'提示语:清理缓存',1571210392,2130706433,'/admin/index/cache.html?type=all&_=1571210374026'),(188,0,1,'提示语:该页面不存在！',1571210398,2130706433,'/member/robotmanage/time/menuid/187.html'),(189,1,1,'提示语:模块卸载成功！一键清理缓存后生效！',1571210735,2130706433,'/admin/module/uninstall.html?module=member'),(190,1,1,'提示语:清理缓存',1571210738,2130706433,'/admin/index/cache.html?type=all&_=1571210411398'),(191,1,1,'提示语:模块安装成功！一键清理缓存后生效！',1571210749,2130706433,'/admin/module/install.html?module=member'),(192,1,1,'提示语:清理缓存',1571210752,2130706433,'/admin/index/cache.html?type=all&_=1571210741365'),(193,1,1,'提示语:删除成功！',1571210782,2130706433,'/member/robotmanage/delete.html?ids=23'),(194,1,1,'提示语:删除成功！',1571210805,2130706433,'/games/games/delete.html?ids=300002'),(195,0,1,'提示语:该页面不存在！',1571211658,2130706433,'/member/robottimemanage/add.html'),(196,0,1,'提示语:该时间段不存在！',1571211672,2130706433,'/member/robottimemanage/edit.html?id=undefined'),(197,1,1,'提示语:更新成功！',1571212319,2130706433,'/member/robottimemanage/edit.html?id=1'),(198,1,1,'提示语:更新成功！',1571212349,2130706433,'/member/robottimemanage/edit.html?id=1'),(199,1,1,'提示语:删除成功！',1571212480,2130706433,'/member/robottimemanage/delete.html?ids=1'),(200,0,1,'提示语:该页面不存在！',1571212577,2130706433,'/member/robottimemanage/add.html'),(201,1,1,'提示语:添加成功！',1571212732,2130706433,'/member/robottimemanage/add.html'),(202,1,1,'提示语:添加成功！',1571212748,2130706433,'/member/robottimemanage/add.html'),(203,1,1,'提示语:删除成功！',1571212755,2130706433,'/member/robottimemanage/delete.html'),(204,1,1,'提示语:添加成功！',1571212765,2130706433,'/member/robottimemanage/add.html'),(205,1,1,'提示语:恭喜您，登陆成功',1571215885,2130706433,'/admin/index/login.html'),(206,0,0,'提示语:请先登陆',1571360076,2130706433,'/admin/'),(207,1,1,'提示语:恭喜您，登陆成功',1571360093,2130706433,'/admin/index/login.html'),(208,0,1,'提示语:该模块未安装，无需卸载！',1571360596,2130706433,'/admin/module/uninstall.html?module=gameserver'),(209,1,1,'提示语:清理缓存',1571360600,2130706433,'/admin/index/cache.html?type=all&_=1571360095813'),(210,1,1,'提示语:模块安装成功！一键清理缓存后生效！',1571360687,2130706433,'/admin/module/install.html?module=gameserver'),(211,1,1,'提示语:清理缓存',1571360847,2130706433,'/admin/index/cache.html?type=all&_=1571360845902'),(212,1,0,'提示语:注销成功！',1571368096,2130706433,'/admin/index/logout.html'),(213,1,1,'提示语:恭喜您，登陆成功',1571368114,2130706433,'/admin/index/login.html'),(214,1,1,'提示语:恭喜您，登陆成功',1571379416,2130706433,'/admin/index/login.html'),(215,1,1,'提示语:删除成功！',1571379426,2130706433,'/gameserver/transferrecord/delete.html?ids=1'),(216,1,1,'提示语:删除成功！',1571379432,2130706433,'/gameserver/transferrecord/delete.html'),(217,1,1,'提示语:删除成功！',1571379513,2130706433,'/gameserver/transferrecord/delete.html'),(218,1,1,'提示语:删除成功！',1571380159,2130706433,'/member/member/delete.html?ids=23'),(219,1,1,'提示语:更新成功！',1571380360,2130706433,'/member/member/gamedata.html?id=2'),(220,1,1,'提示语:添加成功！',1571380466,2130706433,'/member/robotmanage/add.html'),(221,1,1,'提示语:删除成功！',1571380479,2130706433,'/member/robotmanage/delete.html'),(222,1,1,'提示语:删除成功！',1571380492,2130706433,'/member/robotmanage/delete.html'),(223,1,1,'提示语:加钱成功！',1571380513,2130706433,'/member/robotmanage/updateandroidmoney.html'),(224,1,1,'提示语:恭喜您，登陆成功',1571385100,2130706433,'/admin/index/login.html'),(225,0,0,'提示语:验证码输入错误！',1571445248,2130706433,'/admin/index/login.html'),(226,1,1,'提示语:恭喜您，登陆成功',1571445261,2130706433,'/admin/index/login.html'),(227,0,0,'提示语:验证码输入错误！',1571707872,2130706433,'/admin/index/login.html'),(228,0,0,'提示语:验证码输入错误！',1571707879,2130706433,'/admin/index/login.html'),(229,1,1,'提示语:恭喜您，登陆成功',1571707890,2130706433,'/admin/index/login.html'),(230,1,1,'提示语:更新成功！',1571708019,2130706433,'/games/games/edit.html?id=300001'),(231,0,0,'提示语:请先登陆',1571724031,2130706433,'/admin/'),(232,1,1,'提示语:恭喜您，登陆成功',1571724043,2130706433,'/admin/index/login.html'),(233,1,1,'提示语:恭喜您，登陆成功',1571807421,2130706433,'/admin/index/login.html'),(234,1,1,'提示语:恭喜您，登陆成功',1572316115,2130706433,'/admin/index/login.html'),(235,1,1,'提示语:解锁玩家成功！',1572316759,2130706433,'/member/member/resetstate.html'),(236,0,0,'提示语:请先登陆',1572325897,2130706433,'/member/member/manage.html?page=1&limit=10&curgamingstate=1'),(237,1,1,'提示语:恭喜您，登陆成功',1572325922,2130706433,'/admin/index/login.html'),(238,1,1,'提示语:更新成功！',1572328261,2130706433,'/member/member/gamedata.html?id=2'),(239,1,1,'提示语:解锁玩家成功！',1572328723,2130706433,'/member/member/resetstate.html'),(240,1,1,'提示语:更新成功！',1572331005,2130706433,'/gameserver/gameserver/edit.html?id=1'),(241,1,1,'提示语:更新成功！',1572331477,2130706433,'/gameserver/gameserver/edit.html?id=1'),(242,1,1,'提示语:更新成功！',1572331636,2130706433,'/gameserver/gameserver/edit.html?id=1'),(243,1,1,'提示语:更新成功！',1572331700,2130706433,'/gameserver/gameserver/edit.html?id=1'),(244,1,1,'提示语:更新成功！',1572331714,2130706433,'/gameserver/gameserver/edit.html?id=1'),(245,1,1,'提示语:恭喜您，登陆成功',1572336258,2130706433,'/admin/index/login.html'),(246,1,1,'提示语:更新成功！',1572336300,2130706433,'/gameserver/gameserver/edit.html?id=1'),(247,1,1,'提示语:更新成功！',1572338480,2130706433,'/gameserver/gameserver/edit.html?id=1'),(248,1,1,'提示语:恭喜您，登陆成功',1572396601,2130706433,'/admin/index/login.html'),(249,0,0,'提示语:请先登陆',1572404738,2130706433,'/admin/index/index.html'),(250,0,0,'提示语:请先登陆',1572412155,2130706433,'/gameserver/gameserver/manage/menuid/196.html'),(251,1,0,'提示语:注销成功！',1572412161,2130706433,'/admin/index/logout.html'),(252,1,1,'提示语:恭喜您，登陆成功',1572412175,2130706433,'/admin/index/login.html'),(253,1,1,'提示语:解锁玩家成功！',1572424378,2130706433,'/member/member/resetstate.html'),(254,1,1,'提示语:恭喜您，登陆成功',1572486575,2130706433,'/admin/index/login.html'),(255,0,1,'提示语:该模块未安装，无需卸载！',1572488836,2130706433,'/admin/module/uninstall.html?module=gameserver'),(256,1,1,'提示语:清理缓存',1572488842,2130706433,'/admin/index/cache.html?type=all&_=1572486848486'),(257,1,1,'提示语:模块安装成功！一键清理缓存后生效！',1572488963,2130706433,'/admin/module/install.html?module=gameserver'),(258,1,1,'提示语:清理缓存',1572488970,2130706433,'/admin/index/cache.html?type=all&_=1572488948122'),(259,1,1,'提示语:删除成功！',1572488985,2130706433,'/gameserver/gamerecord/delete.html?ids=2'),(260,1,1,'提示语:删除成功！',1572488987,2130706433,'/gameserver/gamerecord/delete.html?ids=3'),(261,1,1,'提示语:删除成功！',1572488989,2130706433,'/gameserver/gamerecord/delete.html?ids=4'),(262,1,1,'提示语:删除成功！',1572488998,2130706433,'/member/member/delete.html?ids=23'),(263,1,1,'提示语:删除成功！',1572489001,2130706433,'/member/member/delete.html?ids=24'),(264,1,1,'提示语:更新成功！',1572490272,2130706433,'/gameserver/accountserver/edit.html?id=1'),(265,1,1,'提示语:添加成功！',1572490478,2130706433,'/gameserver/accountserver/add.html'),(266,0,1,'提示语:请选择需要删除的服务器！',1572490678,2130706433,'/gameserver/accountserver/delete.html'),(267,0,1,'提示语:请选择需要删除的服务器！',1572490693,2130706433,'/gameserver/accountserver/delete.html'),(268,0,1,'提示语:请选择需要删除的服务器！',1572490721,2130706433,'/gameserver/accountserver/delete.html'),(269,0,1,'提示语:请选择需要删除的服务器！',1572490797,2130706433,'/gameserver/accountserver/delete.html'),(270,1,1,'提示语:删除成功！',1572490811,2130706433,'/gameserver/accountserver/delete.html?ids=2'),(271,0,0,'提示语:请先登陆',1572497702,2130706433,'/gameserver/accountserver/manage/menuid/204.html'),(272,1,0,'提示语:注销成功！',1572497708,2130706433,'/admin/index/logout.html'),(273,1,1,'提示语:恭喜您，登陆成功',1572497718,2130706433,'/admin/index/login.html'),(274,1,0,'提示语:注销成功！',1572501663,2130706433,'/admin/index/logout.html'),(275,0,0,'提示语:验证码输入错误！',1572501688,2130706433,'/admin/index/login.html'),(276,0,0,'提示语:请先登陆',1572501693,2130706433,'/gameserver/gameserver/manage/menuid/202.html'),(277,1,0,'提示语:注销成功！',1572501696,2130706433,'/admin/index/logout.html'),(278,1,1,'提示语:恭喜您，登陆成功',1572501708,2130706433,'/admin/index/login.html'),(279,1,1,'提示语:解锁玩家成功！',1572504384,2130706433,'/member/member/resetstate.html'),(280,1,1,'提示语:解锁玩家成功！',1572504513,2130706433,'/member/member/resetstate.html'),(281,1,1,'提示语:恭喜您，登陆成功',1572511531,2130706433,'/admin/index/login.html'),(282,1,1,'提示语:删除成功！',1572511543,2130706433,'/member/member/delete.html?ids=26'),(283,1,1,'提示语:清理缓存',1572513680,2130706433,'/admin/index/cache.html?type=all&_=1572513677675'),(284,1,1,'提示语:恭喜您，登陆成功',1572534995,2130706433,'/admin/index/login.html'),(285,1,1,'提示语:更新成功！',1572535009,2130706433,'/member/member/gamedata.html?id=2'),(286,0,0,'提示语:验证码输入错误！',1572569244,2130706433,'/admin/index/login.html'),(287,1,1,'提示语:恭喜您，登陆成功',1572569252,2130706433,'/admin/index/login.html'),(288,1,1,'提示语:恭喜您，登陆成功',1572670572,2130706433,'/admin/index/login.html'),(289,1,1,'提示语:更新成功！',1572672324,2130706433,'/gameserver/gameserver/edit.html?id=1'),(290,1,1,'提示语:更新成功！',1572672361,2130706433,'/gameserver/gameserver/edit.html?id=1'),(291,1,1,'提示语:更新成功！',1572672370,2130706433,'/gameserver/gameserver/edit.html?id=1'),(292,1,1,'提示语:恭喜您，登陆成功',1572673412,2130706433,'/admin/index/login.html'),(293,1,1,'提示语:恭喜您，登陆成功',1572685726,2130706433,'/admin/index/login.html'),(294,1,1,'提示语:模块卸载成功！一键清理缓存后生效！',1572685784,2130706433,'/admin/module/uninstall.html?module=member'),(295,1,1,'提示语:清理缓存',1572685789,2130706433,'/admin/index/cache.html?type=all&_=1572685728927'),(296,1,1,'提示语:模块安装成功！一键清理缓存后生效！',1572685806,2130706433,'/admin/module/install.html?module=member'),(297,1,1,'提示语:清理缓存',1572685811,2130706433,'/admin/index/cache.html?type=all&_=1572685792843'),(298,1,1,'提示语:添加成功！',1572686763,2130706433,'/member/robotchatmsg/add.html'),(299,1,1,'提示语:删除成功！',1572686774,2130706433,'/member/robotchatmsg/delete.html?ids=3'),(300,1,1,'提示语:添加成功！',1572686780,2130706433,'/member/robotchatmsg/add.html'),(301,1,1,'提示语:删除成功！',1572686787,2130706433,'/member/robotchatmsg/delete.html'),(302,1,1,'提示语:添加成功！',1572686791,2130706433,'/member/robotchatmsg/add.html'),(303,1,1,'提示语:更新成功！',1572686993,2130706433,'/member/robotchatmsg/edit.html?id=5'),(304,1,1,'提示语:恭喜您，登陆成功',1572690556,2130706433,'/admin/index/login.html'),(305,1,1,'提示语:添加成功！',1572690573,2130706433,'/member/robotchatmsg/add.html'),(306,1,1,'提示语:更新成功！',1572690582,2130706433,'/member/robotchatmsg/edit.html?id=6'),(307,1,1,'提示语:更新成功！',1572690747,2130706433,'/member/robotchatmsg/edit.html?id=6'),(308,1,1,'提示语:解锁玩家成功！',1572690897,2130706433,'/member/member/resetstate.html'),(309,1,1,'提示语:恭喜您，登陆成功',1572833230,2130706433,'/admin/index/login.html'),(310,1,1,'提示语:更新成功！',1572834274,2130706433,'/member/member/gamedata.html?id=2'),(311,1,1,'提示语:更新成功！',1572834292,2130706433,'/member/member/gamedata.html?id=2'),(312,1,1,'提示语:更新成功！',1572834350,2130706433,'/member/member/gamedata.html?id=2'),(313,1,1,'提示语:模块卸载成功！一键清理缓存后生效！',1572834566,2130706433,'/admin/module/uninstall.html?module=member'),(314,1,1,'提示语:清理缓存',1572834570,2130706433,'/admin/index/cache.html?type=all&_=1572833232380'),(315,1,1,'提示语:模块安装成功！一键清理缓存后生效！',1572834585,2130706433,'/admin/module/install.html?module=member'),(316,1,1,'提示语:清理缓存',1572834590,2130706433,'/admin/index/cache.html?type=all&_=1572834573826'),(317,1,1,'提示语:更新成功！',1572835705,2130706433,'/member/robotcontrol/manage/menuid/222.html'),(318,1,1,'提示语:更新成功！',1572835716,2130706433,'/member/robotcontrol/manage.html'),(319,1,1,'提示语:恭喜您，登陆成功',1572853942,2130706433,'/admin/index/login.html'),(320,1,1,'提示语:模块卸载成功！一键清理缓存后生效！',1572853951,2130706433,'/admin/module/uninstall.html?module=member'),(321,1,1,'提示语:清理缓存',1572853955,2130706433,'/admin/index/cache.html?type=all&_=1572853943988'),(322,1,1,'提示语:模块安装成功！一键清理缓存后生效！',1572853972,2130706433,'/admin/module/install.html?module=member'),(323,1,1,'提示语:清理缓存',1572853976,2130706433,'/admin/index/cache.html?type=all&_=1572853960690'),(324,0,1,'提示语:该代理不存在！',1572855889,2130706433,'/member/member/agent.html?id=2'),(325,1,1,'提示语:更新成功！',1572855910,2130706433,'/member/member/agent.html?id=2'),(326,1,1,'提示语:更新成功！',1572855939,2130706433,'/member/member/agent.html?id=28'),(327,0,1,'提示语:该代理不存在！',1572857543,2130706433,'/member/agentuser/edit.html?id=1'),(328,0,1,'提示语:该代理不存在！',1572857568,2130706433,'/member/agentuser/edit.html?id=1'),(329,1,1,'提示语:更新成功！',1572857658,2130706433,'/member/agentuser/edit.html?id=1'),(330,1,1,'提示语:更新成功！',1572858601,2130706433,'/member/member/agent.html?id=2'),(331,1,1,'提示语:更新成功！',1572859181,2130706433,'/member/agentuser/edit.html?id=1'),(332,1,1,'提示语:更新成功！',1572859206,2130706433,'/member/agentuser/edit.html?id=1'),(333,1,1,'提示语:删除成功！',1572859263,2130706433,'/member/agentuser/delete.html?ids=6'),(334,1,1,'提示语:删除成功！',1572859266,2130706433,'/member/agentuser/delete.html?ids=5'),(335,1,1,'提示语:恭喜您，登陆成功',1572921898,2130706433,'/admin/index/login.html'),(336,0,0,'提示语:验证码输入错误！',1573011331,2130706433,'/admin/index/login.html'),(337,1,1,'提示语:恭喜您，登陆成功',1573011344,2130706433,'/admin/index/login.html'),(338,1,1,'提示语:恭喜您，登陆成功',1573019703,2130706433,'/admin/index/login.html'),(339,1,1,'提示语:更新成功！',1573021296,2130706433,'/member/agentuser/edit.html?id=1'),(340,1,1,'提示语:更新成功！',1573021304,2130706433,'/member/agentuser/edit.html?id=4'),(341,1,1,'提示语:模块卸载成功！一键清理缓存后生效！',1573021455,2130706433,'/admin/module/uninstall.html?module=member'),(342,1,1,'提示语:清理缓存',1573021462,2130706433,'/admin/index/cache.html?type=all&_=1573019843900'),(343,1,1,'提示语:模块安装成功！一键清理缓存后生效！',1573021472,2130706433,'/admin/module/install.html?module=member'),(344,1,1,'提示语:清理缓存',1573021476,2130706433,'/admin/index/cache.html?type=all&_=1573021465028');
/*!40000 ALTER TABLE `yzn_adminlog` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `yzn_article`
--

DROP TABLE IF EXISTS `yzn_article`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `yzn_article` (
  `id` mediumint(8) unsigned NOT NULL AUTO_INCREMENT COMMENT '文档ID',
  `catid` smallint(5) unsigned NOT NULL DEFAULT '0' COMMENT '栏目ID',
  `title` varchar(255) COLLATE utf8_unicode_ci NOT NULL DEFAULT '' COMMENT '标题',
  `thumb` int(5) unsigned NOT NULL DEFAULT '0' COMMENT '缩略图',
  `flag` varchar(32) COLLATE utf8_unicode_ci NOT NULL DEFAULT '' COMMENT '属性',
  `keywords` varchar(255) COLLATE utf8_unicode_ci NOT NULL DEFAULT '' COMMENT 'SEO关键词',
  `description` varchar(255) COLLATE utf8_unicode_ci NOT NULL DEFAULT '' COMMENT 'SEO描述',
  `tags` varchar(255) COLLATE utf8_unicode_ci NOT NULL DEFAULT '' COMMENT 'Tags标签',
  `listorder` smallint(5) unsigned NOT NULL DEFAULT '100' COMMENT '排序',
  `uid` mediumint(8) unsigned NOT NULL DEFAULT '0' COMMENT '用户id',
  `username` varchar(20) COLLATE utf8_unicode_ci NOT NULL DEFAULT '' COMMENT '用户名',
  `sysadd` tinyint(1) unsigned NOT NULL DEFAULT '0' COMMENT '是否后台添加',
  `hits` mediumint(8) unsigned DEFAULT '0' COMMENT '点击量',
  `inputtime` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '创建时间',
  `updatetime` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '更新时间',
  `status` tinyint(2) NOT NULL DEFAULT '0' COMMENT '状态',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=7 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='文章模型模型表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `yzn_article`
--

LOCK TABLES `yzn_article` WRITE;
/*!40000 ALTER TABLE `yzn_article` DISABLE KEYS */;
INSERT INTO `yzn_article` VALUES (1,9,'让客户留住更长时间访问你的网站',0,'','','什么能让您的客户“一见钟情”？除了网站的界面，没有其他因素。网站的界面是非常重要的因素之一。因为这是客户访问网站时的第一印象。那时，您需要为客户提供一个吸引人且引人注目的界面。要做到这一点非常容易，你只需要有一个合理布局的界面，整洁不要分散读者的注意力。在与网站互动时，客户可以轻松搜索他们需要学习的信息。此外，您还可以使用一些额外的注释来使界面更加美观：首先，效果的最大效果用于避免分散用户的注意力。这些效果甚至会使网站更重，并且加载速度更慢。其次，您可以创建更多可用空间并消除不重要的信息，从而使关键消息更','',100,1,'admin',1,0,1550188136,1550476672,1),(2,9,'移动网站需要吸引哪些观众并将其转化为客户',0,'','','在移动设备上设计网站以吸引观众并使他们成为他们的客户并不容易。移动网站是否只有两个友好元素，下载速度是否足够快？使用移动设备访问网站的人是那些时间很少的人，所以他们总是希望事情快速而正确。','',100,1,'admin',1,0,1550202861,1550450153,1),(3,14,'空壳网站是什么？如何避免成为空壳网站？空壳网站怎么处理？',0,'','','一、备案数据，包括：主体信息、网站信息、接入信息。\r\n（1）主体信息是指，网站主办者（网站开办者）的注册信息。\r\n（2）网站信息是指，网站主办者开办的（一个或多个）网站的注册信息。\r\n（3）接入信息是指，网站主办者（每个）网站的数据存放的虚拟空间的接入信息。','',100,1,'admin',1,0,1550448808,1550476816,1),(4,14,'单位或网站涉及金融类关键词，办理网站备案注意事项',0,'','','1.根据《国务院办公厅关于印发互联网金融风险专项整治工作实施方案的通知（国办发〔2016〕21号）》要求，公司注册名称或经营范围中使用“交易所”、“交易中心”、“金融”、“资产管理”、“理财”、“基金”、“基金管理”、“投资管理（包括投资）”、“财富管理”、“股权投资基金”、“网贷”、“网络借贷”、“P2P”、“股权众筹”、“互联网保险”、“支付”、“信托”等字样的企业，在做网站备案业务办理时，需提供金融管理部门的专项审批文件。','',100,1,'admin',1,0,1550449235,1550449733,1),(5,10,'个人建设网站有哪些步骤？',0,'','','虽然互联网上付费提供网站建设和网站制作服务的公司或者个人有很多，都是为企业或者个人提供网站建设和网页设计服务的，但是对于那些刚刚走出校门或者刚刚参加工作的朋友来说，如果想通过互联网创业，想要做一个自己的网站，但是又没有明确的经营理念，只是想要尝试一下互联网创业，这时候大部分人都会选择自己建网站，一方面是为了能够节省较高的网站建设费用，另一方面也可以简单的学习一些网站建设或网站制作的一些基本知识，那么自己建网站到底应该如何入手呢今天小编就跟大家写一篇自己建网站的全攻略，希望能够帮助那些想要自己建网站的朋友有','',100,1,'admin',1,0,1550449817,1550476910,1),(6,10,'企业建设手机网站注意的事项？',0,'','',' 虽然很多企业都专门弄起了APP软件，不过从综合层面来说，还是网站更加靠谱一些，因为网站比制作APP成本要低廉很多，而且受传统思维习惯的影响，大部分的会主动寻找相关内容的人来说，他们还是更加习惯利用搜索引擎去进行寻找。并且这一群人在社会上面也拥有一定的社会经验以及地位，像是销售人员、采购人员等等，如果他们不再办公室，正好在上班途中或者是出差途中的话，肯定是需要使用手机来搜索某些信息的，所以从实用性角度来看的话，反倒是企业网站比APP更好一些，那么，企业建设手机网站的时候要注意什么?','',100,1,'admin',1,0,1550450424,1550476700,1);
/*!40000 ALTER TABLE `yzn_article` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `yzn_article_data`
--

DROP TABLE IF EXISTS `yzn_article_data`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `yzn_article_data` (
  `did` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `content` text COLLATE utf8_unicode_ci COMMENT '内容',
  PRIMARY KEY (`did`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='文章模型模型表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `yzn_article_data`
--

LOCK TABLES `yzn_article_data` WRITE;
/*!40000 ALTER TABLE `yzn_article_data` DISABLE KEYS */;
INSERT INTO `yzn_article_data` VALUES (1,'&lt;p&gt;&lt;strong&gt;通过网站让客户“一见钟情”&lt;/strong&gt;&lt;/p&gt;&lt;p&gt;什么能让您的客户“一见钟情”？除了网站的界面，没有其他因素。网站的界面是非常重要的因素之一。因为这是客户访问网站时的第一印象。那时，您需要为客户提供一个吸引人且引人注目的界面。要做到这一点非常容易，你只需要有一个合理布局的界面，整洁不要分散读者的注意力。在与网站互动时，客户可以轻松搜索他们需要学习的信息。此外，您还可以使用一些额外的注释来使界面更加美观：首先，效果的最大效果用于避免分散用户的注意力。这些效果甚至会使网站更重，并且加载速度更慢。其次，您可以创建更多可用空间并消除不重要的信息，从而使关键消息更容易，更快地到达客户。&amp;nbsp;&lt;/p&gt;&lt;p&gt;&lt;br/&gt;&lt;/p&gt;&lt;p&gt;&lt;strong&gt;内容不仅要有回报，还应该精美呈现&lt;/strong&gt;&lt;/p&gt;&lt;p&gt;对客户有用的内容是让客户保持更长时间的首要因素之一。但是，不仅昂贵的信息足够，内容的呈现和格式是您的网站有更长的时间留在客户的技巧。您可以设计一个白色背景的网站，以便所有信息变得更加突出。绝对不要使用色彩鲜艳的花朵和图案的深色背景，因为它可能使读者难以获取信息。此外，字体用法和段落间距同样重要。选择的字体不应该太挑剔，时尚，但应该是简单，易于看到的字体和显示专业性。附加，线条之间应该是合理的距离，内容布局的段落更加开放。&lt;/p&gt;&lt;p&gt;&lt;br/&gt;&lt;/p&gt;&lt;p&gt;&lt;strong&gt;优化网站以与所有设备兼容&lt;/strong&gt;&lt;/p&gt;&lt;p&gt;通常情况下，企业只能优化显示在计算机或笔记本电脑上，但往往会忽略各种其他重要设备，如智能手机或平板电脑等。但是，用户数量的情况随着移动设备的访问越来越多，新网站可确保与设备（包括移动设备）的兼容性。&amp;nbsp;&lt;/p&gt;&lt;p&gt;&lt;br/&gt;&lt;/p&gt;&lt;p&gt;&lt;strong&gt;适当地浏览网站中的信息&lt;/strong&gt;&lt;/p&gt;&lt;p&gt;如果您通过主页给客户留下了深刻印象，客户一直渴望了解您的业务。为了使此过程更好地运行，您需要确保子页面的所有链接与前面提到的链接标题的内容一致。\r\n您还可以为关键位置的内容创建重音，以提高点击率。而且您也不要忘记确保您没有基本错误，例如链接到损坏的页面，丢失图像甚至丢失链接。&lt;/p&gt;&lt;p&gt;&lt;br/&gt;&lt;/p&gt;&lt;p&gt;&lt;strong&gt;定期更新网站内容&lt;/strong&gt;&lt;/p&gt;&lt;p&gt;内容不需要质量，但在网站上也需要有数量。这里的金额并不意味着猖獗的金额，而是每天都是偶数。如果客户返回您的网站但仍然是旧内容，则您可能会失去客户，因为客户不想返回网站更新旧内容。\r\n有了这些提示，您需要立即更新缺少的元素以完成网站并留住客户。做好这些事情后，您会很快注意到您网站的跳出率大&lt;/p&gt;'),(2,'&lt;p&gt;在移动设备上设计网站以吸引观众并使他们成为他们的客户并不容易。移动网站是否只有两个友好元素，下载速度是否足够快？&lt;br/&gt;&lt;/p&gt;&lt;p&gt;使用移动设备访问网站的人是那些时间很少的人，所以他们总是希望事情快速而正确。&lt;/p&gt;&lt;p&gt;&lt;br/&gt;&lt;/p&gt;&lt;p&gt;&lt;strong&gt;1：并非计算机上显示的所有内容都需要显示在移动设备上&lt;/strong&gt;&lt;/p&gt;&lt;p&gt;很容易看出微小的移动屏幕不能像计算机那样宽，因此需要选择出现在移动屏幕上的网站内容。重要内容，您需要将它们推上去，以便它们可以显示在移动屏幕上&lt;/p&gt;&lt;p&gt;还需要选择移动屏幕上显示的网站内容&lt;br/&gt;&lt;br/&gt;&lt;/p&gt;&lt;p&gt;&lt;strong&gt;2：网站下载速度&lt;/strong&gt;&lt;/p&gt;&lt;p&gt;专业网站设计师应该始终关注的一件事是下载网站的速度。根据一项研究，谷歌正在研究，53％的用户将离开一个网站，如果下载需要超过3秒。提高网站的下载速度有时只是为了删除图像，减少图像的大小是网站可以更快下载。但是，有时原因比我们想象的更复杂，例如，原因来自网站代码，或者可能是因为您的网站开发的内容远远超过原始网站，而且当前主机不再响应了。&lt;/p&gt;&lt;p&gt;所以你需要找出你的网站下载速度慢的原因以及在哪里立即修复它。&lt;/p&gt;&lt;p&gt;&lt;br/&gt;&lt;/p&gt;&lt;p&gt;&lt;strong&gt;3：添加通话按钮&lt;/strong&gt;&lt;/p&gt;&lt;p&gt;对于移动用户，尤其是移动用户访问网站，他们说话的时间非常重要。因此，在查看产品信息之后，他们会立即打电话询问产品。但你确定他们会耐心等待撤回并找到你的电话号码吗？绝对不是。现在，移动屏幕上始终可用的简单呼叫按钮是可以立即按下客户想要呼叫的最全面的解决方案。&lt;/p&gt;&lt;p&gt;&lt;br/&gt;&lt;/p&gt;&lt;p&gt;&lt;strong&gt;4：集成返回顶部按钮&lt;/strong&gt;&lt;/p&gt;&lt;p&gt;对于网站，菜单始终是一个重要的导航栏，可帮助用户导航到网站内的子页面。对于某些网站，当用户向下滚动并阅读下面的内容时，此菜单栏将始终显示在屏幕上。但是，其他一些网站没有。因此，当用户在网站底部附近阅读时，想要查看菜单，他们必须进行大量冲浪，现在，返回顶部按钮将非常有效并让用户感到舒适。&lt;/p&gt;&lt;p&gt;&lt;br/&gt;&lt;/p&gt;&lt;p&gt;&lt;strong&gt;5：网站上的菜单&lt;/strong&gt;&lt;/p&gt;&lt;p&gt;与网站显示在计算机上的不同，手机上显示的网站菜单将减少到一行。当您想要查看时，用户将单击以显示子菜单。菜单图标现在是3个图块，但不是每个人都知道图标是菜单，因此如果您想让它更容易理解，您可以立即编写菜单字母。&lt;/p&gt;'),(6,'&lt;p&gt;&amp;nbsp; &amp;nbsp; &amp;nbsp; &amp;nbsp;虽然很多企业都专门弄起了APP软件，不过从综合层面来说，还是网站更加靠谱一些，因为网站比制作APP成本要低廉很多，而且受传统思维习惯的影响，大部分的会主动寻找相关内容的人来说，他们还是更加习惯利用搜索引擎去进行寻找。并且这一群人在社会上面也拥有一定的社会经验以及地位，像是销售人员、采购人员等等，如果他们不再办公室，正好在上班途中或者是出差途中的话，肯定是需要使用手机来搜索某些信息的，所以从实用性角度来看的话，反倒是企业网站比APP更好一些，那么，企业建设手机网站的时候要注意什么?&lt;/p&gt;&lt;p&gt;&lt;br/&gt;&lt;/p&gt;&lt;p&gt;&lt;strong&gt;&amp;nbsp; &amp;nbsp; &amp;nbsp; &amp;nbsp;第一、图片设计&lt;/strong&gt;&lt;/p&gt;&lt;p&gt;  虽然当前的手机企业网站在建设的时候都是弄成的响应式网站，它可以按照上网具体设备的不同，而自动调整成符合当前屏幕大小的格式，但是只要稍微留心一些，就会发现，就算是那些超级大型的网站，他们在图片处理方面都已经十分谨慎了，还是会出现因为图片出现一些问题，因为只要出现图片就会消耗流量，另外如果图片太多的话，也会导致企业网站页面的加载速度变得非常慢，导致用户体验严重受到影响，因此在不是特别有必要的情况之下，最好还是少使用图片比较合适。&lt;/p&gt;&lt;p&gt;&lt;br/&gt;&lt;/p&gt;&lt;p&gt;  &lt;strong&gt;第二、页面简洁&lt;/strong&gt;&lt;/p&gt;&lt;p&gt;  既然是建设手机企业网站，那在设计的时候建议还是弄得简单一些更合适，不需要像电脑PC端的网站一样弄很多的内容，因为手机本身的屏幕就要比PC端小很多，如果手机企业网站建设的时候设计很多的内容，会导致人们浏览起来变得比较困难的，特别是用内容作为主导倾向的网站，建设成简洁的形式，更加容易让网友找到自己需要的信息&lt;/p&gt;&lt;p&gt;&lt;br/&gt;&lt;/p&gt;&lt;p&gt;  &lt;strong&gt;第三、断点功能&lt;/strong&gt;&lt;/p&gt;&lt;p&gt;  对于移动网站的断点设置，在CSS模式样式中，都支持断点功能设置，而传统PC端网站就缺少这个功能设置，所以经常会出现网站显示不合理，大量乱码等现象。但是，网站断点功能并非就保证网站访问流畅，对于断点技术的研究，还在进一步探讨中，比如说，在移动设备显示不错的网站，可是反过来用PC端却显示紊乱，在特别注重移动端网站的时候，也要注意到传统网站的感受，只要这样全兼容的设计，才符合未来网站的发展方向。&lt;/p&gt;&lt;p&gt;&lt;br/&gt;&lt;/p&gt;'),(3,'&lt;p style=&quot;margin-top: 0px; margin-bottom: 0px; padding: 3px 5px; word-break: break-all; letter-spacing: 1px; text-indent: 0em; line-height: 22px; color: rgb(90, 90, 90); &quot;&gt;&lt;strong&gt;&lt;span style=&quot;font-size: 16px; text-indent: 0em;&quot;&gt;一、备案数据，包括：主体信息、网站信息、接入信息。&lt;/span&gt;&lt;/strong&gt;&lt;/p&gt;&lt;p style=&quot;margin-top: 0px; margin-bottom: 0px; padding: 3px 5px; word-break: break-all; letter-spacing: 1px; text-indent: 0em; line-height: 22px; color: rgb(90, 90, 90);&quot;&gt;&lt;span style=&quot;margin: 0px; padding: 0px; font-size: 14px;&quot;&gt;（1）主体信息是指，网站主办者（网站开办者）的注册信息。&lt;/span&gt;&lt;/p&gt;&lt;p style=&quot;margin-top: 0px; margin-bottom: 0px; padding: 3px 5px; word-break: break-all; letter-spacing: 1px; text-indent: 0em; line-height: 22px; color: rgb(90, 90, 90);&quot;&gt;&lt;span style=&quot;font-size: 14px; text-indent: 0em;&quot;&gt;&lt;span style=&quot;line-height: 22px;&quot;&gt;（2）&lt;/span&gt;网站信息是指，网站主办者开办的（一个或多个）网站的注册信息。&lt;/span&gt;&lt;/p&gt;&lt;p style=&quot;margin-top: 0px; margin-bottom: 0px; padding: 3px 5px; word-break: break-all; letter-spacing: 1px; text-indent: 0em; line-height: 22px; color: rgb(90, 90, 90); font-size: 13px; &quot;&gt;&lt;span style=&quot;margin: 0px; padding: 0px; font-size: 14px;&quot;&gt;&lt;span style=&quot;line-height: 22px;&quot;&gt;（3）&lt;/span&gt;接入信息是指，网站主办者（每个）网站的数据存放的虚拟空间的接入信息。&lt;/span&gt;&lt;/p&gt;&lt;p style=&quot;margin-top: 0px; margin-bottom: 0px; padding: 3px 5px; word-break: break-all; letter-spacing: 1px; text-indent: 0em; line-height: 22px; color: rgb(90, 90, 90); font-size: 13px; &quot;&gt;&lt;span style=&quot;margin: 0px; padding: 0px; font-size: 14px;&quot;&gt;&lt;br/&gt;&lt;/span&gt;&lt;/p&gt;&lt;p style=&quot;margin-top: 0px; margin-bottom: 0px; padding: 3px 5px; word-break: break-all; letter-spacing: 1px; text-indent: 0em; line-height: 22px; color: rgb(90, 90, 90); font-size: 13px; &quot;&gt;&lt;strong&gt;&lt;span style=&quot;margin: 0px; padding: 0px; font-size: medium;&quot;&gt;&lt;span style=&quot;margin: 0px; padding: 0px; font-size: 14px;&quot;&gt;&lt;span style=&quot;margin: 0px; padding: 0px; font-size: 16px; font-family: 黑体, simhei;&quot;&gt;二、空壳类备案数据，包括:空壳主体和空壳网站。&lt;/span&gt;&lt;/span&gt;&lt;/span&gt;&lt;/strong&gt;&lt;/p&gt;&lt;p style=&quot;margin-top: 0px; margin-bottom: 0px; padding: 3px 5px; word-break: break-all; letter-spacing: 1px; text-indent: 0em; line-height: 22px; color: rgb(90, 90, 90); font-size: 13px; &quot;&gt;&lt;span style=&quot;margin: 0px; padding: 0px; font-size: 14px;&quot;&gt;&lt;span style=&quot;line-height: 22px;&quot;&gt;（1）&lt;/span&gt;空壳主体是指，在工业和信息化部备案系统中，网站主办者的历史备案信息只存在主体信息，没有网站信息和接入信息。&lt;/span&gt;&lt;/p&gt;&lt;p style=&quot;margin-top: 0px; margin-bottom: 0px; padding: 3px 5px; word-break: break-all; letter-spacing: 1px; text-indent: 0em; line-height: 22px; color: rgb(90, 90, 90); font-size: 13px; &quot;&gt;&lt;span style=&quot;margin: 0px; padding: 0px; font-size: 14px;&quot;&gt;&lt;span style=&quot;line-height: 22px;&quot;&gt;（2）&lt;/span&gt;空壳网站是指，在工业和信息化部备案系统中，网站主办者的历史备案信息中含有主体信息和网站信息，但（一个或多个网站）没有接入信息（即网站有备案号，但由于网站实际使用空间IP地址变更，之前空间接入商已将网站的备案信息取消接入，同时网站主办者并没有在新的空间接入商办理备案信息转接入）。&lt;/span&gt;&lt;/p&gt;&lt;p style=&quot;margin-top: 0px; margin-bottom: 0px; padding: 3px 5px; word-break: break-all; letter-spacing: 1px; text-indent: 0em; line-height: 22px; color: rgb(90, 90, 90); font-size: 13px; &quot;&gt;&lt;span style=&quot;margin: 0px; padding: 0px; font-size: 14px;&quot;&gt;&amp;nbsp; &amp;nbsp; &amp;nbsp; 通俗来讲，空壳网站是指，用户域名绑定IP发生变更（主要是更换了不同空间接入商IP），但备案信息没有及时变更，因此就变成了空壳网站。&lt;/span&gt;&lt;/p&gt;&lt;p style=&quot;margin-top: 0px; margin-bottom: 0px; padding: 3px 5px; word-break: break-all; letter-spacing: 1px; text-indent: 0em; line-height: 22px; color: rgb(90, 90, 90); font-size: 13px; &quot;&gt;&lt;span style=&quot;margin: 0px; padding: 0px; font-size: 14px;&quot;&gt;&lt;br/&gt;&lt;/span&gt;&lt;/p&gt;&lt;p style=&quot;margin-top: 0px; margin-bottom: 0px; padding: 3px 5px; word-break: break-all; letter-spacing: 1px; text-indent: 0em; line-height: 22px; color: rgb(90, 90, 90); font-size: 13px; &quot;&gt;&lt;strong&gt;&lt;span style=&quot;margin: 0px; padding: 0px; font-size: medium;&quot;&gt;&lt;span style=&quot;margin: 0px; padding: 0px; font-size: 16px; font-family: 黑体, simhei;&quot;&gt;三、&lt;/span&gt;&lt;/span&gt;&lt;span style=&quot;margin: 0px; padding: 0px; font-size: medium;&quot;&gt;&lt;span style=&quot;margin: 0px; padding: 0px; font-size: 14px;&quot;&gt;&lt;/span&gt;&lt;span style=&quot;margin: 0px; padding: 0px; font-size: 14px;&quot;&gt;&lt;/span&gt;&lt;span style=&quot;margin: 0px; padding: 0px; font-size: 16px; font-family: 黑体, simhei;&quot;&gt;空壳类备案数据处理方式。&lt;/span&gt;&lt;/span&gt;&lt;/strong&gt;&lt;/p&gt;&lt;p style=&quot;margin-top: 0px; margin-bottom: 0px; padding: 3px 5px; word-break: break-all; letter-spacing: 1px; text-indent: 0em; line-height: 22px; color: rgb(90, 90, 90); font-size: 13px; &quot;&gt;&lt;span style=&quot;margin: 0px; padding: 0px; font-size: 14px;&quot;&gt;&lt;span style=&quot;line-height: 22px;&quot;&gt;（1）&lt;/span&gt;若网站主办者存在空壳主体信息，则唯一解决方式：&lt;/span&gt;&lt;/p&gt;&lt;p style=&quot;margin-top: 0px; margin-bottom: 0px; padding: 3px 5px; word-break: break-all; letter-spacing: 1px; text-indent: 0em; line-height: 22px; color: rgb(90, 90, 90); font-size: 13px; &quot;&gt;&lt;span style=&quot;margin: 0px; padding: 0px; font-size: medium;&quot;&gt;&lt;/span&gt;&lt;span style=&quot;font-size:&quot;&gt;&amp;nbsp; &amp;nbsp; &amp;nbsp; 需网站主办者携带相关证件到网站实际的接入商重新办理备案。已被注销（收回）的备案号及备案信息无法恢复。&lt;/span&gt;&lt;/p&gt;&lt;p style=&quot;margin-top: 0px; margin-bottom: 0px; padding: 3px 5px; word-break: break-all; letter-spacing: 1px; text-indent: 0em; line-height: 22px; color: rgb(90, 90, 90); font-size: 13px; &quot;&gt;&lt;span style=&quot;font-size:&quot;&gt;&lt;br/&gt;&lt;/span&gt;&lt;/p&gt;&lt;p style=&quot;margin-top: 0px; margin-bottom: 0px; padding: 3px 5px; word-break: break-all; letter-spacing: 1px; text-indent: 0em; line-height: 22px; color: rgb(90, 90, 90); font-size: 13px; &quot;&gt;&lt;strong&gt;&lt;span style=&quot;margin: 0px; padding: 0px; font-size: medium;&quot;&gt;&lt;span style=&quot;margin: 0px; padding: 0px; font-size: 16px; font-family: 黑体, simhei;&quot;&gt;四、&lt;/span&gt;&lt;/span&gt;&lt;span style=&quot;margin: 0px; padding: 0px; font-size: medium;&quot;&gt;&lt;span style=&quot;margin: 0px; padding: 0px; font-size: 14px;&quot;&gt;&lt;/span&gt;&lt;span style=&quot;margin: 0px; padding: 0px; font-size: 16px; font-family: 黑体, simhei;&quot;&gt;如何避免成为空壳类数据。&lt;/span&gt;&lt;/span&gt;&lt;/strong&gt;&lt;/p&gt;&lt;p style=&quot;margin-top: 0px; margin-bottom: 0px; padding: 3px 5px; word-break: break-all; letter-spacing: 1px; text-indent: 0em; line-height: 22px; color: rgb(90, 90, 90); font-size: 13px; &quot;&gt;&lt;span style=&quot;margin: 0px; padding: 0px; font-size: 14px;&quot;&gt;&lt;span style=&quot;line-height: 22px;&quot;&gt;（1）&lt;/span&gt;不可随意变更域名绑定IP（若需变更请及时联系网站实际使用的空间接入商）；&lt;/span&gt;&lt;/p&gt;&lt;p style=&quot;margin-top: 0px; margin-bottom: 0px; padding: 3px 5px; word-break: break-all; letter-spacing: 1px; text-indent: 0em; line-height: 22px; color: rgb(90, 90, 90); font-size: 13px; &quot;&gt;&lt;span style=&quot;margin: 0px; padding: 0px; font-size: medium;&quot;&gt;&lt;/span&gt;&lt;span style=&quot;font-size:&quot;&gt;以此避免因未及时变更网站备案接入信息，而成为空壳类备案数据，从而被当地省通信管理局注销（收回）备案号。&lt;/span&gt;&lt;/p&gt;&lt;p style=&quot;margin-top: 0px; margin-bottom: 0px; padding: 3px 5px; word-break: break-all; letter-spacing: 1px; text-indent: 0em; line-height: 22px; color: rgb(90, 90, 90); font-size: 13px; &quot;&gt;&lt;span style=&quot;font-size:&quot;&gt;&lt;br/&gt;&lt;/span&gt;&lt;/p&gt;&lt;p style=&quot;margin-top: 0px; margin-bottom: 0px; padding: 3px 5px; word-break: break-all; letter-spacing: 1px; text-indent: 0em; line-height: 22px; color: rgb(90, 90, 90); font-size: 13px; &quot;&gt;&lt;strong&gt;&lt;span style=&quot;margin: 0px; padding: 0px; font-size: medium;&quot;&gt;&lt;span style=&quot;margin: 0px; padding: 0px; font-size: 16px; font-family: 黑体, simhei;&quot;&gt;五、&lt;/span&gt;&lt;/span&gt;&lt;span style=&quot;margin: 0px; padding: 0px; font-size: medium;&quot;&gt;&lt;span style=&quot;margin: 0px; padding: 0px; font-size: 14px;&quot;&gt;&lt;/span&gt;&lt;span style=&quot;line-height: 22px; text-indent: 24px;&quot;&gt;&lt;span style=&quot;margin: 0px; padding: 0px; font-size: 16px; line-height: 22px;&quot;&gt;办理&lt;/span&gt;&lt;/span&gt;&lt;span style=&quot;margin: 0px; padding: 0px; font-size: 16px; font-family: 黑体, simhei;&quot;&gt;网站备案真实性核验&lt;/span&gt;&lt;span style=&quot;margin: 0px; padding: 0px; font-size: 16px; font-family: 黑体, simhei;&quot;&gt;，&lt;/span&gt;&lt;/span&gt;&lt;span style=&quot;margin: 0px; padding: 0px; font-size: medium;&quot;&gt;&lt;span style=&quot;margin: 0px; padding: 0px; font-size: 14px;&quot;&gt;&lt;/span&gt;&lt;span style=&quot;margin: 0px; padding: 0px;&quot;&gt;&lt;span style=&quot;margin: 0px; padding: 0px; font-size: 14px;&quot;&gt;请网站负责人携带以下材料&lt;/span&gt;&lt;/span&gt;&lt;/span&gt;&lt;span style=&quot;font-size: medium; margin: 0px; padding: 0px;&quot;&gt;：&lt;/span&gt;&lt;/strong&gt;&lt;/p&gt;&lt;p style=&quot;margin-top: 0px; margin-bottom: 0px; padding: 3px 5px; word-break: break-all; letter-spacing: 1px; text-indent: 0em; line-height: 22px; color: rgb(90, 90, 90); font-size: 13px; &quot;&gt;&lt;span style=&quot;margin: 0px; padding: 0px; font-size: 14px;&quot;&gt;&lt;span style=&quot;line-height: 22px;&quot;&gt;（1）&lt;/span&gt;本人身份证原件&lt;/span&gt;&lt;/p&gt;&lt;p style=&quot;margin-top: 0px; margin-bottom: 0px; padding: 3px 5px; word-break: break-all; letter-spacing: 1px; text-indent: 0em; line-height: 22px; color: rgb(90, 90, 90); font-size: 13px; &quot;&gt;&lt;span style=&quot;margin: 0px; padding: 0px; font-size: 14px;&quot;&gt;&lt;span style=&quot;line-height: 22px;&quot;&gt;（2）&lt;/span&gt;单位有效证件（含年检页）原件&lt;/span&gt;&lt;/p&gt;&lt;p style=&quot;margin-top: 0px; margin-bottom: 0px; padding: 3px 5px; word-break: break-all; letter-spacing: 1px; text-indent: 0em; line-height: 22px; color: rgb(90, 90, 90); font-size: 13px; &quot;&gt;&lt;span style=&quot;margin: 0px; padding: 0px; font-size: 14px;&quot;&gt;&lt;span style=&quot;line-height: 22px;&quot;&gt;（3）&lt;/span&gt;企业法人身份证原件&lt;/span&gt;&lt;/p&gt;&lt;p style=&quot;margin-top: 0px; margin-bottom: 0px; padding: 3px 5px; word-break: break-all; letter-spacing: 1px; text-indent: 0em; line-height: 22px; color: rgb(90, 90, 90); font-size: 13px; &quot;&gt;&lt;span style=&quot;margin: 0px; padding: 0px; font-size: 14px;&quot;&gt;&lt;span style=&quot;line-height: 22px;&quot;&gt;（4）&lt;/span&gt;单位公章&lt;/span&gt;&lt;/p&gt;&lt;p&gt;&lt;br/&gt;&lt;/p&gt;'),(4,'&lt;p style=&quot;margin-top: 0px; margin-bottom: 0px; padding: 0px; word-break: break-all; color: rgb(102, 102, 102); font-size: 14px; letter-spacing: 1px; white-space: normal; background-color: rgb(255, 255, 255); text-indent: 2em;&quot;&gt;&lt;span style=&quot;color: rgb(128, 128, 128); font-variant-east-asian: normal; line-height: 22px; widows: 1;&quot;&gt;1.根据《国务院办公厅关于印发互联网金融风险专项整治工作实施方案的通知（国办发〔2016〕21号）》要求，公司注册名称或经营范围中使用“交易所”、“交易中心”、“金融”、“资产管理”、“理财”、“基金”、“基金管理”、“投资管理（包括投资）”、“财富管理”、“股权投资基金”、“网贷”、“网络借贷”、“P2P”、“股权众筹”、“互联网保险”、“支付”、“信托”等字样的企业，在做网站备案业务办理时，需提供金融管理部门的专项审批文件。&lt;/span&gt;&lt;/p&gt;&lt;p style=&quot;margin-top: 0px; margin-bottom: 0px; padding: 0px; word-break: break-all; color: rgb(102, 102, 102); font-size: 14px; letter-spacing: 1px; white-space: normal; background-color: rgb(255, 255, 255); text-indent: 2em;&quot;&gt;&lt;span style=&quot;color: rgb(128, 128, 128); font-variant-east-asian: normal; line-height: 22px; widows: 1;&quot;&gt;2.无相关金融许可的不允许接入。若网站内容确实和金融活动无关的，需要用户更改公司注册名称或经营范围，否则不予备案。&lt;/span&gt;&lt;/p&gt;&lt;p style=&quot;margin-top: 0px; margin-bottom: 0px; padding: 0px; word-break: break-all; color: rgb(102, 102, 102); font-size: 14px; letter-spacing: 1px; white-space: normal; background-color: rgb(255, 255, 255); text-indent: 2em;&quot;&gt;&lt;span style=&quot;color: rgb(128, 128, 128); font-variant-east-asian: normal; line-height: 22px; widows: 1;&quot;&gt;&lt;span style=&quot;font-variant-numeric: normal; font-variant-east-asian: normal; line-height: 22px;&quot;&gt;3.对于上述存量网站，景安备案中心将会不定期进行核查，一旦发现违规从事金融活动，将直接予以注销备案号处置。&lt;/span&gt;&lt;/span&gt;&lt;/p&gt;&lt;p style=&quot;margin-top: 0px; margin-bottom: 0px; padding: 0px; word-break: break-all; color: rgb(102, 102, 102); font-size: 14px; letter-spacing: 1px; white-space: normal; background-color: rgb(255, 255, 255); text-indent: 2em;&quot;&gt;&lt;span style=&quot;color: rgb(128, 128, 128); font-variant-east-asian: normal; line-height: 22px; widows: 1;&quot;&gt;&lt;span style=&quot;font-variant-numeric: normal; font-variant-east-asian: normal; line-height: 22px;&quot;&gt;4.（仅供参考）涉及金融类业务&lt;/span&gt;&lt;/span&gt;&lt;span style=&quot;color: rgb(128, 128, 128); font-family: 微软雅黑, tahoma, 宋体; widows: 1; text-indent: 2em;&quot;&gt;相关许可证办理部门：&lt;/span&gt;&lt;/p&gt;&lt;p style=&quot;margin-top: 0px; margin-bottom: 0px; padding: 0px; word-break: break-all; color: rgb(102, 102, 102); font-size: 14px; letter-spacing: 1px; white-space: normal; background-color: rgb(255, 255, 255); text-indent: 2em;&quot;&gt;&lt;span style=&quot;color: rgb(128, 128, 128); font-variant-east-asian: normal; line-height: 22px; widows: 1;&quot;&gt;&lt;span style=&quot;text-indent: 32px;&quot;&gt;&amp;nbsp; ①&amp;nbsp;&lt;/span&gt;p2p网站需要金融办和银监会两家一起发的许可证；&lt;/span&gt;&lt;/p&gt;&lt;p style=&quot;margin-top: 0px; margin-bottom: 0px; padding: 0px; word-break: break-all; color: rgb(102, 102, 102); font-size: 14px; letter-spacing: 1px; white-space: normal; background-color: rgb(255, 255, 255); text-indent: 2em;&quot;&gt;&lt;span style=&quot;color: rgb(128, 128, 128); font-variant-east-asian: normal; line-height: 22px; widows: 1;&quot;&gt;&lt;span style=&quot;text-indent: 32px;&quot;&gt;&amp;nbsp; ②&amp;nbsp;&lt;/span&gt;股票、公募基金是证监会发的证；&lt;/span&gt;&lt;/p&gt;&lt;p style=&quot;margin-top: 0px; margin-bottom: 0px; padding: 0px; word-break: break-all; color: rgb(102, 102, 102); font-size: 14px; letter-spacing: 1px; white-space: normal; background-color: rgb(255, 255, 255); text-indent: 2em;&quot;&gt;&lt;span style=&quot;color: rgb(128, 128, 128); font-variant-east-asian: normal; line-height: 22px; widows: 1;&quot;&gt;&lt;span style=&quot;text-indent: 32px;&quot;&gt;&amp;nbsp; ③&amp;nbsp;&lt;/span&gt;私募基金是证券协会的备案（股权投资指的就是私募基金）；&lt;/span&gt;&lt;/p&gt;&lt;p style=&quot;margin-top: 0px; margin-bottom: 0px; padding: 0px; word-break: break-all; color: rgb(102, 102, 102); font-size: 14px; letter-spacing: 1px; white-space: normal; background-color: rgb(255, 255, 255); text-indent: 2em;&quot;&gt;&lt;span style=&quot;color: rgb(128, 128, 128); font-variant-east-asian: normal; line-height: 22px; widows: 1;&quot;&gt;&lt;span style=&quot;text-indent: 32px;&quot;&gt;&amp;nbsp; ④&amp;nbsp;&lt;/span&gt;小额贷款是银监会发证；&lt;/span&gt;&lt;/p&gt;&lt;p style=&quot;margin-top: 0px; margin-bottom: 0px; padding: 0px; word-break: break-all; color: rgb(102, 102, 102); font-size: 14px; letter-spacing: 1px; white-space: normal; background-color: rgb(255, 255, 255); text-indent: 2em;&quot;&gt;&lt;span style=&quot;color: rgb(128, 128, 128); font-variant-east-asian: normal; line-height: 22px; widows: 1;&quot;&gt;&lt;span style=&quot;text-indent: 32px;&quot;&gt;&amp;nbsp; ⑤&amp;nbsp;&lt;/span&gt;第三方支付是人民银行发证；&lt;/span&gt;&lt;/p&gt;&lt;p style=&quot;margin-top: 0px; margin-bottom: 0px; padding: 0px; word-break: break-all; color: rgb(102, 102, 102); font-size: 14px; letter-spacing: 1px; white-space: normal; background-color: rgb(255, 255, 255); text-indent: 2em;&quot;&gt;&lt;span style=&quot;color: rgb(128, 128, 128); font-variant-east-asian: normal; line-height: 22px; widows: 1;&quot;&gt;&lt;span style=&quot;text-indent: 32px;&quot;&gt;&amp;nbsp; ⑥&amp;nbsp;&lt;/span&gt;保险是保监会发证；&lt;/span&gt;&lt;/p&gt;&lt;p style=&quot;margin-top: 0px; margin-bottom: 0px; padding: 0px; word-break: break-all; color: rgb(102, 102, 102); font-size: 14px; letter-spacing: 1px; white-space: normal; background-color: rgb(255, 255, 255); text-indent: 2em;&quot;&gt;&lt;span style=&quot;color: rgb(128, 128, 128); font-variant-east-asian: normal; line-height: 22px; widows: 1;&quot;&gt;&lt;span style=&quot;text-indent: 32px;&quot;&gt;&amp;nbsp; ⑦&amp;nbsp;&lt;/span&gt;金融机构发证比如银行什么的都是银监会；&lt;/span&gt;&lt;/p&gt;&lt;p style=&quot;margin-top: 0px; margin-bottom: 0px; padding: 0px; word-break: break-all; color: rgb(102, 102, 102); font-size: 14px; letter-spacing: 1px; white-space: normal; background-color: rgb(255, 255, 255); text-indent: 2em;&quot;&gt;&lt;span style=&quot;color: rgb(128, 128, 128); font-variant-east-asian: normal; line-height: 22px; widows: 1;&quot;&gt;&lt;span style=&quot;text-indent: 32px;&quot;&gt;&amp;nbsp; ⑧&amp;nbsp;&lt;/span&gt;证券公司发证都是证监会；&lt;/span&gt;&lt;/p&gt;&lt;p style=&quot;margin-top: 0px; margin-bottom: 0px; padding: 0px; word-break: break-all; color: rgb(102, 102, 102); font-size: 14px; letter-spacing: 1px; white-space: normal; background-color: rgb(255, 255, 255); text-indent: 2em;&quot;&gt;&lt;span style=&quot;color: rgb(128, 128, 128); font-variant-east-asian: normal; line-height: 22px; widows: 1;&quot;&gt;&lt;span style=&quot;text-indent: 32px;&quot;&gt;&amp;nbsp; ⑨&amp;nbsp;&lt;/span&gt;信托公司是银监会；&lt;/span&gt;&lt;/p&gt;&lt;p&gt;&lt;br/&gt;&lt;/p&gt;'),(5,'&lt;p&gt;&amp;nbsp; &amp;nbsp; &amp;nbsp; &amp;nbsp;虽然互联网上付费提供网站建设和网站制作服务的公司或者个人有很多，都是为企业或者个人提供网站建设和网页设计服务的，但是对于那些刚刚走出校门或者刚刚参加工作的朋友来说，如果想通过互联网创业，想要做一个自己的网站，但是又没有明确的经营理念，只是想要尝试一下互联网创业，这时候大部分人都会选择自己建网站，一方面是为了能够节省较高的网站建设费用，另一方面也可以简单的学习一些网站建设或网站制作的一些基本知识，那么自己建网站到底应该如何入手呢今天小编就跟大家写一篇自己建网站的全攻略，希望能够帮助那些想要自己建网站的朋友有一个系统的认识和了解。&lt;/p&gt;&lt;p&gt;&lt;br/&gt;&lt;/p&gt;&lt;p&gt;&lt;strong&gt;1、了解基础的脚本语言&lt;/strong&gt;&lt;/p&gt;&lt;p&gt;  无论你是打算用网络上分享出来的免费源代码做网站还是用自助建站系统来建网站，首先应该学习和了解的就是网站前台脚本语言，网站前台脚本语言主要是html/js/css这三种，其中html是客户端网页源代码的主要语言，js脚本语言用来实现各种网页特效，css脚本语言用来实现网站的各种布局及网页色调的调整。&lt;/p&gt;&lt;p&gt;  相对于php、java等编程语言来说，脚本语言更加容易记忆和学习，所以一般想要接触网站建设的朋友都应该首先学习和认识上边说道的三种脚本语言。&lt;/p&gt;&lt;p&gt;&lt;br/&gt;&lt;/p&gt;&lt;p&gt;&lt;strong&gt;2、免费的源码程序&lt;/strong&gt;&lt;/p&gt;&lt;p&gt;  对于刚学网站建设的朋友来说，自己建网站肯定不可能上来就自己写一套强大的网站CMS系统，这几乎是不可能的，而且也是不现实的，毕竟一套功能强大的网站管理系统往往都是很多人开发测试很久才能完成的，依靠一个人的力量快速的完全一套建站系统显然难度很大，所以这就需要借助网络上已经分享出来的免费网站源码程序来快速完成自己建网站的目的。&lt;/p&gt;&lt;p&gt;  自建网站虽然除了使用免费的源码程序还可以通过选择一些免费的自助建站平台来完成，但是小编这里推荐大家使用免费的源码程序来做网站，这样一方面是保证网站最终的控制权在自己手里，另一方面也有助于更好的提升自己的网站建设的认识和熟悉，如果你使用自助建站平台，永远都不可能明白网站开发的基本框架设计，但是你通过研究别人分享出来的免费源码就可以很好的掌握整个程序的框架结构和页面设计方面的一些知识，能够更好的提升自己的专业技能。&lt;/p&gt;&lt;p&gt;&lt;br/&gt;&lt;/p&gt;&lt;p&gt;&lt;strong&gt;3、服务器及域名&lt;/strong&gt;&lt;/p&gt;&lt;p&gt;  通过学习第一步骤的那些前台脚本语言，然后按照第二步骤说的去下载和研究别人的源码程序，相信很快你就可以自己建网站了，但是建立好的网站如果只是在本地运行，那只有你自己可以访问和看到，如何才能让网络上的所有人都看到自己做的网站呢，这就涉及到了网站的发布，网站发布就需要使用服务器和域名，这时候就需要我们去接触服务器和域名。&lt;/p&gt;&lt;p&gt;  虽然说大部分的新手自己建网站都不希望花费太高的成本，但是服务器和域名的成本是每一个做网站的人都要承担的，而且一个稳定的服务器直接影响到你网站将来的打开速度、网站性能及搜索引擎收录情况，所以建议新手们在购买服务器的时候还是要选择性价比比较高的服务器。&lt;/p&gt;&lt;p&gt;  哦，最后还得补充一下，要想自己建网站，除了上边说道的这些都必须要学习和了解之外，还有两个重要的软件需要下载安装和学习怎么使用，一个是Dreamweaver(简称DW)另一个就是Photoshop(简称PS)这两款软件即使你不打算自己开发设计，你就是研究和修改别人的源码程序也一样需要用到，比如你在步骤二中需要修改别人的网页代码，肯定需要用DW打开网页文件来编辑，需要修改别人程序的中图标或者图片就肯定需要使用PS来作图，所以这两款软件也是自己建网站过程中必须要学习使用的。&lt;/p&gt;');
/*!40000 ALTER TABLE `yzn_article_data` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `yzn_attachment`
--

DROP TABLE IF EXISTS `yzn_attachment`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `yzn_attachment` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `aid` smallint(3) unsigned NOT NULL DEFAULT '0' COMMENT '管理员id',
  `uid` mediumint(8) unsigned NOT NULL DEFAULT '0' COMMENT '用户id',
  `name` char(50) NOT NULL DEFAULT '' COMMENT '文件名',
  `module` char(15) NOT NULL DEFAULT '' COMMENT '模块名，由哪个模块上传的',
  `path` varchar(255) NOT NULL DEFAULT '' COMMENT '文件路径',
  `thumb` varchar(255) NOT NULL DEFAULT '' COMMENT '缩略图路径',
  `url` varchar(255) NOT NULL DEFAULT '' COMMENT '文件链接',
  `mime` varchar(100) NOT NULL DEFAULT '' COMMENT '文件mime类型',
  `ext` char(4) NOT NULL DEFAULT '' COMMENT '文件类型',
  `size` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '文件大小',
  `md5` char(32) NOT NULL DEFAULT '' COMMENT '文件md5',
  `sha1` char(40) NOT NULL DEFAULT '' COMMENT 'sha1 散列值',
  `driver` varchar(16) NOT NULL DEFAULT 'local' COMMENT '上传驱动',
  `create_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '上传时间',
  `update_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '更新时间',
  `listorders` int(5) NOT NULL DEFAULT '100' COMMENT '排序',
  `status` tinyint(2) NOT NULL DEFAULT '0' COMMENT '状态',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='附件表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `yzn_attachment`
--

LOCK TABLES `yzn_attachment` WRITE;
/*!40000 ALTER TABLE `yzn_attachment` DISABLE KEYS */;
/*!40000 ALTER TABLE `yzn_attachment` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `yzn_auth_group`
--

DROP TABLE IF EXISTS `yzn_auth_group`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `yzn_auth_group` (
  `id` mediumint(8) unsigned NOT NULL AUTO_INCREMENT COMMENT '用户组id,自增主键',
  `parentid` mediumint(8) unsigned NOT NULL DEFAULT '0' COMMENT '父组别',
  `module` varchar(20) NOT NULL COMMENT '用户组所属模块',
  `type` tinyint(4) NOT NULL COMMENT '组类型',
  `title` char(20) NOT NULL DEFAULT '' COMMENT '用户组中文名称',
  `description` varchar(80) NOT NULL DEFAULT '' COMMENT '描述信息',
  `rules` varchar(500) NOT NULL DEFAULT '' COMMENT '用户组拥有的规则id，多个规则 , 隔开',
  `status` tinyint(2) NOT NULL DEFAULT '0' COMMENT '状态',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=3 DEFAULT CHARSET=utf8 COMMENT='权限组表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `yzn_auth_group`
--

LOCK TABLES `yzn_auth_group` WRITE;
/*!40000 ALTER TABLE `yzn_auth_group` DISABLE KEYS */;
INSERT INTO `yzn_auth_group` VALUES (1,0,'admin',1,'超级管理员','拥有所有权限','',1),(2,1,'admin',1,'编辑','编辑','',1);
/*!40000 ALTER TABLE `yzn_auth_group` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `yzn_auth_rule`
--

DROP TABLE IF EXISTS `yzn_auth_rule`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `yzn_auth_rule` (
  `id` mediumint(8) unsigned NOT NULL AUTO_INCREMENT COMMENT '规则id,自增主键',
  `module` varchar(20) NOT NULL COMMENT '规则所属module',
  `type` tinyint(2) NOT NULL DEFAULT '1' COMMENT '1-url;2-主菜单',
  `name` char(80) NOT NULL DEFAULT '' COMMENT '规则唯一英文标识',
  `title` char(20) NOT NULL DEFAULT '' COMMENT '规则中文描述',
  `condition` varchar(300) NOT NULL DEFAULT '' COMMENT '规则附加条件',
  `status` tinyint(2) NOT NULL DEFAULT '0' COMMENT '状态',
  PRIMARY KEY (`id`),
  KEY `module` (`module`,`status`,`type`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='规则表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `yzn_auth_rule`
--

LOCK TABLES `yzn_auth_rule` WRITE;
/*!40000 ALTER TABLE `yzn_auth_rule` DISABLE KEYS */;
/*!40000 ALTER TABLE `yzn_auth_rule` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `yzn_cache`
--

DROP TABLE IF EXISTS `yzn_cache`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `yzn_cache` (
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `key` char(100) NOT NULL DEFAULT '' COMMENT '缓存KEY值',
  `name` char(100) NOT NULL DEFAULT '' COMMENT '名称',
  `module` char(20) NOT NULL DEFAULT '' COMMENT '模块名称',
  `model` char(30) NOT NULL DEFAULT '' COMMENT '模型名称',
  `action` char(30) NOT NULL DEFAULT '' COMMENT '方法名',
  `system` tinyint(1) NOT NULL DEFAULT '0' COMMENT '是否系统',
  PRIMARY KEY (`id`),
  KEY `ckey` (`key`)
) ENGINE=MyISAM AUTO_INCREMENT=48 DEFAULT CHARSET=utf8 COMMENT='缓存列队表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `yzn_cache`
--

LOCK TABLES `yzn_cache` WRITE;
/*!40000 ALTER TABLE `yzn_cache` DISABLE KEYS */;
INSERT INTO `yzn_cache` VALUES (1,'Config','网站配置','admin','Config','config_cache',1),(2,'Menu','后台菜单','admin','Menu','menu_cache',1),(3,'Module','可用模块列表','admin','Module','module_cache',1),(4,'Model','模型列表','admin','Models','model_cache',1),(5,'ModelField','模型字段','admin','ModelField','model_field_cache',1),(19,'Member_Config','游戏服务器管理配置','gameserver','gameserver','gameserver_cache',0),(18,'Member_Config','游戏服务器管理配置','gameserver','gameserver','gameserver_cache',0),(8,'Model_form','自定义表单模型','formguide','Formguide','formguide_cache',0),(47,'Member_Group','会员组','member','MemberGroup','membergroup_cache',0),(46,'Member_Config','会员配置','member','Member','member_cache',0),(11,'Pay_Config','支付配置','pay','Payment','pay_cache',0),(20,'Member_Config','游戏管理配置','games','games','games_cache',0),(21,'Member_Config','游戏服务器管理配置','gameserver','gameserver','gameserver_cache',0),(38,'Member_Config','游戏服务器管理配置','gameserver','gameserver','gameserver_cache',0),(39,'Member_Config','游戏服务器管理配置','gameserver','gameserver','gameserver_cache',0);
/*!40000 ALTER TABLE `yzn_cache` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `yzn_category`
--

DROP TABLE IF EXISTS `yzn_category`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `yzn_category` (
  `id` smallint(5) unsigned NOT NULL AUTO_INCREMENT COMMENT '栏目ID',
  `catname` varchar(30) NOT NULL DEFAULT '' COMMENT '栏目名称',
  `catdir` varchar(30) NOT NULL DEFAULT '' COMMENT '唯一标识',
  `type` tinyint(1) unsigned NOT NULL DEFAULT '0' COMMENT '类别',
  `modelid` smallint(5) unsigned NOT NULL DEFAULT '0' COMMENT '模型ID',
  `parentid` smallint(5) unsigned NOT NULL DEFAULT '0' COMMENT '父ID',
  `arrparentid` varchar(255) NOT NULL DEFAULT '' COMMENT '所有父ID',
  `arrchildid` varchar(255) NOT NULL DEFAULT '' COMMENT '所有子栏目ID',
  `child` tinyint(1) unsigned NOT NULL DEFAULT '0' COMMENT '是否存在子栏目，1存在',
  `image` mediumint(8) unsigned NOT NULL DEFAULT '0' COMMENT '栏目图片',
  `description` mediumtext NOT NULL COMMENT '栏目描述',
  `url` varchar(100) NOT NULL DEFAULT '' COMMENT '链接地址',
  `setting` text COMMENT '相关配置信息',
  `listorder` smallint(5) unsigned NOT NULL DEFAULT '0' COMMENT '排序',
  `status` tinyint(2) NOT NULL DEFAULT '0' COMMENT '状态',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=19 DEFAULT CHARSET=utf8 COMMENT='栏目表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `yzn_category`
--

LOCK TABLES `yzn_category` WRITE;
/*!40000 ALTER TABLE `yzn_category` DISABLE KEYS */;
INSERT INTO `yzn_category` VALUES (2,'公司简介','Introduction',1,0,1,'0,1','2',0,0,'','','a:4:{s:10:\"meta_title\";s:0:\"\";s:13:\"meta_keywords\";s:0:\"\";s:16:\"meta_description\";s:0:\"\";s:13:\"page_template\";s:9:\"page.html\";}',1,1),(3,'企业文化','culture',1,0,1,'0,1','3',0,0,'','','a:4:{s:10:\"meta_title\";s:0:\"\";s:13:\"meta_keywords\";s:0:\"\";s:16:\"meta_description\";s:0:\"\";s:13:\"page_template\";s:9:\"page.html\";}',2,1),(4,'公司荣誉','honor',2,2,1,'0,1','4',0,0,'','','a:7:{s:10:\"meta_title\";s:0:\"\";s:13:\"meta_keywords\";s:0:\"\";s:16:\"meta_description\";s:0:\"\";s:17:\"category_template\";s:21:\"category_picture.html\";s:13:\"list_template\";s:17:\"list_picture.html\";s:13:\"show_template\";s:17:\"show_picture.html\";s:13:\"page_template\";s:9:\"page.html\";}',3,1),(5,'案例展示','case',2,3,0,'0','5',0,0,'','','a:7:{s:10:\"meta_title\";s:0:\"\";s:13:\"meta_keywords\";s:0:\"\";s:16:\"meta_description\";s:0:\"\";s:17:\"category_template\";s:18:\"category_case.html\";s:13:\"list_template\";s:14:\"list_case.html\";s:13:\"show_template\";s:17:\"show_picture.html\";s:13:\"page_template\";s:9:\"page.html\";}',2,1),(6,'新闻中心','news',2,1,0,'0','6,9,10,14',1,0,'','','a:7:{s:10:\"meta_title\";s:0:\"\";s:13:\"meta_keywords\";s:0:\"\";s:16:\"meta_description\";s:0:\"\";s:17:\"category_template\";s:13:\"category.html\";s:13:\"list_template\";s:9:\"list.html\";s:13:\"show_template\";s:9:\"show.html\";s:13:\"page_template\";s:9:\"page.html\";}',3,1),(8,'联系我们','contact',1,0,0,'0','8,18,19',1,0,'','cms/index/lists?catid=18','N;',4,1),(9,'网络营销','marketing',2,1,6,'0,6','9',0,0,'','','a:7:{s:10:\"meta_title\";s:0:\"\";s:13:\"meta_keywords\";s:0:\"\";s:16:\"meta_description\";s:0:\"\";s:17:\"category_template\";s:13:\"category.html\";s:13:\"list_template\";s:9:\"list.html\";s:13:\"show_template\";s:9:\"show.html\";s:13:\"page_template\";s:9:\"page.html\";}',1,1),(10,'网站知识','knowledge',2,1,6,'0,6','10',0,0,'','','a:7:{s:10:\"meta_title\";s:0:\"\";s:13:\"meta_keywords\";s:0:\"\";s:16:\"meta_description\";s:0:\"\";s:17:\"category_template\";s:13:\"category.html\";s:13:\"list_template\";s:9:\"list.html\";s:13:\"show_template\";s:9:\"show.html\";s:13:\"page_template\";s:9:\"page.html\";}',2,1),(14,'备案知识','record',2,1,6,'0,6','14',0,0,'','','a:7:{s:10:\"meta_title\";s:0:\"\";s:13:\"meta_keywords\";s:0:\"\";s:16:\"meta_description\";s:0:\"\";s:17:\"category_template\";s:13:\"category.html\";s:13:\"list_template\";s:9:\"list.html\";s:13:\"show_template\";s:9:\"show.html\";s:13:\"page_template\";s:9:\"page.html\";}',3,1),(1,'关于我们','about',1,0,0,'0','1,2,3,4',1,0,'','cms/index/lists?catid=2','N;',1,1),(18,'联系方式','fangshi',1,0,8,'0,8','18',0,0,'','','a:4:{s:10:\"meta_title\";s:0:\"\";s:13:\"meta_keywords\";s:0:\"\";s:16:\"meta_description\";s:0:\"\";s:13:\"page_template\";s:9:\"page.html\";}',100,1);
/*!40000 ALTER TABLE `yzn_category` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `yzn_category_priv`
--

DROP TABLE IF EXISTS `yzn_category_priv`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `yzn_category_priv` (
  `catid` smallint(5) unsigned NOT NULL DEFAULT '0',
  `roleid` smallint(5) unsigned NOT NULL DEFAULT '0' COMMENT '角色或者组ID',
  `is_admin` tinyint(1) unsigned NOT NULL DEFAULT '0' COMMENT '是否为管理员 1、管理员',
  `action` char(30) NOT NULL DEFAULT '' COMMENT '动作',
  KEY `catid` (`catid`,`roleid`,`is_admin`,`action`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='栏目权限表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `yzn_category_priv`
--

LOCK TABLES `yzn_category_priv` WRITE;
/*!40000 ALTER TABLE `yzn_category_priv` DISABLE KEYS */;
/*!40000 ALTER TABLE `yzn_category_priv` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `yzn_config`
--

DROP TABLE IF EXISTS `yzn_config`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `yzn_config` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT '配置ID',
  `name` varchar(30) NOT NULL DEFAULT '' COMMENT '配置名称',
  `type` varchar(32) NOT NULL DEFAULT '' COMMENT '配置类型',
  `title` varchar(50) NOT NULL DEFAULT '' COMMENT '配置说明',
  `group` varchar(32) NOT NULL DEFAULT '' COMMENT '配置分组',
  `options` varchar(255) NOT NULL DEFAULT '' COMMENT '配置项',
  `remark` varchar(100) NOT NULL DEFAULT '' COMMENT '配置说明',
  `create_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '创建时间',
  `update_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '更新时间',
  `status` tinyint(2) NOT NULL DEFAULT '0' COMMENT '状态',
  `value` text COMMENT '配置值',
  `listorder` smallint(3) unsigned NOT NULL DEFAULT '0' COMMENT '排序',
  PRIMARY KEY (`id`),
  UNIQUE KEY `uk_name` (`name`),
  KEY `type` (`type`),
  KEY `group` (`group`)
) ENGINE=MyISAM AUTO_INCREMENT=23 DEFAULT CHARSET=utf8 COMMENT='网站配置';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `yzn_config`
--

LOCK TABLES `yzn_config` WRITE;
/*!40000 ALTER TABLE `yzn_config` DISABLE KEYS */;
INSERT INTO `yzn_config` VALUES (1,'web_site_icp','text','备案信息','base','','',1551244923,1551244971,1,'',1),(2,'web_site_statistics','textarea','站点代码','base','','',1551244957,1551244957,1,'',100),(3,'mail_type','radio','邮件发送模式','email','1:SMTP\r\n2:Mail','',1553652833,1553652915,1,'1',1),(4,'mail_smtp_host','text','邮件服务器','email','','错误的配置发送邮件会导致服务器超时',1553652889,1553652917,1,'smtp.163.com',2),(5,'mail_smtp_port','text','邮件发送端口','email','','不加密默认25,SSL默认465,TLS默认587',1553653165,1553653292,1,'465',3),(6,'mail_auth','radio','身份认证','email','0:关闭\r\n1:开启','',1553658375,1553658392,1,'1',4),(7,'mail_smtp_user','text','用户名','email','','',1553653267,1553658393,1,'',5),(8,'mail_smtp_pass','text','密码','email','','',1553653344,1553658394,1,'',6),(9,'mail_verify_type','radio','验证方式','email','1:TLS\r\n2:SSL','',1553653426,1553658395,1,'2',7),(10,'mail_from','text','发件人邮箱','email','','',1553653500,1553658397,1,'',8),(11,'config_group','array','配置分组','system','','',1494408414,1494408414,1,'base:基础\r\nemail:邮箱\r\nsystem:系统\r\nupload:上传\r\ndevelop:开发',0),(12,'theme','text','主题风格','system','','',1541752781,1541756888,1,'default',1),(13,'admin_allow_ip','textarea','后台允许访问IP','system','','匹配IP段用\"*\"占位，如192.168.*.*，多个IP地址请用英文逗号\",\"分割',1551244957,1551244957,1,'',2),(14,'upload_image_size','text','图片上传大小限制','upload','','0为不限制大小，单位：kb',1540457656,1552436075,1,'0',2),(15,'upload_image_ext','text','允许上传的图片后缀','upload','','多个后缀用逗号隔开，不填写则不限制类型',1540457657,1552436074,1,'gif,jpg,jpeg,bmp,png',1),(16,'upload_file_size','text','文件上传大小限制','upload','','0为不限制大小，单位：kb',1540457658,1552436078,1,'0',3),(17,'upload_file_ext','text','允许上传的文件后缀','upload','','多个后缀用逗号隔开，不填写则不限制类型',1540457659,1552436080,1,'doc,docx,xls,xlsx,ppt,pptx,pdf,wps,txt,rar,zip,gz,bz2,7z',4),(18,'upload_driver','radio','上传驱动','upload','local:本地','图片或文件上传驱动',1541752781,1552436085,1,'local',9),(19,'upload_thumb_water','switch','添加水印','upload','','',1552435063,1552436080,1,'0',5),(20,'upload_thumb_water_pic','image','水印图片','upload','','只有开启水印功能才生效',1552435183,1552436081,1,'',6),(21,'upload_thumb_water_position','radio','水印位置','upload','1:左上角\r\n2:上居中\r\n3:右上角\r\n4:左居中\r\n5:居中\r\n6:右居中\r\n7:左下角\r\n8:下居中\r\n9:右下角','只有开启水印功能才生效',1552435257,1552436082,1,'9',7),(22,'upload_thumb_water_alpha','text','水印透明度','upload','','请输入0~100之间的数字，数字越小，透明度越高',1552435299,1552436083,1,'50',8);
/*!40000 ALTER TABLE `yzn_config` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `yzn_download`
--

DROP TABLE IF EXISTS `yzn_download`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `yzn_download` (
  `id` mediumint(8) unsigned NOT NULL AUTO_INCREMENT COMMENT '文档ID',
  `catid` smallint(5) unsigned NOT NULL DEFAULT '0' COMMENT '栏目ID',
  `title` varchar(255) COLLATE utf8_unicode_ci NOT NULL DEFAULT '' COMMENT '标题',
  `thumb` int(5) unsigned NOT NULL DEFAULT '0' COMMENT '缩略图',
  `flag` varchar(32) COLLATE utf8_unicode_ci NOT NULL DEFAULT '' COMMENT '属性',
  `keywords` varchar(255) COLLATE utf8_unicode_ci NOT NULL DEFAULT '' COMMENT 'SEO关键词',
  `description` varchar(255) COLLATE utf8_unicode_ci NOT NULL DEFAULT '' COMMENT 'SEO描述',
  `tags` varchar(255) COLLATE utf8_unicode_ci NOT NULL DEFAULT '' COMMENT 'Tags标签',
  `listorder` smallint(5) unsigned NOT NULL DEFAULT '100' COMMENT '排序',
  `uid` mediumint(8) unsigned NOT NULL DEFAULT '0' COMMENT '用户id',
  `username` varchar(20) COLLATE utf8_unicode_ci NOT NULL COMMENT '用户名',
  `sysadd` tinyint(1) unsigned NOT NULL DEFAULT '0' COMMENT '是否后台添加',
  `hits` mediumint(8) unsigned DEFAULT '0' COMMENT '点击量',
  `inputtime` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '创建时间',
  `updatetime` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '更新时间',
  `status` tinyint(2) NOT NULL DEFAULT '0' COMMENT '状态',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='下载模型模型表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `yzn_download`
--

LOCK TABLES `yzn_download` WRITE;
/*!40000 ALTER TABLE `yzn_download` DISABLE KEYS */;
/*!40000 ALTER TABLE `yzn_download` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `yzn_download_data`
--

DROP TABLE IF EXISTS `yzn_download_data`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `yzn_download_data` (
  `did` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `content` text COLLATE utf8_unicode_ci COMMENT '内容',
  PRIMARY KEY (`did`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='下载模型模型表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `yzn_download_data`
--

LOCK TABLES `yzn_download_data` WRITE;
/*!40000 ALTER TABLE `yzn_download_data` DISABLE KEYS */;
/*!40000 ALTER TABLE `yzn_download_data` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `yzn_ems`
--

DROP TABLE IF EXISTS `yzn_ems`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `yzn_ems` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT 'ID',
  `event` varchar(30) NOT NULL DEFAULT '' COMMENT '事件',
  `email` varchar(100) NOT NULL DEFAULT '' COMMENT '邮箱',
  `code` varchar(10) NOT NULL DEFAULT '' COMMENT '验证码',
  `times` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '验证次数',
  `ip` bigint(20) NOT NULL DEFAULT '0' COMMENT 'IP',
  `create_time` int(10) unsigned DEFAULT '0' COMMENT '创建时间',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='邮箱验证码表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `yzn_ems`
--

LOCK TABLES `yzn_ems` WRITE;
/*!40000 ALTER TABLE `yzn_ems` DISABLE KEYS */;
/*!40000 ALTER TABLE `yzn_ems` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `yzn_field_type`
--

DROP TABLE IF EXISTS `yzn_field_type`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `yzn_field_type` (
  `name` varchar(32) NOT NULL COMMENT '字段类型',
  `title` varchar(64) NOT NULL DEFAULT '' COMMENT '中文类型名',
  `listorder` int(4) NOT NULL DEFAULT '0' COMMENT '排序',
  `default_define` varchar(128) NOT NULL DEFAULT '' COMMENT '默认定义',
  `ifoption` tinyint(1) unsigned NOT NULL DEFAULT '0' COMMENT '是否需要设置选项',
  `ifstring` tinyint(1) unsigned NOT NULL DEFAULT '0' COMMENT '是否自由字符',
  `vrule` varchar(256) NOT NULL DEFAULT '' COMMENT '验证规则',
  PRIMARY KEY (`name`),
  UNIQUE KEY `name` (`name`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='字段类型表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `yzn_field_type`
--

LOCK TABLES `yzn_field_type` WRITE;
/*!40000 ALTER TABLE `yzn_field_type` DISABLE KEYS */;
INSERT INTO `yzn_field_type` VALUES ('text','输入框',1,'varchar(255) NOT NULL DEFAULT \'\'',0,1,''),('checkbox','复选框',2,'varchar(32) NOT NULL DEFAULT \'\'',1,0,''),('textarea','多行文本',3,'varchar(255) NOT NULL DEFAULT \'\'',0,1,''),('radio','单选按钮',4,'char(10) NOT NULL DEFAULT \'\'',1,0,''),('switch','开关',5,'tinyint(2) UNSIGNED NOT NULL DEFAULT \'0\'',0,0,'isBool'),('array','数组',6,'varchar(512) NOT NULL DEFAULT \'\'',0,0,''),('select','下拉框',7,'char(10) NOT NULL DEFAULT \'\'',1,0,''),('image','单张图',8,'int(5) UNSIGNED NOT NULL DEFAULT \'0\'',0,0,'isNumber'),('tags','标签',10,'varchar(255) NOT NULL DEFAULT \'\'',0,1,''),('number','数字',11,'int(10) UNSIGNED NOT NULL DEFAULT \'0\'',0,0,'isNumber'),('datetime','日期和时间',12,'int(10) UNSIGNED NOT NULL DEFAULT \'0\'',0,0,''),('Ueditor','百度编辑器',13,'text NOT NULL',0,1,''),('images','多张图',9,'varchar(256) NOT NULL DEFAULT \'\'',0,0,''),('color','颜色值',17,'varchar(7) NOT NULL DEFAULT \'\'',0,0,''),('files','多文件',15,'varchar(255) NOT NULL DEFAULT \'\'',0,0,''),('summernote','简洁编辑器',14,'text NOT NULL',0,1,''),('file','单文件',16,'int(5) UNSIGNED NOT NULL DEFAULT \'0\'',0,0,'isNumber');
/*!40000 ALTER TABLE `yzn_field_type` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `yzn_hooks`
--

DROP TABLE IF EXISTS `yzn_hooks`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `yzn_hooks` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT '主键',
  `name` varchar(40) NOT NULL DEFAULT '' COMMENT '钩子名称',
  `description` text NOT NULL COMMENT '描述',
  `type` tinyint(1) unsigned NOT NULL DEFAULT '1' COMMENT '类型',
  `update_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '更新时间',
  `addons` varchar(255) NOT NULL DEFAULT '' COMMENT '钩子挂载的插件 ''，''分割',
  `modules` varchar(255) NOT NULL DEFAULT '' COMMENT '钩子挂载的模块 ''，''分割',
  `system` tinyint(1) unsigned NOT NULL DEFAULT '0' COMMENT '是否系统',
  `status` tinyint(2) NOT NULL DEFAULT '0' COMMENT '状态',
  PRIMARY KEY (`id`),
  UNIQUE KEY `name` (`name`)
) ENGINE=MyISAM AUTO_INCREMENT=18 DEFAULT CHARSET=utf8 COMMENT='插件和模块钩子';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `yzn_hooks`
--

LOCK TABLES `yzn_hooks` WRITE;
/*!40000 ALTER TABLE `yzn_hooks` DISABLE KEYS */;
INSERT INTO `yzn_hooks` VALUES (1,'pageHeader','页面header钩子，一般用于加载插件CSS文件和代码',1,1509174020,'','',1,1),(2,'pageFooter','页面footer钩子，一般用于加载插件JS文件和JS代码',1,1509174020,'','',1,1),(3,'smsGet','短信获取行为',2,1509174020,'','',1,1),(4,'smsSend','短信发送行为',2,1509174020,'','',1,1),(5,'smsNotice','短信发送通知',2,1509174020,'','',1,1),(6,'smsCheck','检测短信验证是否正确',2,1509174020,'','',1,1),(7,'smsFlush','清空短信验证行为',2,1509174020,'','',1,1),(8,'emsGet','邮件获取行为',2,1509174020,'','',1,1),(9,'emsSend','邮件发送行为',2,1509174020,'','',1,1),(10,'emsNotice','邮件发送通知',2,1509174020,'','',1,1),(11,'emsCheck','检测邮件验证是否正确',2,1509174020,'','',1,1),(12,'emsFlush','清空邮件验证行为',2,1509174020,'','',1,1),(13,'uploadAfter','第三方附件上传钩子',2,1509174020,'','',1,1),(14,'uploadDelete','第三方附件删除钩子',2,1509174020,'','',1,1),(15,'userSidenavAfter','会员左侧导航',1,1569297051,'','message,pay',0,1),(16,'contentEditEnd','内容编辑后调用',2,1569297064,'','gameserver,games,member',0,1),(17,'contentDeleteEnd','内容删除后调用',2,1570851474,'','gameserver,games,member',0,1);
/*!40000 ALTER TABLE `yzn_hooks` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `yzn_menu`
--

DROP TABLE IF EXISTS `yzn_menu`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `yzn_menu` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT '菜单ID',
  `title` varchar(50) NOT NULL DEFAULT '' COMMENT '标题',
  `icon` varchar(64) NOT NULL DEFAULT '' COMMENT '图标',
  `parentid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '上级分类ID',
  `app` char(20) NOT NULL DEFAULT '' COMMENT '应用标识',
  `controller` char(20) NOT NULL DEFAULT '' COMMENT '控制器标识',
  `action` char(20) NOT NULL DEFAULT '' COMMENT '方法标识',
  `parameter` char(255) NOT NULL DEFAULT '' COMMENT '附加参数',
  `status` tinyint(2) NOT NULL DEFAULT '0' COMMENT '状态',
  `tip` varchar(255) NOT NULL DEFAULT '' COMMENT '提示',
  `is_dev` tinyint(1) unsigned NOT NULL DEFAULT '0' COMMENT '是否开发者可见',
  `listorder` smallint(6) unsigned NOT NULL DEFAULT '0' COMMENT '排序ID',
  PRIMARY KEY (`id`),
  KEY `pid` (`parentid`)
) ENGINE=MyISAM AUTO_INCREMENT=244 DEFAULT CHARSET=utf8 COMMENT='后台菜单表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `yzn_menu`
--

LOCK TABLES `yzn_menu` WRITE;
/*!40000 ALTER TABLE `yzn_menu` DISABLE KEYS */;
INSERT INTO `yzn_menu` VALUES (1,'首页','',0,'admin','index','index','',0,'',0,0),(2,'控制面板','',0,'admin','main','index','',0,'',0,1),(3,'设置','icon-setup',0,'admin','setting','index','',1,'',0,0),(4,'模块','icon-supply',0,'admin','module','index1','',1,'',0,9),(5,'扩展','icon-tools',0,'addons','addons','index1','',1,'',0,10),(10,'系统配置','icon-zidongxiufu',3,'admin','config','index1','',1,'',0,0),(11,'配置管理','icon-apartment',10,'admin','config','index','',1,'',0,0),(12,'删除日志','',20,'admin','adminlog','deletelog','',1,'',0,0),(13,'网站设置','icon-setup',10,'admin','config','setting','',1,'',0,0),(14,'菜单管理','icon-other',10,'admin','menu','index','',1,'',0,0),(15,'权限管理','icon-guanliyuan',3,'admin','manager','index1','',1,'',0,0),(16,'管理员管理','icon-guanliyuan',15,'admin','manager','index','',1,'',0,0),(17,'角色管理','icon-group',15,'admin','authManager','index','',1,'',0,0),(18,'添加管理员','',16,'admin','manager','add','',1,'',0,0),(19,'编辑管理员','',16,'admin','manager','edit','',1,'',0,0),(20,'管理日志','icon-rizhi',15,'admin','adminlog','index','',1,'',0,0),(21,'删除管理员','',16,'admin','manager','del','',1,'',0,0),(22,'添加角色','',17,'admin','authManager','createGroup','',1,'',0,0),(23,'附件管理','icon-accessory',10,'attachment','attachments','index','',1,'',0,1),(24,'新增配置','',11,'admin','config','add','',1,'',0,1),(25,'编辑配置','',11,'admin','config','edit','',1,'',0,2),(26,'删除配置','',11,'admin','config','del','',1,'',0,3),(27,'新增菜单','',14,'admin','menu','add','',1,'',0,0),(28,'编辑菜单','',14,'admin','menu','edit','',1,'',0,0),(29,'删除菜单','',14,'admin','menu','delete','',1,'',0,0),(30,'附件上传','',23,'attachment','attachments','upload','',1,'',0,0),(31,'附件删除','',23,'attachment','attachments','delete','',1,'',0,0),(32,'编辑器附件','',23,'attachment','ueditor','run','',0,'',0,0),(33,'图片列表','',23,'attachment','attachments','showFileLis','',0,'',0,0),(34,'图片本地化','',23,'attachment','attachments','getUrlFile','',0,'',0,0),(38,'插件扩展','icon-tools',5,'addons','addons','index2','',1,'',0,0),(39,'插件管理','icon-plugins-',38,'addons','addons','index','',1,'',0,0),(40,'行为管理','icon-hangweifenxi',38,'addons','addons','hooks','',1,'',0,0),(41,'插件后台列表','icon-liebiaosousuo',5,'addons','addons','addonadmin','',0,'',0,0),(42,'在线云平台','icon-cloud-download',4,'admin','cloud','index','',1,'',0,0),(43,'本地模块','icon-supply',4,'admin','module','index2','',1,'',0,0),(44,'模块管理','icon-mokuaishezhi',43,'admin','module','index','',1,'',0,0),(45,'模块后台列表','icon-liebiaosousuo',4,'admin','module','index','',1,'',0,0),(46,'模块商店','icon-caigou-xianxing',42,'admin','moduleshop','index','',1,'',0,0),(47,'插件商店','icon-caigou-xianxing',42,'admin','addonshop','index','',1,'',0,0),(48,'编辑角色','',17,'admin','authManager','editGroup','',1,'',0,0),(49,'删除角色','',17,'admin','authManager','deleteGroup','',1,'',0,0),(50,'访问授权','',17,'admin','authManager','access','',1,'',0,0),(51,'角色授权','',17,'admin','authManager','writeGroup','',1,'',0,0),(52,'模块安装','',44,'admin','module','install','',1,'',0,0),(53,'模块卸载','',44,'admin','module','uninstall','',1,'',0,0),(54,'本地安装','',44,'admin','module','local','',1,'',0,0),(55,'缓存更新','',0,'admin','index','cache','',0,'',0,0),(242,'代理列表','icon-huiyuan',241,'member','agentuser','manage','',1,'',0,0),(243,'充值记录','icon-huiyuan',241,'member','userchongzhi','manage','',1,'',0,0),(239,'机器人聊天发言','icon-huiyuan',236,'member','robotchatmsg','manage','',1,'',0,0),(240,'机器人控制','icon-huiyuan',236,'member','robotcontrol','manage','',1,'',0,0),(238,'机器人上岗时间','icon-huiyuan',236,'member','robottimemanage','manage','',1,'',0,0),(207,'转账记录','icon-huiyuan',205,'gameserver','transferrecord','manage','',1,'',0,0),(206,'游戏记录','icon-huiyuan',205,'gameserver','gamerecord','manage','',1,'',0,0),(205,'游戏记录管理','icon-huiyuan2',200,'gameserver','recordmanage','index','',1,'',0,0),(204,'服务器列表','icon-huiyuan',203,'gameserver','accountserver','manage','',1,'',0,0),(200,'游戏服务器','icon-people',0,'gameserver','gameserver','index1','',1,'',0,4),(150,'游戏列表','icon-huiyuan',149,'games','games','manage','',1,'',0,0),(149,'游戏管理','icon-people',148,'games','games','index','',1,'',0,0),(148,'游戏','icon-people',0,'games','games','index1','',1,'',0,4),(203,'账号服务器管理','icon-people',200,'gameserver','accountserver','index','',1,'',0,0),(202,'服务器列表','icon-huiyuan',201,'gameserver','gameserver','manage','',1,'',0,0),(201,'游戏服务器管理','icon-people',200,'gameserver','gameserver','index','',1,'',0,0),(241,'代理管理','icon-people',233,'member','agentuser','index','',1,'',0,0),(100,'表单管理','icon-shiyongwendang',45,'formguide','formguide','index','',1,'',0,3),(101,'添加表单','',100,'formguide','formguide','add','',0,'',0,0),(102,'编辑表单','',100,'formguide','formguide','edit','',0,'',0,0),(103,'删除表单','',100,'formguide','formguide','delete','',0,'',0,0),(104,'字段管理','',100,'formguide','field','index','',0,'',0,0),(105,'添加字段','',100,'formguide','field','add','',0,'',0,0),(106,'编辑字段','',100,'formguide','field','edit','',0,'',0,0),(107,'删除字段','',100,'formguide','field','delete','',0,'',0,0),(108,'信息列表','',100,'formguide','info','index','',0,'',0,0),(109,'信息删除','',100,'formguide','info','delete','',0,'',0,0),(234,'玩家管理','icon-people',233,'member','member','index','',1,'',0,0),(235,'玩家列表','icon-huiyuan',234,'member','member','manage','',1,'',0,0),(237,'机器人列表','icon-huiyuan',236,'member','robotmanage','manage','',1,'',0,0),(236,'机器人管理','icon-huiyuan2',233,'member','robotmanage','index','',1,'',0,0),(233,'玩家','icon-people',0,'member','member','index1','',1,'',0,4),(117,'短消息','icon-systemprompt',45,'message','message','index','',1,'',0,3),(118,'群发消息列表','',117,'message','message','message_group_manage','',1,'',0,0),(119,'删除短消息','',117,'message','message','delete','',1,'',0,0),(120,'删除群发消息','',117,'message','message','delete_group','',1,'',0,0),(121,'发消息','',117,'message','message','message_send','',1,'',0,0),(122,'收件箱','',117,'message','message','my_inbox','',1,'',0,0),(123,'发件箱','',117,'message','message','my_outbox','',1,'',0,0),(124,'删除收件箱','',117,'message','message','delete_inbox','',1,'',0,0),(125,'删除发件箱','',117,'message','message','delete_outbox','',1,'',0,0),(126,'支付管理','icon-yue',45,'pay','payment','pay_list','',1,'',0,3),(127,'支付模块','',126,'pay','payment','index','',0,'',0,0),(128,'支付配置','',127,'pay','payment','edit','',0,'',0,0);
/*!40000 ALTER TABLE `yzn_menu` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `yzn_message`
--

DROP TABLE IF EXISTS `yzn_message`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `yzn_message` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `send_from` char(30) NOT NULL DEFAULT '0' COMMENT '发件人',
  `send_to` char(30) NOT NULL DEFAULT '0' COMMENT '收件人',
  `create_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '创建时间',
  `subject` char(80) DEFAULT NULL COMMENT '主题',
  `content` text NOT NULL COMMENT '内容',
  `replyid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '回复ID',
  `status` tinyint(2) NOT NULL DEFAULT '0' COMMENT '状态',
  PRIMARY KEY (`id`),
  KEY `replyid` (`replyid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `yzn_message`
--

LOCK TABLES `yzn_message` WRITE;
/*!40000 ALTER TABLE `yzn_message` DISABLE KEYS */;
/*!40000 ALTER TABLE `yzn_message` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `yzn_message_data`
--

DROP TABLE IF EXISTS `yzn_message_data`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `yzn_message_data` (
  `id` int(4) NOT NULL AUTO_INCREMENT,
  `userid` mediumint(8) NOT NULL,
  `group_message_id` int(5) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `message` (`userid`,`group_message_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `yzn_message_data`
--

LOCK TABLES `yzn_message_data` WRITE;
/*!40000 ALTER TABLE `yzn_message_data` DISABLE KEYS */;
/*!40000 ALTER TABLE `yzn_message_data` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `yzn_message_group`
--

DROP TABLE IF EXISTS `yzn_message_group`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `yzn_message_group` (
  `id` smallint(5) unsigned NOT NULL AUTO_INCREMENT,
  `groupid` tinyint(4) unsigned NOT NULL DEFAULT '0' COMMENT '用户组id',
  `subject` char(80) DEFAULT NULL COMMENT '标题',
  `content` text NOT NULL COMMENT '内容',
  `create_time` int(10) unsigned DEFAULT '0' COMMENT '创建时间',
  `status` tinyint(2) NOT NULL DEFAULT '0' COMMENT '状态',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `yzn_message_group`
--

LOCK TABLES `yzn_message_group` WRITE;
/*!40000 ALTER TABLE `yzn_message_group` DISABLE KEYS */;
/*!40000 ALTER TABLE `yzn_message_group` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `yzn_model`
--

DROP TABLE IF EXISTS `yzn_model`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `yzn_model` (
  `id` smallint(5) unsigned NOT NULL AUTO_INCREMENT,
  `module` varchar(15) NOT NULL DEFAULT '' COMMENT '所属模块',
  `name` char(30) NOT NULL DEFAULT '' COMMENT '模型名称',
  `tablename` char(20) NOT NULL DEFAULT '' COMMENT '表名',
  `description` char(100) NOT NULL DEFAULT '' COMMENT '描述',
  `setting` text COMMENT '配置信息',
  `type` tinyint(1) unsigned NOT NULL DEFAULT '1' COMMENT '模型类别：1-独立表，2-主附表',
  `create_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '添加时间',
  `update_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '更新时间',
  `listorders` tinyint(3) NOT NULL DEFAULT '0' COMMENT '排序',
  `status` tinyint(2) NOT NULL DEFAULT '0' COMMENT '状态',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=5 DEFAULT CHARSET=utf8 COMMENT='模型列表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `yzn_model`
--

LOCK TABLES `yzn_model` WRITE;
/*!40000 ALTER TABLE `yzn_model` DISABLE KEYS */;
INSERT INTO `yzn_model` VALUES (1,'cms','文章模型','article','文章模型','a:3:{s:17:\"category_template\";s:13:\"category.html\";s:13:\"list_template\";s:9:\"list.html\";s:13:\"show_template\";s:9:\"show.html\";}',2,1546574975,1566893866,0,1),(2,'cms','图片模型','picture','图片模型','a:3:{s:17:\"category_template\";s:21:\"category_picture.html\";s:13:\"list_template\";s:17:\"list_picture.html\";s:13:\"show_template\";s:17:\"show_picture.html\";}',2,1548754193,1566896531,0,1),(3,'cms','产品模型','product','产品模型','a:3:{s:17:\"category_template\";s:21:\"category_picture.html\";s:13:\"list_template\";s:17:\"list_picture.html\";s:13:\"show_template\";s:17:\"show_picture.html\";}',2,1549165800,1566894329,0,1),(4,'cms','下载模型','download','下载模型','a:3:{s:17:\"category_template\";s:13:\"category.html\";s:13:\"list_template\";s:9:\"list.html\";s:13:\"show_template\";s:9:\"show.html\";}',2,1549624988,1566894292,0,1);
/*!40000 ALTER TABLE `yzn_model` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `yzn_model_field`
--

DROP TABLE IF EXISTS `yzn_model_field`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `yzn_model_field` (
  `id` smallint(5) unsigned NOT NULL AUTO_INCREMENT,
  `modelid` smallint(5) unsigned NOT NULL DEFAULT '0' COMMENT '模型ID',
  `name` varchar(20) NOT NULL DEFAULT '' COMMENT '字段名',
  `title` varchar(30) NOT NULL DEFAULT '' COMMENT '别名',
  `remark` tinytext NOT NULL COMMENT '字段提示',
  `pattern` varchar(255) NOT NULL COMMENT '数据校验正则',
  `errortips` varchar(255) NOT NULL COMMENT '数据校验未通过的提示信息',
  `type` varchar(20) NOT NULL DEFAULT '' COMMENT '字段类型',
  `setting` mediumtext,
  `ifsystem` tinyint(1) unsigned NOT NULL DEFAULT '1' COMMENT '是否主表字段 1 是',
  `iscore` tinyint(1) unsigned NOT NULL DEFAULT '0' COMMENT '是否内部字段',
  `iffixed` tinyint(1) unsigned NOT NULL DEFAULT '0' COMMENT '是否固定不可修改',
  `ifrequire` tinyint(1) unsigned NOT NULL DEFAULT '0' COMMENT '是否必填',
  `ifsearch` tinyint(1) unsigned NOT NULL DEFAULT '0' COMMENT '作为搜索条件',
  `isadd` tinyint(1) unsigned NOT NULL DEFAULT '0' COMMENT '在投稿中显示',
  `create_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '创建时间',
  `update_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '更新时间',
  `listorder` smallint(5) unsigned NOT NULL DEFAULT '0' COMMENT '排序',
  `status` tinyint(2) NOT NULL DEFAULT '0' COMMENT '状态',
  PRIMARY KEY (`id`),
  KEY `name` (`name`,`modelid`)
) ENGINE=MyISAM AUTO_INCREMENT=76 DEFAULT CHARSET=utf8 COMMENT='模型字段列表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `yzn_model_field`
--

LOCK TABLES `yzn_model_field` WRITE;
/*!40000 ALTER TABLE `yzn_model_field` DISABLE KEYS */;
INSERT INTO `yzn_model_field` VALUES (1,1,'id','文档id','','','','hidden','',1,0,1,0,0,1,1546574975,1546574975,100,1),(2,1,'catid','栏目id','','','','hidden','',1,0,1,0,0,1,1546574975,1546574975,100,1),(3,1,'title','标题','','','','text','a:3:{s:6:\"define\";s:32:\"varchar(255) NOT NULL DEFAULT \'\'\";s:7:\"options\";s:0:\"\";s:5:\"value\";s:0:\"\";}',1,0,1,1,1,1,1546574975,1546574975,100,1),(4,1,'flag','属性','','','','checkbox','a:3:{s:6:\"define\";s:31:\"varchar(32) NOT NULL DEFAULT \'\'\";s:7:\"options\";s:76:\"1:置顶[1]\r\n2:头条[2]\r\n3:特荐[3]\r\n4:推荐[4]\r\n5:热点[5]\r\n6:幻灯[6]\";s:5:\"value\";s:0:\"\";}',1,0,1,0,0,0,1551846870,1551846870,100,1),(5,1,'keywords','SEO关键词','多关键词之间用空格或者“,”隔开','','','text','a:3:{s:6:\"define\";s:32:\"varchar(255) NOT NULL DEFAULT \'\'\";s:7:\"options\";s:0:\"\";s:5:\"value\";s:0:\"\";}',1,0,0,0,0,1,1546574975,1546574975,100,1),(6,1,'description','SEO摘要','如不填写，则自动截取附表中编辑器的200字符','','','textarea','a:3:{s:6:\"define\";s:32:\"varchar(255) NOT NULL DEFAULT \'\'\";s:7:\"options\";s:0:\"\";s:5:\"value\";s:0:\"\";}',1,0,0,0,0,1,1546574975,1546574975,100,1),(7,1,'tags','Tags标签','多关键词之间用空格或者“,”隔开','','','text','a:3:{s:6:\"define\";s:32:\"varchar(255) NOT NULL DEFAULT \'\'\";s:7:\"options\";s:0:\"\";s:5:\"value\";s:0:\"\";}',1,0,0,0,0,0,1546574975,1546574975,100,1),(8,1,'uid','用户id','','','','number',NULL,1,1,1,0,0,0,1546574975,1546574975,100,1),(9,1,'username','用户名','','','','text',NULL,1,1,1,0,0,0,1558767044,1558767044,100,1),(10,1,'sysadd','是否后台添加','','','','number',NULL,1,1,1,0,0,0,1558767044,1558767044,100,1),(11,1,'listorder','排序','','','','number','a:3:{s:6:\"define\";s:40:\"tinyint(3) UNSIGNED NOT NULL DEFAULT \'0\'\";s:7:\"options\";s:0:\"\";s:5:\"value\";s:3:\"100\";}',1,0,1,0,0,0,1546574975,1546574975,100,1),(12,1,'status','状态','','','','radio','a:3:{s:6:\"define\";s:40:\"tinyint(2) UNSIGNED NOT NULL DEFAULT \'0\'\";s:7:\"options\";s:21:\"0:待审核\r\n1:通过\";s:5:\"value\";s:1:\"1\";}',1,0,1,0,0,0,1546574975,1546574975,100,1),(13,1,'inputtime','创建时间','','','','datetime','a:3:{s:6:\"define\";s:37:\"int(10) UNSIGNED NOT NULL DEFAULT \'0\'\";s:7:\"options\";s:0:\"\";s:5:\"value\";s:0:\"\";}',1,0,1,0,0,0,1546574975,1546574975,200,1),(14,1,'updatetime','更新时间','','','','datetime','a:3:{s:6:\"define\";s:37:\"int(10) UNSIGNED NOT NULL DEFAULT \'0\'\";s:7:\"options\";s:0:\"\";s:5:\"value\";s:0:\"\";}',1,0,1,0,0,0,1546574975,1546574975,200,1),(15,1,'hits','点击量','','','','number','a:3:{s:6:\"define\";s:42:\"mediumint(8) UNSIGNED NOT NULL DEFAULT \'0\'\";s:7:\"options\";s:0:\"\";s:5:\"value\";s:1:\"0\";}',1,0,1,0,0,0,1546574975,1546574975,200,1),(16,1,'did','附表文档id','','','','hidden','',0,1,1,0,0,0,1546574975,1546574975,100,1),(17,1,'content','内容','','','','Ueditor','a:3:{s:6:\"define\";s:13:\"text NOT NULL\";s:7:\"options\";s:0:\"\";s:5:\"value\";s:0:\"\";}',0,0,0,0,0,1,1546574975,1546574975,100,1),(18,2,'id','文档id','','','','hidden','',1,0,1,0,0,1,1548754192,1548754192,100,1),(19,2,'catid','栏目id','','','','hidden','',1,0,1,0,0,1,1548754192,1548754192,100,1),(20,2,'title','标题','','','','text','a:3:{s:6:\"define\";s:32:\"varchar(255) NOT NULL DEFAULT \'\'\";s:7:\"options\";s:0:\"\";s:5:\"value\";s:0:\"\";}',1,0,1,1,1,1,1548754192,1548754192,100,1),(21,2,'flag','属性','','','','checkbox','a:3:{s:6:\"define\";s:31:\"varchar(32) NOT NULL DEFAULT \'\'\";s:7:\"options\";s:76:\"1:置顶[1]\r\n2:头条[2]\r\n3:特荐[3]\r\n4:推荐[4]\r\n5:热点[5]\r\n6:幻灯[6]\";s:5:\"value\";s:0:\"\";}',1,0,1,0,0,0,1551846870,1551846870,100,1),(22,2,'keywords','SEO关键词','多关键词之间用空格或者“,”隔开','','','text','a:3:{s:6:\"define\";s:32:\"varchar(255) NOT NULL DEFAULT \'\'\";s:7:\"options\";s:0:\"\";s:5:\"value\";s:0:\"\";}',1,0,0,0,0,1,1548754192,1548754192,100,1),(23,2,'description','SEO摘要','如不填写，则自动截取附表中编辑器的200字符','','','textarea','a:3:{s:6:\"define\";s:32:\"varchar(255) NOT NULL DEFAULT \'\'\";s:7:\"options\";s:0:\"\";s:5:\"value\";s:0:\"\";}',1,0,0,0,0,1,1548754192,1548754192,100,1),(24,2,'tags','Tags标签','多关键词之间用空格或者“,”隔开','','','text','a:3:{s:6:\"define\";s:32:\"varchar(255) NOT NULL DEFAULT \'\'\";s:7:\"options\";s:0:\"\";s:5:\"value\";s:0:\"\";}',1,0,0,0,0,0,1546574975,1546574975,100,1),(25,2,'uid','用户id','','','','number',NULL,1,1,1,0,0,0,1548754192,1548754192,100,1),(26,2,'username','用户名','','','','text',NULL,1,1,1,0,0,0,1558767044,1558767044,100,1),(27,2,'sysadd','是否后台添加','','','','number',NULL,1,1,1,0,0,0,1558767044,1558767044,100,1),(28,2,'listorder','排序','','','','number','a:3:{s:6:\"define\";s:40:\"tinyint(3) UNSIGNED NOT NULL DEFAULT \'0\'\";s:7:\"options\";s:0:\"\";s:5:\"value\";s:3:\"100\";}',1,0,1,0,0,0,1548754192,1548754192,100,1),(29,2,'status','状态','','','','radio','a:3:{s:6:\"define\";s:40:\"tinyint(2) UNSIGNED NOT NULL DEFAULT \'0\'\";s:7:\"options\";s:21:\"0:待审核\r\n1:通过\";s:5:\"value\";s:1:\"1\";}',1,0,1,0,0,0,1548754192,1548754192,100,1),(30,2,'inputtime','创建时间','','','','datetime','a:3:{s:6:\"define\";s:37:\"int(10) UNSIGNED NOT NULL DEFAULT \'0\'\";s:7:\"options\";s:0:\"\";s:5:\"value\";s:0:\"\";}',1,0,1,0,0,0,1548754192,1548754192,200,1),(31,2,'updatetime','更新时间','','','','datetime','a:3:{s:6:\"define\";s:37:\"int(10) UNSIGNED NOT NULL DEFAULT \'0\'\";s:7:\"options\";s:0:\"\";s:5:\"value\";s:0:\"\";}',1,0,1,0,0,0,1548754192,1548754192,200,1),(32,2,'hits','点击量','','','','number','a:3:{s:6:\"define\";s:42:\"mediumint(8) UNSIGNED NOT NULL DEFAULT \'0\'\";s:7:\"options\";s:0:\"\";s:5:\"value\";s:1:\"0\";}',1,0,1,0,0,0,1548754192,1548754192,200,1),(33,2,'did','附表文档id','','','','hidden','',0,1,1,0,0,0,1548754192,1548754192,100,1),(34,2,'content','内容','','','','Ueditor','a:3:{s:6:\"define\";s:13:\"text NOT NULL\";s:7:\"options\";s:0:\"\";s:5:\"value\";s:0:\"\";}',0,0,0,0,0,1,1548754192,1548754192,100,1),(35,3,'id','文档id','','','','hidden','',1,0,1,0,0,1,1549165800,1549165800,100,1),(36,3,'catid','栏目id','','','','hidden','',1,0,1,0,0,1,1549165800,1549165800,100,1),(37,3,'title','标题','','','','text','a:3:{s:6:\"define\";s:32:\"varchar(255) NOT NULL DEFAULT \'\'\";s:7:\"options\";s:0:\"\";s:5:\"value\";s:0:\"\";}',1,0,1,1,1,1,1549165800,1549165800,100,1),(38,3,'flag','属性','','','','checkbox','a:3:{s:6:\"define\";s:31:\"varchar(32) NOT NULL DEFAULT \'\'\";s:7:\"options\";s:76:\"1:置顶[1]\r\n2:头条[2]\r\n3:特荐[3]\r\n4:推荐[4]\r\n5:热点[5]\r\n6:幻灯[6]\";s:5:\"value\";s:0:\"\";}',1,0,1,0,0,0,1551846870,1551846870,100,1),(39,3,'keywords','SEO关键词','多关键词之间用空格或者“,”隔开','','','text','a:3:{s:6:\"define\";s:32:\"varchar(255) NOT NULL DEFAULT \'\'\";s:7:\"options\";s:0:\"\";s:5:\"value\";s:0:\"\";}',1,0,0,0,0,1,1549165800,1549165800,100,1),(40,3,'description','SEO摘要','如不填写，则自动截取附表中编辑器的200字符','','','textarea','a:3:{s:6:\"define\";s:32:\"varchar(255) NOT NULL DEFAULT \'\'\";s:7:\"options\";s:0:\"\";s:5:\"value\";s:0:\"\";}',1,0,0,0,0,1,1549165800,1549165800,100,1),(41,3,'tags','Tags标签','多关键词之间用空格或者“,”隔开','','','text','a:3:{s:6:\"define\";s:32:\"varchar(255) NOT NULL DEFAULT \'\'\";s:7:\"options\";s:0:\"\";s:5:\"value\";s:0:\"\";}',1,0,0,0,0,0,1546574975,1546574975,100,1),(42,3,'uid','用户id','','','','number',NULL,1,1,1,0,0,0,1549165800,1549165800,100,1),(43,3,'username','用户名','','','','text',NULL,1,1,1,0,0,0,1558767044,1558767044,100,1),(44,3,'sysadd','是否后台添加','','','','number',NULL,1,1,1,0,0,0,1558767044,1558767044,100,1),(45,3,'listorder','排序','','','','number','a:3:{s:6:\"define\";s:40:\"tinyint(3) UNSIGNED NOT NULL DEFAULT \'0\'\";s:7:\"options\";s:0:\"\";s:5:\"value\";s:3:\"100\";}',1,0,1,0,0,0,1549165800,1549165800,100,1),(46,3,'status','状态','','','','radio','a:3:{s:6:\"define\";s:40:\"tinyint(2) UNSIGNED NOT NULL DEFAULT \'0\'\";s:7:\"options\";s:21:\"0:待审核\r\n1:通过\";s:5:\"value\";s:1:\"1\";}',1,0,1,0,0,0,1549165800,1549165800,100,1),(47,3,'inputtime','创建时间','','','','datetime','a:3:{s:6:\"define\";s:37:\"int(10) UNSIGNED NOT NULL DEFAULT \'0\'\";s:7:\"options\";s:0:\"\";s:5:\"value\";s:0:\"\";}',1,0,1,0,0,0,1549165800,1549165800,200,1),(48,3,'updatetime','更新时间','','','','datetime','a:3:{s:6:\"define\";s:37:\"int(10) UNSIGNED NOT NULL DEFAULT \'0\'\";s:7:\"options\";s:0:\"\";s:5:\"value\";s:0:\"\";}',1,0,1,0,0,0,1549165800,1549165800,200,1),(49,3,'hits','点击量','','','','number','a:3:{s:6:\"define\";s:42:\"mediumint(8) UNSIGNED NOT NULL DEFAULT \'0\'\";s:7:\"options\";s:0:\"\";s:5:\"value\";s:1:\"0\";}',1,0,1,0,0,0,1549165800,1549165800,200,1),(50,3,'did','附表文档id','','','','hidden','',0,1,1,0,0,0,1549165800,1549165800,100,1),(51,3,'content','内容','','','','Ueditor','a:3:{s:6:\"define\";s:13:\"text NOT NULL\";s:7:\"options\";s:0:\"\";s:5:\"value\";s:0:\"\";}',0,0,0,0,0,1,1549165800,1549165800,100,1),(52,3,'type','类型','','','','radio','a:4:{s:6:\"define\";s:40:\"tinyint(2) UNSIGNED NOT NULL DEFAULT \'0\'\";s:7:\"options\";s:91:\"1:营销网站\r\n2:电商网站\r\n3:响应式网站\r\n4:手机网站\r\n5:外贸网站\r\n6:其他\";s:10:\"filtertype\";s:1:\"1\";s:5:\"value\";s:0:\"\";}',1,0,0,1,0,1,1552368369,1552372294,0,1),(53,3,'trade','行业','','','','radio','a:4:{s:6:\"define\";s:40:\"tinyint(2) UNSIGNED NOT NULL DEFAULT \'0\'\";s:7:\"options\";s:78:\"1:机械设备\r\n2:车辆物流\r\n3:地产建筑装修\r\n4:教育培训\r\n5:其他\";s:10:\"filtertype\";s:1:\"1\";s:5:\"value\";s:0:\"\";}',1,0,0,1,0,1,1552372387,1552372387,0,1),(54,3,'price','价格','','','','radio','a:4:{s:6:\"define\";s:40:\"tinyint(2) UNSIGNED NOT NULL DEFAULT \'0\'\";s:7:\"options\";s:42:\"1:≤2500\r\n2:≤5000\r\n3:≤8000\r\n4:≥1万\";s:10:\"filtertype\";s:1:\"1\";s:5:\"value\";s:0:\"\";}',1,0,0,1,0,1,1552372433,1552372433,0,1),(55,4,'id','文档id','','','','hidden','',1,0,1,0,0,1,1549624988,1549624988,100,1),(56,4,'catid','栏目id','','','','hidden','',1,0,1,0,0,1,1549624988,1549624988,100,1),(57,4,'title','标题','','','','text','a:3:{s:6:\"define\";s:32:\"varchar(255) NOT NULL DEFAULT \'\'\";s:7:\"options\";s:0:\"\";s:5:\"value\";s:0:\"\";}',1,0,1,1,1,1,1549624988,1549624988,100,1),(58,4,'flag','属性','','','','checkbox','a:3:{s:6:\"define\";s:31:\"varchar(32) NOT NULL DEFAULT \'\'\";s:7:\"options\";s:76:\"1:置顶[1]\r\n2:头条[2]\r\n3:特荐[3]\r\n4:推荐[4]\r\n5:热点[5]\r\n6:幻灯[6]\";s:5:\"value\";s:0:\"\";}',1,0,1,0,0,0,1551846870,1551846870,100,1),(59,4,'keywords','SEO关键词','多关键词之间用空格或者“,”隔开','','','text','a:3:{s:6:\"define\";s:32:\"varchar(255) NOT NULL DEFAULT \'\'\";s:7:\"options\";s:0:\"\";s:5:\"value\";s:0:\"\";}',1,0,0,0,0,1,1549624988,1549624988,100,1),(60,4,'description','SEO摘要','如不填写，则自动截取附表中编辑器的200字符','','','textarea','a:3:{s:6:\"define\";s:32:\"varchar(255) NOT NULL DEFAULT \'\'\";s:7:\"options\";s:0:\"\";s:5:\"value\";s:0:\"\";}',1,0,0,0,0,1,1549624988,1549624988,100,1),(61,4,'tags','Tags标签','多关键词之间用空格或者“,”隔开','','','text','a:3:{s:6:\"define\";s:32:\"varchar(255) NOT NULL DEFAULT \'\'\";s:7:\"options\";s:0:\"\";s:5:\"value\";s:0:\"\";}',1,0,0,0,0,0,1546574975,1546574975,100,1),(62,4,'uid','用户id','','','','number',NULL,1,1,1,0,0,0,1549624988,1549624988,100,1),(63,4,'username','用户名','','','','text',NULL,1,1,1,0,0,0,1558767044,1558767044,100,1),(64,4,'sysadd','是否后台添加','','','','number',NULL,1,1,1,0,0,0,1558767044,1558767044,100,1),(65,4,'listorder','排序','','','','number','a:3:{s:6:\"define\";s:40:\"tinyint(3) UNSIGNED NOT NULL DEFAULT \'0\'\";s:7:\"options\";s:0:\"\";s:5:\"value\";s:3:\"100\";}',1,0,1,0,0,0,1549624988,1549624988,100,1),(66,4,'status','状态','','','','radio','a:3:{s:6:\"define\";s:40:\"tinyint(2) UNSIGNED NOT NULL DEFAULT \'0\'\";s:7:\"options\";s:21:\"0:待审核\r\n1:通过\";s:5:\"value\";s:1:\"1\";}',1,0,1,0,0,0,1549624988,1549624988,100,1),(67,4,'inputtime','创建时间','','','','datetime','a:3:{s:6:\"define\";s:37:\"int(10) UNSIGNED NOT NULL DEFAULT \'0\'\";s:7:\"options\";s:0:\"\";s:5:\"value\";s:0:\"\";}',1,0,1,0,0,0,1549624988,1549624988,200,1),(68,4,'updatetime','更新时间','','','','datetime','a:3:{s:6:\"define\";s:37:\"int(10) UNSIGNED NOT NULL DEFAULT \'0\'\";s:7:\"options\";s:0:\"\";s:5:\"value\";s:0:\"\";}',1,0,1,0,0,0,1549624988,1549624988,200,1),(69,4,'hits','点击量','','','','number','a:3:{s:6:\"define\";s:42:\"mediumint(8) UNSIGNED NOT NULL DEFAULT \'0\'\";s:7:\"options\";s:0:\"\";s:5:\"value\";s:1:\"0\";}',1,0,1,0,0,0,1549624988,1549624988,200,1),(70,4,'did','附表文档id','','','','hidden','',0,1,1,0,0,0,1549624988,1549624988,100,1),(71,4,'content','内容','','','','Ueditor','a:3:{s:6:\"define\";s:13:\"text NOT NULL\";s:7:\"options\";s:0:\"\";s:5:\"value\";s:0:\"\";}',0,0,0,0,0,1,1549624988,1549624988,100,1),(72,1,'thumb','缩略图','','','','image','a:3:{s:6:\"define\";s:36:\"int(5) UNSIGNED NOT NULL DEFAULT \'0\'\";s:7:\"options\";s:0:\"\";s:5:\"value\";s:0:\"\";}',1,0,0,0,0,1,1565948216,1565948216,100,1),(73,2,'thumb','缩略图','','','','image','a:3:{s:6:\"define\";s:36:\"int(5) UNSIGNED NOT NULL DEFAULT \'0\'\";s:7:\"options\";s:0:\"\";s:5:\"value\";s:0:\"\";}',1,0,0,0,0,1,1565948216,1565948216,100,1),(74,3,'thumb','缩略图','','','','image','a:3:{s:6:\"define\";s:36:\"int(5) UNSIGNED NOT NULL DEFAULT \'0\'\";s:7:\"options\";s:0:\"\";s:5:\"value\";s:0:\"\";}',1,0,0,0,0,1,1565948216,1565948216,100,1),(75,4,'thumb','缩略图','','','','image','a:3:{s:6:\"define\";s:36:\"int(5) UNSIGNED NOT NULL DEFAULT \'0\'\";s:7:\"options\";s:0:\"\";s:5:\"value\";s:0:\"\";}',1,0,0,0,0,1,1565948216,1565948216,100,1);
/*!40000 ALTER TABLE `yzn_model_field` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `yzn_module`
--

DROP TABLE IF EXISTS `yzn_module`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `yzn_module` (
  `module` varchar(15) NOT NULL COMMENT '模块',
  `name` varchar(20) NOT NULL DEFAULT '' COMMENT '模块名称',
  `sign` varchar(255) NOT NULL DEFAULT '' COMMENT '签名',
  `iscore` tinyint(1) unsigned NOT NULL DEFAULT '0' COMMENT '内置模块',
  `version` varchar(50) NOT NULL DEFAULT '' COMMENT '版本',
  `setting` mediumtext COMMENT '设置信息',
  `installtime` int(10) NOT NULL DEFAULT '0' COMMENT '安装时间',
  `updatetime` int(10) NOT NULL DEFAULT '0' COMMENT '更新时间',
  `listorder` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT '排序',
  `status` tinyint(2) NOT NULL DEFAULT '0' COMMENT '状态',
  PRIMARY KEY (`module`),
  KEY `sign` (`sign`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='已安装模块列表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `yzn_module`
--

LOCK TABLES `yzn_module` WRITE;
/*!40000 ALTER TABLE `yzn_module` DISABLE KEYS */;
INSERT INTO `yzn_module` VALUES ('gameserver','游戏服务器管理模块','23ae619de3f13fa5b4bf8e33a43a56bc',0,'1.0.0','a:6:{s:13:\"allowregister\";s:1:\"1\";s:14:\"registerverify\";s:1:\"0\";s:16:\"openverification\";s:1:\"1\";s:12:\"defualtpoint\";s:1:\"0\";s:13:\"defualtamount\";s:1:\"0\";s:14:\"rmb_point_rate\";s:2:\"10\";}',1572488963,1572488963,0,1),('games','游戏管理模块','23ae619de3f13fa5b4bf8e33a43a56bc',0,'1.0.0','a:6:{s:13:\"allowregister\";s:1:\"1\";s:14:\"registerverify\";s:1:\"0\";s:16:\"openverification\";s:1:\"1\";s:12:\"defualtpoint\";s:1:\"0\";s:13:\"defualtamount\";s:1:\"0\";s:14:\"rmb_point_rate\";s:2:\"10\";}',1571042085,1571042085,0,1),('formguide','表单模块','1fa2d9a6f16e75616918c57ce3b88440',0,'1.0.0',NULL,1569297058,1569297058,0,1),('member','会员模块','fcfe4d97f35d1f30df5d6018a84f74ba',0,'1.0.0','a:6:{s:13:\"allowregister\";s:1:\"1\";s:14:\"registerverify\";s:1:\"0\";s:16:\"openverification\";s:1:\"1\";s:12:\"defualtpoint\";s:1:\"0\";s:13:\"defualtamount\";s:1:\"0\";s:14:\"rmb_point_rate\";s:2:\"10\";}',1573021472,1573021472,0,1),('message','消息模块','b19cc279ed484c13c96c2f7142e2f437',0,'1.0.0',NULL,1569297072,1569297072,0,1),('pay','支付模块','b803d6de0bf866df8350ca074efdd02e',0,'1.0.0',NULL,1569297076,1569297076,0,1);
/*!40000 ALTER TABLE `yzn_module` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `yzn_page`
--

DROP TABLE IF EXISTS `yzn_page`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `yzn_page` (
  `catid` smallint(5) unsigned NOT NULL DEFAULT '0' COMMENT '栏目ID',
  `title` varchar(160) NOT NULL DEFAULT '' COMMENT '标题',
  `keywords` varchar(255) NOT NULL DEFAULT '' COMMENT '关键字',
  `description` varchar(255) NOT NULL DEFAULT '' COMMENT 'SEO描述',
  `content` text COMMENT '内容',
  `inputtime` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '创建时间',
  `updatetime` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '更新时间',
  PRIMARY KEY (`catid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='单页内容表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `yzn_page`
--

LOCK TABLES `yzn_page` WRITE;
/*!40000 ALTER TABLE `yzn_page` DISABLE KEYS */;
INSERT INTO `yzn_page` VALUES (2,'关于我们','','','<p>&nbsp; &nbsp; xxx网络科技股份有限公司是一家集策略咨询、创意创新、视觉设计、技术研发、内容制造、营销推广为一体的综合型数字化创新服务企业，其利用公司持续积累的核心技术和互联网思维，提供以互联网、移动互联网为核心的网络技术服务和互动整合营销服务，为传统企业实现“互联网+”升级提供整套解决方案。公司定位于中大型企业为核心客户群，可充分满足这一群体相比中小企业更为丰富、高端、多元的互联网数字综合需求。</p><p><br/></p><p>&nbsp; &nbsp; xxx网络科技股份有限公司作为一家互联网数字服务综合商，其主营业务包括移动互联网应用开发服务、数字互动整合营销服务、互联网网站建设综合服务和电子商务综合服务。</p><p><br/></p><p>&nbsp; &nbsp; xxx网络科技股份有限公司秉承实现全网价值营销的理念，通过实现互联网与移动互联网的精准数字营销和用户数据分析，日益深入到客户互联网技术建设及运维营销的方方面面，在帮助客户形成自身互联网运作体系的同时，有效对接BAT(百度，阿里，腾讯)等平台即百度搜索、阿里电商、腾讯微信，通过平台的推广来推进互联网综合服务，实现企业、用户、平台三者完美对接，并形成高效互动的枢纽，在帮助客户获取互联网高附加价值的同时获得自身的不断成长和壮大。</p>',0,0),(3,'企业文化','','','<p>【愿景】</p><ul class=\" list-paddingleft-2\" style=\"list-style-type: disc;\"><li><p>不断倾听和满足用户需求，引导并超越用户需求，赢得用户尊敬</p></li><li><p>通过提升品牌形象，使员工具有高度企业荣誉感，赢得员工尊敬&nbsp;</p></li><li><p>推动互联网行业的健康发展，与合作伙伴共成长，赢得行业尊敬</p></li><li><p>注重企业责任，用心服务，关爱社会、回馈社会，赢得社会尊敬</p></li></ul><p><br/></p><p>【使命】</p><ul class=\" list-paddingleft-2\" style=\"list-style-type: disc;\"><li><p>使产品和服务像水和电融入人们的生活，为人们带来便捷和愉悦</p></li><li><p>关注不同地域、群体，并针对不同对象提供差异化的产品和服务</p></li><li><p>打造开放共赢平台，与合作伙伴共同营造健康的互联网生态环境</p></li></ul><p><br/></p><p>【管理理念】</p><ul class=\" list-paddingleft-2\" style=\"list-style-type: disc;\"><li><p>为员工提供良好的工作环境和激励机制&nbsp;</p></li><li><p>完善员工培养体系和职业发展通道，使员工与企业同步成长</p></li><li><p>充分尊重和信任员工，不断引导和鼓励，使其获得成就的喜悦</p></li></ul>',0,0),(18,'联系我们','','','<p>手　机：158-88888888</p><p>传　真：0512-88888888</p><p>邮　编：215000</p><p>邮　箱：admin@admin.com</p><p>地　址：江苏省苏州市吴中区某某工业园一区</p><p><br/></p><p><img width=\"530\" height=\"340\" src=\"http://api.map.baidu.com/staticimage?center=116.404,39.915&zoom=10&width=530&height=340&markers=116.404,39.915\"/></p>',0,0);
/*!40000 ALTER TABLE `yzn_page` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `yzn_pay_account`
--

DROP TABLE IF EXISTS `yzn_pay_account`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `yzn_pay_account` (
  `id` mediumint(8) unsigned NOT NULL AUTO_INCREMENT COMMENT '交易ID',
  `trade_sn` char(50) NOT NULL COMMENT '订单ID',
  `uid` int(11) unsigned NOT NULL COMMENT '用户ID',
  `username` char(16) NOT NULL COMMENT '用户名',
  `money` char(8) NOT NULL COMMENT '价格',
  `addtime` int(10) NOT NULL DEFAULT '0' COMMENT '添加时间',
  `paytime` int(10) NOT NULL DEFAULT '0' COMMENT '支付时间',
  `usernote` char(255) NOT NULL COMMENT '备注',
  `pay_type` enum('offline','recharge','selfincome','online') NOT NULL DEFAULT 'recharge' COMMENT '支付类型',
  `payment` char(90) NOT NULL COMMENT '支付方式名称',
  `type` tinyint(1) unsigned NOT NULL DEFAULT '1' COMMENT '1金钱or2点数',
  `ip` bigint(20) NOT NULL DEFAULT '0' COMMENT 'IP',
  `status` enum('succ','failed','error','progress','timeout','cancel','waitting','unpay') NOT NULL DEFAULT 'unpay' COMMENT '状态',
  `adminnote` char(20) NOT NULL COMMENT '管理员记录',
  PRIMARY KEY (`id`),
  KEY `status` (`status`),
  KEY `uid` (`uid`),
  KEY `trade_sn` (`trade_sn`,`money`,`status`,`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='交易数据表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `yzn_pay_account`
--

LOCK TABLES `yzn_pay_account` WRITE;
/*!40000 ALTER TABLE `yzn_pay_account` DISABLE KEYS */;
/*!40000 ALTER TABLE `yzn_pay_account` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `yzn_pay_payment`
--

DROP TABLE IF EXISTS `yzn_pay_payment`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `yzn_pay_payment` (
  `id` tinyint(3) unsigned NOT NULL AUTO_INCREMENT COMMENT '方式ID',
  `name` varchar(120) NOT NULL COMMENT '方式名称',
  `title` varchar(50) NOT NULL,
  `icon` varchar(64) NOT NULL DEFAULT '' COMMENT '图标',
  `pay_desc` text NOT NULL COMMENT '支付描述',
  `config` text NOT NULL COMMENT '配置信息',
  `status` tinyint(2) NOT NULL DEFAULT '0' COMMENT '状态',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=3 DEFAULT CHARSET=utf8 COMMENT='支付模块表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `yzn_pay_payment`
--

LOCK TABLES `yzn_pay_payment` WRITE;
/*!40000 ALTER TABLE `yzn_pay_payment` DISABLE KEYS */;
INSERT INTO `yzn_pay_payment` VALUES (1,'alipay','支付宝','icon-zhifubao-copy-copy','支付宝是国内领先的独立第三方支付平台，由阿里巴巴集团创办。致力于为中国电子商务提供“简单、安全、快速”的在线支付解决方案。','',1),(2,'wechat','微信','icon-weixin','腾讯集团旗下中国领先的第三方支付平台，致力于为用户和企业提供安全、便捷、专业的在线支付服务','',1);
/*!40000 ALTER TABLE `yzn_pay_payment` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `yzn_pay_spend`
--

DROP TABLE IF EXISTS `yzn_pay_spend`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `yzn_pay_spend` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT '消费ID',
  `creat_at` char(20) NOT NULL COMMENT '消费流水号',
  `uid` int(11) unsigned NOT NULL COMMENT '用户ID',
  `username` char(16) NOT NULL COMMENT '用户名',
  `type` tinyint(1) unsigned NOT NULL DEFAULT '1' COMMENT '1金钱or2点数',
  `money` char(8) NOT NULL COMMENT '价格',
  `msg` varchar(30) NOT NULL COMMENT '类型说明',
  `addtime` int(10) NOT NULL DEFAULT '0' COMMENT '添加时间',
  `ip` bigint(20) NOT NULL DEFAULT '0' COMMENT 'IP',
  `remarks` char(255) NOT NULL COMMENT '备注说明',
  PRIMARY KEY (`id`),
  KEY `type` (`type`),
  KEY `creat_at` (`creat_at`),
  KEY `uid` (`uid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='消费记录表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `yzn_pay_spend`
--

LOCK TABLES `yzn_pay_spend` WRITE;
/*!40000 ALTER TABLE `yzn_pay_spend` DISABLE KEYS */;
/*!40000 ALTER TABLE `yzn_pay_spend` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `yzn_picture`
--

DROP TABLE IF EXISTS `yzn_picture`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `yzn_picture` (
  `id` mediumint(8) unsigned NOT NULL AUTO_INCREMENT COMMENT '文档ID',
  `catid` smallint(5) unsigned NOT NULL DEFAULT '0' COMMENT '栏目ID',
  `title` varchar(255) COLLATE utf8_unicode_ci NOT NULL DEFAULT '' COMMENT '标题',
  `thumb` int(5) unsigned NOT NULL DEFAULT '0' COMMENT '缩略图',
  `flag` varchar(32) COLLATE utf8_unicode_ci NOT NULL DEFAULT '' COMMENT '属性',
  `keywords` varchar(255) COLLATE utf8_unicode_ci NOT NULL DEFAULT '' COMMENT 'SEO关键词',
  `description` varchar(255) COLLATE utf8_unicode_ci NOT NULL DEFAULT '' COMMENT 'SEO描述',
  `tags` varchar(255) COLLATE utf8_unicode_ci NOT NULL DEFAULT '' COMMENT 'Tags标签',
  `listorder` smallint(5) unsigned NOT NULL DEFAULT '100' COMMENT '排序',
  `uid` mediumint(8) unsigned NOT NULL DEFAULT '0' COMMENT '用户id',
  `username` varchar(20) COLLATE utf8_unicode_ci NOT NULL DEFAULT '' COMMENT '用户名',
  `sysadd` tinyint(1) unsigned NOT NULL DEFAULT '0' COMMENT '是否后台添加',
  `hits` mediumint(8) unsigned DEFAULT '0' COMMENT '点击量',
  `inputtime` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '创建时间',
  `updatetime` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '更新时间',
  `status` tinyint(2) NOT NULL DEFAULT '0' COMMENT '状态',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=6 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='图片模型模型表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `yzn_picture`
--

LOCK TABLES `yzn_picture` WRITE;
/*!40000 ALTER TABLE `yzn_picture` DISABLE KEYS */;
INSERT INTO `yzn_picture` VALUES (1,4,'ISO9001证书',0,'','','','',100,1,'admin',1,0,1550552511,1550554247,1),(2,4,'ISO14001证书',0,'','','','',100,1,'admin',1,0,1550554284,1550554288,1),(3,4,'OHSAS18001证书',0,'','','','',100,1,'admin',1,0,1550554298,1550554301,1),(4,4,'企业信用等级评价',0,'','','','',100,1,'admin',1,0,1550554307,1550554309,1),(5,4,'企业荣誉证书',0,'','','','',100,1,'admin',1,0,1550554314,1550554316,1);
/*!40000 ALTER TABLE `yzn_picture` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `yzn_picture_data`
--

DROP TABLE IF EXISTS `yzn_picture_data`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `yzn_picture_data` (
  `did` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `content` text COLLATE utf8_unicode_ci COMMENT '内容',
  PRIMARY KEY (`did`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='图片模型模型表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `yzn_picture_data`
--

LOCK TABLES `yzn_picture_data` WRITE;
/*!40000 ALTER TABLE `yzn_picture_data` DISABLE KEYS */;
/*!40000 ALTER TABLE `yzn_picture_data` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `yzn_product`
--

DROP TABLE IF EXISTS `yzn_product`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `yzn_product` (
  `id` mediumint(8) unsigned NOT NULL AUTO_INCREMENT COMMENT '文档ID',
  `catid` smallint(5) unsigned NOT NULL DEFAULT '0' COMMENT '栏目ID',
  `title` varchar(255) COLLATE utf8_unicode_ci NOT NULL DEFAULT '' COMMENT '标题',
  `thumb` int(5) unsigned NOT NULL DEFAULT '0' COMMENT '缩略图',
  `flag` varchar(32) COLLATE utf8_unicode_ci NOT NULL DEFAULT '' COMMENT '属性',
  `keywords` varchar(255) COLLATE utf8_unicode_ci NOT NULL DEFAULT '' COMMENT 'SEO关键词',
  `description` varchar(255) COLLATE utf8_unicode_ci NOT NULL DEFAULT '' COMMENT 'SEO描述',
  `tags` varchar(255) COLLATE utf8_unicode_ci NOT NULL DEFAULT '' COMMENT 'Tags标签',
  `listorder` smallint(5) unsigned NOT NULL DEFAULT '100' COMMENT '排序',
  `uid` mediumint(8) unsigned NOT NULL DEFAULT '0' COMMENT '用户id',
  `username` varchar(20) COLLATE utf8_unicode_ci NOT NULL DEFAULT '' COMMENT '用户名',
  `sysadd` tinyint(1) unsigned NOT NULL DEFAULT '0' COMMENT '是否后台添加',
  `hits` mediumint(8) unsigned DEFAULT '0' COMMENT '点击量',
  `inputtime` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '创建时间',
  `updatetime` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '更新时间',
  `status` tinyint(2) NOT NULL DEFAULT '0' COMMENT '状态',
  `type` tinyint(2) unsigned NOT NULL DEFAULT '0' COMMENT '类型',
  `trade` tinyint(2) unsigned NOT NULL DEFAULT '0' COMMENT '行业',
  `price` tinyint(2) unsigned NOT NULL DEFAULT '0' COMMENT '价格',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=7 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='产品模型模型表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `yzn_product`
--

LOCK TABLES `yzn_product` WRITE;
/*!40000 ALTER TABLE `yzn_product` DISABLE KEYS */;
INSERT INTO `yzn_product` VALUES (1,5,'苏州欧泊**机电进出口有限公司',0,'','','苏州欧伯**机电进出口有限公司成立于2014年，是德国HUK/DOPAG/METER MIX定量注脂、打胶产品正式授权的代理商，另经销德国CAPTRON、NORELEM等众多国外知名品牌，可为客户提供从技术咨询、产品销售、技术支持到售后服务的全程服务。公司自成立以来，一直致力于为客户提供德国及欧美地区原产的各类工业备品、备件，并确保100%原厂全新正品。','注脂,打胶',100,1,'admin',1,0,1552365964,1553067407,1,4,1,1),(4,5,'南通红*居餐饮管理有限公司',0,'4','','南通红*居餐饮管理有限公司是一家具有自己的厨师团队，具备丰富的经验，专业承包及管理企事业机关单位、学校、医院、大型工业园区、工厂，建筑工地、写字楼的食堂及营养配餐等后勤项目的大型餐饮承包企业，在上海、江苏、浙江、都有设立公司营业部。团队有500余人，并可为各企业提供专业厨师团队、厨工，勤杂工，免费厨房餐饮管理;餐饮服务，保洁服务，家庭服务；停车场管理服务；劳务派遣经营；绿化维护，食品经营;婚庆礼仪服务;会...','餐饮管理,婚庆礼仪,劳务派遣',100,1,'admin',1,0,1552366803,1553067319,1,3,5,3),(2,5,'海安华**仓储有限公司',0,'','','海安华**仓储有限公司主要以海安物流、仓储为主。 公司秉承“诚信经营、服务至上”的核心价值观；先进的物流理念和丰富的物流操作经验，为不同客户量身定做及提供专业物流方案和优质、高效的物流服务，从而帮客户降低成本，提升市场竞争力。　　公司已和众多知名企业携手合作，共创辉煌；承运范围涵盖了化工、机械、建材、纺织、电子电器、食品、制药、高科技产品等各行各业。','物流服务',100,1,'admin',1,0,1552366267,1553067377,1,5,2,3),(3,5,'苏州领*线教育科技有限公司',0,'4','','苏州领*线教育科技有限公司是一家专注于高品质少儿培训和智能课程开发、提供精品化与专业化相结合的少儿教育科技机构。我们多年来，始终致力于将我们的课堂打造成为孩子快乐成长的乐园与成功的起点，并且成为江苏校外教育的品牌。我们已在苏州市区、常熟、张家港、吴江、昆山、太仓、常州、无锡、杭州、江西九江、内蒙古通辽、山东淄博等地成功开办了教学基地。','校外教育,智能课程',100,1,'admin',1,0,1552366358,1553067339,1,3,4,4),(5,5,'苏州威*莱斯升降机械设备有限公司',0,'4','','苏州威*莱斯升降机械设备有限公司是专业提供各类升降平台出租租赁服务的厂家，自公司创立以来，经过长期对液压升降机、升降平台、高空作业车的研制和探索，积累了丰富的专业经验。凭借丰富的专业技术及不断开拓创新的精神，致力于产品的开发和创新，以自身专业特长，创造高品质的产品。','机械设备,升降机,租赁服务',100,1,'admin',1,0,1552366872,1553067296,1,3,1,2),(6,5,'苏州非*搬运包装有限公司',0,'4','','苏州非*搬运安装有限公司于2016年7月在苏州工业园区这块沸腾的热土上重组合并挂牌成立。随着经济的发展,，公司秉承创新、创优、与时共进的原则把原有四家搬运公司重组洗牌、强强联手。作为一家专业承接各类精密机器设备搬运、装卸、安装定位工程的大企业，公司装备齐全、技术力量雄厚。施工人员均受过国家安全生产监督部门正规的专业培训，持有相应的资格证书，对各种精密机器设备的搬运、安装工程有着丰富的工作经验。施工车辆都是...','精密机器,专业培训,资格证书',100,1,'admin',1,0,1552366920,1553067267,1,1,5,1);
/*!40000 ALTER TABLE `yzn_product` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `yzn_product_data`
--

DROP TABLE IF EXISTS `yzn_product_data`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `yzn_product_data` (
  `did` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `content` text COLLATE utf8_unicode_ci COMMENT '内容',
  PRIMARY KEY (`did`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='产品模型模型表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `yzn_product_data`
--

LOCK TABLES `yzn_product_data` WRITE;
/*!40000 ALTER TABLE `yzn_product_data` DISABLE KEYS */;
INSERT INTO `yzn_product_data` VALUES (1,'&lt;p&gt;苏州欧伯**机电进出口有限公司成立于2014年，是德国HUK/DOPAG/METER MIX定量注脂、打胶产品正式授权的代理商，另经销德国CAPTRON、NORELEM等众多国外知名品牌，可为客户提供从技术咨询、产品销售、技术支持到售后服务的全程服务。公司自成立以来，一直致力于为客户提供德国及欧美地区原产的各类工业备品、备件，并确保100%原厂全新正品。&lt;/p&gt;'),(4,'&lt;p&gt;南通红*居餐饮管理有限公司是一家具有自己的厨师团队，具备丰富的经验，专业承包及管理企事业机关单位、学校、医院、大型工业园区、工厂，建筑工地、写字楼的食堂及营养配餐等后勤项目的大型餐饮承包企业，在上海、江苏、浙江、都有设立公司营业部。团队有500余人，并可为各企业提供专业厨师团队、厨工，勤杂工，免费厨房餐饮管理;餐饮服务，保洁服务，家庭服务；停车场管理服务；劳务派遣经营；绿化维护，食品经营;婚庆礼仪服务;会务服务;展览展示服务;厨房设备及用品、餐具、办公用品的销售、餐厅规划设计，厨具设计、出售、安装等服务&lt;/p&gt;'),(2,'&lt;p&gt;海安华**仓储有限公司主要以海安物流、仓储为主。 公司秉承“诚信经营、服务至上”的核心价值观；先进的物流理念和丰富的物流操作经验，为不同客户量身定做及提供专业物流方案和优质、高效的物流服务，从而帮客户降低成本，提升市场竞争力。\r\n　　公司已和众多知名企业携手合作，共创辉煌；承运范围涵盖了化工、机械、建材、纺织、电子电器、食品、制药、高科技产品等各行各业。&lt;/p&gt;'),(3,'&lt;p&gt;苏州领*线教育科技有限公司是一家专注于高品质少儿培训和智能课程开发、提供精品化与专业化相结合的少儿教育科技机构。我们多年来，始终致力于将我们的课堂打造成为孩子快乐成长的乐园与成功的起点，并且成为江苏校外教育的品牌。我们已在苏州市区、常熟、张家港、吴江、昆山、太仓、常州、无锡、杭州、江西九江、内蒙古通辽、山东淄博等地成功开办了教学基地。&lt;/p&gt;'),(5,'&lt;p&gt;苏州威*莱斯升降机械设备有限公司是专业提供各类升降平台出租租赁服务的厂家，自公司创立以来，经过长期对液压升降机、升降平台、高空作业车的研制和探索，积累了丰富的专业经验。凭借丰富的专业技术及不断开拓创新的精神，致力于产品的开发和创新，以自身专业特长，创造高品质的产品。&lt;/p&gt;'),(6,'&lt;p&gt;苏州非*搬运安装有限公司于2016年7月在苏州工业园区这块沸腾的热土上重组合并挂牌成立。随着经济的发展,，公司秉承创新、创优、与时共进的原则把原有四家搬运公司重组洗牌、强强联手。作为一家专业承接各类精密机器设备搬运、装卸、安装定位工程的大企业，公司装备齐全、技术力量雄厚。施工人员均受过国家安全生产监督部门正规的专业培训，持有相应的资格证书，对各种精密机器设备的搬运、安装工程有着丰富的工作经验。施工车辆都是定期由特种设备监督局定期安检的，搬运工具都是领先的。长期与多家财险、寿险公司合作，保证客户利益的最大化，员工利益的最大保障。&lt;/p&gt;');
/*!40000 ALTER TABLE `yzn_product_data` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `yzn_sms`
--

DROP TABLE IF EXISTS `yzn_sms`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `yzn_sms` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT 'ID',
  `event` varchar(30) NOT NULL DEFAULT '' COMMENT '事件',
  `mobile` varchar(20) NOT NULL DEFAULT '' COMMENT '手机号',
  `code` varchar(10) NOT NULL DEFAULT '' COMMENT '验证码',
  `times` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '验证次数',
  `ip` bigint(20) NOT NULL DEFAULT '0' COMMENT 'IP',
  `create_time` int(10) unsigned DEFAULT '0' COMMENT '创建时间',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='短信验证码表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `yzn_sms`
--

LOCK TABLES `yzn_sms` WRITE;
/*!40000 ALTER TABLE `yzn_sms` DISABLE KEYS */;
/*!40000 ALTER TABLE `yzn_sms` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `yzn_tags`
--

DROP TABLE IF EXISTS `yzn_tags`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `yzn_tags` (
  `id` smallint(5) unsigned NOT NULL AUTO_INCREMENT COMMENT 'tagID',
  `tag` char(20) NOT NULL DEFAULT '' COMMENT 'tag名称',
  `seo_title` varchar(255) NOT NULL DEFAULT '' COMMENT 'seo标题',
  `seo_keyword` varchar(255) NOT NULL DEFAULT '' COMMENT 'seo关键字',
  `seo_description` varchar(255) NOT NULL DEFAULT '' COMMENT 'seo简介',
  `usetimes` smallint(5) unsigned NOT NULL DEFAULT '0' COMMENT '信息总数',
  `hits` mediumint(8) unsigned NOT NULL DEFAULT '0' COMMENT '点击数',
  `create_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '添加时间',
  `update_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '更新时间',
  `listorder` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT '排序',
  PRIMARY KEY (`id`),
  UNIQUE KEY `tag` (`tag`),
  KEY `usetimes` (`usetimes`,`listorder`),
  KEY `hits` (`hits`,`listorder`)
) ENGINE=MyISAM AUTO_INCREMENT=15 DEFAULT CHARSET=utf8 COMMENT='tags主表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `yzn_tags`
--

LOCK TABLES `yzn_tags` WRITE;
/*!40000 ALTER TABLE `yzn_tags` DISABLE KEYS */;
INSERT INTO `yzn_tags` VALUES (1,'精密机器','','','',1,0,1553067267,1553067267,0),(2,'专业培训','','','',1,0,1553067267,1553067267,0),(3,'资格证书','','','',1,0,1553067267,1553067267,0),(4,'机械设备','','','',1,0,1553067296,1553067296,0),(5,'升降机','','','',1,0,1553067296,1553067296,0),(6,'租赁服务','','','',1,0,1553067296,1553067296,0),(7,'餐饮管理','','','',1,0,1553067319,1553067319,0),(8,'婚庆礼仪','','','',1,0,1553067319,1553067319,0),(9,'劳务派遣','','','',1,0,1553067319,1553067319,0),(10,'校外教育','','','',1,0,1553067339,1553067339,0),(11,'智能课程','','','',1,0,1553067339,1553067339,0),(12,'物流服务','','','',1,0,1553067377,1553067377,0),(13,'注脂','','','',1,0,1553067408,1553067408,0),(14,'打胶','','','',1,0,1553067408,1553067408,0);
/*!40000 ALTER TABLE `yzn_tags` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `yzn_tags_content`
--

DROP TABLE IF EXISTS `yzn_tags_content`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `yzn_tags_content` (
  `tag` char(20) NOT NULL COMMENT 'tag名称',
  `modelid` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT '模型ID',
  `contentid` mediumint(8) unsigned NOT NULL DEFAULT '0' COMMENT '信息ID',
  `catid` smallint(5) unsigned NOT NULL DEFAULT '0' COMMENT '栏目ID',
  `updatetime` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '更新时间',
  KEY `modelid` (`modelid`,`contentid`),
  KEY `tag` (`tag`(10))
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='tags数据表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `yzn_tags_content`
--

LOCK TABLES `yzn_tags_content` WRITE;
/*!40000 ALTER TABLE `yzn_tags_content` DISABLE KEYS */;
INSERT INTO `yzn_tags_content` VALUES ('精密机器',3,6,5,1553067267),('专业培训',3,6,5,1553067267),('资格证书',3,6,5,1553067267),('机械设备',3,5,5,1553067296),('升降机',3,5,5,1553067296),('租赁服务',3,5,5,1553067296),('餐饮管理',3,4,5,1553067319),('婚庆礼仪',3,4,5,1553067319),('劳务派遣',3,4,5,1553067319),('校外教育',3,3,5,1553067339),('智能课程',3,3,5,1553067339),('物流服务',3,2,5,1553067377),('注脂',3,1,5,1553067408),('打胶',3,1,5,1553067408);
/*!40000 ALTER TABLE `yzn_tags_content` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `yzn_terms`
--

DROP TABLE IF EXISTS `yzn_terms`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `yzn_terms` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT COMMENT '分类ID',
  `parentid` smallint(5) NOT NULL DEFAULT '0' COMMENT '父ID',
  `name` varchar(20) NOT NULL DEFAULT '' COMMENT '分类名称',
  `module` varchar(15) NOT NULL DEFAULT '' COMMENT '所属模块',
  `setting` mediumtext COMMENT '相关配置信息',
  PRIMARY KEY (`id`),
  KEY `name` (`name`),
  KEY `module` (`module`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='分类表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `yzn_terms`
--

LOCK TABLES `yzn_terms` WRITE;
/*!40000 ALTER TABLE `yzn_terms` DISABLE KEYS */;
/*!40000 ALTER TABLE `yzn_terms` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Dumping routines for database 'yzncms'
--
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2019-11-06 15:00:48
