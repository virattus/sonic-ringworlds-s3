#include "debug_window.h"

#include <yaul.h>

#define CANVAS_MAX_WIDTH	(DEBUGWINDOW_MAX_WIDTH)
#define CANVAS_MAX_HEIGHT	(DEBUGWINDOW_MAX_HEIGHT)

static char canvas[CANVAS_MAX_WIDTH * DEBUGWINDOW_MAX_HEIGHT + 1];


void DebugWindow_DrawWindow(
	const int16_vec2_t* position, 
	const int16_vec2_t* size, 
	const char* title)
{
	assert(position->x < CANVAS_MAX_WIDTH);
	assert(position->y < CANVAS_MAX_HEIGHT);
	
	const int countX = min(size->x, CANVAS_MAX_WIDTH - position->x);
	const int countY = min(size->y, CANVAS_MAX_HEIGHT - position->y);
	
	int16_t currentRow = position->y;
	
	//Draw Titlebar
	const int titleSize = strlen(title);
	//Top
	int topOffset = (CANVAS_MAX_WIDTH * currentRow) + position->x;
	canvas[topOffset] = '+';
	canvas[topOffset + titleSize + 1] = '+';
	memset(&canvas[topOffset + 1], '-', titleSize);
	
	currentRow++;
	topOffset = (CANVAS_MAX_WIDTH * currentRow) + position->x;

	//Middle (includes titles
	canvas[topOffset] = '|';
	memcpy(&canvas[topOffset + 1], title, titleSize);
	canvas[topOffset + titleSize + 1] = '|';
	
	currentRow++;
	topOffset = (CANVAS_MAX_WIDTH * currentRow) + position->x;
	
	//Bottom
	memset(&canvas[topOffset + 1], '-', countX - 1);
	canvas[topOffset] = '+';
	if(position->x + size->x < CANVAS_MAX_WIDTH)
	{
		canvas[topOffset + countX - 1] = '+';
	}
	
	currentRow++;

	
	//Draw Middle
	char lineBuffer[size->x];
	memset(lineBuffer, ' ', size->x);
	lineBuffer[0] = '|';
	lineBuffer[size->x - 1] = '|';
	
	for(int i = currentRow; i < (countY); i++)
	{
		const int canvasOffset = (i * CANVAS_MAX_WIDTH) + position->x;
		////memcpy(&canvas[canvasOffset], lineBuffer, countX);
		memcpy(&canvas[canvasOffset], lineBuffer, countX);
		currentRow++;
	}
	
	//Draw Bottom
	
	if(currentRow < CANVAS_MAX_HEIGHT)
	{
		topOffset = (CANVAS_MAX_WIDTH * (size->y - 1)) + position->x;
		canvas[topOffset] = '+';
		canvas[topOffset + countX - 1] = '+';
		memset(&canvas[topOffset + 1], '-', countX - 2);
	}
}

void DebugWindow_DrawLine(const int16_vec2_t* position, const char* line)
{
	const int offset = (position->y * CANVAS_MAX_WIDTH) + position->x;
	const int count = min(strlen(line), CANVAS_MAX_WIDTH - position->y);
	
	memcpy(&canvas[offset], line, count);
}

void DebugWindow_DrawCanvas(void)
{
	dbgio_puts(canvas);
}


void DebugWindow_ClearCanvas(void)
{
	memset(canvas, ' ', CANVAS_MAX_WIDTH * CANVAS_MAX_HEIGHT);
	canvas[CANVAS_MAX_WIDTH * CANVAS_MAX_HEIGHT] = '\0';
}
