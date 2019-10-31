-- MySQL dump 10.13  Distrib 5.7.27, for Linux (x86_64)
--
-- Host: localhost    Database: moleweb
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
-- Table structure for table `mol_accountserver`
--

DROP TABLE IF EXISTS `mol_accountserver`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `mol_accountserver` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `servername` varchar(150) DEFAULT NULL,
  `serverport` int(6) NOT NULL DEFAULT '0',
  `serverip` varchar(120) NOT NULL DEFAULT '',
  `createtime` datetime NOT NULL,
  `serverstate` int(1) NOT NULL DEFAULT '0',
  `curplayercount` int(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `mol_accountserver`
--

LOCK TABLES `mol_accountserver` WRITE;
/*!40000 ALTER TABLE `mol_accountserver` DISABLE KEYS */;
INSERT INTO `mol_accountserver` VALUES (1,'验证服务器2',3333,'127.0.0.1','2019-10-31 09:54:00',1,0);
/*!40000 ALTER TABLE `mol_accountserver` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `mol_androiduserinfo`
--

DROP TABLE IF EXISTS `mol_androiduserinfo`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `mol_androiduserinfo` (
  `userid` int(6) NOT NULL,
  `nullity` int(1) NOT NULL DEFAULT '0',
  `kindid` int(6) NOT NULL DEFAULT '0',
  `serverid` int(6) NOT NULL,
  `createdate` datetime NOT NULL,
  PRIMARY KEY (`userid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `mol_androiduserinfo`
--

LOCK TABLES `mol_androiduserinfo` WRITE;
/*!40000 ALTER TABLE `mol_androiduserinfo` DISABLE KEYS */;
INSERT INTO `mol_androiduserinfo` VALUES (22,0,300001,1,'2019-10-16 14:37:28');
/*!40000 ALTER TABLE `mol_androiduserinfo` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `mol_game`
--

DROP TABLE IF EXISTS `mol_game`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `mol_game` (
  `id` int(6) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(100) NOT NULL,
  `type` int(1) NOT NULL,
  `maxversion` int(10) NOT NULL DEFAULT '0',
  `processname` varchar(32) NOT NULL,
  `gamelogo` varchar(32) NOT NULL,
  `content` text NOT NULL,
  `state` int(1) NOT NULL DEFAULT '0',
  `showindex` int(1) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=300002 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `mol_game`
--

LOCK TABLES `mol_game` WRITE;
/*!40000 ALTER TABLE `mol_game` DISABLE KEYS */;
INSERT INTO `mol_game` VALUES (300001,'欢乐斗地主',0,16777218,'libgame_ddz.so','hlddzlog.png','haha',0,12);
/*!40000 ALTER TABLE `mol_game` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `mol_gamerecords`
--

DROP TABLE IF EXISTS `mol_gamerecords`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `mol_gamerecords` (
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `userid` int(6) NOT NULL,
  `score` int(10) NOT NULL DEFAULT '0',
  `revenue` int(10) NOT NULL DEFAULT '0',
  `gameid` int(6) NOT NULL,
  `serverid` int(6) NOT NULL,
  `roomname` varchar(100) NOT NULL,
  `collectdate` datetime NOT NULL,
  `tableindex` int(6) NOT NULL DEFAULT '-1',
  `chairindex` int(6) NOT NULL DEFAULT '-1',
  `lastmoney` bigint(15) NOT NULL DEFAULT '0',
  `gametip` text,
  `agentmoney` bigint(15) NOT NULL DEFAULT '0',
  `curjetton` bigint(15) NOT NULL DEFAULT '0',
  KEY `id` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `mol_gamerecords`
--

LOCK TABLES `mol_gamerecords` WRITE;
/*!40000 ALTER TABLE `mol_gamerecords` DISABLE KEYS */;
INSERT INTO `mol_gamerecords` VALUES (1,2,-2,0,300001,1,'欢乐斗地主普通场','2019-10-22 13:38:24',0,0,4998,'',0,0);
/*!40000 ALTER TABLE `mol_gamerecords` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `mol_gameroom`
--

DROP TABLE IF EXISTS `mol_gameroom`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `mol_gameroom` (
  `id` int(6) unsigned NOT NULL AUTO_INCREMENT,
  `gameid` int(6) NOT NULL,
  `servername` varchar(120) NOT NULL DEFAULT '',
  `serverport` int(6) NOT NULL DEFAULT '0',
  `tablecount` int(6) NOT NULL DEFAULT '0',
  `tableplayercount` int(6) NOT NULL DEFAULT '0',
  `serverip` varchar(120) NOT NULL DEFAULT '',
  `gamingtype` int(1) NOT NULL DEFAULT '0',
  `lastmoney` bigint(15) NOT NULL DEFAULT '0',
  `pielement` bigint(15) NOT NULL DEFAULT '0',
  `roomrevenue` int(6) NOT NULL DEFAULT '0',
  `currealplayercount` int(6) NOT NULL DEFAULT '0',
  `currobotplayercount` int(6) NOT NULL DEFAULT '0',
  `createtime` datetime NOT NULL,
  `state` int(1) DEFAULT '0',
  `QueueGaming` int(1) DEFAULT '0',
  `serverparameter` text,
  PRIMARY KEY (`id`),
  KEY `gameid` (`gameid`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `mol_gameroom`
--

LOCK TABLES `mol_gameroom` WRITE;
/*!40000 ALTER TABLE `mol_gameroom` DISABLE KEYS */;
INSERT INTO `mol_gameroom` VALUES (1,300001,'欢乐斗地主普通场',3335,10,3,'127.0.0.1',0,10,1,1,1,0,'2019-10-31 15:47:56',0,0,'{\r\n\"方块\":3.8,\r\n\"梅花\":3.8,\r\n\"黑桃\":4.0,\r\n\"红桃\":4.0,\r\n\"王\":20.0,\r\n\"开始时间\":3,\r\n\"下注时间\":55,\r\n\"开牌时间\":5,\r\n\"结算时间\":5\r\n}');
/*!40000 ALTER TABLE `mol_gameroom` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `mol_goldoperaterecords`
--

DROP TABLE IF EXISTS `mol_goldoperaterecords`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `mol_goldoperaterecords` (
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `suid` int(11) NOT NULL,
  `duid` int(11) NOT NULL DEFAULT '0',
  `money` bigint(15) NOT NULL,
  `type` enum('1','2','3') DEFAULT NULL,
  `operatedate` datetime NOT NULL,
  `amoney` bigint(15) DEFAULT '0',
  `bmoney` bigint(15) DEFAULT '0',
  `aftermoney` bigint(15) DEFAULT '0',
  `afterbankmoney` bigint(15) DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `suid` (`suid`),
  KEY `duid` (`duid`),
  KEY `sduid` (`suid`,`duid`),
  KEY `stuid` (`suid`,`type`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `mol_goldoperaterecords`
--

LOCK TABLES `mol_goldoperaterecords` WRITE;
/*!40000 ALTER TABLE `mol_goldoperaterecords` DISABLE KEYS */;
/*!40000 ALTER TABLE `mol_goldoperaterecords` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `mol_member`
--

DROP TABLE IF EXISTS `mol_member`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `mol_member` (
  `uid` int(11) NOT NULL AUTO_INCREMENT,
  `gtype` int(1) DEFAULT NULL,
  `username` varchar(20) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `password` varchar(50) NOT NULL,
  `useravatar` varchar(150) NOT NULL,
  `bankpassword` varchar(50) NOT NULL,
  `email` varchar(50) NOT NULL,
  `sex` int(1) NOT NULL,
  `realname` varchar(100) NOT NULL,
  `homeplace` varchar(100) NOT NULL DEFAULT '',
  `telephone` varchar(100) NOT NULL DEFAULT '',
  `qq` varchar(100) NOT NULL DEFAULT '',
  `ipaddress` varchar(25) NOT NULL DEFAULT '',
  `createtime` int(11) NOT NULL,
  `lastlogintime` int(11) DEFAULT NULL,
  `genable` int(1) NOT NULL DEFAULT '1',
  `ruid` int(11) NOT NULL DEFAULT '0',
  `identitycard` varchar(20) NOT NULL DEFAULT '',
  `machinecode` varchar(50) NOT NULL DEFAULT '',
  `glockmachine` int(1) NOT NULL DEFAULT '0',
  `commissionratio` int(5) NOT NULL DEFAULT '0',
  PRIMARY KEY (`uid`),
  KEY `username` (`username`)
) ENGINE=InnoDB AUTO_INCREMENT=27 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `mol_member`
--

LOCK TABLES `mol_member` WRITE;
/*!40000 ALTER TABLE `mol_member` DISABLE KEYS */;
INSERT INTO `mol_member` VALUES (2,0,'test','d0970714757783e6cf17b26fb8e2298f','/asdf/sadfsa.png','14e1b600b1fd579f47433b88e8d85291','test@126.com',0,'test12','','23423434','','127.0.0.1',1570688921,1572338553,1,0,'','html5',0,0),(22,1,'悲凉的小蘑菇','35fafcb81da97d05bb0d56cb3a2cf770','7.png','35fafcb81da97d05bb0d56cb3a2cf770','80484@163.com',1,'悲凉的小蘑菇 ','','','','66.141.188.142',1571207848,NULL,1,0,'','',0,0);
/*!40000 ALTER TABLE `mol_member` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `mol_robotcontroltimes`
--

DROP TABLE IF EXISTS `mol_robotcontroltimes`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `mol_robotcontroltimes` (
  `id` int(6) NOT NULL AUTO_INCREMENT,
  `startcollectdate` time NOT NULL,
  `endcollectdate` time NOT NULL,
  PRIMARY KEY (`id`),
  KEY `id` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `mol_robotcontroltimes`
--

LOCK TABLES `mol_robotcontroltimes` WRITE;
/*!40000 ALTER TABLE `mol_robotcontroltimes` DISABLE KEYS */;
INSERT INTO `mol_robotcontroltimes` VALUES (4,'03:00:00','04:00:00');
/*!40000 ALTER TABLE `mol_robotcontroltimes` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `mol_userdata`
--

DROP TABLE IF EXISTS `mol_userdata`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `mol_userdata` (
  `userid` int(6) NOT NULL,
  `money` bigint(15) NOT NULL DEFAULT '0',
  `bankmoney` bigint(15) NOT NULL DEFAULT '0',
  `revenue` bigint(15) NOT NULL DEFAULT '0',
  `totalresult` bigint(15) NOT NULL DEFAULT '0',
  `level` int(6) NOT NULL DEFAULT '0',
  `experience` int(6) NOT NULL DEFAULT '0',
  `totalbureau` int(6) NOT NULL DEFAULT '0',
  `sbureau` int(6) NOT NULL DEFAULT '0',
  `failbureau` int(6) NOT NULL DEFAULT '0',
  `runawaybureau` int(6) NOT NULL DEFAULT '0',
  `successrate` float NOT NULL DEFAULT '0',
  `runawayrate` float NOT NULL DEFAULT '0',
  `curtableindex` int(6) NOT NULL DEFAULT '-1',
  `curchairindex` int(6) NOT NULL DEFAULT '-1',
  `curgametype` int(6) NOT NULL DEFAULT '0',
  `curserverport` int(6) NOT NULL DEFAULT '0',
  `curgamingstate` int(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`userid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `mol_userdata`
--

LOCK TABLES `mol_userdata` WRITE;
/*!40000 ALTER TABLE `mol_userdata` DISABLE KEYS */;
INSERT INTO `mol_userdata` VALUES (2,4954,0,0,-46,0,4,2,1,2,0,0.5,0,-1,-1,0,0,0),(22,3695,0,0,0,0,0,0,0,0,0,0,0,-1,-1,0,0,0),(25,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,0,0,0);
/*!40000 ALTER TABLE `mol_userdata` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Dumping routines for database 'moleweb'
--
/*!50003 DROP PROCEDURE IF EXISTS `delandroid` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `delandroid`(
	in paid int(11),
	in pkindid int(6),
	in pserverid int(6),
	in ptype int
	)
delandroidproc:begin
	declare t_error int default 0;
	
	declare continue handler for sqlexception set t_error=1;
	
	start transaction;

	if ptype=3 then
		DELETE FROM `mol_member` WHERE uid in (SELECT userid FROM mol_androiduserinfo WHERE kindid=pkindid AND serverid=pserverid);
		DELETE FROM `mol_userdata` WHERE userid in (SELECT userid FROM mol_androiduserinfo WHERE kindid=pkindid AND serverid=pserverid);
		DELETE FROM `mol_androiduserinfo` WHERE kindid=pkindid AND serverid=pserverid;
	
	elseif ptype=2 then
		DELETE FROM `mol_member` WHERE uid in (SELECT userid FROM mol_androiduserinfo WHERE kindid=pkindid);
		DELETE FROM `mol_userdata` WHERE userid in (SELECT userid FROM mol_androiduserinfo WHERE kindid=pkindid);
		DELETE FROM `mol_androiduserinfo` WHERE kindid=pkindid;
	
	elseif ptype=1 then
		DELETE FROM `mol_member` WHERE uid=paid;
		DELETE FROM `mol_userdata` WHERE userid=paid;
		DELETE FROM `mol_androiduserinfo` WHERE userid=paid;
	
	else
		set t_error = 1;
	end if;
	
	if t_error=1 then
		rollback;
		select(0);
	else
		commit;  
		select(1);
	end if;
end ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `deluser` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `deluser`(
	in puid int(11)
	)
deluserproc:begin
	declare oldgtype int default 0;
	declare t_error int default 0;
	
	declare continue handler for sqlexception set t_error=1;
	
	start transaction;
	
	DELETE FROM `mol_member` WHERE uid=puid;
	DELETE FROM `mol_userdata` WHERE userid=puid;
	
	if t_error=1 then
		rollback;
		select(0);
	else
		commit;  
		select(1);
	end if;
end ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `gameserver_getuserdata` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `gameserver_getuserdata`(
	in puserid int(11)
	)
begin
	select * from mol_userdata where userid=puserid;
	select gtype,username,sex,realname,ipaddress from mol_member where uid=puserid;
end ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `gameserver_insertgamerecord` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `gameserver_insertgamerecord`(
	in puserid int(11),
	in pscore bigint(15),
	in prevenue bigint(15),	
	in pgameid int(11),	
	in pserverid int(11),		
	in proomname varchar(120) CHARSET utf8,
	in ptableid int,
	in pchairid int,
	in plastmoney bigint(15),	
	in pgametip text CHARSET utf8,
	in pagentmoney bigint(15),
	in pcurjetton bigint(15)
	)
insertgamerecordproc:begin
	declare pcruid int;
	declare t_error int default 0; 

	declare continue handler for sqlexception set t_error=1; 
	
	set pcruid = 0;
	
	insert into mol_gamerecords (userid,score,revenue,gameid,serverid,roomname,collectdate,tableindex,chairindex,lastmoney,gametip,agentmoney,curjetton) values(puserid,pscore,prevenue,pgameid,pserverid,proomname,NOW(),ptableid,pchairid,plastmoney,pgametip,pagentmoney,pcurjetton);

	if pagentmoney = 0 then
		select(0);
		leave insertgamerecordproc;	
	end if;
	
	select ruid into pcruid from mol_member where uid=puserid;
	
	if pcruid = 0 then
		select(1);
		leave insertgamerecordproc;	
	end if;
	
	start transaction;
	
	update mol_userdata set bankmoney=bankmoney+pagentmoney,agentmoney=agentmoney+pagentmoney where userid=pcruid;
	
	if t_error=1 then
		rollback; 
		select(0);
	else
		commit; 
		select(2);
	end if;		
end ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `gameserver_unlockgameuser` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `gameserver_unlockgameuser`(
	)
begin
	update mol_userdata set curtableindex=-1,curchairindex=-1,curgametype=0,curserverport=0,curgamingstate=0;
end ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `getuserdata` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `getuserdata`(
	in puserid int(11)
	)
begin
	select mb.*,ud.money,ud.bankmoney,ud.level,ud.experience from mol_member as mb left join mol_userdata as ud on ud.userid=puserid where mb.uid=puserid;
end ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `isexistuser` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `isexistuser`(
	in pusername varchar(20) CHARSET utf8,
	in ppassword varchar(50) CHARSET utf8
	)
isexistuserproce:begin
	declare lastuserid int;

	set lastuserid = 0;
	
	select uid into lastuserid from mol_member where username=pusername and password=ppassword limit 1;
	
	if lastuserid = 0 then
		select(0);
		leave isexistuserproce;
	end if;
	select(lastuserid);

end ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `isrobotincontroltimes` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `isrobotincontroltimes`()
isrobotincontroltimesproc:begin
    declare done int;
    declare pstartcollectdate time;
    declare pendcollectdate time;
    declare pcurtime time;

    declare getrobotcontroltimes CURSOR for   
		select startcollectdate as pstartcollectdate,endcollectdate as pendcollectdate from mol_robotcontroltimes; 
	DECLARE CONTINUE HANDLER FOR NOT FOUND SET done=1; 
    
    select date_format(now(), '%H:%i:%s') into pcurtime;
    
    OPEN getrobotcontroltimes; 
		posLoop:LOOP
			IF done=1 THEN 
				LEAVE posLoop;
			END IF; 
			
            FETCH  getrobotcontroltimes into pstartcollectdate,pendcollectdate;  
            
            if pcurtime >= pstartcollectdate  and pcurtime < pendcollectdate then
				select(1);
				leave isrobotincontroltimesproc;
            end if;
            
		END LOOP posLoop; 
    CLOSE getrobotcontroltimes;    
    
    select(pendcollectdate);
end ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `registerandroid` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `registerandroid`(
	in pname varchar(20) CHARSET utf8,
	in ppassword varchar(50) CHARSET utf8,
	in pemail varchar(50) CHARSET utf8,
	in psex int(1),
	in prealname varchar(100) CHARSET utf8,
	in ptelephone varchar(100) CHARSET utf8,
	in puseravatar varchar(100) CHARSET utf8,
	in pipaddress varchar(25) CHARSET utf8,
	in pid varchar(20) CHARSET utf8,
	in pmoney bigint(15),
	in pnullity int(1),
	in pkindid int(6),
	in pserverid int(6)
	)
registerandroidproc:begin
	declare lastuserid int;
	declare t_error int default 0;

	declare continue handler for sqlexception set t_error=1;
	
	set lastuserid = 0;
	
	if pname ='' then
		select(0);
		leave registerandroidproc;
	end if;
	
	select uid into lastuserid from mol_member where username=pname;
	
	if lastuserid > 0 then
		select(0);
		leave registerandroidproc;
	end if;
	
	set lastuserid = 0;
	
	start transaction;

	insert into mol_member (gtype,username,password,useravatar,bankpassword,email,sex,realname,telephone,ipaddress,createtime,ruid,identitycard) values(
		1,pname,ppassword,puseravatar,ppassword,pemail,psex,prealname,ptelephone,pipaddress,unix_timestamp(NOW()),lastuserid,pid);
	
	select LAST_INSERT_ID() into lastuserid; 
	
	if lastuserid > 0 then
		insert into mol_userdata (userid,money) values(lastuserid,pmoney);
		insert into mol_androiduserinfo (userid,nullity,kindid,serverid,createdate) VALUES (lastuserid,pnullity,pkindid,pserverid,NOW());
			
		if t_error=1 then
        		rollback;
		        select(0);
		else
		        commit; 
		        select(lastuserid);
		end if;
		
		leave registerandroidproc;
	end if;
	rollback;
	select(0);
end ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `registergameuser` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `registergameuser`(
	in pname varchar(20) CHARSET utf8,
	in ppassword varchar(50) CHARSET utf8,
	in pemail varchar(50) CHARSET utf8,
	in psex int(1),
	in prealname varchar(100) CHARSET utf8,
	in ptelephone varchar(100) CHARSET utf8,
	in puseravatar varchar(100) CHARSET utf8,
	in preferrer varchar(20) CHARSET utf8,
	in pipaddress varchar(25) CHARSET utf8,
	in pid varchar(20) CHARSET utf8
	)
registergameuserproc:begin
	declare lastuserid int;
	declare t_error int default 0;

	declare continue handler for sqlexception set t_error=1; 
	
	set lastuserid = 0;
	
	select uid into lastuserid from mol_member where username=pname;
	
	if lastuserid > 0 then
		select(lastuserid);
		leave registergameuserproc;
	end if;
	
	start transaction;

	insert into mol_member (gtype,username,password,bankpassword,email,sex,realname,useravatar,telephone,ipaddress,createtime,ruid,identitycard) values(
		0,pname,ppassword,ppassword,pemail,psex,prealname,puseravatar,ptelephone,pipaddress,unix_timestamp(NOW()),lastuserid,pid);
	
	select LAST_INSERT_ID() into lastuserid; 	
    
    insert into mol_userdata (userid) values(lastuserid);
	
	if lastuserid > 0 then
		if t_error=1 then
        		rollback;
		        select(0);
		else
		        commit; 
		        select(lastuserid);
		end if;
		leave registergameuserproc;
	else
		rollback;
		select(0);
	end if;
end ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `transferaccounts` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `transferaccounts`(
	in psenduserid int(11),
	in preceiveruser varchar(20) CHARSET utf8,
	in pmoney bigint(15)
	)
transferaccountsproc:begin
	declare pcurgamingstate int;
	declare o_time int;
	declare plastreceiveruserid int;
	declare pusermoney bigint(15);
	declare puserbankmoney bigint(15);
	declare busermoney bigint(15);
	declare buserbankmoney bigint(15);	
	declare t_error int default 0;

	declare continue handler for sqlexception set t_error=1;
	
	set plastreceiveruserid = 0;
	set o_time = 3;
	set busermoney = 0;
	set buserbankmoney = 0;	
	
	select uid into plastreceiveruserid from mol_member where username=preceiveruser;
	
	if plastreceiveruserid <= 0 or plastreceiveruserid = psenduserid then
		select(-1);
		leave transferaccountsproc;	
	end if;
	
	if pmoney < 0 then
		select(1);
		leave transferaccountsproc;
	end if;

	set pcurgamingstate = 0;	

	select curgamingstate into pcurgamingstate from mol_userdata where userid=psenduserid;
	
	if pcurgamingstate > 0 then
		select(-2);
		leave transferaccountsproc;
	end if;

	select money,bankmoney into pusermoney,puserbankmoney from mol_userdata where userid=psenduserid;
	
	if pmoney > pusermoney then
		select(-3);
		leave transferaccountsproc;	
	end if;
	
	start transaction;

	update mol_userdata set money=money-pmoney where userid=psenduserid;
	
	/*set pcurgamingstate = 0;	

	select curgamingstate into pcurgamingstate from mol_userdata where userid=plastreceiveruserid;
	
	if pcurgamingstate > 0 then
		update mol_userdata set bankmoney=bankmoney+pmoney where userid=plastreceiveruserid;	
	else
		update mol_userdata set money=money+pmoney where userid=plastreceiveruserid;
	end if;	*/	
	update mol_userdata set bankmoney=bankmoney+pmoney where userid=plastreceiveruserid;	
	
	select money,bankmoney into busermoney,buserbankmoney from mol_userdata where userid=psenduserid;
	
	insert into mol_goldoperaterecords (suid,duid,money,type,operatedate,amoney,bmoney,aftermoney,afterbankmoney) 
		values (psenduserid,plastreceiveruserid,pmoney,3,NOW(),pusermoney,puserbankmoney,busermoney,buserbankmoney);

	if t_error=1 then
		rollback;
		select(-4);
	else
		commit; 
		select(plastreceiveruserid);
	end if;
end ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `updateandroidmoney` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `updateandroidmoney`(in pname varchar(20),
	in psmoney int(11),
	in pfmoney int(11),
	in pgameid int(11),
	in pserverid int(11))
begin
  declare t_error int default 0;
  declare uuid int default 0;
  declare duid int default 0;
  declare buid int default 0;
  declare pstate int default 0;
  
  declare continue handler for sqlexception set t_error=1;
  
  start transaction;
  if pname !='' then
	  select uid into uuid from mol_member where username = pname;
		if uuid>0 then 
				select count(userid) into buid from mol_androiduserinfo where userid = uuid;
				if buid >0 then
					update mol_userdata set `money` = FLOOR(psmoney + (RAND() * (pfmoney-psmoney+1))) where userid = uuid AND curtableindex ='-1' AND curchairindex ='-1' AND curgametype = 0 AND curserverport =0 AND curgamingstate =0;
				  select row_count() into pstate; 
				end if;
		end if;
  else
    if pgameid !='' AND pserverid !='' then
		select count(userid) into buid from mol_androiduserinfo where kindid = pgameid AND serverid = pserverid;
		if buid >0 then
			update mol_userdata set `money` = FLOOR(psmoney + (RAND() * (pfmoney-psmoney+1))) where userid in(select userid from mol_androiduserinfo where kindid = pgameid AND serverid = pserverid) AND curtableindex ='-1' AND curchairindex ='-1' AND curgametype = 0 AND curserverport =0 AND curgamingstate =0;
		  select row_count() into pstate;
    end if;
	elseif pgameid >0 then
		select count(userid) into buid from mol_androiduserinfo where kindid = pgameid;
		if buid >0 then
			update mol_userdata set `money` = FLOOR(psmoney + (RAND() * (pfmoney-psmoney+1))) where userid in(select userid from mol_androiduserinfo where kindid = pgameid) AND curtableindex ='-1' AND curchairindex ='-1' AND curgametype = 0 AND curserverport =0 AND curgamingstate =0;
		  select row_count() into pstate;
    end if;
	end if;	
  end if;
 
  if t_error=1 then
		rollback;
		select(-1);
	else
		commit;  
		select(pstate);
	end if;
end ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `updateplayergamestate` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `updateplayergamestate`(
	in puserid int(11),
	in pcurtableindex int(11),
	in pcurchairindex int(11),
	in pcurgametype int(11),
	in pcurserverport int(11),
	in pcurgamingstate int(1)
	)
getupdateplayergamestate:begin
	declare t_error int default 0; 

	declare continue handler for sqlexception set t_error=1; 
	
	start transaction;
	
	update mol_userdata set curtableindex=pcurtableindex,curchairindex=pcurchairindex,curgametype=pcurgametype,curserverport=pcurserverport,curgamingstate=pcurgamingstate where userid=puserid;	
	
	if t_error=1 then
		rollback; 
		select(0);
	else
		commit; 
		select(1);
	end if;	
end ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `updateuserinfo` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `updateuserinfo`(
	in puserid int(11),
	in prealname varchar(100) CHARSET utf8,
	in pemail varchar(50) CHARSET utf8,
	in ptelephone varchar(100) CHARSET utf8,	
	in pqq varchar(100) CHARSET utf8,		
	in puseravatar varchar(100) CHARSET utf8,
	in psex int(1)	
	)
begin
	declare t_error int default 0;  
	declare continue handler for sqlexception set t_error=1;

	start transaction;

	update mol_member set realname=prealname,email=pemail,telephone=ptelephone,
		qq=pqq,sex=psex,useravatar=puseravatar where uid=puserid;

	if t_error=1 then
		rollback; 
		select(0);
	else
		commit;  
		select(1);
	end if;	
end ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2019-10-31 16:47:03
