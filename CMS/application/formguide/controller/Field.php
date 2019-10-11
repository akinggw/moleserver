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
// | 表单字段后台管理
// +----------------------------------------------------------------------
namespace app\formguide\controller;

use app\common\controller\Adminbase;
use app\formguide\model\ModelField as Model_Field;
use think\Db;
use think\facade\Cookie;

class Field extends AdminBase
{
    public $fields, $banfie;
    //初始化
    protected function initialize()
    {
        parent::initialize();
        //允许使用的字段列表
        $this->banfie = array("text", "checkbox", "textarea", "radio", "select", "image", "number", "Ueditor", "color", "file");
        $this->modelfield = new Model_Field;
    }

    //首页
    public function index()
    {
        // 记录当前列表页的cookie
        Cookie::set('__forward__', $_SERVER['REQUEST_URI']);
        $fieldid = $this->request->param('id/d', 0);
        if ($this->request->isAjax()) {
            $data = $this->modelfield->where(['modelid' => $fieldid])->order('listorder,id')->select();
            return json(["code" => 0, "data" => $data]);
        } else {
            $this->assign("id", $fieldid);
            return $this->fetch();
        }
    }

    //添加
    public function add()
    {
        if ($this->request->isPost()) {
            //增加字段
            $data = $this->request->post();
            $result = $this->validate($data, 'ModelField');
            if (true !== $result) {
                return $this->error($result);
            }
            try {
                $res = $this->modelfield->addField($data);
            } catch (\Exception $e) {
                $this->error($e->getMessage());
            }
            $this->success('新增成功', Cookie::get('__forward__'));
        } else {
            $fieldid = $this->request->param('id/d', 0);
            $fieldType = Db::name('field_type')->where('name', 'in', $this->banfie)->order('listorder')->column('name,title,default_define,ifoption,ifstring');
            $modelInfo = Db::name('model')->where('id', $fieldid)->find();
            $this->assign(
                [
                    'modelType' => $modelInfo['type'],
                    "modelid" => $fieldid,
                    'fieldType' => $fieldType,
                ]
            );
            return $this->fetch();
        }

    }

    //编辑
    public function edit()
    {
        $fieldid = $this->request->param('id/d', 0);
        if ($this->request->isPost()) {
            $data = $this->request->post();
            $result = $this->validate($data, 'ModelField');
            if (true !== $result) {
                return $this->error($result);
            }
            try {
                $this->modelfield->editField($data, $fieldid);
            } catch (\Exception $e) {
                $this->error($e->getMessage());
            }
            $this->success("更新成功！", Cookie::get('__forward__'));
        } else {
            //字段信息
            $fieldData = Model_Field::get($fieldid);
            //字段扩展配置
            $fieldData['setting'] = unserialize($fieldData['setting']);
            if (empty($fieldData)) {
                $this->error('该字段信息不存在！');
            }
            //模型信息
            $modedata = Db::name('model')->where('id', $fieldData->getAttr('modelid'))->find();
            if (empty($modedata)) {
                $this->error('该模型不存在！');
            }
            $fieldType = Db::name('field_type')->where('name', 'in', $this->banfie)->order('listorder')->column('name,title,default_define,ifoption,ifstring');
            $this->assign([
                'data' => $fieldData,
                'fieldid' => $fieldid,
                'fieldType' => $fieldType,
            ]);
            return $this->fetch();
        }
    }

    //删除
    public function delete()
    {
        $fieldid = $this->request->param('id/d', 0);
        if (empty($fieldid)) {
            $this->error('字段ID不能为空！');
        }
        try {
            $this->modelfield->deleteField($fieldid);
        } catch (\Exception $e) {
            $this->error($e->getMessage());
        }
        $this->success("字段删除成功！");
    }

    /**
     * 排序
     */
    public function listorder()
    {
        $id = $this->request->param('id/d', 0);
        $listorder = $this->request->param('value/d', 0);
        $rs = Model_Field::update(['listorder' => $listorder], ['id' => $id], true);
        if ($rs) {
            $this->success("排序成功！");
        } else {
            $this->error("排序失败！");
        }
    }

}
