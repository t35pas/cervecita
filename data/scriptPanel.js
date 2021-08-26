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

function vEstado4(element) {
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/vEstado4", true);
    console.log("vEstado4");
    xhr.send();
}

function vEstado5(element) {
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/vEstado5", true);
    console.log("vEstado5");
    xhr.send();
}

function vEstado6(element) {
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/vEstado6", true);
    console.log("vEstado6");
    xhr.send();
}

function vEstado7(element) {
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/vEstado7", true);
    console.log("vEstado7");
    xhr.send();
}

function vEstado8(element) {
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/vEstado8", true);
    console.log("vEstado8");
    xhr.send();
}

function vEstado9(element) {
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/vEstado9", true);
    console.log("vEstado9");
    xhr.send();
}

function vEstado10(element) {
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/vEstado10", true);
    console.log("vEstado10");
    xhr.send();
}

function vEstado11(element) {
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/vEstado11", true);
    console.log("vEstado11");
    xhr.send();
}