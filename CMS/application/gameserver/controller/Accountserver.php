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
// | 账号服务器管理
// +----------------------------------------------------------------------
namespace app\gameserver\controller;

use app\common\controller\Adminbase;
//use app\gameserver\model\gameroom as gameroom_Model;
//use app\gameserver\model\game as game_Model;
use app\gameserver\model\accountserver as accountserver_Model;
use think\Db;

class Accountserver extends Adminbase
{
    //初始化
    protected function initialize()
    {
        parent::initialize();
 //       $this->gameroom_Model = new gameroom_Model;
 //       $this->game_Model = new game_Model;
        $this->accountserver_Model = new accountserver_Model;
    }

    /**
     * 账号服务器列表
     */
    public function manage()
    {
        if ($this->request->isAjax()) {
            $limit = $this->request->param('limit/d', 10);
            $page = $this->request->param('page/d', 10);

            $servername = $this->request->param('servername');

            $_list = $this->accountserver_Model->page($page, $limit)->
            where('mol_accountserver.servername like "%'.$servername.'%"')->
            withAttr('serverstate', function ($value, $data) { if($value == 0) return '开放'; else return '封闭';})->
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

            //获取服务器信息
            $serverinfo = $this->accountserver_Model->where(["id" => $id])->find();
            if (empty($serverinfo)) {
                $this->error('该服务器不存在！');
            }

            //更新除基本资料外的其他信息
            if (false === $this->accountserver_Model->allowField(true)->save($data, ['id' => $id])) {
                $this->error('更新失败！');
            }
            $this->success("更新成功！", url("accountserver/manage"));

        } else {
            $id = $this->request->param('id/d', 0);
            $data = $this->accountserver_Model->where(["id" => $id])->find();
            if (empty($data)) {
                $this->error("该服务器不存在！");
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
            $this->error('请选择需要删除的服务器！');
        }
        if (!is_array($ids)) {
            $ids = array(0 => $ids);
        }
        foreach ($ids as $uid) {
            $info = $this->accountserver_Model->find($uid);
            if (!empty($info)) {
                $this->accountserver_Model->where(['id' => $uid])->delete();
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

            $data['createtime'] = date('Y-m-d H:i:s',time());

            //更新除基本资料外的其他信息
            if (false === $this->accountserver_Model->allowField(true)->save($data)) {
                $this->error('添加失败！');
            }
            $this->success("添加成功！", url("accountserver/manage"));

        } else {
            return $this->fetch();
        }
    }
}
