package com.foocode.manager.service;

import com.foocode.manager.model.ReaderInfo;
import org.springframework.stereotype.Service;

import java.util.ArrayList;
import java.util.Map;

@Service
public interface ReaderInfoService {

    Object addReaderInfo(ReaderInfo readerInfo);

    Object deleteReaderInfo(long readerId);

    Object editReaderInfo(ReaderInfo readerInfo);

    Object getAllReaderInfos(Map<String,String> data);

    Object getReaderInfoByReaderId(Map<String,String> data);
}
