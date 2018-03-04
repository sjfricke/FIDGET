var fidgetIndex = 0;
const MAX_FIDGET_INDEX=3;

function init(){
  updateFidget();
  updateLight();
  initGarage();
  updateTemperature();
  updateCamera();
  // updateJoystick();
};


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
       document.getElementById('recentTwilo').innerHTML+=json[0];
     }
   };

 }
