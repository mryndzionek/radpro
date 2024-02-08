/*
 * Rad Pro
 * Flash memory
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#if !defined(FLASH_H)
#define FLASH_H

#include <stdbool.h>
#include <stdint.h>

typedef struct
{
    uint8_t beginPageIndex;
    uint8_t endPageIndex;
} FlashRegion;

extern const FlashRegion flashSettingsRegion;
extern const FlashRegion flashDatalogRegion;

extern const uint32_t flashPageDataSize;
extern const uint32_t flashBlockSize;

typedef struct
{
    const FlashRegion *region;

    uint8_t pageIndex;
    uint32_t index;
} FlashIterator;

void initFlash(void);

bool verifyFlash(void);

uint8_t *getFlash(const FlashIterator *iterator);
void eraseFlash(const FlashIterator *iterator);
void programFlash(const FlashIterator *iterator,
                  uint8_t *source, uint32_t size);

bool isFlashPageFull(FlashIterator *iterator);
void setFlashPageHead(FlashIterator *iterator);
void setFlashPageTail(FlashIterator *iterator);
void setFlashPageNext(FlashIterator *iterator);
void setFlashPagePrev(FlashIterator *iterator);
void programFlashPage(FlashIterator *iterator,
                      uint8_t *source, uint32_t size);

#endif
