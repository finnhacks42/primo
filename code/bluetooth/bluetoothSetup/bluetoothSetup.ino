
//at+init\r\n ---- Initialize the SPP profile lib( can’t repeat initialization) 
//     -> OK 

//at+iac=9e8b33\r\n ----Inquire Bluetooth device has an access code 
//     -> OK 

//at+class=0\r\n ----Inquire the Bluetooth device type 

//at+inqm=1,9,48\r\n 

//At+inq\r\n ----inquire the Bluetooth device around 

// This script is used to configure the bluetooth modules to automatically pair to one-another.
// On of the modules must be set up in slave mode, the other in master mode. 
// Run this script to set up the slave module first and record the address of the module.
// Then swap out the slave module for the master module, set role to MASTER and bindMaster to include the address of the slave module.

#define SLAVE 1
#define MASTER 2

// PARAMETERS TO EDIT
//------------------------------------
// When you are setting up the master, edit this to the address of your slave.
// NOTE: replace the : in the address with ,
#define ADDRESS "3014,11,172028"
#define ROLE MASTER //SLAVE or MASTER
#define ID '5' // If you are setting up multiple robot/board sets increment this for each set so that the names of the chips are not all the same
//------------------------------------

#define baudRate 38400 //The default baud rate for HC-05 chips in AT mode.
#define MAX_RESPONSE_SIZE 20
#define OK 0
#define ERROR_RESPONSE 1
#define TIMEOUT 5000 //how long will we wait to get a response from the HC-05


char pair[] = "PAIR=" ADDRESS ",9";
char bind[] = "BIND=" ADDRESS;
char link[] = "LINK=" ADDRESS;

char c=' ';
boolean NL = true;
char response[MAX_RESPONSE_SIZE];

char restorDefault[] = "ORGL";
char nameQuery[] = "NAME?";
char versionQuery[] = "VERSION?";
char addressQuery[] = "ADDR?";
char setPassword[] = "PSWD=1234";
char setCommunicationSpeed[] = "UART=9600,0,0";
char resetModule[] = "RESET";
char clearPaired[] = "RMAAD";
char cmode[] = "CMODE=0";
char initModule[] = "INIT";
char setToSlave[] = "ROLE=0";
char bindSlave[] = "BIND=";
char setSlaveName[] = "NAME=KINDERBOTX";

char setToMaster[] = "ROLE=1";
char setMasterName[] = "NAME=KINDERBOARDX";


char* slaveCommands[] = {restorDefault,nameQuery,versionQuery,setPassword,setCommunicationSpeed,cmode,
                            setToSlave,setSlaveName,bindSlave,addressQuery};
char* masterCommands[] = {restorDefault,nameQuery,versionQuery,setPassword,setCommunicationSpeed,cmode,
                            setMasterName,setToMaster,resetModule,clearPaired,initModule,pair,bind,link};

int executeCommands(char** commands,int numCommands){
  int result = OK;
  for (int i = 0; i < numCommands; i++){
     result = executeAT(commands[i]);
     if (result != OK){
        return result;
     }
  }
  return OK;
}

void setup() 
{
   //
   // This function sets up the bluetooth by sending AT commands 
   // and monitoring the response to check the commands executed successfully. 
    Serial.begin(9600);
    Serial1.begin(baudRate);  
    while (!Serial){;} //wait for Serial to be ready
    Serial.print("Sketch:   ");   Serial.println(__FILE__);
    Serial.print("Uploaded: ");   Serial.println(__DATE__);
    Serial.println(" ");
    Serial.println("");
    Serial.print("BTserial started at "); Serial.println(baudRate);
    Serial.println("5 seconds till bluetooth configuration");
    delay(5000);
    Serial.println("starting configuration");
    
    int result = OK;
    if (ROLE == SLAVE){   
      setSlaveName[sizeof(setSlaveName) - 2] = ID;
      int numSlaveCommands = sizeof(slaveCommands)/sizeof(slaveCommands[0]);
      result = executeCommands(slaveCommands,numSlaveCommands);
    } else if (ROLE == MASTER){
       setMasterName[sizeof(setMasterName) - 2] = ID;
       int numMasterCommands = sizeof(masterCommands)/sizeof(masterCommands[0]);
       result = executeCommands(masterCommands,numMasterCommands);
    }

    if (result == OK){
      Serial.println("Bluetooth module setup sucessfully!");
    } else {
      Serial.println("Error configuring Bluetooth module");
    }
    delay(2000);
}

 
void loop()
{ 
    // The bluetooth module should still be in AT mode and you can send additional AT commands from the Serial monitor if needed. 
    // Ensure the serial monitor is set to baud rate 9600 and is using 'Both NL & CR' to terminate lines.
      
    // Read from the Bluetooth module and send to the Arduino Serial Monitor
    if (Serial1.available())
    {
        c = Serial1.read();
        Serial.print(c);
    }
 
    // Read from the Serial Monitor and send to the Bluetooth module
    if (Serial.available())
    {
        c = Serial.read();
        Serial1.write(c);   
 
        // Echo the user input to the main window. The ">" character indicates the user entered text.
        if (NL) { Serial.print(">");  NL = false; }
        Serial.write(c);
        if (c==10) { NL = true; }
    }
 
}

int executeAT(char* command){
  Serial.print("Sending AT+");
  Serial.println(command);
  
  Serial1.write("AT+");
  Serial1.write(command);
  Serial1.write("\r\n");

  // wait for a response
  unsigned long start = millis();
  unsigned long waited = 0;
  while (!Serial1.available()){
    delay(100);
    waited = millis()-start;
    if (waited>TIMEOUT){
      Serial.println("Timed out waiting for response from HC-05");
      break;
    }
  }
  
  while(Serial1.available()>0){
    int bytesRead = Serial1.readBytesUntil('\n', response, MAX_RESPONSE_SIZE);
    for (int i = 0; i < bytesRead; i++){
      Serial.write(response[i]);
    }
    Serial.println("");
  }
  
  if ((response[0] == 'O')&&(response[1] == 'K')){
    return OK;
  }
  return ERROR_RESPONSE;
}

void printArray(char* a){
  for (int i = 0; i <17; i++){
    Serial.print(a[i]);
  }
  Serial.println();
}
