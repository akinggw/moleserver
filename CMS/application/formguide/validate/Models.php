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
namespace app\formguide\validate;

use think\Validate;

class Models extends Validate
{
    //定义验证规则
    protected $rule = [
        'name|表单名称' => 'require|chs|max:30|unique:model',
        'tablename|表单键名' => 'require|lower|max:20|alpha|unique:model',
    ];

    // edit 验证场景定义
    public function sceneEdit()
    {
        return $this->only(['name']);
    }
}
