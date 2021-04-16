//引入express 模块
const express = require('express');
const app = express()
app.use(express.static('public'))

//引入mongodb
var MongoClient = require('mongodb').MongoClient;
//mongodb的端口号和数据库名字，如果数据库不存在会被创建
var dburl = 'mongodb://localhost:27017';

/*-------------------------------------------------------------------- */

var rtData = {    //用来保存温湿度
    temp: 0,
    humi: 0
};
var sqlData;
var returnData;


var count = 0;
/*-------------------------------------------------------------------- */
app.get('/set', (req, res) => {
    if (req.query.temp != undefined && req.query.humi != undefined) {
        rtData.temp = req.query.temp;
        rtData.humi = req.query.humi;
    }
    returnData = {
        rtData: rtData,
        sqlData: sqlData
    }
})
app.post('/get', (req, res) => {
    if((count++)==6){
        count = 0;
        addDate();
    }
    addDate();
    getDate();
    res.type("application/json");
    res.jsonp(returnData);
})
/*-------------------------------------------------------------------- */
//数据库代码


//查
function getDate(callback) {
    //console.log("进入getDate（）函数");
    // 标准动作，连接数据库之后然后对数据库进行CRUD操作
    const client = new MongoClient(dburl, { useNewUrlParser: true });
    client.connect(err => {
        const collection = client.db("stm32").collection("th");
        // perform actions on the collection object
        collection.find({}).toArray(function (err, arr) {
            returnData = {
                rtData: rtData,
                sqlData: arr
            }
        });
        client.close();
    });
}
//增
function addDate() {
    console.log("进入addDate（）函数");
    // 标准动作，连接数据库之后然后对数据库进行CRUD操作
    const client = new MongoClient(dburl, { useNewUrlParser: true});
    client.connect(err => {
        const collection = client.db("stm32").collection("th");
        // perform actions on the collection object
        collection.insertOne({ "temp": rtData.temp, "humi": rtData.humi }, function (err, result) {
            console.log("执行addData()函数")
            //callback(result);
        });
        client.close();
    });
}

//删除
function removeDate() {
    //console.log("进入removeDate（）函数");
    // 标准动作，连接数据库之后然后对数据库进行CRUD操作
    const client = new MongoClient(dburl, { useNewUrlParser: true});
    client.connect(err => {
        const collection = client.db("stm32").collection("th");
        // perform actions on the collection object
        collection.deleteOne({ a: 3 }, function (err, result) {
            //callback(result);
        });
        client.close();
    });
}

/*-------------------------------------------------------------------- */
// 开启服务器
app.listen(3000, () => {
    console.log('服务器在3000端口开启。。。。。');
})