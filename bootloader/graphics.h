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
 * @param[in]  targetWidth		The target width to search for.
 * @param[in]  targetHeight		The target height to search for.
 * @param[in]  targetPixelFormat	The target pixel format to search for.
 *
 * @return the GOP pointer.
 */
EFI_GRAPHICS_OUTPUT_PROTOCOL * graphics_init(const UINT32 targetWidth,
	const UINT32 targetHeight,
	const EFI_GRAPHICS_PIXEL_FORMAT targetPixelFormat);

/*
 * @brief Draws a rectangle onto the framebuffer.
 * Draws a rectangle onto the frame buffer of a particular protocol.
 * @param[in] protocol	The protocol containing the framebuffer to draw to.
 * @param[in] x		The x coordinate to draw the rect to.
 * @param[in] y		The y coordinate to draw the rect to.
 * @param[in] width	The width of the rectangle to draw.
 * @param[in] height	The height of the rectangle to draw.
 * @param[in] color	The color to draw.
 */
void graphics_draw_rect(EFI_GRAPHICS_OUTPUT_PROTOCOL * const protocol,
	const UINT16 x, const UINT16 y, const UINT16 width, const UINT16 height,
	const UINT32 color);

/*
 * @brief Draws a test screen.
 * Draws a test screen which can be used to assess that the graphics output
 * protocol is functioning correctly. Draws to the provided framebuffer.
 * @param[in] protocol	The protocol containing the framebuffer to draw to.
 */
void graphics_draw_test(EFI_GRAPHICS_OUTPUT_PROTOCOL * const protocol);

#endif // GRAPHICS_H
