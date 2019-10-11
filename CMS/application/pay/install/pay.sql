DROP TABLE IF EXISTS `yzn_pay_account`;
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

DROP TABLE IF EXISTS `yzn_pay_payment`;
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

INSERT INTO `yzn_pay_payment` VALUES ('1', 'alipay', '支付宝','icon-zhifubao-copy-copy', '支付宝是国内领先的独立第三方支付平台，由阿里巴巴集团创办。致力于为中国电子商务提供“简单、安全、快速”的在线支付解决方案。', '', '1');
INSERT INTO `yzn_pay_payment` VALUES ('2', 'wechat', '微信','icon-weixin', '腾讯集团旗下中国领先的第三方支付平台，致力于为用户和企业提供安全、便捷、专业的在线支付服务', '', '1');


DROP TABLE IF EXISTS `yzn_pay_spend`;
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