#include "soundtest.h"


#include <yaul.h>
#include <mic3d.h>

#include <backend/sound/soundcontrol.h>


void LoadSamples(void)
{
}


void TestADPCMVoice(void)
{
	
}


void SoundTestState_Init(void)
{
	LoadSoundDriver();
	LoadSamples();
}


void SoundTestState_Free(void)
{
}


void SoundTestState_Update(void)
{
	TestADPCMVoice();
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
