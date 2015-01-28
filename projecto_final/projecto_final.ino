/////LDR
#define Medir 0
#define Apagar 1
#define GerarSinal 2

//Definir Variáveis
float adcLDR;
float voltageLDR;
float adcTERM;
float voltageTERM;
float A;
float B;
float C;
float D;
float I;
float Rfix;
float R25c;
float Rterm;
float Tkel;
float Tc;
int luzes;
int estado;
float ValorPWM;


//Inicialização:
void setup(){
  Serial.begin(9600);
  pinMode(A1, INPUT);
  pinMode(A0, INPUT);
  A=3.354016E-03;
  B=2.884193E-04;
  C=4.118032E-06;
  D=1.786790E-07;
  Rfix=3544;
  R25c=2200;
  estado=Medir;
  luzes=10;
  pinMode(luzes, OUTPUT);
}

 
//Definição das actividades
void VoltimetroLDR(){
  adcLDR=analogRead(A1);
  voltageLDR=adcLDR*5/1024;
  Serial.println(voltageLDR);
}

void Termometro(){
  adcTERM=analogRead(A0);
  voltageTERM=adcTERM*5/1024;
  I=voltageTERM/Rfix;
  Rterm=(5-voltageTERM)/I;
  Tkel=pow(A+B*log(Rterm/R25c)+C*pow(log(Rterm/R25c), 2)+D*pow(log(Rterm/R25c), 3), -1);
  Tc=Tkel-273.15;
  Serial.print(Tc);
  Serial.println(" C");
}

void calcularDuty(){
  ValorPWM=constrain(-404.76*voltageLDR+1874.04,0 , 255);
  Serial.print("Valor PWM=");
  Serial.println(ValorPWM);
}

void anularPWM(){
  ValorPWM=0;
}

void gerarPWM(){
  analogWrite(luzes,ValorPWM);
}

 
void Administrador(){
   switch (estado) {
       case Medir:
         Termometro();
         VoltimetroLDR();
         calcularDuty();
         if(Tc>=30){
           estado=Apagar;
            }else{
            estado=GerarSinal;
          }
          break;
     case Apagar:
          anularPWM();
          estado=GerarSinal;
          break;
      case GerarSinal:
          gerarPWM();
          delay(1000);
          estado=Medir;
          break;
    } // fecho do switch
} // fecho do método

void loop(){
  Administrador();
}

