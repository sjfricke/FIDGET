const http = require('http');
const express = require('express');
const io = require('socket.io');
const PORT =  3000;
const path = require('path'); //Node.js module used for getting path of file
const MessagingResponse = require('twilio').twiml.MessagingResponse;
const app = express();
const bodyParser = require('body-parser'); //allows the use of req.body in POST request
const WebSocket = require('ws');
const innerWS = new WebSocket.Server({ port: 8000 });
const clientSCP = require('scp2')

var arr = [];

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
      twiml.message('Your soylent powered fidget spinner deactivated!');
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

//////////////////////////
///// Inner Web Socket ///
//////////////////////////
innerWS.on('connection', function connection(ws) {
  ws.on('message', function incoming(message) {
    console.log('received: %s', message);
  });

});

// Broadcast to all.
innerWS.broadcast = function broadcast(key, data) {
  innerWS.clients.forEach(function each(client) {
    if (client.readyState === WebSocket.OPEN) {
      client.send(key + ":" + data);
    }
  });
};

/////////////////////////////
/// DRAGON BOARD AUDIO //////
/////////////////////////////
var cameraStart = false;

const dragonAudio = new WebSocket('ws://192.168.43.151:8000');
dragonAudio.on('open', function open() {
	dragonAudio.send('0:0');
	console.log("ATSETSET")
	cameraStart = true;
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
	//console.log(read);
  	var key = read.substring(0, read.indexOf(":"));
  	console.log("TEST::::: "+ key);
  	if (key == 8) {
  		piData(read.substr(2));
  	} else if (key == 4) {
  		myoData(read.substr(2));
  	}else if (key == 2) {
  		leapLightData(read.substr(2));
  	}else if (key == 1) {
  		leapVolData(read.substr(2));
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
	innerWS.broadcast("joyX", sec[0]);
	innerWS.broadcast("joyY", sec[1]);
	innerWS.broadcast("joyBtn", sec[2]);
	innerWS.broadcast("tempPot", sec[3]);
	innerWS.broadcast("tempReal", sec[4]);

	if (sec[1] == 1) {
		dragonAudio.send('2:0');
	} else if (sec[1] == -1) {
		dragonAudio.send('1:0');
	}
}

function myoData(data) {
	console.log(data);
  var sec = data.split(",");
  //sec[0] == Fldget DataC
  //sec[1] == clostet Data
  innerWS.broadcast("fidget", sec[0]);
	innerWS.broadcast("closet", sec[1]);
}

function leapLightData(data) {
		dragonAudio.send("0:"+data);
}
function leapVolData(data) {
	console.log("vol: " + data);
	if (data == NaN || data < 120 || data > 150 ){ return; }
	dragonAudio.send("3:"+data);
}
// Put a friendly message on the terminal of the server.
console.log("Chat server running at port 5000\n");

process.on('uncaughtException',function(err){
   console.log('something terrible happened..')
})

var cameraCount = 0;
var scpInterval = setInterval(function() {
  if (cameraStart) {
  	clientSCP.scp('linaro:linaro@192.168.43.151:/home/linaro/FIDGET/boards/dragonAudio/output.png',
  			 "./public/images/camera" + cameraCount + ".png", function(err) {
  				console.error(err);
	})
	innerWS.broadcast("camera", cameraCount);
	cameraCount++;
  }  
}, 3000);
