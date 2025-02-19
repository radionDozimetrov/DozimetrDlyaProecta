#include <DHT.h>                   // Библиотека для датчика температуры и влажности
#include <Adafruit_GFX.h>          // Графическая библиотека
#include <Adafruit_ST7735.h>       // Библиотека для дисплея
#include <Wire.h>                  // Для работы с I2C
#include <CG_RadSens.h>               // Библиотека для работы с RadSens

// Настройки DHT11
#define DHTPIN 2                   // Пин подключения DHT11
#define DHTTYPE DHT11              // Тип датчика DHT
DHT dht(DHTPIN, DHTTYPE);          // Инициализация объекта для работы с DHT

// Настройки TFT-дисплея (пины: CS, DC, RESET)
Adafruit_ST7735 tft = Adafruit_ST7735(10, 8, 9);

// Настройки RadSens
#define RADSENS_ADDRESS 0x66       // Адрес I2C устройства RadSens (по умолчанию 0x66)
CG_RadSens rad(RADSENS_ADDRESS);   // Инициализация объекта RadSens

void setup() {
    // Инициализация датчиков и дисплея
    dht.begin();                   // Запуск DHT
    Serial.begin(9600);            // Подключение монитора порта
    tft.initR(INITR_BLACKTAB);     // Инициализация TFT
    tft.fillScreen(ST77XX_BLACK);  // Очистка экрана
    tft.setTextColor(ST77XX_WHITE); // Цвет текста
    tft.setTextSize(1);            // Размер текста

    // Инициализация RadSens
    if (!rad.init()) {             // Проверка подключения RadSens
        Serial.println("RadSens не найден. Проверьте подключение!");
        while (1);                 // Остановка программы, если датчик не найден
    }
    Serial.println("RadSens подключён.");
}

void loop() {
    // Считываем данные с DHT11
    float temperature = dht.readTemperature();  // Температура
    float humidity = dht.readHumidity();        // Влажность

    // Проверяем корректность данных
    if (isnan(temperature) || isnan(humidity)) {
        Serial.println("Ошибка чтения DHT11!");
        return;
    }

    // Считываем данные с RadSens
    float radIntensityDynamic = rad.getRadIntensyDynamic();  // Динамическая интенсивность радиации
    uint32_t pulseCount = rad.getNumberOfPulses();           // Общее число импульсов

    // Вывод данных в монитор порта
    Serial.print("Температура: ");
    Serial.print(temperature);
    Serial.println(" °C");
    Serial.print("Влажность: ");
    Serial.print(humidity);
    Serial.println(" %");
    Serial.print("Интенсивность радиации (динамическая): ");
    Serial.print(radIntensityDynamic);
    Serial.println(" мкР/ч");
    Serial.print("Число импульсов: ");
    Serial.println(pulseCount);
    uint16_t sensitivity = rad.getSensitivity();
Serial.print("Чувствительность: ");
Serial.println(sensitivity);

    // Отображение данных на дисплее
    tft.fillScreen(ST77XX_BLACK);  // Очистка экрана

    tft.setCursor(0, 0);
    tft.print("Temp: ");
    tft.print(temperature);
    tft.println(" C");

    tft.setCursor(0, 20);
    tft.print("Humidity: ");
    tft.print(humidity);
    tft.println(" %");

    tft.setCursor(0, 40);
    tft.print("Rad: ");
    tft.print(radIntensityDynamic);
    tft.println(" mcR/h");

    tft.setCursor(0, 60);
    tft.print("Pulses: ");
    tft.print(pulseCount);

    tft.setCursor(0,80);
    tft.print("Sensevity: ");
    tft.print(sensitivity);

    // Задержка между измерениями
    delay(3000);
}
