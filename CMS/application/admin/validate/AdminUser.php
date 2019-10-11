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
// | 登录验证
// +----------------------------------------------------------------------
namespace app\admin\validate;

use think\Validate;

class AdminUser extends Validate
{

    //定义验证规则
    protected $rule = [
        'username|用户名' => 'unique:admin|require|alphaDash|length:3,20',
        'password|密码' => 'require|length:3,20|confirm',
        'email|邮箱' => 'email',
        'roleid|权限组' => 'require',
    ];

    // 登录验证场景定义
    public function sceneUpdate()
    {
        return $this->only(['username', 'password', 'email', 'roleid'])
            ->remove('password', 'require');
    }

    //定义验证场景
    protected $scene = [
        'insert' => ['username', 'password', 'email', 'roleid'],

    ];

}
