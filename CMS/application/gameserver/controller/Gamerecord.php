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
// | 游戏记录管理
// +----------------------------------------------------------------------
namespace app\gameserver\controller;

use app\common\controller\Adminbase;
use app\gameserver\model\gameroom as gameroom_Model;
use app\gameserver\model\game as game_Model;
use app\gameserver\model\gamerecords as gamerecords_Model;
use app\gameserver\model\goldoperaterecords as goldoperaterecords_Model;

class Gamerecord extends Adminbase
{
    //初始化
    protected function initialize()
    {
        parent::initialize();
     //   $this->gameroom_Model = new gameroom_Model;
     //   $this->game_Model = new game_Model;
        $this->gamerecords_Model = new gamerecords_Model;
        $this->goldoperaterecords_Model = new goldoperaterecords_Model;
    }

    /**
     * 游戏记录
     */
    public function manage()
    {
        if ($this->request->isAjax()) {
            $limit = $this->request->param('limit/d', 10);
            $page = $this->request->param('page/d', 10);
            $_list = $this->gamerecords_Model->page($page, $limit)->
            join('game ge','ge.id = mol_gamerecords.gameid','left')->
            join('member mb','mb.uid = mol_gamerecords.userid','left')->
            field('mol_gamerecords.*,ge.name,mb.username')->
            select();
            $total = count($_list);
            $result = array("code" => 0, "count" => $total, "data" => $_list);
            return json($result);

        }
        return $this->fetch();
    }

    /**
     * 删除
     */
    public function delete()
    {
        $ids = $this->request->param('ids/a', null);
        if (empty($ids)) {
            $this->error('请选择需要删除的记录！');
        }
        if (!is_array($ids)) {
            $ids = array(0 => $ids);
        }
        foreach ($ids as $uid) {
            $info = $this->gamerecords_Model->find($uid);
            if (!empty($info)) {
                $this->gamerecords_Model->where(['id' => $uid])->delete();
            }
        }

        $this->success("删除成功！");
    }
}
