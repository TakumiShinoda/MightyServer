const path = require('path');

module.exports = {
  mode: 'development',
  entry: './www/src/javascript/AddApi/entry.js',
  output: {
    filename: 'bundle.js',
    path: path.join(__dirname, 'www/dist/AddApi/')
  }
};