package com.foocode.manager.service.impl;

import com.baomidou.mybatisplus.core.conditions.query.QueryWrapper;
import com.baomidou.mybatisplus.core.conditions.update.UpdateWrapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.foocode.manager.mapper.BookMapper;
import com.foocode.manager.model.Book;
import com.foocode.manager.model.Response;
import com.foocode.manager.service.BookService;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Service;

import javax.annotation.Resource;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

/**
 * @ClassName :BookServiceImpl
 * @Description :
 * @Author :Mox
 * @Date :2022/11/29 17:51
 * @Version : v1.0
 **/

@Service("book")
public class BookServiceImpl implements BookService{
    @Resource
    private BookMapper bookMapper;
    //  配置logger
    private final static Logger logger = LoggerFactory.getLogger(BookService.class);

    private final static String projectName = "book-system-server";
    private final static String text = "书籍";

    @Override
    public Object addBook(Book book) {
        try {
            int res = bookMapper.insert(book);
            Response<Object> response = new Response<>(res, "已添加一条数据！");
            logger.info("[{}] BookService::添加{}数据 >>> 添加成功！[{}]", projectName, text, response);
            return response;
        } catch (NullPointerException e) {
            Response<Object> response = new Response<>(-1, "创建失败！");
            logger.error("[{}] BookService::添加{}数据 >>> 添加失败！[{}]", projectName, text, e);
            return response;
        }
    }

    @Override
    public Object deleteBook(Long bookId) {
        try {
            System.out.println(bookId);
            int res = bookMapper.deleteById(bookId);
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
    public Object editBook(Book book) {
        try {
            UpdateWrapper<Book> updateWrapper = new UpdateWrapper<>();
            updateWrapper.eq("book_id", book.getBookId());
            int res = bookMapper.update(book, updateWrapper);
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
    public Object getAllBooks() {
        try {
            List<Book> bookList = bookMapper.selectList(null);
            logger.info("[{}]:: 查询所有{}信息 >>> 查询成功 {}", projectName, text, bookList);
            return new Response<>(bookList);
        } catch (NullPointerException e) {
            Response<Object> response = new Response<>(-1, "未查询到{}！", text);
            logger.error("[{}]::查询所有{}信息 >>> 查询失败！{}", projectName, text, response);
            return response;
        }
    }

    @Override
    public Object getBookByBookId(Map<String,String> data) {
        logger.info("[{}]:: 查询{}信息:: 查询模式-> " + "bookId" + " 查询参数->" + data.get("options"), projectName, text);
        Page<Book> page = new Page<>(Integer.parseInt(data.get("pageIndex")), Integer.parseInt(data.get("pageSize")));
        try {
            Book book = bookMapper.selectById(data.get("options"));
            List<Book> bookList = new ArrayList<>();
            bookList.add(book);
            logger.info("[{}]:: 查询{}信息:: 查询模式-> {} >>> 查询成功 {}", projectName, text, "bookId", bookList);
            return new Response<>(bookList,(int)page.getTotal());
        } catch (NullPointerException e) {
            Response<Object> response = new Response<>(-1, "未查询到{}！", text);
            logger.error("[{}]::查询所有{}信息 >>> 查询失败！{}", projectName, text, response);
            return response;
        }
    }

    @Override
    public Object getBookByClassId(Map<String,String> data) {
        logger.info("[{}]:: 查询{}信息:: 查询模式-> " + "bookId" + " 查询参数->" + data.get("options"), projectName, text);
        Page<Book> page = new Page<>(Integer.parseInt(data.get("pageIndex")), Integer.parseInt(data.get("pageSize")));
        try {
            QueryWrapper<Book> wrapper = new QueryWrapper<>();
            wrapper.eq("class_id",data.get("options"));
            bookMapper.selectPage(page,wrapper);
            List<Book> bookList = page.getRecords();
            logger.info("[{}]:: 查询{}信息:: 查询模式-> {} >>> 查询成功 {}", projectName, text, "bookId", bookList);
            return new Response<>(bookList,(int)page.getTotal());
        } catch (NullPointerException e) {
            Response<Object> response = new Response<>(-1, "未查询到{}！", text);
            logger.error("[{}]::查询所有{}信息 >>> 查询失败！{}", projectName, text, response);
            return response;
        }
    }

    @Override
    public Object queryBook(Map<String,String> data) {
        logger.info("[{}]:: 查询{}信息:: 查询模式-> " + "searchWord" + " 查询参数->" +data.get("options"), projectName, text);
        Page<Book> page = new Page<>(Integer.parseInt(data.get("pageIndex")), Integer.parseInt(data.get("pageSize")));
        try {
            QueryWrapper<Book> wrapper = new QueryWrapper<>();
            wrapper.like("name",data.get("options"))
                    .or()
                    .like("isbn",data.get("options"))
                    .or()
                    .like("author",data.get("options"));
            bookMapper.selectPage(page,wrapper);
            List<Book> bookList = page.getRecords();
            logger.info("[{}]:: 查询所有{}信息 >>> 查询成功 {}", projectName, text, bookList);
            return new Response<>(bookList,(int)page.getTotal());
        } catch (NullPointerException e) {
            Response<Object> response = new Response<>(-1, "未查询到{}！", text);
            logger.error("[{}]::查询所有{}信息 >>> 查询失败！{}", projectName, text, response);
            return response;
        }
    }
}
