//----------------------------------------------------------------------------
//
//                      www.lightbyhelios.com
//
// C main line
//
//----------------------------------------------------------------------------
    
#include <m8c.h>            // part specific constants and macros
#include "PSoCAPI.h"        // PSoC API definitions for all User Modules  
#include "PSoCGPIOINT.h"    // include the GPIO API header file 
#include "adcinc.h"         // A/D
#include "pga.h"            // programmable gain amplifier
#include "FlashBlock.h"     // flash memory api
#include "debounce.h"
#include "btn_events.h"
#include "menu_fsm.h"
#include "switches.h"
#include "main.h"

// LED Duty Constants
#define LED_MAX        255      // 100% LED duty
#define LED_MIN          0      //   0% LED duty
#define LED_LOW        64      //  25% LED duty
#define LED_MED        127      //  50% LED duty

#define DELAY_ONE 		1000	// 1 second delay 1ms x 1000
#define DELAY_THREE		3000	// 3 second delay 1ms x 3000

// Temperature Threshold - used to shut down LEDs
#define TEMP_MAX        100      // 100 = 85 degrees C 
//#define TEMP_MAX        85      // 85 = 64 degrees C 
//#define TEMP_MAX        90      // 90 = 70 degrees C 
//#define TEMP_MAX        60      // 60 = 30 degrees C 
//#define TEMP_MAX        76      // 76 = 50 degrees C 
//#define TEMP_MAX        70      // 70 = 40 degrees C

// buttons_report bits defined
#define VOLT_MASK       0x80    // bit 7
#define TEMP_MASK       0x40    // bit 6

// lights_report bits defined
#define USB_MOT_MASK    0x01    // bit 0 - Motor power status
#define USB_DIR_MASK    0x02    // bit 1 - Motor direction
#define USB_LED_MASK    0x04    // bit 2 - LED power status
#define USB_AUTO_MASK   0x08    // bit 3 - toggles to indicate software running
#define USB_BIT4_MASK   0x10    // bit 4 - spare
#define USB_BIT5_MASK   0x20    // bit 5 - spare
#define USB_BIT6_MASK   0x40    // bit 6 - spare
#define USB_BOOT_MASK   0x80    // bit 7 - Boot entry flag

extern char SOF_Flag;       // 1ms USB start of frame interrupt flag, set in SOF_ISR
extern char USB_INTERFACE_0_OUT_RPT_DATA[8];
char buttons_report[8];	    // OUT report
char lights_report;		    // A one char IN report
char Temperature = 0;       // A/D temperature value
char Voltage = 0;           // A/D voltage value 
char VoltFlag = 0;          // flag to indicate voltage threshold exceeded
char Dir = 0;              // flag for adjusting height
char ThermoFlag = 0;        // indicates thermal overload
char Auto = 1;              // flag for adjusting control of light
char ButtonTimer = 0;          // button debounce counter
char USBTimer = 0;			// USB count used to monitor software control
char USB_Tog = 0;			// toggles to indicate software status OK
char BtnState = 0;          // state flag
char BtnReturnValue = 1;    // button register
char Mode = 0;              // LED blink mode
char status;                // I2C communication status  
char i;                     // Temporary counter variable
char j;                     // Temporary counter variable
char LedPowerTog = 1;       // flags a change of power logic
char LedPower = 1;          // LED status, 0 = OFF, 1 = ON
char i;                     // temporary counter variable
char RedDuty = 0;           // RED LED dutycycle
char BlueDuty = 0;          // BLUE LED dutycycle            
char GreenDuty = 0;         // GREEN LED dutycycle  
char RedDutyTarget = LED_MAX;   // use to transition from target dutycycle to actual duty smoothly
char BlueDutyTarget = LED_MAX;  // use to transition from target dutycycle to actual duty smoothly              
char GreenDutyTarget = LED_MAX; // use to transition from target dutycycle to actual duty smoothly
char RedDutyMax = LED_MAX;      // maximum RED LED dutycycle
char BlueDutyMax = LED_MAX;     // maximum BLUE LED dutycycle              
char GreenDutyMax = LED_MAX;    // maximum WHITE LED dutycycle
char RedUpFlag = 0;         // RED LED Flag to indicate initial ramp up condition
char BlueUpFlag = 0;        // BLUE LED Flag to indicate initial ramp up condition           
char GreenUpFlag = 0;       // GREEN LED Flag to indicate initial ramp up condition
char RedDownFlag = 0;         // RED LED Flag to indicate initial ramp up condition
char BlueDownFlag = 0;        // BLUE LED Flag to indicate initial ramp up condition           
char GreenDownFlag = 0;       // GREEN LED Flag to indicate initial ramp up condition
char RedBlueUpFlag = 0;     // Red Blue LED Flag to indicate initial ramp up condition
char RedGreenUpFlag = 0;    // Red Green LED Flag to indicate initial ramp up condition
char GreenBlueUpFlag = 0;   // Green Blue LED Flag to indicate initial ramp up condition
int  LedTimer = 0;          // used to slow LED dutycycle transitions
char LedPulseState = 0;          // LED state, on or off
char ledChangeRate = 1;           // LED rate, on or off
char BlinkTimer = 0;        // Blink patter counter
char DutyTimer = 0;         // used to slow LED dutycycle transitions
char MultiplexCount = 0;    // multiplex counter, 0 = red, 1 = green, 2 = blue
char Colour = 0;			// Colour state
int  ButtonHoldTimer = 0; 		// Button hold counter
char BtnHoldFlag = 0; 		// Button hold flag
char BtnRateChangeFlag = 0; // Indicates rate change
char BtnPressFlag = 0;		// Button press flag
char WriteFlashFlag = 0; 	// Flag to indicate storage to flash memory
char ColourCount = 0;		// Colour state counter for press and hold
char RedIncFlag = 0;		// Red increment flag
char GreenIncFlag = 0;		// Green increment flag
char BlueIncFlag = 0;		// Blue increment flag
char pulseLedFlag = 0; 		// Pulse LEDs 3 times to indicate state changes
char PauseTimer = 0; 		// Pause time counter
char CandleTimer = 0; 		// Candle counter
int  SaveFlashTimer = 0;	// Used to delay save to flash after button release event
char g_timer_button_release = 0;
char SpectrumLedsEntry = 0; // For initialization of variables
//char cDieTemp = 20; 		// Allocate a variable to hold the die temperature
							// Use 20C as the default value

eFsmState_t the_state;      // finite state machine 'the_state' is the current state
btn_events_t Events;        // button events

struct FlashBlock  {
    unsigned char Dummy;
    unsigned char ledChangeRate;
    unsigned char the_state;
    unsigned char ColourCount;
    unsigned char PowerState;    
    unsigned char RedDuty;
    unsigned char BlueDuty;
    unsigned char GreenDuty;
}RamFlashBlock;

// function prototype  
void MeasureTemperature(void);
void ButtonStates(void);
void CommunicateUSB(void);
void DebounceButton(btn_events_t *Events);
int  GetSample(char bChannel);
void MeasureTemperature(void);
void PulseLeds(void);
void blinkLeds(void);
void LedBrightness(void);
void LedStates(void);
void CandleLeds(void);
void SetFlashDefaults(void);
void ReadFlash(void);
void WriteFlash(void);
void ThermalProtection(void);
void DelayedSaveFlash(void);
void SpectrumLeds(void);
//void BL_SetTemp (char cTemp);

// timer interrupt prototype
#pragma interrupt_handler Timer8_ISR
#pragma interrupt_handler PWM8_ISR

#pragma abs_address:0x3F80  // 2nd to last 64 bytes of flash (note in flashsecurity.txt "U" unprotected flash)
const unsigned char FlashParameters[64];
//there are not 64 parameters in my application, but this is mapped for correct spacing
#pragma end_abs_address

void main(void)  
{
    // Initialize Pull Up/Down ressistors
    //Port_0_Data_SHADE = 0x80;                     // Enable distance input pull-up P0.7
    //PRT0DR = Port_0_Data_SHADE;
    Port_2_Data_SHADE = 0x04;                     // Enable pull-down resistor on LED P2.2
    PRT2DR = Port_2_Data_SHADE;
    Port_3_Data_SHADE = 0x20;                     // Enable pull-up on button bit P3.5 
    PRT3DR = Port_3_Data_SHADE;
    Port_4_Data_SHADE = 0x44;						// Enable pull-down LED P4.2, P4.6
    PRT4DR = Port_4_Data_SHADE;

    Timer8_WritePeriod(50);				// 12MHz/15/16/50 = 1KHz => 1ms main timer interrupt
    Timer8_WriteCompareValue(0);
    Timer8_EnableInt();
    Timer8_Start();
	PRS8_WritePolynomial(0x78);			// load the PRS polynomial
	PRS8_WriteSeed(0xFF);				// load the PRS seed
	PRS8_Start();						// start the PRS8
	RED_Start();
	GREEN_Start();
	BLUE_Start();
    PWM8_WritePeriod(100);				// set period to eight clocks
    PWM8_WritePulseWidth(0); 			// set pulse width to generate a % duty cycle 
    PWM8_EnableInt(); 					// ensure interrupt is enabled  
    PWM8_Start();						// start PWM 
    //DAC_CR &= ~0x80;	                // turn off SplitMUX bit 7 (P0[7] on right, others on left)    
    PGA_Start(PGA_HIGHPOWER);           // Start PGA
    ADCINC_Start(ADCINC_HIGHPOWER);     // Start ADC
    ADCINC_GetSamples(1);               // initiate the first sample
	
    M8C_EnableGInt;						// Global interrupt enable
  
	ReadFlash();						// read on/off times and LED dutycyle from FLASH
    if(!(RamFlashBlock.Dummy == 0x55))
    {
        SetFlashDefaults();             // clear flash first time
    } 

	LedPowerTog = 1;						// flag change				
	LedPower = RamFlashBlock.PowerState;
    RedDutyMax = RamFlashBlock.RedDuty;
    BlueDutyMax = RamFlashBlock.BlueDuty;
    GreenDutyMax = RamFlashBlock.GreenDuty;
	the_state = RamFlashBlock.the_state;		
	ledChangeRate = RamFlashBlock.ledChangeRate;
	
    Events.press = 0;
    Events.hold = 0;
    Events.release = 0;  	
    MenuFsm(&Events, &the_state);       // initlaize the state machine 
	
    USB_Start(0, USB_3V_OPERATION);     // Start USB
    //while (!USB_bGetConfiguration());   // Wait to be enumerated
    USB_INT_REG |= USB_INT_SOF_MASK;
    USB_EnableOutEP(1);                 // Post a buffer to wait for a command
			    
    while(1)                            // cycle the puck here
    {       
        MeasureTemperature();               // sample input temperature sensor voltage
        ThermalProtection();                 // decrease LED power if temperature rises above limit
		ButtonStates();                    // button driver
        CommunicateUSB();                       // USB driver
		LedStates();						// LED Cadence state machine
		DelayedSaveFlash();					// Save power state and RGB dutycycle 10 seconds after last button event
	}
}          

/************************************************************************
*  LedStates - Drive RGB LEDs based in state machine
*
*  Parameters: value  
*  Returns:    none 

*
************************************************************************/
void LedStates(void)
{
    if(Auto)                             // if USB connected then disable state machine
    {
	    switch(the_state)                   // LED cadence state switch                     
        {
            case STATE_SOLID: 
			{
				LedBrightness();            // Smooth LED dutycycle transition       
                break;
			}
            case STATE_PULSE:
			{
				PulseLeds();
				LedBrightness();            // Smooth LED dutycycle transition       				
                break;
			}
            case STATE_SPECTRUM:
			{
				SpectrumLeds();
                break;
			}
            case STATE_FLICKER:
			{
				CandleLeds();
				LedBrightness();            // Smooth LED dutycycle transition       				
                break;	
			}
            default:
			{
	        }
		}
    }        
    else
    {
		LedBrightness();            		// Smooth LED dutycycle transition       				
	}
      
} 

/************************************************************************
* LedBrightness - smooth dutycycle transition
* maximum transition time = ledChangeRate x 1ms
*
************************************************************************/
void LedBrightness(void)
{           
    if(!DutyTimer)
    {
        DutyTimer = ledChangeRate;                            
        //DutyTimer = 1;                             // reset 1ms timer

        if(LedPowerTog)                             // only call once every toggle
        {
            LedPowerTog = 0;                        // clear flag
            if(LedPower)
            {
                RedDutyTarget = RedDutyMax;         // LEDs smooth on
                BlueDutyTarget = BlueDutyMax;              
                GreenDutyTarget = GreenDutyMax; 
            }
            else
            {
				RedDutyTarget = LED_MIN;			// LEDs smooth off
				BlueDutyTarget = LED_MIN;
				GreenDutyTarget = LED_MIN;             
			}
        }
    
        // smooth transition   
        if(RedDuty < RedDutyTarget)
        {
            RedDuty++;
        }
        if(RedDuty > RedDutyTarget)
        {
            RedDuty--;
        } 
        
        if(BlueDuty < BlueDutyTarget)
        {
            BlueDuty++;
        }
        if(BlueDuty > BlueDutyTarget)
        {
            BlueDuty--;
        }
        
        if(GreenDuty < GreenDutyTarget)
        {
            GreenDuty++;

        }
        if(GreenDuty > GreenDutyTarget)
        {
            GreenDuty--;
        }
    }
}

/************************************************************************
*  PulseLeds - Pulse the LEDs on and off every second
*
*  Parameters: value  
*  Returns:    none 

*
************************************************************************/
void PulseLeds(void)
{
	if((!LedTimer) && (LedPower))
	{
        if(LedPulseState)
        {
			LedTimer = 1000*ledChangeRate;
			LedPulseState = 0;	                // state off
			RedDutyTarget = LED_MIN;			// LEDs smooth off
			BlueDutyTarget = LED_MIN;
			GreenDutyTarget = LED_MIN;   			
        }
        else
        {
			LedTimer = 1000*ledChangeRate;		
			LedPulseState = 1;	                // state on		
            RedDutyTarget = RedDutyMax;         // LEDs smooth on
            BlueDutyTarget = BlueDutyMax;              
            GreenDutyTarget = GreenDutyMax;           
		}		
	}
}

/************************************************************************
*  blinkLeds -  Blink the LEDs on for 100ms and off for one second
*
*  Parameters: value  
*  Returns:    none 

*
************************************************************************/
void blinkLeds(void)
{
	if((!LedTimer) && (LedPower))
	{
        if(LedPulseState)
        {
			LedTimer = 1000;
			LedPulseState = 0;	                // state off
			RedDuty = LED_MIN;			// LEDs smooth off
			BlueDuty = LED_MIN;
			GreenDuty = LED_MIN;   			
        }
        else
        {
			LedTimer = 1000;		
			LedPulseState = 1;	                // state on		
            RedDuty = RedDutyMax;         // LEDs smooth on
            BlueDuty = BlueDutyMax;              
            GreenDuty = GreenDutyMax;           
		}	
	}
}  

/************************************************************************
*  CandleLeds - flicker LED like a candle flame
*  
*  Parameters: none  
*  Returns:    none 
************************************************************************/
void CandleLeds (void)
{
	if((!CandleTimer) && (LedPower))
	{
		CandleTimer = PRS8_bReadPRS();     // restart 1ms x [0 - 20] = 0 - 20ms button delay
		
		if(CandleTimer > 20)						// flicker limit
		{
			CandleTimer--;
			ColourCount = 255;
		}
		else
		{
			ColourCount = PRS8_bReadPRS();		
		}
		
		switch(ColourCount)          	//                     
		{		
			case 0:	  					// Flicker off
			{
				RedDutyTarget = LED_LOW;
				BlueDutyTarget = LED_MIN;
				GreenDutyTarget = LED_LOW;
				CandleTimer++;
				break;		
			}
			case 127:	 					// Flicker orange
			{
				RedDutyTarget = LED_MAX;
				BlueDutyTarget = LED_MIN;
				GreenDutyTarget = LED_LOW;
				CandleTimer++;
				break;
			}
			default:
			{
				RedDutyTarget = LED_MAX;
				BlueDutyTarget = LED_MIN;
				GreenDutyTarget = LED_MED;	
			}
		}
	}
	else
	{
		CandleTimer--;
	}
}    

/************************************************************************
*  SpectrumLeds - Gradual spectrum change like a candle flame
*  
*  Parameters: none  
*  Returns:    none 
************************************************************************/
void SpectrumLeds(void)
{
	if((!LedTimer) && (LedPower))
	{
		if(!SpectrumLedsEntry)
		{
			ledChangeRate = 1;
			SpectrumLedsEntry = 1;						// first time here
			ColourCount = 0; 
			RedDutyTarget = RamFlashBlock.RedDuty;		
			BlueDutyTarget = RamFlashBlock.BlueDuty;
			GreenDutyTarget = RamFlashBlock.GreenDuty;
		}
	
		switch(ColourCount)          	// Gradually change LED colour (Chameleon)                     
		{		
			case 0:	  					// Increment Red
			{
				if(RedDutyTarget < LED_MAX)
				{
					RedDutyTarget++;
				}
				else
				{
					ColourCount++;
				}
				break;	
			}
			case 1:	 					// Increment Green and Decrement Red
			{
				if(GreenDutyTarget < LED_MAX)
				{
					GreenDutyTarget++;
				}
				
				if(RedDutyTarget > 0)
				{
					RedDutyTarget--;
				}
				if((RedDutyTarget == 0) && (GreenDutyTarget == LED_MAX))
				{
					ColourCount++;
				}
				break;	
			}			
			case 2:	 					// Increment Blue and Decrement Green
			{
				if(BlueDutyTarget < LED_MAX)
				{
					BlueDutyTarget++;
				}
				if(GreenDutyTarget > 0)
				{
					GreenDutyTarget--;
				}
				if((GreenDutyTarget == 0) && (BlueDutyTarget == LED_MAX))
				{
					ColourCount++;
				}
				break;	
			}
			case 3:	  					// Increment Red
			{
				if(RedDutyTarget < LED_MAX)
				{
					RedDutyTarget++;
				}
				if(RedDutyTarget == LED_MAX)
				{
					ColourCount++;
				}
				break;			
			}
			case 4:	  					// Increment Green
			{
				if(GreenDutyTarget < LED_MAX)
				{
					GreenDutyTarget++;
				}
				if(GreenDutyTarget == LED_MAX)
				{
					ColourCount++;
				}
				break;
			}
			case 5:	  					// Decrement blue and Green
			{
				if(BlueDutyTarget > 0)
				{
					BlueDutyTarget--;
				}
				if(GreenDutyTarget > 0)
				{
					GreenDutyTarget--;
				}
				if((BlueDutyTarget == 0) && (GreenDutyTarget == 0))
				{
					ColourCount = 1;
				}
				break;
			}
			default:
			{
				ColourCount = 0;
			}
		}
	}
}			

/************************************************************************
*  ThermalProtection - decrease LED power if temperature rises above threshold
*  
*  Parameters: none  
*  Returns:    none 
************************************************************************/
void ThermalProtection (void)
{
    if(Temperature > TEMP_MAX)          // thermal over-temperature protection
    {
        ThermoFlag = 1;                 // set thermal overload flag
        //LedPowerTog = 1;            // and turn power back on
		//LedPower = 0;	            // power off 
    }
    if(ThermoFlag)
    {
        if(Temperature < TEMP_MAX)      // if cool enough again then
        {           
            ThermoFlag = 0;             // clear thermal overload flag
            //LedPowerTog = 1;            // and turn power back on
			//LedPower = 1;	            // power on
        }
    }
}  

/************************************************************************
*  ReadFlash
*  
*  Parameters: none  
*  Returns:    none 
************************************************************************/
void ReadFlash (void)
{//reads back vital data
    RamFlashBlock.Dummy=(FlashParameters[0]);
    RamFlashBlock.ledChangeRate=FlashParameters[1];     
    RamFlashBlock.the_state=FlashParameters[2];
	RamFlashBlock.ColourCount=FlashParameters[3];     
    RamFlashBlock.PowerState=FlashParameters[4];    
    RamFlashBlock.RedDuty=(FlashParameters[5]);
    RamFlashBlock.BlueDuty=(FlashParameters[6]);
    RamFlashBlock.GreenDuty=(FlashParameters[7]);
} 

/************************************************************************
*  SetFlashDefaults
*  
*  Parameters: none  
*  Returns:    none 
************************************************************************/
void SetFlashDefaults(void)
{
    RamFlashBlock.Dummy = 0x55;
    RamFlashBlock.ledChangeRate = 1;
    RamFlashBlock.the_state = FIRST_STATE; 
    RamFlashBlock.ColourCount = 0;
    RamFlashBlock.PowerState = 1;    
    RamFlashBlock.RedDuty = LED_MAX;
    RamFlashBlock.BlueDuty = LED_MAX;
    RamFlashBlock.GreenDuty = LED_MAX;
    WriteFlash();
}

/************************************************************************
*  write 64 bytes because of block size of Flash, all 64 bytes must be written
*  
*  Parameters: none  
*  Returns:    none 
************************************************************************/
void WriteFlash(void)
{
    FLASH_WRITE_STRUCT sFlashWriteData;

    sFlashWriteData.wARG_BlockId = 254;
    sFlashWriteData.pARG_FlashBuffer = &RamFlashBlock.Dummy;
    sFlashWriteData.cARG_Temperature = (Temperature - 20);
    bFlashWriteBlock( &sFlashWriteData );
}

/************************************************************************
*  DelayedSaveFlash - Save power state and RGB dutycycle 10 seconds after 
*	last button event
*  
*  Parameters: none  
*  Returns:    none 
************************************************************************/
void DelayedSaveFlash (void)
{
	if((!SaveFlashTimer) && WriteFlashFlag)		// If counter expires and flash flag set
	{
		WriteFlashFlag = 0;
		PWM8_Stop();
		WriteFlash();                  			// store to Flash
		PWM8_Start();		
	}
}

/************************************************************************
*  ButtonStates - handle button press input
*
*  Parameters: none  
*  Returns:    none 
************************************************************************/
void ButtonStates(void)
{	
    if(!ButtonTimer)
    {              
        ButtonTimer = 10;                             // 10ms button delay
        DebounceButton(&Events);                     // 4  x 10ms = 40ms button poll
		
        if(Auto)                                    // if USB connected then dissable state machine
        {
            MenuFsm(&Events, &the_state);           // button events control the state machine 
        
			if (Events.double_press & SW_PWR)		// Double press
			{
				LedPowerTog = 1;						// flag change
				LedPower = 1;	                	    // power on
				SaveFlashTimer = 0;						// delay one second before save
				WriteFlashFlag = 1;		
    			RamFlashBlock.the_state = the_state;			
				RamFlashBlock.PowerState = LedPower;
			}
		
			if ((Events.press & SW_PWR) && (ButtonHoldTimer == 0))
			{
				BtnPressFlag = 1;                    	// flag button press
				ButtonHoldTimer = 500;                  	// start 500 x 1ms = 1/2 second button hold count			
			} 
			
			// Change rate
			if ((Events.hold & SW_PWR) && (ButtonHoldTimer == 0) && (the_state == STATE_PULSE))
			{
				ButtonHoldTimer = 500;                  		// start 1000 x 1ms = 1second button repeat rate
				if(!BtnHoldFlag)						// first time into hold
				{
					BtnPressFlag = 0;                    	// flag button press
					BtnRateChangeFlag = 1;						// first time here
					pulseLedFlag = 1; 
					
					// Increment rate once per press
					ledChangeRate++;
					if(ledChangeRate > 3)
					{
						ledChangeRate = 1;
					}	
					// blink LED the number of times to indicate rate
					// blinkLed(ledChangeRate);
				}
			}
			
			// Change colour
			if ((Events.hold & SW_PWR) && (ButtonHoldTimer == 0) && (the_state == STATE_SOLID))
			{
				ButtonHoldTimer = 10;                  		// start 10 x 1ms = 10ms button repeat rate
				
				if(!BtnHoldFlag)						// first time into hold
				{
					BtnPressFlag = 0;                    	// flag button press
					BtnHoldFlag = 1;						// first time here
					ColourCount = RamFlashBlock.ColourCount; // retrieve last setting
					RedDutyTarget = RamFlashBlock.RedDuty;		
					BlueDutyTarget = RamFlashBlock.BlueDuty;
					GreenDutyTarget = RamFlashBlock.GreenDuty;
				}
				
				switch(ColourCount)          	// Gradually change LED colour (Chameleon)                     
				{		
					case 0:	  					// Increment Red
					{
						if(RedDutyTarget < LED_MAX)
						{
							RedDutyTarget++;
						}
						else
						{
							ColourCount++;
						}
						break;	
					}
					case 1:	  					// Pause on Red		
					{
						if(PauseTimer < LED_MED)
						{
							PauseTimer++;
						}
						if(PauseTimer == LED_MED)
						{
							ColourCount++;
							PauseTimer = 0;
						}
						break;		
					}
					case 2:	 					// Increment Green and Decrement Red
					{
						if(GreenDutyTarget < LED_MAX)
						{
							GreenDutyTarget++;
						}
						
						if(RedDutyTarget > 0)
						{
							RedDutyTarget--;
						}
						if((RedDutyTarget == 0) && (GreenDutyTarget == LED_MAX))
						{
							ColourCount++;
						}
						break;
					}
					case 3:	  					// Pause on Green	
					{
						if(PauseTimer < LED_MED)
						{
							PauseTimer++;
						}
						if(PauseTimer == LED_MED)
						{
							ColourCount++;
							PauseTimer = 0;						
						}
						break;
					}
					case 4:	 					// Increment Blue and Decrement Green
					{
						if(BlueDutyTarget < LED_MAX)
						{
							BlueDutyTarget++;
						}
						if(GreenDutyTarget > 0)
						{
							GreenDutyTarget--;
						}
						if((GreenDutyTarget == 0) && (BlueDutyTarget == LED_MAX))
						{
							ColourCount++;
						}
						break;
					}
					case 5:	  					// Pause on Blue
					{
						if(PauseTimer < LED_MED)
						{
							PauseTimer++;
						}
						if(PauseTimer == LED_MED)
						{
							ColourCount++;
							PauseTimer = 0;						
						}
						break;
					}
					case 6:	  					// Increment Red
					{
						if(RedDutyTarget < LED_MAX)
						{
							RedDutyTarget++;
						}
						if(RedDutyTarget == LED_MAX)
						{
							ColourCount++;
						}
						break;	
					}
					case 7:	  					// Increment Green
					{
						if(GreenDutyTarget < LED_MAX)
						{
							GreenDutyTarget++;
						}
						if(GreenDutyTarget == LED_MAX)
						{
							ColourCount++;
						}
						break;
					}
					case 8:	  					// Decrement blue and Green
					{
						if(BlueDutyTarget > 0)
						{
							BlueDutyTarget--;
						}
						if(GreenDutyTarget > 0)
						{
							GreenDutyTarget--;
						}
						if((BlueDutyTarget == 0) && (GreenDutyTarget == 0))
						{
							ColourCount = 1;
						}
						break;	
					}
					default:
					{
						ColourCount = 0;
					}
				}
			}
			
			if ((Events.release & SW_PWR) && BtnPressFlag)
			{	
				g_timer_button_release = 250;			
				BtnHoldFlag = 0;									
				BtnPressFlag = 0;
				LedPowerTog = 1;						// flag change
				LedPower ^= 0x01;	                	// toggle power (flip bit 1)
				RamFlashBlock.PowerState = LedPower;								
				// If toggled from on to off then delay till off before save to flash
				// avoids flicker
				if(!LedPower)
				{
					SaveFlashTimer = DELAY_ONE;			// delay one second before save
				}
				else
				{
					SaveFlashTimer = 0;			
				}
				WriteFlashFlag = 1;
			}
			
			if ((Events.release & SW_PWR) && BtnRateChangeFlag)
			{
				BtnRateChangeFlag = 0;						
				SaveFlashTimer = 0;						// delay one second before save
				WriteFlashFlag = 1;
				LedPower = 1;	                		 // power on
				RamFlashBlock.PowerState = LedPower;				
				RamFlashBlock.ledChangeRate = ledChangeRate;
			}
			
			if ((Events.release & SW_PWR) && BtnHoldFlag)
			{
				BtnHoldFlag = 0;						
				SaveFlashTimer = 0;						// delay one second before save
				WriteFlashFlag = 1;
				RamFlashBlock.ColourCount = ColourCount;
				LedPower = 1;	                		 // power on
				RamFlashBlock.PowerState = LedPower;				
				RedDutyMax = RedDutyTarget;
				RamFlashBlock.RedDuty = RedDutyTarget;
				BlueDutyMax = BlueDutyTarget;
				RamFlashBlock.BlueDuty = BlueDutyTarget;
				GreenDutyMax = GreenDutyTarget;
				RamFlashBlock.GreenDuty = GreenDutyTarget;	
			}		
		}
	}
}
 
/************************************************************************
*  PWM8_ISR
*  Interrupt Period: 4ms 250Hz
*  Parameters: none  
*  Returns:    none 
************************************************************************/
void PWM8_ISR(void)
{
	PWM8_Stop();	

	// There are 6 special cases of colour display where the dutycyle is
	// no longer 1/3 RGB, the first three cases the dutycycle is unity while 
	// the last three are 1/2 dutycyle
	// 1. Red
	// 2. Green
	// 3. Blue
	// 4. Red/Green 
	// 5. Red/Blue 
	// 6. Blue/Green 
	
	
	// 1. Red
	if(RedDuty && !GreenDuty && !BlueDuty)
	{
		MultiplexCount = 0;				// Override multiplex = red/red/red
		if(!RedUpFlag)			    	// First time for 100% red case
		{
			RedUpFlag = 1;	
			if(RedBlueUpFlag || RedGreenUpFlag)
			{
				//RedDuty = RedDuty/2;		// We are currently at 1/2 duty
			}
			else
			{
				//RedDuty = RedDuty/3;		// We are currently at 1/3 duty
			}
		}
	}
	else
	{
		RedUpFlag = 0;					// Clear flag on exit
	}
	// 2. Green
	if(!RedDuty && GreenDuty && !BlueDuty) 	
	{
		MultiplexCount = 1;				// Override multiplex green	
		if(!GreenUpFlag)			    	// First time for 100% Green case
		{
			GreenUpFlag = 1;	
			if(GreenBlueUpFlag || RedGreenUpFlag)
			{
				//GreenDuty = GreenDuty/2;		// We are currently at 1/2 duty
			}
			else
			{
				//GreenDuty = GreenDuty/3;		// We are currently at 1/3 duty
			}
		}
	}
	else
	{
		GreenUpFlag = 0;				// Clear flag on exit	
	}
	// 3. Blue
	if(!RedDuty && !GreenDuty && BlueDuty) 	
	{
		MultiplexCount = 2;				// Override multiplex glue	
		if(!BlueUpFlag)			    	// First time for 100% Blue case
		{
			BlueUpFlag = 1;	
			if(RedBlueUpFlag || GreenBlueUpFlag)
			{
				//BlueDuty = BlueDuty/2;		// We are currently at 1/2 duty
			}
			else
			{
				//BlueDuty = BlueDuty/3;		// We are currently at 1/3 duty
			}
		}
	}
	else
	{
		BlueUpFlag = 0;					// Clear flag on exit
	}
	
	// 4. Red/Green
	if(RedDuty && GreenDuty && !BlueDuty) 
	{
		if(MultiplexCount)
		{
			MultiplexCount = 0;			// Override multiplex red
		}
		else
		{
			MultiplexCount = 1;			// Override multiplex green
		}
		if(!RedGreenUpFlag)					
		{
			//RedDuty = RedDuty/2;		// Reduce to 1/2 duty
			//GreenDuty = GreenDuty/2;	// Reduce to 1/2 duty	
			RedGreenUpFlag = 1;			// Clear flag on exit
		}
	}
	else
	{
		RedGreenUpFlag = 0;				// 	
	}		
	
	// 5. Red/Blue
	if(RedDuty && !GreenDuty && BlueDuty) 
	{
		if(MultiplexCount)
		{
			MultiplexCount = 0;			// Override multiplex red
		}
		else
		{
			MultiplexCount = 2;			// Override multiplex Blue
		}
		if(!RedBlueUpFlag)					
		{
			//RedDuty = RedDuty/2;		// Reduce to 1/2 duty
			//BlueDuty = BlueDuty/2;		// Reduce to 1/2 duty	
			RedBlueUpFlag = 1;			// 
		}
	}
	else
	{
		RedBlueUpFlag = 0;				// 
	}	
	
	// 6. Green/Blue
	if(!RedDuty && GreenDuty && BlueDuty) 
	{
		if(MultiplexCount == 1)
		{
			MultiplexCount = 2;			// Override multiplex Blue
		}
		else
		{
			MultiplexCount = 1;			// Override multiplex Green
		}
		if(!GreenBlueUpFlag)					
		{
			//GreenDuty = GreenDuty/2;	// Reduce to 1/2 duty
			//BlueDuty = BlueDuty/2;		// Reduce to 1/2 duty	
			GreenBlueUpFlag = 1;		// 
		}
	}
	else
	{
		GreenBlueUpFlag = 0;			// 
	}		
	// Red/Green/Blue
	if(RedDuty && GreenDuty && BlueDuty)								
	{
		if(MultiplexCount < 2)			// count 0, 1, 2, 0, 1, 2 ...
		{
			MultiplexCount++;
		}			
		else
		{
			MultiplexCount = 0;
		}
	}	
	
	switch(MultiplexCount)          // Multiplex LED                     
	{		
		case 0:	
		{
			PWM8_WritePulseWidth(RedDuty);
			GREEN_Switch(0);
			BLUE_Switch(0);	
			RED_Switch(1);
			break;
		}
		case 1:	  
		{
			PWM8_WritePulseWidth(GreenDuty);
			RED_Switch(0);
			GREEN_Switch(1);
			BLUE_Switch(0);
			break;		
		}
		case 2:
		{
			PWM8_WritePulseWidth(BlueDuty);
			GREEN_Switch(0);
			BLUE_Switch(1);	
			RED_Switch(0);
			break;		
		}
		default:
		{
			PWM8_WritePulseWidth(0);		
		}
	}		
	PWM8_Start();			
}

/************************************************************************
*  USB interface
*  
*  Parameters: none  
*  Returns:    none 
*  
*  4 bytes Module -> PC:
*  
*  char 0 - sensor and button inputs as defined below
*     bit 0 - power button
*     bit 1 - spare
*     bit 2 - spare
*     bit 3 - spare
*     bit 4 - spare
*     bit 5 - spare
*     bit 6 - USB connected (of no use to software as it will always be set)
*  char 1 - Red dutycyle
*  char 2 - Green dutycyle
*  char 3 - Blue dutycyle
*  
*  4 bytes PC  -> Module:
*  
*  char 0 - control bits defined below
*     bit 0 - spare
*     bit 1 - spare
*     bit 2 - LED enable bit
*     bit 3 - toggles to indicate software running (if this stops toggling then hardware reverts to auto)
*     bit 4 - spare
*     bit 5 - spare
*     bit 6 - spare
*     bit 7 - Boot entry flag
*  char 1 - Red dutycycle
*  char 2 - Green dutycycle
*  char 3 - Blue dutycycle
*  
************************************************************************/

void CommunicateUSB(void)
{
    if (SOF_Flag) 
    {
		char temp;
	
        // Arrive here every 1 msec
		SOF_Flag = 0;
		
        // send data to PC
		buttons_report[0] = debounced_state;        
        buttons_report[1] = RedDuty;
        buttons_report[2] = GreenDuty;
        buttons_report[3] = BlueDuty;
        buttons_report[4] = Temperature;
        buttons_report[5] = Temperature+1;
        buttons_report[6] = Temperature+2;
        buttons_report[7] = Temperature+3;

		USB_LoadInEP(1, &buttons_report[0], 8, USB_TOGGLE);
        		
		// receive data from PC
		lights_report = USB_INTERFACE_0_OUT_RPT_DATA[0]; // Control input from PC
		RedDutyMax = USB_INTERFACE_0_OUT_RPT_DATA[1];    // Red brightness from PC
		GreenDutyMax = USB_INTERFACE_0_OUT_RPT_DATA[2];   // Green brightness from PC
		BlueDutyMax = USB_INTERFACE_0_OUT_RPT_DATA[3];  // Blue brightness from PC		 
		temp = USB_INTERFACE_0_OUT_RPT_DATA[4]; // 
		temp = USB_INTERFACE_0_OUT_RPT_DATA[5];    // 
		temp = USB_INTERFACE_0_OUT_RPT_DATA[6];   // 
		temp = USB_INTERFACE_0_OUT_RPT_DATA[7];  // 		 
		
		if(USB_Tog != (lights_report & USB_AUTO_MASK))  // USB Auto status bit toggles
		{												// if software is running
			USB_Tog = (lights_report & USB_AUTO_MASK); 	// mask for auto bit
			USBTimer = 255;								// reset watchdog counter
		}
			
		if(USBTimer)
		{
			Auto = 0;							// PC is in control
						
			if(lights_report & USB_BOOT_MASK)   // USB Boot status bit
			{
				// Stop everything
				USB_Stop();                     
				Timer8_Stop();
				PRS8_Stop();						
				RED_Stop();
				GREEN_Stop();
				BLUE_Stop();
				PWM8_Stop();						
				PGA_Stop();           
				ADCINC_Stop();  
				M8C_DisableGInt;			    // Global interrupt disable

				USB_EnterBootloader();          
			}
			
			if(lights_report & USB_LED_MASK)    // USB LED status bit        
			{
                LedPower = 1;					// turn LEDs on
                LedPowerTog = 1;               	// flag change			
			}
			else
			{
                LedPower = 0;					// turn LEDs on
                LedPowerTog = 1;               	// flag change			
			}
			
			PRT2DR = Port_2_Data_SHADE;         // update port2        
		
		}
		else
		{
			Auto = 1;							// auto (software not running)
			RedDutyMax = RamFlashBlock.RedDuty;	// reset dutycycle
			BlueDutyMax = RamFlashBlock.BlueDuty;
			GreenDutyMax = RamFlashBlock.GreenDuty;			
		}
    }      
}   

/************************************************************************
*  MeasureTemperature read temperature sensor voltage
*  
*  Parameters: none  
*  Returns:    none 
************************************************************************/
void MeasureTemperature(void)
{
    if (ADCINC_fIsDataAvailable()) 
    {    	
		Temperature = ADCINC_iClearFlagGetData();  // Clear ADC flag and get data
		ADCINC_GetSamples(1);                 	// start the next sample
    }
}
    
/************************************************************************
*  GetSample temperature sensor voltage
*  
*  Parameters: bChannel  
*  Returns:    iADC_Data 
************************************************************************/
/*
int GetSample(char bChannel)
{   
int ADC_Data = 0;

    ADC_Data = ADCINC_iClearFlagGetData();  // Clear ADC flag and get data
    MUX_CR0 = 0x00;	                        // disconnect all port 0 pins
	switch(bChannel)                        // select a new pin                     
	{
		case 0:	                            // for voltage
			MUX_CR0 |= 0x08; 	            // Connect P0[3]
			break;
		case 1:	                            // for temperature
			MUX_CR0 |= 0x20;	            // connect P0[5]
			break;
		default:
			break;
	}                                                                                   
	ADCINC_GetSamples(1);                 	// read the connected pin
 
	return ADC_Data;
}
*/

/************************************************************************
*  Timer8_ISR
*  Interrupt Period: 1ms/1KHz
*  Parameters: none  
*  Returns:    none 
************************************************************************/
void Timer8_ISR(void)
{
// Button debounce counter
    if(ButtonTimer > 0)
    {
        ButtonTimer--;
    }	
// Button hold counter
	if(ButtonHoldTimer > 0)
	{
		ButtonHoldTimer--;
	} 
// Button release counter
	if(g_timer_button_release > 0)
	{
		g_timer_button_release--;
	}	
// Save to flash counter
    if(SaveFlashTimer > 0)
    {
        SaveFlashTimer--;
    }		
// LED pulse patter counter 
    if(LedTimer > 0)
    {
        LedTimer--;
    } 	
// LED counter for smooth LED dutycyle transition 
	if(DutyTimer > 0)
	{
		DutyTimer--;
	} 	
// USB control delay 
    if(USBTimer > 0)
    {
        USBTimer--;
    }			
}

