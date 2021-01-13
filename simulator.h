#define CITY_WIDTH            800   // The width and height of the city window
#define CITY_HEIGHT           600

#define VEHICLE_RADIUS          5   // The radius of a vehicle (shown as a circle)
#define MAX_CONNECTIONS        10   // Maximum # of vehicles allowed to connect to a cell tower
#define VEHICLE_SPEED           3   // Forward pixels per movement
#define VEHICLE_TURN_ANGLE      5   // degrees to turn for each vehicle movement

#define NUM_TOWERS              7   // Number of cell towers within the city borders

#define SERVER_IP     "127.0.0.1"   // IP address of simulator server
#define SERVER_PORT          6000   // PORT of the simulator server

// Command codes sent from client to server
#define SHUTDOWN           1
#define CONNECT            2
#define UPDATE             3

// Command codes sent from server to client
#define YES                5
#define NO                 6
#define NOT_OK_BOUNDARY    7
#define NOT_OK_COLLIDE     8


typedef struct {
  int   x;
  int   y;
  int   direction;
  char  towerID;
} Vehicle;

typedef struct {
  int   x;
  int   y;
  char  connected;
} ConnectedVehicle;


typedef struct {
  char               online; // 0 = no, 1 = yes
  char               id;
  short              x;
  short              y;
  short              radius;
  int                color;
  ConnectedVehicle   connectedVehicles[MAX_CONNECTIONS];
  short              numConnectedVehicles;
} CellTower;

typedef struct {
  CellTower towers[NUM_TOWERS];
  short     numTowers;
} City;