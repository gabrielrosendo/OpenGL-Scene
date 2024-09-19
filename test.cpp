#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

void drawSky() {
	glColor3f(0.53f, 0.81f, 0.92f); // Light blue color for sky
	glBegin(GL_QUADS);
    	glVertex2f(-1.0f, 1.0f);
    	glVertex2f(1.0f, 1.0f);
    	glVertex2f(1.0f, -1.0f);
    	glVertex2f(-1.0f, -1.0f);
	glEnd();
}

void drawBuilding() {
	glColor3f(0.8f, 0.8f, 0.8f); // Light gray for the building
	glBegin(GL_QUADS);
    	glVertex2f(-0.5f, -0.5f);
    	glVertex2f(0.5f, -0.5f);
    	glVertex2f(0.5f, 0.5f);
    	glVertex2f(-0.5f, 0.5f);
	glEnd();
}

void drawWindows() {
	glColor3f(0.1f, 0.1f, 0.3f); // Dark color for windows

	// Building dimensions
	float building_left = -0.5f;
	float building_right = 0.5f;
	float building_bottom = -0.5f;
	float building_top = 0.5f;

	// Margins and spacing
	float edge_margin_x = 0.1f;
	float edge_margin_y = 0.1f;
	float window_spacing_x = 0.05f;
	float window_spacing_y = 0.05f;

	int num_columns = 3;
	int num_rows = 5;

	// Calculate total window area
	float total_window_area_width = building_right - building_left - 2 * edge_margin_x;
	float total_window_area_height = building_top - building_bottom - 2 * edge_margin_y;

	// Calculate total spacing between windows
	float total_spacing_x = (num_columns - 1) * window_spacing_x;
	float total_spacing_y = (num_rows - 1) * window_spacing_y;

	// Calculate window dimensions
	float window_width = (total_window_area_width - total_spacing_x) / num_columns;
	float window_height = (total_window_area_height - total_spacing_y) / num_rows;

	// Starting positions
	float start_x = building_left + edge_margin_x;
	float start_y = building_bottom + edge_margin_y;

	for (int i = 0; i < num_rows; i++) {
    	float y = start_y + i * (window_height + window_spacing_y);
    	for (int j = 0; j < num_columns; j++) {
        	float x = start_x + j * (window_width + window_spacing_x);
        	glBegin(GL_QUADS);
            	glVertex2f(x, y);
            	glVertex2f(x + window_width, y);
            	glVertex2f(x + window_width, y + window_height);
            	glVertex2f(x, y + window_height);
        	glEnd();
    	}
	}
}

void drawScene() {
	drawSky();
	drawBuilding();
	drawWindows();
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	drawScene();
	glutSwapBuffers();
}

void init() {
	glClearColor(0.53f, 0.81f, 0.92f, 1.0f); // Background color matches the sky
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
}

void reshape(int width, int height) {
	glViewport(0, 0, width, height);
}

void timer(int value) {
	// Placeholder for future animations
	glutPostRedisplay();
	glutTimerFunc(16, timer, 0); // Approximately 60 FPS
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // Use double buffering
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Improved Building Visualization");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutTimerFunc(0, timer, 0); // Start the timer for animations
	glutMainLoop();
	return 0;
}


