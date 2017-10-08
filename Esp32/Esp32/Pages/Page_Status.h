const char PAGE_status[] PROGMEM = R"=====(
<div id="content">
        <article>
          <h1>Trenutno stanje</h1>
          <div style="text-align: center;max-width: 1250px;margin: 0 auto;">
            <div class="textValue"><div class="text underline">Pump Status</div><div class="text" id="pumpstatus">-</div></div>
            <div class="textValue"><div class="text underline">Pump Auto Mode</div><div class="text" id="pumpautomode">-</div></div>
            <div class="textValue"><div class="text underline">Pump Operating Time</div><span class="text" id="operatinghours">-</span><span class="text"> h </span><span class="text" id="operatingminutes">-</span><span class="text"> m</span></div>
          </div>
          <div style="text-align: center;max-width: 1250px;margin: 0 auto;">
            <div id="boilerTempContainer" class="progressbar"><div class="text progressbarText">Boiler Temp</div></div>
            <div id="collectorTempContainer" class="progressbar"><div class="text progressbarText">Collector Temp</div></div>
            <div id="t1TempContainer" class="progressbar"><div class="text progressbarText">T1 Temp</div></div>
            <div id="t2TempContainer" class="progressbar"><div class="text progressbarText">T2 Temp</div></div>
            <div id="roomTempContainer" class="progressbar"><div class="text progressbarText">Room Temp</div></div>
            <div id="roomHumidityContainer" class="progressbar"><div class="text progressbarText">Room Humidity</div></div>
            <div id="airPressureContainer" class="progressbar"><div class="text progressbarText">Air Pressure</div></div>
          </div>
          <button onclick="GetStatusData()" class="shadow button">Osveži</button>
        </article>
      </div>
      <script>
        var progressbarSettings = {
          color: '#ff551d',
          strokeWidth: 4,
          trailWidth: 1,
          easing: 'easeInOut',
          duration: 1400,
          text: {
            autoStyleContainer: false
          },
          from: { color: '#27ae60', width: 3 },
          to: { color: '#ff3f00', width: 5 },
          attachment: { units: "°C" },
          step: function(state, circle, attachment) {
            circle.path.setAttribute('stroke', state.color);
            circle.path.setAttribute('stroke-width', state.width);

          var value = Math.round(circle.value() * 1000)/10;
            if (value === 0) {
              circle.setText('');
            } else {
            if (attachment.min != undefined){
              var diff = attachment.max - attachment.min;
              console.log(diff);
              
              value = Math.round(value*diff+attachment.min*100)/100;
              value = Math.round(value);
              circle.setText(value + " " + attachment.units);
            }
            else circle.setText(value + " " + attachment.units);          
            }

          }
        }
        
        var progressBars = new Array(new ProgressBar.Circle(boilerTempContainer, progressbarSettings),
        new ProgressBar.Circle(collectorTempContainer, progressbarSettings),
        new ProgressBar.Circle(t1TempContainer, progressbarSettings),
        new ProgressBar.Circle(t2TempContainer, progressbarSettings),
        new ProgressBar.Circle(roomTempContainer, progressbarSettings),
        new ProgressBar.Circle(roomHumidityContainer, progressbarSettings),
        new ProgressBar.Circle(airPressureContainer, progressbarSettings));

        for(var i = 0; i < progressBars.length; i++){
          progressBars[i].text.style.fontFamily = '"Raleway", Helvetica, sans-serif';
          progressBars[i].text.style.fontSize = '1.5rem';
        }
      </script>
      <script>
        function GetStatusData()
        {
          setValues("/status/data");
        }

        window.onload = function ()
        {
          load("microajax.js","js", function() 
          {
              GetStatusData();
          });
        }
        function load(e,t,n){if("js"==t){var a=document.createElement("script");a.src=e,a.type="text/javascript",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}else if("css"==t){var a=document.createElement("link");a.href=e,a.rel="stylesheet",a.type="text/css",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}}
      </script>
)=====";

void send_system_status_data()
{
  unsigned long lastRequest = 0;
  SerialHandler::requestData();
  while(!Serial.available()){
    if(millis() - lastRequest >= noDataRecivedInterval){
      SerialHandler::requestData();
      lastRequest = millis();
    }
  }
  SerialHandler::handle();
	String values = "";
	values += "pumpstatus|";
	if(ardData.pump1operating == 1)values += "ON";
	else values += "OFF";
	values += "|span\n";
	values += "pumpautomode|";
	if(ardData.pump1Status == "A") values += "ON";
	else values += "OFF";
	values += "|span\n";
  
	values += "operatinghours|" + (String)ardData.operatinghours + "|span\n";
	values += "operatingminutes|" + (String)ardData.operatingminutes + "|span\n";
	values += "tempcollector|" + (String)ardData.tempcollector + "|span\n";
	values += "tempboiler|" + (String)ardData.tempboiler + "|span\n";
	values += "tempt1|" + (String)ardData.tempt1 + "|span\n";
	values += "tempt2|" + (String)ardData.tempt2 + "|span\n";
	values += "temproom|" + (String)ardData.temproom + "|span\n";
	values += "humidityroom|" + (String)ardData.humidityroom + "|span\n";
	values += "pressureroom|" + (String)ardData.pressureroom + "|span\n";
	server.send(200, "text/plain", values);
}

void handleStatus() {
  String content = String(PAGE_head); 
  if (is_authentified())content += String(PAGE_menu_logedin);
  else content += String(PAGE_menu_normal);
  content += String(PAGE_status) + String(PAGE_foot);
  server.send(200, "text/html", content);
}