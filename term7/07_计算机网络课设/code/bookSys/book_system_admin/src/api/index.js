import request from '../utils/request';

export const fetchData = query => {
    return request({
        url: './table.json',
        method: 'get',
        params: query
    });
};

export const insertData = (data,path) => {
    return request({
        url: 'http://localhost:8080' + path,
        method: 'post',
        data: data
    })
}

export const insertDataParam = (data,path) => {
    return request({
        url: 'http://localhost:8080' + path,
        method: 'post',
        params: data
    })
}

export const deleteData = (data,path) => {
    return request({
        url: 'http://localhost:8080' + path,
        method: 'post',
        params: data
    })
}

export const updateData = (data ,path) => {
    return request({
        url: 'http://localhost:8080' + path,
        method: 'post',
        data: data
    })
}

export const getDataParam = (query, path) => {
    return request({
        url: 'http://localhost:8080' + path,
        method: 'get',
        params: query
    })
};

export const getDataNoParam = (path) => {
    return request({
        url: 'http://localhost:8080' + path,
        method: 'get',
        params: ""
    })
};
