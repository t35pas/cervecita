let button = document.getElementById("siguienteID");

window.addEventListener('resize', onWindowResize, false);

if (!!window.EventSource) {
  var source = new EventSource('/events');

  source.addEventListener('open', function(e) {
    console.log("Events Connected");
  }, false);

  source.addEventListener('error', function(e) {
    if (e.target.readyState != EventSource.OPEN) {
      console.log("Events Disconnected");
    }
  }, false);
}
  
  function siguiente(element){
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/siguiente", true);
  console.log("siguienteLOG");
  xhr.send(); 
}

function resetear(element) {
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/resetear", true);
    console.log("reseteo a estado 0");
    xhr.send();
}