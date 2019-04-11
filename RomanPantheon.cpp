#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>
#include <glut.h>
#define PI 3.1415926535897932384626433832795

double cameraAngle, cameraAngleDelta;
int canDrawGrid;

double cameraHeight;
double cameraRadius;

int num_texture  = -1;
GLuint brickimg;
GLuint concrete;
GLuint concreteborder;
GLuint pillar;
GLuint roofborderfront;
GLuint roofborderleft;
GLuint rooftopfront;
GLuint border;
GLuint pillarborder;
GLuint brick2;
GLuint conpillar;
GLuint conpillarinner;
GLuint concretewall;
GLuint rooftop;
GLuint mainwall;
GLuint tomb;
GLuint backroof;
GLuint backwall;
GLuint backback;
GLuint cylwall1;
GLuint cylwall2;
GLuint roofborderleft3;
GLuint roof3front;
GLuint roof3left;
GLuint toproof;
GLuint pillartex;
GLuint horizon;
GLuint sky;

int LoadBitmapImage(char *filename) {
    int i, j=0;
    FILE *l_file;
    unsigned char *l_texture;

    BITMAPFILEHEADER fileheader;
    BITMAPINFOHEADER infoheader;
    RGBTRIPLE rgb;

    num_texture++;

    if((l_file = fopen(filename, "rb"))==NULL) return (-1);

    fread(&fileheader, sizeof(fileheader), 1, l_file);
    fseek(l_file, sizeof(fileheader), SEEK_SET);
    fread(&infoheader, sizeof(infoheader), 1, l_file);

    l_texture = (byte *) malloc(infoheader.biWidth * infoheader.biHeight * 4);
    memset(l_texture, 0, infoheader.biWidth * infoheader.biHeight * 4);
	for (i=0; i < infoheader.biWidth*infoheader.biHeight; i++) {
		fread(&rgb, sizeof(rgb), 1, l_file);
		l_texture[j+0] = rgb.rgbtRed;
		l_texture[j+1] = rgb.rgbtGreen;
		l_texture[j+2] = rgb.rgbtBlue;
		l_texture[j+3] = 255;
		j += 4;
	}
    fclose(l_file);

    glBindTexture(GL_TEXTURE_2D, num_texture);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

// glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, infoheader.biWidth, infoheader.biHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, l_texture);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 4, infoheader.biWidth, infoheader.biHeight, GL_RGBA, GL_UNSIGNED_BYTE, l_texture);
    free(l_texture);
    return (num_texture);
}

void loadImage() {
	brickimg = LoadBitmapImage("image/brick.bmp");
	concrete = LoadBitmapImage("image/concrete.bmp");
	concreteborder = LoadBitmapImage("image/concreteborder.bmp");
	pillar = LoadBitmapImage("image/pillar.bmp");
	roofborderfront = LoadBitmapImage("image/roofborderfront.bmp");
	roofborderleft = LoadBitmapImage("image/roofborderleft.bmp");
	rooftopfront = LoadBitmapImage("image/rooftopfront.bmp");
	border = LoadBitmapImage("image/border.bmp");
	pillarborder = LoadBitmapImage("image/pillarborder.bmp");
	brick2 = LoadBitmapImage("image/brick2.bmp");
	conpillar = LoadBitmapImage("image/conpillar.bmp");
	conpillarinner = LoadBitmapImage("image/conpillarinner.bmp");
	concretewall = LoadBitmapImage("image/concretewall.bmp");
	rooftop = LoadBitmapImage("image/rooftop.bmp");
	mainwall = LoadBitmapImage("image/mainwall.bmp");
	tomb = LoadBitmapImage("image/tomb.bmp");
	backroof = LoadBitmapImage("image/backroof.bmp");
	backwall = LoadBitmapImage("image/backwall.bmp");
	backback = LoadBitmapImage("image/backback.bmp");
	cylwall1 = LoadBitmapImage("image/cylwall1.bmp");
	cylwall2 = LoadBitmapImage("image/cylwall2.bmp");
	roofborderleft3 = LoadBitmapImage("image/roofborderleft3.bmp");
	roof3front = LoadBitmapImage("image/roof3front.bmp");
	roof3left = LoadBitmapImage("image/roof3left.bmp");
	toproof = LoadBitmapImage("image/toproof.bmp");
	pillartex = LoadBitmapImage("image/pillartex.bmp");
	horizon = LoadBitmapImage("image/horizon.bmp");
	sky = LoadBitmapImage("image/sky.bmp");

	printf("Load successful");
}

void display(){
	//codes for Models, Camera

	//clear the display
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		//clear buffers to preset values

	/***************************
	/ set-up camera (view) here
	****************************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);		//specify which matrix is the current matrix

	//initialize the matrix
	glLoadIdentity();				//replace the current matrix with the identity matrix [Diagonals have 1, others have 0]

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(0,-150,20,	0,0,0,	0,0,1);
	gluLookAt(cameraRadius*sin(cameraAngle), -cameraRadius*cos(cameraAngle), cameraHeight,		0,0,0,		0,0,1);

	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/**************************************************
	/ Grid and axes Lines(You can remove them if u want)
	***************************************************/


	/****************************
	/ Add your objects from here
	****************************/
	//GROUND
	glEnable(GL_TEXTURE_2D); {
		glBindTexture(GL_TEXTURE_2D, brick2);
		glBegin(GL_QUADS); {
			glColor3f(1.0, 1.0, 1.0);
			glTexCoord2f(0, 1);
			glVertex3f(-500, 500, 0);
			glTexCoord2f(0, 0);
			glVertex3f(-500, -500, 0);
			glTexCoord2f(1, 0);
			glVertex3f(500, -500, 0);
			glTexCoord2f(1, 1);
			glVertex3f(500, 500, 0);
		} glEnd();
	}
	glDisable(GL_TEXTURE_2D);
	//BASE
	glEnable(GL_TEXTURE_2D); {
		glBindTexture(GL_TEXTURE_2D, concrete);
		glBegin(GL_QUADS); {
			glColor3f(1.0, 1.0, 1.0);
			glTexCoord2f(0, 1);
			glVertex3f(-100, 70, 5);
			glTexCoord2f(0, 0);
			glVertex3f(-100, -55, 5);
			glTexCoord2f(1, 0);
			glVertex3f(100, -55, 5);
			glTexCoord2f(1, 1);
			glVertex3f(100, 70, 5);
		} glEnd();
	}
	glDisable(GL_TEXTURE_2D);
	//BASE BORDER BACK
	glEnable(GL_TEXTURE_2D); {
		glBindTexture(GL_TEXTURE_2D, concrete);
		glBegin(GL_QUADS); {
			glColor3f(1.0, 1.0, 1.0);
			glTexCoord2f(0, 1);
			glVertex3f(-100, 70, 5);
			glTexCoord2f(0, 0);
			glVertex3f(100, 70, 5);
			glTexCoord2f(1, 0);
			glVertex3f(100, 70, 0);
			glTexCoord2f(1, 1);
			glVertex3f(-100, 70, 0);
		} glEnd();
	}
	glDisable(GL_TEXTURE_2D);
	//BASE BORDER FRONT
	glEnable(GL_TEXTURE_2D); {
		glBindTexture(GL_TEXTURE_2D, concreteborder);
		glBegin(GL_QUADS); {
			glColor3f(1.0, 1.0, 1.0);
			glTexCoord2f(1, 0);
			glVertex3f(-100, -55, 5);
			glTexCoord2f(0, 0);
			glVertex3f(100, -55, 5);
			glTexCoord2f(0, 1);
			glVertex3f(100, -55, 0);
			glTexCoord2f(1, 1);
			glVertex3f(-100, -55, 0);
		} glEnd();
	}
	glDisable(GL_TEXTURE_2D);
	//BASE BORDER LEFT-RIGHT
	for(int blr = -100; blr <= 100; blr = blr + 200) {
		glEnable(GL_TEXTURE_2D); {
			glBindTexture(GL_TEXTURE_2D, concreteborder);
			glBegin(GL_QUADS); {
				glColor3f(1.0, 1.0, 1.0);
				glTexCoord2f(1, 0);
				glVertex3f(blr, -55, 5);
				glTexCoord2f(0, 0);
				glVertex3f(blr, 70, 5);
				glTexCoord2f(0, 1);
				glVertex3f(blr, 70, 0);
				glTexCoord2f(1, 1);
				glVertex3f(blr, -55, 0);
			} glEnd();
		}
		glDisable(GL_TEXTURE_2D);
	}
	//PILLAR
	glColor3f(1.0, 1.0, 1.0);
	for(float c = -45; c < 10; c = c + 25.8) {
		for(float r = -90; r < 100; r = r + 25.8) {
			glPushMatrix(); {
				glTranslatef(r, c, 0);
				glEnable(GL_TEXTURE_2D); {
					glBindTexture(GL_TEXTURE_2D, pillar);
					GLUquadricObj *obj = gluNewQuadric();
					gluQuadricTexture(obj, GL_TRUE);
					gluCylinder(obj, 5, 5, 90, 20, 20);
				} glDisable(GL_TEXTURE_2D);
			} glPopMatrix();
			//PILLAR BASE
			glPushMatrix(); {
				glColor3f(0.59, 0.58, 0.51);
				glTranslatef(r, c, 5);
				glScalef(11, 11, 5);
				glutSolidCube(1);
			} glPopMatrix();
			//PILLAR TOP
			glPushMatrix(); {
				glColor3f(1.0, 1.0, 1.0);
				glTranslatef(r, c, 80);
				glEnable(GL_TEXTURE_2D); {
					glBindTexture(GL_TEXTURE_2D, border);
					GLUquadricObj *obj = gluNewQuadric();
					gluQuadricTexture(obj, GL_TRUE);
					gluCylinder(obj, 5, 7, 10, 20, 20);
				} glDisable(GL_TEXTURE_2D);
			} glPopMatrix();
		}
	}
	//ROOF BASE
	int rr;
	for(int rb = 90; rb <= 105; rb = rb + 15) {
		if(rb == 90) rr = 98;
		else rr = 100;
		glEnable(GL_TEXTURE_2D); {
			glBindTexture(GL_TEXTURE_2D, concrete);
			glBegin(GL_QUADS); {
				glColor3f(1.0, 1.0, 1.0);
				glTexCoord2f(0, 1);
				glVertex3f(-rr, 70, rb);
				glTexCoord2f(0, 0);
				glVertex3f(-rr, -55, rb);
				glTexCoord2f(1, 0);
				glVertex3f(rr, -55, rb);
				glTexCoord2f(1, 1);
				glVertex3f(rr, 70, rb);
			} glEnd();
		}
		glDisable(GL_TEXTURE_2D);
	}
	//ROOF BORDER LEFT-RIGHT
	for(int blr = -98; blr <= 98; blr = blr + 196) {
		glEnable(GL_TEXTURE_2D); {
			glBindTexture(GL_TEXTURE_2D, roofborderleft);
			glBegin(GL_QUADS); {
				glColor3f(1.0, 1.0, 1.0);
				glTexCoord2f(1, 1);
				glVertex3f(blr, -55, 105);
				glTexCoord2f(0, 1);
				glVertex3f(blr, 14, 105);
				glTexCoord2f(0, 0);
				glVertex3f(blr, 14, 90);
				glTexCoord2f(1, 0);
				glVertex3f(blr, -55, 90);
			} glEnd();
		}
		glDisable(GL_TEXTURE_2D);
	}
	//ROOF BORDER LEFT-RIGHT EXTENDED
	for(int blr = -98; blr <= 98; blr = blr + 196) {
		glEnable(GL_TEXTURE_2D); {
			glBindTexture(GL_TEXTURE_2D, roofborderleft);
			glBegin(GL_QUADS); {
				glColor3f(1.0, 1.0, 1.0);
				glTexCoord2f(1, 1);
				glVertex3f(blr, 11, 105);
				glTexCoord2f(0, 1);
				glVertex3f(blr, 70, 105);
				glTexCoord2f(0, 0);
				glVertex3f(blr, 70, 90);
				glTexCoord2f(1, 0);
				glVertex3f(blr, 11, 90);
			} glEnd();
		}
		glDisable(GL_TEXTURE_2D);
	}
	//ROOF BORDER LEFT-RIGHT EXTENDED PILLAR
	for(int blr = -95; blr <= 95; blr = blr + 190) {
		glEnable(GL_TEXTURE_2D); {
			glBindTexture(GL_TEXTURE_2D, conpillar);
			glBegin(GL_QUADS); {
				glColor3f(1.0, 1.0, 1.0);
				glTexCoord2f(1, 1);
				glVertex3f(blr, 20, 105);
				glTexCoord2f(0, 1);
				glVertex3f(blr, 70, 105);
				glTexCoord2f(0, 0);
				glVertex3f(blr, 70, 0);
				glTexCoord2f(1, 0);
				glVertex3f(blr, 20, 0);
			} glEnd();
		}
		glDisable(GL_TEXTURE_2D);
	}
	//ROOF BORDER LEFT-RIGHT EXTENDED PILLAR INNER
	for(int blr = -85; blr <= 85; blr = blr + 170) {
		glEnable(GL_TEXTURE_2D); {
			glBindTexture(GL_TEXTURE_2D, conpillar);
			glBegin(GL_QUADS); {
				glColor3f(1.0, 1.0, 1.0);
				glTexCoord2f(1, 1);
				glVertex3f(blr, 20, 105);
				glTexCoord2f(0, 1);
				glVertex3f(blr, 70, 105);
				glTexCoord2f(0, 0);
				glVertex3f(blr, 70, 0);
				glTexCoord2f(1, 0);
				glVertex3f(blr, 20, 0);
			} glEnd();
		}
		glDisable(GL_TEXTURE_2D);
	}
	//PILLAR BACK EXTENDED FRONT
	for(int bpef = -95; bpef < 95; bpef = bpef + 190) {
		glEnable(GL_TEXTURE_2D); {
			glBindTexture(GL_TEXTURE_2D, conpillarinner);
			glBegin(GL_QUADS); {
				glColor3f(1.0, 1.0, 1.0);
				glTexCoord2f(1, 1);
				glVertex3f(bpef, 20, 90);
				glTexCoord2f(0, 1);
				glVertex3f(-85, 20, 90);
				glTexCoord2f(0, 0);
				glVertex3f(-85, 20, 0);
				glTexCoord2f(1, 0);
				glVertex3f(bpef, 20, 0);
			} glEnd();
		}
		glDisable(GL_TEXTURE_2D);
	}
	//PILLAR BACK EXTENDED BACK
	glEnable(GL_TEXTURE_2D); {
		glBindTexture(GL_TEXTURE_2D, concretewall);
		glBegin(GL_QUADS); {
			glColor3f(1.0, 1.0, 1.0);
			glTexCoord2f(1, 1);
			glVertex3f(-95, 70, 90);
			glTexCoord2f(0, 1);
			glVertex3f(95, 70, 90);
			glTexCoord2f(0, 0);
			glVertex3f(95, 70, 0);
			glTexCoord2f(1, 0);
			glVertex3f(-95, 70, 0);
		} glEnd();
	}
	glDisable(GL_TEXTURE_2D);
	//BACK PILLAR EXTENDED BACK
	glEnable(GL_TEXTURE_2D); {
		glBindTexture(GL_TEXTURE_2D, concrete);
		glBegin(GL_QUADS); {
			glColor3f(1.0, 1.0, 1.0);
			glTexCoord2f(1, 1);
			glVertex3f(98, 20, 90);
			glTexCoord2f(0, 1);
			glVertex3f(85, 20, 90);
			glTexCoord2f(0, 0);
			glVertex3f(85, 20, 0);
			glTexCoord2f(1, 0);
			glVertex3f(95, 20, 0);
		} glEnd();
	}
	glDisable(GL_TEXTURE_2D);
	//ROOF BORDER FRONT
	glEnable(GL_TEXTURE_2D); {
		glBindTexture(GL_TEXTURE_2D, roofborderfront);
		glBegin(GL_QUADS); {
			glColor3f(1.0, 1.0, 1.0);
			glTexCoord2f(1, 1);
			glVertex3f(-98, -55, 105);
			glTexCoord2f(0, 1);
			glVertex3f(98, -55, 105);
			glTexCoord2f(0, 0);
			glVertex3f(98, -55, 90);
			glTexCoord2f(1, 0);
			glVertex3f(-98, -55, 90);
		} glEnd();
	}
	glDisable(GL_TEXTURE_2D);
	//ROOFTOP 1
	glEnable(GL_TEXTURE_2D); {
		glBindTexture(GL_TEXTURE_2D, rooftop);
		glBegin(GL_QUADS); {
			glColor3f(1.0, 1.0, 1.0);
			glTexCoord2f(0, 1);
			glVertex3f(-100, 70, 105);
			glTexCoord2f(0, 0);
			glVertex3f(-100, -55, 105);
			glTexCoord2f(1, 0);
			glVertex3f(0, -55, 140);
			glTexCoord2f(1, 1);
			glVertex3f(0, 70, 140);
		} glEnd();
	}
	glDisable(GL_TEXTURE_2D);
	//ROOFTOP 2
	glEnable(GL_TEXTURE_2D); {
		glBindTexture(GL_TEXTURE_2D, rooftop);
		glBegin(GL_QUADS); {
			glColor3f(1.0, 1.0, 1.0);
			glTexCoord2f(0, 1);
			glVertex3f(100, 70, 105);
			glTexCoord2f(0, 0);
			glVertex3f(100, -55, 105);
			glTexCoord2f(1, 0);
			glVertex3f(0, -55, 140);
			glTexCoord2f(1, 1);
			glVertex3f(0, 70, 140);
		} glEnd();
	}
	glDisable(GL_TEXTURE_2D);
	//ROOFTOP FRONT
	glEnable(GL_TEXTURE_2D); {
		glBindTexture(GL_TEXTURE_2D, rooftopfront);

		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_TRIANGLES); {
			glTexCoord2f(0, 0);
			glVertex3f(-100, -53, 105);
			glTexCoord2f(1, 0);
			glVertex3f(100, -53, 105);
			glTexCoord2f(0.5, 1);
			glVertex3f(0, -53, 140);
		} glEnd();
	}
	glDisable(GL_TEXTURE_2D);
	//ROOF TOP 2 LEFT-RIGHT
	for(int blr = -95; blr <= 95; blr = blr + 190) {
		glEnable(GL_TEXTURE_2D); {
			glBindTexture(GL_TEXTURE_2D, roofborderleft);
			glBegin(GL_QUADS); {
				glColor3f(1.0, 1.0, 1.0);
				glTexCoord2f(1, 1);
				glVertex3f(blr, 20, 120);
				glTexCoord2f(0, 1);
				glVertex3f(blr, 70, 120);
				glTexCoord2f(0, 0);
				glVertex3f(blr, 70, 105);
				glTexCoord2f(1, 0);
				glVertex3f(blr, 20, 105);
			} glEnd();
		}
		glDisable(GL_TEXTURE_2D);
	}
	//ROOF TOP 2 FRONT
	glEnable(GL_TEXTURE_2D); {
		glBindTexture(GL_TEXTURE_2D, roofborderleft3);
		glBegin(GL_QUADS); {
			glColor3f(1.0, 1.0, 1.0);
			glTexCoord2f(1, 1);
			glVertex3f(-95, 20, 120);
			glTexCoord2f(0, 1);
			glVertex3f(95, 20, 120);
			glTexCoord2f(0, 0);
			glVertex3f(95, 20, 100);
			glTexCoord2f(1, 0);
			glVertex3f(-95, 20, 100);
		} glEnd();
	glDisable(GL_TEXTURE_2D);
	}
	//ROOFTOP 2 FRONT
	glEnable(GL_TEXTURE_2D); {
		glBindTexture(GL_TEXTURE_2D, rooftopfront);

		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_TRIANGLES); {
			glTexCoord2f(0, 0);
			glVertex3f(-95, 20, 120);
			glTexCoord2f(1, 0);
			glVertex3f(95, 20, 120);
			glTexCoord2f(0.5, 1);
			glVertex3f(0, 20, 155);
		} glEnd();
	}
	glDisable(GL_TEXTURE_2D);
	//ROOFTOP_2 1
	glEnable(GL_TEXTURE_2D); {
		glBindTexture(GL_TEXTURE_2D, rooftop);
		glBegin(GL_QUADS); {
			glColor3f(1.0, 1.0, 1.0);
			glTexCoord2f(0, 1);
			glVertex3f(-95, 20, 120);
			glTexCoord2f(0, 0);
			glVertex3f(-95, 70, 120);
			glTexCoord2f(1, 0);
			glVertex3f(0, 70, 155);
			glTexCoord2f(1, 1);
			glVertex3f(0, 20, 155);
		} glEnd();
	}
	glDisable(GL_TEXTURE_2D);
	//ROOFTOP_2 2
	glEnable(GL_TEXTURE_2D); {
		glBindTexture(GL_TEXTURE_2D, rooftop);
		glBegin(GL_QUADS); {
			glColor3f(1.0, 1.0, 1.0);
			glTexCoord2f(0, 1);
			glVertex3f(95, 20, 120);
			glTexCoord2f(0, 0);
			glVertex3f(95, 70, 120);
			glTexCoord2f(1, 0);
			glVertex3f(0, 70, 155);
			glTexCoord2f(1, 1);
			glVertex3f(0, 20, 155);
		} glEnd();
	}
	glDisable(GL_TEXTURE_2D);
	//ROOFTOP 3 LEFT-RIGHT
	for(int blr = -92; blr <= 92; blr = blr + 184) {
		glEnable(GL_TEXTURE_2D); {
			glBindTexture(GL_TEXTURE_2D, roof3left);
			glBegin(GL_QUADS); {
				glColor3f(1.0, 1.0, 1.0);
				glTexCoord2f(1, 1);
				glVertex3f(blr, 22, 160);
				glTexCoord2f(0, 1);
				glVertex3f(blr, 70, 160);
				glTexCoord2f(0, 0);
				glVertex3f(blr, 70, 120);
				glTexCoord2f(1, 0);
				glVertex3f(blr, 22, 120);
			} glEnd();
		}
		glDisable(GL_TEXTURE_2D);
	}
	//ROOFTOP 3 FRONT
	glEnable(GL_TEXTURE_2D); {
		glBindTexture(GL_TEXTURE_2D, roof3front);
		glBegin(GL_QUADS); {
			glColor3f(1.0, 1.0, 1.0);
			glTexCoord2f(1, 1);
			glVertex3f(-92, 22, 160);
			glTexCoord2f(0, 1);
			glVertex3f(92, 22, 160);
			glTexCoord2f(0, 0);
			glVertex3f(92, 22, 120);
			glTexCoord2f(1, 0);
			glVertex3f(-92, 22, 120);
		} glEnd();
	glDisable(GL_TEXTURE_2D);
	}
	//ROOFTOP 3 TOP
	glEnable(GL_TEXTURE_2D); {
		glBindTexture(GL_TEXTURE_2D, toproof);
		glBegin(GL_QUADS); {
			glColor3f(1.0, 1.0, 1.0);
			glTexCoord2f(0, 1);
			glVertex3f(-92, 70, 160);
			glTexCoord2f(0, 0);
			glVertex3f(-92, 22, 160);
			glTexCoord2f(1, 0);
			glVertex3f(92, 22, 160);
			glTexCoord2f(1, 1);
			glVertex3f(92, 70, 160);
		} glEnd();
	}
	glDisable(GL_TEXTURE_2D);
	//MAIN UNIT FRONT CLIPPING
	glPushMatrix();{
		glColor3f(1.0, 0.0, 0.0);
		double equ[4] = {0, 1, 0, -54};
		glClipPlane(GL_CLIP_PLANE0, equ);
		glEnable(GL_CLIP_PLANE0);
			//MAIN UNIT WALL
			glColor3f(1.0, 1.0, 1.0);
			glTranslatef(0, 185, 0);
			glEnable(GL_TEXTURE_2D); {
				glBindTexture(GL_TEXTURE_2D, cylwall1);
				GLUquadricObj *obj = gluNewQuadric();
				gluQuadricTexture(obj, GL_TRUE);
				gluCylinder(obj, 150, 150, 160, 150, 150);
			} glDisable(GL_TEXTURE_2D);
		glDisable(GL_CLIP_PLANE0);
	}glPopMatrix();
	//MAIN ROOF TOP
	glPushMatrix(); {
		GLfloat twicePi = 2.0f * 3.1416;
		glTranslatef(0, 185, 160);
		glColor3f(0.78, 0.73, 0.72);
		glEnable(GL_TEXTURE_2D); {
			glBindTexture(GL_TEXTURE_2D, mainwall);
			glBegin(GL_TRIANGLE_FAN);
			glVertex2f(0, 0); // center of circle
			for(int i = 0; i <= 150; i++) {
				glVertex2f(
					0 + (150 * cos(i *  twicePi / 150)),
					0 + (150 * sin(i * twicePi / 150))
				);
			}
			glEnd();
		} glDisable(GL_TEXTURE_2D);
	} glPopMatrix();
	//INNER TOMB LOOP
	int top = 135, bot = 135;
	float height = 167;
		for(int i = 0; i < 7; i++) {
			glPushMatrix(); {
				double equ[4] = {0, 1, 0, -54};
				glClipPlane(GL_CLIP_PLANE0, equ);
				glEnable(GL_CLIP_PLANE0);
					glTranslatef(0, 190, 0);
					glEnable(GL_TEXTURE_2D); {
						glBindTexture(GL_TEXTURE_2D, cylwall2);
						GLUquadricObj *obj = gluNewQuadric();
						gluQuadricTexture(obj, GL_TRUE);
						gluCylinder(obj, top, bot, height, 130, 130);
					} glDisable(GL_TEXTURE_2D);
				glDisable(GL_CLIP_PLANE0);
			} glPopMatrix();
		top = top - 6;
		bot = bot - 6;
		height = height + 2;
	}
	//INNER ROOF LOOP
	int rad = 135, zind = 167;
	for(int i = 0; i < 7; i++) {
		glPushMatrix(); {
			GLfloat twicePi = 2.0f * 3.1416;
			glTranslatef(0, 190, zind);
			glColor3f(0.78, 0.73, 0.72);
			glEnable(GL_TEXTURE_2D); {
				glBindTexture(GL_TEXTURE_2D, mainwall);
				glBegin(GL_TRIANGLE_FAN);
				glVertex2f(0, 0); // center of circle
				for(int i = 0; i <= rad; i++) {
					glVertex2f(
						0 + (rad * cos(i *  twicePi / rad)),
						0 + (rad * sin(i * twicePi / rad))
					);
				}
				glEnd();
			} glDisable(GL_TEXTURE_2D);
		} glPopMatrix();
		rad = rad - 6;
		zind = zind + 2;
	}
	//MAIN TOMB
	glPushMatrix(); {
		double equ[4] = {0, 0, 1, 0};
		glClipPlane(GL_CLIP_PLANE0, equ);
		glEnable(GL_CLIP_PLANE0);
		glTranslatef(0, 185, 80);
		glColor3f(1.0, 1.0, 1.0);
		glEnable(GL_TEXTURE_2D); {
			glBindTexture(GL_TEXTURE_2D, tomb);
			GLUquadricObj *obj = gluNewQuadric();
			gluQuadricTexture(obj, GL_TRUE);
			gluSphere(obj, 130, 140, 140);
		} glDisable(GL_TEXTURE_2D);
		glDisable(GL_CLIP_PLANE0);
	} glPopMatrix();

	//BACK PART
	//BACK WALL
	glEnable(GL_TEXTURE_2D); {
		glBindTexture(GL_TEXTURE_2D, backback);
		glBegin(GL_QUADS); {
			glColor3f(1.0, 1.0, 1.0);
			glTexCoord2f(1, 0);
			glVertex3f(-95, 360, 105);
			glTexCoord2f(0, 0);
			glVertex3f(-95, 360, 0);
			glTexCoord2f(0, 1);
			glVertex3f(95, 360, 0);
			glTexCoord2f(1, 1);
			glVertex3f(95, 360, 105);
		} glEnd();
	}
	glDisable(GL_TEXTURE_2D);
	//BACK UNIT LEFT-RIGHT WALL
	for(int blr = -95; blr <= 95; blr = blr + 190) {
		glEnable(GL_TEXTURE_2D); {
			glBindTexture(GL_TEXTURE_2D, backwall);
			glBegin(GL_QUADS); {
				glColor3f(1.0, 1.0, 1.0);
				glTexCoord2f(1, 0);
				glVertex3f(blr, 310, 105);
				glTexCoord2f(1, 1);
				glVertex3f(blr, 360, 105);
				glTexCoord2f(0, 1);
				glVertex3f(blr, 360, 0);
				glTexCoord2f(0, 0);
				glVertex3f(blr, 310, 0);
			} glEnd();
		}
		glDisable(GL_TEXTURE_2D);
	}
	//BACK UNIT ROOF
	glEnable(GL_TEXTURE_2D); {
		glBindTexture(GL_TEXTURE_2D, backroof);
		glBegin(GL_QUADS); {
			glColor3f(1.0, 1.0, 1.0);
			glTexCoord2f(0, 1);
			glVertex3f(-95, 360, 105);
			glTexCoord2f(0, 0);
			glVertex3f(-95, 300, 105);
			glTexCoord2f(1, 0);
			glVertex3f(95, 300, 105);
			glTexCoord2f(1, 1);
			glVertex3f(95, 360, 105);
		} glEnd();
	}
	glDisable(GL_TEXTURE_2D);


	//HORIZON BACK
	glEnable(GL_TEXTURE_2D); {
		glBindTexture(GL_TEXTURE_2D, horizon);
		glBegin(GL_QUADS); {
			glColor3f(1.0, 1.0, 1.0);
			glTexCoord2f(1, 1);
			glVertex3f(-500, 500, 200);
			glTexCoord2f(0, 1);
			glVertex3f(500, 500, 200);
			glTexCoord2f(0, 0);
			glVertex3f(500, 500, 0);
			glTexCoord2f(1, 0);
			glVertex3f(-500, 500, 0);
		} glEnd();
	}
    //HORIZON FRONT
	glEnable(GL_TEXTURE_2D); {
		glBindTexture(GL_TEXTURE_2D, horizon);
		glBegin(GL_QUADS); {
			glColor3f(1.0, 1.0, 1.0);
			glTexCoord2f(1, 1);
			glVertex3f(500, -500, 200);
			glTexCoord2f(0, 1);
			glVertex3f(-500, -500, 200);
			glTexCoord2f(0, 0);
			glVertex3f(-500, -500, 0);
			glTexCoord2f(1, 0);
			glVertex3f(500, -500, 0);
		} glEnd();
	}
	//HORIZON LEFT RIGHT
	for(int i = -500; i <= 500; i = i + 1000) {
		glEnable(GL_TEXTURE_2D); {
			glBindTexture(GL_TEXTURE_2D, horizon);
			glBegin(GL_QUADS); {
				glColor3f(1.0, 1.0, 1.0);
				glTexCoord2f(1, 1);
				glVertex3f(i, -500, 200);
				glTexCoord2f(0, 1);
				glVertex3f(i, 500, 200);
				glTexCoord2f(0, 0);
				glVertex3f(i, 500, 0);
				glTexCoord2f(1, 0);
				glVertex3f(i, -500, 0);
			} glEnd();
		}
	}
	//SKY
	glPushMatrix(); {
		double equ[4] = {0, 0, 1, -150};
		glClipPlane(GL_CLIP_PLANE0, equ);
		glEnable(GL_CLIP_PLANE0);
		glTranslatef(0, 0, 0);
		glColor3f(1.0, 1.0, 1.0);
		glEnable(GL_TEXTURE_2D); {
			glBindTexture(GL_TEXTURE_2D, sky);
			GLUquadricObj *obj = gluNewQuadric();
			gluQuadricTexture(obj, GL_TRUE);
			gluSphere(obj, 1000, 500, 500);
		} glDisable(GL_TEXTURE_2D);
		glDisable(GL_CLIP_PLANE0);
	} glPopMatrix();


	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}

void animate(){
	//codes for any changes in Models, Camera

	cameraAngle += cameraAngleDelta;	// camera will rotate at 0.002 radians per frame.
	//codes for any changes in Models

	//MISSING SOMETHING? -- YES: add the following
	glutPostRedisplay();	//this will call the display AGAIN

}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':	//reverse the rotation of camera
			cameraAngleDelta = -cameraAngleDelta;
			break;

		case '2':	//increase rotation of camera by 10%
			cameraAngleDelta *= 1.1;
			break;

		case '3':	//decrease rotation
			cameraAngleDelta /= 1.1;
			break;

		case '8':	//toggle grids
			canDrawGrid = 1 - canDrawGrid;
			break;

		case 'p':	//toggle grids
			cameraAngleDelta = 0;
			break;
		case 'r':	//toggle grids
			cameraAngleDelta = .001;
			break;

		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			cameraRadius += 10;
			break;
		case GLUT_KEY_UP:		// up arrow key
			if(cameraRadius > 10)
				cameraRadius -= 10;
			break;

		case GLUT_KEY_RIGHT:
			cameraAngle += 0.05;
			break;
		case GLUT_KEY_LEFT:
			cameraAngle -= 0.05;
			break;

		case GLUT_KEY_PAGE_UP:
			cameraHeight += 10;
			break;
		case GLUT_KEY_PAGE_DOWN:
			cameraHeight -= 10;
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				cameraAngleDelta = -cameraAngleDelta;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}



void init(){
	//codes for initialization
	loadImage();
	cameraAngle = 0;	//angle in radian
	cameraAngleDelta = 0.001;
	canDrawGrid = 1;

	cameraHeight = 50;
	cameraRadius = 150;
	//clear the screen
	glClearColor(0,0,0, 0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	/*
		gluPerspective() — set up a perspective projection matrix

		fovy -         Specifies the field of view angle, in degrees, in the y direction.
        aspect ratio - Specifies the aspect ratio that determines the field of view in the x direction. The aspect ratio is the ratio of x (width) to y (height).
        zNear -        Specifies the distance from the viewer to the near clipping plane (always positive).
        zFar  -        Specifies the distance from the viewer to the far clipping plane (always positive).
    */

	gluPerspective(70,	1,	0.1,	10000.0);

}

int main(int argc, char **argv){

	glutInit(&argc,argv);							//initialize the GLUT library

	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);

	/*
		glutInitDisplayMode - inits display mode
		GLUT_DOUBLE - allows for display on the double buffer window
		GLUT_RGBA - shows color (Red, green, blue) and an alpha
		GLUT_DEPTH - allows for depth buffer
	*/
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);

	glutCreateWindow("Some Title");

	init();						//codes for initialization

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	//ADD keyboard listeners:
	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);

	//ADD mouse listeners:
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
