<?php
return [
    [
        //父菜单ID，NULL或者不写系统默认，0为顶级菜单
        "parentid" => 45,
        //地址，[模块/]控制器/方法
        "route" => "formguide/formguide/index",
        //类型，1：权限认证+菜单，0：只作为菜单
        "type" => 0,
        //状态，1是显示，0不显示（需要参数的，建议不显示，例如编辑,删除等操作）
        "status" => 1,
        //名称
        "name" => "表单管理",
        //图标
        "icon" => "icon-shiyongwendang",
        //备注
        "remark" => "",
        //排序
        "listorder" => 3,
        //子菜单列表
        "child" => [
            [
                "route" => "formguide/formguide/add",
                "type" => 1,
                "status" => 0,
                "name" => "添加表单",
            ],
            [
                "route" => "formguide/formguide/edit",
                "type" => 1,
                "status" => 0,
                "name" => "编辑表单",
            ],
            [
                "route" => "formguide/formguide/delete",
                "type" => 1,
                "status" => 0,
                "name" => "删除表单",
            ],
            [
                "route" => "formguide/field/index",
                "type" => 1,
                "status" => 0,
                "name" => "字段管理",
            ],
            [
                "route" => "formguide/field/add",
                "type" => 1,
                "status" => 0,
                "name" => "添加字段",
            ],
            [
                "route" => "formguide/field/edit",
                "type" => 1,
                "status" => 0,
                "name" => "编辑字段",
            ],
            [
                "route" => "formguide/field/delete",
                "type" => 1,
                "status" => 0,
                "name" => "删除字段",
            ],
            [
                "route" => "formguide/info/index",
                "type" => 1,
                "status" => 0,
                "name" => "信息列表",
            ],
            [
                "route" => "formguide/info/delete",
                "type" => 1,
                "status" => 0,
                "name" => "信息删除",
            ],
        ],
    ],
];
