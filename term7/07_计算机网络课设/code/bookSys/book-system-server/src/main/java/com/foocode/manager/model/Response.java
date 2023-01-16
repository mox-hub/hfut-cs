package com.foocode.manager.model;

/**
 * @ClassName :Response
 * @Description :
 * @Author :Mox
 * @Date :2022/4/9 20:02
 * @Version : v1.0
 **/

public class Response<T> {
    private int code;
    private String msg;
    private int pageTotal;
    private T data;

    /**
     * 若没有数据返回，默认状态码为0，提示信息为：操作成功！
     *
     * @param i
     * @param s
     * @param text
     */
    public Response(int i, String s, String text) {

        this.code = 0;
        this.msg = "操作成功！";
    }

    /**
     * 若没有数据返回，可以人为指定状态码和提示信息
     *
     * @param code
     * @param msg
     **/
    public Response(int code, String msg) {

        this.code = code;
        this.msg = msg;
    }

    /**
     * 有数据返回时，状态码为0，默认提示信息为：操作成功！
     *
     * @param data
     **/
    public Response(T data) {

        this.code = 0;
        this.msg = "操作成功！";
        this.data = data;
    }

    public Response(T data, Integer pageTotal) {

        this.code = 0;
        this.msg = "操作成功！";
        this.pageTotal = pageTotal;
        this.data = data;
    }

    /**
     * 有数据返回，状态码为0，人为指定提示信息
     *
     * @param data
     * @param msg
     **/
    public Response(T data, String msg) {

        this.code = 0;
        this.msg = msg;
        this.data = data;
    }

    public int getPageTotal() {
        return pageTotal;
    }

    public void setPageTotal(int pageTotal) {
        this.pageTotal = pageTotal;
    }

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

    public T getData() {
        return data;
    }

    public void setData(T data) {
        this.data = data;
    }

    @Override
    public String toString() {
        return "Response{" +
                "code=" + code +
                ", msg='" + msg + '\'' +
                ", data=" + data +
                '}';
    }
}
