const http = require('http');
const express = require('express');
const io = require('socket.io');
const PORT =   3000;
const path = require('path'); //Node.js module used for getting path of file
const MessagingResponse = require('twilio').twiml.MessagingResponse;
const app = express();
const bodyParser = require('body-parser'); //allows the use of req.body in POST request
const WebSocket = require('ws');

var arr=[];

app.use(express.static(path.join(__dirname, 'public'))); //sets all static file calls to

app.use(bodyParser.json()); //parses json and sets to body
app.use(bodyParser.urlencoded({ extended: false }));
app.post('/sms', (req, res) => {

  //Write code to send to fidget spinner.
  var recvMesg = req.body.Body;
  arr.push(recvMesg);
  console.log (recvMesg);
  //check if they said fidget or spinner
  //check if they said go or stop
  //send something back if  they didn't send the right message
  const twiml = new MessagingResponse();
  if(recvMesg.toLowerCase().indexOf('fidget')>=0||recvMesg.toLowerCase().indexOf('spinner')>=0){
    if(recvMesg.toLowerCase().indexOf('go')>=0||recvMesg.toLowerCase().indexOf('on')>=0){
      twiml.message('Your soylent powered fidget spinner is activated!');
      res.writeHead(200, {'Content-Type': 'text/xml'});
       return res.end(twiml.toString());
    } else if(recvMesg.toLowerCase().indexOf('off')>=0||recvMesg.toLowerCase().indexOf('stop')>=0){
      //stop
      twiml.message('Out of soylent... sorry! Fidget spinner deactived.');
      res.writeHead(200, {'Content-Type': 'text/xml'});
       return res.end(twiml.toString());
    }else{
      twiml.message("Sorry didn't catch what you said... text go fidget or stop fidget to (206)-889-7536 to try again!");
      res.writeHead(200, {'Content-Type': 'text/xml'});
       return res.end(twiml.toString());
    }
  } else{
    twiml.message("Sorry didn't catch what you said... text go fidget or stop fidget to (206)-889-7536 to try again!");
    res.writeHead(200, {'Content-Type': 'text/xml'});
     return res.end(twiml.toString());
  }
});

app.get('/data', (req, res) => {
  res.json(JSON.parse(JSON.stringify(arr)));
  res.end();
});

http.createServer(app).listen(PORT, () => {
  console.log('Express server listening on port 3000');
});


/////////////////////////////
/// DRAGON BOARD AUDIO //////
/////////////////////////////
const dragonAudio = new WebSocket('ws://192.168.43.151:8000');

dragonAudio.on('open', function open() { 
	dragonAudio.send('0:0');
});

dragonAudio.on('message', function incoming(data) {
  console.log(data);
});



/////////////////////////
////// TCP SERVER ///////
/////////////////////////

// Load the TCP Library
const net = require('net');

// Keep track of the chat clients
var clients = [];
var completeData;
// Start a TCP Server
net.createServer(function (tSocket) {


  // Identify this client
  tSocket.name = tSocket.remoteAddress + ":" + tSocket.remotePort 
  console.log(tSocket.name + " has joined!");

  // Put this new client in the list
  clients.push(tSocket);

  // Send a nice welcome message and announce
 // tSocket.write("Welcome " + tSocket.name + "\n");

  // Handle incoming messages from clients.
  tSocket.on('data', function (data) {
  	var read = data.toString();
	console.log(read);
  	var key = read.substring(0, read.indexOf(":"));
  	if (key == 8) {
  		piData(read.substr(2));
  	} else if (key == 4) {
  		myoData(read.substr(2));
  	} else if (key == 2) {
  		leapData(read.substr(2));
  	}
  });

  // Remove the client from the list when it leaves
  tSocket.on('end', function () {
    clients.splice(clients.indexOf(tSocket), 1);
    console.log(tSocket.name + " left the chat.\n");
  });
  
  // Send a message to all clients

}).listen(5000);

function piData(data) {
	var sec = data.split(",");
	//Joy_X, Joy_y, Joy_Btn, Pot, Temp
}

function myoData(data) {
	console.log(data);
}

function leapData(data) {
	console.log(data);
}


// Put a friendly message on the terminal of the server.
console.log("Chat server running at port 5000\n");

process.on('uncaughtException',function(err){
   console.log('something terrible happened..')
})