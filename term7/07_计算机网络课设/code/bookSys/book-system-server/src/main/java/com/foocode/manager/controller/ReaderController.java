package com.foocode.manager.controller;

import com.foocode.manager.model.ReaderInfo;
import com.foocode.manager.service.ReaderInfoService;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.*;

import javax.annotation.Resource;
import java.util.Map;

@CrossOrigin
@RestController
public class ReaderController {

    @Resource(name = "readerInfo")
    private ReaderInfoService readerInfoService;


    @PostMapping("/reader/editReaderCard")
    public Object addReaderInfo(@RequestBody ReaderInfo readerInfo) {
        return readerInfoService.addReaderInfo(readerInfo);
    }

    @PostMapping("/reader/deleteReaderInfo")
    public Object deleteReaderInfo(@RequestParam long readerId){
        return readerInfoService.deleteReaderInfo(readerId);
    }

    @PostMapping("/reader/editReaderInfo")
    public Object editReaderInfo(@RequestBody ReaderInfo readerInfo){
        return readerInfoService.editReaderInfo(readerInfo);
    }

    @GetMapping("/reader/getAllReaderInfos")
    public Object getAllReaderInfos(@RequestParam Map<String,String> data){
        return readerInfoService.getAllReaderInfos(data);
    }

    @GetMapping("/reader/getReaderInfoByReaderId")
    public Object getReaderInfoByReaderId(@RequestParam Map<String,String> data){
        return readerInfoService.getReaderInfoByReaderId(data);
    }


}
