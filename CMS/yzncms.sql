/*
Navicat MySQL Data Transfer

Source Server         : localhost_3306
Source Server Version : 50553
Source Host           : localhost:3306
Source Database       : yzncms

Target Server Type    : MYSQL
Target Server Version : 50553
File Encoding         : 65001

Date: 2019-02-27 13:23:24
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `yzn_addons`
-- ----------------------------
DROP TABLE IF EXISTS `yzn_addons`;
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

-- ----------------------------
-- Table structure for `yzn_admin`
-- ----------------------------
DROP TABLE IF EXISTS `yzn_admin`;
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

-- ----------------------------
-- Records of yzn_admin
-- ----------------------------
INSERT INTO `yzn_admin` VALUES ('1', 'admin', '9724b5e6c56b95f5723009ef81961bfe', '1', 'Wo0bAa', '御宅男', '1546940765', '2130706433', '530765310@qq.com', '1');
INSERT INTO `yzn_admin` VALUES ('2', 'ken678', '932e31f030b850a87702a86c0e16db16', '2', 'Sxq6dR', '御宅男', '1542781151', '2130706433', '530765310@qq.com', '1');

-- ----------------------------
-- Table structure for `yzn_adminlog`
-- ----------------------------
DROP TABLE IF EXISTS `yzn_adminlog`;
CREATE TABLE `yzn_adminlog` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT COMMENT '日志ID',
  `status` tinyint(2) NOT NULL DEFAULT '0' COMMENT '状态',
  `uid` smallint(3) NOT NULL DEFAULT '0' COMMENT '操作者ID',
  `info` text COMMENT '说明',
  `create_time` int(10) unsigned NOT NULL DEFAULT '0',
  `ip` bigint(20) unsigned NOT NULL DEFAULT '0',
  `get` varchar(255) NOT NULL DEFAULT '',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COMMENT='操作日志';

-- ----------------------------
-- Table structure for `yzn_attachment`
-- ----------------------------
DROP TABLE IF EXISTS `yzn_attachment`;
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

-- ----------------------------
-- Table structure for `yzn_auth_group`
-- ----------------------------
DROP TABLE IF EXISTS `yzn_auth_group`;
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

-- ----------------------------
-- Records of yzn_auth_group
-- ----------------------------
INSERT INTO `yzn_auth_group` VALUES ('1', '0', 'admin', '1', '超级管理员', '拥有所有权限', '', '1');
INSERT INTO `yzn_auth_group` VALUES ('2', '1', 'admin', '1', '编辑', '编辑', '', '1');

-- ----------------------------
-- Table structure for `yzn_auth_rule`
-- ----------------------------
DROP TABLE IF EXISTS `yzn_auth_rule`;
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

-- ----------------------------
-- Table structure for `yzn_cache`
-- ----------------------------
DROP TABLE IF EXISTS `yzn_cache`;
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
) ENGINE=MyISAM AUTO_INCREMENT=6 DEFAULT CHARSET=utf8 COMMENT='缓存列队表';

-- ----------------------------
-- Records of yzn_cache
-- ----------------------------
INSERT INTO `yzn_cache` VALUES ('1', 'Config', '网站配置', 'admin', 'Config', 'config_cache', '1');
INSERT INTO `yzn_cache` VALUES ('2', 'Menu', '后台菜单', 'admin', 'Menu', 'menu_cache', '1');
INSERT INTO `yzn_cache` VALUES ('3', 'Module', '可用模块列表', 'admin', 'Module', 'module_cache', '1');
INSERT INTO `yzn_cache` VALUES ('4', 'Model', '模型列表', 'admin', 'Models', 'model_cache', '1');
INSERT INTO `yzn_cache` VALUES ('5', 'ModelField', '模型字段', 'admin', 'ModelField', 'model_field_cache', '1');

-- ----------------------------
-- Table structure for `yzn_config`
-- ----------------------------
DROP TABLE IF EXISTS `yzn_config`;
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

-- ----------------------------
-- Records of yzn_config
-- ----------------------------
INSERT INTO `yzn_config` VALUES ('1', 'web_site_icp', 'text', '备案信息', 'base', '', '', '1551244923', '1551244971', '1', '', '1');
INSERT INTO `yzn_config` VALUES ('2', 'web_site_statistics', 'textarea', '站点代码', 'base', '', '', '1551244957', '1551244957', '1', '', '100');
INSERT INTO `yzn_config` VALUES ('3', 'mail_type', 'radio', '邮件发送模式', 'email', '1:SMTP\r\n2:Mail', '', '1553652833', '1553652915', '1', '1', '1');
INSERT INTO `yzn_config` VALUES ('4', 'mail_smtp_host', 'text', '邮件服务器', 'email', '', '错误的配置发送邮件会导致服务器超时', '1553652889', '1553652917', '1', 'smtp.163.com', '2');
INSERT INTO `yzn_config` VALUES ('5', 'mail_smtp_port', 'text', '邮件发送端口', 'email', '', '不加密默认25,SSL默认465,TLS默认587', '1553653165', '1553653292', '1', '465', '3');
INSERT INTO `yzn_config` VALUES ('6', 'mail_auth', 'radio', '身份认证', 'email', '0:关闭\r\n1:开启', '', '1553658375', '1553658392', '1', '1', '4');
INSERT INTO `yzn_config` VALUES ('7', 'mail_smtp_user', 'text', '用户名', 'email', '', '', '1553653267', '1553658393', '1', '', '5');
INSERT INTO `yzn_config` VALUES ('8', 'mail_smtp_pass', 'text', '密码', 'email', '', '', '1553653344', '1553658394', '1', '', '6');
INSERT INTO `yzn_config` VALUES ('9', 'mail_verify_type', 'radio', '验证方式', 'email', '1:TLS\r\n2:SSL', '', '1553653426', '1553658395', '1', '2', '7');
INSERT INTO `yzn_config` VALUES ('10', 'mail_from', 'text', '发件人邮箱', 'email', '', '', '1553653500', '1553658397', '1', '', '8');
INSERT INTO `yzn_config` VALUES ('11', 'config_group', 'array', '配置分组', 'system', '', '', '1494408414', '1494408414', '1', 'base:基础\r\nemail:邮箱\r\nsystem:系统\r\nupload:上传\r\ndevelop:开发', '0');
INSERT INTO `yzn_config` VALUES ('12', 'theme', 'text', '主题风格', 'system', '', '', '1541752781', '1541756888', '1', 'default', '1');
INSERT INTO `yzn_config` VALUES ('13', 'admin_allow_ip', 'textarea', '后台允许访问IP', 'system', '', '匹配IP段用\"*\"占位，如192.168.*.*，多个IP地址请用英文逗号\",\"分割', '1551244957', '1551244957', '1', '', '2');
INSERT INTO `yzn_config` VALUES ('14', 'upload_image_size', 'text', '图片上传大小限制', 'upload', '', '0为不限制大小，单位：kb', '1540457656', '1552436075', '1', '0', '2');
INSERT INTO `yzn_config` VALUES ('15', 'upload_image_ext', 'text', '允许上传的图片后缀', 'upload', '', '多个后缀用逗号隔开，不填写则不限制类型', '1540457657', '1552436074', '1', 'gif,jpg,jpeg,bmp,png', '1');
INSERT INTO `yzn_config` VALUES ('16', 'upload_file_size', 'text', '文件上传大小限制', 'upload', '', '0为不限制大小，单位：kb', '1540457658', '1552436078', '1', '0', '3');
INSERT INTO `yzn_config` VALUES ('17', 'upload_file_ext', 'text', '允许上传的文件后缀', 'upload', '', '多个后缀用逗号隔开，不填写则不限制类型', '1540457659', '1552436080', '1', 'doc,docx,xls,xlsx,ppt,pptx,pdf,wps,txt,rar,zip,gz,bz2,7z', '4');
INSERT INTO `yzn_config` VALUES ('18', 'upload_driver', 'radio', '上传驱动', 'upload', 'local:本地', '图片或文件上传驱动', '1541752781', '1552436085', '1', 'local', '9');
INSERT INTO `yzn_config` VALUES ('19', 'upload_thumb_water', 'switch', '添加水印', 'upload', '', '', '1552435063', '1552436080', '1', '0', '5');
INSERT INTO `yzn_config` VALUES ('20', 'upload_thumb_water_pic', 'image', '水印图片', 'upload', '', '只有开启水印功能才生效', '1552435183', '1552436081', '1', '', '6');
INSERT INTO `yzn_config` VALUES ('21', 'upload_thumb_water_position', 'radio', '水印位置', 'upload', '1:左上角\r\n2:上居中\r\n3:右上角\r\n4:左居中\r\n5:居中\r\n6:右居中\r\n7:左下角\r\n8:下居中\r\n9:右下角', '只有开启水印功能才生效', '1552435257', '1552436082', '1', '9', '7');
INSERT INTO `yzn_config` VALUES ('22', 'upload_thumb_water_alpha', 'text', '水印透明度', 'upload', '', '请输入0~100之间的数字，数字越小，透明度越高', '1552435299', '1552436083', '1', '50', '8');

-- ----------------------------
-- Table structure for `yzn_field_type`
-- ----------------------------
DROP TABLE IF EXISTS `yzn_field_type`;
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

-- ----------------------------
-- Records of yzn_field_type
-- ----------------------------
INSERT INTO `yzn_field_type` VALUES ('text', '输入框', '1', 'varchar(255) NOT NULL DEFAULT \'\'', '0', '1', '');
INSERT INTO `yzn_field_type` VALUES ('checkbox', '复选框', '2', 'varchar(32) NOT NULL DEFAULT \'\'', '1', '0', '');
INSERT INTO `yzn_field_type` VALUES ('textarea', '多行文本', '3', 'varchar(255) NOT NULL DEFAULT \'\'', '0', '1', '');
INSERT INTO `yzn_field_type` VALUES ('radio', '单选按钮', '4', 'char(10) NOT NULL DEFAULT \'\'', '1', '0', '');
INSERT INTO `yzn_field_type` VALUES ('switch', '开关', '5', 'tinyint(2) UNSIGNED NOT NULL DEFAULT \'0\'', '0', '0', 'isBool');
INSERT INTO `yzn_field_type` VALUES ('array', '数组', '6', 'varchar(512) NOT NULL DEFAULT \'\'', '0', '0', '');
INSERT INTO `yzn_field_type` VALUES ('select', '下拉框', '7', 'char(10) NOT NULL DEFAULT \'\'', '1', '0', '');
INSERT INTO `yzn_field_type` VALUES ('image', '单张图', '8', 'int(5) UNSIGNED NOT NULL DEFAULT \'0\'', '0', '0', 'isNumber');
INSERT INTO `yzn_field_type` VALUES ('tags', '标签', '10', 'varchar(255) NOT NULL DEFAULT \'\'', '0', '1', '');
INSERT INTO `yzn_field_type` VALUES ('number', '数字', '11', 'int(10) UNSIGNED NOT NULL DEFAULT \'0\'', '0', '0', 'isNumber');
INSERT INTO `yzn_field_type` VALUES ('datetime', '日期和时间', '12', 'int(10) UNSIGNED NOT NULL DEFAULT \'0\'', '0', '0', '');
INSERT INTO `yzn_field_type` VALUES ('Ueditor', '百度编辑器', '13', 'text NOT NULL', '0', '1', '');
INSERT INTO `yzn_field_type` VALUES ('images', '多张图', '9', 'varchar(256) NOT NULL DEFAULT \'\'', '0', '0', '');
INSERT INTO `yzn_field_type` VALUES ('color', '颜色值', '17', 'varchar(7) NOT NULL DEFAULT \'\'', '0', '0', '');
INSERT INTO `yzn_field_type` VALUES ('files', '多文件', '15', 'varchar(255) NOT NULL DEFAULT \'\'', '0', '0', '');
INSERT INTO `yzn_field_type` VALUES ('summernote', '简洁编辑器', '14', 'text NOT NULL', '0', '1', '');
INSERT INTO `yzn_field_type` VALUES ('file', '单文件', '16', 'int(5) UNSIGNED NOT NULL DEFAULT \'0\'', '0', '0', 'isNumber');

-- ----------------------------
-- Table structure for `yzn_hooks`
-- ----------------------------
DROP TABLE IF EXISTS `yzn_hooks`;
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
) ENGINE=MyISAM AUTO_INCREMENT=15 DEFAULT CHARSET=utf8 COMMENT='插件和模块钩子';

-- ----------------------------
-- Records of yzn_hooks
-- ----------------------------
INSERT INTO `yzn_hooks` VALUES ('1', 'pageHeader', '页面header钩子，一般用于加载插件CSS文件和代码', '1', '1509174020', '', '', '1', '1');
INSERT INTO `yzn_hooks` VALUES ('2', 'pageFooter', '页面footer钩子，一般用于加载插件JS文件和JS代码', '1', '1509174020', '', '', '1', '1');
INSERT INTO `yzn_hooks` VALUES ('3', 'smsGet', '短信获取行为', '2', '1509174020', '', '', '1', '1');
INSERT INTO `yzn_hooks` VALUES ('4', 'smsSend', '短信发送行为', '2', '1509174020', '', '', '1', '1');
INSERT INTO `yzn_hooks` VALUES ('5', 'smsNotice', '短信发送通知', '2', '1509174020', '', '', '1', '1');
INSERT INTO `yzn_hooks` VALUES ('6', 'smsCheck', '检测短信验证是否正确', '2', '1509174020', '', '', '1', '1');
INSERT INTO `yzn_hooks` VALUES ('7', 'smsFlush', '清空短信验证行为', '2', '1509174020', '', '', '1', '1');
INSERT INTO `yzn_hooks` VALUES ('8', 'emsGet', '邮件获取行为', '2', '1509174020', '', '', '1', '1');
INSERT INTO `yzn_hooks` VALUES ('9', 'emsSend', '邮件发送行为', '2', '1509174020', '', '', '1', '1');
INSERT INTO `yzn_hooks` VALUES ('10', 'emsNotice', '邮件发送通知', '2', '1509174020', '', '', '1', '1');
INSERT INTO `yzn_hooks` VALUES ('11', 'emsCheck', '检测邮件验证是否正确', '2', '1509174020', '', '', '1', '1');
INSERT INTO `yzn_hooks` VALUES ('12', 'emsFlush', '清空邮件验证行为', '2', '1509174020', '', '', '1', '1');
INSERT INTO `yzn_hooks` VALUES ('13', 'uploadAfter', '第三方附件上传钩子', '2', '1509174020', '', '', '1', '1');
INSERT INTO `yzn_hooks` VALUES ('14', 'uploadDelete', '第三方附件删除钩子', '2', '1509174020', '', '', '1', '1');

-- ----------------------------
-- Table structure for `yzn_menu`
-- ----------------------------
DROP TABLE IF EXISTS `yzn_menu`;
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
) ENGINE=MyISAM AUTO_INCREMENT=56 DEFAULT CHARSET=utf8 COMMENT='后台菜单表';

-- ----------------------------
-- Records of yzn_menu
-- ----------------------------
INSERT INTO `yzn_menu` VALUES ('1', '首页', '', '0', 'admin', 'index', 'index', '', '0', '', '0', '0');
INSERT INTO `yzn_menu` VALUES ('2', '控制面板', '', '0', 'admin', 'main', 'index', '', '0', '', '0', '1');
INSERT INTO `yzn_menu` VALUES ('3', '设置', 'icon-setup', '0', 'admin', 'setting', 'index', '', '1', '', '0', '0');
INSERT INTO `yzn_menu` VALUES ('4', '模块', 'icon-supply', '0', 'admin', 'module', 'index1', '', '1', '', '0', '9');
INSERT INTO `yzn_menu` VALUES ('5', '扩展', 'icon-tools', '0', 'addons', 'addons', 'index1', '', '1', '', '0', '10');
INSERT INTO `yzn_menu` VALUES ('10', '系统配置', 'icon-zidongxiufu', '3', 'admin', 'config', 'index1', '', '1', '', '0', '0');
INSERT INTO `yzn_menu` VALUES ('11', '配置管理', 'icon-apartment', '10', 'admin', 'config', 'index', '', '1', '', '0', '0');
INSERT INTO `yzn_menu` VALUES ('12', '删除日志', '', '20', 'admin', 'adminlog', 'deletelog', '', '1', '', '0', '0');
INSERT INTO `yzn_menu` VALUES ('13', '网站设置', 'icon-setup', '10', 'admin', 'config', 'setting', '', '1', '', '0', '0');
INSERT INTO `yzn_menu` VALUES ('14', '菜单管理', 'icon-other', '10', 'admin', 'menu', 'index', '', '1', '', '0', '0');
INSERT INTO `yzn_menu` VALUES ('15', '权限管理', 'icon-guanliyuan', '3', 'admin', 'manager', 'index1', '', '1', '', '0', '0');
INSERT INTO `yzn_menu` VALUES ('16', '管理员管理', 'icon-guanliyuan', '15', 'admin', 'manager', 'index', '', '1', '', '0', '0');
INSERT INTO `yzn_menu` VALUES ('17', '角色管理', 'icon-group', '15', 'admin', 'authManager', 'index', '', '1', '', '0', '0');
INSERT INTO `yzn_menu` VALUES ('18', '添加管理员', '', '16', 'admin', 'manager', 'add', '', '1', '', '0', '0');
INSERT INTO `yzn_menu` VALUES ('19', '编辑管理员', '', '16', 'admin', 'manager', 'edit', '', '1', '', '0', '0');
INSERT INTO `yzn_menu` VALUES ('20', '管理日志', 'icon-rizhi', '15', 'admin', 'adminlog', 'index', '', '1', '', '0', '0');
INSERT INTO `yzn_menu` VALUES ('21', '删除管理员', '', '16', 'admin', 'manager', 'del', '', '1', '', '0', '0');
INSERT INTO `yzn_menu` VALUES ('22', '添加角色', '', '17', 'admin', 'authManager', 'createGroup', '', '1', '', '0', '0');
INSERT INTO `yzn_menu` VALUES ('23', '附件管理', 'icon-accessory', '10', 'attachment', 'attachments', 'index', '', '1', '', '0', '1');
INSERT INTO `yzn_menu` VALUES ('24', '新增配置', '', '11', 'admin', 'config', 'add', '', '1', '', '0', '1');
INSERT INTO `yzn_menu` VALUES ('25', '编辑配置', '', '11', 'admin', 'config', 'edit', '', '1', '', '0', '2');
INSERT INTO `yzn_menu` VALUES ('26', '删除配置', '', '11', 'admin', 'config', 'del', '', '1', '', '0', '3');
INSERT INTO `yzn_menu` VALUES ('27', '新增菜单', '', '14', 'admin', 'menu', 'add', '', '1', '', '0', '0');
INSERT INTO `yzn_menu` VALUES ('28', '编辑菜单', '', '14', 'admin', 'menu', 'edit', '', '1', '', '0', '0');
INSERT INTO `yzn_menu` VALUES ('29', '删除菜单', '', '14', 'admin', 'menu', 'delete', '', '1', '', '0', '0');
INSERT INTO `yzn_menu` VALUES ('30', '附件上传', '', '23', 'attachment', 'attachments', 'upload', '', '1', '', '0', '0');
INSERT INTO `yzn_menu` VALUES ('31', '附件删除', '', '23', 'attachment', 'attachments', 'delete', '', '1', '', '0', '0');
INSERT INTO `yzn_menu` VALUES ('32', '编辑器附件', '', '23', 'attachment', 'ueditor', 'run', '', '0', '', '0', '0');
INSERT INTO `yzn_menu` VALUES ('33', '图片列表', '', '23', 'attachment', 'attachments', 'showFileLis', '', '0', '', '0', '0');
INSERT INTO `yzn_menu` VALUES ('34', '图片本地化', '', '23', 'attachment', 'attachments', 'getUrlFile', '', '0', '', '0', '0');
INSERT INTO `yzn_menu` VALUES ('38', '插件扩展', 'icon-tools', '5', 'addons', 'addons', 'index2', '', '1', '', '0', '0');
INSERT INTO `yzn_menu` VALUES ('39', '插件管理', 'icon-plugins-', '38', 'addons', 'addons', 'index', '', '1', '', '0', '0');
INSERT INTO `yzn_menu` VALUES ('40', '行为管理', 'icon-hangweifenxi', '38', 'addons', 'addons', 'hooks', '', '1', '', '0', '0');
INSERT INTO `yzn_menu` VALUES ('41', '插件后台列表', 'icon-liebiaosousuo', '5', 'addons', 'addons', 'addonadmin', '', '0', '', '0', '0');
INSERT INTO `yzn_menu` VALUES ('42', '在线云平台', 'icon-cloud-download', '4', 'admin', 'cloud', 'index', '', '1', '', '0', '0');
INSERT INTO `yzn_menu` VALUES ('43', '本地模块', 'icon-supply', '4', 'admin', 'module', 'index2', '', '1', '', '0', '0');
INSERT INTO `yzn_menu` VALUES ('44', '模块管理', 'icon-mokuaishezhi', '43', 'admin', 'module', 'index', '', '1', '', '0', '0');
INSERT INTO `yzn_menu` VALUES ('45', '模块后台列表', 'icon-liebiaosousuo', '4', 'admin', 'module', 'index', '', '1', '', '0', '0');
INSERT INTO `yzn_menu` VALUES ('46', '模块商店', 'icon-caigou-xianxing', '42', 'admin', 'moduleshop', 'index', '', '1', '', '0', '0');
INSERT INTO `yzn_menu` VALUES ('47', '插件商店', 'icon-caigou-xianxing', '42', 'admin', 'addonshop', 'index', '', '1', '', '0', '0');
INSERT INTO `yzn_menu` VALUES ('48', '编辑角色', '', '17', 'admin', 'authManager', 'editGroup', '', '1', '', '0', '0');
INSERT INTO `yzn_menu` VALUES ('49', '删除角色', '', '17', 'admin', 'authManager', 'deleteGroup', '', '1', '', '0', '0');
INSERT INTO `yzn_menu` VALUES ('50', '访问授权', '', '17', 'admin', 'authManager', 'access', '', '1', '', '0', '0');
INSERT INTO `yzn_menu` VALUES ('51', '角色授权', '', '17', 'admin', 'authManager', 'writeGroup', '', '1', '', '0', '0');
INSERT INTO `yzn_menu` VALUES ('52', '模块安装', '', '44', 'admin', 'module', 'install', '', '1', '', '0', '0');
INSERT INTO `yzn_menu` VALUES ('53', '模块卸载', '', '44', 'admin', 'module', 'uninstall', '', '1', '', '0', '0');
INSERT INTO `yzn_menu` VALUES ('54', '本地安装', '', '44', 'admin', 'module', 'local', '', '1', '', '0', '0');
INSERT INTO `yzn_menu` VALUES ('55', '缓存更新', '', '0', 'admin', 'index', 'cache', '', '0', '', '0', '0');

-- ----------------------------
-- Table structure for `yzn_module`
-- ----------------------------
DROP TABLE IF EXISTS `yzn_module`;
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

-- ----------------------------
-- Table structure for `yzn_model`
-- ----------------------------
DROP TABLE IF EXISTS `yzn_model`;
CREATE TABLE `yzn_model` (
  `id` smallint(5) unsigned NOT NULL AUTO_INCREMENT,
  `module` varchar(15) NOT NULL  DEFAULT '' COMMENT '所属模块',
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
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='模型列表';

-- ----------------------------
-- Table structure for `yzn_model_field`
-- ----------------------------
DROP TABLE IF EXISTS `yzn_model_field`;
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
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='模型字段列表';

-- ----------------------------
-- Table structure for `yzn_terms`
-- ----------------------------
DROP TABLE IF EXISTS `yzn_terms`;
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

-- ----------------------------
-- Table structure for `yzn_sms`
-- ----------------------------
DROP TABLE IF EXISTS `yzn_sms`;
CREATE TABLE `yzn_sms` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT 'ID',
  `event` varchar(30) NOT NULL DEFAULT '' COMMENT '事件',
  `mobile` varchar(20) NOT NULL DEFAULT '' COMMENT '手机号',
  `code` varchar(10) NOT NULL DEFAULT '' COMMENT '验证码',
  `times` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '验证次数',
  `ip` bigint(20) NOT NULL DEFAULT '0' COMMENT 'IP',
  `create_time` int(10) unsigned DEFAULT '0' COMMENT '创建时间',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COMMENT='短信验证码表';

-- ----------------------------
-- Table structure for `yzn_ems`
-- ----------------------------
DROP TABLE IF EXISTS `yzn_ems`;
CREATE TABLE `yzn_ems` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT 'ID',
  `event` varchar(30) NOT NULL DEFAULT '' COMMENT '事件',
  `email` varchar(100) NOT NULL DEFAULT '' COMMENT '邮箱',
  `code` varchar(10) NOT NULL DEFAULT '' COMMENT '验证码',
  `times` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '验证次数',
  `ip` bigint(20) NOT NULL DEFAULT '0' COMMENT 'IP',
  `create_time` int(10) unsigned DEFAULT '0' COMMENT '创建时间',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COMMENT='邮箱验证码表';