#include "Arduino.h"
int PWM_PIN = 6; // pin for mosfet
int irsensor = 3;  // pin for IR-sensor
int trigger = 0; // Used to pick which equations to use
float t = millis();
float RPM = 0;
int newInput = 0;
int oldInput = 0; // See later, used to measure the RPM
float irvalueChange = 0;// ir sensor
unsigned long Time = 0; // Used for the timer
unsigned long lastTime = 0; // Time it takes for the previous RPM and current to be measured
//int t = 0; // Incrememnt values later on to control the flow of the code
float speeds[] = {110, 110,100, 100,95,95, 90, 90, 85,85, 80 ,80, 75, 75, 70,70, 60,60}; // List with all the different fractions of the voltage input into the motor from the PWM
int s = 0; // Index used for the speeds[] list
int speed0 = 255; //fraction of voltage used at that time
float RPMs[] = {};
long viscosity[] = {};
float allCurrents[] = {};
float shearStress[] = {};
float shearRate[] = {};
float M_Pi = 3.1415;
float averageCurrent= 0;
unsigned long timeElapsed = 0;
 // declared lists for RPMs, currents and result viscosity

 

#include "EasyNextionLibrary.h" 
#include "Nextion.h"
#include "LinearRegression.h"

 

EasyNex myNex(Serial);
// Include EasyNextionLibrary
void setup() {
  // put your setup code here, to run once:1
  Serial.begin(9600);
 

 

  myNex.begin(9600); // Begin the object with a baud rate of 9600
                     // If no parameter was given in the begin(), the default baud rate of 9600 will be used

 

  delay(500);               // give Nextion some time to finish initialize
  //myNex.writeStr("page 0"); // For synchronizing Nextion page in case of reset to Arduino
  delay(50);
   // At the first run of the loop, the currentPageId and the lastCurrentPageId
                              
}

 
void graphs(long lists[]){
long lowVal = lists[0];
long prevVal = 0.0;
//float highVal = 0.0;
for (int c1 = 0; c1<18;c1 = c1 + 2){
prevVal = lists[c1];
if (prevVal < lowVal){
lowVal = prevVal;
}
}
long highVal = lists[0];
prevVal = 0.0;
for (int c1 = 0; c1<18;c1 = c1 + 2){
prevVal = lists[c1];
if (prevVal > highVal){
highVal = prevVal;
}
} 
long displayValueStorage[] ={};
Serial.println("High val= "+ String(highVal));
Serial.println("Low val= " + String(lowVal));

for (int count = 0; count<18; count=count+2){
// sends values of viscosity to graph on nextion
long divVal = lists[count]/10000;
Serial.println("Division values:");
Serial.println(divVal);
long tempStorage = (255*divVal);
Serial.println("Multiplied value");
Serial.println(tempStorage);

displayValueStorage[count/2] = (tempStorage/highVal);
Serial.println("Divided value");
Serial.println(displayValueStorage[count/2]);
//map((int)lists[count],lowVal,highVal,0,255);////(255*(lists[count]))/(highVal-lowVal);
}
String dis = "";
//dis.concat(displayVal);
Serial.println("List of storage values");
for (int i = 0; i <18; i = i + 2){
  Serial.println(displayValueStorage[i]);
}
Serial.println("Original list");
for (int i = 0; i <18; i = i + 2){
  Serial.println(lists[i]);
}
for (int i = 0; i < 18; i = i + 2){
  
  myNex.writeNum("va0.val",displayValueStorage[i]);
  delay(500);
}
//Serial.println("displayVal = " + dis);
//myNex.writeNum("va0.val",displayVal);

//displayVal = map(lists[count],lowVal,highVal,0,255);
//Serial.println("displayVal = "+ String(displayVal));
//myNex.writeNum("va0.val",displayVal);
//delay(1000);// updates values of waveform
//Serial.write(0xff);
//Serial.write(0xff); // these three lines needs to be written when updating the nextion
//Serial.write(0xff);
/*Serial.println("High val= "+String(highVal,3));
Serial.println("Low val= " + String(lowVal,3));
for (int count = 0; count<18; count=count+2){
// sends values of viscosity to graph on nextion
int displayVal = 0;
displayVal = map(lists[count],lowVal,highVal,0,255);
Serial.println("displayVal = "+ String(displayVal));
myNex.writeNum("va0.val",displayVal);
delay(1000);// updates values of waveform
//Serial.write(0xff);
//Serial.write(0xff); // these three lines needs to be written when updating the nextion
//Serial.write(0xff);
*/
}

  

 
float linearRegression (float xarray[18], float yarray[18]){
// Define Objects
// R^2 value calculation
int i;
//float xarray[18] = list1; // = {1,7,13,19,50,70,82,90};
//float yarray[18] = list2 ; // = {1,9,28,57,340,760,1500,2600};
int n = sizeof( xarray ) / sizeof( xarray[0] );
float xyarray [] = {};
float sumxyarray = 0;
float sumxarray = 0;
float sumyarray = 0;
float xxarray [] = {};
float yyarray [] = {};
float sumxxarray = 0;
float sumyyarray = 0;
float len = 8;
for(i = 0; i < len; i ++) {
xyarray[i] = xarray[i] * yarray[i];
sumxyarray = sumxyarray + xyarray[i];
}
for (i = 0; i < len; i++) {
sumxarray = sumxarray + xarray[i];
}
for (i = 0; i < len; i++) {
sumyarray = sumyarray + yarray[i];
}
for(i = 0; i < len; i=i+1) {
xxarray[i] = xarray[i] * xarray[i];
sumxxarray = sumxxarray + xxarray[i];
}
for(i = 0; i < len; i++) {
yyarray[i] = yarray[i] * yarray[i];
sumyyarray = sumyyarray + yyarray[i];
}
//formula
float R = ( ((n * sumxyarray) - (sumxarray * sumyarray)) / (sqrt( ( ( n* sumxxarray) - ( sumxarray * sumxarray ) ) * ( ( n* sumyyarray) - ( sumyarray * sumyarray )))));
float Rsquared = R * R;
// understanding if R or LR is better
if ( Rsquared > 0.8) {
// Set Data
// calculation of the gradient
float gradient1;
float gradient2;
float gradient3;
float gradient4;
float gradient5;
float gradient6;
float gradient7;
gradient1 = (yarray[1]-yarray[0])/(xarray[1]-xarray[0]);
gradient2 = (yarray[2]-yarray[0])/(xarray[2]-xarray[0]);
gradient3 = (yarray[3]-yarray[0])/(xarray[3]-xarray[0]);
gradient4 = (yarray[4]-yarray[0])/(xarray[4]-xarray[0]);
gradient5 = (yarray[5]-yarray[0])/(xarray[5]-xarray[0]);
gradient6 = (yarray[6]-yarray[0])/(xarray[6]-xarray[0]);
gradient7 = (yarray[7]-yarray[0])/(xarray[7]-xarray[0]);
float average = (gradient1 + gradient2 + gradient3 + gradient4 + gradient5 + gradient6 + gradient7) / 7;
float yintercept = yarray[0] - average;
//Serial.print(lr.Samples()); 
//Serial.println(" Point Linear Regression Calculation...");
//Serial.print("Values: ");
//Serial.print("Y = ");
Serial.println(average);
String averageStr = "";
averageStr.concat(average);
myNex.writeStr("t0.txt",averageStr);
myNex.writeStr("t1.txt","Newtonian");
//Serial.print("*X + ");
//Serial.println(yintercept);
delay(5000);
//return lr.Data(yarray[i]);
//name of the variable for the data collected
}
else {
float gradient;
float gradients[] = {};
float changex;
float changey;
int countincrease;
int countdecrease;
for (int i=1; i <= 7; i++) {
changex = (xarray[i] - xarray[i-1]);
changey = (yarray[i] - yarray[i-1]);
gradient = changey/changex;
gradients[i] = gradient;
}
countincrease = 0;
countdecrease = 0;
for(int i = 1; i <= 7; i++){
if (gradients[i-1]>gradients[i]){
countincrease ++;
}
else{
countdecrease ++;
}
}
if(countincrease>countdecrease){
/*Serial.print("n0.txt = Shearthickening Material");
Serial.write(0xff);
Serial.write(0xff);
Serial.write(0xff);*/
myNex.writeStr("t1.txt","shearthn");
Serial.println("shearthickening");
}
if (countdecrease>countincrease){
myNex.writeStr("t1.txt","shearthc");
Serial.println("Shearthinning Material");
//Serial.write(0xff);
//Serial.write(0xff);
//Serial.write(0xff);
//Serial.println("shearthickening");
}
/*float dummyArray[18];
for (int x = 0; x < 18; x++){
dummyArray[x] = 0.0;
}
//return dummyArray[];*/
}
}


void trigger4(){// stop buttons
trigger=4;}
void trigger1(){ // Parallel plate 
  trigger = 1;}
void trigger2( ) {// Cone and plate
  trigger = 2;}
void trigger3() {// coaxial cylinder
   trigger = 3;}     // Nextion triggers functions to chose the mechanisms

 

   
void(* resetFunc) (void) = 0; // declare reset function at address 0
void loop() {
  Serial.println("Start");
 if (trigger == 0){
  analogWrite(PWM_PIN,0);
  myNex.NextionListen();

}
  
  unsigned long currentMillis = millis();
  unsigned long previousMillis = millis();
  //(unsigned long)(currentMillis - previousMillis);
  myNex.NextionListen();
  analogWrite(PWM_PIN,0);
if (trigger != 0){
  if (trigger == 4){
    resetFunc();
  }
  for (int i = 0; i<18 ; i++){
    myNex.NextionListen();
    float currentSpeed = speeds[i];
    s += 1 ;
    Serial.println(currentSpeed);

 

    for (;timeElapsed<1000;){
      myNex.NextionListen();
        if (trigger == 4){
    resetFunc();}
      analogWrite(PWM_PIN,speeds[i]);
      timeElapsed = (unsigned long)(currentMillis - previousMillis);
      currentMillis = millis();
      }
    for (;timeElapsed<5000;){
      myNex.NextionListen();
        if (trigger == 4){
        resetFunc();}
      Serial.println(trigger);
      // read ir and then rpm
      //Serial.println(1739046379);  
      newInput = digitalRead(irsensor); 
      if (oldInput != newInput) {  
          irvalueChange = irvalueChange + 1; 
          }
          
      oldInput = newInput;
      RPM = irvalueChange*30000/17000; // rpm 
      averageCurrent = averageCurrent + (.0264 * analogRead(A0) -13.51) / 17000; // average current measured over 12s

 

      RPMs[i] = RPM;
      allCurrents[i] = averageCurrent; // puts the RPM and average in the lists 

 

      
      timeElapsed = (unsigned long)(currentMillis - previousMillis);
      currentMillis = millis();
      
    }
    timeElapsed = (unsigned long)(currentMillis - previousMillis);
    previousMillis = millis();
    currentMillis = millis();
      
    }
    
    if (trigger == 1){ 
     myNex.NextionListen();
  analogWrite(PWM_PIN,0);
    for(int p = 0; p < 8 ; p++)
{
    Serial.println(p);
    float PlateRadius = 0.025; // convert from mm
    float Gap = 0.005 ;// convert from mm gap between plates max is 5mm 
    float E = 1;
    float V = (speeds[p]/255)*9; // PWM voltage
    float I = allCurrents[p]; 
    float RPMc = RPMs[p]; //sets the variables for the calculations
    float torque = 0.0;
    
    
    if (RPMc != 0.0){
    torque = (I*V * E *60)/(RPMc *2*M_PI);
    float M = torque * 2* M_PI ; // torque * geometry 
    float Omega = RPMc * 0.10472; // first value is rpm but second converts to 
    //to rad/s , rpm input from ir sensor since its real time rotation
    shearRate[p] = (PlateRadius*Omega)/Gap;
    shearStress[p] = (2*M)/(M_PI*pow(PlateRadius,3));
    viscosity[p] = shearStress[p]/shearRate[p];
    }
     else {
    viscosity[p] = 0;//{90,90, 100, 100,95, 95,97,97,93,93,85,85,105,105,95,95,95,95};
    shearRate[p] = 0.0;
    shearStress[p] = 0.0;
    Serial.println("tttviscosity = " + String(viscosity[p]));
    }
    
}

    }
    if (trigger == 2){
    for(int p = 0; p < 8 ; p++)
{
    float PlateRadius = 0.025;
    float E = 1;
    float V = (speeds[p]/255)*9;
    float I = allCurrents[p];
    float RPMc = RPMs[p];
    float alpha = 4*M_Pi/180;
    float torque = (I*V * E *60)/(RPMc *2*M_PI);
    float omega = RPMc * 0.10472; 
    float M = 60*torque/2*PI*omega;// calculate torque  
    shearStress[p] = 3*M/(2*PI*(pow(PlateRadius,3))) ;// calculate shear stress
    shearRate[p] = omega/alpha ;
    viscosity[p] = shearStress[p]/shearRate[p];
}

    }
    if (trigger == 3){
    for(int p = 0; p < 8 ; p++)
{
    float E = 1;
    float Ri = 10.0;//Radius of inner cylinder 
    float Ro = 12.0;//Radius of outer cylinder 
    float L = 10.0;//Height of the Bob 
    float V = (speeds[p]/255)*9;
    float I = allCurrents[p];
    float RPMc = RPMs[p];
    float torque = (I*V * E *60)/(RPMc *2*M_PI);
    float M = torque * 2* M_PI ; // torque * geometry
// take rpm vaue
    float omega = RPMc * 0.10472; 
// Calculations:
    float rr = log(M)/log(omega);//it's in the equation dlnM/dlnOmega 
    shearRate[p] = (2*omega)/(rr*(1-pow((Ri/Ro),(2/rr))));
    shearStress[p] = M/(2*M_Pi*pow(Ri,2)*L);
    viscosity[p] = shearStress[p]/shearRate[p];
    
} 


}
for (int i = 0; i<18; i = i + 2){
  Serial.print("Viscosity = ");
  Serial.println(viscosity[i]);
}
linearRegression(shearStress, shearRate);
graphs(viscosity);

 while(1){
  }

}



//Serial.print(trigger);

 

}// How much time has passed 
