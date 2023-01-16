package com.foocode.manager.model;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;

/**
 * @ClassName :BookClass
 * @Description :
 * @Author :Mox
 * @Date :2022/11/29 20:26
 * @Version : v1.0
 **/

@TableName(value = "class_info")
public class BookClass {
    @TableId(type = IdType.AUTO)
    private Integer classId;
    private String className;
}
