package com.foocode.manager.model;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;

import java.io.Serializable;
import java.util.Date;

@TableName(value = "lend_list")
public class Lend implements Serializable {

    @TableId(type = IdType.AUTO)
    private Integer serNum;
    private Integer bookId;
    private Integer readerId;
    private Date lendDate;
    private Date backDate;

    public void setSerNum(Integer serNum) {
        this.serNum = serNum;
    }

    public void setBookId(Integer bookId) {
        this.bookId = bookId;
    }

    public void setReaderId(Integer readerId) {
        this.readerId = readerId;
    }

    public Integer getSerNum() {
        return serNum;
    }

    public Integer getBookId() {
        return bookId;
    }

    public Integer getReaderId() {
        return readerId;
    }

    public Date getLendDate() {
        return lendDate;
    }

    public void setLendDate(Date lendDate) {
        this.lendDate = lendDate;
    }

    public Date getBackDate() {
        return backDate;
    }

    public void setBackDate(Date backDate) {
        this.backDate = backDate;
    }
}
