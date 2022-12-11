/*
 * Contains functionality for initiating and working with the graphics protocol
 * and its associated framebuffers.
 */

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "efilib.h"

/*
 * @brief Initializes the GOP.
 * Initializes the GOP, finds and sets the requested video mode.
 * @param[in] targetWidth		The target width to search for.
 * @param[in] targetHeight		The target height to search for.
 * @param[in] targetPixelFormat		The target pixel format to search for.
 *
 * @return the GOP pointer.
 */
EFI_GRAPHICS_OUTPUT_PROTOCOL * graphics_init(const UINT32 targetWidth,
	const UINT32 targetHeight,
	const EFI_GRAPHICS_PIXEL_FORMAT targetPixelFormat);

#endif // GRAPHICS_H
