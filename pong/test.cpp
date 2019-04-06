#include <GL/glut.h>
#include <iostream>

void keyDown (unsigned char key, int x, int y)
{
    if (key == 's')
        std::cout << "keydown " << key << "\n";
}

void keyUp (unsigned char key, int x, int y)
{
    if (key == 'w')
        std::cout << "keyup " << key << "\n";
}

void render(void)
{
    std::cout << "render\n";
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(300, 300);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Hello world :D");
    glutDisplayFunc(render);
    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);
    glutMainLoop();

    return 0;
}