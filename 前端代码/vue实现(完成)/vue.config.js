const webpack = require('webpack')
module.exports = {
  configureWebpack: {
    plugins: [
      new webpack.ProvidePlugin({
        $: "jquery",

        jQuery: "jquery",
        "windows.jQuery": "jquery"
      })
    ]
  },
  chainWebpack: config => {
    config
      .plugin('html')
      .tap(args => {
        args[0].title = 'vue实现的前端页面'
        return args
      })
  }
}