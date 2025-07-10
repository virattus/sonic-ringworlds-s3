#ifndef __SSEE_DEBUG_WINDOW_H__
#define __SSEE_DEBUG_WINDOW_H__


#include <yaul.h>

#include <stdint.h>
#include <stdbool.h>

#define DEBUGWINDOW_MAX_WIDTH		(44)
#define DEBUGWINDOW_MAX_HEIGHT		(28)

void DebugWindow_DrawWindow(
	const int16_vec2_t* position, 
	const int16_vec2_t* size, 
	const char* title);

//Draws a line of text in the window, used to populate the window contents
void DebugWindow_DrawLine(const int16_vec2_t* position, const char* line);

void DebugWindow_DrawCanvas(void);
void DebugWindow_ClearCanvas(void);


#endif
