/*
    Savjeti za daljnji razvoj: typedef struct, typedef enum,
    pass pointer to function
*/
/*********************************INCLUDOVI************************************/
#include "PDU_RRC6.h"

/***************************FORWARD DECLARATIONS*******************************/
void can_channel_control (uint8_t channel_num, uint8_t val);
void control_channel(uint8_t *buf);

/**********************************TIMERS**************************************/



/*********************************COUNTERS*************************************/
uint8_t IGN_COILS_counter;  //ign_coils_counter ili ignCoilsCounter _err_counter
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
uint8_t SHIFT_P_counter;        
uint8_t SHIFT_N_counter;    
uint8_t CLUTCH_counter;    


/**********************************CAN BUS*************************************/
unsigned long previousMillis = 0;
#define SPI_CS_PIN 53          //chip select pin za CAN bus transceiver [SS]


/*********************************VARIABLES************************************/

MCP_CAN CAN(SPI_CS_PIN);               // Set CS pin

uint8_t CHANN_D [NUM_OF_CHANNELS] = {
    O_IGN_COILS,
    O_FAN_L,    
    O_FAN_R,    
    O_WPUMP_L,  
    O_WPUMP_R,  
    O_BRAKEL,   
    O_SDCIRCUIT,
    O_FPUMP,    
    O_ECU,      
    O_INJECT,   
    O_AUX,      
    O_SHIFT_N,  
    O_SHIFT_P,  
    O_CLUTCH    
}

uint8_t CHANN_D [NUM_OF_CHANNELS] = {
    D_IGN_COILS,
    D_FAN_L,    
    D_FAN_R,    
    D_WPUMP_L,  
    D_WPUMP_R,  
    D_BRAKEL,   
    D_SDCIRCUIT,
    D_FPUMP,    
    D_ECU,      
    D_INJECT,   
    D_AUX,      
    D_SHIFT_P,  
    D_SHIFT_N,  
    D_CLUTCH     
}

uint8_t CHANN_S [NUM_OF_CHANNELS] = {
    S_IGN_COILS
    S_FAN_L,    
    S_FAN_R,    
    S_WPUMP_L,  
    S_WPUMP_R,  
    S_BRAKEL,   
    S_SDCIRCUIT,
    S_FPUMP,    
    S_ECU,      
    S_INJECT,   
    S_AUX,      
    S_SHIFT_P,  
    S_SHIFT_N,  
    S_CLUTCH     
}
/*******************************************************************************
 *                                  SETUP
*******************************************************************************/
void setup() {
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
    digitalWrite(O_SDCIRCUIT, HIGH);
    digitalWrite(O_FPUMP,      LOW);
    digitalWrite(O_ECU,        LOW);
    digitalWrite(O_INJECT,    HIGH);
    digitalWrite(O_AUX,       HIGH);
    digitalWrite(O_SHIFT_P,    LOW);
    digitalWrite(O_SHIFT_N,    LOW);
    digitalWrite(O_CLUTCH,    HIGH);


    CAN.begin(CAN_500KBPS);
}

/*******************************************************************************
 *                                  LOOP
*******************************************************************************/
void loop() 
{
 //millis

 uint32_t currentMillis = millis();

  
// CAN BUS DATA READING CYCLE
//***********************************************************************************
unsigned char canId;
unsigned char len = 0;           // 
unsigned char buf[8];            // Buffer za poruke 8 bytova = 64bita max

//uint32_t timer_millis = 0;        // VIDI ME

//******************************************************************PRIMANJE PORUKA***
if(CAN_MSGAVAIL == CAN.checkReceive()) // check if data coming
{  
    CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf
    canId = CAN.getCanId(); 

    if(canId == 0x03 or canId == 0x02) //0x03 je CAN bus device ID za ECU, 0x02 je ID za DASH
    {
    control_channel(buf);
    }
}

//**********************************************************TRIP CURRENT REACHED///////////////

///////////////////////////////////////////////////////////IGNITION COILS////////////

/**
 *  ISTRAZI SWITCH CASE 
 *  implementiraj switch case kroz counter u if statementu
 * */        
if ((TC_IGN_COILS < S_IGN_COILS) AND (HIGH == O_IGN_COILS)) 
{
    digitalWrite (O_IGN_COILS, LOW);   // pazi sta radi ova linija, digitalWrite
    can_channel_control(IGN_COIL, LOW);
    IGN_COILS_millis = millis();        //  VIDI ME GORE    - inicijaliziraj!!
    IGN_COILS_counter += 1;             // preimenuj
}

if ((LOW == O_IGN_COILS) AND (CH_RST < (current_millis - IGN_COILS_millis)))
{
    O_IGN_COILS = HIGH;                 // greska, pazi liniju
    can_channel_control(IGN_COILS, HIGH);
}

if (IGN_COILS_MAX_ERR <= IGN_COILS_counter)
{
   O_SDCIRCUIT = LOW;                   // opet greska
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

if ((TC_SHIFT_P < S_SHIFT_+) AND (HIGH == O_SHIFT_P)) 
{
    O_SHIFT_P = LOW;      
    can_channel_control(IGN_COIL, LOW);
    SHIFT_+_millis = millis();
    SHIFT_P_counter += 1;
}

if ((LOW == O_SHIFT_P) AND (CH_RST < (current_millis - SHIFT_+_millis)))
{
    O_SHIFT_P = HIGH;
    can_channel_control(SHIFT_+, HIGH);
}

if (SHIFT_+_MAX_ERR <= SHIFT_P_counter)
{
   O_SDCIRCUIT = LOW;
   can_channel_control(SDCIRCUIT, LOW);
   digitalWrite(O_SDCIRCUIT, LOW);
}                     

///////////////////////////////////////////////////////////SHIFT -////////////

if ((TC_SHIFT_N < S_SHIFT_-) AND (HIGH == O_SHIFT_N)) 
{
    O_SHIFT_N = LOW;      
    can_channel_control(IGN_COIL, LOW);
    SHIFT_-_millis = millis();
    SHIFT_N_counter += 1;
}

if ((LOW == O_SHIFT_N) AND (CH_RST < (current_millis - SHIFT_-_millis)))
{
    O_SHIFT_N = HIGH;
    can_channel_control(SHIFT_-, HIGH);
}

if (SHIFT_-_MAX_ERR <= SHIFT_N_counter)
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

/*******************************************************************************
 *                               FUNCTIONS
*******************************************************************************/

/*SLANJE*/

void can_channel_control (uint8_t channel_num, uint8_t val)
{
  buf[1]=channel_num;
  buf[0]=val;
  CAN.sendMsgBuf(PCB_CAN_ID, 0, 2, buf);
}
/*END OF SLANJE*/

/*PRIMANJE*/

void control_channel(uint8_t *buf)
{
   digitalWrite(buf[1], buf[0]);         
}
/*END OF PRIMANJE*/

