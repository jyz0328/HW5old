#include <iostream>
#include <fstream>
#include <chrono>
#include <cstring>
using namespace std;
//cd /Users/zhangjingyi (i use this code for sepcial case you can ignore it)
//g++ -o connect connect.cpp (use this code for activate connect.cpp)
//./connect graph_8_14.txt > result_8_14.txt (after activate, use this code to create result)
/***********************************************************
Given two arrays: FirstVertex pointing to the EdgeList with destination 
node for an undirected graph G(V,E). 

The input file begins with number of vertices (V = |V|)
and number of edges (E =|E|) and the list of value for 

int FirstVertex[V+1]
int EdgeList[E+1]

The vertices(nodes) are numbered 0,..,|V| -1.  The last entry 
FirstVertex[V] = E pointing to  EdgeList[E] = -1;
represented as a null

You are to compute the number of the connected components.

************************************************************/

// Here is remendary Queue give at end of file.
// Definition of the Queue structure
// 定义队列结构
typedef struct Queue
{
        int capacity;   // Maximum number of elements the queue can hold
        int size;       // Current number of elements in the queue
        int front;      // Index of the front element
        int rear;       // Index of the rear element
        int *elements;  // Array to store elements of the queue
}Queue;

Queue * createQueue(int maxElements);
void Enqueue(Queue *Q,int element);
int Dequeue(Queue *Q);
int find_connected_components_BFS(int *FirstVertex, int V, int *EdgeList,int E);
void PrintAdjacencyListFormat(int *FirstVertex, int V, int *EdgeList,int E);
// void BFS(Queue * Q,, int *Found, int *FirstVertex, int V, int *EdgeList,int E);

//*change part i :change code start from here,  create a stack sturture first*/改变从这里开始
typedef struct Stack
{
        int capacity;
        int size;
        int front;
        int rear;
        int *elements;
}Stack;

Stack * createStack(int maxElements);//create a new Stack with a given maximum number of elements
void Push(Stack *S, int element);//add an element to the top of the stack
int Pop(Stack *S);//remove and return an element from the top of the stack
int find_connected_components_DFS(int *FirstVertex, int V, int *EdgeList,int E);//Find the number of connected components using DFS
//**here is end of my change part i改变到这里结束

int main(int argc, char *argv[]){

  chrono::time_point<chrono::steady_clock> start, stop; 
  chrono::duration<double> difference_in_time;
  double difference_in_seconds_BFS; // Holds the final run time for BFS
  double difference_in_seconds_DFS; // Holds the final run time for DFS这句是新加的
  
  ifstream infile1;
  int V;
  int E;
  int NumberCC_BFS;
  int NumberCC_DFS;
  
  infile1.open(argv[1]);
  if(!infile1){
    cout << "Error opening file " <<endl;
    return -1;
  } 
  
     infile1 >> V;
     infile1 >> E;
    
    int *FirstVertex = new int[V+1];
    for(int i=0; i< V+1 ; i++) 
      infile1 >> FirstVertex[i];  //  Note: FirstVertex[V] = E  fake extra link
    
    int *EdgeList = new int[E+1];
    for(int i=0; i< E +1 ; i++)
      infile1 >> EdgeList[i];   // Note  EdgeList[E] = -1;  is null flag
    
    infile1.close();

 #if 1 // set to 1 to debug 
     cout << endl << "A print in Adjacency  List form to help with Debugging! " << endl;
     PrintAdjacencyListFormat(FirstVertex, V, EdgeList, E);
#endif
    
    /* Find NumberCC_BFS   */
    start = chrono::steady_clock::now(); // Measure the start time for DFS
    NumberCC_BFS=find_connected_components_BFS(FirstVertex, V, EdgeList,E);  // Find the number of connected components using DFS
    stop = chrono::steady_clock::now();
    difference_in_time = stop - start;
    difference_in_seconds_BFS = double(difference_in_time.count());

    //* i set change here as change part ii*改变从这里开始/
    start = chrono::steady_clock::now(); // Measure the start time for DFS
    NumberCC_DFS=find_connected_components_DFS(FirstVertex, V, EdgeList,E);// Find the number of connected components using DFS
    stop = chrono::steady_clock::now(); // Measure the stop time for DFS
    difference_in_time = stop - start;  // Calculate the time taken for DFS in seconds
    difference_in_seconds_DFS = double(difference_in_time.count());
    /*改变到这里结束part ii change stop here*/

    //Begin output file : DO NOT CHANGE
    ofstream outfile(strcat(argv[1],"_out"));
    outfile << difference_in_seconds_BFS << endl;
    outfile << NumberCC_BFS << endl;
    outfile << difference_in_seconds_DFS << endl;  
    outfile << NumberCC_DFS << endl;
    //End output file
     
    return 0;
}

//Function to find the number of connected components using Breadth-First Search (BFS)
int find_connected_components_BFS(int *FirstVertex, int V, int *EdgeList,int E)
{  
    int NumberCC_BFS  = 0;// Variable to store the number of connected components
    int *Found = new int[V];// Array to mark visited vertices
    fill_n(Found, V, -1);//Initialize Found array with -1 (unvisited)
    Queue *Q = createQueue(V);//// Create a new queue for BFS traversal

     // Write code to interate over BFS to find all connected compoents.
     //part iii :Starting from vertex 0, Set Found to 1 when enqueue改变从这里开始
     Enqueue(Q,0);
     Found[0] = 1;
     //// BFS traversal loop: Continue until the queue is empty
     while (Q->size > 0){
        int current_Node = Dequeue(Q); //remove the current node from the queue 
        int start = FirstVertex[current_Node];
        int end = FirstVertex[current_Node+1];

        // Iterate over the neighbors of the current node
        for (int i = start; i<end; i++){
                // Check if the neighbor is not visited
                if (Found[EdgeList[i]] == -1){
                        Enqueue(Q,EdgeList[i]);// Enqueue unvisited neighbor
                        Found[EdgeList[i]] = 1;// Mark the neighbor as visited
                }
        }
        // Check if the queue is empty
        if (Q->size == 0){ 
                NumberCC_BFS ++;// Increment the count of connected components
                // Check if exist unexplored nodes
                for (int j = 0; j<V; j++){
                        if (Found[j] == -1){
                                // if exist, add to queue as a new starting node
                                Enqueue(Q,j); 
                                Found[j] = 1;// Mark the new starting node as visited
                                break;
                        }
                }
        }
     }
    // BFS(Q,Found, FirstVertex,  V, EdgeList,int E);
    //cout << endl << NumberCC_BFS << endl; // Self Debugging改变到这里结束
    //part iii change end here
    return NumberCC_BFS;
}
//part iv here :Function to find the number of connected components using DFS function改变从这里开始
int find_connected_components_DFS(int *FirstVertex, int V, int *EdgeList,int E){
        int NumberCC_DFS  = 0;
        int *Found = new int[V];
        fill_n(Found, V, -1);
        Stack *S = createStack(V);

        //Starting from vertex 0, Set Found to 1 when Push
        Push(S,0); 
        Found[0] = 1;

        while (S->size > 0){
                int current_Node = Pop(S);
                int start = FirstVertex[current_Node];
                int end = FirstVertex[current_Node+1];
                // Iterate over the neighbors of the current node
                for (int i = start; i<end; i++){
                        // Check if the neighbor is not visited
                        if (Found[EdgeList[i]] == -1){
                                Push(S,EdgeList[i]); // Enqueue unvisited neighbor
                                Found[EdgeList[i]] = 1; // Mark the neighbor as visited
                        }
                }
        
                if (S->size == 0){ 
                        NumberCC_DFS ++;
                        // Check if exist unexplored nodes
                        for (int j = 0; j<V; j++){
                                if (Found[j] == -1){
                                        // if exist, add to queue as a new starting node
                                        Push(S,j);
                                        Found[j] = 1;
                                        break;
                                }
                        }
                }
        }

        

        return NumberCC_DFS;
}
/* STACK FUNCTIONS */
// Function to create a new Stack with a given maximum number of elements
// 创建一个具有给定最大元素数量的新堆栈
Stack * createStack(int maxElements)
{
        Stack *S;
        S = (Stack*)malloc(sizeof(Stack));// Allocate memory for the Stack structure
        S->elements = (int *)malloc(sizeof(int)*maxElements); // Allocate memory for the stack elements
        S->size = 0;// Initialize the size of the stack to 0
        S->capacity = maxElements;// Set the maximum capacity of the stack
        S->front = -1; // Initialize the front index to -1
        S->rear = 0;// Initialize the rear index to 0
        return S;// Return the created Stack
}
// Function to push an element onto the stack
// 将元素推入堆栈的函数
void Push(Stack *S, int element)
{
        if(S->size == S->capacity){
                printf("Stack is Full\n");// Print an error message if the stack is full
        }
        else{
                S->size ++;// Increment the size of the stack
                S->front = S->front + 1;// Move the front index to the next position
                S->elements[S->front] = element;// Add the element to the stack
        }
        return;
}
// Function to pop an element from the stack and return it
// 从堆栈中弹出元素并返回的函数
int Pop(Stack *S){
        int element = -1;// Initialize the element to be returned to -1 (default if the stack is empty)
        if (S->size == 0){
                //printf("Stack is Empty\n");
                return element;
        }else{
                element = S->elements[S->front];// Retrieve the top element from the stack
                S->size --;// Decrement the size of the stack
                S->front --;// Move the front index to the previous position
        }
        return element;
}


/* QUEUE FUCTIONS *change of part 4 stop here改变到这里结束*/


Queue * createQueue(int maxElements)
{
        /* Create a Queue */
        Queue *Q;
        Q = (Queue*)malloc(sizeof(Queue));
        /* Initialise its properties */
        Q->elements = (int *)malloc(sizeof(int)*maxElements);
        Q->size = 0;
        Q->capacity = maxElements;
        Q->front = 0;
        Q->rear = -1;
        /* Return the pointer */
        return Q;
}

void Enqueue(Queue *Q,int element)
{
        /* If the Queue is full, we cannot push an element into it as there is no space for it.*/
        if(Q->size == Q->capacity)
        {
                printf("Queue is Full\n");
        }
        else
        {
                Q->size++;
                Q->rear = Q->rear + 1;
                /* As we fill the queue in circular fashion */
                if(Q->rear == Q->capacity)
                {
                        Q->rear = 0;
                }
                /* Insert the element in its rear side */ 
                Q->elements[Q->rear] = element;
        }
        return;
}

int  Dequeue(Queue *Q)
{
         int  element = -1;
        /* If Queue size is zero then it is empty. So we cannot pop */
        if(Q->size==0)
        {
	  //              printf("Queue is Empty\n");
                return element;
        }
        /* Removing an element is equivalent to incrementing index of front by one */
        else
        {       element = Q->elements[Q->front];
                Q->size--;
                Q->front++;
                /* As we fill elements in circular fashion */
                if(Q->front==Q->capacity)
                {
                        Q->front=0;
                }
        }
        return element;
}

void PrintAdjacencyListFormat(int *FirstVertex, int V, int *EdgeList,int E)
  {
    int v;
    int e;
    cout << "V = " << V << "  E = " <<E <<endl;
    for( v = 0; v < V; v++)
      {
	cout << endl;
	cout << v  << ": ";
	
	for(e = FirstVertex[v]; e < FirstVertex[v+1]; e++) 
	  cout << EdgeList[e]<< "-> ";

	cout <<"nil";
      }

    cout << endl;
    cout << v  << " : ";
    cout << EdgeList[e]  << " That's Edge List  nil  Folks " << endl;
 
  }
