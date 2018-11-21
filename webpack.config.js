const path = require('path');

module.exports = {
  config: (entry, name) => {
    return {
      mode: 'production',
      entry: entry,
      output: {
        filename: name,
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
    }
  }
}