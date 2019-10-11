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
// | 栏目模型
// +----------------------------------------------------------------------
namespace app\cms\model;

use think\facade\Cache;
use \think\Db;
use \think\Model;

/**
 * 模型
 */
class Category extends Model
{
    public $categorys;
    //新增栏目
    public function addCategory($data, $fields)
    {
        if (empty($data)) {
            $this->error = '添加栏目数据不能为空！';
            return false;
        }
        //序列化setting数据
        $data['setting'] = serialize($data['setting']);
        $catid = self::create($data, $fields);
        if ($catid) {
            cache('Category', null);
            return $catid;
        } else {
            $this->error = '栏目添加失败！';
            return false;

        }
    }

    //编辑栏目
    public function editCategory($data, $fields)
    {
        if (empty($data)) {
            $this->error = '编辑栏目数据不能为空！';
            return false;
        }
        $catid = $data['id'];
        //栏目类型
        $data['type'] = (int) $data['type'];

        //查询该栏目是否存在
        $info = self::where(array('id' => $catid))->find();
        if (empty($info)) {
            $this->error = '该栏目不存在！';
            return false;
        }

        //应用模板到所有子栏目
        if ($data['template_child']) {
            $idstr = $this->get_arrchildid($catid);
            if (!empty($idstr)) {
                $arr = self::all($idstr);
                foreach ($arr as $key => $val) {
                    $setting = unserialize($val->getAttr('setting'));
                    $setting['category_template'] = $data['setting']['category_template'];
                    $setting['list_template'] = $data['setting']['list_template'];
                    $setting['show_template'] = $data['setting']['show_template'];
                    $rs = self::where('id', $val->getAttr('id'))->update(['setting' => serialize($setting)]);
                }
            }
        }
        //序列化setting数据
        $data['setting'] = serialize($data['setting']);
        //更新数据
        if (self::update($data, [], $fields) !== false) {
            //更新栏目缓存
            cache('Category', null);
            getCategory($catid, '', true);
            return true;
        } else {
            $this->error = '栏目编辑失败！';
            return false;
        }
    }

    /**
     * 删除栏目
     */
    public function deleteCatid($catid)
    {
        if (!$catid) {
            throw new \Exception("栏目ID不得为空！");
        }
        $catInfo = self::get($catid);
        //是否存在子栏目
        if (self::where('parentid', $catid)->find()) {
            throw new \Exception("栏目含有子栏目，不得删除！");
        }
        /*if ($catInfo['child']) {
        throw new \Exception("栏目含有子栏目，不得删除！");
        }*/
        //检查是否存在数据，存在数据不执行删除
        if ($catInfo['modelid'] && $catInfo['type'] == 2) {
            $tbname = ucwords(getModel($catInfo['modelid'], 'tablename'));
            if ($tbname && Db::name($tbname)->where(['catid' => $catid])->find()) {
                throw new \Exception("栏目含有信息，不得删除！");
            }
        }
        self::where('id', $catid)->delete();
        return true;
        /*if (!$catid) {
    return false;
    }
    $where = array();
    $catInfo = self::get($catid);
    //是否存在子栏目
    if ($catInfo['child']) {
    $arrchildid = explode(",", $catInfo['arrchildid']);
    unset($arrchildid[0]);
    $catid = array_merge($arrchildid, array($catid));
    }
    $where = ['id' => $catid];
    //检查是否存在数据，存在数据不执行删除
    if (is_array($catid)) {
    $modeid = array();
    foreach ($catid as $cid) {
    $catinfo = getCategory($cid);
    if ($catinfo['modelid'] && $catinfo['type'] == 2) {
    $modeid[$catinfo['modelid']] = $catinfo['modelid'];
    }
    foreach ($modeid as $mid) {
    $tbname = ucwords(getModel($mid, 'tablename'));
    if ($tbname && Db::name($tbname)->where(['catid' => $catid])->find()) {
    return false;
    }
    }
    }
    } else {
    $catinfo = getCategory($catid);
    $tbname = ucwords(getModel($catInfo['modelid'], 'tablename'));
    //含资料无法删除
    if ($tbname && $catinfo['type'] == 2 && Db::name($tbname)->where(["catid" => $catid])->find()) {
    return false;
    }
    }
    $status = self::where($where)->delete();
    //更新缓存
    cache('Category', null);
    if (false !== $status) {
    //TD
    return true;
    } else {
    return false;
    }*/
    }

    /**
     *
     * 获取父栏目ID列表
     * @param integer $catid              栏目ID
     * @param array $arrparentid          父目录ID
     * @param integer $n                  查找的层次
     */
    public function get_arrparentid($catid, $arrparentid = '', $n = 1)
    {
        if (empty($this->categorys)) {
            $this->categorys = cache('Category');
        }
        if ($n > 10 || !is_array($this->categorys) || !isset($this->categorys[$catid])) {
            return false;
        }
        //获取当前栏目的上级栏目ID
        $parentid = $this->categorys[$catid]['parentid'];
        //所有父ID
        $arrparentid = $arrparentid ? $parentid . ',' . $arrparentid : $parentid;
        if ($parentid) {
            $arrparentid = $this->get_arrparentid($parentid, $arrparentid, ++$n);
        } else {
            $this->categorys[$catid]['arrparentid'] = $arrparentid;
        }
        //$parentid = $this->categorys[$catid]['parentid'];
        return $arrparentid;
    }

    /**
     *
     * 获取子栏目ID列表
     * @param $catid 栏目ID
     */
    public function get_arrchildid($catid)
    {
        if (!$this->categorys) {
            $this->categorys = cache('Category');
        }
        $arrchildid = $catid;
        if (is_array($this->categorys)) {
            foreach ($this->categorys as $id => $cat) {
                if ($cat['parentid'] && $id != $catid && $cat['parentid'] == $catid) {
                    $arrchildid .= ',' . $this->get_arrchildid($id);
                }
            }
        }
        return $arrchildid;
    }

    /**
     * 获取栏目相关信息
     * @param type $catid 栏目id
     * @param type $field 返回的字段，默认返回全部，数组
     * @param type $newCache 是否强制刷新
     * @return boolean
     */
    public static function getCategory($catid, $field = '', $newCache = false)
    {
        if (empty($catid)) {
            return false;
        }
        $key = 'getCategory_' . $catid;
        //强制刷新缓存
        if ($newCache) {
            Cache::rm($key, null);
        }
        $cache = Cache::get($key);
        if ($cache === 'false') {
            return false;
        }
        if (empty($cache)) {
            //读取数据
            $cache = db('category')->where(['id' => $catid])->find();
            if (empty($cache)) {
                Cache::set($key, 'false', 60);
                return false;
            } else {
                //扩展配置
                $cache['setting'] = unserialize($cache['setting']);
                $cache['url'] = self::buildCatUrl($catid, $cache['url']);
                //栏目扩展字段
                //$cache['extend'] = $cache['setting']['extend'];
                $cache['image'] = empty($cache['image']) ? '' : get_file_path($cache['image']);
                Cache::set($key, $cache, 3600);
            }
        }
        if ($field) {
            //支持var.property，不过只支持一维数组
            if (false !== strpos($field, '.')) {
                $vars = explode('.', $field);
                return $cache[$vars[0]][$vars[1]];
            } else {
                return $cache[$field];
            }
        } else {
            return $cache;
        }
    }

    /**
     * 生成栏目URL
     */
    public static function buildCatUrl($id, $url = '')
    {
        return empty($url) ? url('cms/index/lists', ['catid' => $id]) : ((strpos($url, '://') !== false) ? $url : url($url));
    }

    //刷新栏目索引缓存
    public function category_cache()
    {
        $data = self::order("listorder ASC")->select();
        $CategoryIds = array();
        foreach ($data as $r) {
            $CategoryIds[$r['id']] = array(
                'id' => $r['id'],
                'parentid' => $r['parentid'],
            );
        }
        cache("Category", $CategoryIds);
        return $CategoryIds;
    }

}
