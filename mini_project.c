#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ROWS 20
#define COLS 40
#define MAXOBJ 50

char canvas[ROWS][COLS];

typedef struct {
    int type;      // 1-Line 2-Rectangle 3-Circle 4-Triangle
    int p[6];
} Shape;

Shape shapes[MAXOBJ];
int count = 0;

/* Initialize Canvas */
void initCanvas() {
    int i, j;
    for(i = 0; i < ROWS; i++)
        for(j = 0; j < COLS; j++)
            canvas[i][j] = '_';
}

/* Display Canvas */
void displayCanvas() {
    int i, j;
    for(i = 0; i < ROWS; i++) {
        for(j = 0; j < COLS; j++)
            printf("%c", canvas[i][j]);
        printf("\n");
    }
}

/* Plot a Point */
void plot(int x, int y) {
    if(x >= 0 && x < ROWS && y >= 0 && y < COLS)
        canvas[x][y] = '*';
}

/* Draw Line */
void drawLine(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int steps = (dx > dy) ? dx : dy;

    float xInc = (float)(x2 - x1) / steps;
    float yInc = (float)(y2 - y1) / steps;

    float x = x1, y = y1;

    for(int i = 0; i <= steps; i++) {
        plot((int)(x + 0.5), (int)(y + 0.5));
        x += xInc;
        y += yInc;
    }
}

/* Draw Rectangle */
void drawRectangle(int x, int y, int width, int height) {
    drawLine(x, y, x, y + width);
    drawLine(x + height, y, x + height, y + width);
    drawLine(x, y, x + height, y);
    drawLine(x, y + width, x + height, y + width);
}

/* Draw Circle */
void drawCircle(int xc, int yc, int r) {
    int x, y;

    for(x = 0; x < ROWS; x++) {
        for(y = 0; y < COLS; y++) {
            int d = (x - xc)*(x - xc) + (y - yc)*(y - yc);
            if(abs(d - r*r) <= r)
                plot(x, y);
        }
    }
}

/* Draw Triangle */
void drawTriangle(int x1,int y1,int x2,int y2,int x3,int y3) {
    drawLine(x1,y1,x2,y2);
    drawLine(x2,y2,x3,y3);
    drawLine(x3,y3,x1,y1);
}

/* Redraw all objects */
void redraw() {
    initCanvas();

    for(int i = 0; i < count; i++) {

        if(shapes[i].type == 1)
            drawLine(shapes[i].p[0], shapes[i].p[1],
                     shapes[i].p[2], shapes[i].p[3]);

        else if(shapes[i].type == 2)
            drawRectangle(shapes[i].p[0], shapes[i].p[1],
                          shapes[i].p[2], shapes[i].p[3]);

        else if(shapes[i].type == 3)
            drawCircle(shapes[i].p[0], shapes[i].p[1],
                       shapes[i].p[2]);

        else if(shapes[i].type == 4)
            drawTriangle(shapes[i].p[0], shapes[i].p[1],
                         shapes[i].p[2], shapes[i].p[3],
                         shapes[i].p[4], shapes[i].p[5]);
    }
}

/* Add Shape */
void addShape() {
    int choice;

    printf("\n1.Line\n2.Rectangle\n3.Circle\n4.Triangle\n");
    printf("Enter shape: ");
    scanf("%d", &choice);

    shapes[count].type = choice;

    if(choice == 1) {
        printf("Enter x1 y1 x2 y2: ");
        scanf("%d%d%d%d",
              &shapes[count].p[0],
              &shapes[count].p[1],
              &shapes[count].p[2],
              &shapes[count].p[3]);
    }

    else if(choice == 2) {
        printf("Enter x y width height: ");
        scanf("%d%d%d%d",
              &shapes[count].p[0],
              &shapes[count].p[1],
              &shapes[count].p[2],
              &shapes[count].p[3]);
    }

    else if(choice == 3) {
        printf("Enter centerX centerY radius: ");
        scanf("%d%d%d",
              &shapes[count].p[0],
              &shapes[count].p[1],
              &shapes[count].p[2]);
    }

    else if(choice == 4) {
        printf("Enter x1 y1 x2 y2 x3 y3: ");
        scanf("%d%d%d%d%d%d",
              &shapes[count].p[0],
              &shapes[count].p[1],
              &shapes[count].p[2],
              &shapes[count].p[3],
              &shapes[count].p[4],
              &shapes[count].p[5]);
    }

    count++;
}

/* Delete Shape */
void deleteShape() {
    int index;

    printf("Enter object index (0-%d): ", count - 1);
    scanf("%d", &index);

    if(index < 0 || index >= count) {
        printf("Invalid Index\n");
        return;
    }

    for(int i = index; i < count - 1; i++)
        shapes[i] = shapes[i + 1];

    count--;
}

/* Modify Shape */
void modifyShape() {
    int index;

    printf("Enter object index (0-%d): ", count - 1);
    scanf("%d", &index);

    if(index < 0 || index >= count) {
        printf("Invalid Index\n");
        return;
    }

    printf("Re-enter parameters:\n");

    if(shapes[index].type == 1)
        scanf("%d%d%d%d",
              &shapes[index].p[0],
              &shapes[index].p[1],
              &shapes[index].p[2],
              &shapes[index].p[3]);

    else if(shapes[index].type == 2)
        scanf("%d%d%d%d",
              &shapes[index].p[0],
              &shapes[index].p[1],
              &shapes[index].p[2],
              &shapes[index].p[3]);

    else if(shapes[index].type == 3)
        scanf("%d%d%d",
              &shapes[index].p[0],
              &shapes[index].p[1],
              &shapes[index].p[2]);

    else if(shapes[index].type == 4)
        scanf("%d%d%d%d%d%d",
              &shapes[index].p[0],
              &shapes[index].p[1],
              &shapes[index].p[2],
              &shapes[index].p[3],
              &shapes[index].p[4],
              &shapes[index].p[5]);
}

/* Main */
int main() {
    int choice;

    do {
        printf("\n--- 2D Graphics Editor ---\n");
        printf("1.Add Object\n");
        printf("2.Delete Object\n");
        printf("3.Modify Object\n");
        printf("4.Display Picture\n");
        printf("5.Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch(choice) {

            case 1:
                addShape();
                break;

            case 2:
                deleteShape();
                break;

            case 3:
                modifyShape();
                break;

            case 4:
                redraw();
                displayCanvas();
                break;

            case 5:
                printf("Exiting...\n");
                break;

            default:
                printf("Invalid Choice\n");
        }

    } while(choice != 5);

    return 0;
}
