var fidgetIndex = 0;
const MAX_FIDGET_INDEX=3;
const CL_LEFT_EDGE = 420;
const CL_RIGHT_EDGE = 780;
const CL_CENTER = 600;

function init(){
  setWebSocket();
  updateFidget();
  updateLight();
  initGarage();
  closetInit();
  updateTemperature();
  updateCamera();
  // updateJoystick();
};

function setWebSocket() {
    // Attempts to just reload webpage if it was not able to get websocket
    // Will cause loop if not connect, but app is useless anyways without WS
    try {
     webSocket = new WebSocket('ws://127.0.0.1:8000');
     webSocket.onmessage = wsOnMessage;
    } catch (e) {
     location.reload();
    }
}


window.requestAnimFrame = (function(){
  return  window.requestAnimationFrame       ||
          window.webkitRequestAnimationFrame ||
          window.mozRequestAnimationFrame    ||
          function( callback ){
            window.setTimeout(callback, 1000 / 60);
          };
})();

var canvas = document.getElementById('canvas'),
	context = canvas.getContext('2d'),
	width = canvas.width = document.body.offsetWidth,
	height = canvas.height = document.body.offsetHeight,
	ball = {
		x: 0,
		y: height / 2,
	},
	point = {
		x: 0,
		y: ball.y
	},
	current_point = 0;

var points = [
		{y:0,x:20},
		{y:0,x:1},
        {y:3,x:1},
	    {y:-10,x:2},
		{y:10,x:2},
	    {y:-12,x:3},
	    {y:35,x:5},
	    {y:-25,x:4},
	    {y:14,x:3},
	    {y:5,x:2},
		{y:0,x:1},
		{y:0,x:20}
	];

context.fillStyle = "rgba(255, 0, 0, 1)";
render();
function animateTo() {
	function dist(x1,x2,y1,y2) {
		var dx = x1 - x2,
			dy = y1 - y2;
		return {
			d: Math.sqrt(dx*dx + dy*dy),
			dx: dx,
			dy: dy
		};
	}
	var dis = dist(ball.x, point.x+points[current_point].x,ball.y, point.y+points[current_point].y);
	if( dis.d > 1 ) {
		var s = Math.abs(dis.dy) > 13 ? 2 : 1;
		ball.x += -( dis.dx / dis.d )*s;
		ball.y += -( dis.dy / dis.d )*s;
	} else {
		ball.x = point.x+points[current_point].x;
		ball.y = point.y+points[current_point].y;
		point.x += points[current_point].x;
		current_point++;
		if( current_point >= points.length || ball.x > width ) {
			current_point = 0;
			if( ball.x > width ) {
				point.x = ball.x = 0;
			}
		}
	}
}
function render() {
	requestAnimFrame(render);
	animateTo();
	context.fillStyle = "rgba(0, 0, 0, .01)";
	context.fillRect(0,0,width,height);
	context.fillStyle = "rgba(255, 0, 0, 1)";
	context.beginPath();
	context.arc(ball.x, ball.y, 1, 0, 2*Math.PI, true);
	context.closePath();
	context.fill();
}

// Pass in true if moving fidget index up
function updateFidget(up){
  if(up){
    fidgetIndex++;
    if(fidgetIndex> MAX_FIDGET_INDEX){
      fidgetIndex=0;
    }
  } else{
      fidgetIndex--;
      if(fidgetIndex<0){
        fidgetIndex = MAX_FIDGET_INDEX;
      }
  }
    document.getElementById('fidget').src = "images/fidget_"+fidgetIndex+".png";
    document.getElementById('fidgetBackground').src = "images/fidget_background_"+fidgetIndex+".png";
}

// Pass in true if turning light on
function updateLight(on){
  if(on){
        document.getElementById('lightbulb').src = "images/light_bulb_on.png";
  } else{
  document.getElementById('lightbulb').src = "images/light_bulb_off.png";
  }
}



function updateCamera(){
return false;
};

function updateTemperature(){
  document.getElementById('temperatureBackgroud');
  document.getElementById('temperatureGauge');
};

function initGarage() {

  var garageUp = document.getElementById('garageUp');
  var garageDown = document.getElementById('garageDown');

  garageUp.src=garageUp.src;
      garageDown.src=garageDown.src;

    garageUp.style.visibility="hidden";
    garageDown.style.visibility="visible";
}

function updateGarage(up){
var garageUp = document.getElementById('garageUp');
var garageDown = document.getElementById('garageDown');
  if(up){
garageDown.style.visibility="hidden";
garageUp.style.visibility="visible";

garageUp.src=garageUp.src;
  }else{
    garageUp.style.visibility="hidden";
    garageDown.style.visibility="visible";

    garageDown.src=garageDown.src;
  }
 };

 function getDataUpdates() {
   const xmlHttp = new XMLHttpRequest();
   xmlHttp.open('GET', 'http://localhost:3000/data');
   xmlHttp.send(null);
   xmlHttp.onreadystatechange = ()=>{
     if(xmlHttp.readyState === XMLHttpRequest.DONE && xmlHttp.status === 200) {
        var json =JSON.parse(xmlHttp.responseText);
       document.getElementById('recentTwilo').innerHTML = '';
       document.getElementById('recentTwilo').innerHTML+=json[this.length-1];
     }
   };
 }

var closetIndex = 4;
var d_shirt = [];
function closetInit() {
  d_shirt[0] = document.getElementById("shirt0");
  d_shirt[1] = document.getElementById("shirt1");
  d_shirt[2] = document.getElementById("shirt2");
  d_shirt[3] = document.getElementById("shirt3");
  d_shirt[4] = document.getElementById("shirt4");
}

function closetLeft() {
  if (closetIndex >= 4) { return; }

  d_shirt[closetIndex].src = "images/shirt_" + closetIndex + "_side.png";
  d_shirt[closetIndex].style.left = (CL_LEFT_EDGE + (closetIndex*15)) + "px";
  d_shirt[closetIndex].style.zIndex = closetIndex + 3;


  closetIndex++;

  d_shirt[closetIndex].src = "images/shirt_" + closetIndex + ".png";
  d_shirt[closetIndex].style.left = CL_CENTER + "px";
  d_shirt[closetIndex].style.zIndex = 100;
}

function closetRight() {
  if (closetIndex <= 0) { return; }

  d_shirt[closetIndex].src = "images/shirt_" + closetIndex + "_side.png";
  d_shirt[closetIndex].style.left = (CL_RIGHT_EDGE - ((3 - closetIndex)*15)) + "px";
  d_shirt[closetIndex].style.zIndex = closetIndex + 3;

  closetIndex--;

  d_shirt[closetIndex].src = "images/shirt_" + closetIndex + ".png";
  d_shirt[closetIndex].style.left = CL_CENTER + "px";
  d_shirt[closetIndex].style.zIndex = 100;

}

function closetSelect() {

}
