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
// | 表单后台管理
// +----------------------------------------------------------------------
namespace app\formguide\controller;

use app\common\controller\Adminbase;
use app\formguide\model\Models as Models_Model;
use think\Db;

class Formguide extends Adminbase
{
    //模板存放目录
    protected $filepath, $tpl;
    protected function initialize()
    {
        parent::initialize();
        //模块安装后，模板安装在Default主题下！
        $this->filepath = TEMPLATE_PATH . (empty(config('theme')) ? "default" : config('theme')) . DIRECTORY_SEPARATOR . "formguide" . DIRECTORY_SEPARATOR;
        $this->Models = new Models_Model;
    }

    //首页
    public function index()
    {
        if ($this->request->isAjax()) {
            $data = $this->Models->where(['module' => 'formguide'])->select();
            return json(["code" => 0, "data" => $data]);
        } else {
            return $this->fetch();
        }
    }

    //添加表单
    public function add()
    {
        if ($this->request->isPost()) {
            $data = $this->request->post();
            $result = $this->validate($data, 'Models');
            if (true !== $result) {
                return $this->error($result);
            }
            try {
                $this->Models->addModelFormguide($data);
            } catch (\Exception $e) {
                $this->error($e->getMessage());
            }
            $this->success('模型新增成功！', url('index'));
        } else {
            $this->tpl = str_replace($this->filepath, "", glob($this->filepath . 'show*'));
            $this->tpl = str_replace("." . config("template.view_suffix"), "", $this->tpl);
            foreach ($this->tpl as $v) {
                $show_template[$v] = $v;
            }
            $this->assign('show_template', $show_template);
            return $this->fetch();
        }
    }

    //编辑表单
    public function edit()
    {
        if ($this->request->isPost()) {
            $data = $this->request->post();
            unset($data['type'], $data['tablename']);
            $result = $this->validate($data, 'Models.edit');
            if (true !== $result) {
                return $this->error($result);
            }
            $data['setting'] = serialize($data['setting']);
            if ($this->Models->save($data, ['id' => (int) $data['id']]) !== false) {
                $this->success("更新模型成功！", url("index"));
            } else {
                $this->error("更新失败！");
            }
        } else {
            $id = $this->request->param('id/d', 0);
            $r = $this->Models->where(array("id" => $id))->find();
            if (!$r) {
                $this->error("该表单不存在！");
            }
            $r['setting'] = unserialize($r['setting']);
            $r['tablename'] = str_replace("form_", "", $r['tablename']);

            $this->tpl = str_replace($this->filepath, "", glob($this->filepath . 'show*'));
            $this->tpl = str_replace("." . config("template.view_suffix"), "", $this->tpl);
            foreach ($this->tpl as $v) {
                $show_template[$v] = $v;
            }
            $this->assign('show_template', $show_template);
            $this->assign('data', $r);
            return $this->fetch();
        }
    }

    //表单删除
    public function delete()
    {
        $id = $this->request->param('id/d');
        empty($id) && $this->error('参数不能为空！');
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

}
