#include <LiquidCrystal.h> //incluimos la libreria para que funcione el lcd

//pines que estamos utilizando de nuestra pantalla, las conexiones
int rs = 12;
int e = 11;
int d4 = 5;
int d5 = 4;
int d6 = 3;
int d7 = 2;
int pulsador = 10;
int xPin = A1;
int yPin = A0;
int buttonPin = 7;
int buttonVal;
int buzzPin = 8;
int xVal;
int yVal;

//variable global para saber si ha pulsado el boton, si es 0 no ha pulsado, si es 1 o mas pasa para dentro
bool jugadorListo = false;

//variable para que no me imprima todo el rato el mensaje de "estas dentro Empezamos..."
bool mensajeBienvenida = false;

//llamamos a la funcion de la libreria, y asignamos los pines que tenemos conectados
LiquidCrystal lcd(rs, e, d4, d5, d6, d7);



void setup() {
  // inicializamos la pantalla, con la medida 16x2
  lcd.begin(16,2);
  // inicializamos también el pulsador con su pin
  pinMode(10,INPUT);

  //joystick
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
  pinMode(buttonPin, INPUT);
  digitalWrite(buttonPin, HIGH);

  //buzzer, esta vez es auto porque tiene que sonar
  pinMode(buzzPin,OUTPUT);

  Serial.begin(9600);

}

void loop() {

  //controlamos si se ha pulsado el boton
  if(digitalRead(10)==HIGH){      
      jugadorListo = true;
  }

  if(!jugadorListo) {
  // decimos donde vamos a empezar a escribir en la pantalla, la coordenada
    lcd.setCursor(0,0);
    lcd.print("   Bienvenido");
    lcd.setCursor(0,1);
    lcd.print("    Jugador 1");
    delay(1000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Presiona para");
    lcd.setCursor(0,1);
    lcd.print("empezar...");
    delay(1000);
    lcd.clear();

    mensajeBienvenida = false;

  }else{
    if(!mensajeBienvenida){
      lcd.setCursor(0,0);
      lcd.print("Estas dentro!");
      lcd.setCursor(0,1);
      lcd.print(" Empezamos...");
      delay(3000);
      lcd.clear();

      mensajeBienvenida = true;
    }
    // guardamos los valores que recoja el joystick
    xVal = analogRead(xPin);
    yVal = analogRead(yPin);
    buttonVal = digitalRead(buttonPin); //esto es el btn del joystick presionado
    Serial.print("Valor X = ");
    Serial.print(xVal);
    Serial.print(" - Valor Y = ");
    Serial.print(yVal);
    Serial.print(" - Valor Botón = ");
    Serial.println(buttonVal);
    
    //cuando subamos el joystick para arriba, en el eje de Y, el buzzer sonara
    if (yVal < 500){//pongo 500 porque son el rango de valores hacia arriba, si fuese hacia aabajo llegaria hasta 1023
      digitalWrite(buzzPin, 1); 
    }else{
      digitalWrite(buzzPin, 0); 
    }

    //controlamos los movimientos

    if(yVal < 500){
      lcd.setCursor(0,1);
      lcd.print("Saltar");
      delay(100);
      lcd.clear();
    } else if(yVal > 510){
      lcd.setCursor(0,1);
      lcd.print("Agacharse");
      delay(100);
      lcd.clear();
    } else if(xVal < 500){
      lcd.setCursor(0,1);
      lcd.print("Atras");
      delay(100);
      lcd.clear();
    } else if(xVal > 510){
      lcd.setCursor(0,1);
      lcd.print("Adelante");
      delay(100);
      lcd.clear();
    }else{
      lcd.setCursor(0,1);
      lcd.print("Andando...");
      delay(100);
      lcd.clear();
    }

  }

}
