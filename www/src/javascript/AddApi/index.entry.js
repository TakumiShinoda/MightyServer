import '../../css/AddApi/header.css'
import '../../css/AddApi/index.css'

$(document).ready(() => {
  console.log('hoge');

  $('#addBtn').on('click', () => {
    window.location.href = "add.html";
  });
});