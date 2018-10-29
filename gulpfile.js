const gulp = require('gulp');
const pug = require('gulp-pug');
const plumber = require('gulp-plumber');
const notify = require('gulp-notify');
const webpack = require('webpack');
const webpackStream = require('webpack-stream');
const ex = require('express');

const Root = './www/dist';
const webpackConfig = require('./webpack.config');
const pugOptions = {
  pretty: false
}
const webpackEntries = [
  {
    path: './www/src/javascript/AddApi/index.js',
    dist: './www/dist/AddApi',
    name: 'index.js'
  }
]

gulp.task("webpack-compile", () => {
  return new Promise((res) => {
    for(var i = 0; i < webpackEntries.length; i++){
      let entry = webpackEntries[i];

      webpackStream(webpackConfig.config(entry.path, entry.name), webpack)
        .pipe(gulp.dest(webpackEntries[i].dist));
    }
    res();
  });
});

gulp.task('pug-compile', () =>{
  return gulp.src(['www/src/views/**/*.pug', '!' + 'www/src/views/**/_*.pug'])
    .pipe(plumber({errorHandler: notify.onError("Error: <%= error.message %>") }))
    .pipe(pug(pugOptions))
    .pipe(gulp.dest('./www/dist'));
});

gulp.task('watcher', () => {
  new Promise((res, rej) => {
    gulp.watch('./www/src/**', gulp.parallel('pug-compile', 'webpack-compile'));
    res();
  })
});

gulp.task('open-server', () => {
  return new Promise((res, rej) => {
    let app = ex();
    let server = app.listen(3000, () => {
      console.log('Server running.');
    });

    app.use(ex.static(Root, {index: false}));
    res();
  })
})

gulp.task('start', gulp.series('pug-compile', 'open-server', 'webpack-compile', 'watcher'));