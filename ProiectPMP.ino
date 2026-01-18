

//Aceste valori sunt pentru intensitatea led-urilor si pot fi schimbate in intensitati mai mici sau mai mari
#define ply1 255
#define ply2 40

const byte anode[] = {13, A5, 10, 11, 3, 9, 5, 6};
const byte cathode[] = {A2, 7, 8, A0, 12, A1, A3, A4};
const byte input[] = {0, 1, 2, 4};

typedef enum{UP = 0,DOWN = 1,LEFT = 2,RIGHT = 3,UP_LEFT = 4,DOWN_RIGHT = 5,DOWN_LEFT = 6,UP_RIGHT = 7} direction;

byte matrix[8][8];
int pos = 0;
byte turn = ply1;
byte winner = 0;

byte buttonPush[] = {0, 0, 0, 0};
byte buttonReg[] = {0, 0, 0, 0};

const byte wply1[8][8] = {
  {0,0,0,0,255,0,0,0},
  {0,0,0,255,255,0,0,0},
  {0,0,255,0,255,0,0,0},
  {0,255,0,0,255,0,0,0},
  {0,0,0,0,255,0,0,0},
  {0,0,0,0,255,0,0,0},
  {0,0,0,0,255,0,0,0},
  {0,0,0,0,255,0,0,0}
};
const byte wply2[8][8] = {
  {0,0,255,255,255,0,0,0},
  {0,255,0,0,0,255,0,0},
  {0,255,0,0,0,255,0,0},
  {0,0,0,0,255,0,0,0},
  {0,0,0,255,0,0,0,0},
  {0,0,255,0,0,0,0,0},
  {0,255,0,0,0,0,0,0},
  {0,255,255,255,255,255,0}
};

int iterative_search(direction, int, int, char);
void search_lines(int);
void place();
void render(byte **);
void specialRender();
void reset();
void moveLeft();
void moveRight();

void setup() {
  for(int i = 0; i < 8; i++)
  {
    pinMode(anode[i], OUTPUT);
    pinMode(cathode[i], OUTPUT);
    digitalWrite(anode[i], LOW);
    digitalWrite(cathode[i], LOW);
  }
  pinMode(input[0], INPUT_PULLUP);
  pinMode(input[1], INPUT_PULLUP);
  pinMode(input[2], INPUT_PULLUP);
  pinMode(input[3], INPUT_PULLUP);

  reset();
}

void loop() {
  buttonPush[0] = digitalRead(0);
  buttonPush[1] = digitalRead(1);
  buttonPush[2] = digitalRead(2);
  buttonPush[3] = digitalRead(4);
    
  if(buttonPush[0] == LOW && buttonReg[0] == HIGH)
  {
    moveLeft();
  }
  if(buttonPush[1] == LOW && buttonReg[1] == HIGH)
  {
    place();
  }
  if(buttonPush[2] == LOW && buttonReg[2] == HIGH)
  {
    moveRight();
  }
  if(buttonPush[3] == LOW && buttonReg[3] == HIGH)
  {
    reset();
  }
  if(winner)
  {
    specialRender();
    reset();
  }
  buttonReg[0] = buttonPush[0];
  buttonReg[1] = buttonPush[1];
  buttonReg[2] = buttonPush[2];
  buttonReg[3] = buttonPush[3];
  
  render(matrix);
}

void reset(){
  pos = 0;
  turn = ply1;
  winner = 0;
  for(int i = 0; i < 8; i++)
    for(int j = 0; j < 8; j++)
      matrix[i][j] = 0;
  matrix[0][pos] = ply1;
}

void moveLeft(){
  pos--;
  if(pos < 0)
    pos = 0;
  else
  {
    matrix[0][pos] = matrix[0][pos + 1];
    matrix[0][pos + 1] = 0;
  }
}

void moveRight(){
  pos++;
  if (pos > 6)
    pos = 6;
  else 
  {
    matrix[0][pos] = matrix[0][pos - 1];
    matrix[0][pos - 1] = 0;
  }
}

void place()
{
    int x = 1;
    while(x < 7 && matrix[x + 1][pos] == 0)
        x++;
    if(x != 1)
    {
        matrix[x][pos] = turn;
        search_lines(x);

        if(turn == ply1)
        {
          matrix[0][pos] = ply1;
          turn = ply2;
        }
        else
        {
          matrix[0][pos] = ply1;
          turn = ply1;
        }
    }
}

void search_lines(int x)
{
    for(int y = 0; y < 4; y++)
    {
      int I = iterative_search((direction)(y + y), x, pos, turn) + iterative_search((direction)(y + y + 1), x, pos, turn);
      if(I >= 3)
      {
        winner = turn;
        break;
      }
    }
}

int iterative_search(direction D, int x, int y, byte player)
{
    int dx = 0;
    int dy = 0;
    int connected = 0;
    switch(D)
    {
    case UP:
        dx = -1;
        break;
    case DOWN:
        dx = +1;
        break;
    case LEFT:
        dy = -1;
        break;
    case RIGHT:
        dy = +1;
        break;
    case UP_LEFT:
        dx = -1;
        dy = -1;
        break;
    case UP_RIGHT:
        dx = -1;
        dy = +1;
        break;
    case DOWN_LEFT:
        dx = +1;
        dy = -1;
        break;
    case DOWN_RIGHT:
        dx = +1;
        dy = +1;
        break;
    }
    while(x >= 2 && x <= 7 && y >= 0 && y <= 6)
    {
        x += dx;
        y += dy;
        if(matrix[x][y] == player)
            connected++;
        else
            break;
    }
    return connected;
}

void specialRender(){
  if(winner == ply1)
    for(int i = 0; i < 100; i++)
      render(wply1);
  else
    for(int i = 0; i < 100; i++)
      render(wply2);
}

void render(byte mat[8][8]){
  for(int j = 0; j < 8; j++)
  {
  for(int i = 0; i < 8; i++)
  {
    analogWrite(anode[i], mat[i][j]);
  }
    digitalWrite(cathode[j], LOW);
    delay(3);
    digitalWrite(cathode[j], HIGH);
  }
  
}

