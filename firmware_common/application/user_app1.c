/**********************************************************************************************************************
File: user_app1.c                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app1 as a template:
 1. Copy both user_app1.c and user_app1.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "user_app1" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "USER_APP1" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

Description:
This is a user_app1.c file template 

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:


Protected System functions:
void UserApp1Initialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void UserApp1RunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_UserApp1"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags;                       /* Global state flags */
LedNumberType G_LedPattern[100];

/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
//static u32 UserApp1_u32Timeout;                      /* Timeout counter used across states */


/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------
Function: UserApp1Initialize

Description:
Initializes the State Machine and its variables.

Requires:
  -

Promises:
  - 
*/
void UserApp1Initialize(void)
{
  // initialize all LEDs to off
  LedOff(WHITE);
  LedOff(PURPLE);
  LedOff(BLUE);
  LedOff(CYAN);
  LedOff(GREEN);
  LedOff(YELLOW);
  LedOff(ORANGE);
  LedOff(RED);
  
  // choose a random sequence for the led pattern
  for(int i = 0; i < 100; i++)
  {
    G_LedPattern[i] = ( (rand() % 4) * 2 ) - 1;
  }
  
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp1_StateMachine = UserApp1SM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp1_StateMachine = UserApp1SM_Error;
  }

} /* end UserApp1Initialize() */

  
/*----------------------------------------------------------------------------------------------------------------------
Function UserApp1RunActiveState()

Description:
Selects and runs one iteration of the current state in the state machine.
All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
  - State machine function pointer points at current state

Promises:
  - Calls the function to pointed by the state machine function pointer
*/
void UserApp1RunActiveState(void)
{
  UserApp1_StateMachine();

} /* end UserApp1RunActiveState */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */
static void UserApp1SM_Idle(void)
{
    static u16 u16Timer = 0;
    static u16 u16PatternIndex = 0; // keeps track of which index the LED pattern is at
    static u16 u16UserInputIndex = 0; // keeps track of the index of the led pattern that the user is at
    static u16 u16Level = 0; // the number of colors in the pattern on certain loop
    static GameState currentState = COMPUTER_PLAY;
    
    u16Timer++;
    
    // if state == COMPUTER_PLAY, the computer will pay the LED pattern
    // up to the current level
    if(currentState == COMPUTER_PLAY)
    {
      if(u16Timer == 1)
      {
        // reset all LEDs to off
        LedOff(WHITE);
        LedOff(PURPLE);
        LedOff(BLUE);
        LedOff(CYAN);
        LedOff(GREEN);
        LedOff(YELLOW);
        LedOff(ORANGE);
        LedOff(RED);
      
        LedOn(G_LedPattern[u16PatternIndex]);
        u16PatternIndex++;
        
        if(u16PatternIndex > u16Level)
        {
          currentState = USER_PLAY;
          u16Timer = 0;
        }
      }
      else if(u16Timer == 501)
      {
        // turn off all LEDs
        LedOff(WHITE);
        LedOff(PURPLE);
        LedOff(BLUE);
        LedOff(CYAN);
        LedOff(GREEN);
        LedOff(YELLOW);
        LedOff(ORANGE);
        LedOff(RED);
      }
      else if(u16Timer == 801)
      {
        u16Timer = 0;
      }
    }
    
    if(currentState == USER_PLAY)
    {
      if(u16Timer == 501)
      {
        // turn off all LEDs
        LedOff(WHITE);
        LedOff(PURPLE);
        LedOff(BLUE);
        LedOff(CYAN);
        LedOff(GREEN);
        LedOff(YELLOW);
        LedOff(ORANGE);
        LedOff(RED);
      }
      if(u16Timer > 501)
      {
        // check for button 0
        if(WasButtonPressed(BUTTON0))
        {
          ButtonAcknowledge(BUTTON0);
          // if the user entered the correct pattern item
          if(G_LedPattern[u16UserInputIndex] == 1)
          {
            u16UserInputIndex++;
            if(u16UserInputIndex > u16Level)
            {
              currentState = USER_SUCCESS;
              u16Timer = 0;
            }
          }
          // if the user entered the incorrect pattern item
          else
          {
            currentState = USER_FAIL;
            u16Timer = 0;
          }
        }
        
        // check for button 1
        else if(WasButtonPressed(BUTTON1))
        {
          ButtonAcknowledge(BUTTON1);
          if(G_LedPattern[u16UserInputIndex] == 3)
          {
            u16UserInputIndex++;
            if(u16UserInputIndex > u16Level)
            {
              currentState = USER_SUCCESS;
              u16Timer = 0;
            }
          }
          // if the user entered the incorrect pattern item
          else
          {
            currentState = USER_FAIL;
            u16Timer = 0;
          }
        }
        
        // check for button 2
        else if(WasButtonPressed(BUTTON2))
        {
          ButtonAcknowledge(BUTTON2);
          if(G_LedPattern[u16UserInputIndex] == 5)
          {
            u16UserInputIndex++;
            if(u16UserInputIndex > u16Level)
            {
              currentState = USER_SUCCESS;
              u16Timer = 0;
            }
          }
          // if the user entered the incorrect pattern item
          else
          {
            currentState = USER_FAIL;
            u16Timer = 0;
          }
        }
        
        // check for button 3
        else if(WasButtonPressed(BUTTON3))
        {
          ButtonAcknowledge(BUTTON3);
          if(G_LedPattern[u16UserInputIndex] == 7)
          {
            u16UserInputIndex++;
            if(u16UserInputIndex > u16Level)
            {
              currentState = USER_SUCCESS;
              u16Timer = 0;
            }
          }
          // if the user entered the incorrect pattern item
          else
          {
            currentState = USER_FAIL;
            u16Timer = 0;
          }
          
        }
        
        // run PURPLE LED in accordance with BUTTON0
        if(IsButtonPressed(BUTTON0))
        {
          LedOn(PURPLE);
        }
        else
        {
          LedOff(PURPLE);
        }
        
        // run CYAN LED in accordance with BUTTON1
        if(IsButtonPressed(BUTTON1))
        {
          LedOn(CYAN);
        }
        else
        {
          LedOff(CYAN);
        }
        
        // run YELLOW LED in accordance with BUTTON2
        if(IsButtonPressed(BUTTON2))
        {
          LedOn(YELLOW);
        }
        else
        {
          LedOff(YELLOW);
        }
        
        // run RED LED in accordance with BUTTON3
        if(IsButtonPressed(BUTTON3))
        {
          LedOn(RED);
        }
        else
        {
          LedOff(RED);
        }
        
      }
      
      if(currentState == USER_FAIL)
      {
        if(u16Timer == 0)
        {
          LedOn(WHITE);
          LedOn(PURPLE);
          LedOn(BLUE);
          LedOn(CYAN);
          LedOn(GREEN);
          LedOn(YELLOW);
          LedOn(ORANGE);
          LedOn(RED);
        }
        u16Timer++;
        if(u16Timer == 501)
        {
          LedOff(WHITE);
          LedOff(PURPLE);
          LedOff(BLUE);
          LedOff(CYAN);
          LedOff(GREEN);
          LedOff(YELLOW);
          LedOff(ORANGE);
          LedOff(RED);
        }
        
        if(u16Timer == 801)
        {
          u16Level++;
          currentState = COMPUTER_PLAY;
        }
      }
      
      if(currentState == USER_SUCCESS)
      {
        if(u16Timer == 0)
        {
          LedOn(WHITE);
          LedOn(PURPLE);
          LedOn(BLUE);
          LedOn(CYAN);
          LedOn(GREEN);
          LedOn(YELLOW);
          LedOn(ORANGE);
          LedOn(RED);
        }
        
        while(1);

      }
    }
    
} /* end UserApp1SM_Idle() */
    

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
