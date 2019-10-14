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
// | 会员管理
// +----------------------------------------------------------------------
namespace app\member\controller;

use app\common\controller\Adminbase;
use app\member\model\Member as Member_Model;
use app\member\model\Userdata as Userdata_Model;
use think\Db;

class Member extends Adminbase
{
    //初始化
    protected function initialize()
    {
        parent::initialize();
        $this->Member_Model = new Member_Model;
        $this->Userdata_Model = new Userdata_Model;
        $this->groupCache = cache("Member_Group"); //会员模型
    }

    /**
     * 会员列表
     */
    public function manage()
    {
        if ($this->request->isAjax()) {
            $limit = $this->request->param('limit/d', 10);
            $page = $this->request->param('page/d', 10);
            $_list = $this->Member_Model->page($page, $limit)->
                join('userdata ud','ud.userid = mol_member.uid','left')->
                field('mol_member.*,ud.curgamingstate')->
                withAttr('sex', function ($value, $data) { if($value == 0) return '男'; else return '女';})->
                withAttr('genable', function ($value, $data) { if($value == 0) return '封号'; else return '正常';})->
                withAttr('curgamingstate', function ($value, $data) { if($value == 0) return '正常'; elseif($value == 1) return '准备'; elseif($value == 2) return '游戏中'; elseif($value == 3) return '掉线'; elseif($value == 4) return '排队';})->
                withAttr('createtime', function ($value, $data) {return time_format($value);})->
                withAttr('lastlogintime', function ($value, $data) {return time_format($value);})->
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
            $userid = $this->request->param('id/d', 0);
            $data = $this->request->post();

            //获取用户信息
            $userinfo = $this->Member_Model->where(["uid" => $userid])->find();
            if (empty($userinfo)) {
                $this->error('该会员不存在！');
            }

            //修改基本资料
            $data['password'] = md5($data['password']);
            $data['bankpassword'] = md5($data['bankpassword']);

            //更新除基本资料外的其他信息
            if (false === $this->Member_Model->allowField(true)->save($data, ['uid' => $userid])) {
                $this->error('更新失败！');
            }
            $this->success("更新成功！", url("member/manage"));

        } else {
            $userid = $this->request->param('id/d', 0);
            $data = $this->Member_Model->where(["uid" => $userid])->find();
            if (empty($data)) {
                $this->error("该会员不存在！");
            }

            $this->assign("data", $data);
            return $this->fetch();
        }
    }

    /**
     * 编辑玩家游戏数据
     */
    public function gamedata()
    {
        if ($this->request->isPost()) {
            $userid = $this->request->param('id/d', 0);
            $data = $this->request->post();

            //获取用户信息
            $userinfo = $this->Userdata_Model->where(["userid" => $userid])->find();
            if (empty($userinfo)) {
                $this->error('该会员不存在！');
            }

            //更新除基本资料外的其他信息
            if (false === $this->Userdata_Model->allowField(true)->save($data, ['userid' => $userid])) {
                $this->error('更新失败！');
            }
            $this->success("更新成功！", url("member/manage"));

        } else {
            $userid = $this->request->param('id/d', 0);
            $data = $this->Userdata_Model->where(["userid" => $userid])->
                find();
            if (empty($data)) {
                $this->error("该会员不存在！");
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
            $this->error('请选择需要删除的会员！');
        }
        if (!is_array($ids)) {
            $ids = array(0 => $ids);
        }
        foreach ($ids as $uid) {
            $info = $this->Member_Model->find($uid);
            if (!empty($info)) {
                $this->Member_Model->where(['uid' => $uid])->delete($uid);
            }
        }
        $this->success("删除成功！");

    }
}
