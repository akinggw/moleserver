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
// | 模型模型
// +----------------------------------------------------------------------
namespace app\cms\model;

use app\common\model\Modelbase;
use think\Db;
use think\facade\Config;
use \think\Model;

/**
 * 模型
 */
class Models extends Modelbase
{

    protected $name = 'model';
    protected $ext_table = '_data';
    protected $autoWriteTimestamp = true;

    /**
     * 创建模型
     * @param type $data 提交数据
     * @return boolean
     */
    public function addModel($data, $module = 'cms')
    {
        if (empty($data)) {
            throw new \Exception('数据不得为空！');
        }
        $data['module'] = $module;
        $data['setting'] = serialize($data['setting']);
        //添加模型记录
        if (self::allowField(true)->save($data)) {
            $id = $this->getAttr('id');
            cache("Model", null);
            //创建模型表和模型字段
            if ($this->createTable($data)) {
                $this->addFieldRecord($id, $data['type']);
            }
        }
    }

    /**
     * 编辑模型
     * @param type $data 提交数据
     * @return boolean
     */
    public function editModel($data, $modelid = 0)
    {
        if (empty($data)) {
            throw new \Exception('数据不得为空！');
        }
        //模型ID
        $modelid = $modelid ? $modelid : (int) $data['id'];
        if (!$modelid) {
            throw new \Exception('模型ID不能为空！');
        }
        //查询模型数据
        $info = self::where(array("id" => $modelid))->find();
        if (empty($info)) {
            throw new \Exception('该模型不存在！');
        }
        $data['modelid'] = $modelid;
        $data['setting'] = serialize($data['setting']);

        //是否更改表名
        if ($info['tablename'] != $data['tablename'] && !empty($data['tablename'])) {
            //检查新表名是否存在
            if ($this->table_exists($data['tablename']) || $this->table_exists($data['tablename'] . '_data')) {
                throw new \Exception('该表名已经存在！');
            }
            if (false !== $this->allowField(true)->save($data, array("modelid" => $modelid))) {
                //表前缀
                $dbPrefix = Config::get("database.prefix");
                //表名更改
                Db::execute("RENAME TABLE  `{$dbPrefix}{$info['tablename']}` TO  `{$dbPrefix}{$data['tablename']}` ;");
                //修改副表
                if ($info['type'] == 2) {
                    Db::execute("RENAME TABLE  `{$dbPrefix}{$info['tablename']}_data` TO  `{$dbPrefix}{$data['tablename']}_data` ;");
                }
                //更新缓存
                cache("Model", null);
                return true;
            } else {
                throw new \Exception('模型更新失败！');
            }
        } else {
            if (false !== self::allowField(true)->save($data, array("modelid" => $modelid))) {
                //更新缓存
                cache("Model", null);
                return true;
            } else {
                throw new \Exception('模型更新失败！');
            }
        }
    }

    /**
     * 根据模型ID删除模型
     * @param type $id 模型id
     * @return boolean
     */
    public function deleteModel($id)
    {
        $modeldata = self::where(array("id" => $id))->find();
        if (!$modeldata) {
            throw new \Exception('数据不存在！');
        }
        //删除模型数据
        self::destroy($id);
        //更新缓存
        cache("Model", null);
        //删除所有和这个模型相关的字段
        Db::name("ModelField")->where("modelid", $id)->delete();
        //删除主表
        $table_name = Config::get("database.prefix") . $modeldata['tablename'];
        Db::execute("DROP TABLE IF EXISTS `{$table_name}`");
        if ((int) $modeldata['type'] == 2) {
            //删除副表
            $table_name .= $this->ext_table;
            Db::execute("DROP TABLE IF EXISTS `{$table_name}`");
        }
        return true;
    }

    /**
     * 创建内容模型
     */
    protected function createTable($data)
    {
        $data['tablename'] = strtolower($data['tablename']);
        $table = Config::get("database.prefix") . $data['tablename'];
        if ($this->table_exists($data['tablename'])) {
            throw new \Exception('创建失败！' . $table . '表已经存在~');
        }
        $sql = <<<EOF
				CREATE TABLE `{$table}` (
				`id` mediumint(8) unsigned NOT NULL AUTO_INCREMENT COMMENT '文档ID',
                `catid` smallint(5) unsigned NOT NULL DEFAULT '0' COMMENT '栏目ID',
				`title` varchar(255) COLLATE utf8_unicode_ci NOT NULL DEFAULT '' COMMENT '标题',
                `thumb` int(5) unsigned NOT NULL DEFAULT '0' COMMENT '缩略图',
				`flag` varchar(32) COLLATE utf8_unicode_ci NOT NULL DEFAULT '' COMMENT '属性',
				`keywords` varchar(255) COLLATE utf8_unicode_ci NOT NULL DEFAULT '' COMMENT 'SEO关键词',
				`description` varchar(255) COLLATE utf8_unicode_ci NOT NULL DEFAULT '' COMMENT 'SEO描述',
                `tags` varchar(255) COLLATE utf8_unicode_ci NOT NULL DEFAULT '' COMMENT 'Tags标签',
				`listorder` smallint(5) unsigned NOT NULL DEFAULT '100' COMMENT '排序',
				`uid` mediumint(8) unsigned NOT NULL DEFAULT '0' COMMENT '用户id',
				`username` varchar(20) COLLATE utf8_unicode_ci NOT NULL DEFAULT '' COMMENT '用户名',
				`sysadd` tinyint(1) unsigned NOT NULL DEFAULT '0' COMMENT '是否后台添加',
                `hits` mediumint(8) UNSIGNED DEFAULT 0 COMMENT '点击量' ,
				`inputtime` int(10) unsigned NOT NULL DEFAULT '0'  COMMENT '创建时间',
				`updatetime` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '更新时间',
                `status` tinyint(2) NOT NULL DEFAULT '0' COMMENT '状态',
				PRIMARY KEY (`id`)
				) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='{$data['name']}模型表';
EOF;

        $res = Db::execute($sql);
        if ($data['type'] == 2) {
            // 新建附属表
            $sql = <<<EOF
				CREATE TABLE `{$table}{$this->ext_table}` (
				`did` mediumint(8) unsigned NOT NULL DEFAULT '0',
				`content` text COLLATE utf8_unicode_ci COMMENT '内容',
				PRIMARY KEY (`did`)
				) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='{$data['name']}模型表';
EOF;
            $res = Db::execute($sql);
        }
        return true;
    }

    /**
     * 添加默认字段
     */
    protected function addFieldRecord($modelid, $type)
    {
        $default = [
            'modelid' => $modelid,
            'pattern' => '',
            'errortips' => '',
            'create_time' => request()->time(),
            'update_time' => request()->time(),
            'ifsystem' => 1,
            'status' => 1,
            'listorder' => 100,
            'ifsearch' => 0,
            'iffixed' => 1,
            'remark' => '',
            'isadd' => 0,
            'iscore' => 0,
            'ifrequire' => 0,
        ];
        $data = [
            [
                'name' => 'id',
                'title' => '文档id',
                'type' => 'hidden',
                'isadd' => 1,
            ],
            [
                'name' => 'catid',
                'title' => '栏目id',
                'type' => 'hidden',
                'isadd' => 1,
            ],
            [
                'name' => 'title',
                'title' => '标题',
                'type' => 'text',
                'ifsearch' => 1,
                'ifrequire' => 1,
                'setting' => "a:3:{s:6:\"define\";s:32:\"varchar(255) NOT NULL DEFAULT ''\";s:7:\"options\";s:0:\"\";s:5:\"value\";s:0:\"\";}",
                'isadd' => 1,
            ],
            [
                'name' => 'flag',
                'title' => '属性',
                'type' => 'checkbox',
                'setting' => "a:3:{s:6:\"define\";s:31:\"varchar(32) NOT NULL DEFAULT ''\";s:7:\"options\";s:76:\"1:置顶[1]\r\n2:头条[2]\r\n3:特荐[3]\r\n4:推荐[4]\r\n5:热点[5]\r\n6:幻灯[6]\";s:5:\"value\";s:0:\"\";}",
            ],
            [
                'name' => 'keywords',
                'title' => 'SEO关键词',
                'type' => 'text',
                'iffixed' => 0,
                'remark' => '多关键词之间用空格或者“,”隔开',
                'setting' => "a:3:{s:6:\"define\";s:32:\"varchar(255) NOT NULL DEFAULT ''\";s:7:\"options\";s:0:\"\";s:5:\"value\";s:0:\"\";}",
                'isadd' => 1,
            ],
            [
                'name' => 'description',
                'title' => 'SEO摘要',
                'type' => 'textarea',
                'iffixed' => 0,
                'remark' => '如不填写，则自动截取附表中编辑器的200字符',
                'setting' => "a:3:{s:6:\"define\";s:32:\"varchar(255) NOT NULL DEFAULT ''\";s:7:\"options\";s:0:\"\";s:5:\"value\";s:0:\"\";}",
                'isadd' => 1,
            ],
            [
                'name' => 'tags',
                'title' => 'Tags标签',
                'type' => 'text',
                'iffixed' => 0,
                'remark' => '多关键词之间用空格或者“,”隔开',
                'setting' => "a:3:{s:6:\"define\";s:32:\"varchar(255) NOT NULL DEFAULT ''\";s:7:\"options\";s:0:\"\";s:5:\"value\";s:0:\"\";}",
            ],
            [
                'name' => 'uid',
                'title' => '用户id',
                'type' => 'number',
                'iscore' => 1,
            ],
            [
                'name' => 'username',
                'title' => '用户名',
                'type' => 'text',
                'iscore' => 1,
            ],
            [
                'name' => 'sysadd',
                'title' => '是否后台添加',
                'type' => 'number',
                'iscore' => 1,
            ],
            [
                'name' => 'listorder',
                'title' => '排序',
                'type' => 'number',
                'setting' => "a:3:{s:6:\"define\";s:40:\"tinyint(3) UNSIGNED NOT NULL DEFAULT '0'\";s:7:\"options\";s:0:\"\";s:5:\"value\";s:3:\"100\";}",
            ],
            [
                'name' => 'status',
                'title' => '状态',
                'type' => 'radio',
                'setting' => "a:3:{s:6:\"define\";s:40:\"tinyint(2) UNSIGNED NOT NULL DEFAULT '0'\";s:7:\"options\";s:18:\"0:禁用\r\n1:启用\";s:5:\"value\";s:1:\"1\";}",
            ],
            [
                'name' => 'thumb',
                'title' => '缩略图',
                'type' => 'image',
                'ifrequire' => 0,
                'iffixed' => 0,
                'setting' => "a:3:{s:6:\"define\";s:36:\"int(5) UNSIGNED NOT NULL DEFAULT '0'\";s:7:\"options\";s:0:\"\";s:5:\"value\";s:0:\"\";}",
                'isadd' => 1,
            ],
            [
                'name' => 'inputtime',
                'title' => '创建时间',
                'type' => 'datetime',
                'listorder' => 200,
                'setting' => "a:3:{s:6:\"define\";s:37:\"int(10) UNSIGNED NOT NULL DEFAULT '0'\";s:7:\"options\";s:0:\"\";s:5:\"value\";s:0:\"\";}",
            ],
            [
                'name' => 'updatetime',
                'title' => '更新时间',
                'type' => 'datetime',
                'listorder' => 200,
                'setting' => "a:3:{s:6:\"define\";s:37:\"int(10) UNSIGNED NOT NULL DEFAULT '0'\";s:7:\"options\";s:0:\"\";s:5:\"value\";s:0:\"\";}",
                'iscore' => 1,
            ],
            [
                'name' => 'hits',
                'title' => '点击量',
                'type' => 'number',
                'listorder' => 200,
                'setting' => "a:3:{s:6:\"define\";s:42:\"mediumint(8) UNSIGNED NOT NULL DEFAULT '0'\";s:7:\"options\";s:0:\"\";s:5:\"value\";s:1:\"0\";}",
            ],
        ];
        if ($type == 2) {
            array_push($data, [
                'name' => 'did',
                'title' => '附表文档id',
                'type' => 'hidden',
                'iscore' => 1,
                'ifsystem' => 0,
            ],
                [
                    'name' => 'content',
                    'title' => '内容',
                    'type' => 'Ueditor',
                    'ifsystem' => 0,
                    'iffixed' => 0,
                    'setting' => "a:3:{s:6:\"define\";s:13:\"text NOT NULL\";s:7:\"options\";s:0:\"\";s:5:\"value\";s:0:\"\";}",
                    'isadd' => 1,
                ]);

        }
        foreach ($data as $item) {
            $item = array_merge($default, $item);
            Db::name('model_field')->insert($item);
        }
        return true;
    }

}
