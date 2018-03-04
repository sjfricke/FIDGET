var fidgetIndex = 0;
const MAX_FIDGET_INDEX=3;

function init(){
  updateFidget();
  updateLight();
  updateGarage();
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

function updateGarage(up){
  if(up){
    document.getElementById('garage').src="images/garage_open.gif";
  }
  if(!up){
    document.getElementById('garage').src="images/garage_close.gif"
  }
return false;
 };

 function getDataUpdates() {
   const xmlHttp = new XMLHttpRequest();
   xmlHttp.open('GET', 'http://localhost:3000/data');
   xmlHttp.send(null);
   xmlHttp.onreadystatechange = ()=>{
     if(xmlHttp.readyState === XMLHttpRequest.DONE && xmlHttp.status === 200) {
        var json =JSON.parse(xmlHttp.responseText);
       //console.log(json);
       document.getElementById('recentTwilo').innerHTML = '';
       document.getElementById('recentTwilo').innerHTML+=json[0];
     }
   };

 }
