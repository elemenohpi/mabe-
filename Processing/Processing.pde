// 2D Array of objects
Cell[][] grid;
PImage img;

// Number of columns and rows in the grid
int cols;
int rows;
int cycle = 0;
String[] lines;
int iterator = 1;
String file = "0_1";
boolean end_flag = true;
int gridSize = 50;

void setup() {
  size(700,700);
  frameRate(10);
  img = loadImage("pizza.png");
  lines = loadStrings("../Results/"+file+".txt");
  cols = parseInt(lines[0].split(" ")[0]);
  rows = cols;
  grid = new Cell[cols][rows];
  for (int i = 0; i < cols; i++) {
    for (int j = 0; j < rows; j++) {
      // Initialize each object
      grid[i][j] = new Cell(i*gridSize + 50,j*gridSize + 50,50,50);
    }
  }
}

void draw() {
  scale(0.78);
  if(iterator == 1000)
  {
    if(end_flag)
    {
      print("ended");
      end_flag = false;
    }    
    return;
  }
  background(0);
  // The counter variables i and j are also the column and row numbers and 
  // are used as arguments to the constructor for each object in the grid.  
  for (int i = 0; i < cols; i++) {
    for (int j = 0; j < rows; j++) {
      // Display each object
      grid[i][j].display();
    }
  }
  
  //the region lines
  pushMatrix();
  translate(50,50);
  stroke(0);
  strokeWeight(3);
  line(cols/4*gridSize, 0, cols/4*gridSize, cols/4*gridSize);
  line(cols/4*gridSize, cols/4*gridSize, 0, cols/4*gridSize);
  line((cols - cols/4)*gridSize, 0, (cols - cols/4)*gridSize, cols/4*gridSize);
  line((cols - cols/4)*gridSize, cols/4*gridSize, cols*gridSize, cols/4*gridSize);
  line(0, (cols - cols/4)*gridSize, cols/4*gridSize, (cols - cols/4)*gridSize);
  line(cols/4*gridSize, (cols - cols/4)*gridSize, cols/4*gridSize, cols*gridSize);
  line((cols - cols/4)*gridSize, (cols - cols/4)*gridSize, cols*gridSize, (cols - cols/4)*gridSize);
  line((cols - cols/4)*gridSize, (cols - cols/4)*gridSize, (cols - cols/4)*gridSize, cols*gridSize);
  popMatrix();
  strokeWeight(1);
  
  //read file and display the agents
  //ax,ay t ax,ay t ax,ay t 100 ax,ay t ax,ay t ax,ay t 100 -bx,by bx,by bx,by -s,x,y f,x,y f,x,y s,x,ySPACE
  String[] info = lines[iterator].split(":");
  int cycle = parseInt(info[0]);
  String[] teamInfo = info[1].split(" ");
  String[] bulletInfo = info[2].split(" ");
  String[] fsInfo = info[3].split(" ");
  for(int i = 0; i < teamInfo.length; i++)
  {
      //println(teamInfo[0]);
      if(teamInfo[i].split(",").length > 1)
      {
        //agent positions
        int x = parseInt(teamInfo[i].split(",")[0]);
        int y = parseInt(teamInfo[i].split(",")[1]);
        float t = parseFloat(teamInfo[i+1]);
        int fx = parseInt(teamInfo[i+2]);
        int fy = parseInt(teamInfo[i+3]);
        i = i+3;
        displayAgent(x, y, t, fx, fy);
      }
  }
  //scores
  fill(255);
  text("Score: "+teamInfo[12], 50, 25);
  text("Resource: "+teamInfo[13], 50, 40);
  text("Score: "+teamInfo[26], 50, cols*gridSize + 80);
  text("Resource: "+teamInfo[27], 50, cols*gridSize + 95);
  text("Score: "+teamInfo[40], cols*gridSize, 25);
  text("Resource: "+teamInfo[41], cols*gridSize, 40);
  text("Score: "+teamInfo[54], cols*gridSize, cols*gridSize + 80);
  text("Resource: "+teamInfo[55], cols*gridSize, cols*gridSize + 95);
   
  //framerate and cycles
  text("Frame Rate: "+frameRate, (cols*gridSize + 100)/2 - 100, cols*gridSize + 80);
  text("Cycle: "+cycle, (cols*gridSize + 100)/2 + 50, cols*gridSize + 80);
  
  //file name
  text(file, (cols*gridSize + 100)/2, 25);
  //foods and structures
  for(int i = 0; i < fsInfo.length; i++)
  {
    String[] obj = fsInfo[i].split(",");
    if(obj[0].equals("f"))
    {
      int x = parseInt(obj[1]);
      int y = parseInt(obj[2]);
      image(img, x*gridSize + 50 + 10, y*gridSize + 50 + 10);
      //fill(0, 102, 0);
      //rect(x*gridSize + 50, y*gridSize + 50, gridSize, gridSize);
    }else if(obj[0].equals("s"))
    {
      int x = parseInt(obj[1]);
      int y = parseInt(obj[2]);
      fill(51, 51, 204);
      rect(x*gridSize + 50, y*gridSize + 50, gridSize, gridSize);
    }
  }

  //bullets
  for(int i=0; i < bulletInfo.length; i++)
  {
    String[] obj = bulletInfo[i].split(",");
    if(obj.length < 2)
      continue;
    int x = parseInt(obj[0]);
    int y = parseInt(obj[1]);
    fill(255);
    ellipse(x*gridSize + 50 + gridSize/2, y*gridSize + 50 + gridSize/2, 10, 10);
  }

  iterator++;
}

void displayAgent(int x, int y, float task, int fx, int fy)
{
  /*
      how facing direction works:
      -1-1  +0-1  +1-1
  
      -1+0  AAAA  +1+0
      
      -1+1  +0+1  +1+1
      */
    int angle;
    if(fx == -1 && fy == -1)
      angle = -45;
    else if(fx == +1 && fy == -1)
      angle = 45;
    else if(fx == 1 && fy == 0)
      angle = 90;
    else if(fx == 1 && fy == 1)
      angle = 135;
    else if(fx == 0 && fy == 1)
      angle = 180;
    else if(fx == -1 && fy == 1)
      angle = -135;
    else if(fx == -1 && fy == 0)
      angle = -90;
    else
      angle = 0;
    //stroke(255);
    // Color calculated using sine wave
    pushMatrix();
    
    //HARVESTER = 0.001;
    //ATTACKER = 0.002;
    //BUILDER = 0.003;
    
    if(task == 0.001)
      fill(102, 255, 153);
    else if(task == 0.003)
      fill(51, 204, 255);
    else if(task == 0.002)
      fill(255, 102, 102);
   
    float x1 = x*gridSize + 50 + 25;
    float y1 = y*gridSize + 50 + 15;
    float x2 = x*gridSize + 50 + 25 + 10;
    float y2 = y*gridSize + 50 + 15 + 20;
    float x3 = x*gridSize + 50 + 25 - 10;
    float y3 = y*gridSize + 50 + 15 + 20;
    
    translate(x1, y1 + (y2 - y1)/2);
    rotate(radians(angle));
    triangle(x1-x1, y1-y1 - (y2 - y1)/2, x2-x1, y2-y1 - (y2 - y1)/2, x3-x1, y3-y1 - (y2 - y1)/2);
    popMatrix();
}

void keyPressed() {
  if (key == '<')
  {
    //Slow down
    frameRate(frameRate-0.1);
  }else if (key == '>')
  {
    //Speed up
    frameRate(frameRate+0.1);
  }else if (key == 'p' || key == 'P')
  {
    //Pause
    if(looping)
    {
      noLoop();
    }else
    {
      loop();
    }
  }else if (key == ',')
  {
    //Previous file
    if(file == "0_1")
      return;
    String[] temp = file.split("_");
    if(temp[1] == "1")
    {
       int temp2 = parseInt(temp[0]) - 1;
       file = "" + temp2 + "_" + 25;
       lines = loadStrings("../Results/"+file+".txt");
       iterator = 1;
    }else
    {
       int temp2 = parseInt(temp[1]) - 1;
       file = "" + temp[0] + "_" + temp2;
       lines = loadStrings("../Results/"+file+".txt");
       iterator = 1;
    }
  }else if (key == '.')
  {
    //Next file
    String[] temp = file.split("_");
    if(temp[1] == "25")
    {
       int temp2 = parseInt(temp[0]) + 1;
       file = "" + temp2 + "_" + 1;
       lines = loadStrings("../Results/"+file+".txt");
       iterator = 1;
    }else
    {
       int temp2 = parseInt(temp[1]) + 1;
       file = "" + temp[0] + "_" + temp2;
       lines = loadStrings("../Results/"+file+".txt");
       iterator = 1;
    }
  }else if(key == '[')
      iterator--;
    else if(key == ']')
      iterator++;
    else if(key == 'r')
      iterator = 1;
  
}

// A Cell object
class Cell {
  // A cell object knows about its location in the grid 
  // as well as its size with the variables x,y,w,h
  float x,y;   // x,y location
  float w,h;   // width and height
  float angle; // angle for oscillating brightness

  // Cell Constructor
  Cell(float tempX, float tempY, float tempW, float tempH) {
    x = tempX;
    y = tempY;
    w = tempW;
    h = tempH;
  } 
  
  void display() {
    stroke(255);
    // Color calculated using sine wave
    fill(100);
    rect(x,y,w,h); 
  }
}
