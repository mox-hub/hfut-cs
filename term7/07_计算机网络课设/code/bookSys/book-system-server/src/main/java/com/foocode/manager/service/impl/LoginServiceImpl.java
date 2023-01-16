package com.foocode.manager.service.impl;

import com.foocode.manager.service.LoginService;
import org.springframework.stereotype.Service;

/**
 * @ClassName :LoginServiceImpl
 * @Description :
 * @Author :Mox
 * @Date :2022/11/29 19:30
 * @Version : v1.0
 **/
@Service("login")
public class LoginServiceImpl implements LoginService {
    @Override
    public Object hasMatchReader(long readerId, String password) {
        return null;
    }

    @Override
    public Object hasMatchAdmin(long adminId, String password) {
        return null;
    }

    @Override
    public Object getAdminUsername(long adminId) {
        return null;
    }

    @Override
    public Object getReaderCardByReaderId(long readerId) {
        return null;
    }

    @Override
    public Object getAdminPassword(long adminId) {
        return null;
    }

    @Override
    public Object getReaderPassword(long readerId) {
        return null;
    }

    @Override
    public Object readerRePassword(long readerId, String newPassword) {
        return null;
    }
}
