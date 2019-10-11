<?php
// +----------------------------------------------------------------------
// | Yzncms [ 御宅男工作室 ]
// +----------------------------------------------------------------------
// | Copyright (c) 2007 http://yzncms.com All rights reserved.
// +----------------------------------------------------------------------
// | Licensed ( http://www.apache.org/licenses/LICENSE-2.0 )
// +----------------------------------------------------------------------
// | Author: 御宅男 <530765310@qq.com>
// +----------------------------------------------------------------------
namespace app\admin\validate;

use \think\Validate;

/**
 * 用户验证器
 */
class Menu extends Validate
{
    //定义验证规则
    protected $rule = [
        'parentid|上级菜单' => 'require|number',
        'title|名称' => 'require|chsAlphaNum',
        'app|模块' => 'require|alpha',
        'controller|控制器' => 'require|alpha',
        'action|方法' => 'require|alpha',
    ];

    //定义验证提示
    protected $message = [
    ];

    //定义验证场景
    protected $scene = [
        'add' => ['parentid', 'title', 'app', 'controller', 'action'],
        'edit' => ['parentid', 'title', 'app', 'controller', 'action'],
    ];
}
