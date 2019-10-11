DROP TABLE IF EXISTS `yzn_category`;
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
  `items` mediumint(8) unsigned NOT NULL DEFAULT '0' COMMENT '文档数量',
  `setting` text COMMENT '相关配置信息',
  `listorder` smallint(5) unsigned NOT NULL DEFAULT '0' COMMENT '排序',
  `status` tinyint(2) NOT NULL DEFAULT '0' COMMENT '状态',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='栏目表';

DROP TABLE IF EXISTS `yzn_category_priv`;
CREATE TABLE `yzn_category_priv` (
  `catid` smallint(5) unsigned NOT NULL DEFAULT '0',
  `roleid` smallint(5) unsigned NOT NULL DEFAULT '0' COMMENT '角色或者组ID',
  `is_admin` tinyint(1) unsigned NOT NULL DEFAULT '0' COMMENT '是否为管理员 1、管理员',
  `action` char(30) NOT NULL DEFAULT '' COMMENT '动作',
  KEY `catid` (`catid`,`roleid`,`is_admin`,`action`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='栏目权限表';

DROP TABLE IF EXISTS `yzn_page`;
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

DROP TABLE IF EXISTS `yzn_tags`;
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
) ENGINE=MyISAM AUTO_INCREMENT=7 DEFAULT CHARSET=utf8 COMMENT='tags主表';


DROP TABLE IF EXISTS `yzn_tags_content`;
CREATE TABLE `yzn_tags_content` (
  `tag` char(20) NOT NULL COMMENT 'tag名称',
  `modelid` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT '模型ID',
  `contentid` mediumint(8) unsigned NOT NULL DEFAULT '0' COMMENT '信息ID',
  `catid` smallint(5) unsigned NOT NULL DEFAULT '0' COMMENT '栏目ID',
  `updatetime` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '更新时间',
  KEY `modelid` (`modelid`,`contentid`),
  KEY `tag` (`tag`(10))
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='tags数据表';