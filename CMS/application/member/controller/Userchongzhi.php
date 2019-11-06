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
// | 充值记录管理
// +----------------------------------------------------------------------
namespace app\member\controller;

use app\common\controller\Adminbase;
use app\member\model\Rechargerecordes as Rechargerecordes_Model;
use think\Db;

class Userchongzhi extends Adminbase
{
    //初始化
    protected function initialize()
    {
        parent::initialize();
        $this->Rechargerecordes_Model = new Rechargerecordes_Model;
    }

    /**
     * 列表
     */
    public function manage()
    {
        if ($this->request->isAjax()) {
            $limit = $this->request->param('limit/d', 10);
            $page = $this->request->param('page/d', 10);

            $curgamingstate = $this->request->param('curgamingstate');

            if($curgamingstate != null) {
                $_list = $this->Rechargerecordes_Model->page($page, $limit)->
                join('member mb','mb.uid = mol_rechargerecordes.uid','left')->
                where('mol_rechargerecordes.status='.$curgamingstate)->
                field('mol_rechargerecordes.*,mb.username')->
                withAttr('type', function ($value, $data) { if($value == 0) return '点卡充值'; elseif($value == 1) return '支付宝'; else return '微信';})->
                withAttr('status', function ($value, $data) { if($value == 0) return '未支付'; elseif($value == 1) return '已支付'; elseif($value == 2) return '已退款'; else return '微信';})->
                select();
            }
            else {
                $_list = $this->Rechargerecordes_Model->page($page, $limit)->
                join('member mb','mb.uid = mol_rechargerecordes.uid','left')->
                where('username like "%'.$this->request->param('username').'%"')->
                field('mol_rechargerecordes.*,mb.username')->
                withAttr('type', function ($value, $data) { if($value == 0) return '点卡充值'; elseif($value == 1) return '支付宝'; else return '微信';})->
                withAttr('status', function ($value, $data) { if($value == 0) return '未支付'; elseif($value == 1) return '已支付'; elseif($value == 2) return '已退款'; else return '微信';})->
                select();
            }

            $total = count($_list);
            $result = array("code" => 0, "count" => $total, "data" => $_list);
            return json($result);
        }

        return $this->fetch();
    }

    /**
     * 退款
     */
    public function tuikuan()
    {

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
            $info = $this->Rechargerecordes_Model->where(['id' => $uid])->select();
            if (!empty($info)) {
                $this->Rechargerecordes_Model->where(['id' => $uid])->delete();
            }
        }
        $this->success("删除成功！");

    }
}
