const path = require('path');
const  UglifyJSPlugin = require('uglifyjs-webpack-plugin');

module.exports = {
  mode: 'production',
  entry: './www/src/javascript/AddApi/entry.js',
  output: {
    filename: 'bundle.js',
    path: path.join(__dirname, 'www/dist/AddApi/')
  },
  optimization: {
    minimizer: [
      new UglifyJSPlugin({ 
        uglifyOptions: {compress: {drop_console: true}},
      }),
    ],
  }, 
  module: {
    rules: [
      {
        test: /\.css/,
        use: [
          'style-loader',
          {
            loader: 'css-loader',
            options: {
              url: false,
              minimize: true,
              sourceMap: true,
            },
          },
        ],
      },
    ]
  }
};