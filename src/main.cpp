/*
 * Blink
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Ethernet.h>

// Definicoes de IP, mascara de rede e gateway
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(150, 161, 138, 39);      //Define o endereco IP
IPAddress gateway(150, 161, 138, 254);  //Define o gateway
IPAddress subnet(255, 255, 255, 0); //Define a máscara de rede

//Inicializa o servidor web na porta 80
EthernetServer server(80);
// constants won't change. They're used here to set pin numbers:
const int buttonPin = 2; // the number of the pushbutton pin
const int ledPin = 13;   // the number of the LED pin

// variables will change:
int buttonState = 0; // variable for reading the pushbutton status

void setup()
{
    // initialize the LED pin as an output:
    pinMode(ledPin, OUTPUT);
    // initialize the pushbutton pin as an input:
    pinMode(buttonPin, INPUT);
    //Inicializa a interface de rede
    Ethernet.begin(mac, ip, gateway, subnet);
    server.begin();
    Serial.begin(9600);
    while (!Serial)
    {
        ;
    }
    Serial.print("Socket Server:");
    Serial.println(Ethernet.localIP());
}

void loop()
{
    //Aguarda conexao do browser
    EthernetClient client = server.available();

    if (client)
    {
        Serial.println("new client");
        // an http request ends with a blank line
        boolean currentLineIsBlank = true;
        while (client.connected())
        {
            if (client.available())
            {
                char c = client.read();
                Serial.write(c);
                // if you've gotten to the end of the line (received a newline
                // character) and the line is blank, the http request has ended,
                // so you can send a reply
                if (c == 'n' && currentLineIsBlank)
                {
                    // send a standard http response header
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-Type: text/html");
                    client.println("Connection: close");
                    client.println("Refresh: 1"); //Recarrega a pagina a cada 2seg
                    client.println();
                    client.println("<!DOCTYPE HTML>");
                    client.println("<html>");
                    //Configura o texto e imprime o titulo no browser
                    client.print("<font color=#FF0000><b><u>");
                    client.print("Titulo");
                    client.print("</u></b></font>");
                    client.println("<br />");
                    client.println("<br />");
                    //Mostra o estado da porta digital 3
                    int porta_digital = digitalRead(buttonPin);
                    client.print("Botao 1 ");
                    client.print("<b>");
                    client.print(porta_digital);
                    client.println("</b>");
                    client.print("  (0 = Desligada, 1 = Ligada)");
                    client.println("<br />");
                    //Mostra as informacoes lidas pelo sensor ultrasonico
                    client.print("Status Led: ");
                    client.print("<b>");
                    client.print(porta_digital);
                    client.print(".");
                    client.println("</b></html>");
                    if (porta_digital)
                        // turn the LED on (HIGH is the voltage level)
                        digitalWrite(ledPin, HIGH);
                    else
                        // turn the LED off by making the voltage LOW
                        digitalWrite(ledPin, LOW);
                    break;
                }
                if (c == 'n')
                {
                    // you're starting a new line
                    currentLineIsBlank = true;
                }
                else if (c != 'r')
                {
                    // you've gotten a character on the current line
                    currentLineIsBlank = false;
                }
            }
        }
        // give the web browser time to receive the data
        delay(1);
        // close the connection:
        client.stop();
    }
}