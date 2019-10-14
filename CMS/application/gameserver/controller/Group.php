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
// | 会员组管理
// +----------------------------------------------------------------------
namespace app\member\controller;

use app\common\controller\Adminbase;
use app\member\model\Member as Member_Model;
use app\member\model\MemberGroup as Member_Group;

class Group extends Adminbase
{
    //初始化
    protected function initialize()
    {
        parent::initialize();
        $this->Member_Group = new Member_Group;
        $this->Member_Model = new Member_Model;
    }

    /**
     * 会员组列表
     */
    public function index()
    {
        if ($this->request->isAjax()) {
            $_list = $this->Member_Group->order(["listorder" => "ASC", "id" => "DESC"])->select();
            foreach ($_list as $k => $v) {
                //统计会员总数
                $_list[$k]['_count'] = $this->Member_Model->where(["groupid" => $v['id']])->count('id');
            }
            $result = array("code" => 0, "data" => $_list);
            return json($result);
        }
        return $this->fetch();
    }

    /**
     * 会员组添加
     */
    public function add()
    {
        if ($this->request->isPost()) {
            $data = $this->request->post();
            $result = $this->validate($data, 'Group');
            if (true !== $result) {
                return $this->error($result);
            }
            if ($this->Member_Group->groupAdd($data)) {
                $this->success("添加成功！", url("group/index"));
            } else {
                $this->error("添加失败！");
            }

        } else {
            return $this->fetch();
        }
    }

    /**
     * 会员组编辑
     */
    public function edit()
    {
        if ($this->request->isPost()) {
            $data = $this->request->post();
            $result = $this->validate($data, 'Group');
            if (true !== $result) {
                return $this->error($result);
            }
            if ($this->Member_Group->groupEdit($data)) {
                $this->success("修改成功！", url("group/index"));
            } else {
                $this->error("修改失败！");
            }

        } else {
            $groupid = $this->request->param('id/d', 0);
            $data = $this->Member_Group->where(["id" => $groupid])->find();
            if (empty($data)) {
                $this->error("该会员组不存在！", url("Group/index"));
            }
            $this->assign("data", $data);
            //$this->assign('expand', unserialize($data['expand']));
            return $this->fetch();
        }

    }

    /**
     * 会员组删除
     */
    public function delete()
    {
        $groupid = $this->request->param('id/d', 0);
        if (empty($groupid)) {
            $this->error("没有指定需要删除的会员组别！");
        }
        if ($this->Member_Group->groupDelete($groupid)) {
            $this->success("删除成功！", url("group/index"));
        } else {
            $this->error($this->Member_Group->getError());
        }
    }

}
