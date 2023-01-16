package com.foocode.manager.service.impl;

import com.baomidou.mybatisplus.core.conditions.query.QueryWrapper;
import com.baomidou.mybatisplus.core.conditions.update.UpdateWrapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.foocode.manager.mapper.OrderMapper;
import com.foocode.manager.model.Message;
import com.foocode.manager.model.Order;
import com.foocode.manager.model.Response;
import com.foocode.manager.service.OrderService;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Service;

import javax.annotation.Resource;
import java.text.SimpleDateFormat;
import java.util.*;

/**
 * @ClassName :OrderServiceImpl
 * @Description :
 * @Author :Mox
 * @Date :2022/12/6 10:01
 * @Version : v1.0
 **/
@Service("order")
public class OrderServiceImpl implements OrderService {
    @Resource
    private OrderMapper orderMapper;
    //  配置logger
    private final static Logger logger = LoggerFactory.getLogger(OrderService.class);

    private final static String projectName = "order-system-server";
    private final static String text = "订单";
    @Override
    public Object createOrder(Order order) {
        try {
            int res = orderMapper.insert(order);
            Response<Object> response = new Response<>(res, "已添加一条数据！");
            logger.info("[{}] OrderService::添加{}数据 >>> 添加成功！[{}]", projectName, text, response);
            return response;
        } catch (NullPointerException e) {
            Response<Object> response = new Response<>(-1, "创建失败！");
            logger.error("[{}] OrderService::添加{}数据 >>> 添加失败！[{}]", projectName, text, e);
            return response;
        }
    }

    @Override
    public Object deleteOrder(int orderId) {
        try {
            System.out.println(orderId);
            int res = orderMapper.deleteById(orderId);
            Response<Object> response = new Response<>(res, "已删除一条数据！");
            logger.info("[{}]::删除{}数据 >>> 删除成功！[{}]", projectName, text, response);
            return response;
        } catch (NullPointerException e) {
            Response<Object> response = new Response<>(-1, "删除失败！");
            logger.error("[{}]::删除{}数据 >>> 删除失败！[{}]", projectName, text, e);
            return response;
        }
    }

    @Override
    public Object updateOrder(Order order) {
        try {
            UpdateWrapper<Order> updateWrapper = new UpdateWrapper<>();
            int res = orderMapper.updateById(order);
            Response<Object> response = new Response<>(res, "已更新一条数据");
            logger.info("[{}]::更新{}数据 >>> 更新成功！[{}]", projectName, text, response);
            return response;
        } catch (NullPointerException e) {
            Response<Object> response = new Response<>(-1, "更新失败！");
            logger.error("[{}]::更新{}数据 >>> 更新失败！[{}]", projectName, text, e);
            return response;
        }
    }

    @Override
    public Object queryOrder(Map<String, String> data) {
        logger.info("[{}]:: 查询{}信息:: 查询模式-> " + "searchWord" + " 查询参数->" +data.get("options"), projectName, text);
        Page<Order> page = new Page<>(Integer.parseInt(data.get("pageIndex")), Integer.parseInt(data.get("pageSize")));
        try {
            QueryWrapper<Order> wrapper = new QueryWrapper<>();
            wrapper.like("order_id",data.get("options"))
                    .or()
                    .like("order_name",data.get("options"))
                    .or()
                    .like("supplier",data.get("options"))
                    .or()
                    .like("item",data.get("options"));
            orderMapper.selectPage(page,wrapper);
            List<Order> orderList = page.getRecords();
            logger.info("[{}]:: 查询所有{}信息 >>> 查询成功 {}", projectName, text, orderList);
            return new Response<>(orderList,(int)page.getTotal());
        } catch (NullPointerException e) {
            Response<Object> response = new Response<>(-1, "未查询到{}！", text);
            logger.error("[{}]::查询所有{}信息 >>> 查询失败！{}", projectName, text, response);
            return response;
        }
    }

    @Override
    public Object getOrderMessage() {
        Date date = new Date();
        Calendar calendar = new GregorianCalendar();
        SimpleDateFormat dateFormat= new SimpleDateFormat("yyyy-MM-dd hh:mm:ss");

        List<Order> orderList = orderMapper.selectList(null);
        List<Message> messageList = new ArrayList<>();

        for (Order order : orderList) {
            long to = order.getEndTime().getTime();
            long from = date.getTime();
            int days = (int) ((to - from)/(1000 * 60 * 60 * 24));
            if (days <= 7 && order.getStatus().equals("0")) {
                System.out.println(order.getStatus());
                Message msg = new Message();
                calendar.setTime(order.getEndTime());
                calendar.add(Calendar.SECOND,-7*24*3600);
                msg.setDate(dateFormat.format(calendar.getTime()));
                msg.setTitle("您的合同ID为："+order.getOrderId() + "的合同还有"+days+"天将要结束，请尽快验收！");
                messageList.add(msg);
            }
        }
        logger.info("[{}]:: 查询所有提示信息 >>> 查询成功 {}", projectName, orderList);
        return new Response<>(messageList,messageList.size());
    }
}
