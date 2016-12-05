 /*************************************************************************//**
 * @file glib.c
 * @brief Silicon Labs Graphics Library: General Routines
 ******************************************************************************
 * @section License
 * <b>Copyright 2015 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/


/* Standard C header files */
#include <stdint.h>

/* EM types and devices */
#include "em_types.h"
#include "em_device.h"

/* GLIB files */
#include "glib.h"

/* Define the default font */
#ifndef GLIB_NO_DEFAULT_FONT
#define GLIB_DEFAULT_FONT       ((GLIB_Font_t *)&GLIB_FontNormal8x8)
#endif

static __INLINE void GLIB_colorTranslate24bppInl(uint32_t color, uint8_t *red, uint8_t *green, uint8_t *blue)
{
  *red   = (color >> RedShift) & 0xFF;
  *green = (color >> GreenShift) & 0xFF;
  *blue  = (color >> BlueShift) & 0xFF;
}

/**************************************************************************//**
*  @brief
*  Initialize the GLIB_Context_t
*
*  The context is set to default values and gets information about the display
*  from the display driver.
*
*  @param pContext
*  Pointer to a GLIB_Context_t
*
*  @return
*  Returns GLIB_OK on success, or else error code
******************************************************************************/
EMSTATUS GLIB_contextInit(GLIB_Context_t *pContext)
{
  EMSTATUS status;
  DMD_DisplayGeometry *pTmpDisplayGeometry;

  /* Check arguments */
  if (pContext == NULL) return GLIB_ERROR_INVALID_ARGUMENT;

  /* Sets the default background and foreground color */
  pContext->backgroundColor = Black;
  pContext->foregroundColor = White;

  /* Sets a pointer to the display geometry struct */
  status = DMD_getDisplayGeometry(&pTmpDisplayGeometry);
  if (status != DMD_OK) return status;

  pContext->pDisplayGeometry = pTmpDisplayGeometry;

  /* Sets the clipping region to the whole display */
  GLIB_Rectangle_t tmpRect = {0, 0, pTmpDisplayGeometry->xSize - 1, pTmpDisplayGeometry->ySize - 1};
  status = GLIB_setClippingRegion(pContext, &tmpRect);
  if (status != GLIB_OK) return status;

  /* Configure font. Default to NORMAL 8x8 if included in project. */
#ifndef GLIB_NO_DEFAULT_FONT
  GLIB_setFont(pContext, GLIB_DEFAULT_FONT);
#endif

  return status;
}

/**************************************************************************//**
*  @brief
*  Returns the display from sleep mode
*
*  @return
*  Returns DMD_OK on success, or else error code
******************************************************************************/

EMSTATUS GLIB_displayWakeUp()
{
  /* Use display driver's wake up function */
  return DMD_wakeUp();
}

/**************************************************************************//**
*  @brief
*  Sets the display in sleep mode
*
*  @return
*  Returns DMD_OK on success, or else error code
******************************************************************************/

EMSTATUS GLIB_displaySleep()
{
  /* Use Display Driver sleep function */
  return DMD_sleep();
}

/**************************************************************************//**
*  @brief
*  Sets the clippingRegion of the passed in GLIB_Context_t
*
*  @param pContext
*  Pointer to a GLIB_Context_t
*  @param pRect
*  Pointer to a GLIB_Rectangle_t which is the clipping region to be set.
*
*  @return
*  - Returns GLIB_OK on success
*  - Returns GLIB_ERROR_INVALID_CLIPPINGREGION if invalid coordinates
*  - Returns GLIB_OUT_OF_BOUNDS if clipping region is bigger than display clipping
*  area
******************************************************************************/

EMSTATUS GLIB_setClippingRegion(GLIB_Context_t *pContext, GLIB_Rectangle_t *pRect)
{
  EMSTATUS status;

  /* Check arguments */
  if ((pContext == NULL) || (pRect == NULL)) return GLIB_ERROR_INVALID_ARGUMENT;

  /* Check coordinates against the display region */
  if ((pRect->xMin >= pRect->xMax) ||
     (pRect->yMin >= pRect->yMax)) return GLIB_ERROR_INVALID_CLIPPINGREGION;

  if ((pRect->xMin < pContext->pDisplayGeometry->xClipStart) ||
      (pRect->yMin < pContext->pDisplayGeometry->yClipStart) ||
      (pRect->xMax > pContext->pDisplayGeometry->clipWidth - 1) ||
      (pRect->yMax > pContext->pDisplayGeometry->clipHeight - 1)) return GLIB_OUT_OF_BOUNDS;

  /* Set clipping region in driver */
  status = DMD_setClippingArea(pRect->xMin,
                               pRect->yMin,
                               pRect->xMin + pRect->xMax + 1,
                               pRect->yMin + pRect->yMax + 1);

  GLIB_Rectangle_t tmpRect = {pRect->xMin, pRect->yMin, pRect->xMax, pRect->yMax};
  pContext->clippingRegion = tmpRect;

  return status;
}


/**************************************************************************//**
*  @brief
*  Clears the display with the background color of the GLIB_Context_t
*
*  @param pContext
*  Pointer to a GLIB_Context_t which holds the backgroundcolor.
*
*  @return
*  Returns GLIB_OK on success, or else error code
******************************************************************************/
EMSTATUS GLIB_clear(GLIB_Context_t *pContext)
{
  EMSTATUS status;
  uint8_t  red;
  uint8_t  green;
  uint8_t  blue;
  uint32_t width;
  uint32_t height;

  /* Check arguments */
  if (pContext == NULL) return GLIB_ERROR_INVALID_ARGUMENT;

  /* Divide the 24-color into it's components */
  GLIB_colorTranslate24bpp(pContext->backgroundColor, &red, &green, &blue);

  /* Reset display driver clipping area */
  status = GLIB_resetDisplayClippingArea(pContext);
  if (status != GLIB_OK) return status;

  /* Fill the display with the background color of the GLIB_Context_t  */
  width = pContext->pDisplayGeometry->clipWidth;
  height = pContext->pDisplayGeometry->clipHeight;
  return DMD_writeColor(0, 0, red, green, blue, width * height);
}

/**************************************************************************//**
*  @brief
*  Reset the display driver clipping area to the whole display
*
*  @param pContext
*  Pointer to a GLIB_Context_t
*
*  @return
*  Returns GLIB_OK on success, or else error code
******************************************************************************/
EMSTATUS GLIB_resetDisplayClippingArea(GLIB_Context_t *pContext)
{
  /* Check arguments */
  if (pContext == NULL) return GLIB_ERROR_INVALID_ARGUMENT;

  return DMD_setClippingArea(0, 0, pContext->pDisplayGeometry->xSize,
                                        pContext->pDisplayGeometry->ySize);
}

/**************************************************************************//**
*  @brief
*  Reset the GLIB_Context_t clipping region to the whole display
*
*  @param pContext
*  Pointer to a GLIB_Context_t
*
*  @return
*  Returns GLIB_OK on success, or else error code
******************************************************************************/
EMSTATUS GLIB_resetClippingRegion(GLIB_Context_t *pContext)
{
  /* Check arguments */
  if (pContext == NULL) return GLIB_ERROR_INVALID_ARGUMENT;

  pContext->clippingRegion.xMin = pContext->pDisplayGeometry->xClipStart;
  pContext->clippingRegion.yMin = pContext->pDisplayGeometry->yClipStart;
  pContext->clippingRegion.xMax = pContext->clippingRegion.xMin + pContext->pDisplayGeometry->clipWidth - 1;
  pContext->clippingRegion.yMax = pContext->clippingRegion.yMin + pContext->pDisplayGeometry->clipHeight - 1;

  return GLIB_OK;
}

/**************************************************************************//**
*  @brief
*  Extracts the color components from the 32-bit color passed and puts them in
*  the passed in 8-bits ints. The color is 24-bit RGB.
*
*  Example: color = 0x00FFFF00 -> red = 0xFF, green = 0xFF, blue = 0x00.
*
*  @param color
*  The color which is to be translated
*  @param red
*  Pointer to a uint8_t holding the red component
*  @param green
*  Pointer to a uint8_t holding the green component
*  @param blue
*  Pointer to a uint8_t holding the blue component
*
*  @return
*  None.
******************************************************************************/
void GLIB_colorTranslate24bpp(uint32_t color, uint8_t *red, uint8_t *green, uint8_t *blue)
{
  GLIB_colorTranslate24bppInl(color, red, green, blue);
}

/**************************************************************************//**
*  @brief
*  Convert 3 uint8_t color components into a 24-bit color
*
*  Example: red = 0xFF, green = 0xFF, blue = 0x00 -> 0x00FFFF00 = Yellow
*
*  @param red
*  Red component
*  @param green
*  Green component
*  @param blue
*  Blue component
*  @return
*  Returns a 32-bit unsigned integer representing the color. The 8 LSB is blue,
*  the next 8 is green and the next 8 is red. 0x00RRGGBB
*
******************************************************************************/

uint32_t GLIB_rgbColor(uint8_t red, uint8_t green, uint8_t blue)
{
  return (red << RedShift) | (green << GreenShift) | (blue << BlueShift);
}

/**************************************************************************//**
*  @brief
*  Draws a pixel at x, y using foregroundColor defined in the GLIB_Context_t.
*
*  @param pContext
*  Pointer to a GLIB_Context_t which holds the foreground color and clipping region
*  @param x
*  X-coordinate
*  @param y
*  Y-coordinate
*
*  @return
*  Returns GLIB_OK on success, or else error code
******************************************************************************/
EMSTATUS GLIB_drawPixel(GLIB_Context_t *pContext, int32_t x, int32_t y)
{
  uint8_t red;
  uint8_t green;
  uint8_t blue;

  /* Check arguments */
  if (pContext == NULL) return GLIB_ERROR_INVALID_ARGUMENT;
  if (!GLIB_rectContainsPoint(&pContext->clippingRegion, x, y)) return GLIB_ERROR_NOTHING_TO_DRAW;

  /* Translate color and draw pixel */
  GLIB_colorTranslate24bppInl(pContext->foregroundColor, &red, &green, &blue);
  return DMD_writeColor(x, y, red, green, blue, 1);
}

/**************************************************************************//**
*  @brief
*  Draws a pixel at x, y using the color parameter
*
*  @param pContext
*  Pointer to a GLIB_Context_t which holds the clipping region
*  @param x
*  X-coordinate
*  @param y
*  Y-coordinate
*  @param color
*  32-bit int defining the RGB color. The 24 LSB defines the RGB color like this:
*  RRRRRRRRGGGGGGGGBBBBBBBB. Example: Yellow = 0x00FFFF00
*  @return
*  Returns DMD_OK on success, or else error code
******************************************************************************/
EMSTATUS GLIB_drawPixelColor(GLIB_Context_t *pContext, int32_t x, int32_t y,
                             uint32_t color)
{
  uint8_t red;
  uint8_t green;
  uint8_t blue;

  /* Check arguments */
  if (pContext == NULL) return GLIB_ERROR_INVALID_ARGUMENT;
  if (!GLIB_rectContainsPoint(&pContext->clippingRegion, x, y)) return GLIB_ERROR_NOTHING_TO_DRAW;

  /* Translate color and draw pixel */
  GLIB_colorTranslate24bppInl(color, &red, &green, &blue);
  return DMD_writeColor(x, y, red, green, blue, 1);
}

/**************************************************************************//**
*  @brief
*  Draws a pixel at x, y with color defined by red, green and blue
*  1 byte per channel.
*
*  Example: To draw a yellow pixel at (10, 10).
*  x = 10, y = 10, red = 0xFF, green = 0xFF, blue = 0x00
*
*  @param pContext
*  Pointer to a GLIB_Context_t which holds the clipping region
*  @param x
*  X-coordinate
*  @param y
*  Y-coordinate
*  @param red
*  8-bit red code
*  @param green
*  8-bit green code
*  @param blue
*  8-bit blue code
*
*  @return
*  Returns DMD_OK on success, or else error code
******************************************************************************/
EMSTATUS GLIB_drawPixelRGB(GLIB_Context_t *pContext, int32_t x, int32_t y,
                           uint8_t red, uint8_t green, uint8_t blue)
{
  /* Check arguments */
  if (pContext == NULL) return GLIB_ERROR_INVALID_ARGUMENT;
  if (!GLIB_rectContainsPoint(&pContext->clippingRegion, x, y)) return GLIB_ERROR_NOTHING_TO_DRAW;

  /* Call Display driver function */
  return DMD_writeColor(x, y, red, green, blue, 1);
}
