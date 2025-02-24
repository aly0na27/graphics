#include <GLUT/glut.h>
#include <math.h>
#include <stdio.h>

float angleSun = 3.1415926;  // Угол движения солнца
float angleMoon = 0.0; // Угол движения луны (начинаем с противоположной точки)
float radiusX = 1.5; // Ширина траектории (по X)
float radiusY = 1.2; // Высота траектории (по Y)
float speed = 0.02;  // Скорость движения
float offsetY = -0.3; // Смещение вниз
float skyBrightness = 1.0; // Яркость неба (1.0 - день, 0.0 - ночь)
float isDay = true;

// Обобщенная функция для рисования треугольников
void drawTriangle(float x1, float y1, float r1, float g1, float b1,
    float x2, float y2, float r2, float g2, float b2,
    float x3, float y3, float r3, float g3, float b3) {

    glBegin(GL_TRIANGLES);
    glColor3f(r1, g1, b1); glVertex2f(x1, y1); // 1-я вершина
    glColor3f(r2, g2, b2); glVertex2f(x2, y2); // 2-я вершина
    glColor3f(r3, g3, b3); glVertex2f(x3, y3); // 3-я вершина
    glEnd();
}

// Функция рисования овального лепестка
void drawOval(float x, float y, float width, float height, float angle, float r, float g, float b) {
    int segments = 50; // Количество точек для плавности
    float radAngle = angle * 3.1415926 / 180.0; // Перевод в радианы

    glColor3f(r, g, b); // Цвет лепестка
    glBegin(GL_POLYGON);
    for (int i = 0; i < segments; i++) {
        float theta = 2.0 * 3.1415926 * i / segments; // Угол в радианах
        float dx = cos(theta) * width;  // Учитываем ширину овала
        float dy = sin(theta) * height; // Учитываем высоту овала

        // Поворачиваем лепестки
        float rotatedX = dx * cos(radAngle) - dy * sin(radAngle);
        float rotatedY = dx * sin(radAngle) + dy * cos(radAngle);

        glVertex2f(x + rotatedX, y + rotatedY);
    }
    glEnd();
}

// Функция рисования цветкаs
void drawFlower(float x, float y, float r, float g, float b) {
    int petals = 6; // Количество лепестков
    float petalWidth = 0.035;
    float petalHeight = 0.075;
    float centerRadius = 0.025;
    float angleStep = 360.0 / petals;

    // Рисуем лепестки (овалами)
    for (int i = 0; i < petals; i++) {
        float angle = i * angleStep;
        drawOval(x, y, petalWidth, petalHeight, angle, r, g, b);
    }

    // Рисуем центр цветка (жёлтый круг)
    glColor3f(1.0, 1.0, 0.0); // Жёлтый центр
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i += 10) {
        float theta = i * 3.1415926 / 180.0;
        glVertex2f(x + cos(theta) * centerRadius, y + sin(theta) * centerRadius);
    }
    glEnd();
}

// Функция рисования круга (солнца или луны)
void drawCircle(float x, float y, float r, float g, float b) {
    int segments = 100;
    float radius = 0.1;
    
    glColor3f(r, g, b); // Цвет (жёлтый для солнца, белый для луны)
    glBegin(GL_POLYGON);
    for (int i = 0; i < segments; i++) {
        float theta = 2.0 * 3.1415926 * i / segments;
        float dx = cos(theta) * radius;
        float dy = sin(theta) * radius;
        glVertex2f(x + dx, y + dy);
    }
    glEnd();
}

void drawFlowers() {
    drawFlower(-0.8, -0.7, 0.97, 0.62, 0.96);
    drawFlower(0.6, -0.9, 0.95, 0.49, 0.23);
    drawFlower(0.2, -0.55, 0.23, 0.69, 0.95);
    drawFlower(-0.6, -0.9, 0.72, 0.23, 0.95);
    drawFlower(-0.3, -0.89, 0.95, 0.23, 0.75);
    drawFlower(0.7, -0.6, 0.23, 0.35, 0.95);
}


// Рисуем зеленое поле
void drawField() {

    glColor3f(0.37, 0.91, 0.28); // Зелёный цвет
    glRectf(-1.0, -1.0, 1.0, -0.3);
}

void drawHouse() {
    // Здесь рисуем фасад дома
    glColor3f(0.74, 0.49, 0.13); // коричневый цвет
    glRectf(-0.2, -0.4, 0.2, -0.05);

    // Здесь рисуем крышу дома
    drawTriangle(-0.3, -0.05, 0.6, 0.06, 0.16,  // Красный
        0, 0.4, 0.6, 0.06, 0.16,  // Зелёный
        0.3, -0.05, 0.6, 0.06, 0.16);
    
    // Здесь рисуем дверь в дом
    glColor3f(0.6, 0.29, 0.06); // коричневый цвет
    glRectf(-0.08, -0.4, 0.08, -0.2);
}

// функция рисования елового дерева
void drawTree(float x, float y = 0) {
    // рисуем 3 треугольника, а под ним ствол
    glColor3f(0.34, 0.21, 0.07);
    glRectf(x+0.17, -0.45-y, x+0.23, -0.3-y);

    drawTriangle(x, -0.35-y, 0.07, 0.34, 0.07,
        x + 0.2, 0.2-y, 0.07, 0.34, 0.07, 
        x+0.4, -0.35-y, 0.07, 0.34, 0.07);
    
    drawTriangle(x+0.05, -0.1-y, 0.21, 0.48, 0.12,
        x + 0.2, 0.3-y, 0.21, 0.48, 0.12, 
        x+0.4-0.05, -0.1-y, 0.21, 0.48, 0.12);

    drawTriangle(x+0.08, 0.1-y, 0.29, 0.65, 0.16,
        x + 0.2, 0.5-y, 0.29, 0.65, 0.16, 
        x+0.4-0.08, 0.1-y, 0.29, 0.65, 0.16);
}

void drawForest() {
    drawTree(-0.98);
    drawTree(-0.7, 0.1);
    drawTree(0.3);
    drawTree(0.6, -0.1);
}

// Функция отрисовки
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    if (isDay) {
        glClearColor(0.62, 0.94, 0.96, 1.0); // Яркий день
    } else {
        glClearColor(0.16, 0.14, 0.36, 1.0); // Затемнённый фон
    }
    // glClearColor(0.2 * skyBrightness, 0.6 * skyBrightness, 1.0 * skyBrightness, 1.0); // Плавное затемнение

    drawField();
    drawHouse();
    drawForest();

    drawFlowers();
    // Вычисляем координаты солнца и луны по овалу
    float sunX = cos(angleSun) * radiusX;
    float sunY = sin(angleSun) * radiusY + offsetY;
    float moonX = cos(angleMoon) * radiusX;
    float moonY = sin(angleMoon) * radiusY + offsetY;

    drawCircle(sunX, sunY, 1.0, 1.0, 0.0); // Рисуем солнце (жёлтое)
    drawCircle(moonX, moonY, 1.0, 1.0, 1.0); // Рисуем луну (белая)

    if (!isDay) {
        glEnable(GL_BLEND);  // Включаем режим смешивания
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glColor4f(0.0f, 0.0f, 0.0f, 0.5f); // Чёрный цвет с 50% прозрачностью

        glBegin(GL_QUADS); // Рисуем затемняющий квадрат
            glVertex2f(-1.0f, -1.0f);
            glVertex2f( 1.0f, -1.0f);
            glVertex2f( 1.0f,  1.0f);
            glVertex2f(-1.0f,  1.0f);
        glEnd();

        glDisable(GL_BLEND); // Отключаем смешивание после рисования квадрата
    }
    glFlush();
}


// Функция обновления анимации
void update(int value) {
    angleSun -= speed;
    angleMoon -= speed;

    if (angleSun <= 0.0) { // Солнце зашло за границу (π радиан = 180°)
        isDay = false;
    } else {
        isDay = true;
    }
    // Если угол солнца/луны выходит за пределы полного круга, сбрасываем в начало
    if (angleSun <= -3.1415926) {
        angleSun = 3.1415926;
    }
    if (abs(angleMoon) >= 2 * 3.1415926) {
        angleMoon = 0.0;
    }

    glutPostRedisplay(); // Перерисовываем экран
    glutTimerFunc(16, update, 0); // Запускаем следующий кадр через 16 мс (~60 FPS)
}

void init() {
    glClearColor(0.2, 0.6, 1.0, 1.0); // Начальное состояние: день
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(1500, 1500);
    glutCreateWindow("День и ночь");

    init();

    glutDisplayFunc(display);
    glutTimerFunc(16, update, 0); // Тут запускаем анимацию смены дня и ночи

    glutMainLoop();
    return 0;
}
