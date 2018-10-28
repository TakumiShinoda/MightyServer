const ex = require('express');

const Root = './www/dist/';

let app = ex();
let server = app.listen(3000, () => {
  console.log('Server running.');
});

app.use(ex.static(Root, {index: false}));