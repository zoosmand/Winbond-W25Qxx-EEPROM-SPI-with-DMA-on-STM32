#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
  extern "C" {
#endif 
    
#ifndef STM32F10X_HD
#define STM32F10X_HD
#endif 


//#include "stdio.h"
#include <stdbool.h>
#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "utils.h"
#include "w25q64.h"

extern void Delay_Handler(void);


    // _EREG_ Flags    
#define _BT7F_  0 // Basic Timer7 Flag
#define _ALF_   1 // Alarm Flag
#define _RDF_   2 // Run Display Flag
#define _DDF_   3 // Display Delay Flag
#define _DBLF_  4 // Data Buffer is Locked Flag


#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
