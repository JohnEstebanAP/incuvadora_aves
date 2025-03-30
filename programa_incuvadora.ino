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

// Create an instance of the DHT11 class.
//Entradas
DHT11 dht11(D8);

//saldas
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

void setup() {
  Serial.begin(9600);
  // Uncomment the line below to set a custom delay between sensor readings (in milliseconds).
  dht11.setDelay(500);  // Set this to the desired delay. Default is 500ms.

  startTime = millis();  // Registrar el inicio del ciclo

  pinMode(reletTemperature, OUTPUT);
  pinMode(relethumidity, OUTPUT);
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
}
void printInfoDay(int currentDay) {

  if (dayTemp != currentDay) {
    dayTemp = currentDay;
    Serial.print("Día actual: ");
    Serial.println(currentDay);
  }
}

void dht11Info(int result, int temperature, int humidity) {
  if (result != 0) {
    // Print error message based on the error code.
    Serial.println(DHT11::getErrorString(result));
    return;
  }


  if (temperatureTemp != temperature) {
    temperatureTemp = temperature;
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" °C\t");
  }

  if (humidityTemp != humidity) {
    humidityTemp = humidity;
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");
  }
}

void validateTemperature(int currentDay, int temperature) {

  //ciclo1
  if (temperatureCiclo_1 == 0) {
    return;
  }
  if (temperature <= temperatureCiclo_1 && currentDay <= dayCiclo1) {
    digitalWrite(relethumidity, HIGH);
  } else if (temperature > temperatureCiclo_1_5 && currentDay <= dayCiclo1) {
    digitalWrite(relethumidity, LOW);
  }

  //ciclo2
  if (temperatureCiclo_2 == 0) {
    return;
  }
  if (temperature <= temperatureCiclo_2 && currentDay > dayCiclo2) {
    digitalWrite(relethumidity, HIGH);
  } else if (temperature > temperatureCiclo_2_5 && currentDay > dayCiclo2) {
    digitalWrite(relethumidity, LOW);
  }

  //ciclo3
  if (temperatureCiclo_3 == 0) {
    return;
  }
  if (temperature <= temperatureCiclo_3 && currentDay > dayCiclo3) {
    digitalWrite(relethumidity, HIGH);
  } else if (temperature > temperatureCiclo_3_5 && currentDay > dayCiclo3) {
    digitalWrite(relethumidity, LOW);
  }

  //ciclo4
  if (temperatureCiclo_4 == 0) {
    return;
  }
  if (temperature <= temperatureCiclo_4 && currentDay > dayCiclo4) {
    digitalWrite(relethumidity, HIGH);
  } else if (temperature > temperatureCiclo_4_5 && currentDay > dayCiclo4) {
    digitalWrite(relethumidity, LOW);
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