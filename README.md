# SPI_W25QXX
A project to use Winbond EEPROM W25Qxx (W25Q64 in case) on SPI transport with DMA in STM32F103RE microcontroller. Commands transfer with the normal SPI cycles while the data travels by DMA bus.

To provide an accurate short delay, SysTick clang each 10us.
Furthermore, in the main cycle, the cron handler is realized, but it has a tiny bug that I did not overwhelm yet.
The RTC and independent watchdog are triggered.

// Address Pattern

// 0x00BBBBSP

// bits 3..0 - number of page - P

// bits 7..4 - number of sector - S

// bits 23..8 - number of block - B

A test procedure in a handler of basic timer lets to check read and write EEPROM filling the data buffer with CRC calculations. A page overload of writing data displays a technique of the data manipulating.

Remember that the EEPROM has to be erased before writing even a byte, herewith the smallest part affected to erase is 4Kb (1 sector). So, if an only byte is needed to store it holds the whole sector.

The erasing function uses the same address parameter as the write/read ones, but it does not take to account bits 4..0 LSB of a page address.

If more than 8/16 sectors are affected by erasing, the erase block command is used. In this case, sector address bits 7..4 LBS are ignored. It means an address parameter is rounded to a block address.
