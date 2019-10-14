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
// | 模型验证
// +----------------------------------------------------------------------
namespace app\member\validate;

use think\Validate;

class Member extends Validate
{
    //定义验证规则
    protected $rule = [
        'username|用户名' => 'unique:member|require|alphaDash|length:3,20',
        'nickname|昵称' => 'unique:member|chsDash|length:3,20',
        'password|密码' => 'require|length:3,20|confirm',
        'email|邮箱' => 'unique:member|require|email',
        'groupid|会员组' => 'require|number',
    ];

    public function sceneEdit()
    {
        return $this->remove('password', 'require');
    }

    public function sceneRegister()
    {
        return $this->remove('groupid', 'require');
    }

}
