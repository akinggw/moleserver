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
// | 会员首页管理
// +----------------------------------------------------------------------
namespace app\member\controller;

use app\common\model\Ems as Ems_Model;
use app\common\model\Sms as Sms_Model;
use app\member\model\Member as Member_Model;
use app\member\service\User;
use think\facade\Cookie;
use think\facade\Validate;

class Index extends MemberBase
{
    protected $noNeedLogin = ['login', 'register', 'logout', 'forget'];

    //初始化
    protected function initialize()
    {
        parent::initialize();
        $this->Member_Model = new Member_Model;
    }

    //会员中心首页
    public function index()
    {
        return $this->fetch('/index');

    }

    //登录页面
    public function login()
    {
        $cookie_url = $_REQUEST['forward'] ? $_REQUEST['forward'] : Cookie::get('__forward__');
        Cookie::set("forward", null);
        if (!empty($this->userid)) {
            $this->success("您已经是登陆状态！", $forward ? $forward : url("Index/index"));
        }
        if ($this->request->isPost()) {
            //登录验证
            $username = $this->request->param('username');
            $password = $this->request->param('password');
            $verify = $this->request->param('verify');
            $cookieTime = $this->request->param('cookieTime', 0);
            //验证码
            if (empty($verify) && $this->memberConfig['openverification']) {
                $this->error('验证码错误！');
            }
            if ($this->memberConfig['openverification'] && !captcha_check($verify)) {
                $this->error('验证码错误！');
            }
            $userid = $this->Member_Model->loginLocal($username, $password, $cookieTime ? 86400 * 180 : 86400);
            if ($userid > 0) {
                if (!$cookie_url) {
                    $cookie_url = url('index');
                }
                $this->success('登录成功！', $cookie_url);
            } else {
                //登陆失败
                $this->error('账号或者密码错误！');
            }

        } else {
            $this->assign('forward', $forward);
            return $this->fetch('/login');
        }
    }

    //注册页面
    public function register()
    {
        if (empty($this->memberConfig['allowregister'])) {
            $this->error("系统不允许新会员注册！");
        }
        $forward = $_REQUEST['forward'] ?: cookie("forward");
        cookie("forward", null);
        if ($this->userid) {
            $this->success("您已经是登陆状态，无需注册！", $forward ? $forward : url("index"));
        }
        if ($this->request->isPost()) {
            $post = $data = $this->request->post();
            $result = $this->validate($data, 'member.register');
            if (true !== $result) {
                return $this->error($result);
            }
            $userid = $this->Member_Model->userRegister($data['username'], $data['password'], $data['email']);
            if ($userid > 0) {
                unset($data['username'], $data['password'], $data['email']);
                //==============注册设置处理==============
                //新注册用户积分
                $data['point'] = $this->memberConfig['defualtpoint'] ? $this->memberConfig['defualtpoint'] : 0;
                //新会员注册默认赠送资金
                $data['amount'] = $this->memberConfig['defualtamount'] ? $this->memberConfig['defualtamount'] : 0;
                //新会员注册需要邮件验证
                if ($this->memberConfig['enablemailcheck']) {
                    $data['groupid'] = 7;
                    $data['status'] = 0;
                } else {
                    //新会员注册需要管理员审核
                    if ($this->memberConfig['registerverify']) {
                        $data['status'] = 0;
                    } else {
                        $data['status'] = 1;
                    }
                    //计算用户组
                    $data['groupid'] = $this->Member_Model->get_usergroup_bypoint($data['point']);
                }
                //==============注册设置处理==============

                if (false !== $this->Member_Model->save($data, ['id' => $userid])) {
                    //注册登陆状态
                    $this->Member_Model->loginLocal($post['username'], $post['password']);
                    $this->success('会员注册成功！', url('index'));
                } else {
                    //删除
                    $this->Member_Model->userDelete($userid);
                    $this->error("会员注册失败！");
                }
            } else {
                $this->error($this->Member_Model->getError() ?: '帐号注册失败！');
            }
        } else {
            return $this->fetch('/register');
        }
    }

    /**
     * 个人资料
     */
    public function profile()
    {
        if ($this->request->isPost()) {
            $data = $this->request->post();
            //验证数据合法性
            $rule = [
                'nickname|昵称' => 'chsDash|length:3,20',
                'avatar|头像' => 'number',
            ];
            $result = $this->validate($data, $rule);
            if (true !== $result) {
                $this->error($result);
            }
            $userinfo = $this->Member_Model->getLocalUser($this->userid);
            if (empty($userinfo)) {
                $this->error('该会员不存在！');
            }
            if (!empty($data)) {
                //暂时只允许昵称，头像修改
                $this->Member_Model->allowField(['nickname', 'avatar'])->save($data, ["id" => $this->userid]);
            }
            $this->success("基本信息修改成功！");
        } else {
            return $this->fetch('/profile');
        }
    }

    /**
     * 更改密码
     */
    public function changepwd()
    {
        if ($this->request->isPost()) {
            $oldPassword = $this->request->post("oldpassword");
            $newPassword = $this->request->post("newpassword");
            $renewPassword = $this->request->post("renewpassword");
            // 验证数据
            $data = [
                'oldpassword' => $oldPassword,
                'newpassword' => $newPassword,
                'renewpassword' => $renewPassword,
            ];
            $rule = [
                'oldpassword|旧密码' => 'require|length:6,30',
                'newpassword|新密码' => 'require|length:6,30',
                'renewpassword|确认密码' => 'require|length:6,30|confirm:newpassword',
            ];
            $result = $this->validate($data, $rule);
            if (true !== $result) {
                $this->error($result);
            }
            $res = $this->Member_Model->userEdit($this->userinfo['username'], $oldPassword, $newPassword);
            if (!$res) {
                $this->error($this->Member_Model->getError());
            }
            $this->success('修改成功！');
            //注销当前登陆
            $this->logout();
        }
    }

    /**
     * 修改邮箱
     */
    public function changeemail()
    {
        if ($this->request->isPost()) {
            $email = $this->request->post('email');
            $captcha = $this->request->request('captcha');
            if (!$email || !$captcha) {
                $this->error('参数不得为空！');
            }
            if (!Validate::is($email, "email")) {
                $this->error('邮箱格式不正确！');
            }
            if ($this->Member_Model->where('email', $email)->where('id', '<>', $this->userid)->find()) {
                $this->error('邮箱已占用');
            }
            $Ems_Model = new Ems_Model();
            $result = $Ems_Model->check($email, $captcha, 'changeemail');
            if (!$result) {
                $this->error('验证码错误！');
            }
            //只修改邮箱
            $this->Member_Model->allowField(['ischeck_email', 'email'])->save(['email' => $email, 'ischeck_email' => 1], ['id' => 1]);
            $Ems_Model->flush($email, 'changeemail');
            $this->success();
        } else {
            return $this->fetch('/changeemail');
        }

    }

    /**
     * 修改手机号
     */
    public function changemobile()
    {
        if ($this->request->isPost()) {
            $mobile = $this->request->request('mobile');
            $captcha = $this->request->request('captcha');
            if (!$mobile || !$captcha) {
                $this->error('参数不得为空！');
            }
            if (!Validate::isMobile($mobile)) {
                $this->error('手机号格式不正确！');
            }
            if ($this->Member_Model->where('mobile', $mobile)->where('id', '<>', $this->userid)->find()) {
                $this->error('手机号已占用');
            }
            $Sms_Model = new Sms_Model();
            $result = $Sms_Model->check($mobile, $captcha, 'changemobile');
            if (!$result) {
                $this->error('验证码错误！');
            }
            //只修改手机号
            $this->Member_Model->allowField(['ischeck_mobile', 'mobile'])->save(['mobile' => $mobile, 'ischeck_mobile' => 1], ['id' => 1]);
            $Sms_Model->flush($mobile, 'changemobile');
            $this->success();
        } else {
            return $this->fetch('/changemobile');
        }
    }

    /**
     * 激活邮箱
     */
    public function actemail()
    {
        if ($this->request->isPost()) {
            $captcha = $this->request->request('captcha');
            if (!$captcha) {
                $this->error('参数不得为空！');
            }
            $Ems_Model = new Ems_Model();
            $result = $Ems_Model->check($this->userinfo['email'], $captcha, 'actemail');
            if (!$result) {
                $this->error('验证码错误！');
            }
            //只修改邮箱
            $this->Member_Model->save(['ischeck_email' => 1], ['id' => 1]);
            $Ems_Model->flush($this->userinfo['email'], 'actemail');
            $this->success('激活成功！');
        } else {
            return $this->fetch('/actemail');
        }
    }

    /**
     * 激活手机号
     */
    public function actmobile()
    {
        if ($this->request->isPost()) {
            $captcha = $this->request->request('captcha');
            if (!$captcha) {
                $this->error('参数不得为空！');
            }
            $Sms_Model = new Sms_Model();
            $result = $Sms_Model->check($this->userinfo['mobile'], $captcha, 'actmobile');
            if (!$result) {
                $this->error('验证码错误！');
            }
            //只修改手机号
            $this->Member_Model->save(['ischeck_mobile' => 1], ['id' => 1]);
            $Sms_Model->flush($this->userinfo['mobile'], 'actmobile');
            $this->success('激活成功！');
        } else {
            return $this->fetch('/actmobile');
        }
    }

    /**
     *忘记密码
     */
    public function forget()
    {
        if ($this->request->isPost()) {
            $type = $this->request->request("type");
            $mobile = $this->request->request("mobile");
            $email = $this->request->request("email");
            $newpassword = $this->request->request("newpassword");
            $captcha = $this->request->request("captcha");

            // 验证数据
            $data = [
                'mobile' => $mobile,
                'email' => $email,
                'captcha' => $captcha,
                'newpassword' => $newpassword,
            ];
            $rule = [
                'mobile|手机号' => 'require|mobile',
                'email|邮箱' => 'require|email',
                'captcha|验证码' => 'require|number|length:4',
                'newpassword|新密码' => 'require|length:6,30',
            ];
            if ($type == "mobile") {
                unset($rule['email|邮箱']);
            } else {
                unset($rule['mobile|手机号']);
            }
            $result = $this->validate($data, $rule);
            if (true !== $result) {
                $this->error($result);
            }

            if ($type == 'mobile') {
                $user = $this->Member_Model->where('mobile', $mobile)->find();
                if (!$user) {
                    $this->error('用户不存在！');
                }
                $Sms_Model = new Sms_Model();
                $result = $Sms_Model->check($mobile, $captcha, 'resetpwd');
                if (!$result) {
                    $this->error('验证码错误！');
                }
            } elseif ($type == 'email') {
                $user = $this->Member_Model->where('email', $email)->find();
                if (!$user) {
                    $this->error('用户不存在！');
                }
                $Ems_Model = new Ems_Model();
                $result = $Ems_Model->check($email, $captcha, 'resetpwd');
                if (!$result) {
                    $this->error('验证码错误！');
                }
            } else {
                $this->error('类型错误！');
            }
            $res = $this->Member_Model->userEdit($user['username'], '', $newpassword, '', 1);
            if (!$res) {
                $this->error($this->Member_Model->getError());
            }
            $this->success('重置成功！');

        } else {
            return $this->fetch('/forget');
        }

    }

    //手动退出登录
    public function logout()
    {
        if (User::instance()->logout()) {
            //手动登出时，清空forward
            Cookie::set("forward", null);
            $this->success('注销成功！', url("index/login"));
        }
    }

}
