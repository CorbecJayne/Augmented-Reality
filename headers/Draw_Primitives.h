
//#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>
#include <math.h>


/* PI */
#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif


void drawSphere(double r, int lats, int longs) {
	int i, j;
	for(i = 0; i <= lats; i++) {
		double lat0 = M_PI * (-0.5 + (double) (i - 1) / lats);
		double z0  = r * sin(lat0);
		double zr0 = r *  cos(lat0);

		double lat1 = M_PI * (-0.5 + (double) i / lats);
		double z1  = r * sin(lat1);
		double zr1 = r * cos(lat1);

		glBegin(GL_QUAD_STRIP);
		for(j = 0; j <= longs; j++) {
			double lng = 2 * M_PI * (double) (j - 1) / longs;
			double x = cos(lng);
			double y = sin(lng);

			glNormal3f(x * zr0, y * zr0, z0);
			glVertex3f(x * zr0, y * zr0, z0);
			glNormal3f(x * zr1, y * zr1, z1);
			glVertex3f(x * zr1, y * zr1, z1);
		}
		glEnd();
	}
}


void drawCone(GLdouble base, GLdouble height, GLint slices, GLint stacks)
{

    // draw the upper part of the cone
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0, 0, height);
    for (int angle = 0; angle < 360; angle++) {
        glVertex3f(sin((double)angle) * base, cos((double)angle) * base, 0.f);
    }
    glEnd();

    // draw the base of the cone
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0, 0, 0);
    for (int angle = 0; angle < 360; angle++) {
        // normal is just pointing down
        glNormal3f(0, -1, 0);
        glVertex3f(sin((double)angle) * base, cos((double)angle) * base, 0.f);
    }
    glEnd();
}


void draw_snowman(){
    // Draw 3 white spheres
    glColor4f(1.0, 1.0, 1.0, 1.0);
    drawSphere(0.8, 10, 10);
    glTranslatef(0.0, 0.8, 0.0);
    drawSphere(0.6, 10, 10);
    glTranslatef(0.0, 0.6, 0.0);
    drawSphere(0.4, 10, 10);

    // Draw the eyes
    glPushMatrix();
    glColor4f(0.0, 0.0, 0.0, 1.0);
    glTranslatef(0.2, 0.2, 0.2);
    drawSphere(0.066, 10, 10);
    glTranslatef(0, 0, -0.4);
    drawSphere(0.066, 10, 10);
    glPopMatrix();

    // Draw a nose
    glColor4f(1.0, 0.5, 0.0, 1.0);
    glTranslatef(0.3, 0.0, 0.0);
    glRotatef(90, 0, 1, 0);
    drawCone(0.1, 0.3, 10, 10);
}

void draw_triangle(GLfloat red, GLfloat green, GLfloat blue){
	glBegin(GL_TRIANGLES);           
	// Front
	glColor3f(red, green, blue);
	glVertex3f( 0.0f, 1.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);

	// Right
	glColor3f(red, green, blue);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);

	// Back
	glColor3f(red, green, blue);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);

	// Left
	glColor3f(red, green, blue);
	glVertex3f( 0.0f, 1.0f, 0.0f);
	glVertex3f(-1.0f,-1.0f,-1.0f);
	glVertex3f(-1.0f,-1.0f, 1.0f);
	glEnd();   
}

void draw_rectangle(GLfloat red, GLfloat green, GLfloat blue){
	glBegin(GL_QUADS);
	
	glColor3f(red, green, blue);     
	glVertex3f( 1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f,  1.0f);
	glVertex3f( 1.0f, 1.0f,  1.0f);

	glColor3f(red, green, blue);
	glVertex3f( 1.0f, -1.0f,  1.0f); 
	glVertex3f(-1.0f, -1.0f,  1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f( 1.0f, -1.0f, -1.0f);

	glColor3f(red, green, blue);
	glVertex3f( 1.0f,  1.0f, 1.0f);
	glVertex3f(-1.0f,  1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f( 1.0f, -1.0f, 1.0f);

	glColor3f(red, green, blue);
	glVertex3f( 1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f,  1.0f, -1.0f);
	glVertex3f( 1.0f,  1.0f, -1.0f);

	glColor3f(red, green, blue);
	glVertex3f(-1.0f,  1.0f,  1.0f);
	glVertex3f(-1.0f,  1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f,  1.0f);
	
	glColor3f(red, green, blue);
	glVertex3f(1.0f,  1.0f, -1.0f);
	glVertex3f(1.0f,  1.0f,  1.0f);
	glVertex3f(1.0f, -1.0f,  1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glEnd();  
}

void draw_first_figure(){
	// green
	GLfloat red1 = 0 / 255.0;
	GLfloat green1  = 204 / 255.0;
	GLfloat blue1  = 153 / 255.0;
	
	GLfloat red2 = 0 / 255.0;
	GLfloat green2  = 153 / 255.0;
	GLfloat blue2  = 153 / 255.0;
	
	GLfloat red3 = 51 / 255.0;
	GLfloat green3  = 204 / 255.0;
	GLfloat blue3  = 102 / 255.0;
	

	draw_triangle(red1, green1, blue1);
	glTranslatef(2.0, 0.0, 0.0);
	draw_triangle(red2, green2, blue2);
	glTranslatef(-1.0, 1.6, 0.0);
	glScalef(0.7, 0.7, 0.7);
	draw_triangle(red3, green3, blue3);

}

void draw_second_figure(){
	// red
	GLfloat red1 = 153 / 255.0;
	GLfloat green1  = 0 / 255.0;
	GLfloat blue1  = 0 / 255.0;
	
	GLfloat red2 = 204 / 255.0;
	GLfloat green2  = 0 / 255.0;
	GLfloat blue2  = 0 / 255.0;
	
	GLfloat red3 = 255 / 255.0;
	GLfloat green3  = 80 / 255.0;
	GLfloat blue3  = 89 / 255.0;

	glColor3f(red1, green1, blue1);
	drawSphere(0.8, 10, 10);
	glTranslatef(0.0, 0.75, 0.0);
	glColor3f(red2, green2, blue2);
	glRotated(180.0, 0.0, 1.0, 1.0);
	drawCone(0.7, 1.3, 10, 10);
	glRotated(-180.0, 0.0, 1.0, 1.0);

	glTranslatef(0.0, 1.9, 0.0);
	glColor3f(red3, green3, blue3);
	drawSphere(0.7, 10, 10);
}
 
void draw_third_figure(){
	// blue
	GLfloat red1 = 102 / 255.0;
	GLfloat green1  = 153 / 255.0;
	GLfloat blue1  = 255 / 255.0;
	
	GLfloat red2 = 0 / 255.0;
	GLfloat green2  = 102 / 255.0;
	GLfloat blue2  = 204 / 255.0;
	
	GLfloat red3 = 0 / 255.0;
	GLfloat green3  = 0 / 255.0;
	GLfloat blue3  = 102 / 255.0;

	draw_triangle(red1, green1, blue1);
	glTranslatef(1.0, 1.3, 0.0);
	glColor3f(red2, green2, blue2);
	drawSphere(1.0, 10, 10);
	glTranslatef(-2.0, 0.0, 0.0);
	glScalef(0.7, 0.7, 0.7);
	draw_rectangle(red3, green3, blue3);
}

void draw_fourth_figure(){
	// purple
	GLfloat red1 = 204 / 255.0;
	GLfloat green1  = 153 / 255.0;
	GLfloat blue1  = 255 / 255.0;
	
	GLfloat red2 = 153 / 255.0;
	GLfloat green2  = 51 / 255.0;
	GLfloat blue2  = 255 / 255.0;
	
	GLfloat red3 = 102 / 255.0;
	GLfloat green3  = 0 / 255.0;
	GLfloat blue3  = 102 / 255.0;

	glScalef(0.7, 0.7, 0.7);
	draw_rectangle(red2, green2, blue2);
	glTranslatef(2.45, 0.25, 0.0);
	glScalef(1.2, 1.2, 1.2);
	draw_rectangle(red3, green3, blue3);
	glTranslatef(-3.5, -0.55, 0.0);
	glScalef(0.5, 0.5, 0.5);
	draw_rectangle(red1, green1, blue1);
} 


void draw_player(int player_id){
	if(player_id == 1){
		draw_first_figure();
	}
	else if(player_id == 2){
		draw_second_figure();
	}
	else if(player_id == 3){
		draw_third_figure();
	}
	else if(player_id == 4){
		draw_fourth_figure();
	}
	else{
		draw_snowman();
	}
}