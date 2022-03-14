#define _CRT_SECURE_NO_WARNINGS

#pragma region INCLUDES
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <cstring>

#ifdef _WIN32
#include "Include/glew.h"
#include "Include/freeglut.h"
#elif __linux__
#include "GL/glew.h"
#include "GL/glut.h"
#elif __APPLE__
#include "Include/glew.h"
#include "Include/freeglut.h"
#endif

#include "help.h"
#include "window.h"
#include "input.h"
#include "light.h"
#include "wavefront.h"
#include "image.h"
#include "vector.h"
#pragma endregion

void setCamera();		// Kamera platzieren, siehe Maus-Callbacks
void drawScene();		// Zeichnet die Szene im Weltkoordinatensystem

void loadObjects();
void initTextures();

/////////////////////////////////////////////////////////////////////////////////
//	Kamerafunktion
/////////////////////////////////////////////////////////////////////////////////
void setCamera()
{
	cg_mouse mouse;
	// Ansichtstransformationen setzen,
	// SetCamera() zum Beginn der Zeichenfunktion aufrufen
	double x, y, z, The, Phi;
	static double radius = 10;
	// Maus abfragen
	if (cg_mouse::buttonState(GLUT_LEFT_BUTTON))
	{
		cg_globState::cameraHelper[0] += mouse.moveX();
		cg_globState::cameraHelper[1] += mouse.moveY();
	}
	if (cg_mouse::buttonState(GLUT_MIDDLE_BUTTON))
	{
		radius += 0.1 * mouse.moveY();
		if (radius < 1.0) radius = 1.0;
	}
	Phi = 0.2 * cg_globState::cameraHelper[0] / cg_globState::screenSize[0] * M_PI + M_PI * 0.5;
	The = 0.2 * cg_globState::cameraHelper[1] / cg_globState::screenSize[1] * M_PI;
	x = radius * cos(Phi) * cos(The);
	y = radius * sin(The);
	z = radius * sin(Phi) * cos(The);
	cg_globState::cameraPos[0] = x; cg_globState::cameraPos[1] = z;
	int Oben = (The <= 0.5 * M_PI || The > 1.5 * M_PI) * 2 - 1;

	// globale, mausgesteuerte Sicht
	gluLookAt(x, y, z, 0, 0, 0, 0, Oben, 0);
}

/////////////////////////////////////////////////////////////////////////////////
//	main : Anfang des OpenGL Programmes
/////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
	init(argc, argv);

	// Programm startet im Beleuchtungsmodus
	cg_globState::lightMode = true;

	// hier Objekte laden, erstellen etc.
	loadObjects();

	// Texturen laden und vorbereiten
	initTextures();

	// Die Hauptschleife starten
	glutMainLoop();
	return 0;
}

void displayFunc()
{
	// Hilfe-Instanzen
	cg_help help;
	cg_globState globState;
	cg_key key;

	// Tastatur abfragen
	// Achtung: einmaliges Bet�tigen funktioniert so nur mit glutIgnoreKeyRepeat(true) (siehe main())
	if (key.keyState(27))
	{
		exit(0); // Escape -> Programm beenden
	}
	else if (1 == key.keyState('f') || 1 == key.keyState('F'))
	{
		help.toggleFps();	// Framecounter on/off
	}
	else if (1 == key.keyState('h') || 1 == key.keyState('H') || 1 == key.specialKeyState(GLUT_KEY_F1))
	{
		help.toggle();	// Hilfetext on/off
	}
	else if (1 == key.keyState('k') || 1 == key.keyState('K'))
	{
		help.toggleKoordsystem();	// Koordinatensystem on/off
	}
	else if (1 == key.keyState('w') || 1 == key.keyState('W'))
	{
		globState.drawMode = (globState.drawMode == GL_FILL) ? GL_LINE : GL_FILL; // Wireframe on/off
	}
	else if (1 == key.keyState('l') || 1 == key.keyState('L'))
	{
		globState.lightMode = !globState.lightMode;	// Beleuchtung on/off
	}
	else if (1 == key.keyState('i') || 1 == key.keyState('I'))
	{
		globState.cameraHelper[0] = 0;	// Initialisierung der Kamera
		globState.cameraHelper[1] = 0;
	}

	// Szene zeichnen: CLEAR, SETCAMERA, DRAW_SCENE

	// Back-Buffer neu initialisieren
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Kamera setzen (spherische Mausnavigation)
	setCamera();

	// Koordinatensystem zeichnen
	help.drawKoordsystem(-8, 10, -8, 10, -8, 10);

	// Zeichenmodus einstellen (Wireframe on/off)
	glPolygonMode(GL_FRONT_AND_BACK, globState.drawMode);

	// Backface Culling on/off, Standardwert ist Entfernen der R�ckseiten
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	if (globState.cullMode) glEnable(GL_CULL_FACE);
	else glDisable(GL_CULL_FACE);

	// Farbmodus oder Beleuchtungsmodus ?
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	if (globState.lightMode == GL_TRUE) // Beleuchtung aktivieren
	{
		float m_amb[4] = { 0.2, 0.2, 0.2, 1.0 };
		float m_diff[4] = { 0.2, 0.2, 0.6, 1.0 };
		float m_spec[4] = { 0.8, 0.8, 0.8, 1.0 };
		float m_shine = 32.0;
		float m_emiss[4] = { 0.0, 0.0, 0.0, 1.0 };

		setMaterial(GL_FRONT_AND_BACK, m_amb, m_diff, m_spec, m_shine, m_emiss);

		setLights();

		glEnable(GL_LIGHTING);
	}
	else   // Zeichnen im Farbmodus
	{
		glDisable(GL_LIGHTING);
		glColor4f(0.2, 0.2, 0.6, 1.0);
	}

	glEnable(GL_NORMALIZE);

	// die Szene zeichnen 
	drawScene();

	// den Hilfetext �ber die Szene zeichnen, wenn gefordert
	help.draw();

	// Wireframe deaktivieren
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDisable(GL_CULL_FACE);

	glFlush();				// Daten an Server (fuer die Darstellung) schicken
	glutSwapBuffers();		// Buffers wechseln
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �BUNG 12 - Blending ////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

//	Texturdefinition
#define NUM_TEXTURES 2

const char* texture_files[NUM_TEXTURES] = {
									"./textures/container.bmp",
									"./textures/opengl.bmp"
};

cg_image textures[NUM_TEXTURES];		// die GL Texturobjekte

// die Texturen laden und vorbereiten
void initTextures() {

	for (int i = 0; i < NUM_TEXTURES; i++)
	{
		bool success = false;
		success = textures[i].load(texture_files[i], false);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		if (!success) printf("Can not load texture %s!\n", texture_files[i]);
	}
}

#define num_objects 11	// wir haben 11 Wavefront Objekte
const char* objects_dir = "./Scene/";	// ... im Verzeichnis ./Scene
const char* objects_paths[num_objects] = { "chrome.obj", "top.obj", "glass.obj", "wheel_single.obj", "wheel_double.obj", "wheel_screws.obj",
											"axis.obj", "trailer.obj", "cargo_t.obj", "ground_concrete.obj", "ground_street.obj" };

cg_object3D objects[num_objects];
// Objektbezeichner f�r den Zugriff auf die Wavefront Objekte
enum {
	TRUCK_CHROME = 0,
	TRUCK_TOP,
	TRUCK_GLASS,
	WHEEL_SINGLE,
	WHEEL_DOUBLE,
	WHEEL_SCREWS,
	AXIS,

	TRAILER_CHASSIS,
	TRAILER_CARGO,

	GROUND_OBJ1,
	GROUND_OBJ2
};

void loadObjects()
{

	// alle Objekte Laden, wenn der Pfad verf�gbar ist (Pfad != "")
	for (int i = 0; i < num_objects; i++)
		if (strlen(objects_paths[i]) > 0)
		{
			char file[255] = "";
#ifndef _MSC_VER
			strcat(file, ".");
#endif // _MSC_VER
			strcat(file, objects_dir);
			strcat(file, objects_paths[i]);	// file enth�lt nun den vollst�ndigen Dateinamen

			// Hier das Objekt laden
			// --> Aufruf von loadobject(file, false) f�r objects[i]
			objects[i].load(file, true);
		}

	// nun setzen wir die Materialeigenschaften f�r die Objekte
	objects[TRUCK_CHROME].setMaterial(0.8, 0.8, 0.8, 1.0, 1.0, 120.0, 0.0);
	objects[TRUCK_TOP].setMaterial(0.9, 0.2, 0.3, 1.0, 0.5, 90.0, 0.0);

	// TODO U12.1: Die Scheiben mit einem ALPHA kleiner 1.0 versehen
	objects[TRUCK_GLASS].setMaterial(0.5, 0.5, 0.9, 1.0, 0.5, 120.0, 0.0);
	
	objects[WHEEL_SINGLE].setMaterial(0.2, 0.2, 0.2, 1.0, 0.0, 20.0, 0.0);
	objects[WHEEL_DOUBLE].setMaterial(0.2, 0.2, 0.2, 1.0, 0.0, 20.0, 0.0);
	objects[WHEEL_SCREWS].setMaterial(0.8, 0.8, 0.8, 1.0, 1.0, 20.0, 0.0);
	objects[AXIS].setMaterial(0.9, 0.3, 0.3, 1.0, 0.5, 80.0, 1.0);
	objects[TRAILER_CHASSIS].setMaterial(0.8, 0.8, 0.8, 1.0, 1.0, 128.0, 0.0);
	
	// TODO U12.2: Den Container mit einem ALPHA kleiner 1.0 versehen
	objects[TRAILER_CARGO].setMaterial(0.8, 0.8, 0.3, 1.0, 0.0, 64.0, 0.0);

	objects[GROUND_OBJ1].setMaterial(0.4, 0.4, 0.4, 1.0, 0.0, 128.0, 0.0);
	objects[GROUND_OBJ2].setMaterial(0.3, 0.3, 0.3, 1.0, 0.0, 128.0, 0.0);
}

void drawAxis(bool doubleWheel) {
	// eine Achse
	objects[AXIS].draw();

	// Aufruf f�r linkes Rad
	glPushMatrix();
		glTranslatef(-0.9f, 0.0, 0.0);    // Versatz nach Links (-x)
		objects[WHEEL_SCREWS].draw();
		if (doubleWheel)
			objects[WHEEL_DOUBLE].draw();
		else objects[WHEEL_SINGLE].draw();
	glPopMatrix();

	// Aufruf f�r rechts Rad
	glPushMatrix();
		glTranslatef(0.9, 0.0, 0.0);      // Versatz nach Rechts (+x)
		glRotatef(180.0, 0.0, 1.0, 0.0);  // 180� Drehung um Hochachse (Y)
		objects[WHEEL_SCREWS].draw();
		if (doubleWheel)
			objects[WHEEL_DOUBLE].draw();
		else
			objects[WHEEL_SINGLE].draw();
	glPopMatrix();
};

#pragma region VEHICLE
// Superklasse f�r die Fahrzeuge
class CVehicle {
protected: 
	CVector _pos = CVector(0, 0, 0);	//  Position [m]
	float _rot = 0;						//  Rotation [�]
public: 
	void setPos(CVector pos) { _pos = pos; }
	void setPos(float posX, float posZ) { _pos = CVector(posX, 0, posZ); }
	void setRot(float rot) { _rot = rot; }
	CVector getPos(void) { return _pos; }
	float getRot(void) { return _rot; }

	virtual void draw(void) { return; }
};

#pragma endregion

#pragma region TRUCK
// Klasse f�r den Truck
class CTruck : public CVehicle {

public:
	CTruck()	// constructor
	{
		_pos = CVector(0, 0, 0);
		_rot = 0;
	}

	void update()
	{
		// ...
	}

	void draw(void)
	{
		glPushMatrix();
		// Den Truck laut seinen Animationsparametern positionieren und ausrichten
		// pos[], rot
			glTranslatef(_pos.x(), 0.5, _pos.z());
			glRotatef(_rot, 0, 1, 0);

			// Vorderachse
			glPushMatrix();
				glTranslatef(0, 0, 6.2);
				drawAxis(false);
			glPopMatrix();

			// Hinterachse 1
			glPushMatrix();
				glTranslatef(0, 0, 0.7);
				drawAxis(true);
			glPopMatrix();

			// Hinterachse 2
			glPushMatrix();
				glTranslatef(0, 0, -0.4);
				drawAxis(true);
			glPopMatrix();

			// Chassis
			objects[TRUCK_TOP].draw();
			objects[TRUCK_CHROME].draw();

			// Fensterscheiben
				
			cg_globState globState;
			if (globState.blendMode) {
				// TODO U12.1: das OpenGL Blending aktivieren
				// glEnable(...);
				// glBlendFunc(...);
			}

			objects[TRUCK_GLASS].draw();

			// TODO U12:1 das Blending wieder deaktivieren
			// glDisable(...);

		glPopMatrix();
	}
};
#pragma endregion // TRUCK

#pragma region TRAILER
// Klasse f�r die Verwaltung eines Trailers
class CTrailer : public CVehicle {
private: 
	cg_object3D* _geometry;
	cg_image* _texture;

public: 
	CTrailer(cg_object3D* geometry, cg_image* texture) {
		_geometry = geometry;
		_texture = texture;
	}

	// Trailer zeichnen
	void draw(void) {
		glPushMatrix();
			// pos[], rot
			glTranslatef(_pos.x(), 0.5, _pos.z());
			glRotatef(_rot, 0, 1, 0);

			{ // Fahrwerk und Rahmen 
			// Achse 1
				glPushMatrix();
					glTranslatef(0, 0, -9.0);
					drawAxis(true);
				glPopMatrix();

				// Achse 2
				glPushMatrix();
					glTranslatef(0, 0, -10.2);
					drawAxis(true);
				glPopMatrix();

				// Rahmen + Aufbau
				objects[TRAILER_CHASSIS].draw();
			} // Fahrwerk und Rahmen

			{ // Container
				glPushMatrix();
					glTranslatef(0, 0.01, 0);

					// TODO U12.2: Den Container transparent zeichnen:
					// Blending aktivieren, siehe Aufgaben 1
					cg_globState globState;
					if (globState.blendMode) {
						// glEnable(...);
						// glBlendFunc(...);
					}

					// TODO U12.3: die Texturierung aktivieren und die Textur _texture binden,
					// vorher den gew�nschten Texturmodus einstellen, siehe �bung 11
					if (globState.textureMode) {
						// glEnable(...);

						// _texture->setEnvMode(...);
						// _texture->bind();	
					}

					// TODO U12:4: das CARGO-Objekt mittel Backface Culling korrekt darstellen

					// Der Quader ist ein konvexes Objekt. Damit lassen sich Fehler in der 
					// transparenten Darstellung mittels BackFace Culling adressieren.
					// Prinzip: zuerst die R�ckseiten zeichnen, damit werden nur die weiter 
					// entfernten Fl�chen gerastert, danach die Vorderseiten. 

					// wir merken uns den aktuellen Zustand des Cullings
					GLboolean cullingIsEnabled = false;
					glGetBooleanv(GL_CULL_FACE, &cullingIsEnabled);

					// Culling aktivieren f�r Vorderseiten
					// glEnable(...);
					// glCullFace(...);

					// die R�ckseiten zeichnen
					// _geometry->draw();  

					// Culling umschalten auf R�ckseiten
					// glCullFace(...);
					
					// die Vorderseiten zeichnen
					_geometry->draw();	

					// das Backface-Cullung wieder deaktivieren, falls es aus war
					if (!cullingIsEnabled)
						glDisable(GL_CULL_FACE);

					// TODO U12.3: die Texturierung deaktivieren
					// glDisable(...);

					// TODO U12:2 das Blending wieder deaktivieren
					// glDisable(...);
				glPopMatrix();
			} // Container

		glPopMatrix();
	};
}; // CTrailer
#pragma endregion // TRAILER

///////////////////////////////////////////////////////////////////////////////////////////////
// es folgt das Zeichnen der Fahrzeuge (hier ohne Animation) //////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

// globale Instanzen
CTruck truck;

// TODO U12.3: hier eine Textur w�hlen, Index 0 = container.bmp, index 1 = opengl.bmp
CTrailer trailer1(&objects[TRAILER_CARGO], &textures[0]);
CTrailer trailer2(&objects[TRAILER_CARGO], &textures[1]);

void drawScene()
{
	// Zeichnet die Szene 1x im Weltkoordinatensystem

	cg_globState globState;
	cg_key key;

	if (1 == key.keyState('t') || 1 == key.keyState('T'))
	{
		globState.textureMode = !globState.textureMode; // Texturierung on/off
	}
	else if (1 == key.keyState('b') || 1 == key.keyState('B'))
	{
		globState.blendMode = !globState.blendMode; // Blending on/off
	}

	// Stra�e bei Y=0 zeichnen
	objects[GROUND_OBJ1].draw();
	objects[GROUND_OBJ2].draw();

	// Truck und Trailer positionieren
	truck.setPos(-5, 5);
	truck.setRot(90);
	trailer1.setPos(-2, 0);
	trailer1.setRot(25);
	trailer2.setPos(2, -1);
	trailer2.setRot(-25);

	// TODO U12.2: Berechnen der Entfernungen
	// 1. Kamera-Truck
	// 2. Kamera-Trailer1
	// 3. Kamera-Trailer2
	// dann Zeichnen der Fahrzeuge von "hinten" nach "vorn"
	
	// die Kameraposition
	CVector camPos = CVector(cg_globState::cameraPos[0], 0, cg_globState::cameraPos[1]);

	// die Vektoren Kamera-->Objekt
	CVector vec_to_truck;
	CVector vec_to_trailer1;
	CVector vec_to_trailer2;

	// TODO U12.2 Schritt2 - Distanzvektoren bestrimmen
	// vec_to_truck = ...;
	// vec_to_trailer1 = ...;
	// vec_to_trailer2 = ...;

	// wir sortieren nun die Objekt-Indizes absteigend nach der Entfernung 
	const int numObjects = 3;

	// initial sind die Indizes der Objekte truck=0, trailer1=1 und trailer2=2 
	int objectIndexes[numObjects] = { 0,1,2 };

	// die Entfernungen zwischen Kamera und den Objekten
	float distances[numObjects];
	distances[0] = vec_to_truck.len();
	distances[1] = vec_to_trailer1.len();
	distances[2] = vec_to_trailer2.len();
	
	// Indizes nach absteigenden Entfernungen sortieren, Algorthmus bubblesort
	bool switched = false;
	do
	{
		switched = false;
		for (int i = 1; i < numObjects; i++)
		{
			if (distances[objectIndexes[i - 1]] < distances[objectIndexes[i]])
			{
				int tempi = objectIndexes[i];
				objectIndexes[i] = objectIndexes[i - 1];
				objectIndexes[i - 1] = tempi;
				switched = true;
			}
		}
	} while (switched);

	// TODO U12.2 Schritt3 - Zeichnen in der richtigen Reihenfolge
	// zun�chst das unsortierte Zeichnen entfernen
	truck.draw();
	trailer1.draw();
	trailer2.draw();
	
	// TODO U12.3: stattdessen sortiert zeichnen
	for (int i = 0; i < numObjects; i++) {
		switch (objectIndexes[i]) {
			case 0: // den Truck zeichnen 
					// ...
				break;
			case 1: // den Trailer1 zeichnen
					// ...
				break;
			case 2: // den Trailer 2 zeichnen
					// ...
				break;
		}
	}

	// Texturierung und Blending global deaktivieren
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glDisable(GL_CULL_FACE);
}

