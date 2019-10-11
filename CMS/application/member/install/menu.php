<?php
return [
    [
        //父菜单ID，NULL或者不写系统默认，0为顶级菜单
        "parentid" => 0,
        //地址，[模块/]控制器/方法
        "route" => "member/member/index1",
        //类型，1：权限认证+菜单，0：只作为菜单
        "type" => 0,
        //状态，1是显示，0不显示（需要参数的，建议不显示，例如编辑,删除等操作）
        "status" => 1,
        //名称
        "name" => "会员",
        //图标
        "icon" => "icon-people",
        //备注
        "remark" => "",
        //排序
        "listorder" => 4,
        //子菜单列表
        "child" => [
            [
                "route" => "member/member/index",
                "type" => 1,
                "status" => 1,
                "name" => "会员管理",
                "icon" => "icon-people",
                "child" => [
                    [
                        "route" => "member/setting/setting",
                        "type" => 1,
                        "status" => 1,
                        "name" => "会员设置",
                        "icon" => "icon-setup",
                    ],
                    [
                        "route" => "member/member/manage",
                        "type" => 1,
                        "status" => 1,
                        "name" => "会员管理",
                        "icon" => "icon-huiyuan",
                    ],
                    [
                        "route" => "member/member/userverify",
                        "type" => 1,
                        "status" => 1,
                        "name" => "审核会员",
                        "icon" => "icon-shenhe",
                    ],
                ],
            ],
            [
                "route" => "member/group/index1",
                "type" => 1,
                "status" => 1,
                "name" => "会员组",
                "icon" => "icon-huiyuan2",
                "child" => [
                    [
                        "route" => "member/group/index",
                        "type" => 1,
                        "status" => 1,
                        "name" => "会员组管理",
                        "icon" => "icon-huiyuan2",
                    ],
                ],
            ],

        ],
    ],
];
