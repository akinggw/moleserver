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
// | tags管理
// +----------------------------------------------------------------------
namespace app\cms\controller;

use app\cms\model\Tags as Tags_Model;
use app\common\controller\Adminbase;
use think\Db;

class Tags extends Adminbase
{
    protected function initialize()
    {
        parent::initialize();
        $this->Tags = new Tags_Model;
    }

    /**
     * tags列表
     */
    public function index()
    {
        if ($this->request->isAjax()) {
            $limit = $this->request->param('limit/d', 10);
            $page = $this->request->param('page/d', 10);
            $_list = $this->Tags->order(['listorder', 'id' => 'desc'])->page($page, $limit)->select();
            foreach ($_list as $k => &$v) {
                $v['url'] = url('cms/index/tags', ['tag' => $v['tag']]);
            }
            unset($v);

            $total = $this->Tags->count();
            $result = array("code" => 0, "count" => $total, "data" => $_list);
            return json($result);
        }
        return $this->fetch();

    }

    /**
     * tags编辑
     */
    public function edit()
    {
        if ($this->request->isPost()) {
            $data = $this->request->post();
            if ($this->Tags->save($data, ['id' => $data['tagid']]) !== false) {
                if ($data['oldtagsname'] != $data['tag']) {
                    model('TagsContent')->save(['tag' => $data['tag']], ['tag' => $data['oldtagsname']]);
                }
                $this->success('修改成功！');
            }
            $this->success('修改失败！');

        } else {
            $id = $this->request->param('id/d', 0);
            if (empty($id)) {
                $this->error('请指定需要修改的tags！');
            }
            $data = Tags_Model::get($id);
            $this->assign('data', $data);
            return $this->fetch();
        }

    }

    /**
     * tags删除
     */
    public function delete()
    {
        $tagid = $this->request->param('ids/a', null);
        if (!is_array($tagid)) {
            $tagid = array($tagid);
        }
        foreach ($tagid as $tid) {
            $info = $this->Tags->where(array('id' => $tid))->find();
            if (!empty($info)) {
                if ($this->Tags->where(array('tag' => $info['tag']))->delete() !== false) {
                    model('TagsContent')->where(array('tag' => $info['tag']))->delete();
                }
            }
        }
        $this->success("删除成功！");
    }

    /**
     * tags排序
     */
    public function listorder()
    {
        $id = $this->request->param('id/d', 0);
        $listorder = $this->request->param('value/d', 0);
        $rs = $this->Tags->allowField(['listorder'])->isUpdate(true)->save(['id' => $id, 'listorder' => $listorder]);
        if ($rs) {
            $this->success("菜单排序成功！");
        } else {
            $this->error("菜单排序失败！");
        }
    }

    //tags数据重建
    public function create()
    {
        if ($this->request->isPOST() || $this->request->param('modelid/d')) {
            $modelid = $this->request->param('modelid/d', 0);
            $_GET['modelid'] = $modelid;
            $lun = $this->request->param('lun/d', 0); //第几轮 0=>1
            $_GET['zlun'] = $zlun = $this->request->param('zlun/d', 0); //总轮数
            $_GET['mo'] = $this->request->param('mo/d'); //模型id
            if ($lun > (int) $_GET['zlun'] - 1) {
                $lun = (int) $_GET['zlun'] - 1;
            }
            $lun = $lun < 0 ? 0 : $lun;
            $mlun = 100;
            $firstRow = $mlun * ($lun < 0 ? 0 : $lun);
            //$db = Db::name('TagsContent');
            //$tagdb = Db::name('Tags');
            if (0 !== (int) $this->request->param('delete/d')) {
                //清空
                Db::name('TagsContent')->delete(true);
                Db::name('Tags')->delete(true);
            }
            unset($_GET['delete']);
            $model = cache('Model');
            if (1 == (int) $_GET['mo']) {

            } else {
                //模型总数
                $_GET['mocount'] = 1;
            }
            //当全部模型重建时处理
            if ($modelid == 0) {
                $modelCONUT = Db::name("Model")->count();
                $modelDATA = Db::name("Model")->where('type', 2)->order('id', 'ASC')->find();
                $modelid = $modelDATA['id'];
                $_GET['mo'] = 1;
                $_GET['mocount'] = $modelCONUT;
                $_GET['modelid'] = $modelid;
            }
            $models_v = $model[$modelid];
            if (!is_array($models_v)) {
                $this->error("该模型不存在！");
            }
            $tableName = $models_v['tablename'];
            $count = Db::name($tableName)->count();
            if ($count == 0) {
                //结束
                if (isset($_GET['mo'])) {
                    $modelDATA = Db::name('Model')->where([
                        ['type', '=', 2],
                        ['id', '>', $modelid],
                    ])->order('id', 'ASC')->find();
                    if (!$modelDATA) {
                        $this->success('Tags重建结束！', url('index'));
                    }
                    unset($_GET['zlun']);
                    unset($_GET['lun']);
                    $modelid = $modelDATA['id'];
                    $_GET['modelid'] = $modelid;
                    $this->success("模型：{$models_v['name']}，第 " . ($lun + 1) . "/{$zlun} 轮更新成功，进入下一轮更新中...", url('create', $_GET), '', 1);
                } else {
                    $this->error('该模型下没有信息！');
                }
            }
            //总轮数
            $zlun = ceil($count / $mlun);
            $_GET['zlun'] = $zlun;
            $this->createUP($models_v, $firstRow, $mlun);
            if ($lun == (int) $_GET['zlun'] - 1) {
                if (isset($_GET['mo'])) {
                    $modelDATA = Db::name('Model')->where([
                        ['type', '=', 2],
                        ['id', '>', $modelid],
                    ])->order('id', 'ASC')->find();
                    if (!$modelDATA) {
                        $this->success("Tags重建结束！", url('index'));
                        exit;
                    }
                    unset($_GET['zlun']);
                    unset($_GET['lun']);
                    $modelid = $modelDATA['id'];
                    $_GET['modelid'] = $modelid;
                } else {
                    $this->success("Tags重建结束！", url('index'));
                }
            } else {
                $_GET['lun'] = $lun + 1;
            }
            $this->success("模型：" . $models_v['name'] . "，第 " . ($lun + 1) . "/$zlun 轮更新成功，进入下一轮更新中...", url('create', $_GET), '', 1);
        } else {
            $model = cache('Model');
            $mo = array();
            foreach ($model as $k => $v) {
                if ($v['type'] == 2) {
                    $mo[$k] = $v['name'];
                }
            }
            $this->assign('Model', $mo);
            return $this->fetch();

        }
    }

    //数据重建
    protected function createUP($models_v, $firstRow, $mlun)
    {
        $keywords = Db::name(ucwords($models_v['tablename']))->where([
            ["status", '=', 1],
            ['tags', '<>', ''],
        ])->order("id", "ASC")->limit($firstRow, $mlun)->column('id,catid,tags');
        foreach ($keywords as $keyword) {
            $data = array();
            $time = time();
            $key = strpos($keyword['tags'], ',') !== false ? explode(',', $keyword['tags']) : explode(' ', $keyword['tags']);
            foreach ($key as $key_v) {
                if (empty($key_v) || $key_v == "") {
                    continue;
                }
                $key_v = trim($key_v);
                if ($this->Tags->where('tag', $key_v)->find()) {
                    $this->Tags->where('tag', $key_v)->setInc('usetimes');
                } else {
                    $this->Tags->insert(array(
                        "tag" => $key_v,
                        "usetimes" => 1,
                        "create_time" => $time,
                        "update_time" => $time,
                    ));
                }
                $data = array(
                    'tag' => $key_v,
                    "modelid" => $models_v['id'],
                    "contentid" => $keyword['id'],
                    "catid" => $keyword['catid'],
                    "updatetime" => $time,
                );
                Db::name('TagsContent')->insert($data);
            }
        }
        return true;
    }

}
