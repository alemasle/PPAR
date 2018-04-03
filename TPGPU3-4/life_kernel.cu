
__device__ int read_cell(int * source_domain, int x, int y, int dx, int dy, unsigned int domain_x, unsigned int domain_y){
  x = (unsigned int)(x + dx + domain_x) % domain_x;
  y = (unsigned int)(y + dy + domain_y) % domain_y;
  return source_domain[y * domain_x + x];
}

__device__ void write_cell(int * source_domain, int x, int y, int dx, int dy, unsigned int domain_x, unsigned int domain_y, int val){
  x = (unsigned int)(x + dx + domain_x) % domain_x;
  y = (unsigned int)(y + dy + domain_y) % domain_y;
  source_domain[y * domain_x + x] = val;
}

// 1 cell per thread, 1 cell per word kernel
__global__ void life_kernel(int * source_domain, int * dest_domain, int domain_x, int domain_y) {

  extern __shared__ int cells[];

  int tx = blockIdx.x * (blockDim.x-2) + threadIdx.x-1;
  int ty = blockIdx.y * (blockDim.y-2) + threadIdx.y-1;

  int myself = read_cell(source_domain,tx,ty,0,0,domain_x,domain_y);

  cells[threadIdx.y*blockDim.x+threadIdx.x] = myself;

  __syncthreads();

  // Read the 8 neighbors and count number of blue and red
  int num_red = 0;
  int num_blue = 0;

  int neighbors[8];

  neighbors[0] = read_cell(source_domain, tx,ty, -1,-1, domain_x, domain_y);
  neighbors[1] = read_cell(source_domain, tx,ty, -1, 1, domain_x, domain_y);
  neighbors[2] = read_cell(source_domain, tx,ty,  1,-1, domain_x, domain_y);
  neighbors[3] = read_cell(source_domain, tx,ty,  1, 1, domain_x, domain_y);
  neighbors[4] = read_cell(source_domain, tx,ty,  0,-1, domain_x, domain_y);
  neighbors[5] = read_cell(source_domain, tx,ty,  0, 1, domain_x, domain_y);
  neighbors[6] = read_cell(source_domain, tx,ty,  1, 0, domain_x, domain_y);
  neighbors[7] = read_cell(source_domain, tx,ty, -1, 0, domain_x, domain_y);

  for(int i = 0; i < 8; i++){
    if(neighbors[i] == 1){
      num_red ++;
    }
    else if(neighbors[i] == 2){
      num_blue ++;
    }
  }

  int tot = num_red + num_blue;
  int newVal = myself;

  if(myself == 0 && tot == 3){  // If it born
    newVal = num_blue > num_red ? 2 : 1;
  }

  if(myself != 0 && (tot > 3 || tot < 2)){  // If it dies
    newVal = 0;
  }

  else if(myself != 0 && (tot <= 3 || tot >= 2)){ // He continues to live
    newVal = myself;
  }

  write_cell(source_domain, tx, ty, 0, 0, domain_x, domain_y, newVal);
}
