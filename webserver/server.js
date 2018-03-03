const http = require('http');
const express = require('express');
const io = require('socket.io');
const PORT =   3000;
const path = require('path'); //Node.js module used for getting path of file
const MessagingResponse = require('twilio').twiml.MessagingResponse;
const app = express();


app.use(express.static(path.join(__dirname, 'public'))); //sets all static file calls to


app.post('/sms', (req, res) => {
  const twiml = new MessagingResponse();

  twiml.message('Fidget spinner is spinning... grab some soylent /hoss!');

  res.writeHead(200, {'Content-Type': 'text/xml'});
  res.end(twiml.toString());
});

http.createServer(app).listen(PORT, () => {
  console.log('Express server listening on port 3000');
});
