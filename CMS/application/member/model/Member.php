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
// | 会员模型
// +----------------------------------------------------------------------
namespace app\member\model;

use \think\Model;

/**
 * 模型
 */
class Member extends Model
{
    protected $autoWriteTimestamp = true;
    protected $updateTime = false;
    protected $createTime = 'reg_time';
    protected $insert = ['status' => 1, 'reg_ip'];
    protected function setRegIpAttr()
    {
        return request()->ip(1);
    }

    /**
     * 获取头像
     * @param   string $value
     * @param   array  $data
     * @return string
     */
    public function getAvatarAttr($value, $data)
    {
        if (!$value) {
            $value = config('public_url') . 'static/modules/member/img/avatar.png';
            return $value;
            //启用首字母头像，请使用
            //$value = letter_avatar($data['nickname']);
        }
        return get_file_path($value);
    }

    /**
     * 会员登录
     * @param type $identifier 用户/UID
     * @param type $password 明文密码，填写表示验证密码
     * @param type $is_remember_me cookie有效期
     * @return boolean
     */
    public function loginLocal($identifier, $password = null, $is_remember_me = 604800)
    {
        $map = [];
        if (is_int($identifier)) {
            $map['id'] = $identifier;
            $identifier = intval($identifier);
        } else {
            $map['username'] = $identifier;
        }
        $userinfo = $this->getLocalUser($identifier);
        if (empty($userinfo)) {
            return false;
        }
        //是否需要进行密码验证
        if (!empty($password)) {
            $password = encrypt_password($password, $userinfo["encrypt"]);
            if ($password != $userinfo['password']) {
                $this->error = '密码错误！';
                return false;
            }
        }
        $this->autoLogin($userinfo);
        return $userinfo['id'];
    }

    /**
     * 获取用户信息
     * @param type $identifier 用户/UID
     * @param type $password 明文密码，填写表示验证密码
     * @return array|boolean
     */
    public function getLocalUser($identifier, $password = null)
    {
        $map = array();
        if (empty($identifier)) {
            $this->error = '参数为空！';
            return false;
        }
        if (is_int($identifier)) {
            $map['id'] = $identifier;
        } else {
            $map['username'] = $identifier;
        }
        $userinfo = self::where($map)->find();
        if (empty($userinfo)) {
            $this->error = '该用户不存在！';
            return false;
        }
        //是否需要进行密码验证
        if (!empty($password)) {
            //对明文密码进行加密
            $password = encrypt_password($password, $userinfo["encrypt"]);
            if ($password != $userinfo['password']) {
                $this->error = '用户密码错误！';
                //密码错误
                return false;
            }
        }
        return $userinfo;
    }

    /**
     * 注册一个新用户
     * @param string $username  用户名
     * @param string $password  密码
     * @param string $email     邮箱
     * @param string $mobile    手机号
     * @param array $extend    扩展参数
     */
    public function userRegister($username, $password, $email = '', $mobile = '', $extend = [])
    {
        $passwordinfo = encrypt_password($password); //对密码进行处理
        $data = array(
            "username" => $username,
            "password" => $passwordinfo['password'],
            "email" => $email,
            "encrypt" => $passwordinfo['encrypt'],
            "amount" => 0,
        );
        $userid = $this->save($data);
        if ($userid) {
            return $this->getAttr('id');
        }
        $this->error = $Member->getError() ?: '注册失败！';
        return false;
    }

    /**
     * 更新用户基本资料
     * @param type $username 用户名
     * @param type $oldpw 旧密码
     * @param type $newpw 新密码，如不修改为空
     * @param type $email Email，如不修改为空
     * @param type $ignoreoldpw 是否忽略旧密码
     * @param type $data 其他信息
     * @return boolean
     */
    public function userEdit($username, $oldpw, $newpw = '', $email = '', $ignoreoldpw = 0, $data = array())
    {
        //验证旧密码是否正确
        if ($ignoreoldpw == 0) {
            $info = self::where(["username" => $username])->find();
            if (encrypt_password($oldpw, $info['encrypt']) != $info['password']) {
                $this->error = '旧密码错误！';
                return false;
            }
        }
        if ($newpw) {
            $passwordinfo = encrypt_password($newpw);
            $data = array(
                "password" => $passwordinfo['password'],
                "encrypt" => $passwordinfo['encrypt'],
            );
        } else {
            unset($data['password'], $data['encrypt']);
        }
        if ($email) {
            $data['email'] = $email;
        } else {
            unset($data['email']);
        }
        if (empty($data)) {
            return true;
        }
        if (self::allowField(true)->save($data, ["username" => $username]) !== false) {
            return true;
        } else {
            $this->error = '用户资料更新失败！';
            return false;
        }
    }

    /**
     * 删除用户
     * @param type $uid 用户UID
     * @return boolean
     */
    public function userDelete($uid)
    {
        //删除本地用户数据开始
        if (self::where(["id" => $uid])->delete() !== false) {
            return true;
        }
        $this->error = '删除会员失败！';
        return false;
    }

    /**
     * 自动登录用户
     */
    public function autoLogin($userInfo)
    {
        /* 更新登录信息 */
        $data = array(
            'uid' => $userInfo['id'],
            'last_login_time' => time(),
            'last_login_ip' => request()->ip(1),
        );
        $this->loginStatus((int) $userInfo['id']);
        /* 记录登录SESSION和COOKIES */
        $auth = [
            'uid' => $userInfo['id'],
            'username' => $userInfo['username'],
            'last_login_time' => $userInfo['last_login_time'],
        ];
        Session('user_auth', $auth);
        Session('user_auth_sign', data_auth_sign($auth));
    }

    /**
     * 根据积分算出用户组
     * @param $point int 积分数
     */
    public function get_usergroup_bypoint($point = 0)
    {
        $groupid = 2;
        if (empty($point)) {
            $member_setting = cache("Member_Config");
            //新会员默认点数
            $point = $member_setting['defualtpoint'] ? $member_setting['defualtpoint'] : 0;
        }
        //获取会有组缓存
        $grouplist = cache("Member_Group");
        foreach ($grouplist as $k => $v) {
            $grouppointlist[$k] = $v['point'];
        }
        //对数组进行逆向排序
        arsort($grouppointlist);
        //如果超出用户组积分设置则为积分最高的用户组
        if ($point > max($grouppointlist)) {
            $groupid = key($grouppointlist);
        } else {
            foreach ($grouppointlist as $k => $v) {
                if ($point >= $v) {
                    $groupid = $tmp_k;
                    break;
                }
                $tmp_k = $k;
            }
        }
        return $groupid;
    }

    /**
     * 更新登录状态信息
     * @param type $userId
     * @return type
     */
    public function loginStatus($userId)
    {
        $data = ['last_login_time' => time(), 'last_login_ip' => request()->ip(1)];
        return $this->save($data, ['id' => $userId]);
    }

    //会员配置缓存
    public function member_cache()
    {
        $data = unserialize(db('Module')->where(['module' => 'member'])->value('setting'));
        cache("Member_Config", $data);
        return $data;
    }

}
