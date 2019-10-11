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
// | 消费模块模型
// +----------------------------------------------------------------------
namespace app\pay\model;

use think\Db;
use \think\Model;

class Spend extends Model
{
    protected $name = 'pay_spend';
    // 定义时间戳字段名
    protected $autoWriteTimestamp = true;
    protected $createTime = 'addtime';
    protected $updateTime = false;

    public function getIpAttr($value)
    {
        return long2ip($value);
    }

    /**
     * 按用户名、时间、标识查询是否有消费记录
     * @param integer $userid      用户名
     * @param integer $time        时间。  从指定时间到现在的时间范围内。
     * @param string $flag   标识
     */
    public static function spend_time($userid, $time, $flag)
    {
        return self::where(array('uid' => $userid, 'remarks' => $flag))->whereTime('addtime', '-' . $time . ' hours')->order('id DESC')->find();
    }

    /**
     * 消费积分/金钱记录
     */
    public function _spend($type, $money, $uid = '', $username = '', $msg = '', $remarks = '')
    {
        $data = array();
        $data['type'] = isset($type) && intval($type) ? intval($type) : 0;
        $data['creat_at'] = date('YmdHis') . mt_rand(1000, 9999);
        $data['money'] = isset($money) && floatval($money) ? floatval($money) : 0;
        $data['uid'] = isset($uid) && intval($uid) ? intval($uid) : 0;
        $data['username'] = isset($username) ? trim($username) : '';
        $data['money'] = isset($money) && floatval($money) ? floatval($money) : 0;
        $data['msg'] = isset($msg) ? trim($msg) : '';
        $data['remarks'] = isset($remarks) ? trim($remarks) : '';
        $data['ip'] = request()->ip(1);
        if (self::create($data)) {
            if ($data['type'] == 1) {
                //金钱方式消费
                Db::name('member')->where(['id' => $data['uid'], 'username' => $data['username']])->setDec('amount', $data['money']);
            } else {
                //积分方式消费
                Db::name('member')->where(['id' => $data['uid'], 'username' => $data['username']])->setDec('point', $data['money']);
            }
            return true;
        }
        return false;
    }

}
