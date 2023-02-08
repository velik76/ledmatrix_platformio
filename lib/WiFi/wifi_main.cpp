#include "wifi_main.h"

#include <Arduino.h>
#include <NTPClient.h>
#include <WiFi.h>
#include <WiFiUdp.h>

#include "led_matrix_main.h"

#define NETWORK_SSID "Velik1"
#define NETWORK_PASS ""
#define MAX_TRYING_TO_SETUP_CONNECTION 20

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String ledState = "off";

// Assign output variables to GPIO pins
const int ledOutput = 2;

// Settings for NTP
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, 3600);

Wifi::Wifi(LedMatrix* ledMatrix)
    : _ledMatrix{ledMatrix}
{
}

void scanWifi()
{
#if 0    
    // WiFi.scanNetworks will return the number of networks found
    Serial.println("scan start");
    int n = WiFi.scanNetworks();
    Serial.println("scan done");
    if(n == 0)
        Serial.println("no networks found");
    else
    {
        Serial.print(n);
        Serial.println(" networks found");
        for(int i = 0; i < n; ++i)
        {
            // Print SSID and RSSI for each network found
            Serial.print(i + 1);
            Serial.print(": ");
            Serial.print(WiFi.SSID(i));
            Serial.print(" (");
            Serial.print(WiFi.RSSI(i));
            Serial.print(")");
            Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
            delay(10);
        }
    }
    Serial.println("");
#endif
}

void Wifi::http()
{
#if 1
    // Current time
    static unsigned long currentTime = millis();
    // Previous time
    static unsigned long previousTime = 0;
    // Define timeout time in milliseconds (example: 2000ms = 2s)
    const long timeoutTime = 2000;

    WiFiClient client = server.accept(); // Listen for incoming clients

    if(client)
    {                                  // If a new client connects,
        Serial.println("New Client."); // print a message out in the serial port
        String currentLine = "";       // make a String to hold incoming data from the client
        currentTime = millis();
        previousTime = currentTime;
        while(client.connected() && currentTime - previousTime <= timeoutTime)
        { // loop while the client's connected
            currentTime = millis();
            if(client.available())
            {                           // if there's bytes to read from the client,
                char c = client.read(); // read a byte, then
                Serial.write(c);        // print it out the serial monitor
                header += c;
                if(c == '\n')
                { // if the byte is a newline character
                    // if the current line is blank, you got two newline characters in a row.
                    // that's the end of the client HTTP request, so send a response:
                    if(currentLine.length() == 0)
                    {
                        Serial.print("Header: ");
                        Serial.println(header);

                        // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
                        // and a content-type so the client knows what's coming, then a blank line:
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-type:text/html");
                        client.println("Connection: close");
                        client.println();

                        // turns the LED on and off
                        if(header.indexOf("GET /led/on") >= 0)
                        {
                            Serial.println("LED on");
                            ledState = "on";
                            digitalWrite(ledOutput, HIGH);
                        }
                        else if(header.indexOf("GET /led/off") >= 0)
                        {
                            Serial.println("LED off");
                            ledState = "off";
                            digitalWrite(ledOutput, LOW);
                        }
                        else if(header.indexOf("GET /nextEffect") >= 0)
                        {
                            Serial.println("Switching to next effect");
                            _ledMatrix->switchToNextEnabledPattern();
                        }

                        // Display the HTML web page
                        client.println("<!DOCTYPE html><html>");
                        client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
                        client.println("<link rel=\"icon\" href=\"data:,\">");
                        // CSS to style the on/off buttons
                        // Feel free to change the background-color and font-size attributes to fit your preferences
                        client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");

                        client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
                        client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
                        client.println(".button2 { background-color: #77878A; border: none; color: white; padding: 16px 40px;");
                        client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
                        client.println("</style></head>");

                        // Web Page Heading
                        client.println("<body><h1>LED matrix control server</h1>");

                        // Display current state, and ON/OFF buttons for LED
                        client.println("<p>LED - State " + ledState + "</p>");
                        // If the ledState is off, it displays the ON button
                        if(ledState == "off")
                        {
                            client.println("<p><a href=\"/led/on\"><button class=\"button\">ON</button></a></p>");
                        }
                        else
                        {
                            client.println("<p><a href=\"/led/off\"><button class=\"button2\">OFF</button></a></p>");
                        }

                        client.println("<p>Matrix effect: " + _ledMatrix->getPatternName() + "</p>");
                        client.println("<p><a href=\"/nextEffect\"><button class=\"button\">Next effect</button></a></p>");

                        client.println("<p>");
                        client.println("<label for=\"effects\">Choose effect:</label>");
                        client.println("<select id=\"effects\" name=\"effects\" onchange=\"testSelect2()\">");
                        client.println("<option value=\"volvo\">Volvo</option>");
                        client.println("<option value=\"audi\">Audi</option>");
                        client.println("</select>");
                        client.println("<button onclick=\"testSelect()\">Test select</button>");
                        client.println("<p>");
                        client.println("<button id=\"switchEffect\" class=\"button\" onclick=\"window.location.href=\'/nextEffect\'\">Switch effect</button>");
                        client.println("</p>");
                        client.println("<script>");
                        client.println("function testSelect() {");
                        client.println("console.log(\"testSelect()!\");");
                        client.println("document.getElementById(\"demo\").innerHTML = document.getElementById(\"effects\").value ;");
                        client.println("}");
                        client.println("</script>");

                        client.println("<script>");
                        client.println("function testSelect2() {");
                        client.println("console.log(\"testSelect2()!\");");
                        client.println("document.getElementById(\"switchEffect\").onclick=\"window.location.href=\'/led/off\'\";");
                        client.println("}");
                        client.println("</script>");

                        client.println("<button onclick=\"myFunction()\">Click me</button>");
                        client.println("<p id=\"demo\"></p>");

                        client.println("<script>");
                        client.println("function myFunction() {");
                        client.println("document.getElementById(\"demo\").innerHTML = \"Hello World\";");
                        client.println("}");
                        client.println("</script>");

                        client.println("</body></html>");

                        // The HTTP response ends with another blank line
                        client.println();
                        // Break out of the while loop
                        break;
                    }
                    else
                    { // if you got a newline, then clear currentLine
                        currentLine = "";
                    }
                }
                else if(c != '\r')
                {                     // if you got anything else but a carriage return character,
                    currentLine += c; // add it to the end of the currentLine
                }
            }
        }
        // Clear the header variable
        header = "";
        // Close the connection
        client.stop();
        Serial.println("Client disconnected.");
        Serial.println("");
    }
#endif
}

bool Wifi::isConnected()
{
    return (WiFi.status() == WL_CONNECTED);
}

String Wifi::getIpAddress()
{
    if(isConnected())
    {
        return WiFi.localIP().toString();
    }
    return "Not connected";
}

bool Wifi::getNtpTime(int& h, int& m, int& s)
{
    if(isConnected())
    {
        h = timeClient.getHours();
        m = timeClient.getMinutes();
        s = timeClient.getSeconds();
        return true;
    }
    h = 0;
    m = 0;
    s = 0;
    return false;
}

String Wifi::getNtpTime()
{
    String result = "00:00";
    int h, m, s;
    if(getNtpTime(h, m, s))
    {
        String hStr = h < 10 ? "0" + String(h) : String(h);
        String mStr = m < 10 ? "0" + String(m) : String(m);
        result = hStr + ":" + mStr;
    }
    return result;
}

void Wifi::setupWiFi()
{
    if(String(NETWORK_PASS) == "")
    {
        Serial.println("No WiFi password defined in: NETWORK_SSID. Skip WiFi setup");
        return;
    }

    int trying = 0;
    WiFi.mode(WIFI_STA);
    WiFi.disconnect(true);

    // Connect to Wi-Fi network with SSID and password
    Serial.print("Connecting to: ");
    Serial.println(NETWORK_SSID);
    WiFi.begin(NETWORK_SSID, NETWORK_PASS);
    while(WiFi.status() != WL_CONNECTED && trying++ < MAX_TRYING_TO_SETUP_CONNECTION)
    {
        delay(500);
        Serial.print(".");
    }

    if(WiFi.status() == WL_CONNECTED)
    {
        // Print local IP address and start web server
        Serial.println("WiFi connected!");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
        server.begin();
        timeClient.begin();
        timeClient.setUpdateInterval(1000);
        while(!timeClient.update())
            timeClient.forceUpdate();
    }
    else
    {
        Serial.println("Could not connect to WiFi");
    }
}

void Wifi::loopWifi()
{
    if(WiFi.status() == WL_CONNECTED)
    {
        http();
    }
}
