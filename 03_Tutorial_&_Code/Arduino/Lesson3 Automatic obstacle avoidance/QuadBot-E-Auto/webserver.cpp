#include <EEPROM.h>
#include "webserver.h"

char* ssid = "QuadBot-E";
char* password = "12345678";
String FW_Version = "XinLi - Quadbot v1.0";

int Servo_PROGRAM;
int GPIO_ID = 100;
String ival;

ESP8266WebServer server(80);
XinBot* bot;

void webinit()
{
    // AP SSID Name
    uint8_t mac[WL_MAC_ADDR_LENGTH];
    WiFi.softAPmacAddress(mac);
    String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) + String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
    macID.toUpperCase();

    String AP_NameString = "Robot - " + macID;

    char AP_NameChar[AP_NameString.length() + 1];
    memset(AP_NameChar, 0, AP_NameString.length() + 1);

    for (int i = 0; i < AP_NameString.length(); i++)
        AP_NameChar[i] = AP_NameString.charAt(i);
  
    WiFi.softAP(AP_NameChar, password);
	IPAddress myIP = WiFi.softAPIP();

// WiFi.softAP(ssid, password);
// IPAddress myIP = WiFi.softAPIP();

  // EEPROM
  EEPROM.begin(512);
  delay(10);
}

void writeKeyValue(int8_t key, int8_t value)
{
   EEPROM.write(key, value);
   EEPROM.commit();
}

int8_t readKeyValue(int8_t key)
{
   Serial.println("read");
   Serial.println(key);

   int8_t value = EEPROM.read(key);
}

void handleClient()
{
    server.handleClient();
}

void handleSave()
{
	String key = server.arg("key");
	String value = server.arg("value");

	int8_t keyInt = key.toInt();
	int8_t valueInt = value.toInt();

//	// Software PWM PIN detach
//	bot->GPIO14SERVO.detach();
//	bot->GPIO12SERVO.detach();
//	bot->GPIO13SERVO.detach();
//	bot->GPIO15SERVO.detach();
//	bot->GPIO16SERVO.detach();
//	bot->GPIO5SERVO.detach();
//	bot->GPIO4SERVO.detach();
//	bot->GPIO2SERVO.detach();
	delay(50);

	if (keyInt == 100) 
	{
		writeKeyValue(0, 0);
		writeKeyValue(1, 0);
		writeKeyValue(2, 0);
		writeKeyValue(3, 0);
		writeKeyValue(4, 0);
		writeKeyValue(5, 0);
		writeKeyValue(6, 0);
		writeKeyValue(7, 0);
	} 
	else 
	{
		if (valueInt >= -124 && valueInt <= 124) // 确认数据介于 -124 ~ 124 
		{
			writeKeyValue(keyInt, valueInt); // 存储校正值
		}
	}

//	// Software PWM PIN attach
//	bot->GPIO14SERVO.attach(16, SERVOMIN, SERVOMAX);
//	bot->GPIO12SERVO.attach(5, SERVOMIN, SERVOMAX);
//	bot->GPIO13SERVO.attach(4, SERVOMIN, SERVOMAX);
//	bot->GPIO15SERVO.attach(2, SERVOMIN, SERVOMAX);
//	bot->GPIO16SERVO.attach(14, SERVOMIN, SERVOMAX);
//	bot->GPIO5SERVO.attach(12, SERVOMIN, SERVOMAX);
//	bot->GPIO4SERVO.attach(13, SERVOMIN, SERVOMAX);
//	bot->GPIO2SERVO.attach(15, SERVOMIN, SERVOMAX);
	delay(10);

	server.send(200, "text/html", "(key, value)=(" + key + "," + value + ")");
}

void handleController()
{
	String pm = server.arg("pm");
	String servo = server.arg("servo");

	if (pm != "") 
	{
		Servo_PROGRAM = pm.toInt();
	}

	if (servo != "") 
	{
		GPIO_ID = servo.toInt();
		ival = server.arg("value");
//		bot->Set_PWM_to_Servo(GPIO_ID, ival.toInt());
	}

	server.send(200, "text/html", "(pm)=(" + pm + ")");
}

void handleOnLine()
{
	String m0 = server.arg("m0");
	String m1 = server.arg("m1");
	String m2 = server.arg("m2");
	String m3 = server.arg("m3");
	String m4 = server.arg("m4");
	String m5 = server.arg("m5");
	String m6 = server.arg("m6");
	String m7 = server.arg("m7");
	String t1 = server.arg("t1");

	int Servo_Prg_tmp [][ALLMATRIX] = {
	// GPIO14,     GPIO12,     GPIO13,     GPIO15,     GPIO16,     GPIO5,      GPIO4,      GPIO2,      Run Time
	{ m0.toInt(), m1.toInt(), m2.toInt(), m3.toInt(), m4.toInt(), m5.toInt(), m6.toInt(), m7.toInt(), t1.toInt() }
	};

//	bot->Servo_PROGRAM_Run(Servo_Prg_tmp, 1);

	server.send(200, "text/html", "(m0, m1)=(" + m0 + "," + m1 + ")");
}

void enableWebServer()
{
	HTTPMethod getMethod = HTTP_GET;

	server.on("/controller", getMethod, handleController);
	server.on("/save", getMethod, handleSave);
	server.on("/", getMethod, handleIndex);
	server.on("/editor", getMethod, handleEditor);
	server.on("/zero", getMethod, handleZero);
	server.on("/setting", getMethod, handleSetting);
	server.on("/online", getMethod, handleOnLine);

	server.begin();
}

// Zero check
void handleZero()
{
	String content = "";

	content += "<html>";
	content += "<head>";
	content += "<title>Zero check</title>";
	content += "<meta charset=UTF-8>";
	content += "<meta name=viewport content=width=device-width>";
	content += "<style type=text/css>";
	content += "body {";
	content += "margin: 0px;";
	content += "backgound-color: #FFFFFF;";
	content += "font-family: helvetica, arial;";
	content += "font-size: 100%;";
	content += "color: #555555;";
	content += "}";
	content += "td {";
	content += "text-align: center;";
	content += "}";
	content += "span {";
	content += "font-family: helvetica, arial;";
	content += "font-size: 80%;";
	content += "color: #777777;";
	content += "}";
	content += "button {";
	content += "width: 40%;";
	content += "font-family: helvetica, arial;";
	content += "font-size: 90%;";
	content += "color: #555555;";
	content += "background: #BFDFFF;";
	content += "padding: 5px 5px 5px 5px;";
	content += "border: none;";
	content += "}";
	content += "</style>";
	content += "</head>";
	content += "<body>";
	content += "<br>";
	content += "<table width=100% height=90%>";
	content += "<tr>";
	content += "<td width=50%><button type=button style=background:#FFE599 onclick=controlServo(4,45)>D16</button></td>";
	content += "<td width=50%><button type=button style=background:#FFE599 onclick=controlServo(0,135)>D14</button></td>";
	content += "</tr>";
	content += "<tr>";
	content += "<td><button type=button onclick=controlServo(5,135)>D05</button></td>";
	content += "<td><button type=button onclick=controlServo(1,45)>D12</button></td>";
	content += "</tr>";
	content += "<tr>";
	content += "<td><button type=button onclick=controlServo(6,45)>D04</button></td>";
	content += "<td><button type=button onclick=controlServo(2,135)>D13</button></td>";
	content += "</tr>";
	content += "<tr>";
	content += "<td><button type=button style=background:#FFE599 onclick=controlServo(7,135)>D02</button></td>";
	content += "<td><button type=button style=background:#FFE599 onclick=controlServo(3,45)>D15</button></td>";
	content += "</tr>";
	content += "<tr>";
	content += "<td colspan=2><button type=button style=background:#FFBFBF onclick=controlPm(100)>ZERO Postition</button></td>";
	content += "</tr>";
	content += "</table>";
	content += "</body>";
	content += "<script>";
	content += "function controlServo(id, value) {";
	content += "var xhttp = new XMLHttpRequest();";
	content += "xhttp.onreadystatechange = function() {";
	content += "if (xhttp.readyState == 4 && xhttp.status == 200) {";
	content += "document.getElementById(\"demo\").innerHTML = xhttp.responseText;";
	content += "}";
	content += "};";
	content += "xhttp.open(\"GET\", \"controller?servo=\"+id+\"&value=\"+value, true);";
	content += "xhttp.send();";
	content += "}";
	content += "function controlPm(value) {";
	content += "var xhttp = new XMLHttpRequest();";
	content += "xhttp.onreadystatechange = function() {";
	content += "if (xhttp.readyState == 4 && xhttp.status == 200) {";
	content += "document.getElementById(\"demo\").innerHTML = xhttp.responseText;";
	content += "}";
	content += "};";
	content += "xhttp.open(\"GET\", \"controller?pm=\"+value, true);";
	content += "xhttp.send();";
	content += "}";
	content += "</script>";
	content += "</html>";

	server.send(200, "text/html", content);
}

// Servo calibration
void handleSetting()
{
	int servo7Val = readKeyValue(7);
	String servo7ValStr = String(servo7Val);
	int servo6Val = readKeyValue(6);
	String servo6ValStr = String(servo6Val);
	int servo5Val = readKeyValue(5);
	String servo5ValStr = String(servo5Val);
	int servo4Val = readKeyValue(4);
	String servo4ValStr = String(servo4Val);
	int servo3Val = readKeyValue(3);
	String servo3ValStr = String(servo3Val);
	int servo2Val = readKeyValue(2);
	String servo2ValStr = String(servo2Val);
	int servo1Val = readKeyValue(1);
	String servo1ValStr = String(servo1Val);
	int servo0Val = readKeyValue(0);
	String servo0ValStr = String(servo0Val);
	String content = "";

	content += "<html>";
	content += "<head>";
	content += "<title>Servo calibration</title>";
	content += "<meta charset=UTF-8>";
	content += "<meta name=viewport content=width=device-width>";
	content += "<style type=text/css>";
	content += "body {";
	content += "margin: 0px;";
	content += "backgound-color: #FFFFFF;";
	content += "font-family: helvetica, arial;";
	content += "font-size: 100%;";
	content += "color: #555555;";
	content += "}";
	content += "td {";
	content += "text-align: center;";
	content += "}";
	content += "span {";
	content += "font-family: helvetica, arial;";
	content += "font-size: 70%;";
	content += "color: #777777;";
	content += "}";
	content += "input[type=text] {";
	content += "width: 40%;";
	content += "font-family: helvetica, arial;";
	content += "font-size: 90%;";
	content += "color: #555555;";
	content += "text-align: center;";
	content += "padding: 3px 3px 3px 3px;";
	content += "}";
	content += "button {";
	content += "width: 40%;";
	content += "font-family: helvetica, arial;";
	content += "font-size: 90%;";
	content += "color: #555555;";
	content += "background: #BFDFFF;";
	content += "padding: 5px 5px 5px 5px;";
	content += "border: none;";
	content += "}";
	content += "</style>";
	content += "</head>";
	content += "<body>";
	content += "<br>";
	content += "<table width=100% height=90%>";
	content += "<tr>";
	content += "<td width=50%>D16<br/><input type=text id=servo_4 value=\"" + servo4ValStr + "\"><button type=button style=background:#FFE599 onclick=saveServo(4,'servo_4')>SET</button></td>";
	content += "<td width=50%>D14<br/><input type=text id=servo_0 value=\"" + servo0ValStr + "\"><button type=button style=background:#FFE599 onclick=saveServo(0,'servo_0')>SET</button></td>";
	content += "</tr>";
	content += "<tr>";
	content += "<td>D05<br/><input type=text id=servo_5 value=\"" + servo5ValStr + "\"><button type=button onclick=saveServo(5,'servo_5')>SET</button></td>";
	content += "<td>D12<br/><input type=text id=servo_1 value=\"" + servo1ValStr + "\"><button type=button onclick=saveServo(1,'servo_1')>SET</button></td>";
	content += "</tr>";
	content += "<tr>";
	content += "<td>D04<br/><input type=text id=servo_6 value=\"" + servo6ValStr + "\"><button type=button onclick=saveServo(6,'servo_6')>SET</button></td>";
	content += "<td>D13<br/><input type=text id=servo_2 value=\"" + servo2ValStr + "\"><button type=button onclick=saveServo(2,'servo_2')>SET</button></td>";
	content += "</tr>";
	content += "<tr>";
	content += "<td>D02<br/><input type=text id=servo_7 value=\"" + servo7ValStr + "\"><button type=button style=background:#FFE599 onclick=saveServo(7,'servo_7')>SET</button></td>";
	content += "<td>D15<br/><input type=text id=servo_3 value=\"" + servo3ValStr + "\"><button type=button style=background:#FFE599 onclick=saveServo(3,'servo_3')>SET</button></td>";
	content += "</tr>";
	content += "<!--<tr>";
	content += "<td colspan=2><button type=button style=background:#FFBFBF onclick=saveServo(100,0)>RESET ALL</button></td>";
	content += "</tr>-->";
	content += "</table>";
	content += "</body>";
	content += "<script>";
	content += "function saveServo(id, textId) {";
	content += "var xhttp = new XMLHttpRequest();";
	content += "var value = \"0\";";
	content += "if(id==100){";
	content += "document.getElementById(\"servo_7\").value = \"0\";";
	content += "document.getElementById(\"servo_6\").value = \"0\";";
	content += "document.getElementById(\"servo_5\").value = \"0\";";
	content += "document.getElementById(\"servo_4\").value = \"0\";";
	content += "document.getElementById(\"servo_3\").value = \"0\";";
	content += "document.getElementById(\"servo_2\").value = \"0\";";
	content += "document.getElementById(\"servo_1\").value = \"0\";";
	content += "document.getElementById(\"servo_0\").value = \"0\";";
	content += "}else{";
	content += "value = document.getElementById(textId).value;";
	content += "}";
	content += "xhttp.onreadystatechange = function() {";
	content += "if (xhttp.readyState == 4 && xhttp.status == 200) {";
	content += "document.getElementById(\"demo\").innerHTML = xhttp.responseText;";
	content += "}";
	content += "};";
	content += "xhttp.open(\"GET\",\"save?key=\"+id+\"&value=\"+value, true);";
	content += "xhttp.send();";
	content += "}";
	content += "</script>";
	content += "</html>";

	server.send(200, "text/html", content);
}

// Motion editor
void handleEditor()
{
	String content = "";

	content += "<html>";
	content += "<head>";
	content += "<title>Motion editor</title>";
	content += "<meta charset=UTF-8>";
	content += "<meta name=viewport content=width=device-width>";
	content += "<style type=text/css>";
	content += "body {";
	content += "margin: 0px;";
	content += "backgound-color: #FFFFFF;";
	content += "font-family: helvetica, arial;";
	content += "font-size: 100%;";
	content += "color: #555555;";
	content += "}";
	content += "td {";
	content += "text-align: center;";
	content += "}";
	content += "span {";
	content += "font-family: helvetica, arial;";
	content += "font-size: 70%;";
	content += "color: #777777;";
	content += "}";
	content += "input[type=range] {";
	content += "-webkit-appearance: none;";
	content += "background-color: #CCCCCC;";
	content += "width: 70%;";
	content += "height: 20px;";
	content += "}";
	content += "input[type=range]::-webkit-slider-thumb {";
	content += "-webkit-appearance: none;";
	content += "background-color: #4DA6FF;";
	content += "opacity: 0.9;";
	content += "width: 12px;";
	content += "height: 20px;";
	content += "}";
	content += "input[type=text] {";
	content += "width: 40%;";
	content += "font-family: helvetica, arial;";
	content += "font-size: 90%;";
	content += "color: #555555;";
	content += "text-align: center;";
	content += "padding: 3px 3px 3px 3px;";
	content += "}";
	content += "button {";
	content += "width: 40%;";
	content += "font-family: helvetica, arial;";
	content += "font-size: 90%;";
	content += "color: #555555;";
	content += "padding: 5px 5px 5px 5px;";
	content += "border: none;";
	content += "}";
	content += "</style>";
	content += "</head>";
	content += "<body onload='actionCode()'>";
	content += "<br>";
	content += "<table width=100% height=90%>";

	content += "<tr>";
	content += "<td width=50%>D16 <span>Default 110<br>⬇ 135 <input type=range id=range_4 min=45 max=135 value=110 style=direction:rtl onchange=controlServo(4,'range_4')> 45 ⬆</span>";
	content += "<br><input type=text id=servo_4 value=110> <button type=button style=background:#FFE599 onclick=controlServo(4,'servo_4')>Send</button></td>";
	content += "<td width=50%>D14 <span>Default 70<br>⬆ 135 <input type=range id=range_0 min=45 max=135 value=70 style=direction:rtl onchange=controlServo(0,'range_0')> 45 ⬇</span>";
	content += "<br><input type=text id=servo_0 value=70> <button type=button style=background:#FFE599 onclick=controlServo(0,'servo_0')>Send</button></td>";
	content += "</tr>";

	content += "<tr><td colspan=4><span><br></span></td></tr>";

	content += "<tr>";
	content += "<td>D05 <span>Default 90<br>◀ 135 <input type=range id=range_5 min=45 max=135 value=90 style=direction:rtl onchange=controlServo(5,'range_5')> 45 ▶</span>";
	content += "<br><input type=text id=servo_5 value=90> <button type=button style=background:#BFDFFF onclick=controlServo(5,'servo_5')>Send</button></td>";
	content += "<td>D12 <span>Default 90<br>◀ 135 <input type=range id=range_1 min=45 max=135 value=90 style=direction:rtl onchange=controlServo(1,'range_1')> 45 ▶</span>";
	content += "<br><input type=text id=servo_1 value=90> <button type=button style=background:#BFDFFF onclick=controlServo(1,'servo_1')>Send</button></td>";
	content += "</tr>";

	content += "<tr><td colspan=4><span><br></span></td></tr>";

	content += "<tr>";
	content += "<td>D04 <span>Default 90<br>◀ 45 <input type=range id=range_6 min=45 max=135 value=90 onchange=controlServo(6,'range_6')> 135 ▶</span>";
	content += "<br><input type=text id=servo_6 value=90> <button type=button style=background:#BFDFFF onclick=controlServo(6,'servo_6')>Send</button></td>";
	content += "<td>D13 <span>Default 90<br>◀ 45 <input type=range id=range_2 min=45 max=135 value=90 onchange=controlServo(2,'range_2')> 135 ▶</span>";
	content += "<br><input type=text id=servo_2 value=90> <button type=button style=background:#BFDFFF onclick=controlServo(2,'servo_2')>Send</button></td>";
	content += "</tr>";

	content += "<tr><td colspan=4><span><br></span></td></tr>";

	content += "<tr>";
	content += "<td>D02 <span>Default 70<br>⬇ 45 <input type=range id=range_7 min=45 max=135 value=70 onchange=controlServo(7,'range_7')> 135 ⬆</span>";
	content += "<br><input type=text id=servo_7 value=70> <button type=button style=background:#FFE599 onclick=controlServo(7,'servo_7')>Send</button></td>";
	content += "<td>D15 <span>Default 110<br>⬆ 45 <input type=range id=range_3 min=45 max=135 value=110 onchange=controlServo(3,'range_3')> 135 ⬇</span>";
	content += "<br><input type=text id=servo_3 value=110> <button type=button style=background:#FFE599 onclick=controlServo(3,'servo_3')>Send</button></td>";
	content += "</tr>";

	content += "<tr>";
	content += "<td colspan=4><br><span>Action Code:<br><output id=actionCode></output></span></font></td>";
	content += "</tr>";
	content += "<tr><td colspan=4><span><br></span></td></tr>";
	content += "<tr>";
	content += "<td colspan=2><button type=button style=background:#FFCC99 onclick=controlPm(1)>Standby</button></td>";
	content += "</tr>";
	content += "</body>";
	content += "<script>";
	content += "function controlServo(id, textId) {";
	content += "var xhttp = new XMLHttpRequest();";
	content += "var value = document.getElementById(textId).value;";
	content += "document.querySelector('#range_' + id).value = value;";
	content += "document.querySelector('#servo_' + id).value = value;";
	content += "actionCode();";
	content += "xhttp.onreadystatechange = function() {";
	content += "if (xhttp.readyState == 4 && xhttp.status == 200) {";
	content += "document.getElementById(\"demo\").innerHTML = xhttp.responseText;";
	content += "}";
	content += "};";
	content += "xhttp.open(\"GET\",\"controller?servo=\"+id+\"&value=\"+value, true);";
	content += "xhttp.send();";
	content += "}";
	content += "function controlPm(value) {";
	content += "var xhttp = new XMLHttpRequest();";
	content += "xhttp.onreadystatechange = function() {";
	content += "if (xhttp.readyState == 4 && xhttp.status == 200) {";
	content += "document.getElementById(\"demo\").innerHTML = xhttp.responseText;";
	content += "}";
	content += "};";
	content += "xhttp.open(\"GET\", \"controller?pm=\"+value, true);";
	content += "xhttp.send();";
	content += "}";
	content += "function actionCode() {";
	content += "document.querySelector('#actionCode').value =";
	content += "document.getElementById('servo_0').value + ', '";
	content += "+ document.getElementById('servo_1').value + ', '";
	content += "+ document.getElementById('servo_2').value + ', '";
	content += "+ document.getElementById('servo_3').value + ', '";
	content += "+ document.getElementById('servo_4').value + ', '";
	content += "+ document.getElementById('servo_5').value + ', '";
	content += "+ document.getElementById('servo_6').value + ', '";
	content += "+ document.getElementById('servo_7').value;";
	content += "}";
	content += "</script>";
	content += "</html>";

	server.send(200, "text/html", content);
}

// Main controller
void handleIndex()
{
	String content = "";

	content += "<html>";
	content += "<head>";
	content += "<title>Spider Robot</title>";
	content += "<meta charset=UTF-8>";
	content += "<meta name=viewport content=width=device-width>";
	content += "<style type=text/css>";
	content += "body {";
	content += "margin: 0px;";
	content += "backgound-color: #FFFFFF;";
	content += "font-family: helvetica, arial;";
	content += "font-size: 100%;";
	content += "color: #555555;";
	content += "}";
	content += "td {";
	content += "text-align: center;";
	content += "}";
	content += "span {";
	content += "font-family: helvetica, arial;";
	content += "font-size: 70%;";
	content += "color: #777777;";
	content += "}";
	content += "button {";
	content += "width: 90%;";
	content += "height: 90%;";
	content += "font-family: helvetica, arial;";
	content += "font-size: 100%;";
	content += "color: #555555;";
	content += "background: #BFDFFF;";
	content += "padding: 5px 5px 5px 5px;";
	content += "border: none;";
	content += "}";
	content += "</style>";
	content += "</head>";
	content += "<body>";
	content += "<br>";
	content += "<table width=100% height=90%>";
	content += "<tr height=19%>";
	content += "<td width=33%><button type=button style=background:#BFFFCF onclick=controlPm(6)>左转</button></td>"; //Turn left
	content += "<td width=33%><button type=button onclick=controlPm(2)>前进</button></td>"; //Forward
	content += "<td width=33%><button type=button style=background:#BFFFCF onclick=controlPm(7)>右转</button></td>"; //Turn Right
	content += "</tr>";
	content += "<tr height=19%>";
	content += "<td><button type=button onclick=controlPm(4)>左移</button></td>"; //Left shift
	content += "<td><button type=button onclick=controlPm(3)>后退</button></td>"; //Backward
	content += "<td><button type=button onclick=controlPm(5)>右移</button></td>"; //Right shift
	content += "</tr>";
	content += "<tr height=5%><td colspan=3><span><br></span></td></tr>";
	content += "<tr height=20%>";
	content += "<td><button type=button style=background:#FFCC99 onclick=controlPm(1)>待机</button></td>"; //Standby
	content += "<td><button type=button style=background:#FFE599 onclick=controlPm(9)>打招呼</button></td>"; //Say Hi
	content += "<td><button type=button style=background:#FFE599 onclick=controlPm(11)>俯卧撑</button></td>"; //Push up
	content += "</tr>";
	content += "<tr height=19%>";
	content += "<td><button type=button style=background:#FFE599 onclick=controlPm(8)>躺下</button></td>"; //Lie
	content += "<td><button type=button style=background:#FFE599 onclick=controlPm(10)>战斗</button></td>"; //Fighting
	content += "<td><button type=button style=background:#FFBFBF onclick=controlPm(12)>睡眠</button></td>"; //Sleep
	content += "</tr>";
	content += "<tr height=19%>";
	content += "<td><button type=button style=background:#CFBFFF onclick=controlPm(13)>舞步1</button></td>"; //Dance1
	content += "<td><button type=button style=background:#CFBFFF onclick=controlPm(14)>舞步2</button></td>"; //Dance2
	content += "<td><button type=button style=background:#CFBFFF onclick=controlPm(15)>舞步3</button></td>"; //Dance3
	content += "</tr>";
	content += "</table>";
	content += "<span><p align=center>" + FW_Version + "</p></span>";
	content += "</body>";
	content += "<script>";
	content += "function controlPm(id) {";
	content += "var xhttp = new XMLHttpRequest();";
	content += "xhttp.onreadystatechange = function() {";
	content += "if (xhttp.readyState == 4 && xhttp.status == 200) {";
	content += "}";
	content += "};";
	content += "xhttp.open(\"GET\", \"controller?pm=\"+id, true);";
	content += "xhttp.send();";
	content += "}";
	content += "function controlPms(id) {";
	content += "var xhttp = new XMLHttpRequest();";
	content += "xhttp.onreadystatechange = function() {";
	content += "if (xhttp.readyState == 4 && xhttp.status == 200) {";
	content += "}";
	content += "};";
	content += "xhttp.open(\"GET\", \"controller?pms=\"+id, true);";
	content += "xhttp.send();";
	content += "}";
	content += "</script>";
	content += "</html>";

	server.send(200, "text/html", content);
}
