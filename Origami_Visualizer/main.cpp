#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
int polygon_offset = 0;

enum {
  FLAT,
  FLAP1,
  FLAP2,
  CENTER2,
  WING2,
  CENTER1,
  WING1,
  FOLDED
} States;

int motion = 1;
int spinning = 0;
int state = FLAT;
int click = 0;
int delay = 0;
int direction;
float flap1_angle = 0;
float flap2_angle = 0;
float center1_angle = 0;
float center2_angle = 0;
float wing1_angle = 0;
float wing2_angle = 0;
GLfloat angle = -150;
GLfloat angle2 = 30;


typedef GLfloat Point[2];

Point poly1[] =
{
  {-1, 0},
  {-1 / 3.0, 2 / 3.0},
  {-1, 2 / 3.0}
};

Point poly2[] =
{
  {-1, 1},
  {-1, 2 / 3.0},
  {-1 / 3.0, 2 / 3.0},
  {0, 1}
};

Point poly3[] =
{
  {0, 1},
  {1, 1},
  {1, 2 / 3.0},
  {1 / 3.0, 2 / 3.0}
};

Point poly4[] =
{
  {1 / 3.0, 2 / 3.0},
  {1, 2 / 3.0},
  {1, 0}
};

Point poly5[] =
{
  {-1 / 3.0, 2 / 3.0},
  {0, 1},
  {0, -1.5},
  {-1 / 3.0, -1.5}
};

Point poly6[] =
{
  {0, 1},
  {1 / 3.0, 2 / 3.0},
  {1 / 3.0, -1.5},
  {0, -1.5}
};

Point poly7[] =
{
  {-1, 0},
  {-1 / 3.0, 2 / 3.0},
  {-1 / 3.0, -1.5},
  {-1, -1.5}
};

Point poly8[] =
{
  {1, 0},
  {1 / 3.0, 2 / 3.0},
  {1 / 3.0, -1.5},
  {1, -1.5}
};

void
polydlist(int dlist, int num, Point points[])
{
  int i;

  glNewList(dlist, GL_COMPILE);
  glBegin(GL_POLYGON);
  for (i = 0; i < num; i++) {
    glVertex2fv(&points[i][0]);
  }
  glEnd();
  glEndList();
}

void idle(void)
{
  if (spinning)
    click=click+1;
  switch (state) {
  case FLAT:
    delay++;
    if (delay >= 80) {
      delay = 0;
      state = FLAP1;
      glutSetWindowTitle("origami (folding)");
      direction = 1;
    }
    break;
  case FLAP1:
    flap1_angle += 0.02 * direction;
    if (flap1_angle >= 180) {
      state = FLAP2;
    } else if (flap1_angle <= 0) {
      state = FLAT;
    }
    break;
  case FLAP2:
    flap2_angle += 0.02 * direction;
    if (flap2_angle >= 180) {
      state = CENTER2;
    } else if (flap2_angle <= 0) {
      state = FLAP1;
    }
    break;
  case CENTER2:
    center2_angle += 0.02 * direction;
    if (center2_angle >= 84) {
      state = WING2;
    } else if (center2_angle <= 0) {
      state = FLAP2;
    }
    break;
  case WING2:
    wing2_angle += 0.02 * direction;
    if (wing2_angle >= 84) {
      state = CENTER1;
    } else if (wing2_angle <= 0) {
      state = CENTER2;
    }
    break;
  case CENTER1:
    center1_angle += 0.02 * direction;
    if (center1_angle >= 84) {
      state = WING1;
    } else if (center1_angle <= 0) {
      state = WING2;
    }
    break;
  case WING1:
    wing1_angle += 0.02 * direction;
    if (wing1_angle >= 84) {
      state = FOLDED;
    } else if (wing1_angle <= 0) {
      state = CENTER1;
    }
    break;
  case FOLDED:
    delay++;
    if (delay >= 80) {
      delay = 0;
      glutSetWindowTitle("origami (unfolding)");
      direction = -1;
      state = WING1;
    }
    break;
  }

  glutPostRedisplay();
}

void draw_folded_plane(void)
{
  /* INDENT-OFF */
  glPushMatrix();
    glRotatef(click*0.01, 0, 0, 1);
    glRotatef(click*0.01 / 5.0, 0, 1, 0);
    glTranslatef(0, .25, 0);
    glPushMatrix();
      glRotatef(center1_angle, 0, 1, 0);
      glPushMatrix();
        glTranslatef(-.5, .5, 0);
        glRotatef(flap1_angle, 1, 1, 0);
        glTranslatef(.5, -.5, 0);
        glCallList(2);
      glPopMatrix();
      glCallList(5);

      glPushMatrix();
        glTranslatef(-1 / 3.0, 0, 0);
        glRotatef(-wing1_angle, 0, 1, 0);
        glTranslatef(1 / 3.0, 0, 0);

        glCallList(7);
        glPushMatrix();
          glTranslatef(-.5, .5, 0);
          glRotatef(flap1_angle, 1, 1, 0);
          glTranslatef(.5, -.5, 0);
          glCallList(1);
        glPopMatrix();
      glPopMatrix();
    glPopMatrix();

    glPushMatrix();
      glRotatef(-center2_angle, 0, 1, 0);
      glPushMatrix();
        glTranslatef(.5, .5, 0);
        glRotatef(-flap2_angle, -1, 1, 0);
        glTranslatef(-.5, -.5, 0);
        glCallList(3);
      glPopMatrix();
      glCallList(6);

      glPushMatrix();
        glTranslatef(1 / 3.0, 0, 0);
        glRotatef(wing2_angle, 0, 1, 0);
        glTranslatef(-1 / 3.0, 0, 0);

        glCallList(8);
        glPushMatrix();
          glTranslatef(.5, .5, 0);
          glRotatef(-flap2_angle, -1, 1, 0);
          glTranslatef(-.5, -.5, 0);
          glCallList(4);
        glPopMatrix();
      glPopMatrix();
    glPopMatrix();
  glPopMatrix();

  /* INDENT-ON */

}

void
display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glColor3ub(255, 0, 0);
  if (polygon_offset) {
    glPolygonOffset(0.5, 0.0);
    glEnable(GL_POLYGON_OFFSET_FILL);

  }
  draw_folded_plane();
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glColor3ub(1,1,1);
  if (polygon_offset) {
    glPolygonOffset(0.5, 0);
    glDisable(GL_POLYGON_OFFSET_FILL);

  } else {
    glPushMatrix();
    glTranslatef(0, 0, .05);
  }
  draw_folded_plane();
  if (!polygon_offset) {
    glPopMatrix();
  }
  glutSwapBuffers();
}

void visible(int state)
{
  if (state == GLUT_VISIBLE) {
    if (motion)
      glutIdleFunc(idle);
  } else {
    glutIdleFunc(NULL);
  }
}
int moving, startx, starty;

static void motionchange(int x, int y)
{if (moving){
    angle = angle + (x - startx);
    angle2 = angle2 + (y - starty);
    startx = x;
    starty = y;
    glRotatef(angle2*0.01, 1.0, 0.0, 0.0);
    glRotatef(angle*0.01, 0.0, 1.0, 0.0);
    glutPostRedisplay();
  }
}
  static void
mouse(int button, int state, int x, int y)
{
  if (button == GLUT_LEFT_BUTTON) {
    if (state == GLUT_DOWN) {
      moving = 1;
      startx = x;
      starty = y;
    }
    if (state == GLUT_UP) {
      moving = 0;
    }
  }
}
void menu(int value){
  switch (value) {
  case 1:
    direction = -direction;
    if (direction > 0) {
      glutSetWindowTitle("origami (folding)");
    } else {
      glutSetWindowTitle("origami (unfolding)");
    }
    break;
  case 2:
    motion = 1 - motion;
    if (motion) {
      glutIdleFunc(idle);
    } else {
      glutIdleFunc(NULL);
    }
    break;
  case 3:
    spinning = 1 - spinning;
    break;
  case 666:
    exit(0);
  }
}


static int supportsOneDotOne(void)
{
  const char *version;
  int major, minor;

  version = (char *) glGetString(GL_VERSION);
  if (sscanf(version, "%d.%d", &major, &minor) == 2) {
    return major > 1 || minor >= 1;
  }
  return 0;
}

int
main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
  glutInitWindowSize(600,600);
  glutInitWindowPosition(0,0);
  glutCreateWindow("origami");
  glutDisplayFunc(display);
  glutMotionFunc(motionchange);
  glutMouseFunc(mouse);
  glutVisibilityFunc(visible);
  glClearColor(1, 1, 1, 0);
  glMatrixMode(GL_PROJECTION);
  gluPerspective(40.0, 1.0, 0.1, 10.0);
  glMatrixMode(GL_MODELVIEW);
  gluLookAt(0, 0, 5.5,
    0, 0, 0,
    0, 1, 0);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glLineWidth(1.5);
  polydlist(1, sizeof(poly1) / sizeof(Point), poly1);
  polydlist(2, sizeof(poly2) / sizeof(Point), poly2);
  polydlist(3, sizeof(poly3) / sizeof(Point), poly3);
  polydlist(4, sizeof(poly4) / sizeof(Point), poly4);
  polydlist(5, sizeof(poly5) / sizeof(Point), poly5);
  polydlist(6, sizeof(poly6) / sizeof(Point), poly6);
  polydlist(7, sizeof(poly7) / sizeof(Point), poly7);
  polydlist(8, sizeof(poly8) / sizeof(Point), poly8);
  glutCreateMenu(menu);
  glutAddMenuEntry("Play Reverse", 1);
  glutAddMenuEntry("Pause/Play", 2);
  glutAddMenuEntry("Spin the model", 3);
  glutAddMenuEntry("Quit", 666);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
  polygon_offset = supportsOneDotOne();

  glutMainLoop();
  return 0;
}
