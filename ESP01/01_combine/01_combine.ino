#include <Arduino.h>
#include <arduino_homekit_server.h>
#include "wifi_info.h"

#define LOG_D(fmt, ...)   printf_P(PSTR(fmt "\n") , ##__VA_ARGS__);

//Communication with KNX 
int message[7];
byte veri;

void KNXinit(){
	message[0] = 1;
	message[1] = 5;
	message[2] = 1;
	message[3] = 1;
	message[4] = 10;//A for main lamp
	message[5] = 1; //length of value (bit);
	message[6] = 1; //1 for on 0 for off
	message[7] = 8; //the XOR8 verification 
}
void operation (char pos,char val){
  //pos is the ascii value of char 
  //pos value should be 97-107 (a-k)
  //val should be ascii value of 0/1 48-49
    for(int i = 0; i<4;i++){
      Serial.write(message[i]);
    } 
  	veri = 0100;
    //Verification code until here is 00000100
    Serial.write(pos-87);
    veri = B00000100 ^ (pos-87);
    Serial.write(1);
    veri = veri^1;
    Serial.write(val-48);
    veri = veri^(val-48);
    //Verification code until here is 
    Serial.write(veri);
}



//End of Communication with KNX 

void setup() {
	Serial.begin(115200);
   pinMode(LED_BUILTIN, OUTPUT);
	wifi_connect(); // in wifi_info.h
//	homekit_storage_reset(); // to remove the previous HomeKit pairing storage when you first run this new HomeKit example
	my_homekit_setup();
	KNXinit();
  system_set_os_print(0);
}

void loop() {
	my_homekit_loop();
	delay(10);
}

//==============================
// HomeKit setup and loop
//==============================

// access your HomeKit characteristics defined in my_accessory.c
extern "C" homekit_server_config_t config;
extern "C" homekit_characteristic_t cha_mainlight_on;
extern "C" homekit_characteristic_t cha_lightstrip_on;
extern "C" homekit_characteristic_t cha_spotlight_on;
extern "C" homekit_characteristic_t cha_porchlight_on;
extern "C" homekit_characteristic_t cha_bathroomspotlight_on;
extern "C" homekit_characteristic_t cha_bathroomlight_on;

static uint32_t next_heap_millis = 0;

#define PIN_SWITCH 2

//Called when the switch value is changed by iOS Home APP
//void cha_switch_on_setter(const homekit_value_t value) {
//	bool on = value.bool_value;
//	cha_switch_on.value.bool_value = on;	//sync the value
//	LOG_D("Switch: %s", on ? "ON" : "OFF");
//}

void cha_mainlight_on_setter(const homekit_value_t value) {
	bool on = value.bool_value;
	cha_mainlight_on.value.bool_value = on;	//sync the value
	// LOG_D("Mainlight: %s", on ? "ON" : "OFF");
	delay(20);
	operation('a',on ? '1':'0');
	digitalWrite(LED_BUILTIN, on ? LOW : HIGH);
	//KNX implementation here.
}
void cha_lightstrip_on_setter(const homekit_value_t value) {
	bool on = value.bool_value;
	cha_lightstrip_on.value.bool_value = on;	//sync the value
	// LOG_D("Light_strip: %s", on ? "ON" : "OFF");
	delay(20);
	operation('b',on ? '1':'0');
	//KNX implementation here.
}
void cha_spotlight_on_setter(const homekit_value_t value) {
	bool on = value.bool_value;
	cha_spotlight_on.value.bool_value = on;	//sync the value
	// LOG_D("Spot_light: %s", on ? "ON" : "OFF");
	delay(20);
	operation('c',on ? '1':'0');
	//KNX implementation here.
}
void cha_porchlight_on_setter(const homekit_value_t value) {
	bool on = value.bool_value;
	cha_porchlight_on.value.bool_value = on;	//sync the value
	// LOG_D("Porch_light: %s", on ? "ON" : "OFF");
	delay(20);
	operation('d',on ? '1':'0');
	//KNX implementation here.
}
void cha_bathroomspotlight_on_setter(const homekit_value_t value) {
	bool on = value.bool_value;
	cha_bathroomspotlight_on.value.bool_value = on;	//sync the value
	// LOG_D("Bathroom_spotlight: %s", on ? "ON" : "OFF");
	delay(20);
	operation('e',on ? '1':'0');
	//KNX implementation here.
}
void cha_bathroomlight_on_setter(const homekit_value_t value) {
	bool on = value.bool_value;
	cha_bathroomlight_on.value.bool_value = on;	//sync the value
	// LOG_D("Bathroom_light: %s", on ? "ON" : "OFF");
	delay(20);
	operation('f',on ? '1':'0');
	//KNX implementation here.
}
void my_homekit_setup() {
	pinMode(PIN_SWITCH, OUTPUT);
	digitalWrite(PIN_SWITCH, HIGH);

	//Add the .setter function to get the switch-event sent from iOS Home APP.
	//The .setter should be added before arduino_homekit_setup.
	//HomeKit sever uses the .setter_ex internally, see homekit_accessories_init function.
	//Maybe this is a legacy design issue in the original esp-homekit library,
	//and I have no reason to modify this "feature".

	cha_mainlight_on.setter = cha_mainlight_on_setter;
	cha_lightstrip_on.setter = cha_lightstrip_on_setter;
	cha_spotlight_on.setter = cha_spotlight_on_setter;
	cha_porchlight_on.setter = cha_porchlight_on_setter;
	cha_bathroomspotlight_on.setter = cha_bathroomspotlight_on_setter;
	cha_bathroomlight_on.setter = cha_bathroomlight_on_setter;
	arduino_homekit_setup(&config);

	//report the switch value to HomeKit if it is changed (e.g. by a physical button)
	//bool switch_is_on = true/false;
	//cha_switch_on.value.bool_value = switch_is_on;
	//homekit_characteristic_notify(&cha_switch_on, cha_switch_on.value);
}

void my_homekit_loop() {
	arduino_homekit_loop();
	const uint32_t t = millis();
	if (t > next_heap_millis) {
		// show heap info every 5 seconds
		next_heap_millis = t + 5 * 1000;
		// LOG_D("Free heap: %d, HomeKit clients: %d",
		// 		ESP.getFreeHeap(), arduino_homekit_connected_clients_count());

	}
}
