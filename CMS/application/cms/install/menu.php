<?php
return [
    [
        //父菜单ID，NULL或者不写系统默认，0为顶级菜单
        "parentid" => 0,
        //地址，[模块/]控制器/方法
        "route" => "cms/cms/index1",
        //类型，1：权限认证+菜单，0：只作为菜单
        "type" => 0,
        //状态，1是显示，0不显示（需要参数的，建议不显示，例如编辑,删除等操作）
        "status" => 1,
        //名称
        "name" => "内容",
        //图标
        "icon" => "icon-shiyongwendang",
        //备注
        "remark" => "",
        //排序
        "listorder" => 3,
        //子菜单列表
        "child" => [
            [
                "route" => "cms/cms/index2",
                "type" => 1,
                "status" => 1,
                "name" => "内容管理",
                "icon" => "icon-neirongguanli",
                "child" => [
                    [
                        "route" => "cms/cms/index",
                        "type" => 1,
                        "status" => 1,
                        "name" => "管理内容",
                        "icon" => "icon-neirongguanli",
                        "child" => [
                            [
                                "route" => "cms/cms/panl",
                                "type" => 1,
                                "status" => 0,
                                "name" => "面板",
                            ],
                            [
                                "route" => "cms/cms/public_categorys",
                                "type" => 1,
                                "status" => 0,
                                "name" => "栏目列表",
                            ],
                            [
                                "route" => "cms/cms/classlist",
                                "type" => 1,
                                "status" => 0,
                                "name" => "信息列表",
                            ],
                            [
                                "route" => "cms/cms/add",
                                "type" => 1,
                                "status" => 0,
                                "name" => "添加",
                            ],
                            [
                                "route" => "cms/cms/edit",
                                "type" => 1,
                                "status" => 0,
                                "name" => "编辑",
                            ],
                            [
                                "route" => "cms/cms/delete",
                                "type" => 1,
                                "status" => 0,
                                "name" => "删除",
                            ],
                            [
                                "route" => "cms/cms/listorder",
                                "type" => 1,
                                "status" => 0,
                                "name" => "排序",
                            ],
                            [
                                "route" => "cms/cms/remove",
                                "type" => 1,
                                "status" => 0,
                                "name" => "批量移动",
                            ],
                            [
                                "route" => "cms/cms/recycle",
                                "type" => 1,
                                "status" => 0,
                                "name" => "回收站",
                                "icon" => "icon-trash",
                            ],
                        ],
                    ],
                    [
                        "route" => "cms/publish/index",
                        "type" => 1,
                        "status" => 0,
                        "name" => "稿件管理",
                        "icon" => "icon-neirongguanli",
                        "child" => [
                        ],
                    ],
                    [
                        "route" => "cms/tags/index",
                        "type" => 1,
                        "status" => 1,
                        "name" => "Tags管理",
                        "icon" => "icon-label",
                        "child" => [
                            [
                                "route" => "cms/tags/index",
                                "type" => 1,
                                "status" => 0,
                                "name" => "列表",
                            ],
                            [
                                "route" => "cms/tags/add",
                                "type" => 1,
                                "status" => 0,
                                "name" => "添加",
                            ],
                            [
                                "route" => "cms/tags/edit",
                                "type" => 1,
                                "status" => 0,
                                "name" => "编辑",
                            ],
                            [
                                "route" => "cms/tags/delete",
                                "type" => 1,
                                "status" => 0,
                                "name" => "删除",
                            ],
                        ],
                    ],
                ],
            ],
            [
                "route" => "cms/category/index1",
                "type" => 1,
                "status" => 1,
                "name" => "相关设置",
                "icon" => "icon-zidongxiufu",
                "child" => [
                    [
                        "route" => "cms/setting/index",
                        "type" => 1,
                        "status" => 1,
                        "name" => "CMS配置",
                        "icon" => "icon-setup",
                    ],
                    [
                        "route" => "cms/category/index",
                        "type" => 1,
                        "status" => 1,
                        "name" => "栏目列表",
                        "icon" => "icon-other",
                        "child" => [
                            [
                                "route" => "cms/category/add",
                                "type" => 1,
                                "status" => 0,
                                "name" => "添加栏目",
                            ],
                            [
                                "route" => "cms/category/singlepage",
                                "type" => 1,
                                "status" => 0,
                                "name" => "添加单页",
                            ],
                            [
                                "route" => "cms/category/wadd",
                                "type" => 1,
                                "status" => 0,
                                "name" => "添加外部链接",
                            ],
                            [
                                "route" => "cms/category/edit",
                                "type" => 1,
                                "status" => 0,
                                "name" => "编辑栏目",
                            ],
                            [
                                "route" => "cms/category/delete",
                                "type" => 1,
                                "status" => 0,
                                "name" => "删除栏目",
                            ],
                            [
                                "route" => "cms/category/public_cache",
                                "type" => 1,
                                "status" => 0,
                                "name" => "更新栏目",
                            ],
                            [
                                "route" => "cms/category/listorder",
                                "type" => 1,
                                "status" => 0,
                                "name" => "排序",
                            ],
                            [
                                "route" => "cms/category/setstate",
                                "type" => 1,
                                "status" => 0,
                                "name" => "状态设置",
                            ],
                        ],
                    ],
                    [
                        "route" => "cms/models/index",
                        "type" => 1,
                        "status" => 1,
                        "name" => "模型管理",
                        "icon" => "icon-apartment",
                        "child" => [
                            [
                                "route" => "cms/field/index",
                                "type" => 1,
                                "status" => 0,
                                "name" => "字段管理",
                            ],
                            [
                                "route" => "cms/field/add",
                                "type" => 1,
                                "status" => 0,
                                "name" => "字段添加",
                            ],
                            [
                                "route" => "cms/field/edit",
                                "type" => 1,
                                "status" => 0,
                                "name" => "字段编辑",
                            ],
                            [
                                "route" => "cms/field/delete",
                                "type" => 1,
                                "status" => 0,
                                "name" => "字段删除",
                            ],
                            [
                                "route" => "cms/field/listorder",
                                "type" => 1,
                                "status" => 0,
                                "name" => "字段排序",
                            ],
                            [
                                "route" => "cms/field/setstate",
                                "type" => 1,
                                "status" => 0,
                                "name" => "字段状态",
                            ],
                            [
                                "route" => "cms/field/setsearch",
                                "type" => 1,
                                "status" => 0,
                                "name" => "字段搜索",
                            ],
                            [
                                "route" => "cms/field/setvisible",
                                "type" => 1,
                                "status" => 0,
                                "name" => "字段隐藏",
                            ],
                            [
                                "route" => "cms/field/setrequire",
                                "type" => 1,
                                "status" => 0,
                                "name" => "字段必须",
                            ],

                            [
                                "route" => "cms/models/add",
                                "type" => 1,
                                "status" => 0,
                                "name" => "添加模型",
                            ],
                            [
                                "route" => "cms/models/edit",
                                "type" => 1,
                                "status" => 0,
                                "name" => "修改模型",
                            ],
                            [
                                "route" => "cms/models/delete",
                                "type" => 1,
                                "status" => 0,
                                "name" => "删除模型",
                            ],
                            [
                                "route" => "cms/models/setSub",
                                "type" => 1,
                                "status" => 0,
                                "name" => "模型投稿",
                            ],
                            [
                                "route" => "cms/models/setstate",
                                "type" => 1,
                                "status" => 0,
                                "name" => "设置模型状态",
                            ],
                        ],
                    ],
                ],
            ],
        ],
    ],
];
