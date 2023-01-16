<template>
    <div>
        <!-- 小页面标题 -->
        <div class="crumbs">
            <el-breadcrumb separator="/">
                <el-breadcrumb-item>
                    <i class="el-icon-lx-cascades"></i> 图书信息管理
                </el-breadcrumb-item>
            </el-breadcrumb>
        </div>
        <!-- 图书表单 -->
        <div class="container">
            <!-- 标题工具栏 -->
            <div class="handle-box">
                <el-select v-model="query.mode" placeholder="查询模式" class="handle-select mr10">
                    <el-option key="1" label="图书ID查询" value="id"></el-option>
                    <el-option key="2" label="图书关键字查询" value="keyword"></el-option>
                </el-select>
                <el-input  v-model="query.options" placeholder="请输入参数" class="handle-input mr10"></el-input>
                <el-button type="primary" icon="el-icon-search" @click="handleSearch">搜索</el-button>
                <el-button type="success" icon="el-icon-plus" @click="handleAdd">添加图书</el-button>
            </div>
            <!-- 左右分行 -->
            <el-row :gutter="20">
                <!-- 左侧3 -->
                <el-col :span="3">
                    <!-- 侧边栏进行分类选择 -->
                    <el-menu class="sidebar-el-menu" router >
                        <el-menu-item v-for="item in bookClass" :key="item.index" @click="searchForClass(item)">
                            <i :class="item.icon"></i>
                            <template #title>{{ item.title }}</template>
                        </el-menu-item>
                    </el-menu>
                </el-col>
                <!-- 右侧21 -->
                <el-col :span="21">
                    <!-- 分列进行图书展示 -->
                    <el-row :gutter="20" type="flex">
                        <!-- 循环展示 -->
                        <el-col :span="4" v-for="(book,index) in bookData" :key="index" class="el-col">
                            <el-card class="book-card" shadow="hover" :body-style="{ padding: '0rpx' }">
                                <img src="https://img3m9.ddimg.cn/42/28/11102603829-1_w_2.jpg" class="image"/>
                                <div style="padding: 14px">
                                    <div class="book-info">
                                        <span class="book-title">{{book.name}}</span>
                                        <span class="book-author">{{book.author}}</span>
                                    </div>
                                    <time class="time">{{book.pubDate}}</time>
                                    <div class="bottom">
                                        <el-button type="info" plain @click="handleDetail(index)">查看</el-button>
                                        <el-button type="success" plain @click="handleLend(index,book.number)">借阅</el-button>
                                    </div>
                                </div>
                            </el-card>
                        </el-col>               
                    </el-row>
                    <el-empty v-if="pageTotal == 0" description="暂无书籍" />
                </el-col>
            </el-row>

            <!-- 翻页器 -->
            <div class="pagination">
                <el-pagination background layout="total, prev, pager, next" :current-page="query.pageIndex"
                    :page-size="query.pageSize" :total="pageTotal" @current-change="handlePageChange"></el-pagination>
            </div>
        </div>

        <!-- 编辑弹出框 无校验 -->
        <el-dialog title="图书详情" v-model="editVisible" width="30%" center>
            <img src="https://img3m9.ddimg.cn/42/28/11102603829-1_w_2.jpg" class="image-detail"/>
            <el-form label-width="120px">
                <el-form-item label="图书名称：">
                    <text>{{form.name}}</text>
                </el-form-item>
                <el-form-item label="图书简介：">
                    <text>{{form.introduction}}</text>
                </el-form-item>
                <el-form-item label="图书语言：">
                    <text>{{form.language}}</text>
                </el-form-item>
                <el-form-item label="图书类型：">
                    <text>{{form.classId}}</text>
                </el-form-item>
                <el-form-item label="作者：">
                    <text>{{form.author}}</text>
                </el-form-item>
                <el-form-item label="ISBN：">
                    <text>{{form.isbn}}</text>
                </el-form-item>
                <el-form-item label="发布时间：">
                    <text>{{form.pubDate}}</text>
                </el-form-item>
                <el-form-item label="出版社：">
                    <text>{{form.publish}}</text>
                </el-form-item>
                <el-form-item label="图书数量：">
                    <text>{{form.number}}</text>
                </el-form-item>
            </el-form>
            <template #footer>
                <span class="dialog-footer">
                    <el-button @click="editVisible = false">取 消</el-button>
                    <el-button type="primary" @click="handleLend(form.index,form.number)">借 阅</el-button>
                </span>
            </template>
        </el-dialog>

    </div>
</template>

<script>

import { ref, reactive } from "vue";
import { ElMessage, ElMessageBox } from "element-plus";
import { getDataNoParam, getDataParam, insertData, deleteData, updateData } from "../../api/index";

export default {
    name: "bookTable",
    setup() {
        // 可视化 相关数据
        const addVisible = ref(false);
        const editVisible = ref(false);
        // data 相关数据
        const bookData = ref([]);
        const pageTotal = ref(0);
        const menu = ref([]);
        const line = ref(6);
        // 设定行列的初值
        const row = ref(4);
        const column = ref(9);

        const bookClass = ref([
            {
                class: 0,
                icon: "el-icon-lx-home",
                title: "全部图书",
            },
            {
                class: 1,
                icon: "el-icon-lx-home",
                title: "马克思主义",
            },
            {
                class: 2,
                icon: "el-icon-lx-home",
                title: "哲学",
            },
            {
                class: 3,
                icon: "el-icon-lx-home",
                title: "社会科学理论",
            },
        ]);
        // request 相关数据
        const query = reactive({
            mode:"id",
            options:"",          
            pageIndex:1,
            pageSize:10,
        });

        const lendQuery = reactive({
            serNum:"",
            bookId:"",
            readerId:"",
            lendDate:"",
            backDate:"",
        })
        // 表单
        const form = reactive({
            bookId:"",
            name: "",
            author:"",
            classId:"",
            isbn:"",
            introduction:"",
            language:"",
            price:"",
            number:"",
            pubDate:"",
            publish:"",
            index:"",
        });

        // 表单规则
        const ruleFormRef = ref()

        /** 定义方法 */ 

        // 获取表格数据
        const getFormData = () => {
            getDataParam(query,"/book/queryBook").then((res) => {
                console.log(res)
                bookData.value = res.data
                pageTotal.value = res.pageTotal || 10
            });
        };

        // 添加图书数据
        const addLendData = (data) => {
            insertData(data,"/lend/lendBook").then((res) => {
                console.log(res.data);
            });
        }

        /** 按钮事件  */

        // 分页导航
        const handlePageChange = (val) => {
            query.pageIndex = val;
            getFormData();
        };
        // 添加操作
        const handleAdd = () => {
            addVisible.value = true;
        }


        // 查看操作
        const handleDetail = (index) => {
            console.log(bookData.value[index]);
            var book = bookData.value[index];
            Object.keys(form).forEach((item) => {
                form[item] = book[item];
            });
            form.index = index;
            editVisible.value = true;
        };

        // 借阅操作
        const handleLend = (index,number) => {
            console.log(index);
            lendQuery.bookId = bookData.value[index].bookId;
            lendQuery.readerId = 10001;
            lendQuery.lendDate = formatDate(Date());
            console.log(lendQuery);
            if(number >= 1){
                ElMessageBox.confirm("确定要借阅吗？", "提示", {
                    confirmButtonText: '确认',
                    cancelButtonText: '取消',
                    type: "warning",
                })
                .then(() => {
                    addLendData(lendQuery);
                    ElMessage.success("借阅成功");
                    getFormData();
                })
            } else {
                ElMessage.error(`无法借阅：没有剩余图书`);
            }
        };

        // 查询操作
        const handleSearch = () => {
            query.pageIndex = 1;
            console.log(query);
            if(query.mode == 'id') {
                getDataParam(query,"/book/getBookByBookId").then((res) => {
                    console.log(res)
                    bookData.value = res.data
                    pageTotal.value = res.pageTotal 
                });
            }else if(query.mode == 'keyword') {
                getDataParam(query,"/book/queryBook").then((res) => {
                    console.log(res)
                    bookData.value = res.data
                    pageTotal.value = res.pageTotal
                });
            }
        };
        const searchForClass = (index) => {
            console.log(index);
            if(index.class == 0) {
                query.options = "",
                getDataParam(query,"/book/queryBook").then((res) => {
                    console.log(res)
                    bookData.value = res.data
                    pageTotal.value = res.pageTotal
                });
            } else {
                query.options = index.class;
                getDataParam(query,"/book/getBookByClassId").then((res) => {
                    console.log(res)
                    bookData.value = res.data
                    pageTotal.value = res.pageTotal
                });
            }
        }
        // 保存新增内容 
        const saveCreate = () => {
            addVisible.value = false;
            console.log(ruleForm)
            addBookData(ruleForm);
            ElMessage.success(`添加新用户成功`);
            getFormData();
        }

        const formatDate = (time) =>{
            var date = new Date(time);

            var year = date.getFullYear(),
                month = date.getMonth() + 1,//月份是从0开始的
                day = date.getDate(),
                hour = date.getHours(),
                min = date.getMinutes(),
                sec = date.getSeconds();
                var newTime = year + '-' +
                    (month < 10? '0' + month : month) + '-' +
                    (day < 10? '0' + day : day) + 'T' +
                    (hour < 10? '0' + hour : hour) + ':' +
                    (min < 10? '0' + min : min) + ':' +
                    (sec < 10? '0' + sec : sec)+'.000Z';
            return newTime;	
        }

        const handleChange = (value) => {
            console.log(value);
        }
        // setup时执行的函数
        getFormData();

        return {
            query,
            lendQuery,
            bookData,
            pageTotal,
            editVisible,
            addVisible,
            form,
            menu,
            row,
            column,
            bookClass,
            line,
            handlePageChange,
            handleSearch,
            handleAdd,
            saveCreate,
            handleDetail,
            handleChange,
            handleLend,
            searchForClass,
        };
    },
};
</script>

<style>
.el-table .warning-row td{
    background:#fdf6ec !important;
}

/* .el-table .success-row td{
    background:#f0f9eb !important;
} */
.handle-box {
    margin-bottom: 20px;
}

.handle-select {
    width: 200px;
}

.handle-input {
    width: 300px;
    display: inline-block;
}

.red {
    color: #ff0000;
}
.mr10 {
    margin-right: 10px;
}

.table-td-thumb {
    display: block;
    margin: auto;
    width: 40px;
    height: 40px;
}

/* 分栏 */


.el-col {
    border: #fdf6ec;
    border-radius: 20px;
}

.el-row {
    display: flex;
    flex-wrap: wrap;
    margin-bottom: 20px;

}

.el-menu {
    border-radius: 10px;
    border: 1px solid #999;
}
.el-menu-item {
    border-radius: 10px;
}

.book-card {
    margin-top: 20rpx;
    margin-bottom: 20rpx;
}
.book-title {
    font-size: larger;
    font-weight: 800;
}
.book-author {
    margin-left: 10px;
    font-size: medium;
    font-weight: 400;
    font-family: 'Courier New', Courier, monospace;
}


.time {
  font-size: 12px;
  color: #999;
}

.bottom {
  margin-top: 13px;
  line-height: 12px;
  display: flex;
  justify-content: space-between;
  align-items: center;
}

.button {
  padding: 0;
  min-height: auto;
}

.image {
  width: 100%;
  display: block;
}

.image-detail {
    margin-left: 32%;
    width: 200px;
}
</style>
