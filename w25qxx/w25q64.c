
#include "w25q64.h"

const uint16_t w25q[] = {0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0200, 0x0400};
const uint16_t PageSize = 0x0100; // 256
const uint16_t SectorSize = 0x1000; // 4096
const uint16_t Block32KSize = 0x8000; // 32768
const uint32_t BlockSize = 0x00010000; // 65536

w25qxx_t	w25qxx;

// -------------------------------------------------------------  
bool Init_SPI_W25Qxx(void)
{
  bool ret = false;
  GPIO_InitTypeDef gpio_spi_1;
  // MOSI 
  gpio_spi_1.GPIO_Pin = GPIO_Pin_7;
  gpio_spi_1.GPIO_Mode = GPIO_Mode_AF_PP;
  gpio_spi_1.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &gpio_spi_1);  
  GPIO_PinLockConfig(GPIOA, GPIO_Pin_7);
  // MISO
  gpio_spi_1.GPIO_Pin = GPIO_Pin_6;
  gpio_spi_1.GPIO_Mode = GPIO_Mode_AF_PP;
  gpio_spi_1.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &gpio_spi_1);  
  GPIO_PinLockConfig(GPIOA, GPIO_Pin_6);
  // SCK
  gpio_spi_1.GPIO_Pin = GPIO_Pin_5;
  gpio_spi_1.GPIO_Mode = GPIO_Mode_AF_PP;
  gpio_spi_1.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &gpio_spi_1);  
  GPIO_PinLockConfig(GPIOA, GPIO_Pin_5);
  // NSS
  gpio_spi_1.GPIO_Pin = GPIO_Pin_4;
  gpio_spi_1.GPIO_Mode = GPIO_Mode_Out_PP;
//  gpio_spi_1.GPIO_Mode = GPIO_Mode_AF_PP;
  gpio_spi_1.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &gpio_spi_1);  
  GPIO_PinLockConfig(GPIOA, GPIO_Pin_4);
  GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_SET);
  // SPI
  SPI_InitTypeDef spi_1;
  spi_1.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  spi_1.SPI_Mode = SPI_Mode_Master;
  spi_1.SPI_DataSize = SPI_DataSize_8b;
  spi_1.SPI_CPOL = SPI_CPOL_Low;
  spi_1.SPI_CPHA = SPI_CPHA_1Edge;
  spi_1.SPI_NSS = SPI_NSS_Soft;
  spi_1.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
  spi_1.SPI_FirstBit = SPI_FirstBit_MSB;
  spi_1.SPI_CRCPolynomial = 0;
  SPI_Init(SPI1, &spi_1);
  //SPI_SSOutputCmd(SPI1, ENABLE);
  SPI_Cmd(SPI1, ENABLE);

  // DMA Receive from SPI
  DMA_InitTypeDef dma_spi_rx;
  dma_spi_rx.DMA_PeripheralBaseAddr = (int)&SPI1->DR;
  dma_spi_rx.DMA_MemoryBaseAddr = (int)&dataBuffer;
  dma_spi_rx.DMA_DIR = DMA_DIR_PeripheralSRC;
  dma_spi_rx.DMA_BufferSize = 0;
  dma_spi_rx.DMA_PeripheralInc = DMA_MemoryInc_Disable;
  dma_spi_rx.DMA_MemoryInc = DMA_MemoryInc_Enable;
  dma_spi_rx.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  dma_spi_rx.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  dma_spi_rx.DMA_Mode = DMA_Mode_Normal;
  dma_spi_rx.DMA_Priority = DMA_Priority_High;
  dma_spi_rx.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel2, &dma_spi_rx); 
//  DMA_Cmd(DMA1_Channel2, ENABLE);

  // DMA Transmit to SPI
  DMA_InitTypeDef dma_spi_tx;
  dma_spi_tx.DMA_PeripheralBaseAddr = (int)&SPI1->DR;
  dma_spi_tx.DMA_MemoryBaseAddr = (int)&dataBuffer;
  dma_spi_tx.DMA_DIR = DMA_DIR_PeripheralDST;
  dma_spi_tx.DMA_BufferSize = 0;
  dma_spi_tx.DMA_PeripheralInc = DMA_MemoryInc_Disable;
  dma_spi_tx.DMA_MemoryInc = DMA_MemoryInc_Enable;
  dma_spi_tx.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  dma_spi_tx.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  dma_spi_tx.DMA_Priority = DMA_Priority_High;
  dma_spi_tx.DMA_Mode = DMA_Mode_Normal;
  dma_spi_tx.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel3, &dma_spi_tx);
  //DMA_Cmd(DMA1_Channel3, ENABLE);

  ret = true;
  return ret;
}




// -------------------------------------------------------------  
void W25Qxx_TransferDMASPI (uint32_t __bytes, DataDirectionTypeDef __dir, uint32_t __offset){
  SetFlag(&_EREG_, _DBLF_, FLAG_SET);
 
  uint8_t pump = 0;
  if (__dir == WRITE)
  {
    SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);
    DMA1_Channel3->CMAR = ((int)&dataBuffer) + (__offset * 256);
    DMA_SetCurrDataCounter(DMA1_Channel3, __bytes);
    DMA_Cmd(DMA1_Channel3, ENABLE);
  }
  else
  {
    SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);
    SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Rx, ENABLE);
    DMA1_Channel3->CMAR = (int)&pump;
    DMA_SetCurrDataCounter(DMA1_Channel3, __bytes);
    DMA_SetCurrDataCounter(DMA1_Channel2, __bytes);
    DMA_Cmd(DMA1_Channel3, ENABLE);
    DMA_Cmd(DMA1_Channel2, ENABLE);
  }

  while (!DMA_GetFlagStatus(DMA1_FLAG_TC3));
  DMA_ClearFlag(DMA1_FLAG_TC3);
  
  if (__dir == WRITE)
  {
    while (!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));
    SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, DISABLE);
    DMA_Cmd(DMA1_Channel3, DISABLE);
  }
  else
  {
    while (!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));
    SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, DISABLE);
    SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Rx, DISABLE);
    DMA_Cmd(DMA1_Channel3, DISABLE);
    DMA_Cmd(DMA1_Channel2, DISABLE);
  }

  SetFlag(&_EREG_, _DBLF_, FLAG_SET);
}




// -------------------------------------------------------------  
bool W25Qxx_TransferSPI (uint8_t __command, int32_t __address, uint16_t __bytes, DataDirectionTypeDef __dir, uint32_t __offset)
{
  GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_RESET);
  
  SPI_I2S_SendData(SPI1, __command);
  while (!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));
  SPI_I2S_ReceiveData(SPI1);
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));

  if (__address >= 0)
  {
    int i = 4;
    while (--i)
    {
      SPI_I2S_SendData(SPI1, __address >> 8 * i);
      while (!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));
      SPI_I2S_ReceiveData(SPI1);
      while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));
    }
  }
  
  if (__command == FastRead)
  {
    SPI_I2S_SendData(SPI1, 0);
    while (!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));
    SPI_I2S_ReceiveData(SPI1);
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));
  }
  
  if (__bytes)
  {
    W25Qxx_TransferDMASPI(__bytes, __dir, __offset);
  }
    
  GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_SET);
  return true;
}




// -------------------------------------------------------------  
void W25qxx_Init (void)
{
  W25Qxx_TransferSPI(Read_JedecID, -1, 4, READ, 0);
  w25qxx.ID = __rev(dataBuffer[0]) >> 8;
  
  W25Qxx_TransferSPI(Read_UniqueID, -1, 12, READ, 0);
  w25qxx.UniqID[0] = __rev(dataBuffer[1]);
  w25qxx.UniqID[1] = __rev(dataBuffer[2]);

  W25Qxx_TransferSPI(Read_DeviceID, -1, 4, READ, 0);
  w25qxx.BlockCount = w25q[(__rev(dataBuffer[0]) & 0x0f)];  
  w25qxx.Capacity = w25qxx.BlockCount * BlockSize;
  
  SetFlag(&_EREG_, _DBLF_, FLAG_CLEAR);
}




// -------------------------------------------------------------  
void W25qxx_Read (uint32_t __address, uint32_t __bytes)
{
  uint32_t phy_addr = 0;
  W25qxx_IsBusy();
  
  phy_addr = BlockSize * ((__address >> 8) & 0xffff);
  phy_addr += SectorSize * ((__address >> 4) & 0xf);
  phy_addr += PageSize * (__address & 0xf);
  
  W25Qxx_TransferSPI(FastRead, (phy_addr << 8), __bytes, READ, 0);  
}





// -------------------------------------------------------------  
void W25qxx_Write (uint32_t __address, uint32_t __bytes)
{
  uint32_t phy_addr = 0;
  W25qxx_IsBusy();
  
  phy_addr = BlockSize * ((__address >> 8) & 0xffff);
  phy_addr += SectorSize * ((__address >> 4) & 0xf);
  phy_addr += PageSize * (__address & 0xf);
  int i;
  for (i = 0; i < __bytes/256; i++)
  {
    W25Qxx_TransferSPI(WriteEnable, -1, 0, NEUTRAL, 0);
    W25Qxx_TransferSPI(PageProgram, (phy_addr << 8), 256, WRITE, i);  
    phy_addr += PageSize;
    W25qxx_IsBusy();
  }
  
  if (__bytes%256)
  {
    W25Qxx_TransferSPI(WriteEnable, -1, 0, NEUTRAL, 0);
    W25Qxx_TransferSPI(PageProgram, (phy_addr << 8), __bytes%256, WRITE, i);  
    W25qxx_IsBusy();
  }
}





// -------------------------------------------------------------  
void W25qxx_Erase (uint32_t __address, uint32_t __sectors)
{
  
  uint32_t phy_addr = 0;
  phy_addr = BlockSize * ((__address >> 8) & 0xffff);
  
  if (__sectors > 8)
  {
    if (__sectors > 16)
    {
      // *******************************  
      uint32_t blocks = __sectors/16;
      for (int i = 0; i < blocks; i++)
      {
        W25Qxx_TransferSPI(WriteEnable, -1, 0, NEUTRAL, 0);
        W25Qxx_TransferSPI(Erase_Block_64, (phy_addr << 8), 0, NEUTRAL, 0);
        phy_addr += BlockSize;
        W25qxx_IsBusy();
      }
      uint32_t remain_addr = (__address & 0xffffff00) + (blocks << 8);
      if (__sectors%16)
      {
        W25qxx_Erase(remain_addr, __sectors%16);
      }
      // *******************************  
    }
    else
    {
      // ******************************* 
      if (((__address >> 4) & 0xf) > 8)
      {
        phy_addr += SectorSize * ((__address >> 4) & 0xf);
      }
      W25Qxx_TransferSPI(WriteEnable, -1, 0, NEUTRAL, 0);
      W25Qxx_TransferSPI(Erase_Block_32, (phy_addr << 8), 0, NEUTRAL, 0);
      uint32_t remain_addr = (__address & 0xffffff80) + (8 << 4);
      W25qxx_IsBusy();
      
      if (__sectors - 8)
      {
        W25qxx_Erase(remain_addr, __sectors - 8);
      }
    // *******************************  
    }
  }    
  else
  {
    // *******************************  
    phy_addr += SectorSize * ((__address >> 4) & 0xf);
    for (int i = 0; i < __sectors; i++)
    {
      W25Qxx_TransferSPI(WriteEnable, -1, 0, NEUTRAL, 0);
      W25Qxx_TransferSPI(Erase_Sector, (phy_addr << 8), 0, NEUTRAL, 0);
      phy_addr += SectorSize;
      W25qxx_IsBusy();
    }
    // *******************************  
  }
}





void W25qxx_IsBusy ()
{
  uint32_t tmp = dataBuffer[0];
  dataBuffer[0] = _BUSY_;
  while (dataBuffer[0] & 0x1)
  {
    W25Qxx_TransferSPI(Read_StatusRegister_1, -1, 1, READ, 0);
  }
  dataBuffer[0] = tmp;
}



//  *dataBuffer = _BP0_ | _BP1_ | _TB_;
//  W25Qxx_TransferSPI(Write_StatusRegister, 0, 2, WRITE);
//  W25Qxx_TransferSPI(Write_StatusRegister_1, 0, 1, READ);
//  W25Qxx_TransferSPI(Write_StatusRegister_2, 0, 1, READ);

