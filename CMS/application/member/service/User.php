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
// |  前台用户服务
// +----------------------------------------------------------------------
namespace app\member\service;

use think\facade\Session;

class User
{
    protected static $instance = null;
    //当前登录会员详细信息
    protected static $userInfo = array();
    //错误信息
    protected $error = null;

    /**
     * 获取示例
     * @param array $options 实例配置
     * @return static
     */
    public static function instance($options = [])
    {
        if (is_null(self::$instance)) {
            self::$instance = new self($options);
        }

        return self::$instance;
    }

    /**
     * 魔术方法
     * @param type $name
     * @return null
     */
    public function __get($name)
    {
        //从缓存中获取
        if (isset(self::$userInfo[$name])) {
            return self::$userInfo[$name];
        } else {
            $userInfo = $this->getInfo();
            if (!empty($userInfo)) {
                return $userInfo[$name];
            }
            return null;
        }
    }

    /**
     * 获取当前登录用户资料
     * @return array
     */
    public function getInfo()
    {
        if (empty(self::$userInfo)) {
            self::$userInfo = model('member/Member')->getLocalUser($this->isLogin());
        }
        return !empty(self::$userInfo) ? self::$userInfo : false;
    }

    /**
     * 检验用户是否已经登陆
     * @return boolean 失败返回false，成功返回当前登陆用户基本信息
     */
    public function isLogin()
    {
        $user = Session::get('user_auth');
        if (empty($user)) {
            return 0;
        } else {
            return Session::get('user_auth_sign') == data_auth_sign($user) ? $user['uid'] : 0;
        }
    }

    /**
     * 注销登录状态
     * @return boolean
     */
    public function logout()
    {
        Session::set('user_auth', null);
        Session::set('user_auth_sign', null);
        return true;
    }

    /**
     * 获取错误信息
     * @return type
     */
    public function getError()
    {
        return $this->error;
    }

}
