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
// | 机器人控制管理
// +----------------------------------------------------------------------
namespace app\member\controller;

use app\common\controller\Adminbase;
use app\member\model\Gametotalmoney as Gametotalmoney_Model;
use think\Db;

class Robotcontrol extends Adminbase
{
    //初始化
    protected function initialize()
    {
        parent::initialize();
        $this->Gametotalmoney_Model = new Gametotalmoney_Model;
    }

    /**
     * 机器人控制列表
     */
    public function manage()
    {
        if ($this->request->isPost()) {
            $id = $this->request->param('id/d', 0);
            $data = $this->request->post();

            //获取信息
            $userinfo = $this->Gametotalmoney_Model->where(["id" => $id])->find();
            if (empty($userinfo)) {
                $this->error('该信息不存在！');
            }

            $updatedata['robotwinmax'] = $data['robotwinmax'];
            $updatedata['robotlostmax'] = $data['robotlostmax'];

            //更新除基本资料外的其他信息
            if (false === $this->Gametotalmoney_Model->allowField(true)->save($updatedata, ['id' => $id])) {
                $this->error('更新失败！');
            }
            $this->success("更新成功！", url("robotcontrol/manage"));

        } else {
            $data = $this->Gametotalmoney_Model->where("id=1")->find();
            if (empty($data)) {
                $this->error("该信息不存在！");
            }

            $this->assign("data", $data);
            return $this->fetch();
        }
    }
}
