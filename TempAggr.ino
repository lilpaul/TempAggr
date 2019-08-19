#include <LiquidCrystal.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 13

LiquidCrystal lcd(7, 8, 9, 10, 11 , 12);

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

float Celcius=0;
float Fahrenheit=0;

#define MIN_TEMP -40
#define NUM_DEGS 81
#define DEL_MLLS 10000
#define MINUTE_DIVIDEND 6

long Temperatures[NUM_DEGS];
int currentDisplayIdx = 0;

void setup() {
  sensors.begin();

  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.write("");
  lcd.setCursor(0,1);
  lcd.write("");

  for (int idx = 0; idx < NUM_DEGS; idx++)
  {
    Temperatures[idx] = 0;
  }
}

void loop() {
  //read current temperature
  sensors.requestTemperatures();
  Celcius=sensors.getTempCByIndex(0);
  
  //get floored temperature for aggregation
  int temperature = floor(Celcius);
  int idx = temperature - MIN_TEMP;
  Temperatures[idx] = Temperatures[idx] + 1;

  //display current temperature
  String lineOne = "C: " + String(Celcius);
  int lengthOne = lineOne.length() + 1; 
  char charArrOne[lengthOne];
  lineOne.toCharArray(charArrOne, lengthOne);

  //find next temperature to display
  while(Temperatures[currentDisplayIdx] == 0)
  {
    currentDisplayIdx++;
    //reset if needed
    if (currentDisplayIdx >= NUM_DEGS)
    {
      currentDisplayIdx = 0;
    }
  }
  //calculate the minutes for that temp
  int minutes = ceil(Temperatures[currentDisplayIdx] / MINUTE_DIVIDEND);
  int idxTemp = MIN_TEMP + currentDisplayIdx;

  //display minutes for current idx
  String lineTwo = String(idxTemp) + ": " + String(minutes);
  int lengthTwo = lineTwo.length() + 1; 
  char charArrTwo[lengthTwo];
  lineTwo.toCharArray(charArrTwo, lengthTwo);


  //do actual lcd display work
  lcd.setCursor(0,0);
  lcd.write(charArrOne);
  lcd.setCursor(0,1);
  lcd.write(charArrTwo);

  //delay designated time
  delay(DEL_MLLS);
  
  //move on
  currentDisplayIdx++;
  //reset if needed
  if (currentDisplayIdx >= NUM_DEGS)
  {
    currentDisplayIdx = 0;
  }
}
