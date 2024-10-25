#include "Animation.h"

Animation::Animation() : frames(), playSpeed(MIN_PLAY_SPEED){}

Animation::Animation(const std::vector<Frame>& frames, float playSpeed) : playSpeed(playSpeed){
	SetFrames(frames);
}

Animation::Animation(const Animation& other) 
	: name(other.name), playSpeed(other.playSpeed), frames(std::vector<Frame>(other.frames)){
}

Animation& Animation::operator=(const Animation& other) {
	name = other.name;
	playSpeed = other.playSpeed;
	// Deep copy frames
	frames = std::vector<Frame>(other.frames);
	return *this;
}



void Animation::Play(AnimationData* data) {
	data->currentFrame = 0;
	data->playTime = 0.0f;
	data->playing = true;
}

// TODO: is there a more efficient way to do this?
// Calling update like this is kind of "spinning"
// would it be better to use some sort of event?
// Or either way would there have to be the same
// calculation each frame
void Animation::Update(float dt, AnimationData* data) {
	if (!data->playing) return;
	data->playTime += dt;
	// Calculates index in frame vector
	// TODO: make speed = time per frame
	data->currentFrame = (int)floor(data->playTime * playSpeed);
	if (data->currentFrame >= frames.size()) {
		data->currentFrame = (int)frames.size() - 1;
		AnimationEnd(data);
	}
}

const Frame& Animation::GetCurrentFrame(AnimationData* data) {
	return frames[data->currentFrame];
}

void Animation::AnimationEnd(AnimationData* data) {
	if (data->looping) { Play(data); return; }
	Stop(data);
}

void Animation::Stop(AnimationData* data) {
	data->playing = false;
	// Invoke callback
	// TODO: figure this out
	//animator->AnimEndCallback();
}