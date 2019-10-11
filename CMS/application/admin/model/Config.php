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
// | 后台配置模型
// +----------------------------------------------------------------------
namespace app\admin\model;

use \think\Model;

class Config extends Model
{
    // 自动写入时间戳
    protected $autoWriteTimestamp = true;

    /**
     * 获取配置信息
     * @return mixed
     */
    public function config_cache()
    {
        $data = $this->getConfig();
        cache("Config", $data);
        return $data;
    }

    public static function getConfig($where = "status='1'", $fields = 'name,value,type,options', $order = 'listorder,id desc')
    {
        $configs = self::where($where)->order($order)->column($fields);
        $newConfigs = [];
        foreach ($configs as $key => $value) {
            if ($value['options'] != '') {
                $value['options'] = parse_attr($value['options']);
            }
            switch ($value['type']) {
                case 'array':
                    $newConfigs[$key] = parse_attr($value['value']);
                    break;
                case 'radio':
                    $newConfigs[$key] = isset($value['options'][$value['value']]) ? ['key' => $value['value'], 'value' => $value['options'][$value['value']]] : ['key' => $value['value'], 'value' => $value['value']];
                    break;
                case 'select':
                    $newConfigs[$key] = isset($value['options'][$value['value']]) ? ['key' => $value['value'], 'value' => $value['options'][$value['value']]] : ['key' => $value['value'], 'value' => $value['value']];
                    break;
                case 'checkbox':
                    if (empty($value['value'])) {
                        $newConfigs[$key] = [];
                    } else {
                        $valueArr = explode(',', $value['value']);
                        foreach ($valueArr as $v) {
                            if (isset($value['options'][$v])) {
                                $newConfigs[$key][$v] = $value['options'][$v];
                            } elseif ($v) {
                                $newConfigs[$key][$v] = $v;
                            }
                        }
                    }
                    break;
                case 'image':
                    $newConfigs[$key] = empty($value['value']) ? '' : get_file_path($value['value']);
                    break;
                case 'images':
                    $newConfigs[$key] = empty($value['value']) ? [] : get_file_path($value['value']);
                    if (!is_array($newConfigs[$key])) {
                        $newConfigs[$key] = array($newConfigs[$key]);
                    }
                    break;
                case 'Ueditor':
                    $newConfigs[$key] = htmlspecialchars_decode($value['value']);
                    break;
                default:
                    $newConfigs[$key] = $value['value'];
                    break;
            }
        }
        return $newConfigs;
    }

}
