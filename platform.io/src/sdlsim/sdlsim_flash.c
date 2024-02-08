/*
 * Rad Pro
 * SDLSim flash
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#if defined(SDLSIM)

#include <stdio.h>
#include <string.h>

#include "../flash.h"

#define FLASH_PAGE_SIZE 0x400
#define FLASH_BLOCK_SIZE 0x8

#if defined(DISPLAY_MONOCHROME)

#define FLASH_FILENAME "radpro-monochrome.bin"

#elif defined(DISPLAY_COLOR)

#define FLASH_FILENAME "radpro-color.bin"

#endif

uint8_t flashMemory[0x10000];

const FlashRegion flashSettingsRegion = {
    0x20,
    0x21,
};
const FlashRegion flashDatalogRegion = {
    0x21,
    0x40,
};

const uint32_t flashPageDataSize = FLASH_PAGE_SIZE - FLASH_BLOCK_SIZE;
const uint32_t flashBlockSize = FLASH_BLOCK_SIZE;

void initFlash(void)
{
    memset(flashMemory, 0xff, sizeof(flashMemory));
    FILE *fp = fopen(FLASH_FILENAME, "rb");

    if (fp)
    {
        fread(flashMemory, 1, sizeof(flashMemory), fp);
        fclose(fp);
    }
}

bool verifyFlash(void)
{
    return true;
}

static void backupFlash(void)
{
    FILE *fp = fopen(FLASH_FILENAME, "wb");
    if (fp)
    {
        fwrite(flashMemory, 1, sizeof(flashMemory), fp);
        fclose(fp);
    }
}

uint8_t *getFlash(const FlashIterator *iterator)
{
    return &flashMemory[iterator->pageIndex * FLASH_PAGE_SIZE];
}

void eraseFlash(const FlashIterator *iterator)
{
    uint32_t offset = iterator->pageIndex * FLASH_PAGE_SIZE;

    for (uint32_t i = 0; i < FLASH_PAGE_SIZE; i++)
        flashMemory[offset + i] = 0xff;

    backupFlash();
}

void programFlash(const FlashIterator *iterator,
                  uint8_t *source, uint32_t size)
{
    uint32_t dest = iterator->pageIndex * FLASH_PAGE_SIZE + iterator->index;

    for (uint32_t i = 0; i < size; i++)
        flashMemory[dest + i] = source[i];

    backupFlash();
}

#endif
