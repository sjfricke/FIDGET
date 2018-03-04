// global WebSocket pointer
var webSocket;

// decides what do when message arrives
function wsOnMessage(event) {
  // console.log(event.data);
  var key = event.data.substring(0, event.data.indexOf(":"));
  var value = event.data.substr(event.data.indexOf(":") + 1);

  switch(key) {
  case "joyX":
    // do something with -> value
    break;
  case "joyY":
    break;
  case "joyBtn":
    break;
  case "tempPot":
    break;
  case "tempReal":
      break;
  case "fidget":
      console.log("fidget: " + value);
      break;
  case "closet":
      console.log("closet: " + value);
      break;
  // case "":
  //     break;
  // case "":
  //     break;
  // case "":
  //     break;
  // case "":
  //     break;
  // case "":
  //     break;
  // case "":
  //     break;
  // case "":
  //     break;
  default:
	  warn("WebSocket", "No case for data: %0", event.data);
  }
}
