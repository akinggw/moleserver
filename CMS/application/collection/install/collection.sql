DROP TABLE IF EXISTS `yzn_collection_node`;
CREATE TABLE `yzn_collection_node` (
  `id` smallint(6) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(20) NOT NULL COMMENT '采集节点',
  `lastdate` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '最后采集时间',
  `sourcecharset` varchar(8) NOT NULL COMMENT '采集点字符集',
  `sourcetype` tinyint(1) unsigned NOT NULL DEFAULT '0' COMMENT '网址类型:1序列网址,2单页',
  `urlpage` text NOT NULL COMMENT '采集地址',
  `pagesize_start` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT '页码开始',
  `pagesize_end` mediumint(8) unsigned NOT NULL DEFAULT '0' COMMENT '页码结束',
  `par_num` tinyint(3) unsigned NOT NULL DEFAULT '1' COMMENT '每次增加数',
  `down_attachment` tinyint(1) unsigned NOT NULL DEFAULT '0' COMMENT '是否下载图片',
  `watermark` tinyint(1) unsigned NOT NULL DEFAULT '0' COMMENT '图片加水印',
  `coll_order` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT '导入顺序',
  `customize_config` text NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='采集任务表';