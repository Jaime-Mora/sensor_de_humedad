#include <LiquidCrystal_I2C.h> //libreria para usar el adaptador de la pantalla LCD mediante el protocolo I2C
#define LED_VERDE     		0x02	// Puerto de conexión LED verde 100% de humedad.
#define LED_AZUL      		0x03	// Puerto de conexión LED azul 75% de humedad.
#define LED_BLANCO    		0x04    // Puerto de conexión LED blanco 50% de humedad.
#define LED_AMARILLO  		0x05    // Puerto de conexión LED amarillo 25% de humedad.
#define LED_ROJO      		0x06    // Puerto de conexión LED rojo 0% de humedad.
#define SOIL      		      A0    // Puerto analógico para el sensor de humedad. 
#define MAX_SOIL_VALUE       1023    // Valor máximo obtenido en la simulación.
LiquidCrystal_I2C lcd(0x20,16,2 );	//se define un objeto de tipo LiquidCrystal_I2C para manejar la pantalla LCD 

byte gota[] = { //simbolo personalizado de una gota utilizando un array
  B00100,
  B01110,
  B11111,
  B11111,
  B11111,
  B01110,
  B00000,
  B00000
};
/* Reemplazo en el código    condicional      se cumple/no se cumple */
#define controlRojo( a ) 	 a <= 12 			? HIGH : LOW  // Si se recibe un valor menor a 12 entonces equivale a un HIGH(alto, 1, verdadero), en caso contrario un LOW(bajo, 0, falso).
#define controlAmarillo( a ) a > 12 && a <= 25  ? HIGH : LOW	// si se recibe un valor mayor a 12 y menor a 25 es verdadero	
#define controlBlanco( a )   a > 25 && a <= 50  ? HIGH : LOW	
#define controlAzul( a )     a > 50 && a <= 75  ? HIGH : LOW
#define controlVerde( a )    a > 75 && a <= 100 ? HIGH : LOW
/*Más sobre el operador condicional: https://learn.microsoft.com/es-es/cpp/cpp/conditional-operator-q?view=msvc-170*/

/* Variable que indica el valor analógico obtenido del sensor */
float Soilmoisture = 0;

void setup( void ) {
  lcd.init(); //función para inicializar la pantalla LCD
  lcd.backlight(); //función para encender la luz de fondo de la pantalla LCD
  Serial.begin ( 115200 );	// Baudrate a 115200 bits/segundo.
  uint8_t LEDS [ ] = { LED_VERDE, LED_AZUL, LED_BLANCO, LED_AMARILLO, LED_ROJO }; // se declara un array cuyo cada miembro contiene el pin digital al cual está conectado su LED correspondiente.
  
  for ( uint8_t i = 0; i < sizeof( LEDS ); i++ ) {	// Se recorre el array; sizeof retorna el número de bytes que necesita en memoria, solo en este caso como cada miembro puede almacenar 8 bits, retorna el tamaño del array.
   
    pinMode ( LEDS [ i ], OUTPUT );					// Se declara cada pin almacenado en el array como salida.
    digitalWrite ( LEDS [ i ], LOW );				// Se inicializa cada pin almacenado en el array en BAJO.
    
  }
  
  pinMode ( SOIL, INPUT );	// El único pin de entrada en este caso es el que pertenece al sensor de humedad del suelo.
  
}

/* Función que retorna el porcentaje de humedad obtenida por el sensor */
float getSoilmoisture ( void ) {
  
	return ( float )(1023 - analogRead( SOIL )) * 100 / MAX_SOIL_VALUE;	
  // Se retorna el valor correspondiente en una regla de 3 para calcular el 100% según la cantidad máxima que proporciona el sensor, en este caso es: 1023;
  //notese que dado los valores de humedad están invertidos para esto se resta 1023 al valor que lee analogRead();
      
}

/* Modo de abreviar la función mediante macros */

void indicator ( uint8_t value )	{
  
  digitalWrite( LED_ROJO, controlRojo( value ) ); //se establece el valor del pin segun las condiciones declaradas antes
  digitalWrite( LED_AMARILLO, controlAmarillo ( value ) );
  digitalWrite( LED_BLANCO, controlBlanco ( value ) );
  digitalWrite( LED_AZUL, controlAzul ( value ) );
  digitalWrite( LED_VERDE, controlVerde ( value ) );  
  
}

void loop( void ) {
  lcd.createChar(0,gota); //funcion para crear el simbolo personalizado
  lcd.setCursor ( 0,0 );	// se muestra en la posición 0,0 de la LCD
  lcd.write(0);			//se imprime el caracter

  Soilmoisture = getSoilmoisture ( );		// Se obttiene el porcentaje de humedad.
 
  lcd.setCursor ( 1,0 ); //se imprime en la posicion 1,0 del LCD
  lcd.print ( Soilmoisture );
  lcd.print ( "% " );
 
  /* Se manda encender los LEDs de acuerdo al nivel de humedad. */
  indicator ( Soilmoisture );
  
  delay ( 100 ); //retraso de 100 ms
  
}