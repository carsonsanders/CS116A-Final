
//
//  Starter file for Project 3 - Skeleton Builder
//
//  This file includes functionality that supports selection and translate/rotation
//  of scene objects using the mouse.
//
//  Modifer keys for rotatation are x, y and z keys (for each axis of rotation)
//
//  Also demonstates simple keyframing and playback (With 2 keys)
//
//  (c) Kevin M. Smith  - 3 December 2019
//

#include "ofMain.h"
#include "box.h"
#include "Primitives.h"
#include <glm/gtx/intersect.hpp>
#include "ofxAssimpModelLoader.h"
#include "ofxGui.h"


class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	static void drawAxis(glm::mat4 transform = glm::mat4(1.0), float len = 1.0);
	bool mouseToDragPlane(int x, int y, glm::vec3 &point);
	void printChannels(SceneObject *);
	bool objSelected() { return (selected.size() ? true : false); };

	void deleteKeyframes() {
		if (!objSelected()) {
			return;
		}
		else {
			selected[0]->keyframes.clear();
		}
	}

	void addJoint() {
		SceneObject* newjoint = new Sphere();

		if (objSelected()) {
			selected[0]->addChild(newjoint);
		}

		scene.push_back(newjoint);
	}

	void deleteJoint() {
		if (objSelected()) {

			//reattach children to hierarchy
			for (SceneObject* obj : scene) {
				if (obj->parent == selected[0]) {
					obj->parent = selected[0]->parent;
				}
			}


			//remove object
			for (int i = 0; i < scene.size(); i++) {
				if (scene[i] == selected[0]) {
					scene.erase(scene.begin() + i);
				}
			}

			//remove from selection
			selected.erase(selected.begin());
		}
	}

	
	// key framing
	//
	void nextFrame() {
		slider = (frame == frameEnd ? frameBegin : frame + 1);
	}
	void prevFrame() {
		slider = (frame == frameBegin ? frame : frame - 1);
	}
	void startPlayback() {
		bInPlayback = true;
	}

	void stopPlayback() {
		bInPlayback = false;
	}

	bool keyFrameSet() {
		if (key1.frame == -1 && key2.frame == -1) {
			return false;
		}
		else {
			return true;
		}
	}

	glm::vec3 linearInterp(int frame, int frameStart, int frameEnd, const glm::vec3 & start, const glm::vec3 & end) {
		return mapVec(frame, frameStart, frameEnd, start, end);
	}

	glm::vec3 mapVec(float val, float start, float end, const glm::vec3 & outStart, const glm::vec3 & outEnd) {
		return glm::vec3(
			ofMap(val, start, end, outStart.x, outEnd.x),
			ofMap(val, start, end, outStart.y, outEnd.y),
			ofMap(val, start, end, outStart.z, outEnd.z));
	}

	void bindVertices() {
		if (!objSelected()) return;
		for (int i = 0; i < cachedMesh1->getNumVertices(); i++) {
			if (selected[0]->inBounds(cachedMesh1->getVertex(i))) {
				selected[0]->meshIndex.push_back(i);
			}
		}
	}

	void addKeyFrame() {
		if (objSelected()) selected[0]->addKeyFrame(new KeyFrame(), frame);
	}

	// set keyframe for SceneObject at current frame
	// call this function the first time and key1 is set
	// call this function again and key2 is set.
	// this "cycles" until you call resetKeyFrames();
	//
	/*
	void setKeyFrame(SceneObject * obj) {
		if (bKey2Next && key1.obj == obj) {
			key2.frame = frame;
			key2.obj = obj;
			key2.position = obj->position;
			bKey2Next = false;
		}
		else {
			key1.frame = frame;
			key1.obj = obj;
			key1.position = obj->position;
			bKey2Next = true;
		}
	}
	*/

	// reset key frames
	//
	/*
	void resetKeyFrames() {
		key1.frame = key2.frame = -1;
		key1.obj = key2.obj = NULL;
		bKey2Next = false;
	}
	*/

	// Lights
	//
	ofLight light1;

	// Cameras
	//
	ofEasyCam  mainCam;
	ofCamera sideCam;
	ofCamera topCam;
	ofCamera  *theCam;    // set to current camera either mainCam or sideCam

	// Materials
	//
	ofMaterial material;


	// scene components
	//
	vector<SceneObject *> scene;
	vector<SceneObject *> selected;
	ofPlanePrimitive plane;

	Sphere rootJoint;
	Sphere joint;


	//model things..
	ofxAssimpModelLoader model1;
	ofMesh* cachedMesh1;
	ofxAssimpMeshHelper* helper1;


	// GUI
	ofxPanel gui;
	ofxIntSlider slider;
	ofxButton del; 
	ofxButton addjoint;
	ofxButton deletejoint;
	ofxToggle hideJoints;
	ofxButton bind;
	ofxButton addKey;
	bool hidejoints;


	// key framing
	//
	KeyFrame key1, key2;    // this demo just has 2 key frames
	int frame = 1;         // current frame
	int frameBegin = 1;     // first frame of playback range;
	int frameEnd = 200;     // last frame of playback range;
	bool bInPlayback = false;  // true => we are in playback mode
	bool bKey2Next = false;
	int frameCount;


	// state
	//
	bool bDrag = false;
	bool bHide = true;
	bool bAltKeyDown = false;
	bool bRotateX = false;
	bool bRotateY = false;
	bool bRotateZ = false;
	bool bAnimate = false;
	glm::vec3 lastPoint;
};