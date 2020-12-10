/*include*/
#include <mcp_can.h>
#include <mcp_can_dfs.h>
#include <SPI.h>


#define NUM_OF_CHANNELS 14

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
#define TC_SHIFT_P   350     
#define TC_SHIFT_N   350  
#define TC_CLUTCH    350

//////////////////////////////POWER MANAGEMENT CHANELL OUTPUTE ACTIVATION (O)
////////////////////////////////////////CHANELL/////ATMEGA_PIN
#define O_IGN_COILS 44    //PL5          //08        //40
#define O_FAN_L      4    //PG5          //10        //01
#define O_FAN_R      5    //PE3          //09        //05
#define O_WPUMP_L    8    //PH5          //11        //17
#define O_WPUMP_R   12    //PB6          //12        //25
#define O_BRAKEL     2    //PE4(RX)      //18        //06        
#define O_SDCIRCUIT  6    //PH3          //19        //15
#define O_FPUMP      1    //PE1(TX)      //13        //03
#define O_ECU       45    //PL4          //14        //39
#define O_INJECT    03    //PE5          //07        //07
#define O_AUX        0    //PE0          //15        //02
#define O_SHIFT_P   11    //PB5          //16        //24
#define O_SHIFT_N    7    //PH4          //17        //16
#define O_CLUTCH    46    //PL3          //20        //38



//////////////////////////////POWER MANAGEMENT CHANELL OUTPUTE DIAGNOSTIC ACTIVATION (D)
////////////////////////////////////////CHANELL/////ATMEGA_PIN/
#define D_IGN_COILS 33    //PC4           //08       //57        
#define D_FAN_L     32    //PB4           //10       //10        
#define D_FAN_R     39    //PH6           //09       //09        
#define D_WPUMP_L   25    //PB5           //11       //11        
#define D_WPUMP_R   36    //PB6           //12       //12        
#define D_BRAKEL    29    //PD3           //18       //18        
#define D_SDCIRCUIT 27    //PD2           //19       //19        
#define D_FPUMP     30    //PC7           //13       //13        
#define D_ECU       34    //PC3           //14       //14        
#define D_INJECT    28    //PA6           //07       //07        
#define D_AUX       31    //PC6           //15       //15        
#define D_SHIFT_P   37    //PC0           //16       //16        
#define D_SHIFT_N   26    //PH0           //17       //17        
#define D_CLUTCH    35    //PD1           //20       //20        




//////////////////////////////POWER MANAGEMENT CHANELL INPUT SEANS (S)
////////////////////////////////////////CHANELL/////ATMEGA_PIN//ARDUNO PIN
#define S_IGN_COILS 10    //PK2           //08       //87        
#define S_FAN_L      9    //PK1           //10       //88        
#define S_FAN_R      6    //PF6           //09       //91        
#define S_WPUMP_L    1    //PF1           //11       //96        
#define S_WPUMP_R   13    //PK5           //12       //84        
#define S_BRAKEL     5    //PF5           //18       //92        
#define S_SDCIRCUIT  3    //PF3           //19       //94        
#define S_FPUMP      7    //PF7           //13       //90        
#define S_ECU       11    //PK3           //14       //86        
#define S_INJECT     4    //PF5           //07       //93        
#define S_AUX        8    //PK0           //15       //89        
#define S_SHIFT_P   14    //PK6           //16       //83        
#define S_SHIFT_N    2    //PF2           //17       //95        
#define S_CLUTCH    12    //PK4           //20       //85        

#define PCB_CAN_ID 0x100  // ID PDU-a za CAN bus - PORUKE!

////////////////////////////////CAN PORUKE/////////////(CM-CAN massage)

CAN.sendMsgBuf(PCB_CAN_ID, 0, 1, buf);


//|_|_|_|_|_|_|_|_|

//|_|_|_|_|_|_|IGN_COILS|OFF|


can_ign_coil_control(on);
uint8_t buf[8]={0}; 

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
