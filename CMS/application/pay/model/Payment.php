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
// | 支付模块模型
// +----------------------------------------------------------------------
namespace app\pay\model;

use \think\Model;

class Payment extends Model
{
    protected $name = 'pay_payment';

    public function getConfigAttr($value)
    {
        return unserialize($value);
    }

    //支付配置缓存
    public function pay_cache()
    {
        $data = $this->where('status', 1)->select();
        if ($data) {
            $data = $data->toArray();
        } else {
            return;
        }
        $Cache = array();
        foreach ($data as $v) {
            $Cache[$v['name']] = $v;
        }
        cache("Pay_Config", $Cache);
        return $data;
    }

}
