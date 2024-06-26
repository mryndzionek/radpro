/*
 * Rad Pro
 * Simulator flash
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#if defined(SIMULATOR)

#include <stdio.h>

#include "../flash.h"

#define FLASH_PAGE_SIZE 0x400
#define FLASH_WORD_SIZE 0x2

#define FLASH_FILENAME "radpro-simulator-settings.bin"

uint8_t flashImage[0x10000];

const FlashRegion flashSettingsRegion = {
    0x0,
    0x1,
};
const FlashRegion flashDatalogRegion = {
    0x1,
    0x40,
};

const uint32_t flashPageDataSize = FLASH_PAGE_SIZE - FLASH_WORD_SIZE;
const uint32_t flashWordSize = FLASH_WORD_SIZE;

void initFlash(void)
{
    memset(flashImage,
           0xff,
           sizeof(flashImage));

    FILE *fp = fopen(FLASH_FILENAME, "rb");
    if (fp)
    {
        fread(flashImage,
              1,
              sizeof(flashImage),
              fp);
        fclose(fp);
    }
}

bool verifyFlash(void)
{
    return true;
}

static void writeFlashImage(void)
{
    FILE *fp = fopen(FLASH_FILENAME, "wb");
    if (fp)
    {
        fwrite(flashImage,
               1,
               sizeof(flashImage),
               fp);
        fclose(fp);
    }
}

void readFlash(FlashIterator *iterator,
               uint8_t *dest,
               uint32_t size)
{
    uint32_t address = iterator->pageIndex * FLASH_PAGE_SIZE + iterator->index;

    memcpy(dest,
           flashImage + address,
           size);

    iterator->index += size;
}

void eraseFlash(FlashIterator *iterator)
{
    uint32_t address = iterator->pageIndex * FLASH_PAGE_SIZE;

    memset(flashImage + address,
           0xff,
           FLASH_PAGE_SIZE);

    writeFlashImage();
}

void writeFlash(FlashIterator *iterator,
                uint8_t *source,
                uint32_t size)
{
    uint32_t destAddress = iterator->pageIndex * FLASH_PAGE_SIZE + iterator->index;

    memcpy(flashImage + destAddress,
           source,
           size);

    writeFlashImage();

    iterator->index += size;
}

#endif
