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
                <el-input v-model="query.options" placeholder="请输入参数" class="handle-input mr10"></el-input>
                <el-button type="primary" icon="el-icon-search" @click="handleSearch">搜索</el-button>
                <el-button type="success" icon="el-icon-plus" @click="handleAdd">添加图书</el-button>
            </div>
            <!-- 图书表单 -->
            <el-table :data="bookData" :row-class-name="tableRowClassName" border class="table" ref="multipleTable"
                header-cell-class-name="table-header">
                <el-table-column prop="bookId" label="图书ID" align="center"></el-table-column>
                <el-table-column prop="name" label="图书名称" align="center"></el-table-column>
                <el-table-column prop="classId" label="图书类型" align="center"></el-table-column>
                <el-table-column prop="author" label="作者" align="center"></el-table-column>
                <el-table-column label="语言" align="center">
                    <template #default="scope">
                        <el-tag type="success">{{ scope.row.language }}</el-tag>
                    </template>
                </el-table-column>
                <el-table-column label="ISBN" align="center">
                    <template #default="scope">
                        <el-tag>{{ scope.row.isbn }}</el-tag>
                    </template>
                </el-table-column>
                <el-table-column label="发布时间" align="center">
                    <template #default="scope">
                        <el-tag>{{ scope.row.pubDate }}</el-tag>
                    </template>
                </el-table-column>
                <el-table-column prop="publish" label="出版社" align="center"></el-table-column>
                <el-table-column prop="number" label="剩余数量" align="center"></el-table-column>
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
        <el-dialog title="编辑图书信息" v-model="editVisible" width="30%">
            <el-form label-width="100px">
                <el-form-item label="图书ID：">
                    <el-input v-model="form.bookId" disabled placeholder=""></el-input>
                </el-form-item>
                <el-form-item label="图书名称：">
                    <el-input v-model="form.name" placeholder=""></el-input>
                </el-form-item>
                <el-form-item label="图书类型：">
                    <el-input v-model="form.classId" placeholder=""></el-input>
                </el-form-item>
                <el-form-item label="作者：">
                    <el-input v-model="form.author" placeholder=""></el-input>
                </el-form-item>
                <el-form-item label="ISBN：">
                    <el-input v-model="form.isbn" placeholder=""></el-input>
                </el-form-item>
                <el-form-item label="发布时间：">
                    <el-date-picker v-model="form.pubDate" type="datetime" placeholder="选择日期" format="YYYY-MM-DD"
                        value-format="YYYY-MM-DD" />
                </el-form-item>
                <el-form-item label="出版社：">
                    <el-input v-model="form.publish" placeholder=""></el-input>
                </el-form-item>
                <el-form-item label="图书数量：">
                    <el-input-number v-model="form.number" :min="0" :max="1000" @change="handleChange" />
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
        <el-dialog title="添加新图书" v-model="addVisible" width="30%">
            <el-form :model="ruleForm" ref="ruleFormRef" :rules="addRules" label-width="100px">
                <el-form-item label="图书名称：" prop="name">
                    <el-input v-model="ruleForm.name" placeholder=""></el-input>
                </el-form-item>
                <el-form-item label="图书类型：" prop="classId">
                    <el-input v-model="ruleForm.classId" placeholder=""></el-input>
                </el-form-item>
                <el-form-item label="作者：" prop="author">
                    <el-input v-model="ruleForm.author" placeholder=""></el-input>
                </el-form-item>
                <el-form-item label="ISBN：" prop="isbn">
                    <el-input v-model="ruleForm.isbn" placeholder=""></el-input>
                </el-form-item>
                <el-form-item label="发布时间：" prop="pubDate">
                    <el-date-picker v-model="ruleForm.pubDate" type="datetime" placeholder="选择日期" format="YYYY-MM-DD"
                        value-format="YYYY-MM-DD" />
                </el-form-item>
                <el-form-item label="出版社：" prop="publish">
                    <el-input v-model="ruleForm.publish" placeholder=""></el-input>
                </el-form-item>
                <el-form-item label="图书数量：" prop="number">
                    <el-input-number v-model="ruleForm.number" :min="0" :max="1000" @change="handleChange" />
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

import { ref, reactive } from "vue"
import { ElMessage, ElMessageBox } from "element-plus"
import { getDataNoParam, getDataParam, insertData, deleteData, updateData } from "../../api/index"

export default {
    name: "bookTable",
    setup () {
        // 可视化 相关数据
        const addVisible = ref(false)
        const editVisible = ref(false)
        // data 相关数据
        const bookData = ref([])
        const pageTotal = ref(0)
        const menu = ref([])
        // 设定行列的初值
        const row = ref(4)
        const column = ref(9)
        // request 相关数据
        const query = reactive({
            mode: "id",
            options: "",
            pageIndex: 1,
            pageSize: 10,
        })
        // 表单
        const form = reactive({
            bookId: "",
            name: "",
            author: "",
            classId: "",
            isbn: "",
            introduction: "",
            language: "",
            price: "",
            number: "",
            pubDate: "",
            publish: "",
        })
        // 规则校验表单
        const ruleForm = reactive({
            bookId: "",
            name: "",
            author: "",
            classId: "",
            isbn: "",
            introduction: "",
            language: "",
            price: "",
            number: "",
            pubDate: "",
            publish: "",
        })

        const deleteParam = reactive({
            bookId: "",
        })

        // 表单规则
        const ruleFormRef = ref()

        // 校验规则
        const addRules = reactive({
            name: [
                { required: true, message: '请输入图书名称！', trigger: 'blur' },
                { min: 1, max: 20, message: '图书名称长度应介于1到20之间', trigger: 'blur' },
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
                { required: true, message: '请输入图书ISBN！', trigger: 'blur' },
                { min: 1, max: 60, message: '名称长度应介于1到60之间', trigger: 'blur' },
                { type: 'string', message: '请输入汉字！', trigger: 'blur' },
            ],

            introduction: [
                { required: true, message: '请输入图书名称！', trigger: 'blur' },
                { min: 1, max: 20, message: '图书名称长度应介于1到20之间', trigger: 'blur' },
                { type: 'string', message: '请输入汉字！', trigger: 'blur' },
            ],

            language: [
                { required: true, message: '请输入图书名称！', trigger: 'blur' },
                { min: 1, max: 20, message: '图书名称长度应介于1到20之间', trigger: 'blur' },
                { type: 'string', message: '请输入汉字！', trigger: 'blur' },
            ],

            price: [
                { required: true, message: '请输入图书名称！', trigger: 'blur' },
                { min: 1, max: 20, message: '图书名称长度应介于1到20之间', trigger: 'blur' },
                { type: 'string', message: '请输入汉字！', trigger: 'blur' },
            ],

            number: [
                { required: true, message: '请输入图书名称！', trigger: 'blur' },
                { min: 1, max: 20, message: '图书名称长度应介于1到20之间', trigger: 'blur' },
                { type: 'string', message: '请输入汉字！', trigger: 'blur' },
            ],

            pubDate: [
                { type: 'date', required: true, message: '请输入发布时间', trigger: 'change' },
            ],

            publish: [
                { required: true, message: '请输入图书！', trigger: 'blur' },
                { min: 1, max: 60, message: '名称长度应介于1到60之间', trigger: 'blur' },
                { type: 'string', message: '请输入汉字！', trigger: 'blur' },
            ],
        })

        /** 定义方法 */

        // 获取表格数据
        const getFormData = () => {
            getDataParam(query, "/book/queryBook").then((res) => {
                console.log(res)
                bookData.value = res.data
                pageTotal.value = res.pageTotal || 10
            })
        }

        // 添加图书数据
        const addBookData = (data) => {
            insertData(data, "/book/addBook").then((res) => {
                console.log(res.data)
            })
        }
        // 更新图书数据
        const updateBookData = (data) => {
            updateData(data, "/book/editBook").then((res) => {
                console.log(res.data)
                // refresh;
            })
        }

        // 删除图书数据
        const deleteBookData = (data) => {
            deleteData(data, "/book/deleteBook").then((res) => {
                console.log(res.data)
                // refresh;
            })
        }

        /** 按钮事件  */

        // 分页导航
        const handlePageChange = (val) => {
            query.pageIndex = val
            getFormData()
        }
        // 添加操作
        const handleAdd = () => {
            addVisible.value = true
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
        let idx = -1
        const handleEdit = (index, row) => {
            idx = index
            Object.keys(form).forEach((item) => {
                form[item] = row[item]
                console.log(form[item])
            })
            editVisible.value = true
        }

        // 删除操作
        const handleDelete = (index, row) => {
            // 二次确认删除
            ElMessageBox.confirm("确定要删除吗？", "提示", {
                type: "warning",
            })
                .then(() => {
                    deleteParam.bookId = row.bookId
                    deleteBookData(deleteParam)
                    ElMessage.success("删除成功")
                    getFormData()
                })
                .catch(() => { })
        }

        // 查询操作
        const handleSearch = () => {
            query.pageIndex = 1
            console.log(query)
            if (query.mode == 'id') {
                getDataParam(query, "/book/getBookByBookId").then((res) => {
                    console.log(res)
                    bookData.value = res.data
                    pageTotal.value = res.pageTotal || 10
                })
            } else if (query.mode == 'keyword') {
                getDataParam(query, "/book/queryBook").then((res) => {
                    console.log(res)
                    bookData.value = res.data
                    pageTotal.value = res.pageTotal || 10
                })
            }
        }

        // 保存新增内容 
        const saveCreate = () => {
            addVisible.value = false
            console.log(ruleForm)
            addBookData(ruleForm)
            ElMessage.success(`添加新用户成功`)
            getFormData()
        }

        // 保存编辑内容
        const saveEdit = () => {
            editVisible.value = false
            Object.keys(form).forEach((item) => {
                bookData.value[idx][item] = form[item]
            })
            console.log(bookData.value[idx])
            updateBookData(bookData.value[idx])
            ElMessage.success(`修改第 ${idx + 1} 行成功`)
            getFormData()
        }

        const handleChange = (value) => {
            console.log(value)
        }

        // setup时执行的函数
        getFormData()

        return {
            query,
            bookData,
            pageTotal,
            editVisible,
            addVisible,
            form,
            ruleForm,
            ruleFormRef,
            deleteParam,
            addRules,
            menu,
            row,
            column,
            tableRowClassName,
            handlePageChange,
            handleDelete,
            handleSearch,
            handleAdd,
            saveCreate,
            handleEdit,
            saveEdit,
            handleChange,
        }
    },
}
</script>

<style>
.el-table .warning-row td {
    background: #fdf6ec !important;
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
</style>
