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
// | 短信息后台管理
// +----------------------------------------------------------------------
namespace app\message\controller;

use app\member\controller\MemberBase;
use app\member\model\Member as Member_Model;
use app\message\model\Message as Message_Model;
use app\message\model\MessageData as MessageData_Model;
use app\message\model\MessageGroup as MessageGroup_Model;
use think\facade\Config;

class Index extends MemberBase
{
    //初始化
    protected function initialize()
    {
        parent::initialize();
        $this->groupCache = cache("Member_Group"); //会员模型
    }

    //发送消息
    public function send()
    {
        if ($this->request->isPost()) {
            //判断当前会员，是否可发，短消息．
            $this->messagecheck($this->userid);
            $data = $this->request->post('info/a');
            //验证码
            if (!captcha_check($this->request->post('verify/s', ''))) {
                $this->error('验证码输入错误！');
                return false;
            }
            $data['send_from'] = $this->userinfo['username'];
            $result = $this->validate($data, 'message');
            if (true !== $result) {
                return $this->error($result);
            }
            if ($data['send_to'] == $this->userinfo['username']) {
                return $this->error('不能发给自己');
            }
            if (!Member_Model::getByUsername($data['send_to'])) {
                return $this->error('用户不存在');
            }
            if (Message_Model::create($data)) {
                $this->success('发送成功！', url('outbox'));
            } else {
                $this->error('发送失败！');
            }
        } else {
            return $this->fetch('/send');
        }
    }

    //查看短消息
    public function read()
    {
        $messageid = $this->request->param('id/d', 0);
        empty($messageid) && $this->error('参数不能为空！');
        $info = Message_Model::where([
            'id' => $messageid,
            'send_to' => $this->userinfo['username'],
        ])->findOrEmpty();

        empty($info) && $this->error('你查看的信息不存在！');
        if (0 == $info['status']) {
            $info->status = 1;
            $info->save();
        }
        $this->assign("info", $info);
        return $this->fetch('/read');

    }

    //查看自己发的短消息
    public function read_only()
    {
        $messageid = $this->request->param('id/d', 0);
        empty($messageid) && $this->error('参数不能为空！');
        $info = Message_Model::where([
            'id' => $messageid,
            'send_from' => $this->userinfo['username'],
        ])->find();
        empty($info) && $this->error('你查看的信息不存在！');
        $this->assign("info", $info);
        return $this->fetch('/read');

    }

    //系统消息列表
    public function group()
    {
        if ($this->request->isAjax()) {
            $limit = $this->request->param('limit/d', 10);
            $page = $this->request->param('page/d', 1);
            $_list = MessageGroup_Model::where('groupid', $this->userinfo['groupid'])->page($page, $limit)->select();
            $total = MessageGroup_Model::where('groupid', $this->userinfo['groupid'])->count();

            //未读已读判断
            foreach ($_list as $key => $val) {
                $d = MessageData_Model::where(array('userid' => $this->userid, 'group_message_id' => $val['id']))->find();
                if (!$d) {
                    $_list[$key]['isread'] = 0;
                } else {
                    $_list[$key]['isread'] = 1;
                }
            }
            $result = array("code" => 0, "count" => $total, "data" => $_list);
            return json($result);
        }
        return $this->fetch('/group');
    }

    //查看系统消息
    public function read_group()
    {
        $groupid = $this->request->param('id/d', 0);
        empty($groupid) && $this->error('参数不能为空！');
        $info = MessageGroup_Model::get($groupid);
        if (!$info) {
            $this->error('系统消息不存在！');
        }
        //检查查看表是否有记录,无则向message_data 插入浏览记录
        $check = MessageData_Model::where(['userid' => $this->userid, 'group_message_id' => $groupid])->find();
        if (!$check) {
            MessageData_Model::create([
                'userid' => $this->userid,
                'group_message_id' => $groupid,
            ]);
        }
        $this->assign("info", $info);
        return $this->fetch('/read');
    }

    //收件箱列表
    public function inbox()
    {
        if ($this->request->isAjax()) {
            $limit = $this->request->param('limit/d', 10);
            $page = $this->request->param('page/d', 1);
            $_list = Message_Model::where('send_to', $this->userinfo['username'])->page($page, $limit)->order('id DESC')->select();
            $total = Message_Model::where('send_to', $this->userinfo['username'])->count();
            $result = array("code" => 0, "count" => $total, "data" => $_list);
            return json($result);
        }
        return $this->fetch('/inbox');
    }

    //发件箱列表
    public function outbox()
    {
        if ($this->request->isAjax()) {
            $limit = $this->request->param('limit/d', 10);
            $page = $this->request->param('page/d', 1);
            $_list = Message_Model::where('send_from', $this->userinfo['username'])->page($page, $limit)->order('id DESC')->select();
            $total = Message_Model::where('send_from', $this->userinfo['username'])->count();
            $result = array("code" => 0, "count" => $total, "data" => $_list);
            return json($result);
        }
        return $this->fetch('/outbox');
    }

    /**
     *
     * 检查当前用户短消息相关权限
     * @param  $userid 用户ID
     */
    public function messagecheck($userid)
    {
        if ($this->groupCache[$this->userinfo['groupid']]['allowsendmessage'] == 0) {
            $this->error("对不起你没有权限发短消息！");
        } else {
            //判断是否到限定条数
            $num = Message_Model::where('send_from', $this->userinfo['username'])->count();
            if ($num >= $this->groupCache[$this->userinfo['groupid']]['allowmessage']) {
                $this->error('你的短消息条数已达最大值!');
            }
        }
    }

}
