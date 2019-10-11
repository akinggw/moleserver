<?php
// +----------------------------------------------------------------------
// | ThinkPHP [ WE CAN DO IT JUST THINK ]
// +----------------------------------------------------------------------
// | Copyright (c) 2006~2018 http://thinkphp.cn All rights reserved.
// +----------------------------------------------------------------------
// | Licensed ( http://www.apache.org/licenses/LICENSE-2.0 )
// +----------------------------------------------------------------------
// | Author: liu21st <liu21st@gmail.com>
// +----------------------------------------------------------------------

// +----------------------------------------------------------------------
// | 模板设置
// +----------------------------------------------------------------------

return [
    // 模板引擎类型 支持 php think 支持扩展
    'type' => 'Think',
    // 模板路径
    'view_path' => '',
    // 模板后缀
    'view_suffix' => 'html',
    //模板输出替换
    'tpl_replace_string' => [
        '__STATIC__' => ROOT_URL . (defined('IF_PUBLIC') ? 'static' : 'public/static'),
        '__UPLOAD__' => ROOT_URL . (defined('IF_PUBLIC') ? 'uploads' : 'public/uploads'),
        //cms模块
        '__IMG_PATH__' => ROOT_URL . (defined('IF_PUBLIC') ? 'static' : 'public/static') . '/modules/cms/images',
        '__CSS_PATH__' => ROOT_URL . (defined('IF_PUBLIC') ? 'static' : 'public/static') . '/modules/cms/css',
        '__JS_PATH__' => ROOT_URL . (defined('IF_PUBLIC') ? 'static' : 'public/static') . '/modules/cms/js',
        '__HOME_PATH__' => ROOT_URL . (defined('IF_PUBLIC') ? 'static' : 'public/static') . '/modules/cms',
    ],
    // 模板文件名分隔符
    'view_depr' => DIRECTORY_SEPARATOR,
    // 模板引擎普通标签开始标记
    'tpl_begin' => '{',
    // 模板引擎普通标签结束标记
    'tpl_end' => '}',
    // 标签库标签开始标记
    'taglib_begin' => '{',
    // 标签库标签结束标记
    'taglib_end' => '}',
    'taglib_build_in' => 'cx,app\common\taglib\Yzn',
];
