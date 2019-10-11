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
namespace app\cms\validate;

use think\Validate;

class ModelField extends Validate
{
    //定义验证规则
    protected $rule = [
        'name' => 'require|regex:/^[a-zA-Z][A-Za-z0-9]+$/',
        'title' => 'require|chsAlpha',
        'type' => 'require|alphaDash',
        'setting.define' => 'require',
        'setting.value' => 'chsAlphaNum',
        'ifsystem' => 'in:0,1',
        'ifrequire' => 'in:0,1',
        'ifsearch' => 'in:0,1',
        'status' => 'in:0,1',
    ];
    //定义验证提示
    protected $message = [
        'name.require' => '字段名称不得为空',
        'name.regex' => '字段名称只能为字母和数字，并且仅能字母开头',
        'title.require' => '字段标题不得为空',
        'title.chsAlpha' => '字段标题只能为只能是汉字和字母',
        'type.require' => '字段类型不得为空',
        'type.alphaDash' => '字段类型格式错误',
        'setting.define.require' => '字段定义不得为空',
        'setting.value.chsAlphaNum' => '字段默认值格式错误',
        'ifsystem.in' => '主表字段格式错误',
        'ifrequire.in' => '是否必填格式错误',
        'ifsearch.in' => '是否显示搜索格式错误',
        'status.in' => '字段状态格式错误',
    ];
}
