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

class Group extends Validate
{
    //定义验证规则
    protected $rule = [
        'name|会员组名称' => 'unique:member_group|require|chsDash|length:1,20',
        'point|积分' => 'require|number',
        'starnum|星星数' => 'require|number',

        'allowmessage|最大短消息数' => 'number',
        'allowpostnum|日最大投稿数' => 'number',

    ];
}
