
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
// defining communicationion pins for GSM Software Serial
# define GSM_RX 9 // Connect TX of GSM module to arduino's digital pin-9
# define GSM_TX 10 // Connect RX of GSM module to arduino's digital pin-10

// defining communicationion pins for DFmini Software Serial
# define DF_RX 5 // Connect TX of DFmini toarduino's digital pin-5
# define DF_TX 6 // Connect RX of DFmini toarduino's digital pin-6


// Defining interfacing pins for Relay board
# define PORT1 2     //Relay 1
//# define PORT2 4     //Relay 2
//# define PORT3 6     //Relay 3
//# define PORT4 5        //Relay 4



SoftwareSerial gsm_board(GSM_RX, GSM_TX);
SoftwareSerial DF_player(DF_RX, DF_TX);

boolean call_flag = 0, relay_flag = 0;
int i = 0, j = 0, x = -1;
char n[3];
void gsm_initilaize();// inilitize the gsm and chk all its parameters
void relay(); // control relay outputs.

// Create the Player object
DFRobotDFPlayerMini player;





void setup()
{
  // put your setup code here, to run once:

  gsm_board.begin(9600);
  DF_player.begin(9600);
  Serial.begin(9600);
  pinMode(PORT1, OUTPUT);
  pinMode(PORT2, OUTPUT);
  pinMode(PORT3, OUTPUT);
  pinMode(PORT4, OUTPUT);
 // pinMode(MIC, OUTPUT);
  gsm_initilaize();
}







void loop()
{
  String gsm_data; // to hold incomming communication from GSM module
  while (gsm_board.available())
  {
    char c = gsm_board.read();
    gsm_data += c;
    delay(10);
  }  //read serial data and store it to gsm_data STRING instance;
  if (!call_flag) // if call is not in connected, checking for ring
  {
    x = gsm_data.indexOf("+917812045441");
    if (x > -1)
    {
      delay(5000);
      gsm_board.println("ATA");
      Serial.println("ATA");

      
      // Set volume to maximum (0 to 30).
      player.volume(25);
      // Play the first MP3 file on the SD card
      player.play(1);

      call_flag = 1;
    }
  }
  // ring test over, call flag high if sim rings
  if (call_flag) // if call is connected
  {
    x = gsm_data.indexOf("DTMF"); //checkinh dtmf and storing approprietly
    if (x > -1)
    {
      n[j] = gsm_data[x + 6];
      Serial.println(n[j]);
      relay_flag = 1;
    }
    x = gsm_data.indexOf("NO CARRIER"); // Checking whether call is still connected or not
    if (x > -1)
    {
      gsm_board.println("ATH");
      relay_flag = 1;
      call_flag = 0;
      j = 0;
    }
  }
  if (relay_flag) // If a call was resently disconnected, changing relay states accordingly
  {
    relay();
  }
}








void gsm_initilaize()
{
  boolean gsm_Ready = 1;
  Serial.println("initializing GSM module");
  while (gsm_Ready > 0)
  {
    gsm_board.println("AT");
    Serial.println("AT");
    while (gsm_board.available())
    {
      if (gsm_board.find("OK") > 0)
        gsm_Ready = 0;
    }
    delay(2000);
  }
  Serial.println("AT READY");
  // GSM MODULE REPLIED 'OK' TO 'AT' INPUT, INDICAING THE MODULE IS OK AND FUNCTIONING
  boolean ntw_Ready = 1;
  Serial.println("finding network");
  while (ntw_Ready > 0)
  {
    gsm_board.println("AT+CPIN?");
    Serial.println("AT+CPIN?");
    while (gsm_board.available())
    {
      if (gsm_board.find("+CPIN: READY") > 0)
        ntw_Ready = 0;
    }
    delay(2000);
  }
  Serial.println("NTW READY");
  // GSM MODULE REPLIED '+CPIN:READY' TO 'AT+CPIN?' INPUT, INDICAING THE NETWORK IS OK AND FUNCTIONING
  boolean DTMF_Ready = 1;
  Serial.println("turning DTMF ON");
  while (DTMF_Ready > 0)
  {
    gsm_board.println("AT+DDET=1");
    Serial.println("AT+DDET=1");
    while (gsm_board.available())
    {
      if (gsm_board.find("OK") > 0)
        DTMF_Ready = 0;
    }
    delay(2000);
  }
  Serial.println("DTMF READY");// GSM MODULE REPLIED '+OK' TO 'AT+DDET=1?' INPUT, INDICAING THE DTMF IS ON;
}








void relay()
{

  if (n[0] == '1')
  {
    digitalWrite(PORT1, HIGH);
    // Set volume to maximum (0 to 30).
    player.volume(25);
    // Play the 2nd MP3 file on the SD card
    player.play(2);
    Serial.println("PORT1 ON"); 
  }
  //////////////////////////////////////////////////////////////////////////////////////////////////
  else if (n[0] == '2')
  {
    digitalWrite(PORT1, LOW);
      // Set volume to maximum (0 to 30).
      player.volume(25);
      // Play the 3rd MP3 file on the SD card
      player.play(3);
    Serial.println("PORT1 OFF");
  }

  relay_flag = 0;
}  
