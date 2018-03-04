const http = require('http');
const express = require('express');
const io = require('socket.io');
const PORT =  3000;
const path = require('path'); //Node.js module used for getting path of file
const MessagingResponse = require('twilio').twiml.MessagingResponse;
const app = express();
const bodyParser = require('body-parser'); //allows the use of req.body in POST request
var arr = [];

app.use(express.static(path.join(__dirname, 'public'))); //sets all static file calls to

app.use(bodyParser.json()); //parses json and sets to body
app.use(bodyParser.urlencoded({ extended: false }));
var arr =[];
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
