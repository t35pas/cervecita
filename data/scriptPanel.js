let button = document.getElementById("siguienteID");

//window.addEventListener('resize', onWindowResize, false);

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

function vComenzar(element) {
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/vComenzar", true);
    console.log("vComenzar");
    xhr.send();
}

function vEstado2(element) {
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/vEstado2", true);
    console.log("vEstado2");
    xhr.send();
}

function vEstado3(element) {
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/vEstado3", true);
    console.log("vEstado3");
    xhr.send();
}