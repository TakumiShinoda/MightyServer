import '../../css/AddApi/header.css'
import '../../css/AddApi/index.css'

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
  for(var i = 0; i < trs.length; i++) tbody.append(trs[i]);
}

function setDomEvents(){
  return new Promise((res) => {
    $('#addBtn').on('click', () => {
      window.location.href = "add.html";
    });
  
    $('.delete').on('click', () => {
      let confirm = window.confirm("Are you sure?");    
  
      if(confirm){
        fetch('http://'+ location.hostname +':3000/AddApi/remove_api')
          .then((raw) => raw.text())
          .then((text) => {
            if(text == "success"){
              location.reload();
            }else{
              alert('Failed');
              console.log(text);
            }
          });
      }
    });

    res();
  });
}

$(document).ready(() => {
  fetch('http://'+ location.hostname +':3000/AddApi/test_response')
    .then((raw) => raw.json())
    .then((response) => makeAPITable(response))
    .then(() => setDomEvents())
    .catch((err) => console.log(err));
});