package com.foocode.manager.model;

/**
 * @ClassName :WxRequestRes
 * @Description :
 * @Author :Mox
 * @Date :2022/4/8 11:45
 * @Version : v1.0
 **/

public class RequestRes {
    int code;
    String msg;

    public int getCode() {
        return code;
    }

    public void setCode(int code) {
        this.code = code;
    }

    public String getMsg() {
        return msg;
    }

    public void setMsg(String msg) {
        this.msg = msg;
    }

    @Override
    public String toString() {
        return "WxRequestRes{" +
                "code=" + code +
                ", msg='" + msg + '\'' +
                '}';
    }
}
