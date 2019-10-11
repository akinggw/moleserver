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
// | Tag模型
// +----------------------------------------------------------------------
namespace app\cms\model;

use \think\Model;

/**
 * Tag模型
 */
class Tags extends Model
{
    protected $autoWriteTimestamp = true;
    /**
     * 添加tags
     * @param type $tagname tags名称 可以是数组
     * @param type $id 信息id
     * @param type $catid 栏目Id
     * @param type $modelid 模型id
     */
    public function addTag($tagname, $id, $catid, $modelid)
    {
        if (!$tagname || !$id || !$catid || !$modelid) {
            return false;
        }
        $time = time();
        $newdata = array();
        if (is_array($tagname)) {
            foreach ($tagname as $v) {
                if (empty($v) || $v == '') {
                    continue;
                }
                if ($this->where(array("tag" => $v))->find()) {
                    $this->where(array("tag" => $v))->setInc('usetimes');
                } else {
                    self::create(array(
                        "tag" => $v,
                        "usetimes" => 1,
                    ));
                }
                $newdata[] = array(
                    'tag' => $v,
                    "modelid" => $modelid,
                    "contentid" => $id,
                    "catid" => $catid,
                    "updatetime" => $time,
                );
            }
            model('cms/TagsContent')->saveAll($newdata);
        }
    }

    /**
     * 根据指定的条件更新tags数据
     * @param type $tagname
     * @param type $id
     * @param type $catid
     * @param type $modelid
     */
    public function updata($tagname, $id, $catid, $modelid)
    {
        if (!$tagname || !$id || !$catid || !$modelid) {
            return false;
        }
        $tags = model("TagsContent")->where(array(
            "modelid" => $modelid,
            "contentid" => $id,
            "catid" => $catid,
        ))->select();
        foreach ($tags as $key => $val) {
            if (!$val) {
                continue;
            }
            //如果在新的关键字数组找不到，说明已经去除
            if (!in_array($val['tag'], $tagname)) {
                //删除不存在的tag
                $this->deleteTagName($val['tag'], $id, $catid, $modelid);
            } else {
                foreach ($tagname as $k => $v) {
                    if ($val['tag'] == $v) {
                        unset($tagname[$k]);
                    }
                }
            }
        }
        //新增的tags
        if (count($tagname) > 0) {
            $this->addTag($tagname, $id, $catid, $modelid, $data);
        }
    }

    /**
     * 删除tag
     * @param type $tagname
     * @param type $id
     * @param type $catid
     * @param type $modelid
     */
    public function deleteTagName($tagname, $id, $catid, $modelid)
    {
        if (!$id || !$catid || !$modelid || !$tagname) {
            return false;
        }
        $db_tags_content = model("TagsContent");
        if (is_array($tagname)) {
            foreach ($tagname as $name) {
                $r = $this->where(array("tag" => $name))->find();
                if ($r) {
                    if ($r['usetimes'] > 0) {
                        $this->where(array("tag" => $name))->setDec('usetimes');
                    }
                    //删除tags数据
                    $db_tags_content->where(array("tag" => $name, 'contentid' => $id, "catid" => $catid))->delete();
                }
            }
        } else {
            $r = $this->where(array("tag" => $tagname))->find();
            if ($r) {
                if ($r['usetimes'] > 0) {
                    $this->where(array("tag" => $tagname))->setDec('usetimes');
                }
                //删除tags数据
                $db_tags_content->where(array("tag" => $r['tag'], 'contentid' => $id, "catid" => $catid))->delete();
            }
        }
        return true;
    }

    /**
     * 根据信息id删除全部的tags记录
     * @param type $id
     * @param type $catid
     * @param type $modelid
     * @return boolean
     */
    public function deleteAll($id, $catid, $modelid)
    {
        if (!$id || !$catid || !$modelid) {
            return false;
        }
        $db_tags_content = model("cms/TagsContent");
        $where = array('modelid' => $modelid, 'contentid' => $id, "catid" => $catid);
        //取得对应tag数据
        $tagslist = $db_tags_content->where($where)->select();
        if (empty($tagslist)) {
            return true;
        }
        //全部-1
        foreach ($tagslist as $k => $value) {
            $this->where(array("tag" => $value['tag']))->setDec('usetimes');
        }
        //删除tags数据
        $db_tags_content->where($where)->delete();
        return true;
    }

}
