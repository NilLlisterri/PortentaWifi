#include <WiFi.h>


char ssid[] = "PortentaAccessPoint";    // your network SSID (name)
char pass[] = "123Qwerty";    // your network password (use for WPA, or use as key for WEP)

int keyIndex = 0;             // your network key Index number (needed only for WEP)
int status = WL_IDLE_STATUS;

WiFiServer server(80);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial) {
    delay(50); // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("Access Point Web Server");

  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);

  // by default the local IP address of will be 192.168.3.1
  // you can override it with the following:
  // WiFi.config(IPAddress(10, 0, 0, 1));

  if (strlen(pass) < 8) {
    Serial.println("Creating access point failed");
    Serial.println("The Wi-Fi password must be at least 8 characters long");
    // don't continue
    while (true);
  }

  // print the network name (SSID);
  Serial.print("Creating access point named: ");
  Serial.println(ssid);

  //Create the Access point
  status = WiFi.beginAP(ssid, pass);
  if (status != WL_AP_LISTENING) {
    Serial.println("Creating access point failed");
    // don't continue
    while (true);
  }

  // wait 10 seconds for connection:
  delay(10000);

  // start the web server on port 80
  server.begin();
  // you're connected now, so print out the status
  printWiFiStatus();
}


void loop() {
  // compare the previous status to the current status
  if (status != WiFi.status()) {
    // it has changed update the variable
    status = WiFi.status();
    if (status == WL_AP_CONNECTED) {
      // a device has connected to the AP
      Serial.println("Device connected to AP");
    } else {
      // a device has disconnected from the AP, and we are back in listening mode
      Serial.println("Device disconnected from AP");
    }
  }

  WiFiClient client = server.available();   // listen for incoming clients
  if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client

    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor

        client.print("Char received: ");
        client.println(c);
      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }

}

void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  // print your Wi-Fi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}
