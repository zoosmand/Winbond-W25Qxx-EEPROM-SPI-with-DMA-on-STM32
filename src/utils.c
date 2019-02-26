#include "utils.h"

uint32_t delay_tmp = 0;
char* message;
uint32_t dataBuffer[1024];


FlagStateTypeDef GetFlag(uint32_t* __registry, uint8_t __flag)
{
  return (*__registry & (1 << __flag)) ? FLAG_SET : FLAG_CLEAR;
}

void SetFlag(uint32_t* __registry, uint8_t __flag, FlagStateTypeDef __state)
{
  if (__state == FLAG_SET)
  {
    *__registry |= (1 << __flag);
  }
  else if (__state == FLAG_CLEAR)
  {
    *__registry &= ~(1 << __flag);
  }
  else
  {
    Error_Handler("Flag Error!");
  }
}



void Error_Handler(char* __error_message)
{
  //printf(__error_message);

  while(1)
  {
  }
}


void Display_Handler(DisplayTypeTypeDef __dt)
{
  switch (__dt)
  {
  	case TM1637:
      //TM1637_Run();
      Error_Handler("TM1637 Error!");
  		break;
  	case TM1803:
      //TM1803_Run();
      Error_Handler("TM1803 Error!");
  		break;
  	default:
      Error_Handler("Error!");
  		break;
  }
}




void Delay(uint32_t __ms_delay)
{
//  if(GetFlag(&_EREG_, _DDF_))
//  {
//    delay_tmp = sysQuantum + __us_delay;
//  }
  delay_tmp = sysQuantum + __ms_delay * 100;
}
