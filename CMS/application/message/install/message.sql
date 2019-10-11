DROP TABLE IF EXISTS `yzn_message`;
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

DROP TABLE IF EXISTS `yzn_message_data`;
CREATE TABLE `yzn_message_data` (
  `id` int(4) NOT NULL AUTO_INCREMENT,
  `userid` mediumint(8) NOT NULL,
  `group_message_id` int(5) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `message` (`userid`,`group_message_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `yzn_message_group`;
CREATE TABLE `yzn_message_group` (
  `id` smallint(5) unsigned NOT NULL AUTO_INCREMENT,
  `groupid` tinyint(4) unsigned NOT NULL DEFAULT '0' COMMENT '用户组id',
  `subject` char(80) DEFAULT NULL COMMENT '标题',
  `content` text NOT NULL COMMENT '内容',
  `create_time` int(10) unsigned DEFAULT '0' COMMENT '创建时间',
  `status` tinyint(2) NOT NULL DEFAULT '0' COMMENT '状态',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;