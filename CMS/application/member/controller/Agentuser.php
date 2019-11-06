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
// | 代理管理
// +----------------------------------------------------------------------
namespace app\member\controller;

use app\common\controller\Adminbase;
use app\member\model\Member as Member_Model;
use app\member\model\Userdata as Userdata_Model;
use app\member\model\androiduserinfo as AndroidUserInfo_Mode;
use app\gameserver\model\gameroom as gameroom_Model;
use app\gameserver\model\game as game_Model;
use app\member\model\Agentuser as Agentuser_Model;
use app\member\model\Rechargerecordes as Rechargerecordes_Model;
use think\Db;

class Agentuser extends Adminbase
{
    public $agentuserlist;
    public $agentuserlistcount;

    //初始化
    protected function initialize()
    {
        parent::initialize();
        $this->Member_Model = new Member_Model;
        $this->Userdata_Model = new Userdata_Model;
        $this->AndroidUserInfo_Mode = new AndroidUserInfo_Mode;
        $this->gameroom_Model = new gameroom_Model;
        $this->game_Model = new game_Model;
        $this->Agentuser_Model = new Agentuser_Model;
        $this->Rechargerecordes_Model = new Rechargerecordes_Model;
        $this->agentuserlistcount = 0;
        $this->agentuserlist = [];
    }

    /*
     * 得到所有的代理
     */
    public function getAllagents($userid)
    {
        $listdata = $this->Agentuser_Model->
            where('parentuserid='.$userid)->
            select();

        $index=0;
        foreach($listdata as $k=>$v) {
            $this->agentuserlist[$this->agentuserlistcount] = $listdata[$index];
            $this->agentuserlistcount = $this->agentuserlistcount + 1;

            $this->getAllagents($listdata[$index]['id']);

            $index = $index + 1;
        }
    }

    /**
     * 得到所有代理会员的数量
     */
    public function getAllagentUsers($agentlist)
    {
        $count = 0;

        $index=0;
        foreach($agentlist as $k=>$v) {
            $agentuserid = $agentlist[$index]['id'];

            $tmpcount = $this->Member_Model->
                            where('ruid='.$agentuserid)->
                            count();

            $count = $count + $tmpcount;

            $index = $index + 1;
        }

        return $count;
    }

    /**
     * 得到所有代理会员的总充值
     */
    public function getAllagentUserChongZhi($agentlist,$type)
    {
        $count = 0;

        $index=0;
        foreach($agentlist as $k=>$v) {
            $tmpcount = null;

            if($type == 0) {
                $tmpcount = $this->Rechargerecordes_Model->
                join('member mb','mb.ruid='.$agentlist[$index]['id'],'left')->
                where('mol_rechargerecordes.uid=mb.uid')->
                field('sum(mol_rechargerecordes.realmoney) as rechargerecordes')->
                select();
            }
            else
            {
                $tmpcount = $this->Rechargerecordes_Model->
                join('member mb','mb.ruid='.$agentlist[$index]['id'],'left')->
                where('mol_rechargerecordes.uid=mb.uid and PERIOD_DIFF( date_format( now( ) , "%Y%m" ) ,DATE_FORMAT( mol_rechargerecordes.rechargedate, "%Y%m" ) ) = 0')->
                field('sum(mol_rechargerecordes.realmoney) as rechargerecordes')->
                select();
            }

            if($tmpcount)
            {
                $count = $count + $tmpcount[0]['rechargerecordes'];
            }

            $index = $index + 1;
        }

        return $count;
    }

    /**
     * 代理列表
     */
    public function manage()
    {
        if ($this->request->isAjax()) {
            $limit = $this->request->param('limit/d', 10);
            $page = $this->request->param('page/d', 10);

            $_list = $this->Agentuser_Model->page($page, $limit)->
            join('member mb','mb.uid = mol_agentuser.userid','left')->
            where('mb.username like "%'.$this->request->param('username').'%"')->
            field('mol_agentuser.*,mb.username')->
            withAttr('agentlevel', function ($value, $data) { if($value == 1) return '一级代理'; elseif($value == 2) return '二级代理'; else return '三级代理';})->
            select();

            $index=0;
            foreach($_list as $k=>$v) {
                $this->agentuserlist = [];
                $this->agentuserlistcount=0;

                $this->agentuserlist[$this->agentuserlistcount] = $_list[$index];
                $this->agentuserlistcount=1;

                $this->getAllagents($_list[$index]['id']);

                if($_list[$index]['parentuserid'] == 0) {
                    $_list[$index]['parentusername'] = "----无----";
                }
                else {
                    $userinfo = $this->Member_Model->
                        join('mol_agentuser au','au.id='.$_list[$index]['parentuserid'])->
                        where('au.userid=mol_member.uid')->
                        field('mol_member.username as username')->
                        find();

                    $_list[$index]['parentusername'] = $userinfo['username'];
                }

                $_list[$index]['childagentcount'] = count($this->agentuserlist)-1;
                $_list[$index]['childagentusercount'] = $this->getAllagentUsers($this->agentuserlist);
                $_list[$index]['totalmothmoney'] = $this->getAllagentUserChongZhi($this->agentuserlist,0);
                $_list[$index]['thismothmoney'] = $this->getAllagentUserChongZhi($this->agentuserlist,1);

                $index = $index + 1;
            }

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
            $userid = $this->request->param('id/d', 0);
            $data = $this->request->post();

            //获取用户信息
            $userinfo = $this->Agentuser_Model->where(["id" => $userid])->find();
            if (empty($userinfo)) {
                $this->error('该代理不存在！');
            }

            //更新除基本资料外的其他信息
            if (false === $this->Agentuser_Model->allowField(true)->save($data, ['id' => $userid])) {
                $this->error('更新失败！');
            }
            $this->success("更新成功！", url("agentuser/manage"));

        } else {
            $userid = $this->request->param('id/d', 0);
            $data = $this->Agentuser_Model->
            where(["id" => $userid])->
            find();

            if (empty($data)) {
                $this->error("该代理不存在！");
            }

            $agentdata = $this->Agentuser_Model->
            join('member mb','mb.uid = mol_agentuser.userid','left')->
            where('id <>'.$userid)->
            field('mol_agentuser.*,mb.username')->
            select();

            $this->assign("data", $data);
            $this->assign('agentdata',$agentdata);
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
            $this->error('请选择需要删除的代理！');
        }
        if (!is_array($ids)) {
            $ids = array(0 => $ids);
        }
        foreach ($ids as $uid) {
            $info = $this->Agentuser_Model->where(["id" => $uid])->find();
            if (!empty($info)) {
                $this->Agentuser_Model->where(["id" => $uid])->delete();
            }
        }
        $this->success("删除成功！");

    }
}
