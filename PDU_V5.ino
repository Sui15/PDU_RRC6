/************************************************************************************INCLUDOVI******************************************************************/
#include "PDU_FW.h"

/************************************************************************************INICJALIZACIJA PINOVA I KONSTANTI******************************************/

/*********************Trip current******Analog pin read const******Analog read pin pik******messured value******datasheet current*/
#define TC_IGN_COILS 350  
#define TC_FAN_L     350     
#define TC_FAN_R     350     
#define TC_WPUMP_L   350     
#define TC_WPUMP_R   350      
#define TC_BRAKEL    350      
#define TC_SDCIRCUIT 350      
#define TC_FPUMP     350      
#define TC_ECU       350    
#define TC_INJECT    350  
#define TC_AUX       350     
#define TC_SHIFT_+   350     
#define TC_SHIFT_-   350  
#define TC_CLUTCH    350



/******************TIMERS*****************/
#define CH_RST 1000 //Channel reset time 



/******************COUNTERS*****************/
uint8_t IGN_COILS_counter;
uint8_t FAN_L_counter;          
uint8_t FAN_R_counter;          
uint8_t WPUMP_L_counter;        
uint8_t WPUMP_R_counter;         
uint8_t BRAKEL_counter;          
uint8_t SDCIRCUIT_counter;       
uint8_t FPUMP_counter;           
uint8_t ECU_counter;           
uint8_t INJECT_counter;      
uint8_t AUX_counter;            
uint8_t SHIFT_+_counter;        
uint8_t SHIFT_-_counter;    
uint8_t CLUTCH_counter;    


#define IGN_COILS_MAX_ERR       6
#define TC_FAN_L_MAX_ERR        6  
#define TC_FAN_R_MAX_ERR        6  
#define TC_WPUMP_L_MAX_ERR      6  
#define TC_WPUMP_R_MAX_ERR      6   
#define TC_BRAKEL_MAX_ERR       6   
#define TC_SDCIRCUIT_MAX_ERR    6   
#define TC_FPUMP_MAX_ERR        6   
#define TC_ECU_MAX_ERR          6 
#define TC_INJECT_MAX_ERR       6
#define TC_AUX_MAX_ERR          6  
#define TC_SHIFT_+_MAX_ERR      6  
#define TC_SHIFT_-_MAX_ERR      6
#define TC_CLUTCH_MAX_ERR       6



/*********************//*CAN BUS*//
unsigned long previousMillis = 0;

 //CAN
#define SPI_CS_PIN 53    //chip select pin za CAN bus transceiver [SS]


////////////////////////////////////////////////////// INICJALIZACIJA VARIJABLI //////////////////////////////////////////////////////////////////

//////////////////CANBUS ID


MCP_CAN CAN(SPI_CS_PIN);               // Set CS pin


/////////////////////////////////////////////////////////////////////////// MAIN /////////////////////////////////////////////////////////////////////////
void setup() {
  // put your setup code here, to run once:

for(int i = 0; i < NUM_OF_CHANNELS; i++)
{
  pinMode(CHANN_O [i], OUTPUT);             
  pinMode(CHANN_D [i], OUTPUT); 
  pinMode(CHANN_S [i], INPUT);
  
  digitalWrite(CHANN_D [i], OUTPUT);
  analogRead(CHANN_S [i], INPUT); 
}


digitalWrite(O_IGN_COILS, HIGH);
digitalWrite(O_FAN_L,      LOW);
digitalWrite(O_FAN_R,      LOW);
digitalWrite(O_WPUMP_L,    LOW);
digitalWrite(O_WPUMP_R,    LOW); 
digitalWrite(O_BRAKEL,     LOW);
digitalWrite(O_SHCIRCUIT, HIGH);
digitalWrite(O_FPUMP,      LOW);
digitalWrite(O_ECU,        LOW);
digitalWrite(O_INJECT,    HIGH);
digitalWrite(O_AUX,       HIGH);
digitalWrite(O_SHIFT_+,    LOW);
digitalWrite(O_SHIFT_-,    LOW);
digitalWrite(O_CLUTCH,    HIGH);


/////CAN BUS
 CAN.begin(CAN_500KBPS);
}

void loop() 
{



 //millis

 unsigned long currentMillis = millis();

  
// CAN BUS DATA READING CYCLE
//***********************************************************************************
unsigned char canId;
unsigned char len = 0;           // 
unsigned char buf[8];            // Buffer za poruke 8 bytova = 64bita max



//******************************************************************PRIMANJE PORUKA***
if(CAN_MSGAVAIL == CAN.checkReceive()) // check if data coming
    {  
CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf
canId = CAN.getCanId(); 

if(canId == 0x03 or canId == 0x02) //0x03 je CAN bus device ID za ECU, 0x02 je ID za DASH
{
  control_channel(buf);
}}

//********************************************************************************************TRIP CURRENT REACHED///////////////

///////////////////////////////////////////////////////////IGNITION COILS////////////
        
if ((TC_IGN_COILS < S_IGN_COILS) AND (HIGH == O_IGN_COILS)) 
{
    O_IGN_COILS = LOW;      
    can_channel_control(IGN_COIL, LOW);
    IGN_COILS_millis = millis();
    IGN_COILS_counter += 1;
}

if ((LOW == O_IGN_COILS) AND (CH_RST < (current_millis - IGN_COILS_millis)))
{
    O_IGN_COILS = HIGH;
    can_channel_control(IGN_COILS, HIGH);
}

if (IGN_COILS_MAX_ERR <= IGN_COILS_counter)
{
   O_SDCIRCUIT = LOW;
   can_channel_control(SDCIRCUIT, LOW);
   digitalWrite(O_SDCIRCUIT, LOW);
}   

///////////////////////////////////////////////////////////FAN LEFT////////////

if ((TC_FAN_L < S_FAN_L) AND (HIGH == O_FAN_L)) 
{
    O_FAN_L = LOW;      
    can_channel_control(IGN_COIL, LOW);
    FAN_L_millis = millis();
    FAN_L_counter += 1;
}

if ((LOW == O_FAN_L) AND (CH_RST < (current_millis - FAN_L_millis)))
{
    O_FAN_L = HIGH;
    can_channel_control(FAN_L, HIGH);
}

if (FAN_L_MAX_ERR <= FAN_L_counter)
{
   O_SDCIRCUIT = LOW;
   can_channel_control(SDCIRCUIT, LOW);
   digitalWrite(O_SDCIRCUIT, LOW);
}   

///////////////////////////////////////////////////////////FAN RIGHT////////////

if ((TC_FAN_R < S_FAN_R) AND (HIGH == O_FAN_R)) 
{
    O_FAN_R = LOW;      
    can_channel_control(IGN_COIL, LOW);
    FAN_R_millis = millis();
    FAN_R_counter += 1;
}

if ((LOW == O_FAN_R) AND (CH_RST < (current_millis - FAN_R_millis)))
{
    O_FAN_R = HIGH;
    can_channel_control(FAN_R, HIGH);
}

if (FAN_R_MAX_ERR <= FAN_R_counter)
{
   O_SDCIRCUIT = LOW;
   can_channel_control(SDCIRCUIT, LOW);
   digitalWrite(O_SDCIRCUIT, LOW);
}   
         
///////////////////////////////////////////////////////////WATER PUMP LEFT////////////

if ((TC_WPUMP_L < S_WPUMP_L) AND (HIGH == O_WPUMP_L)) 
{
    O_WPUMP_L = LOW;      
    can_channel_control(IGN_COIL, LOW);
    WPUMP_L_millis = millis();
    WPUMP_L_counter += 1;
}

if ((LOW == O_WPUMP_L) AND (CH_RST < (current_millis - WPUMP_L_millis)))
{
    O_WPUMP_L = HIGH;
    can_channel_control(WPUMP_L, HIGH);
}

if (WPUMP_L_MAX_ERR <= WPUMP_L_counter)
{
   O_SDCIRCUIT = LOW;
   can_channel_control(SDCIRCUIT, LOW);
   digitalWrite(O_SDCIRCUIT, LOW);
}    
             

///////////////////////////////////////////////////////////WATER PUMP RIGHT////////////

if ((TC_WPUMP_R < S_WPUMP_R) AND (HIGH == O_WPUMP_R)) 
{
    O_WPUMP_R = LOW;      
    can_channel_control(IGN_COIL, LOW);
    WPUMP_R_millis = millis();
    WPUMP_R_counter += 1;
}

if ((LOW == O_WPUMP_R) AND (CH_RST < (current_millis - WPUMP_R_millis)))
{
    O_WPUMP_R = HIGH;
    can_channel_control(WPUMP_R, HIGH);
}

if (WPUMP_R_MAX_ERR <= WPUMP_R_counter)
{
   O_SDCIRCUIT = LOW;
   can_channel_control(SDCIRCUIT, LOW);
   digitalWrite(O_SDCIRCUIT, LOW);
}            
             
///////////////////////////////////////////////////////////BRAKE LIGHT////////////

if ((TC_BRAKEL < S_BRAKEL) AND (HIGH == O_BRAKEL)) 
{
    O_BRAKEL = LOW;      
    can_channel_control(IGN_COIL, LOW);
    BRAKEL_millis = millis();
    BRAKEL_counter += 1;
}

if ((LOW == O_BRAKEL) AND (CH_RST < (current_millis - BRAKEL_millis)))
{
    O_BRAKEL = HIGH;
    can_channel_control(BRAKEL, HIGH);
}

if (BRAKEL_MAX_ERR <= BRAKEL_counter)
{
   O_SDCIRCUIT = LOW;
   can_channel_control(SDCIRCUIT, LOW);
   digitalWrite(O_SDCIRCUIT, LOW);
}   
           
///////////////////////////////////////////////////////////SHOUT DOUWN CIRCUIT////////////

if ((TC_SDCIRCUIT < S_SDCIRCUIT) AND (HIGH == O_SDCIRCUIT)) 
{
    O_SDCIRCUIT = LOW;      
    can_channel_control(IGN_COIL, LOW);
    SDCIRCUIT_millis = millis();
    SDCIRCUIT_counter += 1;
}

if ((LOW == O_SDCIRCUIT) AND (CH_RST < (current_millis - SDCIRCUIT_millis)))
{
    O_SDCIRCUIT = HIGH;
    can_channel_control(SDCIRCUIT, HIGH);
}

if (SDCIRCUIT_MAX_ERR <= SDCIRCUIT_counter)
{
   O_SDCIRCUIT = LOW;
   can_channel_control(SDCIRCUIT, LOW);
   digitalWrite(O_SDCIRCUIT, LOW);
}             

///////////////////////////////////////////////////////////FUEL PUMP////////////
 
if ((TC_FPUMP < S_FPUMP) AND (HIGH == O_FPUMP)) 
{
    O_FPUMP = LOW;      
    can_channel_control(IGN_COIL, LOW);
    FPUMP_millis = millis();
    FPUMP_counter += 1;
}

if ((LOW == O_FPUMP) AND (CH_RST < (current_millis - FPUMP_millis)))
{
    O_FPUMP = HIGH;
    can_channel_control(FPUMP, HIGH);
}

if (FPUMP_MAX_ERR <= FPUMP_counter)
{
   O_SDCIRCUIT = LOW;
   can_channel_control(SDCIRCUIT, LOW);
   digitalWrite(O_SDCIRCUIT, LOW);
}   
   
///////////////////////////////////////////////////////////ECU////////////

if ((TC_ECU < S_ECU) AND (HIGH == O_ECU)) 
{
    O_ECU = LOW;      
    can_channel_control(IGN_COIL, LOW);
    ECU_millis = millis();
    ECU_counter += 1;
}

if ((LOW == O_ECU) AND (CH_RST < (current_millis - ECU_millis)))
{
    O_ECU = HIGH;
    can_channel_control(ECU, HIGH);
}

if (ECU_MAX_ERR <= ECU_counter)
{
   O_SDCIRCUIT = LOW;
   can_channel_control(SDCIRCUIT, LOW);
   digitalWrite(O_SDCIRCUIT, LOW);
}   
           

///////////////////////////////////////////////////////////INJECTORS////////////

if ((TC_INJECT < S_INJECT) AND (HIGH == O_INJECT)) 
{
    O_INJECT = LOW;      
    can_channel_control(IGN_COIL, LOW);
    INJECT_millis = millis();
    INJECT_counter += 1;
}

if ((LOW == O_INJECT) AND (CH_RST < (current_millis - INJECT_millis)))
{
    O_INJECT = HIGH;
    can_channel_control(INJECT, HIGH);
}

if (INJECT_MAX_ERR <= INJECT_counter)
{
   O_SDCIRCUIT = LOW;
   can_channel_control(SDCIRCUIT, LOW);
   digitalWrite(O_SDCIRCUIT, LOW);
}         
           
///////////////////////////////////////////////////////////AUX////////////

if ((TC_AUX < S_AUX) AND (HIGH == O_AUX)) 
{
    O_AUX = LOW;      
    can_channel_control(IGN_COIL, LOW);
    AUX_millis = millis();
    AUX_counter += 1;
}

if ((LOW == O_AUX) AND (CH_RST < (current_millis - AUX_millis)))
{
    O_AUX = HIGH;
    can_channel_control(AUX, HIGH);
}

if (AUX_MAX_ERR <= AUX_counter)
{
   O_SDCIRCUIT = LOW;
   can_channel_control(SDCIRCUIT, LOW);
   digitalWrite(O_SDCIRCUIT, LOW);
}               

///////////////////////////////////////////////////////////SHIFT +////////////

if ((TC_SHIFT_+ < S_SHIFT_+) AND (HIGH == O_SHIFT_+)) 
{
    O_SHIFT_+ = LOW;      
    can_channel_control(IGN_COIL, LOW);
    SHIFT_+_millis = millis();
    SHIFT_+_counter += 1;
}

if ((LOW == O_SHIFT_+) AND (CH_RST < (current_millis - SHIFT_+_millis)))
{
    O_SHIFT_+ = HIGH;
    can_channel_control(SHIFT_+, HIGH);
}

if (SHIFT_+_MAX_ERR <= SHIFT_+_counter)
{
   O_SDCIRCUIT = LOW;
   can_channel_control(SDCIRCUIT, LOW);
   digitalWrite(O_SDCIRCUIT, LOW);
}                     

///////////////////////////////////////////////////////////SHIFT -////////////

if ((TC_SHIFT_- < S_SHIFT_-) AND (HIGH == O_SHIFT_-)) 
{
    O_SHIFT_- = LOW;      
    can_channel_control(IGN_COIL, LOW);
    SHIFT_-_millis = millis();
    SHIFT_-_counter += 1;
}

if ((LOW == O_SHIFT_-) AND (CH_RST < (current_millis - SHIFT_-_millis)))
{
    O_SHIFT_- = HIGH;
    can_channel_control(SHIFT_-, HIGH);
}

if (SHIFT_-_MAX_ERR <= SHIFT_-_counter)
{
   O_SDCIRCUIT = LOW;
   can_channel_control(SDCIRCUIT, LOW);
   digitalWrite(O_SDCIRCUIT, LOW);
}                 

///////////////////////////////////////////////////////////CLUTCH////////////

if ((TC_CLUTCH < S_CLUTCH) AND (HIGH == O_CLUTCH)) 
{
    O_CLUTCH = LOW;      
    can_channel_control(IGN_COIL, LOW);
    CLUTCH_millis = millis();
    CLUTCH_counter += 1;
}

if ((LOW == O_CLUTCH) AND (CH_RST < (current_millis - CLUTCH_millis)))
{
    O_CLUTCH = HIGH;
    can_channel_control(CLUTCH, HIGH);
}

if (CLUTCH_MAX_ERR <= CLUTCH_counter)
{
   O_SDCIRCUIT = LOW;
   can_channel_control(SDCIRCUIT, LOW);
   digitalWrite(O_SDCIRCUIT, LOW);
}   

                                 

//********************************************************************************************SLANE TRENTNE POTROŠNJE STRUJE I POJEDINOG UREDAJA///////////////

//DODATI NAKNADNO KAD SE SVE SLOŽI






}
