#ifndef __UTILS_H
#define __UTILS_H

#ifdef __cplusplus
  extern "C" {
#endif 

#include "main.h"

typedef enum
{
  TM1637 = 0,
  TM1803 = 1
} DisplayTypeTypeDef;

typedef enum
{
  FLAG_SET = 1,
  FLAG_CLEAR = 0
} FlagStateTypeDef;

typedef enum
{
  NEUTRAL = 2,
  READ = 1,
  WRITE = 0
} DataDirectionTypeDef;

typedef enum
{
  ERR_0 = 0, 
  ERR_1 = 1, 
  ERR_2 = 2, 
  ERR_3 = 3, 
  ERR_4 = 4,
  ERR_5 = 5,
  ERR_6 = 6,
  ERR_7 = 7,
  ERR_8 = 8,
  ERR_9 = 9,
  NO_ERR = 0xff
} ErrorState;


extern uint32_t delay_tmp;
extern uint32_t sysQuantum;
extern char* message;
extern uint32_t dataBuffer[1024];

void Error_Handler(char*);
FlagStateTypeDef GetFlag(uint32_t*, uint8_t);
void SetFlag(uint32_t*, uint8_t, FlagStateTypeDef);
void Display_Handler(DisplayTypeTypeDef);

void Delay(uint32_t);



#ifdef __cplusplus
}
#endif

#endif /* __UTILS_H */
