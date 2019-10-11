<?php
// +----------------------------------------------------------------------
// | Yzncms [ 御宅男工作室 ]
// +----------------------------------------------------------------------
// | Copyright (c) 2018 http://yzncms.com All rights reserved.
// +----------------------------------------------------------------------
// | Licensed ( http://www.apache.org/licenses/LICENSE-2.0 )
// +----------------------------------------------------------------------
// | Author: 御宅男 <530765310@qq.com>
// +----------------------------------------------------------------------

// +----------------------------------------------------------------------
// | cms模块信息文件
// +----------------------------------------------------------------------
return array(
    //模块名称[必填]
    'name' => 'cms模块',
    //模块简介[选填]
    'introduce' => '这是一个功能强大的内容管理模块！',
    //模块作者[选填]
    'author' => 'yzncms',
    //作者地址[选填]
    'authorsite' => 'http://www.yzncms.com',
    //作者邮箱[选填]
    'authoremail' => '530765310@qq.com',
    //版本号，请不要带除数字外的其他字符[必填]
    'version' => '1.0.0',
    //适配最低yzncms版本[必填]
    'adaptation' => '1.0.0',
    //签名[必填]
    'sign' => 'b19cc279ed484c13c96c2f7142e2f437',
    //依赖模块
    'need_module' => [],
    //依赖插件
    'need_plugin' => [],
    //行为注册
    'tags' => [
        'userSidenavAfter' => [
            'type' => 1,
            'description' => '会员左侧导航',
        ],
    ],
    //缓存，格式：缓存key=>array('module','model','action')
    'cache' => [
        'Category' => [
            'name' => '栏目索引',
            'model' => 'Category',
            'action' => 'category_cache',
        ],
        'Cms_Config' => [
            'name' => 'CMS配置',
            'model' => 'Cms',
            'action' => 'cms_cache',
        ],
    ],
    // 数据表，不要加表前缀[有数据库表时必填]
    'tables' => [
        'category',
        'category_priv',
        'page',
        'tags',
        'tags_content',
    ],
);
