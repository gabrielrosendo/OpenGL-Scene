#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <cmath> // For sin, cos
// Constants for window dimensions and layout
const int windowRows = 5; // 5 rows
const int windowCols = 3; // 3 columns
// Dimensions for each window (width, height, and depth)
const float windowWidth = 0.1f;
const float windowHeight = 0.125f;
const float windowDepth = 0.05f; // Added depth for 3D
// Spacing between windows
const float windowSpacingX = 0.20f;
const float windowSpacingY = 0.17f;
// Camera settings
float theta = 0.0f; // Horizontal angle
float phi = 0.0f; // Vertical angle
float radius = 5.0f; // Distance from the center point
// Function to convert spherical coordinates to Cartesian and set the camera view
void updateCameraPosition() {
float eyeX = radius * cos(phi) * sin(theta);
float eyeY = radius * sin(phi);
float eyeZ = radius * cos(phi) * cos(theta);
glLoadIdentity();
gluLookAt(
eyeX, eyeY, eyeZ, // Eye position (camera position)
0.0f, 0.0f, 0.0f, // Center of the scene (focus point)
0.0f, 1.0f, 0.0f // Up vector (y-axis as up)
);
}
// Function to draw a 3D cuboid given two opposite vertices
void drawCuboid(float x1, float y1, float z1, float x2, float y2, float z2, float color[3]) {
glColor3fv(color);
glBegin(GL_QUADS);
// Front face
glVertex3f(x1, y1, z1);
glVertex3f(x2, y1, z1);
glVertex3f(x2, y2, z1);
glVertex3f(x1, y2, z1);
// Back face
glVertex3f(x1, y1, z2);
glVertex3f(x2, y1, z2);
glVertex3f(x2, y2, z2);
glVertex3f(x1, y2, z2);
// Left face
glVertex3f(x1, y1, z2);
glVertex3f(x1, y1, z1);
glVertex3f(x1, y2, z1);
glVertex3f(x1, y2, z2);
// Right face
glVertex3f(x2, y1, z2);
glVertex3f(x2, y1, z1);
glVertex3f(x2, y2, z1);
glVertex3f(x2, y2, z2);
// Top face
glVertex3f(x1, y2, z2);
glVertex3f(x2, y2, z2);
glVertex3f(x2, y2, z1);
glVertex3f(x1, y2, z1);
// Bottom face
glVertex3f(x1, y1, z2);
glVertex3f(x2, y1, z2);
glVertex3f(x2, y1, z1);
glVertex3f(x1, y1, z1);
glEnd();
}
// Function to draw a window with 3D depth
void drawWindow(float x, float y, float width, float height) {
float depth = windowDepth; // Depth of the window
float zFront = 0.01f; // Slightly in front of the building's front face
float zBack = zFront - depth;
// Draw the window frame (as a cuboid)
float frameColor[] = {0.6f, 0.6f, 0.6f}; // Gray color
drawCuboid(x, y - height, zBack, x + width, y, zFront, frameColor);
// Draw the left pane (slightly recessed)
float leftPaneColor[] = {0.1f, 0.2f, 0.4f}; // Dark Blue
drawCuboid(x + 0.01f, y - height + 0.01f, zBack + 0.01f, x + width / 2 - 0.01f, y - 0.01f, zFront - 0.01f, leftPaneColor);
// Draw the right pane (slightly recessed)
float rightPaneColor[] = {0.2f, 0.4f, 0.8f}; // Light Blue
drawCuboid(x + width / 2 + 0.01f, y - height + 0.01f, zBack + 0.01f, x + width - 0.01f, y - 0.01f, zFront - 0.01f, rightPaneColor);
}
// Add rest of the drawing functions here (drawBuilding, drawSky, etc.)
// Display callback function
void display() {
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
updateCameraPosition(); // Update the camera position based on current angles
// Draw the scene
drawSky();
drawBrickWall();
drawPillars();
drawBuilding();
drawAllWindows();
glFlush();
}
// Function to handle special key input (arrow keys)
void specialKeys(int key, int x, int y) {
const float angleStep = 0.05f; // Step for angle change
const float radiusStep = 0.1f; // Step for radius change
switch (key) {
case GLUT_KEY_LEFT:
theta -= angleStep; // Rotate left
break;
case GLUT_KEY_RIGHT:
theta += angleStep; // Rotate right
break;
case GLUT_KEY_UP:
if (phi < M_PI / 2 - angleStep) // Limit vertical rotation
phi += angleStep; // Rotate up
break;
case GLUT_KEY_DOWN:
if (phi > -M_PI / 2 + angleStep) // Limit vertical rotation
phi -= angleStep; // Rotate down
break;
case GLUT_KEY_PAGE_UP:
radius -= radiusStep; // Move closer
if (radius < 1.0f) radius = 1.0f; // Prevent getting too close
break;
case GLUT_KEY_PAGE_DOWN:
radius += radiusStep; // Move farther
break;
}
glutPostRedisplay(); // Redraw the scene
}
// OpenGL initialization
void init() {
glClearColor(1.0, 1.0, 1.0, 1.0); // Background color (white)
glEnable(GL_DEPTH_TEST); // Enable depth testing
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
gluPerspective(45.0, 1.0, 1.0, 100.0); // Perspective projection
glMatrixMode(GL_MODELVIEW);
}
// Main function
int main(int argc, char** argv) {
glutInit(&argc, argv);
glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
glutInitWindowSize(750, 750);
glutCreateWindow("3D Building with Camera Control");
init(); // Initialize OpenGL settings
glutDisplayFunc(display); // Set the display callback function
glutSpecialFunc(specialKeys); // Set the special key callback function for arrow keys
glutMainLoop(); // Start the main loop
return 0;
}
