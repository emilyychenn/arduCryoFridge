$(document).ready(function(){

var received = $('#received');

var hostName = window.location.hostname;
var socket = new WebSocket("ws://" + hostName + ":8080/ws");
 
socket.onopen = function(){  
  console.log("connected"); 
}; 

socket.onmessage = function (message) {
  console.log("receiving: " + message.data);
  received.append(message.data);
  received.append($('<br/>'));

    // Get the container element
    var btnContainer = document.getElementById("errorbuttons");

    // Get all buttons with class="btn" inside the container
    var btns = btnContainer.getElementsByClassName("btn");

    // Loop through the buttons and change the status of buttons depending on message from arduino
    for (var i = 0; i < btns.length; i++) {
        if (message.data.includes("button"+(i+1).toString()+"State: 1")){
            btns[i].className += " active";
        };
        if (message.data.includes("button"+(i+1).toString()+"State: 0")){
            btns[i].className = btns[i].className.replace(" active","");
        };
    };
    
    
    // Get the container element
    var ledBtnContainer = document.getElementById("onoffState");
    
    // Get the button
    var ledbtn = ledBtnContainer.getElementsByClassName("btnled")[0];
    
    // Change the status of the button depending on off/on state change message from arduino
    if (message.data.includes("LED on")) {
      ledbtn.className += " active";
    };
    if (message.data.includes("LED off")) {
      ledbtn.className = ledbtn.className.replace(" active","");
    };
};

socket.onclose = function(){
  console.log("disconnected"); 
};

var sendMessage = function(message) {
  console.log("sending:" + message.data);
  socket.send(message.data);
};


// GUI Stuff


// send a command to the serial port
$("#cmd_send").click(function(ev){
  ev.preventDefault();
  var cmd = $('#cmd_value').val();
  sendMessage({ 'data' : cmd});
  $('#cmd_value').val("");
});

$('#clear').click(function(){
  received.empty();
});


});