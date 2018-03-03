var fidgetIndex = 0;
const MAX_FIDGET_INDEX=3;

function init(){
  updateFidget();
  updateLight();
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

function updateLight(on){
  if(on){
        document.getElementById('light').src = "images/light_bulb_on.png";
  } else{
  document.getElementById('light').src = "images/light_bulb_off.png";
  }
}
