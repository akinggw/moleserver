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
// | 手机短信接口
// +----------------------------------------------------------------------
namespace app\api\controller;

use app\common\controller\Base;
use app\common\model\Sms as Sms_Model;
use app\member\model\Member;
use think\facade\Validate;

class Sms extends Base
{
    /**
     * 初始化
     */
    protected function initialize()
    {
        $this->Sms_Model = new Sms_Model();
        parent::initialize();
    }

    /**
     * 发送验证码
     */
    public function send()
    {
        $mobile = $this->request->request("mobile");
        $event = $this->request->request("event");
        $event = $event ? $event : 'register';

        if (!$mobile || !Validate::isMobile($mobile)) {
            $this->error('手机号不正确');
        }
        $last = $this->Sms_Model->get($mobile, $event);
        if ($last && time() - $last['create_time'] < 60) {
            $this->error('发送频繁');
        }
        $ipSendTotal = $this->Sms_Model->where(['ip' => $this->request->ip(1)])->whereTime('create_time', '-1 hours')->count();
        if ($ipSendTotal >= 5) {
            $this->error('发送频繁');
        }
        if ($event) {
            $userinfo = Member::getByMobile($mobile);
            if ($event == 'register' && $userinfo) {
                $this->error('已被注册');
            } elseif (in_array($event, ['changemobile']) && $userinfo) {
                $this->error('已被占用');
            } elseif (in_array($event, ['changepwd', 'resetpwd', 'actmobile']) && !$userinfo) {
                $this->error('未注册');
            }
        }
        $ret = $this->Sms_Model->send($mobile, null, $event);
        if ($ret) {
            $this->success('发送成功');
        } else {
            $this->error('发送失败');
        }

    }

    /**
     * 检测验证码
     */
    public function check()
    {

    }

}
