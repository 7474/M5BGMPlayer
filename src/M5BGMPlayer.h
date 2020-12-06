#ifndef __INC_M5BGMPLAYER_H
#define __INC_M5BGMPLAYER_H

#include <M5Stack.h>
#include "AudioFileSourceSD.h"
#include "AudioFileSourceID3.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"

struct M5BGMPlayerTrack
{
    int no;
    String path;
    M5BGMPlayerTrack *left;
    M5BGMPlayerTrack *right;
};

class M5BGMPlayer
{
public:
    M5BGMPlayer();
    ~M5BGMPlayer();

    bool init();
    bool loadTrackListFromSD(String path);
    void play(M5BGMPlayerTrack *track);
    void stop();
    void pause();
    void resume();

    void update();

    M5BGMPlayerTrack *trackList();
    M5BGMPlayerTrack *currentTrack();
    bool isPlaying();

private:
    M5BGMPlayerTrack *_trackList;
    M5BGMPlayerTrack *_currentTrack;
    bool playing = true;
    unsigned long currentMillis;

    AudioGeneratorMP3 *mp3;
    AudioOutputI2S *out;
    AudioFileSourceSD *file;
    AudioFileSourceID3 *id3;
};

#endif