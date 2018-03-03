var fidgetIndex = 0;
const MAX_FIDGET_INDEX=3;

function init(){
  updateFidget();
  updateLight();
  updateGarage();
  updateTemperature();
  updateCamera();
  updateJoystick();
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

//Pass in on ==true if joystick is toggled. Pass in direction of joystick
function updateJoystick(direction,on){
 if(direction=="left" && on){
   document.getElementById('joystick').src="images/joystick-up-red.png";
 } else{
  document.getElementById('joystick').src="images/joystick-up.png";
}};



function updateCamera(){
return false;
};




function updateTemperature(){
  document.getElementById('temperatureBackgroud');
  document.getElementById('temperatureGauge');
};

function updateGarage(up){
return false;
 };
