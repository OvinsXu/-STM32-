<template>
  <div id="app">
    <div class=".container-fluid">
      <div class="row">
       <div class="panel panel-primary">
               <div class="panel-heading">
                   <h3 class="panel-title">实时温湿度:</h3>
               </div>
               <div class="panel-body">
                   <h5>温度:{{rtData.temp}}</h5>
                   <h5>湿度:{{rtData.humi}}</h5>
               </div>
           </div>
       </div>
       <div class="row">
           <button type="button" class="col-xs-3 btn btn-lg btn-primary" style="margin-bottom: 20px;"
               @click="saveDataToSQL()">手动保存到数据库</button>
           <div class="col-xs-5 alert alert-warning alert-dismissable" style="height: 45px;">
               <button type="button" class="close" data-dismiss="alert" aria-hidden="true">
                   &times;
               </button>
               注意：一分钟会自动保存一次数据！
           </div>
       </div>
       <div class="row">
           <span class="col-xs-3 label label-default">序号</span>
           <span class="col-xs-3 label label-success">时间</span>
           <span class="col-xs-1 label label-info">温度</span>
           <span class="col-xs-1 label label-warning">湿度</span>
           <span class="col-xs-2 label label-danger">操作</span>
       </div>
       <div v-for="item in dataShow" :key="item._id" class="row" style="border:1px solid #ddd">
           <div class="col-xs-3" style="background-color: rgb(136, 133, 138);">{{item._id}}</div>
           <div class="col-xs-3" style="background-color: rgb(186, 245, 91);">{{item.time}}</div>
           <div class="col-xs-1" style="background-color: rgb(127, 215, 218);">{{item.temp}}</div>
           <div class="col-xs-1" style="background-color: rgb(207, 158, 53);">{{item.humi}}</div>
           <button class="col-xs-2" style="background-color: crimson;border:none;"
               @click="deleteByID(item._id)">删除</button>
       </div>
       <ul class="pagination">
           <li><a @click="prePage()">&laquo;</a></li>
           <li v-for="item in pageList" :key="item"><a @click="setPageNum(item)">第{{item+1}}页</a></li>
           <li><a @click="nextPage()">&raquo;</a></li>
       </ul>
   </div>

    </div>
    <!-- <img alt="Vue logo" src="./assets/logo.png">
    <HelloWorld msg="Welcome to Your Vue.js App"/> -->
  </div>
</template>

<script>
// import HelloWorld from "./components/HelloWorld.vue";
import axios from "axios";
export default {
  data() {
    return {
      timer: null, //定时器名称
      rtData: {},
      sqlData: [],
      // 所有页面的数据
      totalPage: [],
      // 每页显示数量
      pageSize: 15,
      // 共几页
      pageNum: 1,
      // 当前显示的数据
      dataShow: "",
      // 默认当前显示第一页
      currentPage: 0,
      pageList: [],
    };
  },
  methods: {
    nextPage() {
      if (this.currentPage === this.pageNum - 1) return;
      this.dataShow = this.totalPage[++this.currentPage];
    },
    setPageNum(i) {
      this.currentPage = i;
    },
    // 上一页
    prePage() {
      if (this.currentPage === 0) return;
      this.dataShow = this.totalPage[--this.currentPage];
    },
    async ajaxGetData(option) {
      await axios
        .post("http://localhost:3000/webGetData", {
          req: option,
        })
        .then((response) => {
          if (option == "rt") {
            this.rtData = response.data;
          } else {
            this.sqlData = response.data;
            // 根据后台数据的条数和每页显示数量算出一共几页,得0时设为1 ;
            //this.pageNum = 0;

            this.pageNum = Math.ceil(this.sqlData.length / this.pageSize) || 1;
            for (let i = 0; i < this.pageNum; i++) {
              // 每一页都是一个数组 形如 [['第一页的数据'],['第二页的数据'],['第三页数据']]
              // 根据每页显示数量 将后台的数据分割到 每一页,假设pageSize为5， 则第一页是1-5条，即slice(0,5)，第二页是6-10条，即slice(5,10)...
              this.pageList[i] = i;
              //this.totalPage = 0;
              this.totalPage[i] = this.sqlData.slice(
                this.pageSize * i,
                this.pageSize * (i + 1)
              );
            }
            // 获取到数据后显示第一页内容
            this.dataShow = this.totalPage[this.currentPage];
          }
        })
        .catch(function (error) {
          // 请求失败处理
          console.log(error);
        });
    },

    async deleteByID(id) {
      console.log("发送删除请求");
      await axios
        .post("http://localhost:3000/webReqDel", {
          _id: id,
        })
        .then((response) => {
          //console.log("成功删除",response.data.sqlData[0].temp)
          //this.sqlData = response.data
          //console.log(this.sqlData)
        })
        .catch(function (error) {
          // 请求失败处理
          console.log(error);
        });
    },
    async saveDataToSQL() {
      await axios
        .post("http://localhost:3000/webReqSave", {
          rt: this.rtData,
        })
        .then((response) => {
          //this.sqlData = response.data
          //console.log(this.sqlData)
        })
        .catch(function (error) {
          // 请求失败处理
          console.log(error);
        });
    },
  },
  //beforeCreate:function(){},                                    //创建前
  created: function () {
    //创建后
  },
  //beforeMount:function(){},                                     //挂载DOM前
  mounted: function () {
    //挂载DOM后
    //1秒请求一次实时数据
    this.ajaxGetData("rt"),
      /**
       * setTimeout函数用来指定某个函数或某段代码，在多少毫秒之后执行。它返回一个整数，表示定时器的编号，以后可以用来取消这个定时器。
       * 参数：1.执行语句函数，2.推迟时间，3.后面可加若干参数，用于执行语句函数的参数
       * 执行语句函数的上下文是全局
       */
      (this.timer = setInterval(() => {
        setTimeout(() => {
          this.ajaxGetData("rt"), this.ajaxGetData("sql");
        });
      }, 1000));
  },
  //beforeUpdate:function(){},                                    //视图更新前
  //updated:function(){},                                         //视图更新后
  //beforeDestroy:function(){},                                   //实例销毁之前调用
  //destroyed:function(){}                                        //实例销毁后调用

  // name: "App",
  // components: {
  //   HelloWorld,
  // },
};
</script>

<style>
#app {
  font-family: Avenir, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
  margin-top: 60px;
}
</style>
