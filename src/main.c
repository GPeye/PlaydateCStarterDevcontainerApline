//
//  main.c
//  Extension
//
//  Created by Dave Hayden on 7/30/14.
//  Copyright (c) 2014 Panic, Inc. All rights reserved.
// https://github.com/schrockwell/PdLink/blob/main/src/goertzel.c

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <complex.h>

#include "pd_api.h"

static int update(void* userdata);
const struct playdate_sound_sample* pd_sample;
const struct playdate_sound_sampleplayer* pd_sampleplayer;
static int update(void* userdata);


#ifdef _WINDLL
__declspec(dllexport)
#endif

static PlaydateAPI *pd = NULL;
#define MIC_BUFLEN 44100
int16_t micdata[MIC_BUFLEN];
int micdatapos = 0;
AudioSample* sample = NULL;
SamplePlayer* player = NULL;
int running = 0;

int micCallback(void* context, int16_t* data, int len)
{
	pd->system->logToConsole("%d", len);
	if ( len > MIC_BUFLEN - micdatapos )
		len = MIC_BUFLEN - micdatapos;
	
	memcpy(&micdata[micdatapos], data, len * sizeof(int16_t));
	micdatapos += len;
	
	return micdatapos < MIC_BUFLEN;
}

void micChanged(int hp, int mic)
{
	pd->system->logToConsole("micChanged() called: hp=%i mic=%i", hp, mic);
	pd->sound->setOutputsActive(hp, !hp);
}

void drawStatus(const char* msg)
{
	pd->graphics->clear(kColorWhite);
	pd->graphics->drawText(msg, strlen(msg), kUTF8Encoding, 20, 20);
}

void playerFinished(SoundSource* source)
{
	drawStatus("press A to record 1s\nand play back");
	running = 0;
}

int eventHandler(PlaydateAPI* playdate, PDSystemEvent event, uint32_t arg)
{
	(void)arg; // arg is currently only used for event = kEventKeyPressed

	if ( event == kEventInit )
	{
		pd = playdate;
		pd_sample = playdate->sound->sample;
		pd_sampleplayer = playdate->sound->sampleplayer;
		
		int hp, mic;
		pd->sound->getHeadphoneState(&hp, &mic, micChanged);
		pd->system->logToConsole("getHeadphoneState() returned hp=%i mic=%i", hp, mic);
		pd->display->setScale(1);
		drawStatus("press A to record 1s\nand play back");
		
		sample = pd_sample->newSampleFromData((uint8_t*)micdata, kSound16bitMono, 44100, sizeof(micdata));
		player = pd_sampleplayer->newPlayer();
		pd_sampleplayer->setSample(player, sample);
		pd_sampleplayer->setFinishCallback(player, playerFinished, NULL);
		pd->system->setUpdateCallback(update, pd);
	}
	
	return 0;
}

static int update(void* userdata)
{
	// PlaydateAPI* pd = userdata;
	
	// pd->graphics->clear(kColorWhite);
	// pd->graphics->setFont(font);
	// pd->graphics->drawText("Hello World!", strlen("Hello World!"), kASCIIEncoding, x, y);

	// x += dx;
	// y += dy;
	
	// if ( x < 0 || x > LCD_COLUMNS - TEXT_WIDTH )
	// 	dx = -dx;
	
	// if ( y < 0 || y > LCD_ROWS - TEXT_HEIGHT )
	// 	dy = -dy;
        
	pd->system->drawFPS(0,0);
	if ( !running )
	{
		PDButtons pressed;
		pd->system->getButtonState(NULL, &pressed, NULL);
		
		if ( pressed & kButtonA )
		{
			pd->sound->setMicCallback(micCallback, NULL, 0);
			drawStatus("recording");
			running = 1;
		}
	}
	else if ( micdatapos == MIC_BUFLEN )
	{
		micdatapos = 0;
		//pd->sound->sampleplayer->play(player, 1, 1.0);
		drawStatus("playing");
	}

	return 1;
}

