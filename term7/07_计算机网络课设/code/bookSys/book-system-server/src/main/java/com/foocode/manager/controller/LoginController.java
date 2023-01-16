package com.foocode.manager.controller;

import com.foocode.manager.service.LoginService;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.CrossOrigin;
import org.springframework.web.bind.annotation.RestController;


import javax.annotation.Resource;

@CrossOrigin
@RestController
public class LoginController {

    @Resource(name = "login")
    private LoginService loginService;



}