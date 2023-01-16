package com.foocode.manager.service;

import com.foocode.manager.model.Order;
import org.springframework.stereotype.Service;

import java.util.Map;

@Service
public interface OrderService {
    Object createOrder(Order order);

    Object deleteOrder(int orderId);

    Object updateOrder(Order order);

    Object queryOrder(Map<String,String> data);

    Object getOrderMessage();
}
