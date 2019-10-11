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
// | cms函数文件
// +----------------------------------------------------------------------

use app\cms\model\Category as Category_Model;
use think\facade\Cache;
use think\facade\Request;

/**
 * 获取栏目相关信息
 * @param type $catid 栏目id
 * @param type $field 返回的字段，默认返回全部，数组
 * @param type $newCache 是否强制刷新
 * @return boolean
 */
function getCategory($catid, $field = '', $newCache = false)
{
    return Category_Model::getCategory($catid, $field, $newCache);
}

/**
 * 当前路径
 * 返回指定栏目路径层级
 * @param $catid 栏目id
 * @param $symbol 栏目间隔符
 */
function catpos($catid, $symbol = ' &gt; ')
{
    if (getCategory($catid) == false) {
        return '';
    }
    //获取当前栏目的 父栏目列表
    $arrparentid = array_filter(explode(',', getCategory($catid, 'arrparentid') . ',' . $catid));
    foreach ($arrparentid as $cid) {
        //$url = Category_Model::buildCatUrl($cid, getCategory($cid, 'url'));
        $parsestr[] = '<a href="' . getCategory($cid, 'url') . '" >' . getCategory($cid, 'catname') . '</a>';
    }
    $parsestr = implode($symbol, $parsestr);
    return $parsestr;
}

/**
 * 生成分类信息中的筛选菜单
 */
function filters($modelid, $catid)
{
    $data = get_filters_field($modelid);
    Request::filter('trim');
    $param = paramdecode(Request::param('condition'));

    //$catid = Request::param('catid');
    $conditionParam = [];
    foreach ($data as $name => $rs) {
        $all[0] = '不限';
        $data[$name]['options'] = array_merge($all, $data[$name]['options']);
        //判断是否是单选条件
        $ifradio = 'checkbox' == $data[$name]['type'] ? false : true;
        if ($ifradio) {
            //单选选中参数
            if (!empty($param[$name])) {
                $conditionParam[$name]['options'][$param[$name]]['active'] = true;
                $nowParam = $param;
                $nowParam[$name] = '';
                $conditionParam[$name]['options'][$param[$name]]['param'] = paramencode($nowParam);
                unset($nowParam);
            }
        } else {
            //多选选中参数
            if (!empty($param[$name])) {
                $paramContent = explode('_', $param[$name]);
                foreach ($paramContent as $k => $v) {
                    $nowParamContent = $paramContent;
                    unset($nowParamContent[$k]);
                    $nowParam = $param;
                    $nowParam[$name] = implode('_', $nowParamContent);
                    $conditionParam[$name]['options'][$v]['active'] = true;
                    $conditionParam[$name]['options'][$v]['param'] = paramencode($nowParam);
                    unset($nowParam);
                    unset($nowParamContent);
                }
                unset($paramContent);
            }
        }
        $conditionParam[$name]['title'] = $rs['title'];
        $conditionParam[$name]['name'] = $rs['name'];
        //未选中 active param title url
        foreach ($data[$name]['options'] as $k => $v) {
            $conditionParam[$name]['options'][$k]['title'] = $v;
            //未选中条件参数生成
            if (!isset($conditionParam[$name]['options'][$k]['active'])) {
                //未选中条件参数生成
                $conditionParam[$name]['options'][$k]['active'] = 0;
                if ($ifradio) {
                    $nowParam = $param;
                    $nowParam[$name] = $k;
                    $conditionParam[$name]['options'][$k]['param'] = paramencode($nowParam);
                } else {
                    $nowParam = $param;
                    $nowParam[$name] = empty($param[$name]) ? $k : $param[$name] . '_' . $k;
                    $conditionParam[$name]['options'][$k]['param'] = paramencode($nowParam);
                }
            }
            $conditionParam[$name]['options'][$k]['url'] = url('cms/index/lists', ['catid' => $catid, 'condition' => $conditionParam[$name]['options'][$k]['param']]);
            ksort($conditionParam[$name]['options']);
        }
        if (!isset($param[$rs['name']]) && empty($param[$rs['name']])) {
            $conditionParam[$name]['options'][0]['active'] = true;
        }
    }
    return $conditionParam;
}

function structure_filters_sql($modelid)
{
    $data = get_filters_field($modelid);
    $fields_key = array_keys($data);
    $sql = '`status` = \'1\'';
    $param = paramdecode(Request::param('condition'));
    foreach ($param as $k => $r) {
        if (isset($data[$k]['type']) && in_array($k, $fields_key) && intval($r) != 0) {
            if ('radio' == $data[$k]['type']) {
                $sql .= " AND `$k` = '$r'";
            } elseif ('checkbox' == $data[$k]['type']) {
                $r = str_replace('_', ',', $r);
                $sql .= " AND $k in('$r')";
            }
        }
    }
    return $sql;
}

function get_filters_field($modelid)
{

    static $filters_data = [];
    if ($filters_data) {
        return $filters_data;
    }
    $options = cache('ModelField')[$modelid];
    foreach ($options as $_k => $_v) {
        if (isset($_v['filtertype']) && $_v['filtertype']) {
            $_v['options'] = parse_attr($_v['options']);
        } else {
            continue;
        }
        $filters_data[$_v['name']] = $_v;
    }
    return $filters_data;

}

function paramdecode($str)
{
    $arr = [];
    $arr1 = explode('&', $str);
    foreach ($arr1 as $vo) {
        if (!empty($vo)) {
            $arr2 = explode('=', $vo);
            if (!empty($arr2[1])) {
                $arr[$arr2[0]] = $arr2[1];
            }
        }
    }
    return $arr;
}

function paramencode($arr)
{
    $str = '';
    if (!empty($arr)) {
        foreach ($arr as $key => $vo) {
            if (!empty($vo)) {
                $str .= $key . '=' . $vo . '&';
            }
        }
        $str = substr($str, 0, -1);
    }
    return $str;
}

/**
 * 生成SEO
 * @param $catid        栏目ID
 * @param $title        标题
 * @param $description  描述
 * @param $keyword      关键词
 */
function seo($catid = '', $title = '', $description = '', $keyword = '')
{
    if (!empty($title)) {
        $title = strip_tags($title);
    }
    if (!empty($description)) {
        $description = strip_tags($description);
    }
    if (!empty($keyword)) {
        $keyword = str_replace(' ', ',', strip_tags($keyword));
    }
    $site = cache("Cms_Config");
    if (!empty($catid)) {
        $cat = getCategory($catid);
    }
    $seo['site_title'] = isset($site['site_title']) && !empty($site['site_title']) ? $site['site_title'] : $site['site_name'];
    $seo['keyword'] = !empty($keyword) ? $keyword : $site['site_keyword'];
    $seo['description'] = isset($description) && !empty($description) ? $description : (isset($cat['setting']['meta_description']) && !empty($cat['setting']['meta_description']) ? $cat['setting']['meta_description'] : (isset($site['site_description']) && !empty($site['site_description']) ? $site['site_description'] : ''));
    $seo['title'] = (isset($title) && !empty($title) ? $title . ' - ' : '') . (isset($cat['setting']['meta_title']) && !empty($cat['setting']['meta_title']) ? $cat['setting']['meta_title'] . ' - ' : (isset($cat['catname']) && !empty($cat['catname']) ? $cat['catname'] . ' - ' : ''));
    foreach ($seo as $k => $v) {
        $seo[$k] = str_replace(array("\n", "\r"), '', $v);
    }
    return $seo;
}
