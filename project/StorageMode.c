#include "StorageMode.h"

#define STORAGE_LUN_NBR 1

const int8_t STORAGE_Inquirydata[] = 
{
  0x00, 0x80, 0x02, 0x02,
  (USBD_STD_INQUIRY_LENGTH - 5),
  0x00, 0x00, 0x00,
  'A', 'N', 'Y', 'B', 'O', 'D', 'Y', ' ', // Manufacturer : 8 bytes
  'E', 'M', 'F', 'A', 'T', ' ', 's', 't', // Product      : 16 Bytes
  'o', 'r', 'a', 'g', 'e', ' ', ' ', ' ', //
  ' ', ' ', ' ' ,' ',                     // Version      : 4 Bytes
};

int8_t STORAGE_Init(uint8_t lun)
{
	return 0;
}

int8_t STORAGE_GetCapacity(uint8_t lun, uint32_t *block_num, uint32_t *block_size)
{
  *block_size = 512;
  *block_num = emfat.disk_sectors;
  return 0;
}

int8_t STORAGE_IsReady(uint8_t lun)
{
  return 0;
}

int8_t STORAGE_IsWriteProtected(uint8_t lun)
{
	return 1;
}

int8_t STORAGE_Read(
	uint8_t lun,        // logical unit number
	uint8_t *buf,       // Pointer to the buffer to save data
	uint32_t blk_addr,  // address of 1st block to be read
	uint16_t blk_len)   // nmber of blocks to be read
{
	emfat_read(&emfat, buf, blk_addr, blk_len);
	return 0;
}

int8_t STORAGE_Write(uint8_t lun,
	uint8_t *buf,
	uint32_t blk_addr,
	uint16_t blk_len)
{
	emfat_write(&emfat, buf, blk_addr, blk_len);
	return 0;
}

int8_t STORAGE_GetMaxLun(void)
{
  return (STORAGE_LUN_NBR - 1);
}

USBD_STORAGE_cb_TypeDef STORAGE_fops =
{
  STORAGE_Init,
  STORAGE_GetCapacity,
  STORAGE_IsReady,
  STORAGE_IsWriteProtected,
  STORAGE_Read,
  STORAGE_Write,
  STORAGE_GetMaxLun,
  (int8_t *)STORAGE_Inquirydata,
};

USBD_STORAGE_cb_TypeDef *USBD_STORAGE_fops = &STORAGE_fops;

void storage_mode_init(void)
{
}
