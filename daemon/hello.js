var http = require("http");

var manejador = function(solicitud, respuesta){
    console.log("Recibido..");
    respuesta.end("Hola Mundo")
};

var servidor = http.createServer(manejador);

servidor.listen("8080");
console.log("Servidor en puerto 8080");