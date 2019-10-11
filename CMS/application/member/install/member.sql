DROP TABLE IF EXISTS `yzn_member`;
CREATE TABLE `yzn_member` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT COMMENT '用户ID',
  `username` char(16) NOT NULL COMMENT '用户名',
  `nickname` char(16) NOT NULL DEFAULT '' COMMENT '昵称',
  `password` char(32) NOT NULL COMMENT '密码',
  `encrypt` varchar(6) DEFAULT NULL COMMENT '加密因子',
  `point` mediumint(8) NOT NULL DEFAULT '0' COMMENT '用户积分',
  `amount` decimal(8,2) unsigned NOT NULL DEFAULT '0.00' COMMENT '钱金总额',
  `login` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '登录次数',
  `email` char(32) NOT NULL DEFAULT '' COMMENT '电子邮箱',
  `mobile` varchar(11) NOT NULL DEFAULT '' COMMENT '手机号',
  `avatar` mediumint(8) unsigned NOT NULL DEFAULT '0' COMMENT '头像',
  `groupid` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT '用户组ID',
  `modelid` smallint(5) unsigned NOT NULL DEFAULT '0' COMMENT '用户模型ID',
  `reg_ip` bigint(20) NOT NULL DEFAULT '0' COMMENT '注册IP',
  `reg_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '注册时间',
  `last_login_ip` bigint(20) NOT NULL DEFAULT '0' COMMENT '最后登录IP',
  `last_login_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '最后登录时间',
  `ischeck_email` tinyint(1) NOT NULL DEFAULT '0' COMMENT '是否验证过邮箱',
  `ischeck_mobile` tinyint(1) NOT NULL DEFAULT '0' COMMENT '是否验证过手机',
  `status` tinyint(2) NOT NULL DEFAULT '0' COMMENT '状态',
  PRIMARY KEY (`id`),
  UNIQUE KEY `username` (`username`),
  KEY `email` (`email`),
  KEY `mobile` (`mobile`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='会员表';

DROP TABLE IF EXISTS `yzn_member_group`;
CREATE TABLE `yzn_member_group` (
  `id` tinyint(3) unsigned NOT NULL AUTO_INCREMENT COMMENT '会员组id',
  `name` char(15) NOT NULL COMMENT '用户组名称',
  `issystem` tinyint(1) unsigned NOT NULL DEFAULT '0' COMMENT '是否是系统组',
  `starnum` tinyint(2) unsigned NOT NULL COMMENT '会员组星星数',
  `point` smallint(6) unsigned NOT NULL COMMENT '积分范围',
  `allowmessage` smallint(5) unsigned NOT NULL DEFAULT '0' COMMENT '许允发短消息数量',
  `allowvisit` tinyint(1) unsigned NOT NULL DEFAULT '0' COMMENT '是否允许访问',
  `allowpost` tinyint(1) unsigned NOT NULL DEFAULT '0' COMMENT '是否允许发稿',
  `allowpostverify` tinyint(1) unsigned NOT NULL DEFAULT '0' COMMENT '是否投稿不需审核',
  `allowsearch` tinyint(1) unsigned NOT NULL DEFAULT '0' COMMENT '是否允许搜索',
  `allowupgrade` tinyint(1) unsigned NOT NULL DEFAULT '1' COMMENT '是否允许自主升级',
  `allowsendmessage` tinyint(1) unsigned NOT NULL DEFAULT '0' COMMENT '允许发送短消息',
  `allowpostnum` smallint(5) unsigned NOT NULL DEFAULT '0' COMMENT '每天允许发文章数',
  `allowattachment` tinyint(1) unsigned NOT NULL DEFAULT '0' COMMENT '是否允许上传附件',
  `icon` mediumint(8) unsigned NOT NULL DEFAULT '0' COMMENT '用户组图标',
  `usernamecolor` char(7) NOT NULL COMMENT '用户名颜色',
  `description` char(100) NOT NULL COMMENT '描述',
  `listorder` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT '排序',
  `status` tinyint(2) NOT NULL DEFAULT '0' COMMENT '状态',
  `expand` mediumtext COMMENT '拓展',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=9 DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `yzn_member_content`;
CREATE TABLE `yzn_member_content` (
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `catid` smallint(5) NOT NULL COMMENT '栏目ID',
  `content_id` int(10) NOT NULL COMMENT '信息ID',
  `uid` mediumint(8) NOT NULL COMMENT '会员ID',
  `username` char(16) NOT NULL COMMENT '用户名',
  `create_time` int(10) NOT NULL COMMENT '添加时间',
  `status` tinyint(2) NOT NULL DEFAULT '0' COMMENT '状态',
  PRIMARY KEY (`id`),
  KEY `uid` (`catid`,`content_id`,`status`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='会员投稿信息记录表';


INSERT INTO `yzn_member_group` VALUES ('1', '禁止访问', '1', '0', '0', '0', '1', '1', '0', '1', '0', '0', '0', '0', '0', '', '0', '0', '1', '');
INSERT INTO `yzn_member_group` VALUES ('2', '新手上路', '1', '1', '50', '100', '1', '1', '0', '0', '0', '1', '0', '0', '0', '', '', '2', '1', '');
INSERT INTO `yzn_member_group` VALUES ('4', '中级会员', '1', '3', '150', '500', '1', '1', '0', '1', '1', '1', '0', '0', '0', '', '', '4', '1', '');
INSERT INTO `yzn_member_group` VALUES ('5', '高级会员', '1', '5', '300', '999', '1', '1', '0', '1', '1', '1', '0', '0', '0', '', '', '5', '1', '');
INSERT INTO `yzn_member_group` VALUES ('6', '注册会员', '1', '2', '100', '150', '0', '1', '0', '0', '1', '1', '0', '0', '0', '', '', '6', '1', '');
INSERT INTO `yzn_member_group` VALUES ('7', '邮件认证', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '#000000', '', '7', '1', '');
INSERT INTO `yzn_member_group` VALUES ('8', '游客', '1', '0', '0', '0', '0', '0', '0', '1', '0', '0', '0', '0', '0', '', '', '0', '1', '');