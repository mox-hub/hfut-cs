<template>
    <div>
        <!-- 小页面标题 -->
        <div class="crumbs">
            <div class="el-breadcrumb"></div>
            <el-breadcrumb  separator="/">
                <el-breadcrumb-item>
                    <i class="el-icon-lx-cascades"></i> 合同信息管理
                </el-breadcrumb-item>
            </el-breadcrumb>
        </div>

        <!-- 合同表单 -->
        <div class="container">
            <!-- 标题工具栏 -->
            <div class="handle-box">
                <!-- 查询方式选择框 -->
                <el-select v-model="query.mode" placeholder="查询模式" class="handle-select mr10">
                    <el-option key="1" label="合同ID查询" value="id"></el-option>
                    <el-option key="2" label="合同关键字查询" value="keyword"></el-option>
                </el-select>
                <!-- 输入框 -->
                <el-input v-model="query.options" placeholder="请输入参数" class="handle-input mr10"></el-input>
                <el-button type="primary" icon="el-icon-search" @click="handleSearch">搜索</el-button>
                <el-button type="success" icon="el-icon-plus" @click="handleAdd">添加合同</el-button>
            </div>
            <!-- 合同表单 -->
            <el-table :data="orderData" :row-class-name="tableRowClassName" border class="table">
                <el-table-column prop="orderId" label="合同ID"  align="center"></el-table-column>
                <el-table-column prop="orderName" label="合同名称" align="center"></el-table-column>
                <el-table-column prop="supplier" label="供货商" align="center"></el-table-column>
                <el-table-column prop="item" label="物品" align="center"></el-table-column>
                <el-table-column label="number" align="center">
                    <template #default="scope">
                        <el-tag>{{ scope.row.number }}</el-tag>
                    </template>
                </el-table-column>
                <el-table-column label="开始时间" align="center">
                    <template #default="scope">
                        <el-tag>{{ scope.row.orderTime }}</el-tag>
                    </template>
                </el-table-column>
                <el-table-column label="结束时间" align="center">
                    <template #default="scope">
                        <el-tag v-if="scope.row.endTime">{{ scope.row.endTime }}</el-tag>
                    </template>
                </el-table-column>
                <el-table-column label="合同状态" align="center">
                    <template #default="scope">
                        <el-tag type="success" v-if="(scope.row.status == 1)">已结束</el-tag>
                        <el-tag type="warning" v-else>执行中</el-tag>
                    </template>
                </el-table-column>
                <!-- 操作栏 -->
                <el-table-column label="操作" width="180" align="center">
                    <template #default="scope">
                        <el-button type="text" icon="el-icon-edit" @click="handleEdit(scope.$index, scope.row)">编辑
                        </el-button>
                        <el-button type="text" icon="el-icon-delete" class="red"
                            @click="handleDelete(scope.$index, scope.row)">删除</el-button>
                    </template>
                </el-table-column>
            </el-table>
            <!-- 翻页器 -->
            <div class="pagination">
                <el-pagination background layout="total, prev, pager, next" :current-page="query.pageIndex"
                    :page-size="query.pageSize" :total="pageTotal" @current-change="handlePageChange"></el-pagination>
            </div>
        </div>

        <!-- 编辑弹出框 无校验 -->
        <el-dialog title="编辑合同信息" v-model="editVisible" width="30%">
            <el-form label-width="100px">
                <el-form-item label="合同ID：">
                    <el-input v-model="form.orderId" disabled placeholder=""></el-input>
                </el-form-item>
                <el-form-item label="合同名称：">
                    <el-input v-model="form.orderName" placeholder=""></el-input>
                </el-form-item>
                <el-form-item label="供货商：">
                    <el-input v-model="form.supplier" placeholder=""></el-input>
                </el-form-item>
                <el-form-item label="物品：">
                    <el-input v-model="form.item" placeholder=""></el-input>
                </el-form-item>
                <el-form-item label="合同数量：">
                    <el-input-number v-model="form.number" :min="0" :max="10000" @change="handleChange" />
                </el-form-item>
                <el-form-item label="发布时间：">
                    <el-date-picker
                        v-model="dateTime"
                        type="datetimerange"
                        range-separator="至"
                        start-placeholder="开始时间"
                        end-placeholder="结束时间"
                        format="YYYY-MM-DD hh:mm:ss"
                        value-format="YYYY-MM-DD hh:mm:ss"
                    />
                </el-form-item>
                <el-form-item label="合同状态：">
                    <el-select v-model="form.status" placeholder="合同状态" class="handle-select mr10">
                        <el-option key=0 label="执行中" value=0></el-option>
                        <el-option key=1 label="已结束" value=1></el-option>
                    </el-select>
                </el-form-item>
            </el-form>
            <template #footer>
                <span class="dialog-footer">
                    <el-button @click="editVisible = false">取 消</el-button>
                    <el-button type="primary" @click="saveEdit">确 定</el-button>
                </span>
            </template>
        </el-dialog>

        <!-- 添加弹出框 有校验 -->
        <el-dialog title="添加新合同" v-model="addVisible" width="30%">
            <el-form :model="ruleForm" ref="ruleFormRef" :rules="addRules" label-width="100px">
                <el-form-item label="合同名称：">
                    <el-input v-model="ruleForm.orderName" placeholder=""></el-input>
                </el-form-item>
                <el-form-item label="供货商：">
                    <el-input v-model="ruleForm.supplier" placeholder=""></el-input>
                </el-form-item>
                <el-form-item label="物品：">
                    <el-input v-model="ruleForm.item" placeholder=""></el-input>
                </el-form-item>
                <el-form-item label="合同数量：">
                    <el-input-number v-model="ruleForm.number" :min="0" :max="10000" @change="handleChange" />
                </el-form-item>
                <el-form-item label="发布时间：">
                    <el-date-picker
                        v-model="dateTime"
                        type="datetimerange"
                        range-separator="至"
                        start-placeholder="开始时间"
                        end-placeholder="结束时间"
                        format="YYYY-MM-DD hh:mm:ss"
                        value-format="YYYY-MM-DD hh:mm:ss"
                    />
                </el-form-item>
                <el-form-item label="合同状态：">
                    <el-select v-model="ruleForm.status" placeholder="合同状态" class="handle-select mr10">
                        <el-option key=0 label="执行中" value=0></el-option>
                        <el-option key=1 label="已结束" value=1></el-option>
                    </el-select>
                </el-form-item>
            </el-form>
            <template #footer>
                <span class="dialog-footer">
                    <el-button @click="addVisible = false">取 消</el-button>
                    <el-button type="primary" @click="saveCreate">确 定</el-button>
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
    name: "orderTable",
    setup() {
        // 可视化 相关数据
        const addVisible = ref(false);
        const editVisible = ref(false);
        // data 相关数据
        const orderData = ref([]);
        const massageData = ref([]);
        const pageTotal = ref(0);
        const msgTotal = ref(0);
        const menu = ref([]);
        const dateTime = ref([]);
        // 设定行列的初值
        const row = ref(4);
        const column = ref(9);
        // request 相关数据
        const query = reactive({
            mode:"id",
            options:"",
            pageIndex:1,
            pageSize:10,
        });
        // 表单
        const form = reactive({
            orderId:"",
            orderName: "",
            supplier:"",
            item:"",
            number:"",
            orderTime:"",
            endTime:"",
            status:"",
        });
        // 规则校验表单
        const ruleForm = reactive({
            orderId:"",
            orderName: "",
            supplier:"",
            item:"",
            number:"",
            orderTime:"",
            endTime:"",
            status:"",
        });

        const deleteParam = reactive({
            orderId:"",
        })

        // 表单规则
        const ruleFormRef = ref()

        // 校验规则
        const addRules = reactive({
            name: [
                { required: true, message: '请输入合同名称！', trigger: 'blur' },
                { min: 1, max: 20, message: '合同名称长度应介于1到20之间', trigger: 'blur' },
                { type: 'string', message: '请输入汉字！', trigger: 'blur' },
            ],

            author: [
                { required: true, message: '请输入作者名称！', trigger: 'blur' },
                { min: 1, max: 15, message: '作者名称长度应介于1到15之间', trigger: 'blur' },
            ],

            classId: [
                { required: true, message: '请输入作者名称！', trigger: 'blur' },
                { min: 1, max: 15, message: '作者名称长度应介于1到15之间', trigger: 'blur' },
            ],

            isbn: [
                { required: true, message: '请输入合同ISBN！', trigger: 'blur' },
                { min: 1, max: 60, message: '名称长度应介于1到60之间', trigger: 'blur' },
                { type: 'string', message: '请输入汉字！', trigger: 'blur' },
            ],

            introduction: [
                { required: true, message: '请输入合同名称！', trigger: 'blur' },
                { min: 1, max: 20, message: '合同名称长度应介于1到20之间', trigger: 'blur' },
                { type: 'string', message: '请输入汉字！', trigger: 'blur' },
            ],

            language: [
                { required: true, message: '请输入合同名称！', trigger: 'blur' },
                { min: 1, max: 20, message: '合同名称长度应介于1到20之间', trigger: 'blur' },
                { type: 'string', message: '请输入汉字！', trigger: 'blur' },
            ],

            price: [
                { required: true, message: '请输入合同名称！', trigger: 'blur' },
                { min: 1, max: 20, message: '合同名称长度应介于1到20之间', trigger: 'blur' },
                { type: 'string', message: '请输入汉字！', trigger: 'blur' },
            ],

            number: [
                { required: true, message: '请输入合同名称！', trigger: 'blur' },
                { min: 1, max: 20, message: '合同名称长度应介于1到20之间', trigger: 'blur' },
                { type: 'string', message: '请输入汉字！', trigger: 'blur' },
            ],

            pubDate: [
            { type: 'date', required: true, message: '请输入发布时间',trigger: 'change'},
            ],

            publish: [
                { required: true, message: '请输入合同！', trigger: 'blur' },
                { min: 1, max: 60, message: '名称长度应介于1到60之间', trigger: 'blur' },
                { type: 'string', message: '请输入汉字！', trigger: 'blur' },
            ],
        })

        /** 定义方法 */ 

        // 获取表格数据
        const getFormData = () => {
            getDataParam(query,"/order/queryOrder").then((res) => {
                console.log(res)
                var now = Date.now();
                console.log(now)
                orderData.value = res.data
                pageTotal.value = res.pageTotal || 10
                for(var i = 0;i < res.pageTotal; i++) {
                    var end = Date.parse(orderData.value[i].endTime);
                    console.log(end);
                    orderData.value[i].orderTime = formatDateShow(orderData.value[i].orderTime);
                    orderData.value[i].endTime = formatDateShow(orderData.value[i].endTime);
                }
            });
        };


        const getMessageData = () => {
            getDataNoParam("/order/getOrderMessage").then((res) => {
                console.log(res)
                massageData.value = res.data
                console.log(massageData.value[0]);
                msgTotal.value = res.pageTotal
                for(var i=0;i<=res.pageTotal;i++) {
                    var title = massageData.value[i].title;
                    showMessage(title);
                }
            });
        };

        // 添加合同数据
        const addOrderData = (data) => {
            insertData(data,"/order/createOrder").then((res) => {
                console.log(res.data);
            });
        }
        // 更新合同数据
        const updateOrderData = (data) => {
            updateData(data,"/order/updateOrder").then((res) => {
                console.log(res.data)
                // refresh;
            });
        }

        // 删除合同数据
        const deleteOrderData = (data) => {
            deleteData(data,"/order/deleteOrder").then((res) => {
                console.log(res.data)
                // refresh;
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

        // 表格表示行颜色
        const tableRowClassName = (row) => {
            if (row.row.number >= 1) {
                return 'success-row'
            } else if (row.row.number == 0) {
                return 'warning-row'
            }
            return ''
        }
        // 编辑操作
        let idx = -1;
        const handleEdit = (index, row) => {
            idx = index;
            Object.keys(form).forEach((item) => {
                form[item] = row[item];
            });

            dateTime.value.push(row.orderTime);
            dateTime.value.push(row.endTime);
            editVisible.value = true;
        };

        // 删除操作
        const handleDelete = (index, row) => {
            // 二次确认删除
            ElMessageBox.confirm("确定要删除吗？", "提示", {
                type: "warning",
            })
            .then(() => {
                deleteParam.orderId = row.orderId
                deleteOrderData(deleteParam);
                ElMessage.success("删除成功");
                getFormData();
            })
            .catch(() => {});
        };

        // 查询操作
        const handleSearch = () => {
            query.pageIndex = 1;
            console.log(query);
            if(query.mode == 'id') {
                getDataParam(query,"/order/queryOrder").then((res) => {
                    console.log(res)
                    orderData.value = res.data
                    pageTotal.value = res.pageTotal || 10
                });
            }else if(query.mode == 'keyword') {
                getDataParam(query,"/order/queryOrder").then((res) => {
                    console.log(res)
                    orderData.value = res.data
                    pageTotal.value = res.pageTotal || 10
                });
            }
        };

        // 保存新增内容
        const saveCreate = () => {
            addVisible.value = false;
            console.log(ruleForm)
            ruleForm.orderTime = formatDate(dateTime.value[0]);
            ruleForm.endTime = formatDate(dateTime.value[1]);
            addOrderData(ruleForm);
            ElMessage.success(`添加新合同成功`);
            getFormData();
        }

        // 保存编辑内容
        const saveEdit = () => {
            editVisible.value = false;
            Object.keys(form).forEach((item) => {
                orderData.value[idx][item] = form[item];
            });
            orderData.value[idx].orderTime = formatDate(dateTime.value[0]);
            orderData.value[idx].endTime = formatDate(dateTime.value[1]);
            console.log(orderData.value[idx])
            updateOrderData(orderData.value[idx]);
            ElMessage.success(`修改第 ${idx + 1} 行成功`);
            getFormData();
        };

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

        const showMessage = (title) => {
            setTimeout(() => {
                ElMessage({
                            showClose: true,
                            duration: 0,
                            message: title,
                            type: 'warning',
                        })
            }, 10);
        }

        const handleChange = (value) => {
            console.log(value);
        }

        // setup时执行的函数
        getFormData();
        getMessageData();

        return {
            query,
            orderData,
            massageData,
            pageTotal,
            msgTotal,
            editVisible,
            addVisible,
            form,
            ruleForm,
            ruleFormRef,
            deleteParam,
            addRules,
            dateTime,
            menu,
            row,
            column,
            tableRowClassName,
            handlePageChange,
            getMessageData,
            handleDelete,
            handleSearch,
            handleAdd,
            handleEdit,
            handleChange,
            saveEdit,
            saveCreate,
        };
    },
};
</script>

<style>

.el-table .warning-row td{
    background:#fdf6ec !important;
}

.el-table .success-row td{
    background:#f0f9eb !important;
}
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
/* .table {
    width: 100%;
    font-size: 14px;
} */

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
