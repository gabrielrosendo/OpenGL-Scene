#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

// Adjusted window size and layout for the 5x3 matrix
const int windowRows = 5;  // 5 rows
const int windowCols = 3;  // 3 columns

// Halved dimensions for each window (width and height)
const float windowWidth = 0.1f;
const float windowHeight = 0.125f;

// Adjusted spacing between windows
const float windowSpacingX = 0.20f; // Adjusted for proper even spacing
const float windowSpacingY = 0.17f;  // Vertical spacing between rows

// Function to draw a window with dividers and an outline for depth
void drawWindow(float x, float y, float width, float height) {
	// Draw the outline to indicate depth (slightly larger than the window)
	glLineWidth(1.0f);
	glColor3f(0.1f, 0.1f, 0.1f); // Dark gray outline for depth
	glBegin(GL_LINE_LOOP);
    	glVertex2f(x - 0.005f, y + 0.005f);
    	glVertex2f(x + width + 0.005f, y + 0.005f);
    	glVertex2f(x + width + 0.005f, y - height - 0.005f);
    	glVertex2f(x - 0.005f, y - height - 0.005f);
	glEnd();
	
	// Draw the left and right panels
	glBegin(GL_QUADS);

	// Left Panel (Darker Blue)
	glColor3f(0.1, 0.2, 0.4); // Dark Blue
	glVertex2f(x, y);
	glVertex2f(x + width / 2.0f, y);
	glVertex2f(x + width / 2.0f, y - height);
	glVertex2f(x, y - height);

	// Right Panel (Lighter Blue)
	glColor3f(0.2, 0.4, 0.8); // Lighter Blue
	glVertex2f(x + width / 2.0f, y);
	glVertex2f(x + width, y);
	glVertex2f(x + width, y - height);
	glVertex2f(x + width / 2.0f, y - height);

	glEnd();

	// Draw the vertical dividers for the left and right panels with darker gray
	glLineWidth(1.5f); // Thin dividers
	glColor3f(0.3f, 0.3f, 0.3f); // Darker gray for dividers

	glBegin(GL_LINES);

	// Left Panel Dividers (4 bars)
	for (float i = x + (width / 5.0f); i < x + width / 2.0f; i += width / 5.0f) {
    	glVertex2f(i, y);
    	glVertex2f(i, y - height);
	}

	// Right Panel Dividers (4 bars)
	for (float i = x + width / 2.0f + (width / 5.0f); i < x + width; i += width / 5.0f) {
    	glVertex2f(i, y);
    	glVertex2f(i, y - height);
	}

	glEnd();
	
	//Draw window dividers
	glLineWidth(1.5f);
	glColor3f(1.0f, 1.0f, 1.0f); // Pure white
	glBegin(GL_LINE_LOOP);
    	glVertex2f(x + (width/2), y + 0.005f);
    	glVertex2f(x + (width/2), y - height - 0.005f);
	glEnd();
	
	// Draw the window pane (slightly larger than the window)
	glLineWidth(1.0f);
	glColor3f(1.0f, 1.0f, 1.0f); // Pure white
	glBegin(GL_LINE_LOOP);
    	glVertex2f(x - 0.0025f, y + 0.0025f);
    	glVertex2f(x + width + 0.0025f, y + 0.0025f);
    	glVertex2f(x + width + 0.0025f, y - height - 0.0025f);
    	glVertex2f(x - 0.0025f, y - height - 0.0025f);
	glEnd();
}

// Function to draw all windows on the building
void drawAllWindows() {
	// Iterate through the 4x3 matrix of windows
	for (int row = 0; row < windowRows; ++row) {
    	float startY = 0.60f - row * (windowHeight + windowSpacingY);  // Adjust vertical starting position
    	for (int col = 0; col < windowCols; ++col) {
        	float startX = -0.35f + col * (windowWidth + windowSpacingX); // Adjust for 3 windows per row
        	drawWindow(startX, startY, windowWidth, windowHeight);
    	}
	}
}


void drawSky() {
	glBegin(GL_QUADS);
    	// Top-left vertex (sky blue)
    	glColor3f(0.53f, 0.81f, 0.92f);
    	glVertex2f(-1.0f, 1.0f);
	 
    	// Top-right vertex (sky blue)
    	glColor3f(0.53f, 0.81f, 0.92f);
    	glVertex2f(1.0f, 1.0f);
	 
    	// Bottom-right vertex (white)
    	glColor3f(1.0f, 1.0f, 1.0f);
    	glVertex2f(1.0f, -1.0f);
	 
    	// Bottom-left vertex (white)
    	glColor3f(1.0f, 1.0f, 1.0f);
    	glVertex2f(-1.0f, -1.0f);
	glEnd();
}

void drawBuilding() {
	// Draw the main building (light gray)
	glColor3f(0.9f, 0.9f, 0.9f); // Light gray for the building
	glBegin(GL_QUADS);
    	glVertex2f(-0.6f, -0.75f);
    	glVertex2f(0.65f, -0.75f);
    	glVertex2f(0.65f, 0.8f);
    	glVertex2f(-0.6f, 0.8f);
	glEnd();

	// Draw the dark blue square on the building
	glColor3f(0.1f, 0.2f, 0.4f); // Dark blue for square
	glBegin(GL_QUADS);
    	glVertex2f(-0.4f, 0.035f);
    	glVertex2f(0.1f, 0.035f);
    	glVertex2f(0.1f, -0.415f);
    	glVertex2f(-0.4f, -0.415f);
	glEnd();
    
	// Draw the lines on the building
	glColor3f(0.8f, 0.8f, 0.8f); // Light gray for the lines (slightly darker than the building)
    
	// Vertical lines
	glBegin(GL_LINES);
    	glVertex2f(-0.35f, 0.8f);   // Top to bottom left vertical
    	glVertex2f(-0.35f, -0.75f);

    	glVertex2f(-0.25f, 0.8f);	// Top to bottom center left vertical
    	glVertex2f(-0.25f, -0.75f);

    	glVertex2f(0.05f, 0.8f);	// Top to bottom center right vertical
    	glVertex2f(0.05f, -0.75f);
   	 
    	glVertex2f(-0.05f, 0.8f);	// Top to bottom center right vertical
    	glVertex2f(-0.05f, -0.75f);
   	 
    	glVertex2f(0.25f, 0.8f);	// Top to bottom center right vertical
    	glVertex2f(0.25f, -0.75f);

    	glVertex2f(0.35f, 0.8f); 	// Top to bottom right vertical
    	glVertex2f(0.35f, -0.75f);
	glEnd();

	// Horizontal lines
	glBegin(GL_LINES);
    	glVertex2f(-0.6f, 0.7f);	// Left to right top horizontal
    	glVertex2f(0.65f, 0.7f);

    	glVertex2f(-0.6f, 0.4f);	// Left to right upper middle horizontal
    	glVertex2f(0.65f, 0.4f);

    	glVertex2f(-0.6f, 0.1f);	// Left to right lower middle horizontal
    	glVertex2f(0.65f, 0.1f);

    	glVertex2f(-0.6f, -0.2f);   // Left to right bottom horizontal
    	glVertex2f(0.65f, -0.2f);

    	glVertex2f(-0.6f, -0.50f);   // Left to right bottom horizontal
    	glVertex2f(0.65f, -0.50f);
	glEnd();
}   
    

void drawPillars(){
	// Dark brown color
	glColor3f(0.20f, 0.10f, 0.05f);

	// Left pillar (half the height of the right pillar)
	glBegin(GL_QUADS);
    	glVertex2f(-0.80f, -1.0f); // Bottom-left
    	glVertex2f(-0.6f, -1.0f); // Bottom-right
    	glVertex2f(-0.6f, 0.7f);  // Top-right (half height)
    	glVertex2f(-0.80f, 0.7f);  // Top-left
	glEnd();

	// Right pillar (full height)
	glBegin(GL_QUADS);
    	glVertex2f(0.65f, -1.0f);  // Bottom-left
    	glVertex2f(0.95f, -1.0f);  // Bottom-right
    	glVertex2f(0.95f, 0.7f);  // Top-right
    	glVertex2f(0.65f, 0.7f);  // Top-left
	glEnd();
    
	glColor3f(0.9f, 0.9f, 0.9f);
    
	glBegin(GL_QUADS);
    	glVertex2f(0.95f, -1.0f);  // Bottom-left
    	glVertex2f(1.0f, -1.0f);  // Bottom-right
    	glVertex2f(1.0f, 0.7f);  // Top-right
    	glVertex2f(0.95f, 0.7f);  // Top-left
	glEnd();
    
	// Draw the cover at the bottom of the building
    	glColor3f(0.4f, 0.4f, 0.4f); // Dark gray for the cover
    	glBegin(GL_QUADS);
        	glVertex2f(-0.85f, -0.8f);
        	glVertex2f(1.0f, -0.8f);
        	glVertex2f(1.0f, -0.75f);  // Slightly above the bottom of the building
        	glVertex2f(-0.85f, -0.75f);
    	glEnd();
    	
    	// Iterate through the 4x2 matrix of windows for the right pillar
	for (int row = 0; row < windowRows; ++row) {
    	float startY = 0.60f - row * (windowHeight + windowSpacingY);  // Adjust vertical 				starting position
    	for (int col = 0; col < 2; ++col) {
        	float startX = 0.6f + col * (windowWidth + 0.1); // Adjust for 3 windows per row
        	drawWindow(startX, startY, windowWidth, windowHeight);
    	}
	}
	
	// Iterate through the 4x1 matrix of windows for the left pillar
	for (int row = 0; row < windowRows; ++row) {
    	float startY = 0.60f - row * (windowHeight + windowSpacingY);  // Adjust vertical 				starting position
        float startX = -0.7f;
        drawWindow(startX, startY, windowWidth, windowHeight);
    	
	}
}        

void drawBrickWall() {
    // Set the color to brown for the bricks
    glColor3f(0.4f, 0.2f, 0.0f);

    // Dimensions of the wall
    float wall_left = -0.65f;  // Extended slightly to the left
    float wall_right = 0.65f;
    float wall_bottom = -1.0f;
    float wall_top = -0.8f;

    // Brick dimensions
    float brick_width = 0.05f;
    float brick_height = 0.025f;
    float mortar_thickness = 0.005f;

    // Counts Row Number
    int count = 0;
    
    // Draw the bricks
    for (float y = wall_bottom; y < wall_top; y += brick_height + mortar_thickness) {
        // For every second row, offset the x position to stagger the bricks
        count++;
        bool is_offset_row = (count % 2) == 1;
      
        for (float x = wall_left; x < wall_right + 1; x += brick_width + mortar_thickness) {
            // Apply the offset for the rows that need staggering
            float x_offset = is_offset_row ? -(brick_width / 2) : 0.0f;

            // Draw a single brick
            glBegin(GL_QUADS);
                glVertex2f(x + x_offset, y);
                glVertex2f(x + x_offset + brick_width, y);
                glVertex2f(x + x_offset + brick_width, y + brick_height);
                glVertex2f(x + x_offset, y + brick_height);
            glEnd();
        }
    }
}


// Display callback function
void display() {
	glClear(GL_COLOR_BUFFER_BIT);

	// Draw the Sky
	drawSky();
    
	// Draw Brick Wall
	drawBrickWall();
    
	//Draw Pillars
	drawPillars();
	
	// Draw the building (light gray background)
	drawBuilding();

	// Draw all the windows in a 4x3 matrix
	drawAllWindows();

	glFlush();
}


// OpenGL initialization
void init() {
	glClearColor(1.0, 1.0, 1.0, 1.0); // Background color (white)
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1.0, 1.0, -1.0, 1.0); // Set up an orthographic 2D projection
}

// Main function
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(750,750);
	glutCreateWindow("Building with Recreated Windows");

	// Initialize OpenGL settings
	init();

	// Set the display callback function
	glutDisplayFunc(display);

	// Start the main loop
	glutMainLoop();
	return 0;
}
