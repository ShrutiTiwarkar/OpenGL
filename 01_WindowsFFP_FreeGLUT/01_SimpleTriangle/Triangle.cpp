
#include <GL/freeglut.h>
#include <GL/glut.h>

bool bFullscreen_SAT = false;

int main( int argc, char **argv) {

    //declaration

    void initialize(void);
    void resize(int, int);
    void display(void);
    void keyboard(unsigned char, int, int);
    void mouse(int, int, int, int);
    void uninitialize(void);


    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE| GLUT_RGBA);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100,100);
    glutCreateWindow("GLUT : Shruti Tiwarkar");

    initialize();

    glutDisplayFunc(display);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutCloseFunc(uninitialize);

    glutMainLoop();

    return 0;
}

void initialize(void) {

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

}

void resize(int width_SAT, int heigth_SAT) {

    if(heigth_SAT <= 0){
        heigth_SAT = 1;
    }

    glViewport(0,0,(GLsizei)width_SAT, (GLsizei)heigth_SAT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
}

void display(void) {

    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 1.0f, 0.0f);

        glColor3f(1.0f, 0.5f, 0.0f);
        glVertex3f(-1.0f, -1.0f, 0.0f);

        glColor3f(1.0f, 0.8f, 1.0f);
        glVertex3f(1.0f, -1.0f, 0.0f);

    glEnd();

    glFlush();
}

void keyboard(unsigned char key_SAT, int x_SAT, int y_SAT) {

    switch (key_SAT)
    {
    case 27:
        glutLeaveMainLoop();
        break;

    case 'F':
    case 'f':
        if(bFullscreen_SAT == false){
            glutFullScreen();
            bFullscreen_SAT = true;
        }
        else{
            glutLeaveFullScreen();
            bFullscreen_SAT = false;
        }
        break;
    
    default:
        break;
    }
}


void mouse(int button_SAT, int state_SAT, int x_SAT, int y_SAT) {

    switch (button_SAT)
    {
    case GLUT_LEFT_BUTTON:
        break;
    
    case GLUT_RIGHT_BUTTON:
        glutLeaveMainLoop();
        break;
    
    default:
        break;
    }
}

void uninitialize(void) {

}


