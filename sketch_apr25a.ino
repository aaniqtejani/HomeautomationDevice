#include "painlessMesh.h"

#define   MESH_PREFIX     "Mesh"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;
const int r1 = D0;
const int r2 = D1;
const int r3 = D2;
const int r4 = D3;
const int cursen = A0;
int cur1;
int cur2;
int cur3;
int cur4;


// User stub
void sendMessage() ; // Prototype so PlatformIO doesn't complain

Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );

void sendMessage() {
  String msg = "Hi from node 4 ";
  msg += mesh.getNodeId();
  mesh.sendBroadcast( msg );
  taskSendMessage.setInterval(TASK_SECOND * 15);
}
void colab(){
    digitalWrite(r1, LOW);
    delay(2000);
    cur1=analogRead(cursen);
    digitalWrite(r1, HIGH);
    
    digitalWrite(r2, LOW);
    delay(2000);
    cur2=analogRead(cursen);
    digitalWrite(r2, HIGH);
    
    digitalWrite(r3, LOW);
    delay(2000);
    cur3=analogRead(cursen);
    digitalWrite(r3, HIGH);
    
    digitalWrite(r4, LOW);
    delay(2000);
    cur4=analogRead(cursen);
    digitalWrite(r4, HIGH);
}

// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
}

void newConnectionCallback(uint32_t nodeId) {
    Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
}

void nodeTimeAdjustedCallback(int32_t offset) {
    Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
}

void setup() {
  Serial.begin(115200);
  pinMode (r1, OUTPUT);
  pinMode (r2, OUTPUT);
  pinMode (r3, OUTPUT);
  pinMode (r4, OUTPUT);
  digitalWrite(r1, HIGH);
  digitalWrite(r2, HIGH);
  digitalWrite(r3, HIGH);
  digitalWrite(r4, HIGH);
  colab();
  Serial.printf("cur1=%d\ncur2=%d\ncur3=%d\ncur4=%d\n",cur1,cur2,cur3,cur4);
  
//mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages
  
  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  userScheduler.addTask( taskSendMessage );
  taskSendMessage.enable();
}

void loop() {
  // it will run the user scheduler as well
  mesh.update();
}
