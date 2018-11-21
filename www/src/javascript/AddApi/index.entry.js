import '../../css/AddApi/header.css'
import '../../css/AddApi/index.css'

const response = [
  {
    'path': 'admin/apimanager',
    'cleated': '2018/8/2/12:00',
    'update': '2018/8/13/1:00',
    'owner': 'First'
  },
  {
    'path': 'admin/apimanager',
    'cleated': '2018/8/2/12:00',
    'update': '2018/8/13/1:00',
    'owner': 'Second'
  },
  {
    'path': 'admin/apimanager',
    'cleated': '2018/8/2/12:00',
    'update': '2018/8/13/1:00',
    'owner': 'Third'
  },
];

function makeAPITable(objs){ // obj = {'path', 'cleated', 'update', 'owner'}
  let infoHeads = ['path', 'cleated', 'update', 'owner'];
  let tbody = $('#apiTable tbody');
  let trs = [];

  for(var i = 0; i < objs.length; i++){
    let tr =  $('<tr>').attr({
      class: 'apiTableRow'
    });

    for(var j = 0; j < infoHeads.length; j++){
      let td = $('<td>').attr({
        class: 'apiTableCellText'
       });

      td.append(objs[i][infoHeads[j]]);
      tr.append(td);
      if(j == infoHeads.length - 1){
        let deleteButton = $('<div>').attr({
          class: 'btn btn-danger apiTableCellBtn delete'
        }).append('delete');
        let deleteButtonCell = $('<td>').attr({
          class: 'deleteButtonCell'
        }).append(deleteButton);

        tr.append(deleteButtonCell);
      }
    }
    trs.push(tr);
  }
  for(var i = 0; i < trs.length; i++) tbody.append(trs[i])
}

$(document).ready(() => {
  console.log('hoge');

  makeAPITable(response);

  $('#addBtn').on('click', () => {
    window.location.href = "add.html";
  });
});