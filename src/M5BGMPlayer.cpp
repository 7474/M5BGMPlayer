#include "M5BGMPlayer.h"

M5BGMPlayer::M5BGMPlayer()
{
}

M5BGMPlayer::~M5BGMPlayer()
{
    // XXX delete
}

bool M5BGMPlayer::init()
{
    // XXX オブジェクトの生成はコンストラクタでやりたい
    this->mp3 = new AudioGeneratorMP3();
    this->out = new AudioOutputI2S(0, 1);
    this->out->SetOutputModeMono(true);

    this->file = new AudioFileSourceSD();
    this->id3 = new AudioFileSourceID3(this->file);

    this->_trackList = NULL;
    this->_currentTrack = NULL;
    this->playing = false;
    this->currentMillis = 0;
    return true;
}

bool M5BGMPlayer::loadTrackListFromSD(String path)
{
    File root = SD.open(path.c_str());
    if (!root)
    {
        return false;
    }
    Serial.println("SD.open");
    Serial.println(path);

    M5BGMPlayerTrack *first = NULL;
    M5BGMPlayerTrack *last = NULL;
    int no = 0;
    while (true)
    {
        File entry = root.openNextFile();
        if (!entry)
        {
            break;
        }
        if (!entry.isDirectory())
        {
            Serial.println(entry.name());
            if (String(entry.name()).endsWith("mp3"))
            {
                M5BGMPlayerTrack *tmp = new M5BGMPlayerTrack;
                tmp->no = ++no;
                tmp->path = entry.name();
                tmp->left = last;
                tmp->right = NULL;
                if (first == NULL)
                {
                    first = tmp;
                }
                if (last != NULL)
                {
                    last->right = tmp;
                }
                last = tmp;
            }
        }
        entry.close();
    }
    root.close();
    this->_trackList = first;
    return !!first;
}

void M5BGMPlayer::play(M5BGMPlayerTrack *track)
{
    Serial.println("M5BGMPlayer::play");
    if (track == NULL)
    {
        Serial.println("track is null.");
        return;
    }
    if (this->mp3->isRunning())
    {
        this->mp3->stop();
    }
    Serial.println("stop mp3.");
    Serial.println(track->path);
    if (this->id3->isOpen())
    {
        this->id3->close();
    }
    Serial.println("checked id3.");
    if (this->file->isOpen())
    {
        this->file->close();
    }
    Serial.println("checked file.");
    this->file->open(track->path.c_str());
    Serial.println("file->open()");
    this->id3->seek(0, 1);
    Serial.println("file->seek()");

    this->mp3->begin(this->id3, this->out);
    Serial.println("file->begin()");
    this->playing = true;
    this->_currentTrack = track;
}

void M5BGMPlayer::stop()
{
    this->mp3->stop();
    this->playing = false;
}

void M5BGMPlayer::pause()
{
    // XXX これでポーズレジュームになるかは怪しいなー
    this->playing = false;
}

void M5BGMPlayer::resume()
{
    this->playing = true;
}

void M5BGMPlayer::update()
{
    // Serial.println("M5BGMPlayer::update");
    // Serial.println(this->playing);
    // Serial.println(this->mp3->isRunning());
    if (this->playing && this->mp3->isRunning())
    {
        if (!this->mp3->loop())
        {
            this->stop();
            // TODO 次のトラックがあれば移動するとか
        }
    }
}

M5BGMPlayerTrack *M5BGMPlayer::trackList()
{
    return this->_trackList;
}

M5BGMPlayerTrack *M5BGMPlayer::currentTrack()
{
    return this->_currentTrack;
}

bool M5BGMPlayer::isPlaying()
{
    return this->playing;
}
