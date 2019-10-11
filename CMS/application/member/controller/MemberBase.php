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
// | 前台会员管理
// +----------------------------------------------------------------------
namespace app\member\controller;

use app\common\controller\Homebase;
use app\member\service\User;
use think\facade\Config;

class MemberBase extends HomeBase
{
    //用户id
    protected $userid = 0;
    //用户信息
    protected $userinfo = array();
    //会员模型相关配置
    protected $memberConfig = array();
    //会员组缓存
    protected $memberGroup = array();
    //无需登录的方法,同时也就不需要鉴权了
    protected $noNeedLogin = [];
    //初始化
    protected function initialize()
    {
        parent::initialize();
        $this->memberConfig = cache("Member_Config");
        $this->memberGroup = cache("Member_Group");
        $this->check_member();
        //============全局模板变量==============
        $this->assign("Member_group", $this->memberGroup);
        $this->assign("Member_config", $this->memberConfig);
    }

    /**
     * 检测用户是否已经登陆
     */
    final public function check_member()
    {
        $this->userid = User::instance()->id;
        if (substr($this->request->module(), 0, 7) == 'public_' || in_array(strtolower($this->request->action()), $this->noNeedLogin)) {
            //所有以public_开头的方法都无需检测是否登陆
            return true;
        }
        //该类方法不需要验证是否登陆
        /*if ($this->request->module() == 'member' && $this->request->controller() == 'Index' && in_array($this->request->action(), array('login', 'register', 'logout', 'forget'))) {
        return true;
        };*/
        if ($this->userid) {
            //  获取用户信息
            $this->userinfo = User::instance()->getInfo();
            //  判断用户是否被锁定
            if ($this->userinfo['status'] !== 1) {
                $this->error("您的帐号已经被锁定！", url('/'));
            }
            $this->userinfo['groupname'] = $this->memberGroup[$this->userinfo['groupid']]['name'];
            $this->assign("userinfo", $this->userinfo);
            return true;
        } else {
            // 还没登录 跳转到登录页面
            $this->redirect('member/index/login');
        }
    }
}
