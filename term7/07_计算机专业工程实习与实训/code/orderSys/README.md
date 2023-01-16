# Database-Manager

### 项目介绍

数据库管理系统，使用前后端分离技术，没什么技术含量，纯套用模板。

> PS：如有合工大CS的学生，这个是数据库课设，可以拿来参考

- 前端使用Vue 3 + Element Plus 
  - 参考项目地址[lin-xin/vue-manage-system: 基于Vue3 + Element Plus 的后台管理系统解决方案 (github.com)](https://github.com/lin-xin/vue-manage-system)

- 后端使用Spring Boot + Mybatis-plus
- 使用mySQL数据库

### 实现功能

- 数据库的连接实现
- 数据库内容的增删改查
- 数据库数据展示
- 添加数据时的验证功能
- 登录功能（可能没实现）
- 图表数据展示功能（目前是固定数据）

### 仓库结构

```
├─database-manager-server
│  │  pom.xml     项目的pom管理
│  └─src	     标准的spring boot 后端结构
│      ├─main
│      │  ├─java
│      │  │  └─com
│      │  │      └─foocode
│      │  │          └─manager
│      │  │              │  ServerApplication.java      项目启动类
│      │  │              ├─config  					   配置类
│      │  │              ├─controller   
│      │  │              ├─mapper 
│      │  │              ├─model  
│      │  │              ├─service
│      │  │              │  └─impl       
│      │  │              └─utils
│      │  └─resources
│      │      │  application.properties    
│      │      └─templates            
│      └─test                          
├─database-manager-web
│  ├─public 
│  ├─screenshots
│  └─src
│      │  App.vue
│      │  main.js
│      ├─api   
│      ├─assets      样式 
│      ├─components
│      ├─plugins
│      ├─router 
│      ├─store 
│      ├─utils
│      └─views        页面存放位置
└─sql
        graduate.sql    项目的数据库文件
        
```

### 使用指南

#### 1-1. 下载源码

请从本仓库下载源码，使用git下载或者使用zip下载

#### 1-2. 配置后端Java环境

本项目使用Java 1.8版本（202以上），可以从各种渠道下载

#### 1-3.加载maven项目

- 使用IDEA通过database-manager-server里的pom.xml打开项目
- IDEA会自动加载maven项目，如果出现问题请使用IDEA自带的maven工具刷新

#### 1-4. 配置数据库

- 请先下载mysql 8.0（版本没有要求）网上有教程
- 下载Navicat或者其他数据库可视化管理软件
- 新建数据库
- 导入项目文件sql/database.sql
- 在后端项目中修改文件 application.properties 的以下属性，它位于src/main/resources文件夹中

```properties
# 请记得替换下面的汉字提示
# 数据库连接地址
spring.datasource.url=jdbc:mysql://localhost:3306/你的数据库名字?serverTimezone=UTC
# 数据库用户名&密码：
spring.datasource.username=你的数据库用户名
spring.datasource.password=你的数据库密码
```

##### 1-5. 启动后端项目

#### 2-1.配置前端项目环境

- 使用 VS code 打开database-manager-web文件夹
- 依次在终端执行以下指令

```bash
// 安装项目依赖，等待安装完成之后，安装失败可用 cnpm 或 yarn
npm install 

// 开启服务器，浏览器访问 http://localhost:8080
npm run dev

// 执行构建命令，生成的dist文件夹放在服务器下即可访问
npm run build
```

#### 2-2.使用admin登录完成

- 用户名：admin
- 密码：任意

### 项目鸣谢

- [lin-xin/vue-manage-system: 基于Vue3 + Element Plus 的后台管理系统解决方案](https://github.com/lin-xin/vue-manage-system)
- [baomidou/mybatis-plus: An powerful enhanced toolkit of MyBatis for simplify development](https://github.com/baomidou/mybatis-plus)
- [modood/Administrative-divisions-of-China: 中华人民共和国行政区划：省级（省份）、 地级（城市）、 县级（区县）、 乡级（乡镇街道）、 村级（村委会居委会） ，中国省市区镇村二级三级四级五级联动地址数据。 ](https://github.com/modood/Administrative-divisions-of-China)

### License

MIT