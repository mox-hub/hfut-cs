package com.foocode.manager.controller;

import com.foocode.manager.model.Order;
import com.foocode.manager.service.OrderService;
import com.foocode.manager.service.OrderService;
import org.springframework.web.bind.annotation.*;

import javax.annotation.Resource;
import java.util.Map;

/**
 * @ClassName :OrderController
 * @Description :
 * @Author :Mox
 * @Date :2022/12/6 10:10
 * @Version : v1.0
 **/

@CrossOrigin
@RestController
public class OrderController {
    @Resource(name = "order")
    private OrderService orderService;

    @PostMapping("/order/createOrder")
    public Object createOrder(@RequestBody Order order) {
        return orderService.createOrder(order);
    }

    @PostMapping("/order/deleteOrder")
    public Object deleteOrder(@RequestParam int orderId) {
        return orderService.deleteOrder(orderId);
    }

    @PostMapping("/order/updateOrder")
    public Object updateOrder(@RequestBody Order order) {
        return orderService.updateOrder(order);
    }

    @GetMapping("/order/queryOrder")
    public Object queryOrder(@RequestParam Map<String,String> data) {
        return orderService.queryOrder(data);
    }

    @GetMapping("/order/getOrderMessage")
    public Object getOrderMessage() {
        return orderService.getOrderMessage();
    }
}
