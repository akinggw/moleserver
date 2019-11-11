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
namespace app\agentuser\controller;

use app\common\controller\Base;
use app\member\model\Member as Member_Model;
use app\member\model\Userdata as Userdata_Model;
use app\member\model\androiduserinfo as AndroidUserInfo_Mode;
use app\gameserver\model\gameroom as gameroom_Model;
use app\gameserver\model\game as game_Model;
use app\member\model\Agentuser as Agentuser_Model;
use app\member\model\Rechargerecordes as Rechargerecordes_Model;

class Index extends Base
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
            join('mol_agentuser au','au.userid=mol_member.uid','left')->
            where('ruid='.$agentuserid)->
            select();

            foreach($tmpcount as $kk=>$vv) {
                if($vv['id'] <= 0) {
                    $count = $count + 1;
                }
            }

            $index = $index + 1;
        }

        return $count;
    }

    /**
     * 得到所有代理的信息
     */
    public function getAllagentList($agentlist)
    {
        $list = [];

        $index=0;
        foreach($agentlist as $k=>$v) {
            $agentuserid = $agentlist[$index]['userid'];

            $tmpcount = $this->Member_Model->
            join('mol_agentuser au','au.userid=mol_member.uid','left')->
            where('uid='.$agentuserid)->
            withAttr('createtime', function ($value, $data) {return time_format($value);})->
            withAttr('ruid',function($value,$data){
                $tmpUsername = $this->Member_Model->
                join('mol_agentuser','mol_agentuser.id='.$value,'left')->
                where('mol_agentuser.userid=mol_member.uid')->
                field('mol_member.username as agentusername')->
                find();

                if($tmpUsername == null) $tmpUsername['agentusername'] = '无代理';

                return $tmpUsername['agentusername'];
            })->
            find();

            $list[$index] = $tmpcount;

            $index = $index + 1;
        }

        return $list;
    }

    /**
     * 得到所有代理的会员
     */
    public function getAllagentUserInfo($agentlist)
    {
        $list = [];

        $index=0;
        foreach($agentlist as $k=>$v) {
            $agentuserid = $agentlist[$index]['id'];

            $tmplist = $this->Member_Model->
            join('mol_agentuser au','au.userid=mol_member.uid','left')->
            where('ruid='.$agentuserid)->
            select()->
            withAttr('createtime', function ($value, $data) {return time_format($value);})->
            withAttr('ruid',function($value,$data){
                    $tmpUsername = $this->Member_Model->
                        join('mol_agentuser','mol_agentuser.id='.$value,'left')->
                        where('mol_agentuser.userid=mol_member.uid')->
                        field('mol_member.username as agentusername')->
                        find();

                    if($tmpUsername == null) $tmpUsername['agentusername'] = '无代理';

                    return $tmpUsername['agentusername'];
            })->
            toArray();

            $list = array_merge($list,$tmplist);

            $index = $index + 1;
        }

        return $list;
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
                join('mol_member mb','mb.ruid='.$agentlist[$index]['id'],'left')->
                where('mol_rechargerecordes.uid=mb.uid')->
                field('sum(mol_rechargerecordes.realmoney) as rechargerecordes')->
                select();
            }
            else
            {
                $tmpcount = $this->Rechargerecordes_Model->
                join('mol_member mb','mb.ruid='.$agentlist[$index]['id'],'left')->
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
     * 得到所有代理会员的充值列表
     */
    public function getAllagentUserChongZhiList($agentlist,$type)
    {
        $list = [];

        $index=0;
        foreach($agentlist as $k=>$v) {
            $tmplist = [];

            if($type == 0) {
                $tmplist = $this->Rechargerecordes_Model->
                join('mol_member mb','mb.ruid='.$agentlist[$index]['id'],'left')->
                where('mol_rechargerecordes.uid=mb.uid and PERIOD_DIFF( date_format( now( ) , "%Y%m" ) ,DATE_FORMAT( mol_rechargerecordes.rechargedate, "%Y%m" ) ) = 0 and type=1')->
                field('mol_rechargerecordes.*,mb.username')->
                select()->
                toArray();
            }
            elseif($type == 1)
            {
                $tmplist = $this->Rechargerecordes_Model->
                join('mol_member mb','mb.ruid='.$agentlist[$index]['id'],'left')->
                where('mol_rechargerecordes.uid=mb.uid and PERIOD_DIFF( date_format( now( ) , "%Y%m" ) ,DATE_FORMAT( mol_rechargerecordes.rechargedate, "%Y%m" ) ) = 1 and type=1')->
                field('mol_rechargerecordes.*,mb.username')->
                select()->
                toArray();
            }
            else
            {
                $tmplist = $this->Rechargerecordes_Model->
                join('mol_member mb','mb.ruid='.$agentlist[$index]['id'],'left')->
                where('mol_rechargerecordes.uid=mb.uid and to_days(mol_rechargerecordes.rechargedate) = to_days(NOW()) and type=1')->
                field('mol_rechargerecordes.*,mb.username')->
                select()->
                toArray();
            }

            $list = array_merge($list,$tmplist);

            $index = $index + 1;
        }

        return $list;
    }

    /**
     * 代理列表
     */
    public function index()
    {
        $type=input('param.type');

        $agentuserinfo = cookie('agent_user_info');

        if($agentuserinfo != null)
        {
            $agentuserinfoarray = json_decode($agentuserinfo,true);

            $userinfo = $this->Agentuser_Model->
            join('mol_member mb','mb.uid=mol_agentuser.userid','left')->
            where('mb.uid="'.$agentuserinfoarray['uid'].'" and mol_agentuser.id="'.$agentuserinfoarray['id'].'"')->
            find();

            $tmpuseinfo = $this->Member_Model->
                join('mol_agentuser au','au.id='.$userinfo['parentuserid'])->
                where('au.userid=mol_member.uid')->
                field('mol_member.username')->
                find();

            $userinfo['lastagentuser'] =  $tmpuseinfo['username'];

            $this->assign('type',$type);
            $this->assign('agentuserinfo',$userinfo);

            $this->agentuserlist = [];
            $this->agentuserlistcount=0;

            $this->agentuserlist[$this->agentuserlistcount] = $userinfo;
            $this->agentuserlistcount=1;

            $this->getAllagents($userinfo['id']);

            $agentuserdata['childagentcount'] = count($this->agentuserlist)-1;
            $agentuserdata['childagentusercount'] = $this->getAllagentUsers($this->agentuserlist);
            $agentuserdata['totalmothmoney'] = $this->getAllagentUserChongZhi($this->agentuserlist,0);
            $agentuserdata['thismothmoney'] = $this->getAllagentUserChongZhi($this->agentuserlist,1);

            $this->assign('agentuserdata',$agentuserdata);

            $thismothczlist = $this->getAllagentUserChongZhiList($this->agentuserlist,2);

            $this->assign('agent_order_info',$thismothczlist);
            $this->assign('empty','<span style=\'margin:10px 0 0 20px;display:block;\'>没有订单数据</span>');
        }

        return $this->fetch();
    }

    /**
     * 得到所有的代理
     */
    public function getAllagentuserlist()
    {
        $agentuserinfo = cookie('agent_user_info');

        if($agentuserinfo != null) {
            $agentuserinfoarray = json_decode($agentuserinfo, true);

            $userinfo = $this->Agentuser_Model->
            join('mol_member mb', 'mb.uid=mol_agentuser.userid', 'left')->
            where('mb.uid="' . $agentuserinfoarray['uid'] . '" and mol_agentuser.id="' . $agentuserinfoarray['id'] . '"')->
            find();

            $this->agentuserlist = [];
            $this->agentuserlistcount=0;

            //$this->agentuserlist[$this->agentuserlistcount] = $userinfo;
           // $this->agentuserlistcount=1;

            $this->getAllagents($userinfo['id']);

            $thismothczlist = $this->getAllagentList($this->agentuserlist);

            $this->assign('agentuserlistinfo',$thismothczlist);
        }

        $this->assign('empty','<span style=\'margin:10px 0 0 20px;display:block;\'>没有订单数据</span>');
        $this->assign('type','agent');
        return $this->fetch('agentuserlistinfo');
    }

    /**
     * 得到所有代理的会员信息
     */
    public function getAllagentuserlistinfo()
    {
        $agentuserinfo = cookie('agent_user_info');
        $agentuserid=input('param.agentuserid');

        if($agentuserinfo != null) {
            $agentuserinfoarray = json_decode($agentuserinfo, true);

            $userinfo = $this->Agentuser_Model->
            join('mol_member mb', 'mb.uid=mol_agentuser.userid', 'left')->
            where('mb.uid="' . $agentuserinfoarray['uid'] . '" and mol_agentuser.id="' . $agentuserinfoarray['id'] . '"')->
            find();

            $this->agentuserlist = [];
            $this->agentuserlistcount=0;

            $this->agentuserlist[$this->agentuserlistcount] = $userinfo;
            $this->agentuserlistcount=1;

            $this->getAllagents($userinfo['id']);

            $thismothczlist = $this->getAllagentUserInfo($this->agentuserlist,$type);

            $this->assign('agentuserlistinfo',$thismothczlist);
        }

        $this->assign('empty','<span style=\'margin:10px 0 0 20px;display:block;\'>没有订单数据</span>');
        $this->assign('agentuserid',$agentuserid);
        $this->assign('type','agentuser');
        return $this->fetch('agentuserlistinfo');
    }

    /**
     * 充值订单
     */
    public function thismonthrecord()
    {
        $type=input('param.type');
        $agentuserinfo = cookie('agent_user_info');

        if($agentuserinfo != null) {
            $agentuserinfoarray = json_decode($agentuserinfo, true);

            $userinfo = $this->Agentuser_Model->
            join('mol_member mb', 'mb.uid=mol_agentuser.userid', 'left')->
            where('mb.uid="' . $agentuserinfoarray['uid'] . '" and mol_agentuser.id="' . $agentuserinfoarray['id'] . '"')->
            find();

            $this->agentuserlist = [];
            $this->agentuserlistcount=0;

            $this->agentuserlist[$this->agentuserlistcount] = $userinfo;
            $this->agentuserlistcount=1;

            $this->getAllagents($userinfo['id']);

            $thismothczlist = $this->getAllagentUserChongZhiList($this->agentuserlist,$type);

            $this->assign('agent_order_info',$thismothczlist);
        }

        $this->assign('empty','<span style=\'margin:10px 0 0 20px;display:block;\'>没有订单数据</span>');

        if($type == 0)
            $this->assign('ordertitle','本月充值');
        else
            $this->assign('ordertitle','上月充值');

        return $this->fetch('order');
    }

    /**
     * 代理商退出
     */
    public function userquit()
    {
        cookie('agent_user_info',null);
        return ['success'=>'退出成功！','url'=>url('Index/index')];
    }

    /**
     * 修改代理资料
     */
    public function updateuserinfo()
    {
        $data=input('post.');
        $agentuserinfo = cookie('agent_user_info');

        if($agentuserinfo != null) {
            $agentuserinfoarray = json_decode($agentuserinfo, true);

            if($this->Agentuser_Model->
                where('id='.$agentuserinfoarray['id'])->
                update($data))
            {
                return ['success'=>'资料修改成功'];
            }
        }

        return ['error'=>'资料修改失败!'];
    }

    /**
     * 代理商登录
     */
    public function userlogin()
    {
        $data=input('post.');

        if($data['username'] == null or $data['userpwd'] == null)
        {
            return ['error'=>'账户或用户名不能为空!','url'=>url('Index/index')];
        }

        $pwd = md5($data['userpwd']);

        $userinfo = $this->Agentuser_Model->
            join('mol_member mb','mb.uid=mol_agentuser.userid','left')->
            where('mb.username="'.$data['username'].'" and mb.password="'.$pwd.'"')->
            field('mb.uid,mol_agentuser.id')->
            find();

        if($userinfo == null)
        {
            return ['error'=>'账户或用户名错误!','url'=>url('Index/index')];
        }
        else
        {
            cookie('agent_user_info',$userinfo);
            return ['success'=>'登录成功','url'=>url('Index/index')];
        }
    }

    /**
     * 注册为代理商
     */
    public function agentuserto()
    {
        $data=input('post.');

        if($data['agentdivided'] == null or $data['alipayname'] == null or $data['alipayaccount'] == null)
        {
            return ['error'=>'分成比例,淘宝账号不能为空!','url'=>url('Index/getallagentuserlistinfo')];
        }

        $userinfo = $this->Agentuser_Model->
        where('userid="'.$data['uid'].'"')->
        find();

        if($userinfo != null)
        {
            return ['error'=>'当前用户已经是代理了，不能重复注册!','url'=>url('Index/getallagentuserlistinfo')];
        }

        if($this->Agentuser_Model->save($data))
        {
            return ['success'=>'代理注册成功','url'=>url('Index/index')];
        }

        return ['error'=>'代理注册失败!','url'=>url('Index/getallagentuserlistinfo')];
    }

    /**
     * 删除代理
     */
    public function agentuserdel()
    {
        $data=input('post.');

        if($this->Agentuser_Model->
            where('id='.$data['agentid'])->
            delete())
        {
            return ['success'=>'代理删除成功','url'=>url('Index/index')];
        }

        return ['error'=>'代理删除失败!','url'=>url('Index/getallagentuserlistinfo')];
    }
}
