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
// | 转账记录管理
// +----------------------------------------------------------------------
namespace app\gameserver\controller;

use app\common\controller\Adminbase;
use app\gameserver\model\gameroom as gameroom_Model;
use app\gameserver\model\game as game_Model;
use app\gameserver\model\gamerecords as gamerecords_Model;
use app\gameserver\model\goldoperaterecords as goldoperaterecords_Model;

class Transferrecord extends Adminbase
{
    //初始化
    protected function initialize()
    {
        parent::initialize();
       // $this->gameroom_Model = new gameroom_Model;
       // $this->game_Model = new game_Model;
        $this->gamerecords_Model = new gamerecords_Model;
        $this->goldoperaterecords_Model = new goldoperaterecords_Model;
    }

    /**
     * 转账记录
     */
    public function manage()
    {
        if ($this->request->isAjax()) {
            $limit = $this->request->param('limit/d', 10);
            $page = $this->request->param('page/d', 10);

            $username = $this->request->param('username');
            $transfertype = $this->request->param('transfertype');

            if($username != null and $transfertype != null)
            {
                if($transfertype == 0)
                {
                    $_list = $this->goldoperaterecords_Model->page($page, $limit)->
                    join('member mb2','mb2.uid = mol_goldoperaterecords.suid','left')->
                    join('member mb','mb.uid = mol_goldoperaterecords.duid','left')->
                    where('mb2.username like "%'.$username.'%"')->
                    field('mol_goldoperaterecords.*,mb.username as dusername,mb2.username as susername')->
                    //withAttr('operatedate', function ($value, $data) {return time_format($value);})->
                    select();
                }
                else
                {
                    $_list = $this->goldoperaterecords_Model->page($page, $limit)->
                    join('member mb2','mb2.uid = mol_goldoperaterecords.suid','left')->
                    join('member mb','mb.uid = mol_goldoperaterecords.duid','left')->
                    where('mb.username like "%'.$username.'%"')->
                    field('mol_goldoperaterecords.*,mb.username as dusername,mb2.username as susername')->
                    //withAttr('operatedate', function ($value, $data) {return time_format($value);})->
                    select();
                }
            }
            else
            {
                $_list = $this->goldoperaterecords_Model->page($page, $limit)->
                join('member mb2','mb2.uid = mol_goldoperaterecords.suid','left')->
                join('member mb','mb.uid = mol_goldoperaterecords.duid','left')->
                field('mol_goldoperaterecords.*,mb.username as dusername,mb2.username as susername')->
                //withAttr('operatedate', function ($value, $data) {return time_format($value);})->
                select();
            }

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
            $info = $this->goldoperaterecords_Model->find($uid);
            if (!empty($info)) {
                $this->goldoperaterecords_Model->where(['id' => $uid])->delete();
            }
        }

        $this->success("删除成功！");
    }
}
