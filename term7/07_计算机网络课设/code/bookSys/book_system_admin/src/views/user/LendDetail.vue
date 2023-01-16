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
            <!-- 图书表单 -->
            <el-table :data="lendData" border class="table" ref="multipleTable" header-cell-class-name="table-header">
                <el-table-column prop="serNum" label="借阅ID"  align="center"></el-table-column>
                <el-table-column prop="bookId" label="图书ID" align="center"></el-table-column>
                <el-table-column prop="readerId" label="借阅者ID" align="center">
                    <template #default="scope">
                        <el-tag>{{ scope.row.readerId }}</el-tag>
                    </template>
                </el-table-column>
                <el-table-column label="借书日期" align="center">
                    <template #default="scope">
                        <el-tag type="success">{{ scope.row.lendDate }}</el-tag>
                    </template>
                </el-table-column>
                <el-table-column label="还书日期" align="center"      
                :filters="[
                    { text: '已归还', value: 'back' },
                    { text: '借阅中', value: 'no-back' },
                ]"
                :filter-method="filterTag"
                filter-placement="bottom-end">
                    <template #default="scope">
                        <el-tag type="success" v-if="scope.row.backDate">已归还</el-tag>
                        <el-tag v-if="scope.row.backDate">{{ scope.row.backDate }}</el-tag>
                        <el-tag type="warning" v-else>借阅中</el-tag>
                    </template>
                </el-table-column>
            </el-table>
            <!-- 翻页器 -->
            <div class="pagination">
                <el-pagination background layout="total, prev, pager, next" :current-page="query.pageIndex"
                    :page-size="query.pageSize" :total="pageTotal" @current-change="handlePageChange"></el-pagination>
            </div>
        </div>

        <!-- 归还弹出框 -->
        <el-dialog title="归还图书" v-model="editVisible" width="20%" center>
            <span>
                您是否要归还该图书？（系统将记录您的归还时间）
            </span>
            <template #footer>
                <span class="dialog-footer">
                    <el-button @click="editVisible = false">取 消</el-button>
                    <el-button type="primary" @click="saveEdit">确 定</el-button>
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
    name: "lendTable",
    setup() {
        // 可视化 相关数据
        const addVisible = ref(false);
        const editVisible = ref(false);
        // data 相关数据
        const lendData = ref([]);
        const pageTotal = ref(0);
        const menu = ref([]);
        // 设定行列的初值
        const row = ref(4);
        const column = ref(9);
        // request 相关数据
        const query = reactive({
            mode:"id",
            options:"10001",
            pageIndex:1,
            pageSize:10,
        });
        // 表单
        const form = reactive({
            serNum:"",
            bookId:"",
            readerId:"10001",
            lendDate:"",
            backDate:"",
        });

        const deleteParam = reactive({
            serNum:"",
        })

        // 表单规则
        const ruleFormRef = ref()

        /** 定义方法 */

        // 获取表格数据
        const getFormData = () => {
            getDataParam(query,"/lend/getLendByReaderId").then((res) => {
                console.log(res)
                lendData.value = res.data
                pageTotal.value = res.pageTotal;
                for(var i = 0;i < res.pageTotal; i++) {
                    // console.log(lendData.value[i].lendDate);
                    lendData.value[i].lendDate = formatDateShow(lendData.value[i].lendDate);
                    lendData.value[i].backDate = formatDateShow(lendData.value[i].backDate);
                }
            });
        };

        // 添加图书数据
        const addLendData = (data) => {
            insertData(data,"/lend/lendBook").then((res) => {
                console.log(res.data)
            });
        }
        // 更新图书数据
        const updateLendData = (data) => {
            updateData(data,"/lend/returnBook").then((res) => {
                console.log(res.data)
                // refresh;
            });
        }

        // 删除图书数据
        const deleteLendData = (data) => {
            deleteData(data,"/lend/deleteLend").then((res) => {
                console.log(res.data)
                // refresh;
            });
        }

        /** 按钮事件  */
        const filterTag = (value, row) => {
            console.log(row)
            if(value == 'back') {
                return row.backDate != null;
            } else if(value == 'no-back'){
                return row.backDate == null;
            }
        }

        // 分页导航
        const handlePageChange = (val) => {
            query.pageIndex = val;
            getFormData();
        };

        // 编辑操作
        let idx = -1;
        const handleEdit = (index, row) => {
            idx = index;
            Object.keys(form).forEach((item) => {
                form[item] = row[item];
                console.log(form[item]);
            });
            form.backDate = formatDateShow(Date());
            console.log(form.backDate)
            editVisible.value = true;
        };

        // 删除操作
        const handleDelete = (index, row) => {
            // 二次确认删除
            ElMessageBox.confirm("确定要删除吗？", "提示", {
                confirmButtonText: '确认',
                cancelButtonText: '取消',
                type: "warning",
            })
            .then(() => {
                deleteParam.serNum = row.serNum
                deleteLendData(deleteParam);
                ElMessage.success("删除成功");
                getFormData();
            })
            .catch(() => {});
        };

        // 查询操作
        const handleSearch = () => {
            query.pageIndex = 1;
            if(query.mode == 'id') {
                getDataParam(query,"/lend/getLendByReaderId").then((res) => {
                    console.log(res)
                    lendData.value = res.data
                    pageTotal.value = res.pageTotal || 10
                });
            }
        };

        // 保存编辑内容
        const saveEdit = () => {
            editVisible.value = false;
            Object.keys(form).forEach((item) => {
                lendData.value[idx][item] = form[item];
            });
            console.log(lendData.value[idx])
            updateLendData(lendData.value[idx]);
            ElMessage.success(`修改第 ${idx + 1} 行成功`);
            getFormData();
        };

        const formatDateShow = (time) =>{
            if(time != null){
                var date = new Date(time);

                var year = date.getFullYear(),
                    month = date.getMonth() + 1,//月份是从0开始的
                    day = date.getDate(),
                    hour = date.getHours(),
                    min = date.getMinutes(),
                    sec = date.getSeconds();
                var newTime = year + '-' +
                    (month < 10? '0' + month : month) + '-' +
                    (day < 10? '0' + day : day) + ' ' +
                    (hour < 10? '0' + hour : hour) + ':' +
                    (min < 10? '0' + min : min) + ':' +
                    (sec < 10? '0' + sec : sec);
                return newTime;
            }
            return '';
        }


        // setup时执行的函数
        getFormData();

        return {
            query,
            lendData,
            pageTotal,
            editVisible,
            addVisible,
            form,
            ruleFormRef,
            deleteParam,
            menu,
            row,
            column,
            filterTag,
            handlePageChange,
            handleDelete,
            handleSearch,
            handleEdit,
            saveEdit,
        };
    },
};
</script>

<style scoped>
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
.table {
    width: 100%;
    font-size: 14px;
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
</style>
