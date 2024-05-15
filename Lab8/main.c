#include <stdlib.h>
#include <stdio.h>
#include <png.h>
#include <math.h>
#include <pthread.h>

#ifndef M_PI
#define M_PI 3.141592653589 
#endif

double **kernelArray = NULL;
int radius = 10;
int width, height;
png_byte color_type;
png_byte bit_depth;
png_bytep *row_pointers = NULL;
png_bytep *new_row_pointers = NULL;

void read_png_file(char *filename);
void write_png_file(char *filename);
void *process_png_file();
void initKernel();
void destroyKernel();
void proccesThreads(int numThreads);
void makeBlur(int x, int y);

struct argsToProces {
  int start;
  int end;
}argsToProces; 

int main(int argc, char *argv[]) {
    if (argc>4) radius = atoi(argv[4]);
    read_png_file(argv[1]);

    initKernel();

    proccesThreads(atoi(argv[3]));

    write_png_file(argv[2]);

    destroyKernel();
    
  return 0;
}

// Malloc: KernelArray
void initKernel(){
    int sizeKernel = 1+(2*radius);
    kernelArray = malloc(sizeof(double)*sizeKernel);
    for(int i = 0; i<sizeKernel; i++){
        kernelArray[i] = malloc(sizeof(double)*sizeKernel);
    }
    
    // Create Kernel Array
    double sigma = fmax(radius / 2.0, 1.0);
    double sum = 0.0;
    for(int x = -radius;x<=radius;x++){
        for(int y = -radius; y<=radius; y++){
            double exponentNumerator = (-(x*x+y*y));
            double exponentDominator = ( 2*sigma*sigma);
            double eExpression = exp(exponentNumerator/ exponentDominator);
            double kernelValue = (eExpression/(2.0 * M_PI*sigma*sigma));
            kernelArray[x+radius][y+radius] = kernelValue;
            sum+= kernelValue;
        }
    }
    // Normalize Kernel Arr
    for (int x=0; x<sizeKernel; x++){
        for (int y=0; y<sizeKernel;y++){
            kernelArray[x][y] /= sum;
        }
    }
}

// Free: KernelArray
void destroyKernel(){
    int sizeKernel = 1+(2*radius);
    for(int i = 0; i<sizeKernel; i++){
        free(kernelArray[i]);
    }
    free(kernelArray);
}

void proccesThreads(int numThreads){
  pthread_t *threads = malloc(numThreads * sizeof(pthread_t));
  struct argsToProces *threads_args = malloc(numThreads * sizeof(argsToProces));
  int sizeForThread = height / numThreads;
  //create all threads one by one
  for (int i = 0; i < numThreads; i++) {
    printf("In main: Creating thread %d. (Radius: %d)\n", i,radius);
    threads_args[i].start = i*sizeForThread;
    threads_args[i].end = i*sizeForThread + sizeForThread;
    pthread_create(&threads[i], NULL,  process_png_file, &threads_args[i]);
  }

  printf("In main: All threads are created.\n");

  //wait for each thread to complete
  for (int i = 0; i < numThreads; i++) {
    pthread_join(threads[i], NULL);
    printf("In main: Thread %d has ended.\n", i);
  }
  free(threads);
  free(threads_args);
}

void *process_png_file(void *arguments) {
  struct argsToProces *thread_args = (struct argsToProces *)arguments;

  for(int y = thread_args->start; y < thread_args->end; y++) {
    for (int x = 0; x < width; x++) {
        makeBlur(x, y);
    }
  }
}

void makeBlur(int x, int y){
   for (int color = 0; color < 4; ++color)
    {
        double val = 0;
        for (int i = -radius; i <= radius; ++i)
        {
            for (int j = -radius; j <= radius; ++j)
            {
                if (i + x < 0 || i + x >= width)
                    continue;
                if (j + y < 0 || j + y >= height)
                    continue;
                int px_val = row_pointers[j + y][4 * (i + x) + color];
                double kernel_val = kernelArray[j + radius][i + radius];
                val += (double)px_val * kernel_val;
            }
        }
        new_row_pointers[y][4 * x + color] = (png_byte)val;
    }
}

void read_png_file(char *filename) {
  FILE *fp = fopen(filename, "rb");

  png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if(!png) abort();

  png_infop info = png_create_info_struct(png);
  if(!info) abort();

  if(setjmp(png_jmpbuf(png))) abort();

  png_init_io(png, fp);

  png_read_info(png, info);

  width      = png_get_image_width(png, info);
  height     = png_get_image_height(png, info);
  color_type = png_get_color_type(png, info);
  bit_depth  = png_get_bit_depth(png, info);

  if(bit_depth == 16)
    png_set_strip_16(png);

  if(color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_palette_to_rgb(png);

  // PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
  if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
    png_set_expand_gray_1_2_4_to_8(png);

  if(png_get_valid(png, info, PNG_INFO_tRNS))
    png_set_tRNS_to_alpha(png);

  // These color_type don't have an alpha channel then fill it with 0xff.
  if(color_type == PNG_COLOR_TYPE_RGB ||
     color_type == PNG_COLOR_TYPE_GRAY ||
     color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

  if(color_type == PNG_COLOR_TYPE_GRAY ||
     color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
    png_set_gray_to_rgb(png);

  png_read_update_info(png, info);

  if (row_pointers) abort();

  row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
  for(int y = 0; y < height; y++) {
    row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png,info));
  }
  new_row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
  for(int y = 0; y < height; y++) {
    new_row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png,info));
  }

  png_read_image(png, row_pointers);

  fclose(fp);

  png_destroy_read_struct(&png, &info, NULL);
}

void write_png_file(char *filename) {

  FILE *fp = fopen(filename, "wb");
  if(!fp) abort();

  png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png) abort();

  png_infop info = png_create_info_struct(png);
  if (!info) abort();

  if (setjmp(png_jmpbuf(png))) abort();

  png_init_io(png, fp);

  // Output is 8bit depth, RGBA format.
  png_set_IHDR(
    png,
    info,
    width, height,
    8,
    PNG_COLOR_TYPE_RGBA,
    PNG_INTERLACE_NONE,
    PNG_COMPRESSION_TYPE_DEFAULT,
    PNG_FILTER_TYPE_DEFAULT
  );
  png_write_info(png, info);

  // To remove the alpha channel for PNG_COLOR_TYPE_RGB format,
  // Use png_set_filler().
  //png_set_filler(png, 0, PNG_FILLER_AFTER);

  if (!new_row_pointers) abort();

  png_write_image(png, new_row_pointers);
  png_write_end(png, NULL);

  for(int y = 0; y < height; y++) {
    free(row_pointers[y]);
  }
  free(row_pointers);

  for(int y = 0; y < height; y++) {
    free(new_row_pointers[y]);
  }
  free(new_row_pointers);

  fclose(fp);

  png_destroy_write_struct(&png, &info);
}