/*
 * ORIGINALLY FROM:
 * @file graphics.h
 * @author ajxs
 * @date Aug 2019
 * @brief Functionality for working with the graphics protocol.
 * Contains functionality for initiating and working with the graphics protocol
 * and its associated framebuffers.
 */

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "efilib.h"

/*
 * TODO: !!!
 * 1) Figure out where graphics protocol is defined and why is it not in graphics
 * 2) why is handleCount stored in struct Graphics, nobody is using it.
 * On second thought, not even handleBuffer is used anywhere, what is the
 * purpose of this??
 * 3) maybe place protocol in the struct instead?
 */

/*
 * @brief Graphics output service.
 * Holds variables necesssary for using the UEFI graphics output service.
 *
 * Is this an appropriate name or file to define it in?
 */
struct Graphics {
	EFI_HANDLE *	handleBuffer;
	UINTN		handleCount;
};

/*
 * @brief Set the graphics mode for a particular protocol.
 * Sets the graphics mode for a particular protocol handle. Sets the graphics
 * mode by searching all available modes on this protocol for one that matches
 * the target width/height.
 * @param[in] protocol			The protocol to set the mode for.
 * @param[in] targetWidth		The target width.
 * @param[in] targetHeight		The target height.
 * @param[in] targetPixelFormat		The target pixel format.
 */
void graphics_set_mode(EFI_GRAPHICS_OUTPUT_PROTOCOL * const protocol,
	const UINT32 targetWidth, const UINT32 targetHeight,
	const EFI_GRAPHICS_PIXEL_FORMAT targetPixelFormat);

/*
 * @brief Initialises the Graphics output service.
 * Initialises the graphics output service. Populates the handle buffer in the
 * graphics output service.
 * @param[out] graphics
 */
void graphics_init(struct Graphics *graphics);

/*
 * @brief Closes the graphics output service.
 * Closes the graphics output service, freeing the handle buffer used by the
 * service.
 * @param[in] graphics
 */
void graphics_close(struct Graphics *graphics);

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
