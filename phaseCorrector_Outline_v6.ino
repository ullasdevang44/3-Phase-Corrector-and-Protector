#define R 36                             /* Assign Multiplexer control pin S0 connect to pin D0 of NodeMCU */
#define Y 39                             /* Assign Multiplexer contorl pin S1 connect to pin D1 of NodeMCU */
#define B 34                            /* Assign SIG pin as Analog output for all 3 channels of Multiplexer to pin A0 of NodeMCU */
#define PH 19                          

#define relayNormal 4                       
#define relayReverse 16                       
#define relayBlueLine 13  
#define star 18                      
#define delta 15              
int SDtimer = 0;                         
int SDtimerThreshold = 10;                        

int sensorRValue =3000;
int sensorYValue = 3000;
int sensorBValue = 3000;


int LL = 2000;
int LH = 2300;
int HL = 3300;
int HH =3440;
bool PHASE;
bool ReadPhase[3] ={0};
bool FAULT_R =0;
bool FAULT_Y =0;
bool FAULT_B = 0;
bool FAULT = 0;
void readAmplitudeAverage(){
  int sensorR = 0;
for(int x = 0 ; x < 100 ; x++)
    {
    sensorR = sensorR + analogRead(R);

    delayMicroseconds(100);

    }

sensorRValue = sensorR/100;
delayMicroseconds(10);


int sensorY = 0;
for(int x = 0 ; x < 100 ; x++)
    {
    sensorY = sensorY + analogRead(Y);
    delayMicroseconds(100);
    }
 sensorYValue = sensorY/100;
delayMicroseconds(10);

int sensorB = 0;
for(int x = 0 ; x < 100; x++)
    {
    sensorB = sensorB + analogRead(B);
    delayMicroseconds(100);
    }
sensorBValue = sensorB/100;
delayMicroseconds(10);

}

void verifyThreshold(){

 if ((sensorRValue>=LH) && (sensorRValue<=HL)){    FAULT_R = 0;  }
else 
{if (((sensorRValue>=LL) && (sensorRValue<LH)) ||((sensorRValue>HL) && (sensorRValue<=HH))){}
  else{    FAULT_R = 1;  } 
}
//  ---------------------------------------------
 if ((sensorYValue>=LH) && (sensorYValue<=HL)){FAULT_Y = 0;  }
else 
{if (((sensorYValue>=LL) && (sensorYValue<LH)) ||((sensorYValue>HL) && (sensorYValue<=HH))){}
  else{ FAULT_Y = 1;} 
}
//  ---------------------------------------------
 if ((sensorBValue>=LH) && (sensorBValue<=HL)){FAULT_B = 0;  }
else 
{if (((sensorBValue>=LL) && (sensorBValue<LH)) ||((sensorBValue>HL) && (sensorBValue<=HH))){}
  else{    FAULT_B = 1;  } 
}
//  ---------------------------------------------
FAULT = FAULT_R || FAULT_Y || FAULT_B;
}
 


void shutoffRelays(){
  digitalWrite(relayNormal, LOW);
  digitalWrite(relayReverse, LOW);
  digitalWrite(relayBlueLine, LOW);
  digitalWrite(star, LOW);
  digitalWrite(delta, LOW);

  
 }

void triggerRelays(){
for(int x = 0 ; x < 3 ; x++)
    {
   ReadPhase[x] = digitalRead(PH);
    delayMicroseconds(1000);
    }
if ((ReadPhase[0] == ReadPhase[1]) &&  (ReadPhase[1] == ReadPhase[2])) { 
  PHASE = ReadPhase[0];
  }


  if (PHASE == HIGH){
    digitalWrite(relayNormal, HIGH);
    digitalWrite(relayReverse, LOW);
   digitalWrite(relayBlueLine, HIGH);
  }
  else{
    digitalWrite(relayNormal, LOW);
    digitalWrite(relayReverse, HIGH);
   digitalWrite(relayBlueLine, HIGH);
  }
}


void setup() {
Serial.begin(9600);                    
pinMode(R,INPUT);                     
pinMode(Y,INPUT);                     
pinMode(B,INPUT);                     
pinMode(PH, INPUT);
pinMode(relayNormal,OUTPUT); 
pinMode(relayReverse,OUTPUT);    
pinMode(relayBlueLine,OUTPUT);
pinMode(star,OUTPUT); 
pinMode(delta,OUTPUT);  
  digitalWrite(relayNormal, LOW);
   digitalWrite(relayReverse, LOW);
   digitalWrite(relayBlueLine, LOW);
      digitalWrite(star, LOW);
   digitalWrite(delta, LOW);
}

void loop() {

  readAmplitudeAverage();
  verifyThreshold();

  Serial.print("R value =   ");
  Serial.println(sensorRValue);
  Serial.print("Y value =   ");
  Serial.println(sensorYValue);
  Serial.print("B value =   ");
  Serial.println(sensorBValue);
  Serial.print("Fault =   ");
  Serial.println(FAULT);


  if(FAULT == HIGH){

      shutoffRelays();
          SDtimer = 0;  
    }
  else{
    triggerRelays();
 
    if (SDtimer < SDtimerThreshold) {
      SDtimer++;           
      digitalWrite(star, HIGH);  
      digitalWrite(delta, LOW); 
  
       Serial.println("star"); 
    } 
    else {
      digitalWrite(star, LOW);   
      digitalWrite(delta, HIGH);
        
       Serial.println("delta");
  }
  }
    Serial.print("SDtimer  :" );
    Serial.println(SDtimer);
    Serial.println("PHASE  :");
    Serial.println(PHASE);
    delay(1000);

}
