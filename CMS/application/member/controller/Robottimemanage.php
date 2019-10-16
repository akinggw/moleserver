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
// | 机器人上岗时间管理
// +----------------------------------------------------------------------
namespace app\member\controller;

use app\common\controller\Adminbase;
//use app\member\model\Member as Member_Model;
//use app\member\model\Userdata as Userdata_Model;
//use app\member\model\androiduserinfo as AndroidUserInfo_Mode;
//use app\gameserver\model\gameroom as gameroom_Model;
//use app\gameserver\model\game as game_Model;
use app\member\model\Robotcontroltimes as Robotcontroltimes_Model;

class Robottimemanage extends Adminbase
{
    //初始化
    protected function initialize()
    {
        parent::initialize();
        $this->Robotcontroltimes_Model = new Robotcontroltimes_Model;
    }

    /**
     * 机器人上岗时间列表
     */
    public function manage()
    {
        if ($this->request->isAjax()) {
            $limit = $this->request->param('limit/d', 10);
            $page = $this->request->param('page/d', 10);
            $_list = $this->Robotcontroltimes_Model->page($page, $limit)->
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
            $userinfo = $this->Robotcontroltimes_Model->where(["id" => $id])->find();
            if (empty($userinfo)) {
                $this->error('该时间段不存在！');
            }

            //更新除基本资料外的其他信息
            if (false === $this->Robotcontroltimes_Model->allowField(true)->save($data, ['id' => $id])) {
                $this->error('更新失败！');
            }
            $this->success("更新成功！", url("robottimemanage/manage"));

        } else {
            $id = $this->request->param('id/d', 0);
            $data = $this->Robotcontroltimes_Model->where(["id" => $id])->find();
            if (empty($data)) {
                $this->error("该时间段不存在！");
            }

            $this->assign("data", $data);
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
            $this->error('请选择需要删除的时间段！');
        }
        if (!is_array($ids)) {
            $ids = array(0 => $ids);
        }
        foreach ($ids as $uid) {
            $info = $this->Robotcontroltimes_Model->find($uid);
            if (!empty($info)) {
                $this->Robotcontroltimes_Model->where(["id" => $uid])->delete();
            }
        }
        $this->success("删除成功！");

    }

    /**
     * 添加
     */
    public function add()
    {
        if ($this->request->isPost()) {
            $data = $this->request->post();

            //更新除基本资料外的其他信息
            if (false === $this->Robotcontroltimes_Model->allowField(true)->save($data)) {
                $this->error('添加失败！');
            }
            $this->success("添加成功！", url("robottimemanage/manage"));

        } else {
            return $this->fetch();
        }
    }
}
