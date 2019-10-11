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

use app\common\controller\Adminbase;
use app\member\model\Member as Member_Model;
use app\message\model\Message as Message_Model;
use app\message\model\MessageGroup as MessageGroup_Model;

class Message extends Adminbase
{
    //初始化
    protected function initialize()
    {
        parent::initialize();
        $this->groupCache = cache("Member_Group"); //会员模型
        $this->Message_Model = new Message_Model;
        $this->MessageGroup_Model = new MessageGroup_Model;
    }

    public function index()
    {
        if ($this->request->isAjax()) {
            $limit = $this->request->param('limit/d', 10);
            $page = $this->request->param('page/d', 1);
            $_list = $this->Message_Model->page($page, $limit)->select();
            $total = count($_list);
            $result = array("code" => 0, "count" => $total, "data" => $_list);
            return json($result);
        }
        return $this->fetch();
    }

    /**
     * 群发消息管理  ...
     */
    public function group()
    {
        if ($this->request->isAjax()) {
            $limit = $this->request->param('limit/d', 10);
            $page = $this->request->param('page/d', 1);
            $_list = $this->MessageGroup_Model->page($page, $limit)->select();
            foreach ($_list as $k => $v) {
                $_list[$k]['groupname'] = $this->groupCache[$v['groupid']]['name'];
            }
            $total = count($_list);
            $result = array("code" => 0, "count" => $total, "data" => $_list);
            return json($result);
        }
        return $this->fetch();
    }

    /**
     * 管理按组或角色 群发消息
     */
    public function message_send()
    {
        if ($this->request->isPost()) {
            $data = $this->request->post('info/a');
            $result = $this->validate($data, 'message_group');
            if (true !== $result) {
                return $this->error($result);
            }
            if ($this->MessageGroup_Model->allowField(true)->save($data)) {
                $this->success('发送成功！');
            } else {
                $this->error('发送失败！');
            }

        } else {
            foreach ($this->groupCache as $g) {
                $groupCache[$g['id']] = $g['name'];
            }
            $this->assign("Member_group", $groupCache);
            return $this->fetch();
        }

    }

    /**
     * 发消息
     */
    public function send_one()
    {
        if ($this->request->isPost()) {
            $data = $this->request->post('info/a');
            $data['send_from'] = $this->_userinfo['username'];
            $result = $this->validate($data, 'message');
            if (true !== $result) {
                return $this->error($result);
            }
            /*if ($data['send_from'] == $this->_userinfo['username']) {
            return $this->error('不能发给自己');
            }*/
            if (!Member_Model::getByUsername($data['send_to'])) {
                return $this->error('用户不存在');
            }
            if ($this->Message_Model->allowField(true)->save($data)) {
                $this->success('发送成功！');
            } else {
                $this->error('发送失败！');
            }

        } else {
            return $this->fetch();
        }
    }

    /**
     * 删除短消息
     */
    public function delete()
    {
        $ids = $this->request->param('ids/a');
        empty($ids) && $this->error('参数错误！');
        if (!is_array($ids)) {
            $ids = array($ids);
        }
        $res = $this->Message_Model->where('id', 'in', $ids)->delete();
        if ($res !== false) {
            $this->success('删除成功！');
        } else {
            $this->error('删除失败！');
        }
    }

    /**
     * 删除系统 短消息
     * @param   intval  $sid
     */
    public function delete_group()
    {
        $ids = $this->request->param('ids/a');
        empty($ids) && $this->error('参数错误！');
        if (!is_array($ids)) {
            $ids = array($ids);
        }
        $res = $this->MessageGroup_Model->where('id', 'in', $ids)->delete();
        if ($res !== false) {
            $this->success('删除成功！');
        } else {
            $this->error('删除失败！');
        }
    }

}
