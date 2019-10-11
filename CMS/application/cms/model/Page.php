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
// | 单页模型
// +----------------------------------------------------------------------
namespace app\cms\model;

use \think\Model;

/**
 * 模型
 */
class Page extends Model
{
    protected $pk = 'catid';

    /**
     * 根据栏目ID获取内容
     * @param type $catid 栏目ID
     * @return boolean
     */
    public function getPage($catid, $cache = false)
    {
        if (empty($catid)) {
            return false;
        }
        $list = self::get($catid, 10);
        return $list;

    }

    /**
     * 更新单页内容
     * @param type $post 表单数据
     * @return boolean
     */
    public function savePage($data)
    {
        if (empty($data)) {
            $this->error = '内容不能为空！';
            return false;
        }
        $catid = $data['catid'];
        $info = self::get($catid);
        if ($info) {
            //更新
            self::update($data, [], true);
            return true;
        } else {
            //新增
            self::create($data, true);
            return true;
        }
        $this->error = '操作失败！';
        return false;

    }

}
