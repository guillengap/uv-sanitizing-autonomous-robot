// AUTHOR: GUILLERMO PEREZ GUILLEN
// Project: TESLA Robot

#define ENA 3
#define ENB 5
#define IN1 8
#define IN2 9
#define IN3 10
#define IN4 11

/******************************************************************
   NETWORK CONFIGURATION
******************************************************************/
const int ESP32_pin_1= 6; // ESP32 input pin 1 - starting
const int ESP32_pin_2 = 7; // ESP32 input pin 2 - SRF04
const int ESP32_pin_3 = 12; // ESP32 input pin 3 - SRF05
const int InputNodes = 7; // includes BIAS neuron
const int HiddenNodes = 4; //includes BIAS neuron
const int OutputNodes = 4;
int i, j;
double Accum;
double Hidden[HiddenNodes];
double Output[OutputNodes];
float HiddenWeights[7][4] = {{-4.618963658666277, 4.3001137618883325, 7.338055706191847, 2.7355309007172375}, {2.599633307446623, -7.649705724376986, -14.69443684121685, -3.65366992422193}, {-0.7777191662679982, 1.9860139431844053, 5.914809078303235, 0.03170277380327093}, {-2.309653145069323, 6.8379997039119775, 8.892299055796917, 0.6046238076393062}, {1.3276547120093833, 5.085574619860947, 2.384944264717347, 0.05753178068519734}, {-2.7696264005599858, 6.797226565794283, 3.5374247269984713, 0.5475825968169957}, {0.8118152131237218, -1.9324229493484606, -5.264294920291424, -0.036800281071245555}};
float OutputWeights[4][4] = {{-1.6342640637903814, 0.006920937706630823, -5.179205882976105, -0.40268984302793936}, {-1.0162353344988182, 1.3405072244655225, -4.241619375014734, 0.6682851389512594}, {1.3692632942485174, -1.3884291338648505, -0.9245235380688354, 2.246128813012694}, {-1.9802299382328057, 0.06512857708456388, -0.030302930346753857, -3.314024844617794}};

int error=0;
int dif,difAnt=0;
const float Kp=0.1;
const float Kd=0.1;

void setup() {	
	Serial.begin(9600);
	pinMode(A0, INPUT); //left sensor
	pinMode(A1, INPUT); //center sensor
	pinMode(A3, INPUT); //right sensor 
	pinMode(IN1, OUTPUT);
	pinMode(IN2, OUTPUT);
	pinMode(IN3, OUTPUT);
	pinMode(IN4, OUTPUT);
	pinMode(ENA, OUTPUT);
	pinMode(ENB, OUTPUT);
  pinMode(ESP32_pin_1, INPUT); 
  pinMode(ESP32_pin_2, INPUT);
  pinMode(ESP32_pin_3, INPUT);  
} 

void loop()
{

double TestInput[] = {0, 0, 0};
double input1=0,input2=0,input3=0,input4=0,input5=0,input6=0;
    
float volts0 =  analogRead(A0)*0.0048828125;  // value from sensor * (5/1024) 
float volts1 =  analogRead(A1)*0.0048828125;  // value from sensor * (5/1024) 
float volts2 =  analogRead(A3)*0.0048828125;  // value from sensor * (5/1024)

dif = analogRead(A3) - analogRead(A0);	// PID CONTROLLER
error = floor(Kp*(dif)+Kd*(difAnt-dif));	// PID CONTROLLER
difAnt=dif;	// PID CONTROLLER
int d0 = constrain(150 - error, 0, 150);//left speed - PID CONTROLLER
int d1 = constrain(150 + error, 0, 150);//right speed - PID CONTROLLER

float ir_sensor_left =  6*pow(volts0, -1); // worked out from datasheet graph //GP2Y0A51SK0F - 2 a 15 cm
float ir_sensor_center = 12.4*pow(volts1, -1); // worked out from datasheet graph //GP2Y0A41SK0F - 4 a 30 cm
float ir_sensor_right = 5.2*pow(volts2, -1); // worked out from datasheet graph //GP2Y0A51SK0F - 2 a 15 cm

if(digitalRead(ESP32_pin_1) == HIGH){input1=1;} // START TO MOVE
else {input1=0;} // STOP

if (ir_sensor_left<15){input2=1;} // IR SENSOR LEFT
else {input2=0;}

if(digitalRead(ESP32_pin_2) == HIGH){input3=1;} // RF SENSOR LEFT
else {input3=0;}
	
if (ir_sensor_center<30){input4=1;}  // IR SENSOR CENTER
else {input4=0;}

if(digitalRead(ESP32_pin_3) == HIGH){input5=1;} // RF SENSOR RIGHT
else {input5=0;}

if (ir_sensor_right<15){input6=1;} // IR SENSOR RIGHT
else {input6=0;}

/******************************************************************
    WE CALL THE FEEDFORWARD NETWORK WITH THE INPUTS
******************************************************************/

  Serial.print("Input1:");
  Serial.println(input1);
  Serial.print("Input2:");
  Serial.println(input2);
  Serial.print("Input3:");
  Serial.println(input3);  
  Serial.print("Input4:");
  Serial.println(input4);
  Serial.print("Input5:");
  Serial.println(input5);
  Serial.print("Input6:");
  Serial.println(input6); 
  Serial.println("   ");   
  
//THESE ARE THE THREE INPUTS WITH VALUES OF 0 TO 1 ********************
  TestInput[0] = 1.0;//BIAS UNIT
  TestInput[1] = input1;
  TestInput[2] = input2;
  TestInput[3] = input3;  
  TestInput[4] = input4;
  TestInput[5] = input5;
  TestInput[6] = input6;  

// THIS FUNCTION IS TO GET THE OUTPUTS **********************************
  InputToOutput(TestInput[0], TestInput[1], TestInput[2], TestInput[3], TestInput[4], TestInput[5], TestInput[6]); //INPUT to ANN to obtain OUTPUT

  int out1 = round(abs(Output[0]));
  int out2 = round(abs(Output[1]));
  int out3 = round(abs(Output[2]));
  int out4 = round(abs(Output[3]));
  Serial.print("Output1:");
  Serial.println(out1);
  Serial.print("Output2:");
  Serial.println(out2);
  Serial.print("Output3:");
  Serial.println(out3);
  Serial.print("Output4:");
  Serial.println(out4);  
  Serial.println("   ");

/******************************************************************
    DRIVE MOTORS WITH THE NETWORK OUTPUT
******************************************************************/
  analogWrite(ENA, d0);
  analogWrite(ENB, d1);
  digitalWrite(IN1, out1 * HIGH); 
  digitalWrite(IN2, out2 * HIGH); 
  digitalWrite(IN3, out3 * HIGH);
  digitalWrite(IN4, out4 * HIGH);
  delay(20);
}

void InputToOutput(double In1, double In2, double In3, double In4, double In5, double In6, double In7)
{
  double TestInput[] = {0, 0, 0, 0, 0, 0, 0};
  TestInput[0] = In1;
  TestInput[1] = In2;
  TestInput[2] = In3;
  TestInput[3] = In4;
  TestInput[4] = In5;
  TestInput[5] = In6;
  TestInput[6] = In7;    

/******************************************************************
    CALCULATE ACTIVITIES IN HIDDEN LAYERS
******************************************************************/

  for ( i = 0 ; i < HiddenNodes ; i++ ) {	// We go through the four columns of the hidden weights
    Accum = 0;
    for ( j = 0 ; j < InputNodes ; j++ ) {	// Three values of the entry line and each column of hidden weights
      Accum += TestInput[j] * HiddenWeights[j][i] ;
    }
    Hidden[i] = tanh(Accum) ; // We obtain a matrix of a line with four values
  }

/******************************************************************
    CALCULATE ACTIVATION AND ERROR IN THE OUTPUT LAYER
******************************************************************/

  for ( i = 0 ; i < OutputNodes ; i++ ) {
    Accum = 0;
    for ( j = 0 ; j < HiddenNodes ; j++ ) {
        Accum += Hidden[j] * OutputWeights[j][i] ;
    }
    Output[i] = tanh(Accum) ;//tanh
  }
}
