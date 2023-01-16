package com.foocode.manager.service;
import com.foocode.manager.model.Lend;
import org.springframework.stereotype.Service;

import java.util.Map;


@Service
public interface LendService {

    Object lendBook(Lend lend);

    Object deleteLend(long serNum);

    Object returnBook(Lend lend);

    Object getAllLends(Map<String,String> data);

    Object getLendByReaderId(Map<String,String> data);

}
