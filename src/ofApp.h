#pragma once

#include "ofMain.h"
#include <unordered_map>

struct Point3D {
	glm::vec3 position;
	float intensity;
	ofColor color;
};

class ofApp : public ofBaseApp{

	public:
		void setup();
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

		void loadPointCloud(const string& filename);
		void cleanNoise(const string& filename);
		bool pointExists(float x, float y, float z);
		void addPoint(Point3D point);
		void createTriangularMesh();
		void index(ofMesh& mesh, int startNum, int endNum);
		
		std::unordered_map<std::string, Point3D> pointMap, indexMap;
		vector<Point3D> pointCloud;
		ofEasyCam easyCam;
		ofMesh mesh, mesh2, mesh3, mesh4, mesh5;
		long count;
		bool wireframe, vertices;
};



