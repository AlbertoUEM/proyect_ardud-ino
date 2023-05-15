#include <LiquidCrystal.h> //incluimos la libreria para que funcione el lcd
#include <EEPROM.h> //incluimos la libreria de la memoria eprom para poder guardar y leer datos en local del chip

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
int contarPosicion = 16;
int randomByte = 0;//caja por defecto es la 0 la de inicio
int velocidad = 500; //velocidad inicial por defecto
int puntuacion = 0; // Inicializamos la variable puntuacion a 0
unsigned long tiempo; // Variable para guardar el tiempo de inicio
int posicionPersonajeX = 0;
int posicionPersonajeY = 0;
int puntuacionGuardada;

//variable global para saber si ha pulsado el boton, si es 0 no ha pulsado, si es 1 o mas pasa para dentro
bool jugadorListo = false;

//variable para que no me imprima todo el rato el mensaje de "estas dentro Empezamos..."
bool mensajeBienvenida = false;

// Música de fin del juego
int notas[] = {262, 294, 330, 349, 392, 440, 494, 523};
int duracion[] = {200, 200, 200, 200, 200, 200, 200, 400};

//llamamos a la funcion de la libreria, y asignamos los pines que tenemos conectados
LiquidCrystal lcd(rs, e, d4, d5, d6, d7);


//dibujamos las animaciones de los obstaculos
byte caja0[] = {
  B00000,
  B00000,
  B00000,
  B11111,
  B11011,
  B10101,
  B11011,
  B11111
};

byte caja1[] = {
  B11111,
  B11011,
  B10101,
  B11011,
  B10101,
  B11011,
  B10101,
  B11111
};

byte caja2[] = {
  B00000,
  B00000,
  B11111,
  B11011,
  B11111,
  B11111,
  B11011,
  B11111
};
//dibujamos las distintas animaciones de los personajes:
byte andar[] = {
  B01000,
  B11111,
  B11111,
  B01100,
  B01111,
  B01100,
  B01100,
  B10100
};

byte saltar[] = {
  B01000,
  B11111,
  B11111,
  B01100,
  B11110,
  B01000,
  B10100,
  B00000
};

byte agacharse[] = {
  B00000,
  B00000,
  B01000,
  B11111,
  B11111,
  B01100,
  B11110,
  B01100
};

byte puntos[] = {
  B01010,
  B00100,
  B10101,
  B01110,
  B10101,
  B00100,
  B01010,
  B00000
};
//-- fin de las animaciones --


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
  //EEPROM.write(0,25);

  //generamos los obstaculos, llamandolos
  lcd.createChar(0, caja0);
  lcd.createChar(1, caja1);
  lcd.createChar(2, caja2);
  lcd.createChar(3, andar);
  lcd.createChar(4, saltar);
  lcd.createChar(5, agacharse);
  lcd.createChar(6, puntos);
  // Guardamos el tiempo de inicio
  tiempo = millis();
}

void loop() {

  //controlamos si se ha pulsado el boton
  if(digitalRead(10)==HIGH){      
      jugadorListo = true;
  }

  if(!jugadorListo) {

    puntuacionGuardada = EEPROM.read(0); //leemos el valor almacenado en la dirección de memoria 0

  // decimos donde vamos a empezar a escribir en la pantalla, la coordenada
    lcd.setCursor(0,0);
    lcd.print("   Bienvenido");
    lcd.setCursor(0,1);
    lcd.print("    Jugador 1");
    delay(1000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Puntuacion mas");
    lcd.setCursor(0,1);
    lcd.print("alta: ");
    lcd.print(puntuacionGuardada);
    delay(2000);
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
      musicaInicio(); //llamamos al metodo para que haga sonar la música de inicio
      delay(1500);
      lcd.clear();

      mensajeBienvenida = true;
      puntuacion = 0;
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
    // Medimos el tiempo que ha pasado y actualizamos la puntuación
      unsigned long tiempoActual = millis();
      unsigned long tiempoTranscurrido = tiempoActual - tiempo;
      puntuacion = tiempoTranscurrido / 500; // Cada 1000 ms suma 1 punto

    //vamos pintando la puntuación obtenida:
      if(puntuacion < 10){ //si tiene menos de 2 dígitos
        lcd.setCursor(14, 0);//posición donde me pinta la moneda
        lcd.write(byte(6)); //me pinta la moneda, un cuadradito a la izquierda siempre de la puntuación
        lcd.setCursor(15, 0);//posicion donde me muestra la puntuación
        lcd.print(puntuacion);//que variable es la que me imprime, en este caso donde se guarda la puntuación
      }else if(puntuacion < 100){   //si tiene menos de 3 dígitos
        lcd.setCursor(13, 0);
        lcd.write(byte(6));
        lcd.setCursor(14, 0);
        lcd.print(puntuacion);
      }else if(puntuacion < 1000){//si tiene menos de 4 dígitos
        lcd.setCursor(12, 0);
        lcd.write(byte(6));
        lcd.setCursor(13, 0);
        lcd.print(puntuacion);
      }else if(puntuacion < 10000){//si tiene menos de 5 dígitos
        lcd.setCursor(11, 0);
        lcd.write(byte(6));
        lcd.setCursor(12, 0);
        lcd.print(puntuacion);
      }
      
    //generamos la caja
    if(contarPosicion >=0){
      lcd.setCursor(contarPosicion, 1);//vamos pintado la resta de la posicion de la caja
      lcd.write(byte(randomByte));//generamos una caja aleatoria, se establece aleatoriamente abajo, por defecto es la 0
      delay(velocidad);//va a la velocidad que se ha establecido aleatoriamente más abajo, por defecto es 500
      lcd.clear();
      contarPosicion--;
    }else{
      contarPosicion = 16; //vuelve a generar la posicion inicial
      randomByte = random(3); //una vez llega al final la caja coge aleatoriamente otro tipo
      velocidad = random(500 - 100) + 100; //variamos la velocidad cuando llega al final
    }

    //controlamos los movimientos y vamos actualizando las posiciones de los personajes

    if(yVal < 500){
      // Pintamos al muñeco saltando
      lcd.setCursor(0, 0);
      lcd.write(byte(4));
      delay(100);
      lcd.clear();
      posicionPersonajeX = 1;
      //trazas para saber la posicion del jugador
      //lcd.setCursor(0, 0);
      //lcd.print(posicionPersonajeX);
    } else if(yVal > 510){
      // Pintamos al muñeco agachado
      lcd.setCursor(0, 1);
      lcd.write(byte(5));
      delay(100);
      lcd.clear();
       posicionPersonajeX = 0;
      //trazas para saber la posicion del jugador
      //lcd.setCursor(0, 0);
      //lcd.print(posicionPersonajeX);

        //comprobamos si la posicion del personaje es la misma que la caja, de ser asi se terminaria el juego
        if (contarPosicion == posicionPersonajeX){
          // terminamos el juego
          //limpiarmos la pantalla entera para mostrar el mensaje final
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(" Fin del Juego");
        lcd.setCursor(0,1);
        lcd.print("Puntos: ");
        lcd.print(puntuacion);//imprimimos la puntuación
        musicaFin();//llamamos al metodo para que haga sonar la música de fin
        guardarPuntuacion(puntuacion);
        delay(4000);//esperamos 4 segundos y reseteamos todas las variables de inicio
        jugadorListo = false;
        velocidad = 500;
        puntuacion == 0;
        tiempo = millis();
        lcd.clear();
        }

    } else if(xVal < 500){//moverse hacia atras
      lcd.setCursor(0,1);
      lcd.print("Atras");
      delay(100);
      lcd.clear();
    } else if(xVal > 510){//moverse hacia adelante
      lcd.setCursor(0,1);
      lcd.print("Adelante");
      delay(100);
      lcd.clear();
    }else{
      // Pintamos al muñeco antando
      lcd.setCursor(0, 1);
      lcd.write(byte(3));
      delay(100);
      lcd.clear();
      posicionPersonajeX = 0;
      //trazas para saber la posicion del jugador
      //lcd.setCursor(0, 0);
      //lcd.print(posicionPersonajeX);

        //comprobamos si la posicion del personaje es la misma que la caja, de ser asi se terminaria el juego
        if (contarPosicion == posicionPersonajeX){
          // terminamos el juego
          //limpiarmos la pantalla entera para mostrar el mensaje final
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(" Fin del Juego");
        lcd.setCursor(0,1);
        lcd.print("Puntos: ");
        lcd.print(puntuacion);//imprimimos la puntuación
        musicaFin();//llamamos al metodo para que haga sonar la música de fin
        guardarPuntuacion(puntuacion);
        delay(4000);//esperamos 4 segundos y reseteamos todas las variables de inicio
        jugadorListo = false;
        velocidad = 500;
        puntuacion == 0;
        tiempo = millis();
        lcd.clear();
        }
    }
    

  }

}

void musicaInicio() {
  for (int i = 0; i < 8; i++) {
    tone(8, notas[i], duracion[i]);
    delay(100);
  }
}

void musicaFin() {
    for (int i = 0; i < 8; i++) {
          tone(buzzPin, notas[i], duracion[i]);
          delay(duracion[i] * 1.30);
          noTone(buzzPin);
          delay(50);
    }
  
}

void guardarPuntuacion(int puntuacion) {
  //comprobamos la puntuacion mas alta que habia guardada, si es superior entra en el if y la actualiza.
  int puntuacionMasAlta = EEPROM.read(0); //seteamos a esta variable la antigua puntuación guardada para luego compararla

  if(puntuacion > puntuacionMasAlta){
    EEPROM.write(0, puntuacion); //en el caso de que la nueva puntuación obtenida sea mayor a la guardada, la actualizamos en el espacio de memoria 0
  }

  
}

  
