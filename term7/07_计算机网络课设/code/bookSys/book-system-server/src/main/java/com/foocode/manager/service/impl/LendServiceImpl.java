package com.foocode.manager.service.impl;

import com.baomidou.mybatisplus.core.conditions.query.QueryWrapper;
import com.baomidou.mybatisplus.core.conditions.update.UpdateWrapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.foocode.manager.mapper.BookMapper;
import com.foocode.manager.mapper.LendMapper;
import com.foocode.manager.model.Book;
import com.foocode.manager.model.Lend;
import com.foocode.manager.model.ReaderInfo;
import com.foocode.manager.model.Response;
import com.foocode.manager.service.LendService;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Service;

import javax.annotation.Resource;
import java.util.List;
import java.util.Map;

/**
 * @ClassName :LendServiceImpl
 * @Description :
 * @Author :Mox
 * @Date :2022/11/29 18:36
 * @Version : v1.0
 **/

@Service("lend")
public class LendServiceImpl implements LendService {
    @Resource
    private LendMapper lendMapper;
    @Resource
    private BookMapper bookMapper;
    //  配置logger
    private final static Logger logger = LoggerFactory.getLogger(LendService.class);

    private final static String projectName = "lend-system-server";
    private final static String text = "借阅记录";

    @Override
    public Object lendBook(Lend lend) {
        try {

            int res = lendMapper.insert(lend);
            // 更新book的数量
            Book book = bookMapper.selectById(lend.getBookId());
            book.setNumber(book.getNumber()-1);
            lendMapper.updateById(lend);

            Response<Object> response = new Response<>(res, "已添加一条数据！");
            logger.info("[{}] LendService::添加{}数据 >>> 添加成功！[{}]", projectName, text, response);
            return response;
        } catch (NullPointerException e) {
            Response<Object> response = new Response<>(-1, "创建失败！");
            logger.error("[{}] LendService::添加{}数据 >>> 添加失败！[{}]", projectName, text, e);
            return response;
        }
    }

    @Override
    public Object deleteLend(long serNum) {
        try {
            System.out.println(serNum);
            int res = lendMapper.deleteById(serNum);
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
    public Object returnBook(Lend lend) {
        try {
            // 更新book的数量
            Book book = bookMapper.selectById(lend.getBookId());
            book.setNumber(book.getNumber()+1);
            bookMapper.updateById(book);

            // 更新借阅记录
            System.out.println(lend.getBackDate());
            int res = lendMapper.updateById(lend);
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
    public Object getAllLends(Map<String,String> data) {
        Page<Lend> page = new Page<>(Integer.parseInt(data.get("pageIndex")), Integer.parseInt(data.get("pageSize")));
        try {
            lendMapper.selectPage(page,null);
            List<Lend> lendList = page.getRecords();
            logger.info("[{}]:: 查询所有{}信息 >>> 查询成功 {}", projectName, text, lendList);
            return new Response<>(lendList,(int)page.getTotal());
        } catch (NullPointerException e) {
            Response<Object> response = new Response<>(-1, "未查询到{}！", text);
            logger.error("[{}]::查询所有{}信息 >>> 查询失败！{}", projectName, text, response);
            return response;
        }
    }

    @Override
    public Object getLendByReaderId(Map<String,String> data) {
        logger.info("[{}]:: 查询{}信息:: 查询模式-> " + "readerId" + " 查询参数->" + data.get("options"), projectName, text);
        Page<Lend> page = new Page<>(Integer.parseInt(data.get("pageIndex")), Integer.parseInt(data.get("pageSize")));
        try {
            QueryWrapper wrapper = new QueryWrapper();
            wrapper.like("reader_id",data.get("options"));
            lendMapper.selectPage(page,wrapper);
            List<Lend> lendList = page.getRecords();
            logger.info("[{}]:: 查询{}信息:: 查询模式-> {} >>> 查询成功 {}", projectName, text, "lendId", lendList);
            return new Response<>(lendList,(int)page.getTotal());
        } catch (NullPointerException e) {
            Response<Object> response = new Response<>(-1, "未查询到{}！", text);
            logger.error("[{}]::查询所有{}信息 >>> 查询失败！{}", projectName, text, response);
            return response;
        }
    }
}
