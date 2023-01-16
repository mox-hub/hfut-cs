package com.foocode.manager.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.foocode.manager.model.BookClass;
import org.apache.ibatis.annotations.Mapper;

@Mapper
public interface ClassMapper extends BaseMapper<BookClass> {
}
