#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofEnableDepthTest();
	
	ofFbo fbo;
	fbo.allocate(ofGetWidth(), ofGetHeight());
	fbo.begin();
	ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5);
	ofClear(0);
	ofSetColor(0);

	ofTrueTypeFont font;
	font.loadFont("fonts/Kazesawa-Bold.ttf", 400, true, true, true);

	string word = "4";
	font.drawString(word, font.stringWidth(word) * -0.5, font.stringHeight(word));

	fbo.end();

	auto span = 15;

	ofPixels pixels;
	fbo.readToPixels(pixels);
	for (int x = 0; x < fbo.getWidth(); x += span) {

		for (int y = 0; y < fbo.getHeight(); y += span) {

			ofColor color = pixels.getColor(x, y);
			if (color != ofColor(0, 0)) {

				for (int z = -span; z <= span; z += span) {

					this->base_mesh.addVertex(glm::vec3(x - ofGetWidth() * 0.5, ofGetHeight() - y - ofGetWidth() * 0.25, z));
				}
			}
		}
	}

	this->draw_mesh.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);

	ofSetColor(39);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->draw_mesh.clear();

	for (int i = 0; i < this->base_mesh.getNumVertices(); i++) {

		auto location = this->base_mesh.getVertex(i);
		auto param = int(ofGetFrameNum() * 2 + location.y) % 600;
		if (param > 300) {

			auto angle = ofMap(param, 300, 600, 0, PI * 2);
			auto rotation = glm::rotate(glm::mat4(), angle, glm::vec3(0, 1, 0));
			location = glm::vec4(location, 0) * rotation;
		}

		this->draw_mesh.addVertex(location);
	}

	for (int i = 0; i < this->draw_mesh.getNumVertices(); i++) {

		for (int k = 0; k < this->draw_mesh.getNumVertices(); k++) {

			if (i == k) { continue; }

			if (glm::fastDistance(this->draw_mesh.getVertex(i), this->draw_mesh.getVertex(k)) < 20) {

				this->draw_mesh.addIndex(i); this->draw_mesh.addIndex(k);
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();

	this->draw_mesh.drawWireframe();
	for (auto& vertex : this->draw_mesh.getVertices()) {

		ofDrawSphere(vertex, 2);
	}

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}