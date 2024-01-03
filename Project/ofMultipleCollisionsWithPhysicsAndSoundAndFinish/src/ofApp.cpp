#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() 
{
	ofBackground(ofColor(255));

	setSoundSettingsBasedOnOS();

	MAX_LINE_LENGTH = std::sqrt(ofGetWidth() * ofGetWidth() + ofGetHeight() * ofGetHeight());
	MAX_BALL_RADIUS = 160;

	// Friction fehlt noch !
	GRAVITY = glm::vec2(0.0, 0.02);
	MAX_FREQ_OFFSET = 200;
	MAX_FREQ_RANGE = 600;
    
    finish = drawFinish(glm::vec2(ofRandom(ofGetWindowWidth()), ofRandom(ofGetWindowHeight())), 270, ofRandom(360));
    if (finish.line.size() >= 2) {
        for (int i = 0; i < finish.line.size() - 1; i++) {
            lines.push_back(drawLine(finish.line[i], finish.line[i+1], ofColor(255, 0, 0)));
        }
    }
}

//--------------------------------------------------------------
void ofApp::update()
{
	for (int i = balls.size() - 1; i >= 0; i--)
	{
		balls[i].update(balls, lines);

		if (balls[i].location.x - balls[i].radius < 0 || balls[i].location.x + balls[i].radius > ofGetWidth() || balls[i].location.y + balls[i].radius > ofGetHeight())
		{
			balls.erase(balls.begin() + i);
		}

		if (balls[i].collideLines)
		{
			frequency = ((1-(balls[i].lineCircleProperies / MAX_LINE_LENGTH)) * (1-(balls[i].radius / MAX_BALL_RADIUS) * 0.5)) * MAX_FREQ_RANGE + MAX_FREQ_OFFSET;
			audioTrigger = true;
			balls[i].collideLines = false;
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw()
{
    finish.draw();
    
    for (int i = 0; i < balls.size(); i++)
	{
		balls[i].draw();
	}
	
	for (int i = 0; i < lines.size(); i++)
	{
		lines[i].draw();
	}

	if (mouseLeftPressedOnce == false)
	{
		ofSetLineWidth(2.0);
		ofSetColor(0, 0, 0);
		ofFill();
		ofDrawLine(lineStartPosition, glm::vec2(ofGetMouseX(), ofGetMouseY()));
	}

	if (mouseRightPressedOnce == false)
	{
		linesize = glm::vec2(ofGetMouseX(), ofGetMouseY());

		float dist_x = calcDirStart.x - linesize.x;
		float dist_y = calcDirStart.y - linesize.y;

		float angle = std::atan2(dist_y, dist_x);
		double degree = angle * (180.0 / PI);

		int len = sqrt(((dist_x * dist_x) + (dist_y * dist_y)));

		statbox = "Angle: " + to_string(static_cast<int>(degree * -1)) + "\u00B0" + "\nLength: " + to_string(static_cast<int>(len)) + "px";

		ofSetLineWidth(2.0);
		ofSetColor(155, 0, 0);
		ofFill();
		ofDrawLine(calcDirStart, glm::vec2(ofGetMouseX(), ofGetMouseY()));
		ofSetColor(0);
		ofDrawBitmapString(statbox, ofGetMouseX() + 25, ofGetMouseY() - 25);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	if (key == 'd')
	{
		lines.clear();
	}

	if (key == 's')
	{
		ofImage img;
		img.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		ofSaveImage(img, "mySnapshot.png");
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
	if (button == 0 && mouseLeftPressedOnce)
	{
		lineStartPosition = glm::vec2(ofGetMouseX(), ofGetMouseY());
		mouseLeftPressedOnce = false;
	} 

	if (button == 1)
	{
		lines.clear();
	}

	if (button == 2 && mouseRightPressedOnce)
	{
		calcDirStart = glm::vec2(ofGetMouseX(), ofGetMouseY());
		mouseRightPressedOnce = false;
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
	if (button == 0)
	{
		lineEndPosition = glm::vec2(x, y);
		lines.push_back(drawLine(lineStartPosition, lineEndPosition, ofColor(0, 0, 0)));

		mouseLeftPressedOnce = true;
	}

	if (button == 2)
	{
		calcDirEnd = linesize;

		float dist_x = calcDirStart.x - calcDirEnd.x;
		float dist_y = calcDirStart.y - calcDirEnd.y;

		float lineLength = std::sqrt((dist_x * dist_x) + (dist_y * dist_y));

		float angle = std::atan2(dist_y, dist_x);
		float dx = std::cos(angle);
		float dy = std::sin(angle);

		bool blockBallCreation = false;
		for (int lI = 0; lI < lines.size(); lI++)
		{
			if (lineCircle(lines[lI], calcDirStart, glm::vec2(dx, dy), (lineLength / MAX_LINE_LENGTH) * MAX_BALL_RADIUS))
			{
				blockBallCreation = true;
			}
		}
		if (!blockBallCreation)
			balls.push_back(Ball(calcDirStart, 255, (lineLength / MAX_LINE_LENGTH) * MAX_BALL_RADIUS, MAX_BALL_RADIUS, glm::vec2(dx, dy), GRAVITY));

		mouseRightPressedOnce = true;
	}
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void ofApp::audioOut(ofSoundBuffer& buffer)
{
	std::vector<float> envParam = { 0, 1, 1000,  1, 0, 1000 };

	if (audioTrigger)
	{		
		frequencies.push_back(frequency);
		modes.push_back(std::make_shared<modalFilter>());
		noises.push_back(std::make_shared<noiseGenerator>());
		envelopes.push_back(std::make_shared<evelopeGenerator>(100000));

		audioTrigger = false;
	}

	for (int mI = modes.size() - 1; mI >= 0; mI--)
	{
		modes[mI]->init(frequencies[mI], frequencies[mI] * 2.0, 1.0);
	}

	for (int sample = 0; sample < buffer.size(); sample++)
	{
		for (int mI = envelopes.size() - 1; mI >= 0; mI--)
		{
			output += modes[mI]->play(noises[mI]->play() * envelopes[mI]->play(envParam)) * 0.001;

			if (envelopes[mI]->isAlive() == false)
			{
				modes.erase(modes.begin());
				noises.erase(noises.begin());
				envelopes.erase(envelopes.begin());
				frequencies.erase(frequencies.begin());
			}
		}
		buffer[sample] = output;
		output = 0;
	}
}
