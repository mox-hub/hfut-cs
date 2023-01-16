package com.foocode.manager.service;

import org.springframework.stereotype.Service;

@Service
public interface  LoginService {

    Object hasMatchReader(long readerId,String password);

    Object hasMatchAdmin(long adminId,String password);

    Object getAdminUsername(long adminId);

    Object getReaderCardByReaderId(long readerId);

    Object getAdminPassword(long adminId);

    Object getReaderPassword(long readerId);

    Object readerRePassword(long readerId, String newPassword);

}
