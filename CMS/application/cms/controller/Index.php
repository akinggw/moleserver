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
// | cms管理
// +----------------------------------------------------------------------
namespace app\cms\controller;

use app\cms\controller\Homebase;
use app\cms\model\Cms as Cms_Model;
use think\Db;

class Index extends Cmsbase
{
    protected function initialize()
    {
        parent::initialize();
        $this->Cms_Model = new Cms_Model;
    }

    /**
     * 首页
     */
    public function index()
    {
        $page = $this->request->param('page/d', 1);
        $seo = seo();
        $this->assign([
            'SEO' => $seo,
            'page' => $page,
        ]);
        return $this->fetch('/index');
    }

    /**
     * 列表页
     */
    public function lists()
    {
        //栏目ID
        $catid = $this->request->param('catid/d', 0);
        $page = $this->request->param('page/d', 1);
        //获取栏目数据
        $category = getCategory($catid);
        if (empty($category)) {
            $this->error('栏目不存在！');
        }
        //模型ID
        $modelid = $category['modelid'];
        $models = cache('Model');

        //栏目扩展配置信息
        $setting = $category['setting'];
        //类型为列表的栏目
        if ($category['type'] == 2) {
            //栏目首页模板
            $template = $setting['category_template'] ? $setting['category_template'] : 'category';
            //栏目列表页模板
            $template_list = $setting['list_template'] ? $setting['list_template'] : 'list';
            //判断使用模板类型，如果有子栏目使用频道页模板
            $template = $category['child'] ? "{$template}" : "{$template_list}";
            $tpar = explode(".", $template, 2);
            //去除完后缀的模板
            $template = $tpar[0];
            unset($tpar);
            $seo = seo($catid, $setting['meta_title'], $setting['meta_description'], $setting['meta_keywords']);
            //单页
        } else if ($category['type'] == 1) {
            $template = $setting['page_template'] ? $setting['page_template'] : 'page';
            //判断使用模板类型，如果有子栏目使用频道页模板，终极栏目使用的是列表模板
            $template = "{$template}";
            //去除后缀
            $tpar = explode(".", $template, 2);
            $template = $tpar[0];
            unset($tpar);
            $ifcache = $this->cmsConfig['site_cache_time'] ? $this->cmsConfig['site_cache_time'] : false;
            $info = model('Page')->getPage($catid, $ifcache);
            if ($info) {
                $info = $info->toArray();
            }
            //SEO
            $keywords = $info['keywords'] ? $info['keywords'] : $setting['meta_keywords'];
            $description = $info['description'] ? $info['description'] : $setting['meta_description'];
            $seo = seo($catid, $setting['meta_title'], $description, $keywords);
            $this->assign($info);
        }
        //获取顶级栏目ID
        $arrparentid = explode(',', $category['arrparentid']);
        $top_parentid = isset($arrparentid[1]) ? $arrparentid[1] : $catid;
        $this->assign([
            'top_parentid' => $top_parentid,
            'arrparentid' => $arrparentid,
            'SEO' => $seo,
            'catid' => $catid,
            'page' => $page,
            'modelid' => $modelid,
        ]);
        return $this->fetch('/' . $template);

    }

    /**
     * 内容页
     */
    public function shows()
    {
        //ID
        $id = $this->request->param('id/d', 0);
        //栏目ID
        $catid = $this->request->param('catid/d', 0);
        $page = $page = $this->request->param('page/d', 1);
        //获取栏目数据
        $category = getCategory($catid);
        if (empty($category)) {
            $this->error('栏目不存在！');
        }
        //模型ID
        $modelid = $category['modelid'];
        $modelInfo = cache('Model')[$modelid];
        if (empty($modelInfo)) {
            throw new \think\Exception('栏目不存在!', 404);
        }
        //更新点击量
        Db::name($modelInfo['tablename'])->where('id', $id)->setInc('hits');
        //内容所有字段
        $ifcache = $this->cmsConfig['site_cache_time'] ? $this->cmsConfig['site_cache_time'] : false;
        $info = $this->Cms_Model->getContent($modelid, "id='" . $id . "' and status='1'", true, '*', '', $ifcache);
        if (empty($info)) {
            throw new \think\Exception('内容不存在或未审核!', 404);
        }
        //栏目扩展配置信息
        $setting = $category['setting'];
        //内容页模板
        $template = $setting['show_template'] ? $setting['show_template'] : 'show';
        //去除模板文件后缀
        $newstempid = explode(".", $template);
        $template = $newstempid[0];
        unset($newstempid);
        //阅读收费
        $paytype = isset($info['paytype']) && $info['paytype'] == '金钱' ? 1 : 0; //类型 0积分 1金钱
        $readpoint = isset($info['readpoint']) ? (int) $info['readpoint'] : 0; //金额
        $allow_visitor = 1;
        if ($readpoint) {
            //检查是否支付过
            $allow_visitor = self::_check_payment($catid . '_' . $id, $paytype);
            if (!$allow_visitor) {
                //$http_referer = urlencode(\think\facade\Request::url(true));
                $allow_visitor = sys_auth($catid . '_' . $id . '|' . $readpoint . '|' . $paytype);
            } else {
                $allow_visitor = 1;
            }
        }
        //SEO
        $keywords = $info['keywords'] ? $info['keywords'] : $setting['meta_keywords'];
        $title = $info['title'] ? $info['title'] : $setting['meta_title'];
        $description = $info['description'] ? $info['description'] : $setting['meta_description'];
        $seo = seo($catid, $title, $description, $keywords);
        //获取顶级栏目ID
        $arrparentid = explode(',', $category['arrparentid']);
        $top_parentid = isset($arrparentid[1]) ? $arrparentid[1] : $catid;
        $this->assign($info);
        $this->assign([
            'paytype' => $paytype,
            'readpoint' => $readpoint,
            'allow_visitor' => $allow_visitor,
            'top_parentid' => $top_parentid,
            'arrparentid' => $arrparentid,
            'SEO' => $seo,
            'catid' => $catid,
            'page' => $page,
            'modelid' => $modelid,
        ]);
        return $this->fetch('/' . $template);
    }

    /**
     * 搜索
     */
    public function search()
    {
        $seo = seo('', '搜索结果');
        //模型
        $modelid = $this->request->param('modelid/d', 0);
        //关键词
        $keyword = $this->request->param('keyword/s', '', 'trim,safe_replace,strip_tags,htmlspecialchars');
        $keyword = str_replace('%', '', $keyword); //过滤'%'，用户全文搜索
        //时间范围
        $time = $this->request->param('time/s', '');

        $result = $this->validate([
            'keyword' => $keyword,
        ], [
            'keyword|标题关键词' => 'chsDash|max:25',
        ]);
        if (true !== $result) {
            $this->error($result);
        }
        debug('begin');
        //按时间搜索
        if ($time == 'day') {
            $search_time = time() - 86400;
            $sql_time = ' AND inputtime > ' . $search_time;
        } elseif ($time == 'week') {
            $search_time = time() - 604800;
            $sql_time = ' AND inputtime > ' . $search_time;
        } elseif ($time == 'month') {
            $search_time = time() - 2592000;
            $sql_time = ' AND inputtime > ' . $search_time;
        } elseif ($time == 'year') {
            $search_time = time() - 31536000;
            $sql_time = ' AND inputtime > ' . $search_time;
        } else {
            $search_time = 0;
            $sql_time = '';
        }
        //搜索历史记录
        $shistory = cookie("shistory");
        if (!$shistory) {
            $shistory = array();
        }
        array_unshift($shistory, $keyword);
        $shistory = array_slice(array_unique($shistory), 0, 10);
        //加入搜索历史
        cookie("shistory", $shistory);

        $modellist = cache('Model');
        if (!$modellist) {
            return $this->error('没有可搜索模型~');
        }

        if ($modelid) {
            if (!array_key_exists($modelid, $modellist)) {
                $this->error('模型错误~');
            }
            $searchField = Db::name('model_field')->where('modelid', $modelid)->where('ifsystem', 1)->where('ifsearch', 1)->column('name');
            if (empty($searchField)) {
                $this->error('没有设置搜索字段~');
            }
            $where = '';
            foreach ($searchField as $vo) {
                $where .= "$vo like '%$keyword%' or ";
            }
            $where = '(' . substr($where, 0, -4) . ') ';
            $where .= " AND status='1' $sql_time";
            $list = $this->Cms_Model->getList($modelid, $where, false, '*', "listorder,id desc", 10, 1, false, ['query' => ['keyword' => $keyword, 'modelid' => $modelid]]);
        } else {
            foreach ($modellist as $key => $vo) {
                $searchField = Db::name('model_field')->where('modelid', $key)->where('ifsystem', 1)->where('ifsearch', 1)->column('name');
                if (empty($searchField)) {
                    continue;
                }
                $where = '';
                foreach ($searchField as $v) {
                    $where .= "$v like '%$keyword%' or ";
                }
                $where = '(' . substr($where, 0, -4) . ') ';
                $where .= " AND status='1' $sql_time";
                $list = $this->Cms_Model->getList($key, $where, false, '*', 'listorder,id desc', 10, 1, false, ['query' => ['keyword' => $keyword, 'modelid' => $modelid]]);
                if ($list->isEmpty()) {
                    continue;
                } else {
                    break;
                }
            }
        }
        $count = $list->total();
        debug('end');
        $this->assign([
            'time' => $time,
            'modelid' => $modelid,
            'keyword' => $keyword,
            'shistory' => $shistory,
            'SEO' => $seo,
            'list' => $list,
            'count' => $count,
            'modellist' => $modellist,
            'search_time' => debug('begin', 'end', 6), //运行时间
            'pages' => $list->render(),
        ]);
        if (!empty($keyword)) {
            return $this->fetch('/search_result');
        } else {
            return $this->fetch('/search');
        }

    }

    /**
     * tags
     */
    public function tags()
    {
        $page = $page = $this->request->param('page/d', 1);
        $tagid = $this->request->param('tagid/d', 0);
        $tag = $this->request->param('tag/s', '');
        $where = array();
        if (!empty($tag)) {
            $where['tag'] = $tag;
        }
        //如果条件为空，则显示标签首页
        if (empty($where)) {
            $data = Db::name('Tags')->order(['hits' => 'DESC'])->limit(100)->select();
            $this->assign("SEO", seo('', '标签'));
            $this->assign('list', $data);
            return $this->fetch('/tags_list');
        }
        //根据条件获取tag信息
        $info = Db::name('Tags')->where($where)->find();
        if (empty($info)) {
            $this->error('抱歉，沒有找到您需要的内容！');
        }
        //访问数+1
        Db::name('Tags')->where($where)->setInc("hits");
        $this->assign($info);
        $this->assign("SEO", seo('', $tag, $info['seo_description'], $info['seo_keyword']));
        $this->assign("page", $page);
        $this->assign($info);
        return $this->fetch('/tags');
    }

    /**
     * 检查支付状态
     */
    protected function _check_payment($flag, $paytype)
    {
        $this->userid = \app\member\service\User::instance()->id;
        if (!$this->userid) {
            return false;
        }
        if (\app\pay\model\Spend::spend_time($this->userid, '24', $flag)) {
            return true;
        }
        return false;
    }

}
