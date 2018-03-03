var leapjs      = require('leapjs');
var controller  = new leapjs.Controller({enableGestures: true});

controller.on('deviceFrame', function(frame) {
  var numberOfFingers = 0;

  var hand = frame.hands;
  for(var h = 0; h < frame.hands.length; h++) {
    console.log("Number of hands: " + frame.hands.length);
    var hand = frame.hands[h];
    if (hand.pinchStrength >= 0.89) {
      console.log("Pinch: " + hand.pinchStrength);
    }

    for (var f = 0; f < hand.fingers.length; f++){
      console.log(hand.fingers[f] + ' ' + hand.fingers[f].extended);
      numberOfFingers += hand.fingers[f].extended ? 1 : 0;
    }
    console.log("Fingers: " + numberOfFingers);

  }

});

controller.connect();
