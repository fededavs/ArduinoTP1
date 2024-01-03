//Etiquetas
#define temporizado 7
#define disparo 8
#define amarillo 11
#define verde 12
#define rojo 13

/*Declaracion de variables*/
//Banderas control de rutina
bool b=HIGH; // f()->stand_by
bool cd=LOW; // f()->oscilasion_verde
bool pulsador_suelto_disparo=LOW;	// Pulsador suelto despues de ser apretado
bool e=LOW;
bool e_suelto=LOW;
bool f_largo=LOW; //Temporizador 10seg (LoW= 5 seg : HIGH=10 seg)
bool f=LOW;
bool f_estado=LOW; //estado anterior -> f
 
void setup() 
{
  pinMode(temporizado, INPUT);
  pinMode(disparo, INPUT);
  pinMode(amarillo, OUTPUT);
  pinMode(verde, OUTPUT);
  pinMode(rojo, OUTPUT);

  for(int i=0;i<2;i++)
  {
    digitalWrite(verde,HIGH);	//Verde -> Prendido
    digitalWrite(rojo,HIGH);	//Rojo -> Prendido
    delay(500);					//Espero 500 mseg
    digitalWrite(verde,LOW);	//Verde -> Apagado
    digitalWrite(rojo,LOW);		//Rojo -> Apagado
    delay(500);					//Espero 0,5 seg
  }
}

void loop()
{
  stand_by(); //Llama la funcion que hace el punto "B"
  oscilacion_verde();	// Llama f()-> "C";
  retorno_stand_by();
}

//Funcion inciso "C" - "D" -  "F"
void oscilacion_verde()
{
  //PUNTO F -> Modificar el temporizador de 5 a 10 seg
  int tiempo;	//variable del tiempo de temporizador
  f=digitalRead(temporizado); //f= estado del temporizador

  if(f && !f_estado)	//f_estado = almacenar el ultimo estado
  {
    f_largo=!f_largo;	//Cuando se acaba de soltar, f_largo niega su valor anterior como empieza en LOW, al negar pasa en HIGH cuando se aprieta por 1ra vez el temporizador
  }
  if(f_largo)			//cuando f_largo=HIGH -> cada pitido son (1seg OFF - 1 seg ON) x 5 veces da 10 segundos
  {
      digitalWrite(amarillo,HIGH); //Se prende LED amarillo
      tiempo=1000;					// tiempo necesario para los 10 segundos
  }else{							//Caso contrario, queda el delay total de 5 segundos y LED apagado
      digitalWrite(amarillo,LOW);
      tiempo=500;
  }

  f_estado=digitalRead(temporizado);	//guardo el estado actual para la proxima vez que aprieto el temporizador

  //PUNTO C y D
  if(e==0 && digitalRead(disparo)) //Pulsador(izq) ->Presionó	?
  {
    cd=HIGH;	// bandera cd=1
  }

  if(cd && digitalRead(disparo)==0) //CD=1 y (disparo)=0 (soltar boton)
  {
  	for(int i=0;i<5;i++)	//Verde->5 destellos que da en total 5segundos o 10 segundos, segun el temporizado
    {
      digitalWrite(verde,HIGH);	//Verde -> Prendido
      delay(tiempo);					//Variable tiempo= depende si esta en 50 o 100 mseg
      digitalWrite(verde,LOW);	//Verde -> Apagado
      delay(tiempo);					//variable tiempo varia segun el temporizado
    }
    digitalWrite(rojo,HIGH);	//Se apaga verde y enciende rojo
  	cd=LOW;
    e=HIGH;
  }
}

//Funcion inciso "E"
void retorno_stand_by()
{
  if(e && digitalRead(disparo))
  {
    e_suelto=HIGH;
  }
  if(e_suelto && digitalRead(disparo)==0) // !disparo = disparo==0
  {
    b=HIGH;
    e=LOW;
    e_suelto=LOW;
  }
}
