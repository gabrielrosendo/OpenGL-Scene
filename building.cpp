#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <cmath> // For trigonometric functions
#include <iostream> 
#include "getBMP.h"


// Adjusted window size and layout for the 5x3 matrix
const int windowRows = 5;  // 5 rows
const int windowCols = 3;  // 3 columns

// Halved dimensions for each window (width and height)
const float windowWidth = 0.1f;
const float windowHeight = 0.125f;

// Adjusted spacing between windows
const float windowSpacingX = 0.20f; // Adjusted for proper even spacing
const float windowSpacingY = 0.17f;  // Vertical spacing between rows

// Camera position and orientation
float cameraX = 0.0f;
float cameraY = 0.0f;
float cameraZ = 3.0f;
float cameraAngleY = 0.0f; // Yaw rotation angle in degrees


unsigned int texture[1]; // Array of texture indices.

void loadExternalTextures()
{
    std::cout << "Loading texture..." << std::endl;
    try {
        imageFile *image = getBMP("tree-cropped.bmp");
        if (!image) {
            throw std::runtime_error("Failed to load BMP file: trees.bmp");
        }


        // Generate texture ID
        glGenTextures(1, texture);
        glBindTexture(GL_TEXTURE_2D, texture[0]);

        // Load texture data
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width, image->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->data);

        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        std::cout << "Texture loaded." << std::endl;

        // Clean up
        delete[] image->data;
        delete image;
    } catch (const std::exception &e) {
        std::cerr << "Exception during getBMP: " << e.what() << std::endl;
        throw; // Re-throw the exception to be caught in setup
    }
}
// Function to draw a window with dividers and an outline for depth
void drawWindow(float x, float y, float width, float height) {
    float z = 0.01f; // Slightly in front of the building front face to avoid z-fighting

    // Draw the outline to indicate depth (slightly larger than the window)
    glLineWidth(1.0f);
    glColor3f(0.1f, 0.1f, 0.1f); // Dark gray outline for depth
    glBegin(GL_LINE_LOOP);
        glVertex3f(x - 0.005f, y + 0.005f, z);
        glVertex3f(x + width + 0.005f, y + 0.005f, z);
        glVertex3f(x + width + 0.005f, y - height - 0.005f, z);
        glVertex3f(x - 0.005f, y - height - 0.005f, z);
    glEnd();

    // Draw the left and right panels
    glBegin(GL_QUADS);

    // Left Panel (Darker Blue)
    glColor3f(0.1, 0.2, 0.4); // Dark Blue
    glVertex3f(x, y, z);
    glVertex3f(x + width / 2.0f, y, z);
    glVertex3f(x + width / 2.0f, y - height, z);
    glVertex3f(x, y - height, z);

    // Right Panel (Lighter Blue)
    glColor3f(0.2, 0.4, 0.8); // Lighter Blue
    glVertex3f(x + width / 2.0f, y, z);
    glVertex3f(x + width, y, z);
    glVertex3f(x + width, y - height, z);
    glVertex3f(x + width / 2.0f, y - height, z);

    glEnd();

    // Draw the vertical dividers for the left and right panels with darker gray
    glLineWidth(1.5f); // Thin dividers
    glColor3f(0.3f, 0.3f, 0.3f); // Darker gray for dividers

    glBegin(GL_LINES);

    // Left Panel Dividers (4 bars)
    for (float i = x + (width / 5.0f); i < x + width / 2.0f; i += width / 5.0f) {
        glVertex3f(i, y, z);
        glVertex3f(i, y - height, z);
    }

    // Right Panel Dividers (4 bars)
    for (float i = x + width / 2.0f + (width / 5.0f); i < x + width; i += width / 5.0f) {
        glVertex3f(i, y, z);
        glVertex3f(i, y - height, z);
    }

    glEnd();

    // Draw window dividers
    glLineWidth(1.5f);
    glColor3f(1.0f, 1.0f, 1.0f); // Pure white
    glBegin(GL_LINES);
        glVertex3f(x + (width / 2), y + 0.005f, z + 0.001f);
        glVertex3f(x + (width / 2), y - height - 0.005f, z + 0.001f);
    glEnd();

    // Draw the window pane (slightly larger than the window)
    glLineWidth(1.0f);
    glColor3f(1.0f, 1.0f, 1.0f); // Pure white
    glBegin(GL_LINE_LOOP);
        glVertex3f(x - 0.0025f, y + 0.0025f, z + 0.002f);
        glVertex3f(x + width + 0.0025f, y + 0.0025f, z + 0.002f);
        glVertex3f(x + width + 0.0025f, y - height - 0.0025f, z + 0.002f);
        glVertex3f(x - 0.0025f, y - height - 0.0025f, z + 0.002f);
    glEnd();
}

// Function to draw all windows on the building
void drawAllWindows() {
    // Iterate through the 5x3 matrix of windows
    for (int row = 0; row < windowRows; ++row) {
        float startY = 0.60f - row * (windowHeight + windowSpacingY);
        for (int col = 0; col < windowCols; ++col) {
            float startX = -0.35f + col * (windowWidth + windowSpacingX);
            drawWindow(startX, startY, windowWidth, windowHeight);
        }
    }
}

void drawSky() {
    // Set sky colors
    float skyTopColor[3] = {0.53f, 0.81f, 0.92f}; // Sky blue
    float skyBottomColor[3] = {1.0f, 1.0f, 1.0f}; // White near the horizon

    float size = 50.0f; // Size of the skybox (large enough to enclose the entire scene)

    // Draw the top face (sky blue)
    glBegin(GL_QUADS);
        glColor3fv(skyTopColor);
        glVertex3f(-size, size, -size);
        glVertex3f(size, size, -size);
        glVertex3f(size, size, size);
        glVertex3f(-size, size, size);
    glEnd();

    // Draw the bottom face (white)
    glBegin(GL_QUADS);
        glColor3fv(skyBottomColor);
        glVertex3f(-size, -size, -size);
        glVertex3f(size, -size, -size);
        glVertex3f(size, -size, size);
        glVertex3f(-size, -size, size);
    glEnd();

    // Draw the front face
    glBegin(GL_QUADS);
        glColor3fv(skyBottomColor);
        glVertex3f(-size, -size, -size);
        glVertex3f(size, -size, -size);
        glColor3fv(skyTopColor);
        glVertex3f(size, size, -size);
        glVertex3f(-size, size, -size);
    glEnd();

    // Draw the back face
    glBegin(GL_QUADS);
        glColor3fv(skyBottomColor);
        glVertex3f(-size, -size, size);
        glVertex3f(size, -size, size);
        glColor3fv(skyTopColor);
        glVertex3f(size, size, size);
        glVertex3f(-size, size, size);
    glEnd();

    // Draw the left face
    glBegin(GL_QUADS);
        glColor3fv(skyBottomColor);
        glVertex3f(-size, -size, -size);
        glVertex3f(-size, -size, size);
        glColor3fv(skyTopColor);
        glVertex3f(-size, size, size);
        glVertex3f(-size, size, -size);
    glEnd();

    // Draw the right face
    glBegin(GL_QUADS);
        glColor3fv(skyBottomColor);
        glVertex3f(size, -size, -size);
        glVertex3f(size, -size, size);
        glColor3fv(skyTopColor);
        glVertex3f(size, size, size);
        glVertex3f(size, size, -size);
    glEnd();
}


void drawBuilding() {
    // Dimensions
    float left = -0.6f;
    float right = 0.65f;
    float bottom = -0.75f;
    float top = 0.8f;
    float front = 0.0f;
    float back = -0.2f;

    // Draw the main building (as a 3D box)
    glColor3f(0.9f, 0.9f, 0.9f); // Light gray for the building

    // Front face
    glBegin(GL_QUADS);
        glVertex3f(left, bottom, front);
        glVertex3f(right, bottom, front);
        glVertex3f(right, top, front);
        glVertex3f(left, top, front);
    glEnd();

    // Back face
    glBegin(GL_QUADS);
        glVertex3f(left, bottom, back);
        glVertex3f(right, bottom, back);
        glVertex3f(right, top, back);
        glVertex3f(left, top, back);
    glEnd();

    // Left face
    glBegin(GL_QUADS);
        glVertex3f(left, bottom, back);
        glVertex3f(left, bottom, front);
        glVertex3f(left, top, front);
        glVertex3f(left, top, back);
    glEnd();

    // Right face
    glBegin(GL_QUADS);
        glVertex3f(right, bottom, front);
        glVertex3f(right, bottom, back);
        glVertex3f(right, top, back);
        glVertex3f(right, top, front);
    glEnd();

    // Top face
    glBegin(GL_QUADS);
        glVertex3f(left, top, front);
        glVertex3f(right, top, front);
        glVertex3f(right, top, back);
        glVertex3f(left, top, back);
    glEnd();

    // Bottom face
    glBegin(GL_QUADS);
        glVertex3f(left, bottom, back);
        glVertex3f(right, bottom, back);
        glVertex3f(right, bottom, front);
        glVertex3f(left, bottom, front);
    glEnd();

    // Draw the dark blue square on the front face
    glColor3f(0.1f, 0.2f, 0.4f); // Dark blue for square
    glBegin(GL_QUADS);
        glVertex3f(-0.4f, 0.035f, front + 0.001f);
        glVertex3f(0.1f, 0.035f, front + 0.001f);
        glVertex3f(0.1f, -0.415f, front + 0.001f);
        glVertex3f(-0.4f, -0.415f, front + 0.001f);
    glEnd();

    // Draw the lines on the front face
    glColor3f(0.8f, 0.8f, 0.8f); // Slightly darker gray

    // Vertical lines
    glBegin(GL_LINES);
        glVertex3f(-0.35f, top, front + 0.002f);
        glVertex3f(-0.35f, bottom, front + 0.002f);

        glVertex3f(-0.25f, top, front + 0.002f);
        glVertex3f(-0.25f, bottom, front + 0.002f);

        glVertex3f(-0.05f, top, front + 0.002f);
        glVertex3f(-0.05f, bottom, front + 0.002f);

        glVertex3f(0.05f, top, front + 0.002f);
        glVertex3f(0.05f, bottom, front + 0.002f);

        glVertex3f(0.25f, top, front + 0.002f);
        glVertex3f(0.25f, bottom, front + 0.002f);

        glVertex3f(0.35f, top, front + 0.002f);
        glVertex3f(0.35f, bottom, front + 0.002f);
    glEnd();

    // Horizontal lines
    glBegin(GL_LINES);
        glVertex3f(left, 0.7f, front + 0.002f);
        glVertex3f(right, 0.7f, front + 0.002f);

        glVertex3f(left, 0.4f, front + 0.002f);
        glVertex3f(right, 0.4f, front + 0.002f);

        glVertex3f(left, 0.1f, front + 0.002f);
        glVertex3f(right, 0.1f, front + 0.002f);

        glVertex3f(left, -0.2f, front + 0.002f);
        glVertex3f(right, -0.2f, front + 0.002f);

        glVertex3f(left, -0.50f, front + 0.002f);
        glVertex3f(right, -0.50f, front + 0.002f);
    glEnd();
}

void drawPillars() {
    float front = 0.0f;
    float back = -0.1f;

    // Dark brown color
    glColor3f(0.20f, 0.10f, 0.05f);

    // Left pillar (half the height of the right pillar)
    glBegin(GL_QUADS);
        // Front face
        glVertex3f(-0.80f, -1.0f, front);
        glVertex3f(-0.6f, -1.0f, front);
        glVertex3f(-0.6f, 0.7f, front);
        glVertex3f(-0.80f, 0.7f, front);

        // Back face
        glVertex3f(-0.80f, -1.0f, back);
        glVertex3f(-0.6f, -1.0f, back);
        glVertex3f(-0.6f, 0.7f, back);
        glVertex3f(-0.80f, 0.7f, back);

        // Left face
        glVertex3f(-0.80f, -1.0f, back);
        glVertex3f(-0.80f, -1.0f, front);
        glVertex3f(-0.80f, 0.7f, front);
        glVertex3f(-0.80f, 0.7f, back);

        // Right face
        glVertex3f(-0.6f, -1.0f, front);
        glVertex3f(-0.6f, -1.0f, back);
        glVertex3f(-0.6f, 0.7f, back);
        glVertex3f(-0.6f, 0.7f, front);

        // Top face
        glVertex3f(-0.80f, 0.7f, front);
        glVertex3f(-0.6f, 0.7f, front);
        glVertex3f(-0.6f, 0.7f, back);
        glVertex3f(-0.80f, 0.7f, back);

        // Bottom face
        glVertex3f(-0.80f, -1.0f, back);
        glVertex3f(-0.6f, -1.0f, back);
        glVertex3f(-0.6f, -1.0f, front);
        glVertex3f(-0.80f, -1.0f, front);
    glEnd();

    // Right pillar (full height)
    glBegin(GL_QUADS);
        // Front face
        glVertex3f(0.65f, -1.0f, front);
        glVertex3f(0.95f, -1.0f, front);
        glVertex3f(0.95f, 0.7f, front);
        glVertex3f(0.65f, 0.7f, front);

        // Back face
        glVertex3f(0.65f, -1.0f, back);
        glVertex3f(0.95f, -1.0f, back);
        glVertex3f(0.95f, 0.7f, back);
        glVertex3f(0.65f, 0.7f, back);

        // Left face
        glVertex3f(0.65f, -1.0f, back);
        glVertex3f(0.65f, -1.0f, front);
        glVertex3f(0.65f, 0.7f, front);
        glVertex3f(0.65f, 0.7f, back);

        // Right face
        glVertex3f(0.95f, -1.0f, front);
        glVertex3f(0.95f, -1.0f, back);
        glVertex3f(0.95f, 0.7f, back);
        glVertex3f(0.95f, 0.7f, front);

        // Top face
        glVertex3f(0.65f, 0.7f, front);
        glVertex3f(0.95f, 0.7f, front);
        glVertex3f(0.95f, 0.7f, back);
        glVertex3f(0.65f, 0.7f, back);

        // Bottom face
        glVertex3f(0.65f, -1.0f, back);
        glVertex3f(0.95f, -1.0f, back);
        glVertex3f(0.95f, -1.0f, front);
        glVertex3f(0.65f, -1.0f, front);
    glEnd();

    // Right side extension (light gray)
    glColor3f(0.9f, 0.9f, 0.9f);
    glBegin(GL_QUADS);
        // Front face
        glVertex3f(0.95f, -1.0f, front);
        glVertex3f(1.0f, -1.0f, front);
        glVertex3f(1.0f, 0.7f, front);
        glVertex3f(0.95f, 0.7f, front);

        // Back face
        glVertex3f(0.95f, -1.0f, back);
        glVertex3f(1.0f, -1.0f, back);
        glVertex3f(1.0f, 0.7f, back);
        glVertex3f(0.95f, 0.7f, back);

        // Left face
        glVertex3f(0.95f, -1.0f, back);
        glVertex3f(0.95f, -1.0f, front);
        glVertex3f(0.95f, 0.7f, front);
        glVertex3f(0.95f, 0.7f, back);

        // Right face
        glVertex3f(1.0f, -1.0f, front);
        glVertex3f(1.0f, -1.0f, back);
        glVertex3f(1.0f, 0.7f, back);
        glVertex3f(1.0f, 0.7f, front);

        // Top face
        glVertex3f(0.95f, 0.7f, front);
        glVertex3f(1.0f, 0.7f, front);
        glVertex3f(1.0f, 0.7f, back);
        glVertex3f(0.95f, 0.7f, back);

        // Bottom face
        glVertex3f(0.95f, -1.0f, back);
        glVertex3f(1.0f, -1.0f, back);
        glVertex3f(1.0f, -1.0f, front);
        glVertex3f(0.95f, -1.0f, front);
    glEnd();

    // Draw the cover at the bottom of the building
    glColor3f(0.4f, 0.4f, 0.4f); // Dark gray for the cover
    glBegin(GL_QUADS);
        glVertex3f(-0.85f, -0.8f, front + 0.001f);
        glVertex3f(1.0f, -0.8f, front + 0.001f);
        glVertex3f(1.0f, -0.75f, front + 0.001f);
        glVertex3f(-0.85f, -0.75f, front + 0.001f);
    glEnd();

    // Draw windows on the right pillar
    for (int row = 0; row < windowRows; ++row) {
        float startY = 0.60f - row * (windowHeight + windowSpacingY);
        for (int col = 0; col < 2; ++col) {
            float startX = 0.6f + col * (windowWidth + 0.1f);
            drawWindow(startX, startY, windowWidth, windowHeight);
        }
    }

    // Draw windows on the left pillar
    for (int row = 0; row < windowRows; ++row) {
        float startY = 0.60f - row * (windowHeight + windowSpacingY);
        float startX = -0.7f;
        drawWindow(startX, startY, windowWidth, windowHeight);
    }
}
/*
void drawCylinder(float radius, float height, int slices) {
    GLUquadricObj *quadObj = gluNewQuadric();
    gluQuadricDrawStyle(quadObj, GLU_FILL);
    gluCylinder(quadObj, radius, radius, height, slices, 1);
    gluDeleteQuadric(quadObj);
}


void drawCone(float radius, float height, int slices) {
    GLUquadricObj *quadObj = gluNewQuadric();
    gluQuadricDrawStyle(quadObj, GLU_FILL);
    gluCylinder(quadObj, radius, 0, height, slices, 1);
    gluDeleteQuadric(quadObj);
}

void drawTree() {
    // Position the tree to the left of the building
    glPushMatrix();
    glTranslatef(-1.2f, -1.0f, 0.0f);  

    // Draw the trunk
    glColor3f(0.55f, 0.27f, 0.07f);  // Brown color
    glPushMatrix();
    glRotatef(-90, 1.0f, 0.0f, 0.0f);  // Rotate to make the cylinder vertical
    drawCylinder(0.05f, 0.6f, 10);  // Doubled size
    glPopMatrix();

    // Draw the foliage (three cones of different sizes)
    glColor3f(0.0f, 0.5f, 0.0f);  // Green color
    glPushMatrix();
    glTranslatef(0.0f, 0.3f, 0.0f);  // Adjusted to match the height of the trunk
    glRotatef(-90, 1.0f, 0.0f, 0.0f);  // Rotate to make the cones vertical
    drawCone(0.15f, 0.3f, 10);  // Doubled size
    glTranslatef(0.0f, 0.0f, 0.25f);  // Adjusted to match the new size
    drawCone(0.2f, 0.4f, 10);  // Doubled size
    glTranslatef(0.0f, 0.0f, 0.2f);  // Adjusted to match the new size
    glPopMatrix();

    glPopMatrix();
}
*/
void drawBrickWall() {
    // Set the color to brown for the bricks
    glColor3f(0.4f, 0.2f, 0.0f);

    // Dimensions of the wall
    float wall_left = -0.75f;
    float wall_right = -0.1f;
    float wall_bottom = -1.0f;
    float wall_top = -0.8f;
    float z = -0.05f; // Slightly behind the building front face

    // Brick dimensions
    float brick_width = 0.05f;
    float brick_height = 0.025f;
    float mortar_thickness = 0.005f;

    int count = 0;

    // Draw the bricks
    for (float y = wall_bottom; y < wall_top; y += brick_height + mortar_thickness) {
        count++;
        bool is_offset_row = (count % 2) == 1;

        for (float x = wall_left; x < wall_right + 1; x += brick_width + mortar_thickness) {
            float x_offset = is_offset_row ? -(brick_width / 2) : 0.0f;

            // Draw a single brick
            glBegin(GL_QUADS);
                glVertex3f(x + x_offset, y, z);
                glVertex3f(x + x_offset + brick_width, y, z);
                glVertex3f(x + x_offset + brick_width, y + brick_height, z);
                glVertex3f(x + x_offset, y + brick_height, z);
            glEnd();
        }
    }
}
void drawFloor() {
    float floorSize = 10.0f; // Size of the floor (adjust as needed)
    float tileSize = 0.5f;   // Size of each tile (adjust as needed)

    glEnable(GL_DEPTH_TEST);
    glBegin(GL_QUADS);
    for (float x = -floorSize; x < floorSize; x += tileSize) {
        for (float z = -floorSize; z < floorSize; z += tileSize) {
            // Set the color to dark gray for pavement
            glColor3f(0.3f, 0.3f, 0.3f); // Dark gray

            glVertex3f(x, -1.0f, z);
            glVertex3f(x + tileSize, -1.0f, z);
            glVertex3f(x + tileSize, -1.0f, z + tileSize);
            glVertex3f(x, -1.0f, z + tileSize);
        }
    }
    glEnd();
}
void drawTexturedTree()
{
    // Draw the textured tree to the left of the building
    glPushMatrix();
    glTranslatef(-1.2f, -1.0f, 0.0f);  // Adjust the position as needed
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f(-0.25f, 0.0f, 0.0f);  // Scale down by half
    glTexCoord2f(1.0, 0.0); glVertex3f(0.25f, 0.0f, 0.0f);   // Scale down by half
    glTexCoord2f(1.0, 1.0); glVertex3f(0.25f, 0.5f, 0.0f);   // Scale down by half
    glTexCoord2f(0.0, 1.0); glVertex3f(-0.25f, 0.5f, 0.0f);  // Scale down by half
    glEnd();
    glPopMatrix();
}

// Display callback function
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Reset transformations
    glLoadIdentity();

    // Convert camera angle from degrees to radians for trigonometric functions
    float rad = cameraAngleY * M_PI / 180.0f;

    // Calculate the camera's direction vector
    float dirX = sinf(rad);
    float dirZ = -cosf(rad); // Negative because OpenGL's Z axis goes into the screen

    // Set the camera
    gluLookAt(cameraX, cameraY, cameraZ,   // Eye position
              cameraX + dirX, cameraY, cameraZ + dirZ,   // Center position
              0.0f, 1.0f, 0.0f);  // Up vector

    // Draw the Sky
    drawSky();

    // Draw Brick Wall
    drawBrickWall();

    // Draw Pillars
    drawPillars();

    // Draw Floor
    drawFloor();

    // Draw Trees
    //drawTree();

    // Draw the building (light gray background)
    drawBuilding();

    // Draw all the windows in a 5x3 matrix
    drawAllWindows();
    // Draw the textured tree to the left of the building
    drawTexturedTree();


    glutSwapBuffers();
}



// Keyboard callback function for special keys (arrow keys)
void specialKeys(int key, int x, int y) {
    const float moveSpeed = 0.1f;    // Camera movement speed
    const float angleSpeed = 5.0f;   // Camera rotation speed (degrees)

    switch (key) {
        case GLUT_KEY_UP:
            // Move forward
            cameraX += moveSpeed * sinf(cameraAngleY * M_PI / 180.0f);
            cameraZ += moveSpeed * -cosf(cameraAngleY * M_PI / 180.0f);
            break;
        case GLUT_KEY_DOWN:
            // Move backward
            cameraX -= moveSpeed * sinf(cameraAngleY * M_PI / 180.0f);
            cameraZ -= moveSpeed * -cosf(cameraAngleY * M_PI / 180.0f);
            break;
        case GLUT_KEY_LEFT:
            // Rotate left
            cameraAngleY -= angleSpeed;
            break;
        case GLUT_KEY_RIGHT:
            // Rotate right
            cameraAngleY += angleSpeed;
            break;
    }

    glutPostRedisplay(); // Request display update
}

// OpenGL initialization
void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0); // Background color (white)
    glEnable(GL_DEPTH_TEST);          // Enable depth testing

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 1.0, 1.0, 100.0); // Set up a perspective projection

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Load textures
    loadExternalTextures();

    // Enable texturing
    glEnable(GL_TEXTURE_2D);
}
// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(750, 750);
    glutCreateWindow("3D Building with Camera Movement");

    // Initialize OpenGL settings
    init();

    // Set the display callback function
    glutDisplayFunc(display);

    // Register the keyboard callback function for special keys
    glutSpecialFunc(specialKeys);

    // Start the main loop
    glutMainLoop();
    return 0;
}