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
// | 会员模块信息文件
// +----------------------------------------------------------------------
return array(
    //模块名称[必填]
    'name' => '会员模块',
    //模块简介[选填]
    'introduce' => '系统必备的会员基础模块！',
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
    'sign' => 'fcfe4d97f35d1f30df5d6018a84f74ba',
    //依赖模块
    'need_module' => [],
    //依赖插件
    'need_plugin' => [],
    //行为注册
    'tags' => [
        'contentDeleteEnd' => [
            'type' => 2,
            'description' => '内容删除后调用',
        ],
        'contentEditEnd' => [
            'type' => 2,
            'description' => '内容编辑后调用',
        ],
    ],
    //缓存，格式：缓存key=>array('module','model','action')
    'cache' => [
        'Member_Config' => [
            'name' => '会员配置',
            'model' => 'Member',
            'action' => 'member_cache',
        ],
        'Member_Group' => [
            'name' => '会员组',
            'model' => 'MemberGroup',
            'action' => 'membergroup_cache',
        ],
    ],
    // 数据表，不要加表前缀[有数据库表时必填]
    'tables' => [
        'member',
        'member_group',
        'member_content',

    ],
);
