//引入express 模块
const express = require('express');
const app = express()
app.use(express.static('public'))
const axios = require('axios').default;
//引入mongodb
var MongoClient = require('mongodb').MongoClient;
//mongodb的端口号和数据库名字，如果数据库不存在会被创建
var dburl = 'mongodb://localhost:27017';
var ObjectId = require('mongodb').ObjectId; //Work
/*-------------------------------------------------------------------- */

var rtData = { //用来保存温湿度
    temp: 0,
    humi: 0
};
var sqlData = [];

var count = 0;
/*-------------------------------------------------------------------- */
app.get('/StmSet', (req, res) => {
    if (req.query.temp != undefined && req.query.humi != undefined) {
        rtData.temp = req.query.temp;
        rtData.humi = req.query.humi;
    }
    if ((count++) == 60) { //一分钟记录一次数据库
        count = 0;
        addDate(rtData);
    }
})
app.post('/webGetData', (req, res) => {
    req.on('data', (data) => {
        let obj = JSON.parse(data);
        res.type("application/json");
        if (obj.req == "rt") {
            res.jsonp(rtData);

        } else {
            getDate();
            res.jsonp(sqlData)

        }
    })
})
app.post('/webReqDel', (req, res) => {
    req.on('data', function (data) {
        let obj = JSON.parse(data);
        //console.log("收到删除请求")
        removeDate(obj);
        //console.log("删除成功后")
        getDate();
        //console.log("更新数据")
        res.type("application/json");
        res.jsonp(sqlData);

    })
})
app.post('/webReqSave', (req, res) => {
    req.on('data', function (data) {
        let obj = JSON.parse(data);
        addDate(obj.rt);
        getDate();
        res.type("application/json");
        res.jsonp(sqlData);

    })
})
/*-------------------------------------------------------------------- */
//数据库代码


//查
function getDate(callback) {
    //console.log("进入getDate（）函数");
    // 标准动作，连接数据库之后然后对数据库进行CRUD操作
    const client = new MongoClient(dburl, {
        useNewUrlParser: true
    });
    client.connect(err => {
        const collection = client.db("stm32").collection("th");
        // perform actions on the collection object
        collection.find({}).toArray(function (err, arr) {
            sqlData = arr
        });
        client.close();
    });
}
//增
function addDate(data) {
    //console.log("进入addDate（）函数");
    let tData;
    axios
        .post('http://quan.suning.com/getSysTime.do')
        .then(response => {
            tData = {
                time: response.data.sysTime2,
                temp: data.temp,
                humi: data.humi
            }
            //this.items = response.data
            //console.log(tData)
            // 标准动作，连接数据库之后然后对数据库进行CRUD操作
            const client = new MongoClient(dburl, {
                useNewUrlParser: true
            });
            client.connect(err => {
                const collection = client.db("stm32").collection("th");
                // perform actions on the collection object
                collection.insertOne(tData, function (err, result) {});
                client.close();
            });
        })
        .catch(function (error) { // 请求失败处理
            console.log(error);
        });

}

//删除
function removeDate(id) {
    console.log("进入removeDate（）函数", id);
    // 标准动作，连接数据库之后然后对数据库进行CRUD操作
    const client = new MongoClient(dburl, {
        useNewUrlParser: true
    });
    client.connect(err => {
        const collection = client.db("stm32").collection("th");
        // perform actions on the collection object
        collection.findOneAndDelete({
            "_id": ObjectId(id._id)
        }, function (err, result) {});
        client.close();
    });
}

/*-------------------------------------------------------------------- */
// 开启服务器
app.listen(3000, () => {
    console.log('服务器在3000端口开启。。。。。');
})