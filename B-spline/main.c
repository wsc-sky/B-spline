//
//  main.c
//  B-spline
//
//  Created by 韦思聪 on 16/11/28.
//  Copyright © 2016年 Wei sicong. All rights reserved.
//

// This is a template for assignment 2 of DM6122: 3D Modeling and Reconstruction.
// NTU
// August 2016
//
// Open a new project "Win32 Console Application" and add sample.c to Source Files

#include <stdio.h>
#include <GLUT/GLUT.h>
#include <math.h>


typedef struct {
    double x, y;      // x, y coordinates of a 2D point
} Point2d;


typedef struct {
    int      degree;   // degree of the B-spline curve
    int      cntNum;   // number of the deBoor points of the B-spline curve
    double   *knots;   // knot vector of the B-spline curve
    Point2d  *cnt;     // control points of the B-spline curve
} Bspline;


// B-spline curve
Bspline  bcr;

Point2d  curve1;

// The parameters are used to define a visible window in this application's World Coordinate System.
double	winLLx = 0.0;
double  winLLy = 0.0;
double	winLen = 100.0;

//
int    displayCP     = 1;
int    adaptivePlot  = 1;
int    samplingPnt   = 0;
int    tessNum       = 10;
double tessEps     = 2.;


//============================================================
static void Init(void)
{
    glClearColor (1.0, 1.0, 1.0, 0.0);         // set display-window color to white
    glMatrixMode (GL_PROJECTION);	           // set projection parameters
    gluOrtho2D (winLLx, winLLx+winLen, winLLy, winLLy+winLen);   // set an orthogonal projection
}




//============================================================
void uniformRender()
{
    // TODO:  add your own codes
   // printf("d------->%d",seg);
    
    
    int cntNum = bcr.cntNum, degree = bcr.degree;
    float range = bcr.knots[cntNum] - bcr.knots[degree];
    Point2d sample[2000];
    int smp = 0;
    Point2d pro[30][30];
    int i,j,x;
    int j2 = 0,h;
    float t ;

    
    
    for (i = 0; i <tessNum; i++){
        t =bcr.knots [degree] + range/(tessNum-1) * i;
     //           printf("---------%f",t);
        for (j =  degree; j<=cntNum; j++){
            if ((t <= bcr.knots[j])&&(t>=bcr.knots[j-1]))
                j2 = j - 1 ;
        }
      //      printf("---------%d",j2);
        for (x = j2-degree; x <= j2 ; x++ ){
              pro[0][x] = bcr.cnt[x];
      //      printf("---------%lf",pro[0][x].x);

            for (h=1; h<= degree; h++){
                pro[h][x].x = (1- (t - bcr.knots[x] )/ (bcr.knots[x+degree +1 -h] - bcr.knots[x]))* pro[h-1][x-1].x +
                               (t - bcr.knots[x])/ (bcr.knots[x+degree +1 -h] - bcr.knots[x])* pro[h-1][x].x;
                pro[h][x].y = (1- (t - bcr.knots[x] )/ (bcr.knots[x+degree +1 -h] - bcr.knots[x]))* pro[h-1][x-1].y +
                (t - bcr.knots[x])/ (bcr.knots[x+degree +1 -h] - bcr.knots[x])* pro[h-1][x].y;
            }
        }
        
        sample[smp].x = pro[degree][j2].x;
        sample[smp].y = pro[degree][j2].y;
        smp++;
        
    }

    for (i = 0; i < smp - 1; i++) {
        
        if (samplingPnt) {
            glBegin(GL_POINTS);
            glColor3f(0.0, 0.0, 0.0);
            glVertex2f(sample[i].x, sample[i].y);
            glEnd();
        }
        glColor3f(0.0, 0.0, 1.0);
        glBegin(GL_LINES);
        glVertex2f(sample[i].x, sample[i].y);
        glVertex2f(sample[i + 1].x, sample[i + 1].y);
        glEnd();
    }

    
    
  /*
    int seg = bcr.cntNum - bcr.degree;//number of segments
    int cntNum = bcr.cntNum, degree = bcr.degree;
    float uniform;
    
    //float inter = uniform / tessNum;
    int i, j;
    float t;
    int z, h;
    Point2d smpple[2000];
    int smp = 0;
    Point2d pro[30][30];
    int insert = tessNum / seg, leave = tessNum % seg;
    int num = 0;
    float inter = 0;
    //initial
    for (i = 0; i < 30; i++) {
        for (j = 0; j < 30; j++) {
            pro[i][j].x = 0;
            pro[i][j].y = 0;
        }
    }
    
    //calculate all the points in the tree
    for (i = degree; i < cntNum; i++) {
        
        
        uniform = bcr.knots[i+1] - bcr.knots[i];
        //initial all the cntpoints
        int m = i;
        for (z = degree + 1; z > 0; z--) {
            pro[0][z] = bcr.cnt[m];
            m--;
        }
        if (i - degree < leave) {
            num = insert + 1;
        }
        else {
            num = insert;
        }
        inter = uniform / num;
        for (j = 0; j <= num; j++) {
            t = bcr.knots[i] + inter * j;
            for (h = 1; h <= degree; h++) {
                for (z = 1 + h; z <= degree + 1; z++) {
                    
                    pro[h][z].x = (1 - (t - bcr.knots[z - 1 + i - degree]) / (bcr.knots[z + i - h] - bcr.knots[z - 1 + i - degree]))*pro[h - 1][z - 1].x + (t - bcr.knots[z - 1 + i - degree]) / (bcr.knots[z + i - h] - bcr.knots[z - 1 + i - degree])*pro[h - 1][z].x;
                    pro[h][z].y = (1 - (t - bcr.knots[z - 1 + i - degree]) / (bcr.knots[z + i - h] - bcr.knots[z - 1 + i - degree]))*pro[h - 1][z - 1].y + (t - bcr.knots[z - 1 + i - degree]) / (bcr.knots[z + i - h] - bcr.knots[z - 1 + i - degree])*pro[h - 1][z].y;;
                    
                }
            }
            smpple[smp].x = pro[degree][degree + 1].x;
            smpple[smp].y = pro[degree][degree + 1].y;
            smp++;
        }
        
    }
    
    
    
    for (i = 0; i < smp - 1; i++) {
        
        if (samplingPnt) {
            glBegin(GL_POINTS);
            glColor3f(0.0, 0.0, 0.0);
            glVertex2f(smpple[i].x, smpple[i].y);
            glEnd();
        }
        glColor3f(0.0, 0.0, 1.0);
        glBegin(GL_LINES);
        glVertex2f(smpple[i].x, smpple[i].y);
        glVertex2f(smpple[i + 1].x, smpple[i + 1].y);
        glEnd();
    }

*/
    
}


void extractBezier(Point2d* bez, int ind)
{
    int     i, j;
    int     k;
    double  knots[50];
    Point2d cnt[30];
    
    k = bcr.degree;
    
    // copy one segment
    for (i = ind - k, j = 0; i <= ind; i++) {
        cnt[j].x = bcr.cnt[i].x;
        cnt[j].y = bcr.cnt[i].y;
        j++;
    }
    for (i = ind - k, j = 0; i <= ind + k + 1; i++) {
        knots[j] = bcr.knots[i];
        j++;
    }
    
    // insert knots to make the left end be Bezier end
    while (1) {
        for (i = k - 1; i>0; i--) {
            if (knots[i] < knots[k]) {
                j = i;
                break;
            }
            j = 0;
        }
        
        if (j == 0) break;
        // update control points
        for (i = 0; i<j; i++) {
            cnt[i].x = ((knots[k + 1 + i] - knots[k]) / (knots[k + i + 1] - knots[i + 1]))*cnt[i].x
            + ((knots[k] - knots[i + 1]) / (knots[k + i + 1] - knots[i + 1]))*cnt[i + 1].x;
            cnt[i].y = ((knots[k + 1 + i] - knots[k]) / (knots[k + i + 1] - knots[i + 1]))*cnt[i].y
            + ((knots[k] - knots[i + 1]) / (knots[k + i + 1] - knots[i + 1]))*cnt[i + 1].y;
        }
        // update knots
        for (i = 0; i<j; i++)
            knots[i] = knots[i + 1];
        knots[j] = knots[k];
    }
    
    // insert knots to make the right end be Bezier end
    while (1) {
        for (i = k + 2; i< k + k + 1; i++) {
            if (knots[i] > knots[k + 1]) {
                j = i;
                break;
            }
            j = 0;
        }
        
        if (j == 0) break;
        
        // update control points
        for (i = k; i >= j - k; i--) {
            cnt[i].x = ((knots[k + i] - knots[k + 1]) / (knots[k + i] - knots[i]))*cnt[i - 1].x
            + ((knots[k + 1] - knots[i]) / (knots[k + i] - knots[i]))*cnt[i].x;
            cnt[i].y = ((knots[k + i] - knots[k + 1]) / (knots[k + i] - knots[i]))*cnt[i - 1].y
            + ((knots[k + 1] - knots[i]) / (knots[k + i] - knots[i]))*cnt[i].y;
        }
        // update knots
        for (i = k + k + 1; i>j; i--)
            knots[i] = knots[i - 1];
        knots[j] = knots[k + 1];
    }
    
    // return the Bezier control points
    for (i = 0; i< bcr.cntNum; i++) {
        bez[i].x = cnt[i].x;
        bez[i].y = cnt[i].y;
    }
}


//============================================================
void plotBezier(Point2d* bez, int deg)
{
    // TODO:  add your own codes
    double max_distance = 0;
    double h[30];
    int n = deg+1;
    
    
    for (int i = 0; i < n; i++)
    {
        h[i] = fabs((bez[n - 1].x - bez[0].x)*(bez[i].y - bez[0].y) - (bez[n - 1].y - bez[0].y)*(bez[i].x - bez[0].x)) / fabs(sqrt((bez[n - 1].x - bez[0].x)*(bez[n - 1].x - bez[0].x) + (bez[n - 1].y - bez[0].y)*(bez[n - 1].y - bez[0].y)));
        
    }
    for (int i = 0; i < n; i++)
    {
        
        if (h[i] > max_distance)
            max_distance = h[i];
        
    }
    if (max_distance < tessEps)
    {
        if(samplingPnt){
            glColor3f(0.0, 0.0, 0.0);

        glBegin(GL_POINTS);
        glVertex2d(bez[0].x, bez[0].y);
        glVertex2d(bez[n - 1].x, bez[n - 1].y);
        glEnd();
        }
        glColor3f(0.0, 1.0, 0.0);
        glBegin(GL_LINES);
        glVertex2d(bez[0].x, bez[0].y);
        glVertex2d(bez[n - 1].x, bez[n - 1].y);
        glEnd();
       // printf("-----------------%d \n",n);
        return;
        
    }
    else
    {
        Point2d tmp[30];
        Point2d left[30];
        Point2d right[30];
        for (int i = 0; i <= n - 1; i++)
        {
            tmp[i] = bez[i];
        }
        for (int i = 0; i <= n - 1; i++)
        {
            left[i] = tmp[0];
            right[n - 1 - i] = tmp[n - 1 - i];
            for (int j = 0; j <= n - 1 - i - 1; j++)
            {
                tmp[j].x = 0.5 * (tmp[j].x + tmp[j + 1].x);
                tmp[j].y = 0.5 * (tmp[j].y + tmp[j + 1].y);
                
            }
        }
        

        plotBezier(left, deg);
        plotBezier(right, deg);
        
    }
}

//============================================================
void adaptiveRender()
{
    Point2d  bez[30];  // assume the degree is not greater than 29.
    int      i;
    
    for (i = bcr.degree; i < bcr.cntNum; i++)
    {
        if (fabs(bcr.knots[i] - bcr.knots[i + 1]) < 0.00001) continue;  // no segment, skip over
 // adaptively plot a Bezier curve
        extractBezier(bez, i);        // extract the i-th Bezier curve
        plotBezier(bez, bcr.degree);
    
    }

}



//============================================================
static void drawCurve( void )
{
    int i;
    
    glClear(GL_COLOR_BUFFER_BIT);	// clear display window
    glColor3f (1.0,0.0,0.0);   // set line segment color to red
    
    
    // Draw the control polygon
    glColor3f(1.0, 0.0, 0.0);
    glLineWidth(3.0);
    if (displayCP != 0) {
        glBegin(GL_LINE_STRIP);      // display the control polygon
        for (i=0; i<bcr.cntNum; i++)
            glVertex2f(bcr.cnt[i].x, bcr.cnt[i].y);
        glEnd();
        
        glPointSize(6.0);            // display the control points
        glBegin(GL_POINTS);
        for (i=0; i<bcr.cntNum; i++)
            glVertex2f(bcr.cnt[i].x, bcr.cnt[i].y);
        glEnd();
    }
    
    // Draw the curve
    glLineWidth(2.0);
    if (adaptivePlot) {  // plot adaptively
        glColor3f(0.0, 1.0, 0.0);
        adaptiveRender();
    }
    else {  // plot uniformly
        glColor3f(0.0, 0.0, 1.0);
        uniformRender();
    }
    
   // printf("draw curve");
    glFlush();		    // process all openGL routines as quickly as possible
    glutSwapBuffers();  // swap buffers to display the current frame
}

//============================================================
static void idle( void )
{
    drawCurve();
}


//============================================================
static void hotkey(unsigned char k, int x, int y)
{
    // Here we are processing keyboard events.
    switch (k)
    {
        case 27:
        //    free (bcr.cnt);
        //    free (bcr.knots);
            exit (0);
            break;
            
            // Toggle plotting the control polygon
        case 'C':
        case 'c':
            displayCP = !displayCP;
            break;
            
            // Toggle sampling points
        case 'P':
        case 'p':
            samplingPnt = !samplingPnt;
            break;
            
            // Toggle adaptive/uniform plotting
        case 'A':
        case 'a':
            adaptivePlot = !adaptivePlot;
            break;
            
            // Increase tessellation
        case '+':
        case '=':
            if (adaptivePlot) {
                tessEps *= 0.7;
                if (tessEps < 0.5)  tessEps = 0.01;
            }
            else {
                tessNum += 1;
                if (tessNum > 100) tessNum = 100;
            }
            break;
            
            // Decrease tessellation
        case '-':
        case '_':
            if (adaptivePlot) {
                tessEps *= 1.4;
                if (tessEps > 50)  tessEps = 100;
            }
            else {
                tessNum -= 1;
                if (tessNum < 2) tessNum = 2;
            }
            break;
    }
}

//============================================================
void chooseWindow()
{
    int    i;
    double left, right, bottom, top;
    
    
    left = right = bcr.cnt[0].x;
    for (i=1; i< bcr.cntNum; i++) {
        if (left > bcr.cnt[i].x)  left = bcr.cnt[i].x;
        if (right < bcr.cnt[i].x) right = bcr.cnt[i].x;
    }
    
    bottom = top = bcr.cnt[0].y;
    for (i=1; i< bcr.cntNum; i++) {
        if (bottom > bcr.cnt[i].y)  bottom = bcr.cnt[i].y;
        if (top < bcr.cnt[i].y) top = bcr.cnt[i].y;
    }
    
    winLen = top-bottom;
    if (winLen < right-left) winLen = right-left;
    
    winLen += 100;
    winLLy = bottom - 50;
    winLLx = left - 50 ;
}



//============================================================
int readFile( char* filename )
{
    FILE *fp;
    int  i;
        printf("%s,",filename);
    if((fp = fopen(filename, "r")) == NULL)
    {   printf("open fail");
        return 0;  // fail to open the file
    
    }

    fscanf(fp, "%d%d", &(bcr.degree), &(bcr.cntNum));
    bcr.knots = (double *) malloc ((bcr.cntNum+bcr.degree+1)*sizeof(double));
    bcr.cnt = (Point2d *) malloc (bcr.cntNum*sizeof(Point2d));
    
    for (i=0; i<= bcr.cntNum+bcr.degree; i++)
        fscanf(fp, "%lf", &(bcr.knots[i]));
    
    for (i=0; i< bcr.cntNum; i++)
        fscanf(fp, "%lf%lf", &(bcr.cnt[i].x),&(bcr.cnt[i].y));
    fclose (fp);

    
    chooseWindow();
    
    

    
    return 1;
}



//============================================================
void main( int argc, char *argv[] )
{
    // load the curve from a file
    char filename[50];
// /Users/weisicong/Desktop/bunny.m
    
  //  printf ("\n Please enter a filename: ");
 //   scanf("%s",filename);
    
  //  if (readFile(filename)==0) return;
    readFile("/Users/weisicong/Desktop/Bspline_project/quartic.txt");
    // help information
    printf("\n\nB-spline curve plotting\n");
    printf("NTU, September 2006\n");
    printf("\n");
    printf(" ESC      - Quit program\n");
    printf("\n");
    printf(" A/a : Toggle adaptive/uniform plotting (Default adaptive)\n");
    printf(" C/c : Toggle plotting the control polygon (Default On)\n");
    printf(" P/p : Toggle sampling points (Default Off)\n");
    printf(" +   : Increase tessellation\n");
    printf(" -   : Decrease tessellation\n");
    printf("\n");
    

    // set up graphics window
    glutInit(&argc, argv);                         // initialize GLUT
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);  // set display mode
    glutInitWindowSize (650, 650);                 // set display window width and height
    glutInitWindowPosition (100, 100);             // set top-left display window position
    glutCreateWindow ("2D B-spline curve plotting:        use +, -, c, a, p, and Esc keys.");
    
    Init();                        // execute initialization procedure
    glutIdleFunc(idle);            // enables us to make interaction.
    glutDisplayFunc(drawCurve);    // send graphics to display window
    glutKeyboardFunc(hotkey);
    
    glutMainLoop();                // display everything and wait
}

