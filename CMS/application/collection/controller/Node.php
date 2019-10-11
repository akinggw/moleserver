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
// | 采集管理
// +----------------------------------------------------------------------
namespace app\collection\controller;

use app\collection\model\Nodes as Nodes_Model;
use app\common\controller\Adminbase;

class Node extends Adminbase
{
    protected function initialize()
    {
        parent::initialize();
        $this->Nodes = new Nodes_Model;
    }

    public function index()
    {
        if ($this->request->isAjax()) {
            $data = $this->Nodes->select();
            return json(["code" => 0, "data" => $data]);
        }
        return $this->fetch();
    }

    public function add()
    {
        if ($this->request->isPost()) {
            $data = $this->request->post();
            try {
                $this->Nodes->addNode($data);
            } catch (\Exception $e) {
                $this->error($e->getMessage());
            }
            $this->success('新增成功！', url('index'));
        } else {
            return $this->fetch();
        }
    }

    public function edit()
    {
        if ($this->request->isPost()) {
            $data = $this->request->post();
            try {
                $this->Nodes->editNode($data);
            } catch (\Exception $e) {
                $this->error($e->getMessage());
            }
            $this->success('修改成功！', url('index'));
        } else {
            $id = $this->request->param('id/d', 0);
            if (empty($id)) {
                $this->error('请指定需要修改的采集点！');
            }
            $data = $this->Nodes->where(array('id' => $id))->find();
            if (isset($data['customize_config'])) {
                $data['customize_config'] = json_encode(unserialize($data['customize_config']));
            }
            $this->assign('data', $data);
            return $this->fetch();
        }

    }

    public function delete()
    {
        $nodeid = $this->request->param('ids/a', null);
        if (!is_array($nodeid)) {
            $nodeid = array($nodeid);
        }
        foreach ($nodeid as $tid) {
            $this->Nodes->where(array('id' => $tid))->delete();
        }
        $this->success("删除成功！");

    }

}
