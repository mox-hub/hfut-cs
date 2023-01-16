package com.foocode.manager.service.impl;

import com.baomidou.mybatisplus.core.conditions.update.UpdateWrapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.foocode.manager.mapper.ReaderInfoMapper;
import com.foocode.manager.model.ReaderInfo;
import com.foocode.manager.model.Response;
import com.foocode.manager.service.LendService;
import com.foocode.manager.service.ReaderInfoService;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Service;

import javax.annotation.Resource;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

/**
 * @ClassName :ReaderInfoServiceImpl
 * @Description :
 * @Author :Mox
 * @Date :2022/11/29 19:22
 * @Version : v1.0
 **/
@Service("readerInfo")
public class ReaderInfoServiceImpl implements ReaderInfoService {

    @Resource
    private ReaderInfoMapper readerInfoMapper;
    //  配置logger
    private final static Logger logger = LoggerFactory.getLogger(LendService.class);

    private final static String projectName = "lend-system-server";
    private final static String text = "读者";

    @Override
    public Object addReaderInfo(ReaderInfo readerInfo) {
        try {
            int res = readerInfoMapper.insert(readerInfo);
            Response<Object> response = new Response<>(res, "已添加一条数据！");
            logger.info("[{}] ReaderInfoService::添加{}数据 >>> 添加成功！[{}]", projectName, text, response);
            return response;
        } catch (NullPointerException e) {
            Response<Object> response = new Response<>(-1, "创建失败！");
            logger.error("[{}] ReaderInfoService::添加{}数据 >>> 添加失败！[{}]", projectName, text, e);
            return response;
        }
    }

    @Override
    public Object deleteReaderInfo(long readerId) {
        try {
            System.out.println(readerId);
            int res = readerInfoMapper.deleteById(readerId);
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
    public Object editReaderInfo(ReaderInfo readerInfo) {
        try {
            UpdateWrapper<ReaderInfo> updateWrapper = new UpdateWrapper<>();
            updateWrapper.eq("reader_id", readerInfo.getReaderId());
            int res = readerInfoMapper.update(readerInfo, updateWrapper);
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
    public Object getAllReaderInfos(Map<String,String> data) {
        Page<ReaderInfo> page = new Page<>(Integer.parseInt(data.get("pageIndex")), Integer.parseInt(data.get("pageSize")));
        try {
            readerInfoMapper.selectPage(page,null);
            List<ReaderInfo> readerInfoList = page.getRecords();
            logger.info("[{}]:: 查询所有{}信息 >>> 查询成功 {}", projectName, text, readerInfoList);
            return new Response<>(readerInfoList,(int)page.getTotal());
        } catch (NullPointerException e) {
            Response<Object> response = new Response<>(-1, "未查询到{}！", text);
            logger.error("[{}]::查询所有{}信息 >>> 查询失败！{}", projectName, text, response);
            return response;
        }
    }

    @Override
    public Object getReaderInfoByReaderId(Map<String,String> data) {
        logger.info("[{}]:: 查询{}信息:: 查询模式-> " + "readerInfoId" + " 查询参数->" + data.get("readerId"), projectName, text);
        Page<ReaderInfo> page = new Page<>(Integer.parseInt(data.get("pageIndex")), Integer.parseInt(data.get("pageSize")));
        try {
            ReaderInfo readerInfo = readerInfoMapper.selectById(data.get("readerId"));
            List<ReaderInfo> readerInfoList = new ArrayList<>();
            readerInfoList.add(readerInfo);
            logger.info("[{}]:: 查询{}信息:: 查询模式-> {} >>> 查询成功 {}", projectName, text, "readerInfoId", readerInfoList);
            return new Response<>(readerInfoList);
        } catch (NullPointerException e) {
            Response<Object> response = new Response<>(-1, "未查询到{}！", text);
            logger.error("[{}]::查询所有{}信息 >>> 查询失败！{}", projectName, text, response);
            return response;
        }
    }
}
