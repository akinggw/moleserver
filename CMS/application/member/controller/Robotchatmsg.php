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
// | 机器人发言管理
// +----------------------------------------------------------------------
namespace app\member\controller;

use app\common\controller\Adminbase;
use app\member\model\Robotchatmsg as Robotchatmsg_Model;
use think\Db;

class Robotchatmsg extends Adminbase
{
    //初始化
    protected function initialize()
    {
        parent::initialize();
        $this->Robotchatmsg_Model = new Robotchatmsg_Model;
    }

    /**
     * 机器人发言列表
     */
    public function manage()
    {
        if ($this->request->isAjax()) {
            $limit = $this->request->param('limit/d', 10);
            $page = $this->request->param('page/d', 10);

            $_list = $this->Robotchatmsg_Model->page($page, $limit)->
                where('type=1')->
                select();

            $total = count($_list);
            $result = array("code" => 0, "count" => $total, "data" => $_list);
            return json($result);
        }

        return $this->fetch();
    }

    /**
     * 编辑
     */
    public function edit()
    {
        if ($this->request->isPost()) {
            $id = $this->request->param('id/d', 0);
            $data = $this->request->post();

            //获取用户信息
            $userinfo = $this->Robotchatmsg_Model->where(["id" => $id])->find();
            if (empty($userinfo)) {
                $this->error('该信息不存在！');
            }

            //更新除基本资料外的其他信息
            if (false === $this->Robotchatmsg_Model->allowField(true)->save($data, ['id' => $id])) {
                $this->error('更新失败！');
            }
            $this->success("更新成功！", url("robotchatmsg/manage"));

        } else {
            $id = $this->request->param('id/d', 0);
            $data = $this->Robotchatmsg_Model->where(["id" => $id])->find();
            if (empty($data)) {
                $this->error("该信息不存在！");
            }

            $this->assign("data", $data);
            return $this->fetch();
        }
    }

    /**
     * 添加
     */
    public function add()
    {
        if ($this->request->isPost()) {
            $data = $this->request->post();

            $data['type'] = 1;

            //更新除基本资料外的其他信息
            if (false === $this->Robotchatmsg_Model->allowField(true)->save($data)) {
                $this->error('添加失败！');
            }

            $this->success("添加成功！", url("robotchatmsg/manage"));

        } else {
            return $this->fetch();
        }
    }

    /**
     * 删除
     */
    public function delete()
    {
        $ids = $this->request->param('ids/a', null);
        if (empty($ids)) {
            $this->error('请选择需要删除的机器人发言！');
        }
        if (!is_array($ids)) {
            $ids = array(0 => $ids);
        }
        foreach ($ids as $uid) {
            $info = $this->Robotchatmsg_Model->where(['id' => $uid])->select();
            if (!empty($info)) {
                $this->Robotchatmsg_Model->where(['id' => $uid])->delete();
            }
        }
        $this->success("删除成功！");

    }
}
