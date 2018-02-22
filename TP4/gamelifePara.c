
/*
 * Conway's Game of Life
 *
 * A. Mucherino
 *
 * PPAR, TP4
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <mpi.h>

int N = 64;
int itMax = 100;

// allocation only
unsigned int* allocate()
{
   return (unsigned int*)calloc(N*N,sizeof(unsigned int));
};

// conversion cell location : 2d --> 1d
// (row by row)
int code(int x,int y,int dx,int dy)
{
   int i = (x + dx)%N;
   int j = (y + dy)%N;
   if (i < 0)  i = N + i;
   if (j < 0)  j = N + j;
   return i*N + j;
};

// writing into a cell location
void write_cell(int x,int y,unsigned int value,unsigned int *world)
{
   int k;
   k = code(x,y,0,0);
   world[k] = value;
};

// random generation
unsigned int* initialize_random()
{
   int x,y;
   unsigned int cell;
   unsigned int *world;

   world = allocate();
   for (x = 0; x < N; x++)
   {
      for (y = 0; y < N; y++)
      {
         if (rand()%5 != 0)
         {
            cell = 0;
         }
         else if (rand()%2 == 0)
         {
            cell = 1;
         }
         else
         {
            cell = 2;
         };
         write_cell(x,y,cell,world);
      };
   };
   return world;
};

// dummy generation
unsigned int* initialize_dummy()
{
   int x,y;
   unsigned int *world;

   world = allocate();
   for (x = 0; x < N; x++)
   {
      for (y = 0; y < N; y++)
      {
         write_cell(x,y,x%3,world);
      };
   };
   return world;
};

// "glider" generation
unsigned int* initialize_glider()
{
   int x,y,mx,my;
   unsigned int *world;

   world = allocate();
   for (x = 0; x < N; x++)
   {
      for (y = 0; y < N; y++)
      {
         write_cell(x,y,0,world);
      };
   };

   mx = N/2 - 1;  my = N/2 - 1;
   x = mx;      y = my + 1;  write_cell(x,y,1,world);
   x = mx + 1;  y = my + 2;  write_cell(x,y,1,world);
   x = mx + 2;  y = my;      write_cell(x,y,1,world);
                y = my + 1;  write_cell(x,y,1,world);
                y = my + 2;  write_cell(x,y,1,world);

   return world;
};

// "small exploder" generation
unsigned int* initialize_small_exploder()
{
   int x,y,mx,my;
   unsigned int *world;

   world = allocate();
   for (x = 0; x < N; x++)
   {
      for (y = 0; y < N; y++)
      {
         write_cell(x,y,0,world);
      };
   };

   mx = N/2 - 2;  my = N/2 - 2;
   x = mx;      y = my + 1;  write_cell(x,y,2,world);
   x = mx + 1;  y = my;      write_cell(x,y,2,world);
                y = my + 1;  write_cell(x,y,2,world);
                y = my + 2;  write_cell(x,y,2,world);
   x = mx + 2;  y = my;      write_cell(x,y,2,world);
                y = my + 2;  write_cell(x,y,2,world);
   x = mx + 3;  y = my + 1;  write_cell(x,y,2,world);

   return world;
};


// reading a cell
int read_cell(int x,int y,int dx,int dy,unsigned int *world)
{
   int k = code(x,y,dx,dy);
   return world[k];
};

// updating counters
void update(int x,int y,int dx,int dy,unsigned int *world,int *nn,int *n1,int *n2)
{
   unsigned int cell = read_cell(x,y,dx,dy,world);
   if (cell != 0)
   {
      (*nn)++;
      if (cell == 1)
      {
         (*n1)++;
      }
      else
      {
         (*n2)++;
      };
   };
};

// looking around the cell
void neighbors(int x,int y,unsigned int *world,int *nn,int *n1,int *n2)
{
   int dx,dy;

   (*nn) = 0;  (*n1) = 0;  (*n2) = 0;

   // same line
   dx = -1;  dy = 0;   update(x,y,dx,dy,world,nn,n1,n2);
   dx = +1;  dy = 0;   update(x,y,dx,dy,world,nn,n1,n2);

   // one line down
   dx = -1;  dy = +1;  update(x,y,dx,dy,world,nn,n1,n2);
   dx =  0;  dy = +1;  update(x,y,dx,dy,world,nn,n1,n2);
   dx = +1;  dy = +1;  update(x,y,dx,dy,world,nn,n1,n2);

   // one line up
   dx = -1;  dy = -1;  update(x,y,dx,dy,world,nn,n1,n2);
   dx =  0;  dy = -1;  update(x,y,dx,dy,world,nn,n1,n2);
   dx = +1;  dy = -1;  update(x,y,dx,dy,world,nn,n1,n2);
};

// computing a new generation
short newgeneration(unsigned int *world1,unsigned int *world2,int xstart,int xend)
{
   int x,y;
   int nn,n1,n2;
   int k, pop;
   unsigned int cell;
   short change = 0;

   // cleaning destination world
   for (x = 0; x < N; x++)
   {
      for (y = 0; y < N; y++)
      {
         write_cell(x,y,0,world2);
      };
   };

   // generating the new world
   for (x = xstart; x < xend; x++)
   {
      for (y = 0; y < N; y++)
      {

        neighbors(x,y,world1,&nn,&n1,&n2);
        k = read_cell(x,y,0,0, world1);

        if(k == 0 && nn == 3){ // Si il doit naitre
          pop = n1 > n2 ? 1 : 2;
          write_cell(x,y,pop, world2);
          change = 1;
        }

        if(k != 0 && (nn > 3 || nn < 2) ){ // Si il doit mourir
          write_cell(x,y,0,world2);
          change = 1;
        }

        else if(k != 0 && (nn <= 3 || nn >= 2) ){ // Si il vit, il continue a vivre
          write_cell(x,y,k,world2);
        }

      };
   };
   return change;
};

// cleaning the screen
void cls()
{
    int i;
    for (i = 0; i < 10; i++)
    {
        fprintf(stdout,"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    }
}


// diplaying the world
void print(unsigned int *world)
{
   int i;
   cls();
   for (i = 0; i < N; i++)  fprintf(stdout,"-");

   for (i = 0; i < N*N; i++)
   {
      if (i%N == 0)  fprintf(stdout,"\n");
      if (world[i] == 0)  fprintf(stdout," ");
      if (world[i] == 1)  fprintf(stdout,"o");
      if (world[i] == 2)  fprintf(stdout,"x");
   };
   fprintf(stdout,"\n");

   for (i = 0; i < N; i++)  fprintf(stdout,"-");
   fprintf(stdout,"\n");
   sleep(1);
};

// main
int main(int argc,char *argv[])
{
   int it,change, my_rank, NUMBER_PROC, tailleRegion, firstRow, lastRow;
   unsigned int *world1,*world2;
   unsigned int *fstRow, *lstRow;
   unsigned int *worldaux;

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
   MPI_Comm_size(MPI_COMM_WORLD, &NUMBER_PROC);

   tailleRegion = N*N/NUMBER_PROC;

   if(N*N % NUMBER_PROC != 0){
     printf("Execution abort\n");
     exit(-1);
   }

   if (my_rank == 0) {
     // getting started
     //world1 = initialize_dummy();
     //world1 = initialize_random();
     //world1 = initialize_glider();
     world1 = initialize_small_exploder();
   }

   world2 = allocate();

   MPI_Bcast(world1, N*N, MPI_UNSIGNED, 0, MPI_COMM_WORLD);
   if(my_rank == 0){
     print(world1);
   }

   firstRow = my_rank * tailleRegion;
   lastRow = (my_rank * tailleRegion) + (tailleRegion - 1);

   for(int i = 0; i < ; i++){

   }


   it = 0;  change = 1;
   while (change && it < itMax)
   {
      MPI_send(&world2[code(firstRow,0,0,0)], N, MPI_UNSIGNED, my_rank - 1, 0, MPI_COMM_WORLD);
      MPI_send(&world2[code(lastRow,0,0,0)], N, MPI_UNSIGNED, my_rank + 1, 0, MPI_COMM_WORLD);
      MPI_recv(&world2[code(lastRow + N,0,0,0)], N, MPI_UNSIGNED, my_rank + 1, 0, MPI_COMM_WORLD);
      MPI_recv(&world2[code(firstRow - N,0,0,0)], N, MPI_UNSIGNED, my_rank - 1, 0, MPI_COMM_WORLD);

      change = newgeneration(world1, world2, firstRow, lastRow);
      worldaux = world1;  world1 = world2;  world2 = worldaux;


      print(world1);
      it++;
   };

   // ending
   MPI_Finalize();
   free(world1);   free(world2);
   return 0;
};
