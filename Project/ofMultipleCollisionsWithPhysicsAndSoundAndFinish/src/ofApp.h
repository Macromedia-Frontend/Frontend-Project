#pragma once

#include "ofMain.h"
#include "Ball.h"
#include "Line.h"
#include "collisions.h"
#include "modalFilter.h"
#include "envelopes.h"
#include "noise.h"
#include "Finish.h"
#include "ofxDatGui.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
        void setupFinish();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void audioOut(ofSoundBuffer& buffer);

		float MAX_LINE_LENGTH;
		float MAX_BALL_RADIUS;

        glm::vec2 GRAVITY;

		std::vector<Ball> balls;
		glm::vec2 lineStartPosition, lineEndPosition;
		std::vector<drawLine> lines;
    
        drawFinish finish;

		glm::vec2 calcDirStart, calcDirEnd;
		bool mouseLeftPressedOnce = true;
		bool mouseRightPressedOnce = true;

		bool audioTriggerLines;
        bool audioTriggerTest;

		float MAX_FREQ_OFFSET;
		float MAX_FREQ_RANGE;
        float SPREAD;

		ofSoundStream soundStream;
		float frequency;
		std::vector<float> frequencies;
		std::vector<std::shared_ptr<modalFilter>> modes;
		std::vector<std::shared_ptr<evelopeGenerator>> envelopes;
		std::vector<std::shared_ptr<noiseGenerator>> noises;
		float output;
    
    void setSoundSettingsBasedOnOS(){
        ofSoundStreamSettings settings;
        
        #ifdef _WIN32

            settings.setApi(ofSoundDevice::Api::MS_DS);
            auto devices = soundStream.getDeviceList(ofSoundDevice::Api::MS_DS);

        #elif __APPLE__

            settings.setApi(ofSoundDevice::Api::OSX_CORE);
            auto devices = soundStream.getDeviceList(ofSoundDevice::Api::OSX_CORE);

        #elif __linux__

            settings.setApi(ofSoundDevice::Api::OSS);
            auto devices = soundStream.getDeviceList(ofSoundDevice::Api::OSS);

        #else

            settings.setApi(ofSoundDevice::Api::UNSPECIFIED);
            auto devices = soundStream.getDeviceList(ofSoundDevice::Api::UNSPECIFIED);

        #endif
        for (int i = 0; i < devices.size(); i++) {
            std::cout << devices[i] << "\n";
        }
        
        settings.setOutDevice(devices[0]);
        settings.setOutListener(this);
        settings.numOutputChannels = 1;
        settings.sampleRate = 44100;
        settings.bufferSize = 1024;
        settings.numBuffers = 4;
        soundStream.setup(settings);
    }
    
    bool gameFinished() {
        for (int i = balls.size() - 1; i >= 0; i--)
        {
            float distanceToFinish = glm::distance(balls[i].location, finish.getPos());
            
            if(distanceToFinish < finish.getRadius()){
                return true;
            }
        }
        return false;
    }
    
    void resetScreen(){
        balls.clear();
        lines.clear();
        
        setup();
    }

    double degree;
    float len;
    glm::vec2 linesize;
    std::string statbox;

    bool collideTest;
    std::vector<std::shared_ptr<evelopeGenerator>> testEnv;
    std::vector<std::shared_ptr<noiseGenerator>> testNoise;
    float testOutput;
    
    ofxDatGui* gui;
    void onSliderEvent(ofxDatGuiSliderEvent e);
    float klang_höhe_Slider = 100;
    float physik_gravity_Slider = 0;
};
