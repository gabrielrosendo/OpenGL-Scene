#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#ifndef CUSTOM_OBJ_LOADER_H
#define CUSTOM_OBJ_LOADER_H


#include <vector>
#include <string>
#include <cmath> // For trigonometric functions
#include <fstream>
#include <sstream>
#include <iostream>
#include "custom_obj_loader.h"

// Global variables to store the OBJ data
std::vector<float> obj_vertices;
std::vector<float> obj_normals;
std::vector<unsigned int> obj_indices;



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


// Function to draw a window with dividers and an outline for depth
void drawWindow(float x, float y, float z, float width, float height) {
    // Remove the line: float z = 0.01f; // z is now a parameter

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

    // Draw window divider (center line)
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
            // Example in drawAllWindows()
            drawWindow(startX, startY, 0.01f, windowWidth, windowHeight);
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
    float front = -0.1f;
    float back = -0.2f;

    // Dark brown color for the pillars
    glColor3f(0.20f, 0.10f, 0.05f);

    // Left pillar (adjusted to match the building's back on the z-plane)
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

    // Right pillar (adjusted to match the building's back on the z-plane)
    glBegin(GL_QUADS);
	    // Front face (extended to the right)
	    glVertex3f(0.65f, -1.0f, front);
	    glVertex3f(1.0f, -1.0f, front);  // Extended this vertex to x = 1.0f
	    glVertex3f(1.0f, 0.7f, front);   // Extended this vertex to x = 1.0f
	    glVertex3f(0.65f, 0.7f, front);

	    // Back face (extended to the right)
	    glVertex3f(0.65f, -1.0f, back);
	    glVertex3f(1.0f, -1.0f, back);   // Extended this vertex to x = 1.0f
	    glVertex3f(1.0f, 0.7f, back);    // Extended this vertex to x = 1.0f
	    glVertex3f(0.65f, 0.7f, back);

	    // Left face (unchanged)
	    glVertex3f(0.65f, -1.0f, back);
	    glVertex3f(0.65f, -1.0f, front);
	    glVertex3f(0.65f, 0.7f, front);
	    glVertex3f(0.65f, 0.7f, back);

	    // Right face (extended to cover the second block's position)
	    glVertex3f(1.0f, -1.0f, front);  // Now at x = 1.0f
	    glVertex3f(1.0f, -1.0f, back);   // Now at x = 1.0f
	    glVertex3f(1.0f, 0.7f, back);    // Now at x = 1.0f
	    glVertex3f(1.0f, 0.7f, front);   // Now at x = 1.0f

	    // Top face (extended to the right)
	    glVertex3f(0.65f, 0.7f, front);
	    glVertex3f(1.0f, 0.7f, front);   // Extended this vertex to x = 1.0f
	    glVertex3f(1.0f, 0.7f, back);    // Extended this vertex to x = 1.0f
	    glVertex3f(0.65f, 0.7f, back);

	    // Bottom face (extended to the right)
	    glVertex3f(0.65f, -1.0f, back);
	    glVertex3f(1.0f, -1.0f, back);   // Extended this vertex to x = 1.0f
	    glVertex3f(1.0f, -1.0f, front);  // Extended this vertex to x = 1.0f
	    glVertex3f(0.65f, -1.0f, front);
    glEnd();


    // Draw the cover (dark gray divider) at the bottom of the building as a 3D object
    glColor3f(0.4f, 0.4f, 0.4f); // Dark gray for the divider
    float dividerFront = 0.03f;  // Sticks out a bit in the front
    float dividerBack = -0.2f;   // Matches the back of the pillars
    float dividerTop = -0.75f;
    float dividerBottom = -0.8f;
    
    //Draw the cover for the building
    glBegin(GL_QUADS);
        // Front face
        glVertex3f(-0.85f, dividerBottom, dividerFront);
        glVertex3f(1.0f, dividerBottom, dividerFront);
        glVertex3f(1.0f, dividerTop, dividerFront);
        glVertex3f(-0.85f, dividerTop, dividerFront);

        // Back face
        glVertex3f(-0.85f, dividerBottom, dividerBack);
        glVertex3f(1.0f, dividerBottom, dividerBack);
        glVertex3f(1.0f, dividerTop, dividerBack);
        glVertex3f(-0.85f, dividerTop, dividerBack);

        // Left face
        glVertex3f(-0.85f, dividerBottom, dividerBack);
        glVertex3f(-0.85f, dividerBottom, dividerFront);
        glVertex3f(-0.85f, dividerTop, dividerFront);
        glVertex3f(-0.85f, dividerTop, dividerBack);

        // Right face
        glVertex3f(1.0f, dividerBottom, dividerFront);
        glVertex3f(1.0f, dividerBottom, dividerBack);
        glVertex3f(1.0f, dividerTop, dividerBack);
        glVertex3f(1.0f, dividerTop, dividerFront);

        // Top face
        glVertex3f(-0.85f, dividerTop, dividerFront);
        glVertex3f(1.0f, dividerTop, dividerFront);
        glVertex3f(1.0f, dividerTop, dividerBack);
        glVertex3f(-0.85f, dividerTop, dividerBack);

        // Bottom face
        glVertex3f(-0.85f, dividerBottom, dividerBack);
        glVertex3f(1.0f, dividerBottom, dividerBack);
        glVertex3f(1.0f, dividerBottom, dividerFront);
        glVertex3f(-0.85f, dividerBottom, dividerFront);
    glEnd();


     // Draw all windows on the right pillar
     for (int row = 0; row < windowRows; ++row) {
        float startY = 0.60f - row * (windowHeight + windowSpacingY);
        for (int col = 0; col < 2; ++col) {
            float startX = 0.68f + col * (windowWidth + 0.05f);
            float z = front + 0.001f; // Slightly in front of the front face to avoid z-fighting
            drawWindow(startX, startY, z, windowWidth, windowHeight);
        }
    }

    // Draw windows on the left pillar
    for (int row = 0; row < windowRows; ++row) {
        float startY = 0.60f - row * (windowHeight + windowSpacingY);
        float startX = -0.75f;
        float z = front + 0.001f; // Slightly in front of the front face to avoid z-fighting
        drawWindow(startX, startY, z, windowWidth, windowHeight);
    }
}

void drawBrickWall() {
    // Set the color to brown for the bricks
    glColor3f(0.4f, 0.2f, 0.0f);

    // Dimensions of the wall
    float wall_left = -0.75f;
    float wall_right = -0.1f;
    float wall_bottom = -1.0f;
    float wall_top = -0.8f;
    float front = -0.11f;
    float back = -0.13f;

    // Brick dimensions
    float brick_width = 0.05f;
    float brick_height = 0.025f;
    float mortar_thickness = 0.005f;

    int count = 0;

    // Draw the bricks as 3D objects extending from front to back
    for (float y = wall_bottom; y < wall_top; y += brick_height + mortar_thickness) {
        count++;
        bool is_offset_row = (count % 2) == 1;

        for (float x = wall_left; x < wall_right + 1; x += brick_width + mortar_thickness) {
            float x_offset = is_offset_row ? -(brick_width / 2) : 0.0f;

            // Draw a single brick as a 3D block
            glBegin(GL_QUADS);
                // Front face
                glVertex3f(x + x_offset, y, front);
                glVertex3f(x + x_offset + brick_width, y, front);
                glVertex3f(x + x_offset + brick_width, y + brick_height, front);
                glVertex3f(x + x_offset, y + brick_height, front);

                // Back face
                glVertex3f(x + x_offset, y, back);
                glVertex3f(x + x_offset + brick_width, y, back);
                glVertex3f(x + x_offset + brick_width, y + brick_height, back);
                glVertex3f(x + x_offset, y + brick_height, back);

                // Left face
                glVertex3f(x + x_offset, y, back);
                glVertex3f(x + x_offset, y, front);
                glVertex3f(x + x_offset, y + brick_height, front);
                glVertex3f(x + x_offset, y + brick_height, back);

                // Right face
                glVertex3f(x + x_offset + brick_width, y, front);
                glVertex3f(x + x_offset + brick_width, y, back);
                glVertex3f(x + x_offset + brick_width, y + brick_height, back);
                glVertex3f(x + x_offset + brick_width, y + brick_height, front);

                // Top face
                glVertex3f(x + x_offset, y + brick_height, front);
                glVertex3f(x + x_offset + brick_width, y + brick_height, front);
                glVertex3f(x + x_offset + brick_width, y + brick_height, back);
                glVertex3f(x + x_offset, y + brick_height, back);

                // Bottom face
                glVertex3f(x + x_offset, y, back);
                glVertex3f(x + x_offset + brick_width, y, back);
                glVertex3f(x + x_offset + brick_width, y, front);
                glVertex3f(x + x_offset, y, front);
            glEnd();
        }
    }

    // Draw a single 2D mortar plane at z = 0, covering the entire brick wall
    glColor3f(0.7f, 0.7f, 0.7f); // Light gray color for the mortar plane
    glBegin(GL_QUADS);
        // Mortar plane covering the brick wall
        glVertex3f(wall_left, wall_bottom, -0.12f); // bottom-left
        glVertex3f(wall_right + 1.0f , wall_bottom, -0.12f); // bottom-right
        glVertex3f(wall_right + 1.0f, wall_top, -0.12f); // top-right
        glVertex3f(wall_left, wall_top, -0.12f); // top-left
    glEnd();
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

void drawParkingSign() {
    glPushMatrix();

    // Adjust position: move to the left and slightly adjust other coordinates
    glTranslatef(-0.5f, -0.95f, 0.8f);  // Moved left, slightly down, and closer

    // Reduce the scale to make the sign smaller
    glScalef(0.15f, 0.15f, 0.15f);

    // Slightly adjust rotation for better visibility
    glRotatef(-15.0f, 0.0f, 1.0f, 0.0f);

    // Draw the pole
    glColor3f(0.5f, 0.5f, 0.5f);  // Gray color for the pole
    glBegin(GL_QUADS);
    glVertex3f(-0.05f, -1.0f, 0.0f);  // Bottom-left
    glVertex3f(0.05f, -1.0f, 0.0f);   // Bottom-right
    glVertex3f(0.05f, 1.0f, 0.0f);    // Top-right
    glVertex3f(-0.05f, 1.0f, 0.0f);   // Top-left
    glEnd();

    // Draw the sign
    glColor3f(1.0f, 1.0f, 1.0f);  // White color for the sign
    glBegin(GL_QUADS);
    glVertex3f(-0.5f, 1.0f, 0.0f);  // Bottom-left
    glVertex3f(0.5f, 1.0f, 0.0f);   // Bottom-right
    glVertex3f(0.5f, 1.5f, 0.0f);   // Top-right
    glVertex3f(-0.5f, 1.5f, 0.0f);  // Top-left
    glEnd();

    glPopMatrix();
}

void drawSecondSign() {
    glPushMatrix();

    // Adjust position: move to the left and slightly adjust other coordinates
    glTranslatef(0.5f, -0.95f, 0.8f);  // Moved left, slightly down, and closer

    // Reduce the scale to make the sign smaller
    glScalef(0.15f, 0.15f, 0.15f);

    // Slightly adjust rotation for better visibility
    glRotatef(-15.0f, 0.0f, 1.0f, 0.0f);

    // Draw the pole
    glColor3f(0.5f, 0.5f, 0.5f);  // Gray color for the pole
    glBegin(GL_QUADS);
    glVertex3f(-0.05f, -1.0f, 0.0f);  // Bottom-left
    glVertex3f(0.05f, -1.0f, 0.0f);   // Bottom-right
    glVertex3f(0.05f, 1.0f, 0.0f);    // Top-right
    glVertex3f(-0.05f, 1.0f, 0.0f);   // Top-left
    glEnd();

    // Draw the sign
    glColor3f(1.0f, 1.0f, 1.0f);  // White color for the sign
    glBegin(GL_QUADS);
    glVertex3f(-0.5f, 1.0f, 0.0f);  // Bottom-left
    glVertex3f(0.5f, 1.0f, 0.0f);   // Bottom-right
    glVertex3f(0.5f, 1.5f, 0.0f);   // Top-right
    glVertex3f(-0.5f, 1.5f, 0.0f);  // Top-left
    glEnd();

    glPopMatrix();
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


bool loadOBJ(const char* path, std::vector<float>& vertices, std::vector<float>& normals) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cout << "Failed to open OBJ file: " << path << std::endl;
        return false;
    }

    std::vector<float> temp_vertices;
    std::vector<float> temp_normals;
    std::vector<unsigned int> vertexIndices, normalIndices;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream s(line);
        std::string word;
        s >> word;

        if (word == "v") {
            float x, y, z;
            s >> x >> y >> z;
            temp_vertices.push_back(x);
            temp_vertices.push_back(y);
            temp_vertices.push_back(z);
        }
        else if (word == "vn") {
            float x, y, z;
            s >> x >> y >> z;
            temp_normals.push_back(x);
            temp_normals.push_back(y);
            temp_normals.push_back(z);
        }
        else if (word == "f") {
            std::string vertexStr;
            while (s >> vertexStr) {
                unsigned int vertexIndex = 0, normalIndex = 0;
                size_t firstSlash = vertexStr.find('/');
                size_t secondSlash = vertexStr.find('/', firstSlash + 1);

                if (firstSlash == std::string::npos) {
                    // Only vertex index
                    vertexIndex = std::stoi(vertexStr);
                } else if (secondSlash == std::string::npos || firstSlash == secondSlash) {
                    // Format: v//vn (no texture coordinate)
                    vertexIndex = std::stoi(vertexStr.substr(0, firstSlash));
                    normalIndex = std::stoi(vertexStr.substr(firstSlash + 2));
                } else {
                    // Format: v/vt/vn
                    vertexIndex = std::stoi(vertexStr.substr(0, firstSlash));
                    normalIndex = std::stoi(vertexStr.substr(secondSlash + 1));
                }

                if (vertexIndex > 0 && vertexIndex <= temp_vertices.size() / 3) {
                    vertexIndices.push_back(vertexIndex - 1);
                }

                if (normalIndex > 0 && normalIndex <= temp_normals.size() / 3) {
                    normalIndices.push_back(normalIndex - 1);
                }
            }
        }
    }

    // Assemble the vertex and normal arrays
    for (size_t i = 0; i < vertexIndices.size(); ++i) {
        unsigned int vertexIndex = vertexIndices[i];
        unsigned int normalIndex = normalIndices[i];

        vertices.push_back(temp_vertices[vertexIndex * 3]);
        vertices.push_back(temp_vertices[vertexIndex * 3 + 1]);
        vertices.push_back(temp_vertices[vertexIndex * 3 + 2]);

        normals.push_back(temp_normals[normalIndex * 3]);
        normals.push_back(temp_normals[normalIndex * 3 + 1]);
        normals.push_back(temp_normals[normalIndex * 3 + 2]);
    }

    return true;
}


void drawOBJ(const std::vector<float>& vertices, const std::vector<float>& normals) {
    glEnable(GL_DEPTH_TEST);

    glBegin(GL_TRIANGLES);
    for (size_t i = 0; i < vertices.size(); i += 9) {
        // Assuming the first half of the vertices are for the trunk and the second half are for the leaves
        if (i < vertices.size() / 2) {
            // Set color for the trunk (brown)
            glColor3f(0.55f, 0.27f, 0.07f);
        } else {
            // Set color for the leaves (green)
            glColor3f(0.0f, 0.5f, 0.0f);
        }

        // Draw the triangle
        glNormal3f(normals[i], normals[i + 1], normals[i + 2]);
        glVertex3f(vertices[i], vertices[i + 1], vertices[i + 2]);

        glNormal3f(normals[i + 3], normals[i + 4], normals[i + 5]);
        glVertex3f(vertices[i + 3], vertices[i + 4], vertices[i + 5]);

        glNormal3f(normals[i + 6], normals[i + 7], normals[i + 8]);
        glVertex3f(vertices[i + 6], vertices[i + 7], vertices[i + 8]);
    }
    glEnd();
}

void drawHazelnut() {
    glPushMatrix();

    // Set the color to green
    glColor3f(0.0f, 0.5f, 0.0f);  

    // Move the object to the left
    glTranslatef(-1.2f, -1.0f, 0.0f); // Adjust position as needed

    // Scale the object to half its size
    glScalef(0.05f, 0.05f, 0.05f);     // Scale down the object as needed

    // Draw the OBJ model
    drawOBJ(obj_vertices, obj_normals);

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


    // Draw the building (light gray background)
    drawBuilding();

    // Draw all the windows in a 5x3 matrix
    drawAllWindows();

    // Draw the parking sign
    drawParkingSign();

    // Draw the second sign
    drawSecondSign();

    // Create hazelnut
    drawHazelnut();
    
    glutSwapBuffers();
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
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(750, 750);
    glutCreateWindow("3D Building with Camera Movement");

    // Initialize OpenGL settings
    init();

    // Load the OBJ file
    if (!loadOBJ("Hazelnut.obj", obj_vertices, obj_normals)) {
        std::cout << "Failed to load OBJ file" << std::endl;
        return -1;
    }

    // Debug: Print the number of loaded vertices and normals
    std::cout << "Loaded vertices: " << obj_vertices.size() / 3 << std::endl;
    std::cout << "Loaded normals: " << obj_normals.size() / 3 << std::endl;

    // Set the display callback function
    glutDisplayFunc(display);

    // Register the keyboard callback function for special keys
    glutSpecialFunc(specialKeys);

    // Start the main loop
    glutMainLoop();
    return 0;
}

#endif  // End of the #ifndef
