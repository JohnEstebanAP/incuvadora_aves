/*
Dias de incubacion 21, con retraso serian 28 dias.

La temperatura adecuada es clave para el éxito de la incubación. 
Lo ideal es mantener una temperatura constante de 37,5 
grados Celsius durante los primeros 18 días. 
A partir del día 19, se baja la temperatura a 36 grados Celsius
para preparar a los pollitos para su eclosión.

Es importante controlar la humedad dentro de la incubadora, 
ya que una humedad baja puede impedir el desarrollo adecuado 
del embrión y una humedad alta puede causar deformidades en los pollitos. 
La humedad recomendada es del 50% al 60% durante los primeros 18 días 
y del 70% al 80% durante los últimos días de incubación, y 65 de humedad en los ultimos 3 dias


una humedad superior a 70% es perjudicial para el embrion generando
falta de osigeno diminullendo laasorsion del liquido admiotico
generando lamuerte del embrion o malformaciones por lo que es 
muy importante controlar laumeda.


No le de vuelta después del dia 18



// Include the DHT11 library for interfacing with the sensor.
*/
#include <DHT11.h>
#include <ESP8266WiFi.h>

// Create an instance of the DHT11 class.
//Entradas
DHT11 dht11(D8);
// Configuración inicial
WiFiServer server(80);

//salidas
const int reletTemperature = D13;
const int relethumidity = D12;

unsigned long startTime;  // Tiempo inicial
int humiditysensorOutput = 0;
bool isreletTemperatureOpen = false;

//variables temporales para registar cambios de estado
int dayTemp = -1;
int temperatureTemp = -1;
int humidityTemp = -1;

int dayCiclo1 = 18;  //<=
int dayCiclo2 = 18;  //>
int dayCiclo3 = 20;  //>
int dayCiclo4 = 0;   //>

int humidityCiclo_1 = 50;
int humidityCiclo_1_5 = 60;
int humidityCiclo_2 = 70;
int humidityCiclo_2_5 = 80;
int humidityCiclo_3 = 65;
int humidityCiclo_3_5 = 65;
int humidityCiclo_4 = 0;
int humidityCiclo_4_5 = 0;

int temperatureCiclo_1 = 38;
int temperatureCiclo_1_5 = 38;
int temperatureCiclo_2 = 36;
int temperatureCiclo_2_5 = 36;
int temperatureCiclo_3 = 0;
int temperatureCiclo_3_5 = 0;
int temperatureCiclo_4 = 0;
int temperatureCiclo_4_5 = 0;

//configuracion para red local
String red = "MiRed";
String passwordRed = "MiRed1234";

// Servidor HTTP
//configuracion para conecion a red wifi
String logConsola = "";
String ssidClient = "";
String passwordClient = "";

void setup() {
  Serial.begin(9600);
  // Uncomment the line below to set a custom delay between sensor readings (in milliseconds).
  dht11.setDelay(500);  // Set this to the desired delay. Default is 500ms.

  startTime = millis();  // Registrar el inicio del ciclo

  pinMode(reletTemperature, OUTPUT);
  pinMode(relethumidity, OUTPUT);

  iniciarAccessPoint();
}

void loop() {
  int temperature = 0;
  int humidity = 0;

  // Calcular días desde el inicio
  unsigned long elapsedTime = millis() - startTime;
  //int currentDay = elapsedTime / (1000 * 60 * 60 * 24); // Tiempo real de un día
  int currentDay = elapsedTime / (1000 * 60);  // Simulación: 1 minuto = 1 día

  // Attempt to read the temperature and humidity values from the DHT11 sensor.
  int result = dht11.readTemperatureHumidity(temperature, humidity);

  printInfoDay(currentDay);
  dht11Info(result, temperature, humidity);
  validateTemperature(currentDay, temperature);
  validateUmedity(currentDay, humidity);

  startServerWebInLoop();
}

void startServerWebInLoop() {
  WiFiClient client = server.available();
  if (client) {
    logMensaje(F("Nuevo cliente conectado."));

    unsigned long timeout = millis();
    while (!client.available() && client.connected()) {
      if (millis() - timeout > 5000) {  // 5 segundos de espera
        logMensaje(F("Cliente no disponible: timeout.")); 
        return;         // Salir de la función
      }
      delay(1);
    }

    char request[256];
    int length = client.readBytesUntil('\r', request, sizeof(request) - 1);
    request[length] = '\0';  // Asegurar el fin de la cadena
    logMensaje(F("Solicitud recibida:"));
    logMensaje(request);

    manejarComandos(request);
    enviarPaginaWeb(client);
    client.flush();
    client.stop();
  }
}

void printInfoDay(int currentDay) {

  if (dayTemp != currentDay) {
    dayTemp = currentDay;
    logMensaje("Día actual: ");
    logMensaje(String(currentDay));
  }
}

void dht11Info(int result, int temperature, int humidity) {
  if (result != 0) {
    // Print error message based on the error code.
    logMensaje(DHT11::getErrorString(result));
    return;
  }


  if (temperatureTemp != temperature) {
    temperatureTemp = temperature;
    logMensaje("Temperature: ");
    logMensaje(String(temperature));
    logMensaje(" °C\t");
  }

  if (humidityTemp != humidity) {
    humidityTemp = humidity;
    logMensaje("Humidity: ");
    logMensaje(String(humidity));
    logMensaje(" %");
  }
}

void validateTemperature(int currentDay, int temperature) {

  //ciclo1
  if (temperatureCiclo_1 == 0) {
    return;
  }
  if (temperature <= temperatureCiclo_1 && currentDay <= dayCiclo1) {
    digitalWrite(reletTemperature, HIGH);
  } else if (temperature > temperatureCiclo_1_5 && currentDay <= dayCiclo1) {
    digitalWrite(reletTemperature, LOW);
  }

  //ciclo2
  if (temperatureCiclo_2 == 0) {
    return;
  }
  if (temperature <= temperatureCiclo_2 && currentDay > dayCiclo2) {
    digitalWrite(reletTemperature, HIGH);
  } else if (temperature > temperatureCiclo_2_5 && currentDay > dayCiclo2) {
    digitalWrite(reletTemperature, LOW);
  }

  //ciclo3
  if (temperatureCiclo_3 == 0) {
    return;
  }
  if (temperature <= temperatureCiclo_3 && currentDay > dayCiclo3) {
    digitalWrite(reletTemperature, HIGH);
  } else if (temperature > temperatureCiclo_3_5 && currentDay > dayCiclo3) {
    digitalWrite(reletTemperature, LOW);
  }

  //ciclo4
  if (temperatureCiclo_4 == 0) {
    return;
  }
  if (temperature <= temperatureCiclo_4 && currentDay > dayCiclo4) {
    digitalWrite(reletTemperature, HIGH);
  } else if (temperature > temperatureCiclo_4_5 && currentDay > dayCiclo4) {
    digitalWrite(reletTemperature, LOW);
  }
}

void validateUmedity(int currentDay, int humidity) {

  //ciclo1
  //La humedad recomendada es del 50% al 60% durante los primeros 18 días
  if (humidityCiclo_1 == 0) {
    return;
  }
  if (humidity <= humidityCiclo_1 && currentDay <= dayCiclo1) {
    digitalWrite(relethumidity, HIGH);
  } else if (humidity > humidityCiclo_1_5 && currentDay <= dayCiclo1) {
    digitalWrite(relethumidity, LOW);
  }

  //ciclo2
  // del 70% al 80% durante los últimos días de incubación,
  if (humidityCiclo_2 == 0) {
    return;
  }
  if (humidity <= humidityCiclo_2 && currentDay > dayCiclo2) {
    digitalWrite(relethumidity, HIGH);
  } else if (humidity > humidityCiclo_2_5 && currentDay > dayCiclo2) {
    digitalWrite(relethumidity, LOW);
  }

  //ciclo3
  //y 65 de humedad en los ultimos 3 dias
  if (humidityCiclo_3 == 0) {
    return;
  }
  if (humidity <= humidityCiclo_3 && currentDay > dayCiclo3) {
    digitalWrite(relethumidity, HIGH);
  } else if (humidity > humidityCiclo_3_5 && currentDay > dayCiclo3) {
    digitalWrite(relethumidity, LOW);
  }

  //ciclo4
  //y 65 de humedad en los ultimos 3 dias
  if (humidityCiclo_4 == 0) {
    return;
  }
  if (humidity <= humidityCiclo_4_5 && currentDay > dayCiclo4) {
    digitalWrite(relethumidity, HIGH);
  } else if (humidity > humidityCiclo_4_5 && currentDay > dayCiclo4) {
    digitalWrite(relethumidity, LOW);
  }
}

// Función para iniciar en modo Access Point
ICACHE_FLASH_ATTR void iniciarAccessPoint() {
  logMensaje(F("Iniciando Access Point..."));
  server.stop();  // Detener servidor HTTP previamente iniciado
  WiFi.disconnect(true);
  delay(100);
  WiFi.mode(WIFI_AP);

  //WiFi.softAP(redBuffer, passwordBuffer);
  WiFi.softAP(red, passwordRed);
  server.begin();  // Reiniciar servidor en modo AP

  logMensaje(F("Access Point activado. IP:"));
  logMensaje(WiFi.softAPIP().toString().c_str());

  logMensaje(ESP.getResetInfo());
}

// Función para conectarse a modo Estación
ICACHE_FLASH_ATTR void conectarEstacion() {
  server.stop();
  // Detener servidor HTTP previamente iniciado
  WiFi.disconnect(true);  // Desconectar configuraciones previas

  delay(100);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssidClient, passwordClient);
  logMensaje(F("Conectando al WiFi..."));

  unsigned long startAttemptTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 15000) {
    delay(500);
    logMensaje(F("Intentando conectar..."));
  }
  logMensaje("Estado de conexión WiFi: " + String(WiFi.status()));
  if (WiFi.status() == WL_CONNECTED) {
    logMensaje(F("Conexión establecida. IP:"));
    logMensaje(WiFi.localIP().toString());
    server.begin();  // Reiniciar servidor en modo AP
  } else {
    logMensaje(F("Error al conectar. Revisar credenciales."));
    iniciarAccessPoint();
  }
}

// Función para manejar comandos del navegador
ICACHE_FLASH_ATTR void manejarComandos(const char* request) {
  if (strstr(request, "SSID_LOCAL=") && strstr(request, "PASSWORD_LOCAL=")) {
    parsearParametro(request, "SSID_LOCAL=", red);
    parsearParametro(request, "PASSWORD_LOCAL=", passwordRed);

    logMensaje(F("Datos recibidos:"));
    logMensaje(String(F("SSID_LOCAL: ")) + red);
    logMensaje(String(F("Contraseña_LOCAL: ")) + passwordRed);

    iniciarAccessPoint();
  } else if (strstr(request, "SSID=") && strstr(request, "PASSWORD=")) {
    parsearParametro(request, "SSID=", ssidClient);
    parsearParametro(request, "PASSWORD=", passwordClient);

    logMensaje(F("Datos recibidos:"));
    logMensaje(String(F("SSID: ")) + ssidClient);
    logMensaje(String(F("Contraseña: ")) + passwordClient);

    conectarEstacion();
    // Finalizar la conexión
    server.available().stop();
  }
}

// Función para generar la página web
ICACHE_FLASH_ATTR void enviarPaginaWeb(WiFiClient& client) {
  client.println(F("HTTP/1.1 200 OK"));
  client.println(F("Content-Type: text/html"));
  client.println(F("Connection: close"));
  client.println();

  client.println(F("<html><head><title>Control ESP8266</title></head><body>"));
  client.println(F("<h1>Configuración WiFi</h1>"));

  // Mostrar log en la página web
  client.println(F("<h2>Log de la Consola</h2><div style='font-family:monospace;'>"));
  client.println(logConsola);
  client.println(F("</div>"));

  // Formulario de configuración para conectar a red wifi
  client.println(F("<h2>Configuración para conectar a red wifi</h2>"));
  client.println(F("<form method='GET'>"));
  client.println(F("Nombre:<br><input type='text' name='SSID'><br>"));
  client.println(F("Contraseña:<br><input type='text' name='PASSWORD'><br><br>"));
  client.println(F("<input type='submit' value='Conectar a una red wifi'>"));
  client.println(F("</form>"));

  client.println(F("<br>"));
  client.println(F("<br>"));

  // Formulario de configuración de red local
  client.println(F("<h2>Configuración de red local</h2>"));
  client.println(F("<h4>Conectarse o actualizar nombre de Red Local</h4>"));
  client.println(F("<form method='GET'>"));
  client.println(F("Nombre:<br><input type='text' name='SSID_LOCAL'><br>"));
  client.println(F("Contraseña:<br><input type='text' name='PASSWORD_LOCAL'><br><br>"));
  client.println(F("<input type='submit' value='Crear red local'>"));
  client.println(F("</form>"));

  client.println(F("</body></html>"));
}

// Función para extraer parámetros de la solicitud HTTP
ICACHE_FLASH_ATTR void parsearParametro(const String& request, const String& parametro, String& output) {
  // Buscar el inicio del parámetro en el request
  int startIndex = request.indexOf(parametro);
  if (startIndex != -1) {
    startIndex += parametro.length();                 // Avanzar después del parámetro
    int endIndex = request.indexOf('&', startIndex);  // Buscar el fin del parámetro
    if (endIndex == -1) {
      endIndex = request.indexOf(' ', startIndex);  // Buscar espacio si no hay '&'
    }
    if (endIndex == -1) {
      endIndex = request.length();  // Si no hay '&', tomar hasta el final
    }
    output = request.substring(startIndex, endIndex);  // Extraer el valor
  } else {
    output = "";  // Si no se encuentra, devolver vacío
  }
}


// Función para manejar log con literales en memoria flash
ICACHE_FLASH_ATTR void logMensaje(const __FlashStringHelper* mensaje) {
  Serial.println(mensaje);
  logConsola += String(mensaje) + "<br>";
  limitarLog();
}

// Función para manejar log con cadenas dinámicas
ICACHE_FLASH_ATTR void logMensaje(const String& mensaje) {
  Serial.println(mensaje);
  logConsola += mensaje + "<br>";
  limitarLog();
}

// Función auxiliar para limitar el tamaño del log
ICACHE_FLASH_ATTR void limitarLog() {
  if (logConsola.length() > 2048) {  // Limitar el tamaño del log
    logConsola = logConsola.substring(logConsola.length() - 150);
  }
}