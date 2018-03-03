var leapjs = require('leapjs');
var udp = require('dgram');
var controller = new leapjs.Controller({
   enableGestures: true
});
var buffer = require('buffer');
var prevFingerCount = 0;
var prevNormal = 0;
var prevPinch = 0.0;

// creating a client socket
var client = udp.createSocket('udp4');

//buffer msg
var data = Buffer.fom('siddheshrane');

client.on('message',function(msg,info){
  console.log('Data received from server : ' + msg.toString());
  console.log('Received %d bytes from %s:%d\n',msg.length, info.address, info.port);
});

//sending msg
client.send(data,5000,'localhost',function(error){
  if(error){
    client.close();
  }else{
    console.log('Data sent !!!');
  }
});

var data1 = Buffer.from('hello');
var data2 = Buffer.from('world');

//sending multiple msg
client.send([data1,data2],5000,'localhost',function(error){
  if(error){
    client.close();
  }else{
    console.log('Data sent !!!');
  }
});

controller.on('deviceFrame', function(frame) {

   var numberOfFingers = 0;
   var hand = frame.hands;
   var leftOrRight;

   // Check hand orientation
   // positive value sends 1 for ccw rotation
   // negative value sends -1 for cw rotation
   if (frame.hands.length == 2) {
      if (frame.hands[0].palmNormal[1] > 0 && frame.hands[1].palmNormal[1] > 0 && prevNormal != 1) {
         console.log('2:1');
         prevNormal = 1;
      } else if (frame.hands[0].palmNormal[1] < 0 && frame.hands[1].palmNormal[1] < 0 && prevNormal != -1) {
         console.log('2:-1');
         prevNormal = -1;
      }
   } else if (frame.hands.length == 1) {
      if (frame.hands[0].palmNormal[1] > 0 && prevNormal != 1) {
         console.log('2:1');
         prevNormal = 1;
      } else if (frame.hands[0].palmNormal[1] < 0 && prevNormal != -1) {
         console.log('2:-1');
         prevNormal = -1;
      }
   } else if (frame.hands.length == 0 && prevNormal != 0) {
      console.log('2:0');
      prevNormal = 0;
   }
   for (var h = 0; h < frame.hands.length; h++) {
      var hand = frame.hands[h];
      if ((hand.pinchStrength >= 0.89 && prevPinch < 0.89) ||
       hand.pinchStrength < 0.89 && prevPinch >= 0.89) {
         console.log('2:0');
      }
      prevPinch = hand.pinchStrength;
      for (var f = 0; f < hand.fingers.length; f++) {
         numberOfFingers += hand.fingers[f].extended ? 1 : 0;
      }
   }

   if (numberOfFingers != prevFingerCount) {
      if (numberOfFingers == 0) {
         console.log('1:0');
      } else {
         console.log('1:' + numberOfFingers);
      }
      prevFingerCount = numberOfFingers;
   }

});
controller.connect();
