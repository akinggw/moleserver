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
// | 模型管理
// +----------------------------------------------------------------------
namespace app\cms\controller;

use app\cms\model\Models as Models_Model;
use app\common\controller\Adminbase;
use think\Db;

class Models extends Adminbase
{
    protected function initialize()
    {
        parent::initialize();
        $this->Models = new Models_Model;
        //取得当前内容模型模板存放目录
        $this->filepath = TEMPLATE_PATH . (empty(config('theme')) ? "default" : config('theme')) . DIRECTORY_SEPARATOR . "cms" . DIRECTORY_SEPARATOR;
        //取得栏目频道模板列表
        $this->tp_category = str_replace($this->filepath . DIRECTORY_SEPARATOR, '', glob($this->filepath . DIRECTORY_SEPARATOR . 'category*'));
        //取得栏目列表模板列表
        $this->tp_list = str_replace($this->filepath . DIRECTORY_SEPARATOR, '', glob($this->filepath . DIRECTORY_SEPARATOR . 'list*'));
        //取得内容页模板列表
        $this->tp_show = str_replace($this->filepath . DIRECTORY_SEPARATOR, '', glob($this->filepath . DIRECTORY_SEPARATOR . 'show*'));
    }

    //模型列表
    public function index()
    {
        if ($this->request->isAjax()) {
            $data = $this->Models->where(['module' => 'cms'])->select();
            return json(["code" => 0, "data" => $data]);
        }
        return $this->fetch();
    }

    //添加模型
    public function add()
    {
        if ($this->request->isPost()) {
            $data = $this->request->post();
            $result = $this->validate($data, 'Models');
            if (true !== $result) {
                return $this->error($result);
            }
            try {
                $this->Models->addModel($data);
            } catch (\Exception $e) {
                $this->error($e->getMessage());
            }
            $this->success('模型新增成功！', url('index'));
        } else {
            $this->assign("tp_category", $this->tp_category);
            $this->assign("tp_list", $this->tp_list);
            $this->assign("tp_show", $this->tp_show);
            return $this->fetch();
        }
    }

    //模型修改
    public function edit()
    {
        if ($this->request->isPost()) {
            $data = $this->request->post();
            $result = $this->validate($data, 'Models');
            if (true !== $result) {
                return $this->error($result);
            }
            try {
                $this->Models->editModel($data);
            } catch (\Exception $e) {
                $this->error($e->getMessage());
            }
            $this->success('模型修改成功！', url('index'));
        } else {
            $id = $this->request->param('id/d', 0);
            $data = $this->Models->where(array("id" => $id))->find();
            $data['setting'] = unserialize($data['setting']);

            $this->assign("tp_category", $this->tp_category);
            $this->assign("tp_list", $this->tp_list);
            $this->assign("tp_show", $this->tp_show);
            $this->assign("data", $data);
            return $this->fetch();
        }
    }

    //模型删除
    public function delete()
    {
        $id = $this->request->param('id/d');
        empty($id) && $this->error('参数不能为空！');
        //检查该模型是否已经被使用
        $r = Db::name("category")->where(array("modelid" => $id))->find();
        if ($r) {
            $this->error("该模型使用中，删除栏目后再删除！");
        }
        //这里可以根据缓存获取表名
        $modeldata = $this->Models->where(array("id" => $id))->find();
        if (!$modeldata) {
            $this->error("要删除的模型不存在！");
        }
        try {
            $this->Models->deleteModel($id);
        } catch (\Exception $e) {
            $this->error($e->getMessage());
        }
        $this->success("删除成功！", url("index"));
    }

    //模型状态
    public function setstate()
    {
        $id = $this->request->param('id/d');
        empty($id) && $this->error('参数不能为空！');
        cache("Model", null);
        $status = $this->request->param('status/d');
        if (Models_Model::update(['status' => $status], ['id' => $id])) {
            $this->success("操作成功！");
        } else {
            $this->error('操作失败！');
        }

    }
}
