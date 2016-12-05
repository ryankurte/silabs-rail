 /*************************************************************************//**
 * @file glib_font_bold_16x20.c
* @brief Silicon Labs Graphics Library: GLIB font number '0'-'9', ':' and ' ', 16x20 pixels
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
#include "glib.h"

static const uint16_t GLIB_FontNumber16x20PixMap[] =
{
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0x03c0, 0x0300, 0x07c0, 0x03e0, 0x0f00, 0x1ff8, 0x1f00, 0x1ffc, 0x07e0, 0x03e0, 0x0000, 0x0000,
  0x0ff0, 0x03e0, 0x0fe0, 0x0ff0, 0x0f80, 0x1ff8, 0x3fc0, 0x1ffc, 0x0ff8, 0x07f0, 0x0000, 0x0000,
  0x1e78, 0x03fc, 0x1c70, 0x0e38, 0x0f80, 0x0038, 0x31e0, 0x1c0c, 0x1c38, 0x0e38, 0x0180, 0x0000,
  0x1c38, 0x03bc, 0x3838, 0x1c18, 0x0fc0, 0x0038, 0x0070, 0x1c0c, 0x381c, 0x1c1c, 0x03c0, 0x0000,
  0x381c, 0x0380, 0x3838, 0x1c00, 0x0ee0, 0x0038, 0x0038, 0x0e00, 0x381c, 0x181c, 0x03c0, 0x0000,
  0x381c, 0x0380, 0x3838, 0x1c00, 0x0ee0, 0x07b8, 0x0038, 0x0e00, 0x381c, 0x381c, 0x0180, 0x0000,
  0x381c, 0x0380, 0x3c00, 0x0e00, 0x0e70, 0x0ff8, 0x07dc, 0x0e00, 0x1c38, 0x381c, 0x0000, 0x0000,
  0x381c, 0x0380, 0x1c00, 0x07c0, 0x0e70, 0x1c38, 0x0ffc, 0x0700, 0x0ff0, 0x3c1c, 0x0000, 0x0000,
  0x381c, 0x0380, 0x0e00, 0x07c0, 0x0e38, 0x3818, 0x1c7c, 0x0700, 0x0ff0, 0x3e38, 0x0000, 0x0000,
  0x381c, 0x0380, 0x0700, 0x1e00, 0x0e1c, 0x3800, 0x3c3c, 0x0700, 0x1c38, 0x3ff0, 0x0000, 0x0000,
  0x381c, 0x0380, 0x0380, 0x3c00, 0x3ffc, 0x3800, 0x383c, 0x0780, 0x381c, 0x3be0, 0x0180, 0x0000,
  0x381c, 0x0380, 0x01c0, 0x3800, 0x3ffc, 0x3800, 0x381c, 0x0380, 0x381c, 0x3c00, 0x03c0, 0x0000,
  0x381c, 0x0380, 0x00e0, 0x3800, 0x0e00, 0x3800, 0x381c, 0x0380, 0x381c, 0x1c00, 0x03c0, 0x0000,
  0x1c38, 0x0380, 0x0070, 0x3800, 0x0e00, 0x3800, 0x3838, 0x0380, 0x381c, 0x0e00, 0x0180, 0x0000,
  0x1e78, 0x0380, 0x0038, 0x1c0c, 0x0e00, 0x1c0c, 0x1c38, 0x01c0, 0x1c38, 0x0f8c, 0x0000, 0x0000,
  0x0ff0, 0x7ffc, 0x3ffc, 0x1ffc, 0x3fc0, 0x0ffc, 0x1ff0, 0x01c0, 0x1ff8, 0x03fc, 0x0000, 0x0000,
  0x03c0, 0x7ffc, 0x3ffc, 0x07f0, 0x3fc0, 0x07f0, 0x07c0, 0x01c0, 0x07e0, 0x00f8, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
};

const GLIB_Font_t GLIB_FontNumber16x20 = {(void *)GLIB_FontNumber16x20PixMap,
                                          sizeof(GLIB_FontNumber16x20PixMap),
                                          sizeof(GLIB_FontNumber16x20PixMap[0]),
                                          12, 16, 20, 5, 0, NumbersOnlyFont};


