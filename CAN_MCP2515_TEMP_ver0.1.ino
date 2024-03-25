/*
#AUTHOR Дмитрий Витальевич Мельник
#MAIL i.x.c.o.n@yandex.ru
#LICENSE Использование  только  для не коммерческих проектов, без упоминания автора. При использовании автор не несет ни какой отвественности, и не дает ни какой гарантии работы. Не ведет поддержку или исправление ошибок. 
*/

#include <SPI.h> //Библиотека для работу с MCP2515 по SPI
#include <mcp2515.h> //Библиотека с основными функциями для CAN модуля, CAN-A
#include <OneWire.h> //Библиотека для работы с датчиками температуры 1-wire

struct can_frame canMsg1; //Структура кадра
MCP2515 mcp2515(10); //Контакт выбора для работы модуля
OneWire ds(8); // Контакт датчика температуры


void setup() {//Вы полнится один раз
    canMsg1.can_id  = 0x004; // Задаем номер кадра
  canMsg1.can_dlc = 8; // Задаем размер кадра
  canMsg1.data[0] = 0x00; //Данные в байты кадра
  canMsg1.data[1] = 0x04; //Данные в байты кадра
  canMsg1.data[2] = 0x00; //Данные в байты кадра
  canMsg1.data[3] = 0x00; //Данные в байты кадра
  canMsg1.data[4] = 0x00; //Данные в байты кадра
  canMsg1.data[5] = 0x00; //Данные в байты кадра
  canMsg1.data[6] = 0x00; //Данные в байты кадра
  canMsg1.data[7] = 0x00; //Данные в байты кадра
 
  while (!Serial); //Проверка запуска сириал 
  Serial.begin(115200); //Иницилизация сериала
  
  mcp2515.reset(); // сброс настроек MCP2515 так надо, чтобы лучше работал!
  mcp2515.setBitrate(CAN_125KBPS,MCP_8MHZ); //Настройка основных параметров скорости и частота кристала на плате, это важно!!!
  mcp2515.setNormalMode(); //иницилизация и запуск
  
  Serial.println("Example: Write to CAN");
}

void loop() {//выполняется постоянно
  byte data[2];//Работа с датчиком температуры
  ds.reset();
  ds.write(0xCC);
  ds.write(0x44);
  delay(1000);
  ds.reset();
  ds.write(0xCC); 
  ds.write(0xBE);
  data[0] = ds.read();
  data[1] = ds.read();
  float temperature =  ((data[1] << 8) | data[0]) * 0.0625;
  Serial.println(temperature); // вот тут мы ее получили и можем с ней работать
  canMsg1.data[2] = data[0]; //упаковываем значения в кан пакет 
  canMsg1.data[3] = data[1]; //упаковываем значения в кан пакет 
  mcp2515.sendMessage(&canMsg1); //Отправляем сообщение в CAN шину
  Serial.println("Messages sent");
  
  delay(1000);
}
