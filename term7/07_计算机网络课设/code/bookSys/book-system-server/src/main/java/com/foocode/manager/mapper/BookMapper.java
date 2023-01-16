package com.foocode.manager.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.foocode.manager.model.Book;
import org.apache.ibatis.annotations.Mapper;

@Mapper
public interface BookMapper extends BaseMapper<Book> {
    
}
