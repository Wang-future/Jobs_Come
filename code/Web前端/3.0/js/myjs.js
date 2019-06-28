(function submitFn(obj, evt){
  if (typeof(obj) == "undefined") {
   alert("please sdvdd input your txt");
}

  var socket;
  input = document.getElementById("myId");//通过id获取文本框对象
  value=input.value.trim();

  if(!value.length){
    alert("please input your txt");
  }
  else{
    //_html += "<b>" + value + "</b>";
    socket = new WebSocket('ws://110.64.87.175:9000');
    try {

                  socket.onopen = function (msg) {

                      socket.send(value);

                  };

                  socket.onmessage = function (msg) {
                      if (typeof msg.data == "string") {
                          addElementImg(obj, msg.data);
                      }
                      else {
                          alert("not txt message");
                      }
                  };

                  socket.onclose = function (msg) {

                   };
                socket.onerror = function(evt)

                  {
                      alert("WebSocketError!");
                  };
              }
              catch (ex) {
                  log(ex);
              }
    }
})
(function addElementImg(obj, str) {
  if (typeof str != "string") {
      alert("it is not string")
  }
　　　　//设置 img 图片地址
　　　　
　　　　var ul = document.getElementById("zturn");

　　　　//添加 li
　　　　var li = document.createElement("li");
    li.setAttribute("class","poster-item zturn-item")

          //添加 img
          var img = document.createElement("img");
          //设置 img 图片地址
          img.src =str;
          img.width="100%";
          var div=document.createElement("div");
          div.setAttribute("class","students_star");
    　　　     var p=document.createElement("p");
              p.setAttribute("class","cell_list");
                  var span=document.createElement("span");
                  span.text="新表";
                      var span2=document.createElement("span");
                      span2.setAttribute("class","darks");
                      span2.text="趋势图";
                  span.appendChild(span2);
              p.appendChild(span);
              var div2=document.createElement("div");
              div2.setAttribute("class","zwjs");
              div2.text="这个表展示近三年数据";
          div.appendChild(p);
          div.appendChild(div2);

    li.appendChild(img);
    li.appendChild(div);
ul.appendChild(li);
　　})
