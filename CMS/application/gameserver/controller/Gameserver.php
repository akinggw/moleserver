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
// | 游戏服务器管理
// +----------------------------------------------------------------------
namespace app\gameserver\controller;

use app\common\controller\Adminbase;
use app\gameserver\model\gameroom as gameroom_Model;
use app\gameserver\model\game as game_Model;
use think\Db;

class Gameserver extends Adminbase
{
    //初始化
    protected function initialize()
    {
        parent::initialize();
        $this->gameroom_Model = new gameroom_Model;
        $this->game_Model = new game_Model;
    }

    /**
     * 游戏服务器列表
     */
    public function manage()
    {
        if ($this->request->isAjax()) {
            $limit = $this->request->param('limit/d', 10);
            $page = $this->request->param('page/d', 10);

            $servername = $this->request->param('servername');
            $gameid = $this->request->param('gameid');

            if($gameid)
            {
                $_list = $this->gameroom_Model->page($page, $limit)->
                join('game ge','ge.id = mol_gameroom.gameid','left')->
                where('mol_gameroom.servername like "%'.$servername.'%" and ge.id='.$gameid)->
                field('mol_gameroom.*,ge.name')->
                select();
            }
            else
            {
                $_list = $this->gameroom_Model->page($page, $limit)->
                join('game ge','ge.id = mol_gameroom.gameid','left')->
                where('mol_gameroom.servername like "%'.$servername.'%"')->
                field('mol_gameroom.*,ge.name')->
                select();
            }

            $total = count($_list);
            $result = array("code" => 0, "count" => $total, "data" => $_list);
            return json($result);

        }

        $games = $this->game_Model->select();
        $this->assign("games", $games);
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
            $serverinfo = $this->gameroom_Model->where(["id" => $id])->find();
            if (empty($serverinfo)) {
                $this->error('该服务器不存在！');
            }

            //更新除基本资料外的其他信息
            if (false === $this->gameroom_Model->allowField(true)->save($data, ['id' => $id])) {
                $this->error('更新失败！');
            }
            $this->success("更新成功！", url("gameserver/manage"));

        } else {
            $id = $this->request->param('id/d', 0);
            $data = $this->gameroom_Model->where(["id" => $id])->find();
            if (empty($data)) {
                $this->error("该服务器不存在！");
            }

            $games = $this->game_Model->select();

            $this->assign("games", $games);
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
            $info = $this->gameroom_Model->find($uid);
            if (!empty($info)) {
                $this->gameroom_Model->where(['id' => $uid])->delete();
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
            if (false === $this->gameroom_Model->allowField(true)->save($data)) {
                $this->error('添加失败！');
            }
            $this->success("添加成功！", url("gameserver/manage"));

        } else {
            $games = $this->game_Model->select();
            $this->assign("games", $games);
            return $this->fetch();
        }
    }
}
