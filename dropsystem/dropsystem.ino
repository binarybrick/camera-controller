// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// Declare pins
int valvePin = 13 ;
int flashPin = 2 ;

// Setup drop variable array
char* variableLabel[] = {"D1 Delay","D1 Size", "D2 Delay","D2 Size","Flash"};
int variableValue[] = {200,30,300,30,400};  // TODO: Update to good example settings

// Setup menu variables
int currentPage = 0 ;
int currentPageMax = 4 ;  // Number of vars -1 since array index start at 0

void setup() {

  // Setup Debug
  Serial.begin(9600);
  
  // Setup pins
  pinMode (valvePin, OUTPUT);
  pinMode (flashPin, OUTPUT);

}

void loop() {
    
  // Reset button
  int x;
  x = analogRead (0);

  if (x < 100) {
    // Right is pressed

    // Menu handler increase on right-push, ie move to next page
    if(currentPageMax == currentPage){
      currentPage = 0;
    }else{
      currentPage++;
    }
 
    // Debounce
    delay(300);

  }else if (x < 200) {
    // Up is pressed

    // Increase variable value
    variableValue[currentPage] = variableValue[currentPage] + 1;

  }else if (x < 400){
    // Down is pressed

    // Decrease variable value
    variableValue[currentPage] = variableValue[currentPage] - 1;

  }else if (x < 600){
    // Left

  }else if (x < 800){
    // Select is pressed
    
    // Do the drop sequence
    doDropSequence();
    
    // Debounce
    delay(300);
    
  }

  // Print data
  clearLCD();
  lcd.setCursor(0,0);
  lcd.print (variableLabel[currentPage]);
  lcd.setCursor(10,0);
  lcd.print (variableValue[currentPage]);
  delay(60);
}


// Function for reseting the LCD
void clearLCD(){
  lcd.setCursor(0,0);
  lcd.print("              ");
  lcd.setCursor(0,1);
  lcd.print("              ");
  lcd.setCursor(0,0);
}


// The drop sequence
void doDropSequence(){

  Serial.println("----------");
  Serial.println("Running");
  clearLCD();
  lcd.print("DropTime!");

  int passedTime = 0 ;
  int delayTime = 0 ;

  // DropDelay 1
  Serial.println(variableValue[0]);
  delay(variableValue[0]) ;
  passedTime = variableValue[0];  // 300
  digitalWrite(valvePin, HIGH);   //Open valve

  // DropSize 1
  Serial.println(variableValue[1]);
  delay(variableValue[1]) ;
  passedTime += variableValue[1]; // 300 + 30
  digitalWrite(valvePin, LOW);    // Close valve

  // DropDelay 2
  delayTime = variableValue[2] - passedTime ; // 400 - 330 = 70
  Serial.println( delayTime );
  delay( delayTime ) ;
  passedTime += delayTime;  // 330+70
  digitalWrite(valvePin, HIGH);   //Open valve

  // DropSize 2
  Serial.println(variableValue[3]);
  delay(variableValue[3]) ;
  passedTime += variableValue[3]; // 400 + 30 
  digitalWrite(valvePin, LOW);    // Close valve

  // Wait flashdelay then Fire flash!
  delayTime = variableValue[4] - passedTime;  // 500 - 430 = 70
  Serial.println( delayTime );
  delay( delayTime ) ;
  digitalWrite(flashPin, HIGH);     // Turn on flash
  delay(5);
  digitalWrite(flashPin, LOW);      // Turn off flash
  Serial.println("Done");
  Serial.println("--------");
}
