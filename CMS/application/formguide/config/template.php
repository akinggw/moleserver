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
    //模板输出替换
    'tpl_replace_string' => [
        '__STATIC__' => ROOT_URL . (defined('IF_PUBLIC') ? 'static' : 'public/static'),
        '__UPLOAD__' => ROOT_URL . (defined('IF_PUBLIC') ? 'uploads' : 'public/uploads'),

        '__IMG_PATH__' => ROOT_URL . (defined('IF_PUBLIC') ? 'static' : 'public/static') . '/modules/cms/images',
        '__CSS_PATH__' => ROOT_URL . (defined('IF_PUBLIC') ? 'static' : 'public/static') . '/modules/cms/css',
        '__JS_PATH__' => ROOT_URL . (defined('IF_PUBLIC') ? 'static' : 'public/static') . '/modules/cms/js',
        '__HOME_PATH__' => ROOT_URL . (defined('IF_PUBLIC') ? 'static' : 'public/static') . '/modules/cms',
    ],
];
