#include "ofApp.h"

#define REDUCE_NOISE_PER_STEP 10
#define UNIT .15

//--------------------------------------------------------------
void ofApp::setup(){
    easyCam.setPosition(0, 0, 150);  // Adjust the values as needed
    easyCam.lookAt(glm::vec3(0, 0, 0));  // Set the target position
    cleanNoise("apple.txt");
    //loadPointCloud("market_scan_high.txt");
    count = -1;
    wireframe = false;
    vertices = false;
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    easyCam.begin();

    // Set the background color to black
    ofBackground(0);

    if (wireframe) {
        mesh.drawWireframe();
    }
    else if (vertices) {
        mesh.drawVertices();
    }    
    else {
        mesh.draw();
    }
        
    easyCam.end();
}

void ofApp::loadPointCloud(const string& filename) {
    // Open the file and check if it can be read
    ifstream myFile(filename);

    if (myFile) {
        // Clear any existing points
        pointCloud.clear();
        float x, y, z, intensity;
        int r, g, b;
        cout << "Loading..." << endl;
        while (myFile >> x >> y >> z >> intensity >> r >> g >> b) {
            // Create a new point
            Point3D point;
            point.position = glm::vec3(x, y, z);
            point.intensity = intensity;
            point.color = ofColor(r, g, b);

            // Add point to vector
            pointCloud.push_back(point);
            mesh.addVertex(pointCloud.back().position);
            mesh.addColor(pointCloud.back().color);
            mesh2.addVertex(pointCloud.back().position);
            mesh2.addColor(pointCloud.back().color);
            mesh3.addVertex(pointCloud.back().position);
            mesh3.addColor(pointCloud.back().color);
            mesh4.addVertex(pointCloud.back().position);
            mesh4.addColor(pointCloud.back().color);
            mesh5.addVertex(pointCloud.back().position);
            mesh5.addColor(pointCloud.back().color);
        }
    }
    else {
        cout << "Failed to open!" << endl;
    }

    cout << "Load finished with " << pointCloud.size() << " point\n";

    createTriangularMesh();
}

// Function to check if a point already exists in the hashmap
bool ofApp::pointExists(float x, float y, float z) {
    // Create a unique key for the point using string concatenation of x, y, and z
    std::string key = std::to_string(x) + "_" + std::to_string(y) + "_" + std::to_string(z);

    // Check if the key exists in the hashmap
    return (pointMap.find(key) != pointMap.end());
}

// Function to add a point to the hashmap
void ofApp::addPoint(Point3D point) {
    // Create a unique key for the point using string concatenation of x, y, and z
    std::string key = std::to_string(point.position[0]) + "_" + std::to_string(point.position[1]) + "_" + std::to_string(point.position[2]);

    // Add the point to the hashmap using the unique key
    pointMap[key] = point;
}


void ofApp::createTriangularMesh() {
    cout << "Start creating mesh..." << endl;

    // Set the mesh mode to triangles
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);

    thread thread1(&ofApp::index, this, ref(mesh), 0, pointCloud.size() / 10);
    thread thread2(&ofApp::index, this, ref(mesh2), pointCloud.size() / 10, pointCloud.size() / 4);
    thread thread3(&ofApp::index, this, ref(mesh3), pointCloud.size() / 4, pointCloud.size() * 2 / 5);
    thread thread4(&ofApp::index, this, ref(mesh4), pointCloud.size() * 2 / 5, pointCloud.size() * 3 / 5);
    thread thread5(&ofApp::index, this, ref(mesh5), pointCloud.size() * 3 / 5, pointCloud.size());

    // Wait for the threads to finish
    thread1.join();
    thread2.join();
    thread3.join();
    thread4.join();
    thread5.join();

    for (int i = 0; i < mesh2.getNumIndices(); i++)
        mesh.addIndex(mesh2.getIndex(i));
    for (int i = 0; i < mesh3.getNumIndices(); i++)
        mesh.addIndex(mesh3.getIndex(i));
    for (int i = 0; i < mesh4.getNumIndices(); i++)
        mesh.addIndex(mesh4.getIndex(i));
    for (int i = 0; i < mesh5.getNumIndices(); i++)
        mesh.addIndex(mesh5.getIndex(i));

    // Save the mesh to a file
    mesh.save("market_mesh_high.ply");
}

// .5 

void ofApp::index(ofMesh& mesh, int startNum, int endNum) {
    // Add the points to the mesh as vertices and index
    for (int i = startNum; i < endNum - 2; i++) {
        if (i % 1000 == 0)
            cout << "Point " << i << endl;
        for (int j = i + 1; j < pointCloud.size() - 1; j++) {
            if (glm::distance(pointCloud.at(i).position, pointCloud.at(j).position) <= sqrt(pow(UNIT, 2) * 2))
                for (int k = j + 1; k < pointCloud.size(); k++) {
                    if (glm::distance(pointCloud.at(i).position, pointCloud.at(k).position) <= sqrt(pow(UNIT, 2) * 2)) {
                        mesh.addIndex(i);
                        mesh.addIndex(j);
                        mesh.addIndex(k);
                    }
                }
        }
    }
}

void ofApp::cleanNoise(const string& filename) {
    // Open the file and check if it can be read
    ifstream myFile(filename);
    ofstream outFile("apple_scan_high.txt");
    if (myFile) {
        float x, y, z, intensity;
        int r, g, b;
        cout << "Start cleaning..." << endl;
        while (myFile >> x >> y >> z >> r >> g >> b) {
            count++;
            if (count % REDUCE_NOISE_PER_STEP != 0)
                continue;

            // Round number to nearest half (.0 or .5)
            //x = floor((x * 2) + 0.5) / 2;
            //y = floor((y * 2) + 0.5) / 2;
            //z = floor((z * 2) + 0.5) / 2;

            // Round number to 1 decimal place
            x = round(x * 10) / 10;
            y = round(y * 10) / 10;
            z = round(z * 10) / 10;

            if (pointExists(x, y, z))        // Check if that point already exist in hashmap
                continue;

            // Create a new point
            Point3D point;
            point.position = glm::vec3(x, y, z);
            //point.intensity = intensity;
            point.color = ofColor(r, g, b);

            addPoint(point);                // Add new point to hashmap

            // Write to new file
            outFile << x << " " << y << " " << z << " " << endl;
        }
    }
    else {
        cout << "Failed to open!" << endl;
    }

    cout << "Cleaning finished!\nOld data: " << count << " points\nNew data: " << pointMap.size() << " points\n";
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
    case OF_KEY_F1:
        wireframe = !wireframe;
        break;
    case OF_KEY_F2:
        vertices = !vertices;
        break;
    default:
        break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

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
