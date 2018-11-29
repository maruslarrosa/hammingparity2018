#include <LiquidCrystal.h>

#define BUTTON_RIGHT 0
#define BUTTON_UP 1
#define BUTTON_DOWN 2
#define BUTTON_LEFT 3
#define BUTTON_SELECT 4
#define BUTTON_NONE 5

/*********** Declaración de variables ********/
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
bool par1, par2, par3;
int data1 = -1, data2 = -1,  data3 = -1,  data4 = -1;
int dataArray[4] = {data1 ,data2 ,data3 ,data4};
bool enableDataEntry = false;
int currentPosition = 0;
int previousButton;


/*********** Prototipo de funciones **********/
int readButtons();
void setNewDisplay();
void increaseByteValue();
void decreaseByteValue();
void printByteValue();
bool isDataComplete();
void calculateParity();
void turnByteLeds();
int getParityValue();
void turnParityLeds();



/**************** VOID / LOOP ***************/
void setup(){
    Serial.begin(9600);
    lcd.begin(16, 2);
    //Initializes all pins turned off
    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);
    pinMode(12, OUTPUT);
    digitalWrite(12, HIGH);
    pinMode(11, OUTPUT);
    digitalWrite(11, HIGH);
    pinMode(10, OUTPUT);
    digitalWrite(10, HIGH);
    pinMode(A3, OUTPUT);
    digitalWrite(A3, HIGH);
    pinMode(A4, OUTPUT);
    digitalWrite(A4, HIGH);
    pinMode(A5, OUTPUT);
    digitalWrite(A5, HIGH);
}

void loop(){
  int currentButton = readButtons();
  if (currentButton == BUTTON_SELECT && !enableDataEntry){
    enableDataEntry = true;
    setNewDisplay();
  }
  if( enableDataEntry ){
    //validar no none y no previous,
    if(currentButton != BUTTON_NONE && currentButton != previousButton){
      switch (currentButton) {
        case BUTTON_UP:
            increaseByteValue();
            break;
        case BUTTON_DOWN:
            decreaseByteValue();
            break;
        case BUTTON_LEFT:
            lcd.setCursor(currentPosition, 1);
            lcd.print(' '); 
            currentPosition--;
            currentPosition = currentPosition == -1 ? 3 : currentPosition;
            lcd.setCursor(currentPosition, 1);
            lcd.print('^');                
            break;
        case BUTTON_RIGHT:
            lcd.setCursor(currentPosition, 1);
            lcd.print(' '); 
            currentPosition++;
            currentPosition = currentPosition == 4 ? 0 : currentPosition;
            lcd.setCursor(currentPosition, 1);
            lcd.print('^');   
            break;
        case BUTTON_SELECT:
            if(isDataComplete()){
              turnByteLeds();
              delay(500);
              calculateParity();
              }
            break;
        }
      previousButton = currentButton;
      }
    }
}


/************** END VOID / LOOP **************/
int readButtons(){
    int value = analogRead(0); // Leemos A0
    // Mis botones dan:  0, 145, 329,507,743
    // Y ahora los comparamos con un margen comodo
    if (value > 900)
        return BUTTON_NONE; // Ningun boton pulsado
    if (value < 50)
        return BUTTON_RIGHT;
    if (value < 250)
        return BUTTON_UP;
    if (value < 450)
        return BUTTON_DOWN;
    if (value < 650)
        return BUTTON_LEFT;
    if (value < 850)
        return BUTTON_SELECT;
    return BUTTON_NONE; // Por si todo falla
}

void setNewDisplay(){
  //initializes a new formatted display
  lcd.setCursor(0, 0);
  lcd.print('*');
  lcd.setCursor(1, 0);
  lcd.print('*');
  lcd.setCursor(2, 0);
  lcd.print('*');
  lcd.setCursor(3, 0);
  lcd.print('*');
  lcd.setCursor(0, 1);
  lcd.print('^');
  };

void increaseByteValue(){
  dataArray[currentPosition] = dataArray[currentPosition] == 1 ? 0 : 1;
  printByteValue();
  }

void decreaseByteValue(){
  dataArray[currentPosition] = dataArray[currentPosition] == 0 ? 1 : 0;
  printByteValue();
  }
  
void printByteValue(){
  lcd.setCursor(currentPosition, 0);
  lcd.print(dataArray[currentPosition]);
  }

bool isDataComplete(){
  int size = sizeof(dataArray) / sizeof(int);
  for(int i = 0; i < size; i++){
    if (dataArray[i] == -1){ return false; }
    return true;
    }
  }
  
void calculateParity(){
  par1 = getParityValue(dataArray[0], dataArray[1], dataArray[3]);
  par2 = getParityValue(dataArray[0], dataArray[2], dataArray[3]);
  par3 = getParityValue(dataArray[1], dataArray[2], dataArray[3]);
  turnParityLeds();
  }

int getParityValue(int a, int b, int c){
  if( !a&&!b&&c || !a&&b&&!c || a&&b&&c || a&&!b&&!c ) return 1;
  return 0;
  };

void turnByteLeds(){
  if(dataArray[0] == 1){ digitalWrite(11, LOW);}
  if(dataArray[1] == 1){ digitalWrite(A3, LOW);}
  if(dataArray[2] == 1){ digitalWrite(A4, LOW);}
  if(dataArray[3] == 1){ digitalWrite(A5, LOW);}
  } 
  
void turnParityLeds(){
  if(par1){
    digitalWrite(13, LOW);
    }
  if(par2){
    digitalWrite(12, LOW);
    }
  if(par3){
    digitalWrite(11, LOW);
    }
  }
  