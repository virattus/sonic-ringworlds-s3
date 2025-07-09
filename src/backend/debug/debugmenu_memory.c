#include <yaul.h>


static const char teststring[] = {
	0x01,
	0x00
};

static const char TitleBar[] = {
	"┐\n"
	"@MEMORY@@@@@@@@@@@@@\n"
	"@@@@@@@@@@@@@@@@@@@@\n"
};

unsigned int accumulator = 0;

void DebugMemoryUsage_Draw()
{
	smpc_peripheral_digital_t digital;
	smpc_peripheral_process();
	smpc_peripheral_digital_port(1, &digital);
	
	if(digital.held.button.left)
	{
		accumulator = (accumulator - 1) & 0xFF;
	}
	
	if(digital.held.button.right)
	{
		accumulator = (accumulator + 1) & 0xFF;
	}

	if(digital.held.button.up)
	{
		accumulator = (accumulator - 10) & 0xFF;
	}
	
	if(digital.held.button.down)
	{
		accumulator = (accumulator + 10) & 0xFF;
	}
	
	//dbgio_printf("DebugMemoryUsage_Draw called\n");
	//dbgio_printf(TitleBar);
	
	
	char charAcc[2];
	charAcc[0] = (char)accumulator;
	charAcc[1] = 0;
	dbgio_printf("ID: %d Char: ", accumulator);
	dbgio_printf(charAcc);
	
	dbgio_puts("[7;1H\x04 16X16 FONT TEST! \x04\n");
	dbgio_puts("\xBB testing \xBC\n");
}
