const WebSocket = require('ws');

const ws = new WebSocket('ws://192.168.43.151:8000');

ws.on('open', function open() {
  ws.send('1:something');
});

ws.on('message', function incoming(data) {
  console.log(data);
});	