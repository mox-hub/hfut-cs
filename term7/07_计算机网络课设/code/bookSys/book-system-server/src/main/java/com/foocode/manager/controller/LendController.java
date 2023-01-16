package com.foocode.manager.controller;

import com.foocode.manager.model.Lend;
import com.foocode.manager.service.LendService;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.*;

import javax.annotation.Resource;
import java.util.Map;

@CrossOrigin
@RestController
public class LendController {
    @Resource(name = "lend")
    private LendService lendService;

    @PostMapping("/lend/lendBook")
    public Object lendBook(@RequestBody Lend lend) {
        return lendService.lendBook(lend);
    }

    @PostMapping("/lend/deleteLend")
    public Object deleteLend(@RequestParam long serNum) {
        return lendService.deleteLend(serNum);
    }

    @PostMapping("/lend/returnBook")
    public Object returnBook(@RequestBody Lend lend) {
        return lendService.returnBook(lend);
    }

    @GetMapping("/lend/getAllLends")
    public Object getAllLends(@RequestParam Map<String,String> data) {
        return lendService.getAllLends(data);
    }

    @GetMapping("/lend/getLendByReaderId")
    public Object getLendByReaderId(@RequestParam Map<String,String> data) {
        return lendService.getLendByReaderId(data);
    }


}
