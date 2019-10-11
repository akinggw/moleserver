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
// | CMS模型
// +----------------------------------------------------------------------
namespace app\cms\model;

use app\cms\model\Category as Category_Model;
use app\common\model\Modelbase;
use think\Db;
use think\facade\Validate;
use \think\Model;

/**
 * 模型
 */
class Cms extends Modelbase
{
    protected $autoWriteTimestamp = true;
    protected $insert = ['status' => 1];
    protected $ext_table = '_data';
    protected $name = 'ModelField';

    /**
     * 根据模型ID，返回表名
     * @param type $modelid
     * @param type $modelid
     * @return string
     */
    protected function getModelTableName($modelid, $ifsystem = 1)
    {
        $model_cache = cache("Model");
        //表名获取
        $model_table = ucwords($model_cache[$modelid]['tablename']);
        //完整表名获取 判断主表 还是副表
        $tablename = $ifsystem ? $model_table : $model_table . $this->ext_table;
        return $tablename;
    }

    //添加模型内容
    public function addModelData($data, $dataExt = [])
    {
        $catid = (int) $data['catid'];
        $modelid = Category_Model::getCategory($catid, 'modelid');
        //完整表名获取
        $tablename = $this->getModelTableName($modelid);
        if (!$this->table_exists($tablename)) {
            throw new \Exception('数据表不存在！');
        }
        //自动提取摘要，如果有设置自动提取，且description为空，且有内容字段才执行
        $this->description($data, $dataExt);

        if (!defined('IN_ADMIN') || (defined('IN_ADMIN') && IN_ADMIN == false)) {
            empty($data['uid']) ? \app\member\service\User::instance()->id : $data['uid'];
            empty($data['username']) ? \app\member\service\User::instance()->username : $data['username'];
            $data['sysadd'] = 0;
        } else {
            //添加用户名
            $data['uid'] = \app\admin\service\User::instance()->id;
            $data['username'] = \app\admin\service\User::instance()->username;
            $data['sysadd'] = 1;
        }
        //处理数据
        $dataAll = $this->dealModelPostData($modelid, $data, $dataExt);
        list($data, $dataExt) = $dataAll;
        if (!isset($data['inputtime'])) {
            $data['inputtime'] = request()->time();
        }
        if (!isset($data['updatetime'])) {
            $data['updatetime'] = request()->time();
        }
        try {
            //主表
            $id = Db::name($tablename)->insertGetId($data);
            //TAG标签处理
            if (!empty($data['tags'])) {
                $this->tag_dispose($data['tags'], $id, $catid, $modelid);
            }
            //附表
            if (!empty($dataExt)) {
                $dataExt['did'] = $id;
                Db::name($tablename . $this->ext_table)->insert($dataExt);
            }
        } catch (\Exception $e) {
            throw new \Exception($e->getMessage());
        }
        //更新栏目统计数据
        $this->update_category_items($catid, 'add', 1);
        return $id;
    }

    //编辑模型内容
    public function editModelData($data, $dataExt = [])
    {
        $catid = (int) $data['catid'];
        $id = (int) $data['id'];
        unset($data['catid']);
        unset($data['id']);
        $modelid = Category_Model::getCategory($catid, 'modelid');
        //完整表名获取
        $tablename = $this->getModelTableName($modelid);
        if (!$this->table_exists($tablename)) {
            throw new \Exception('数据表不存在！');
        }
        //自动提取摘要，如果有设置自动提取，且description为空，且有内容字段才执行
        $this->description($data, $dataExt);
        //TAG标签处理
        if (!empty($data['tags'])) {
            $this->tag_dispose($data['tags'], $id, $catid, $modelid);
        } else {
            $this->tag_dispose([], $id, $catid, $modelid);
        }
        $dataAll = $this->dealModelPostData($modelid, $data, $dataExt);
        list($data, $dataExt) = $dataAll;

        if (!isset($data['updatetime'])) {
            $data['updatetime'] = request()->time();
        }
        //主表
        Db::name($tablename)->where('id', $id)->update($data);
        //附表
        if (!empty($dataExt)) {
            //查询是否存在ID 不存在则新增
            if (Db::name($tablename . $this->ext_table)->where('did', $id)->find()) {
                Db::name($tablename . $this->ext_table)->where('did', $id)->update($dataExt);
            } else {
                $dataExt['did'] = $id;
                Db::name($tablename . $this->ext_table)->insert($dataExt);
            };
        }
        //标签
        hook('contentEditEnd', $data);
    }

    //删除模型内容
    public function deleteModelData($modeId, $id, $no_delete = false)
    {
        $modelInfo = cache('Model');
        $modelInfo = $modelInfo[$modeId];

        $data = Db::name($modelInfo['tablename'])->where('id', $id)->find();
        if (empty($data)) {
            throw new \Exception("该信息不存在！");
        }
        //处理tags
        if (!empty($data['tags'])) {
            $this->tag_dispose([], $data['id'], $data['catid'], $modeId);
        }

        if ($no_delete) {
            Db::name($modelInfo['tablename'])->where('id', $id)->setField('status', -1);
        } else {
            Db::name($modelInfo['tablename'])->where('id', $id)->delete();
            if (2 == $modelInfo['type']) {
                Db::name($modelInfo['tablename'] . $this->ext_table)->where('did', $id)->delete();
            }
            //更新栏目统计
            $this->update_category_items($data['catid'], 'delete');
        }
        //标签
        hook('contentDeleteEnd', $data);
    }

    //处理post提交的模型数据
    protected function dealModelPostData($modeId, $data, $dataExt = [], $ignoreField = [])
    {
        //字段类型
        $query = self::where('modelid', $modeId)->where('status', 1);
        if ([] != $ignoreField) {
            $query = $query->where('name', 'not in', $ignoreField);
        }
        $filedTypeList = $query->order('listorder,id')->column('name,title,type,ifsystem,ifrequire,pattern,errortips');
        //字段规则
        $fieldRule = Db::name('field_type')->column('vrule', 'name');
        foreach ($filedTypeList as $name => $vo) {
            $arr = $vo['ifsystem'] ? 'data' : 'dataExt';
            if (!isset(${$arr}[$name])) {
                switch ($vo['type']) {
                    // 开关
                    case 'switch':
                        ${$arr}[$name] = 0;
                        break;
                    case 'checkbox':
                        ${$arr}[$name] = '';
                        break;
                }
            } else {
                if (is_array(${$arr}[$name])) {
                    ${$arr}[$name] = implode(',', ${$arr}[$name]);
                }
                switch ($vo['type']) {
                    // 开关
                    case 'switch':
                        ${$arr}[$name] = 1;
                        break;
                    // 日期+时间
                    case 'datetime':
                        //if ($vo['ifeditable']) {
                        ${$arr}[$name] = strtotime(${$arr}[$name]);
                        //}
                        break;
                    // 日期
                    case 'date':
                        ${$arr}[$name] = strtotime(${$arr}[$name]);
                        break;
                    // 百度编辑器
                    case 'Ueditor':
                        ${$arr}[$name] = htmlspecialchars(stripslashes(${$arr}[$name]));
                        break;
                }
            }
            //数据必填验证
            if ($vo['ifrequire'] && ${$arr}[$name] == '') {
                throw new \Exception("'" . $vo['title'] . "'必须填写~");
            }
            //正则校验
            if ($vo['pattern'] && !Validate::regex(${$arr}[$name], $vo['pattern'])) {
                throw new \Exception("'" . $vo['title'] . "'" . (!empty($vo['errortips']) ? $vo['errortips'] : '正则校验失败') . "");
            }
            //数据格式验证
            if (!empty($fieldRule[$vo['type']]) && !empty(${$arr}[$name]) && !Validate::{$fieldRule[$vo['type']]}(${$arr}[$name])) {
                throw new \Exception("'" . $vo['title'] . "'格式错误~");
                //安全过滤
            } else {

            }
        }
        return [$data, $dataExt];
    }

    //查询解析模型数据用以构造from表单
    public function getFieldList($modelId, $id = null)
    {

        $list = self::where('modelid', $modelId)->where('status', 1)->order('listorder asc,id asc')->column("name,title,remark,type,isadd,iscore,ifsystem,ifrequire,setting");
        if (!empty($list)) {
            //编辑信息时查询出已有信息
            if ($id) {
                $modelInfo = Db::name('Model')->where('id', $modelId)->field('tablename,type')->find();
                $dataInfo = Db::name($modelInfo['tablename'])->where('id', $id)->find();
                //查询附表信息
                if ($modelInfo['type'] == 2 && !empty($dataInfo)) {
                    $dataInfoExt = Db::name($modelInfo['tablename'] . $this->ext_table)->where('did', $dataInfo['id'])->find();
                }
            }
            foreach ($list as $key => &$value) {
                //内部字段不显示
                if ($value['iscore']) {
                    unset($list[$key]);
                }
                //核心字段做标记
                if ($value['ifsystem']) {
                    $value['fieldArr'] = 'modelField';
                    if (isset($dataInfo[$value['name']])) {
                        $value['value'] = $dataInfo[$value['name']];
                    }
                } else {
                    $value['fieldArr'] = 'modelFieldExt';
                    if (isset($dataInfoExt[$value['name']])) {
                        $value['value'] = $dataInfoExt[$value['name']];
                    }
                }

                //扩展配置
                $value['setting'] = unserialize($value['setting']);
                $value['options'] = $value['setting']['options'];
                //在新增时候添加默认值
                if (!$id) {
                    $value['value'] = $value['setting']['value'];
                }
                if ('' != $value['options']) {
                    $value['options'] = parse_attr($value['options']);
                }
                if ($value['type'] == 'checkbox') {
                    $value['value'] = empty($value['value']) ? [] : explode(',', $value['value']);
                }
                if ($value['type'] == 'datetime') {
                    $value['value'] = empty($value['value']) ? date('Y-m-d H:i:s') : date('Y-m-d H:i:s', $value['value']);
                }
                if ($value['type'] == 'date') {
                    $value['value'] = empty($value['value']) ? '' : date('Y-m-d', $value['value']);
                }

                if ($value['type'] == 'Ueditor') {
                    $value['value'] = htmlspecialchars_decode($value['value']);

                }
            }
        }
        return $list;
    }

    /**
     * 列表页
     * @param   $modeId  [模型ID]
     * @param   $where   [查询条件]
     * @param   $moreifo [是否含附表]
     * @param   $field   []
     * @param   $order   []
     * @param   $limit   [条数]
     * @param   $page    [是否有分页]
     * @param  int|bool  $simple   是否简洁模式或者总记录数
     * @param  array     $config   配置参数
     */
    public function getList($modeId, $where, $moreifo, $field = '*', $order = '', $limit, $page = null, $simple = false, $config = [])
    {
        $tableName = $this->getModelTableName($modeId);
        $result = [];
        if (isset($tableName) && !empty($tableName)) {
            if (2 == getModel($modeId, 'type') && $moreifo) {
                $extTable = $tableName . $this->ext_table;
                if ($page) {
                    $result = Db::view($tableName, '*')
                        ->where($where)
                        ->view($extTable, '*', $tableName . '.id=' . $extTable . '.did', 'LEFT')
                        ->order($order)
                        ->paginate($limit, $simple, $config);
                } else {
                    $result = Db::view($tableName, '*')
                        ->where($where)
                        ->limit($limit)
                        ->view($extTable, '*', $tableName . '.id=' . $extTable . '.did', 'LEFT')
                        ->order($order)
                        ->select();
                }
            } else {
                if ($page) {
                    $result = Db::name($tableName)->where($where)->order($order)->paginate($limit, $simple, $config);
                } else {
                    $result = Db::name($tableName)->where($where)->limit($limit)->order($order)->select();
                }
            }
        }
        //数据格式化处理
        if (!empty($result)) {
            $ModelField = cache('ModelField');
            foreach ($result as $key => $vo) {
                $vo = $this->dealModelShowData($ModelField[$modeId], $vo);
                $vo['url'] = $this->buildContentUrl($vo['catid'], $vo['id']);
                $result[$key] = $vo;
            }
        }
        return $result;
    }

    /**
     * 详情页
     * @param  [type]  $modeId  [模型ID]
     * @param  [type]  $where   [查询条件]
     * @param  boolean $moreifo [是否含附表]
     * @param  string  $field   []
     * @param  string  $order   []
     */
    public function getContent($modeId, $where, $moreifo = false, $field = '*', $order = '', $cache = false)
    {
        $tableName = $this->getModelTableName($modeId);
        if (2 == getModel($modeId, 'type') && $moreifo) {
            $extTable = $tableName . $this->ext_table;
            $dataInfo = Db::view($tableName, '*')->where($where)->cache($cache)->view($extTable, '*', $tableName . '.id=' . $extTable . '.did', 'LEFT')->find();
        } else {
            $dataInfo = Db::name($tableName)->field($field)->cache($cache)->where($where)->find();
        }
        if (!empty($dataInfo)) {
            $ModelField = cache('ModelField');
            $dataInfo = $this->dealModelShowData($ModelField[$modeId], $dataInfo);
            $dataInfo['url'] = $this->buildContentUrl($dataInfo['catid'], $dataInfo['id']);
        }
        return $dataInfo;
    }

    /**
     * 数据处理 前端显示
     * @param  $fieldinfo
     * @param  $data
     */
    protected function dealModelShowData($fieldinfo, $data)
    {
        $newdata = [];
        foreach ($data as $key => $value) {
            switch ($fieldinfo[$key]['type']) {
                case 'array':
                    $newdata[$key] = parse_attr($value);
                    break;
                case 'radio':
                    if (!empty($value)) {
                        if (!empty($fieldinfo[$key]['options'])) {
                            $optionArr = parse_attr($fieldinfo[$key]['options']);
                            $newdata[$key] = isset($optionArr[$value]) ? $optionArr[$value] : $value;
                        }
                    }
                    break;
                case 'select':
                    if (!empty($value)) {
                        if (!empty($fieldinfo[$key]['options'])) {
                            $optionArr = parse_attr($fieldinfo[$key]['options']);
                            $newdata[$key] = isset($optionArr[$value]) ? $optionArr[$value] : $value;
                        }
                    }
                    break;
                case 'checkbox':
                    if (!empty($value)) {
                        if (!empty($fieldinfo[$key]['options'])) {
                            $optionArr = parse_attr($fieldinfo[$key]['options']);
                            $valueArr = explode(',', $value);
                            foreach ($valueArr as $v) {
                                if (isset($optionArr[$v])) {
                                    $newdata[$key][$v] = $optionArr[$v];
                                } elseif ($v) {
                                    $newdata[$key][$v] = $v;
                                }
                            }
                            //其他表关联
                        } else {
                            $newdata[$key] = [];
                        }
                    }
                    break;
                case 'image':
                    $newdata[$key] = empty($value) ? '' : get_file_path($value);
                    break;
                case 'images':
                    $newdata[$key] = empty($value) ? [] : get_file_path($value);
                    if (!is_array($newdata[$key])) {
                        $newdata[$key] = array($newdata[$key]);
                    }
                    break;
                case 'file':
                    $newdata[$key] = empty($value) ? '' : get_file_path($value);
                    break;
                case 'files':
                    $newdata[$key] = empty($value) ? [] : get_file_path($value);
                    break;
                case 'tags':
                    $newdata[$key] = explode(',', $value);
                    break;
                case 'Ueditor':
                    $newdata[$key] = htmlspecialchars_decode($value);
                    break;
                default:
                    $newdata[$key] = $value;
                    break;
            }
            if (!isset($newdata[$key])) {
                $newdata[$key] = '';
            }
        }
        return $newdata;
    }

    /**
     * TAG标签处理
     */
    private function tag_dispose($tags, $id, $catid, $modelid)
    {
        $tags_mode = model('cms/Tags');
        if (!empty($tags)) {
            if (strpos($tags, ',') === false) {
                $keyword = explode(' ', $tags);
            } else {
                $keyword = explode(',', $tags);
            }
            $keyword = array_unique($keyword);
            if ('add' == request()->action()) {
                $tags_mode->addTag($keyword, $id, $catid, $modelid);
            } else {
                $tags_mode->updata($keyword, $id, $catid, $modelid);
            }

        } else {
            //直接清除已有的tags
            $tags_mode->deleteAll($id, $catid, $modelid);
        }
    }

    /**
     * 自动获取简介
     */
    protected function description(&$data, $dataExt)
    {
        //自动提取摘要，如果有设置自动提取，且description为空，且有内容字段才执行
        if ($data['description'] == '' && isset($dataExt['content'])) {
            $content = $dataExt['content'];
            $data['description'] = str_cut(str_replace(array("\r\n", "\t", '&ldquo;', '&rdquo;', '&nbsp;'), '', strip_tags($content)), 200);
        }
    }

    //创建内容链接
    public function buildContentUrl($catid, $id)
    {
        return url('cms/index/shows', ['catid' => $catid, 'id' => $id]);
    }

    private function update_category_items($catid, $action = 'add', $cache = 0)
    {
        if ($action == 'add') {
            Db::name('Category')->where('id', $catid)->setInc('items');
        } else {
            Db::name('Category')->where('id', $catid)->setDec('items');
        }
    }

    //会员配置缓存
    public function cms_cache()
    {
        $data = unserialize(model('admin/Module')->where(array('module' => 'cms'))->value('setting'));
        cache("Cms_Config", $data);
        return $data;
    }

}
