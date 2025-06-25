#include "soundtest.h"


#include <yaul.h>
#include <mic3d.h>

#include <backend/sound/soundcontrol.h>

uint32_t samples_size = 0;

void LoadSamples(void)
{
	samples_size = CD_LoadFile("SAMPLES.LAD", LWRAM(0));

	snd_LoadSampleData(0, samples_size, LWRAM(0));

}


void TestADPCMVoice(void)
{
	snd_PlayADPCM(0, 0);
}


void SoundTestState_Init(void)
{
	CD_OpenFileList();
	
	snd_LoadDriver();
	LoadSamples();
	
	CD_CloseFileList();
	
	TestADPCMVoice();
}


void SoundTestState_Free(void)
{
}


void SoundTestState_Update(void)
{
	
	dbgio_printf("samples size: %i\n", samples_size);
}



gamestate_t Get_SoundTestState(void)
{
	return (gamestate_t)
	{
		.InitState = SoundTestState_Init,
		.FreeState = SoundTestState_Free,
		.UpdateState = SoundTestState_Update,
	};
}
