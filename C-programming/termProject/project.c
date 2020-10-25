#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
typedef enum{FALSE, TRUE}bool;
typedef struct{
  double x;
  double y;
  int num_of_points;
}Point2D;
typedef struct{
  double R,G,B;
}Color;
typedef struct{
  double width;
  double height;
  char* fill;
}canvas_svg;/*just a regular rectangle with width and height of svg file*/

typedef struct Figure{
    Point2D* points;
    int* num_of_points;/*same as for colors*/
    int num_of_shapes;/*when appended*/
    Color* color;/*array since it might be a number of shapes with different colors when appened*/
    //double width;
    //double height;
    double* thickness;/*same as for colors*/
    double resolution;
    canvas_svg canvas;/*canvas will be inherited to the figure another figure is appended to if appended at all*/
}Figure;

Figure* start_figure(double width,double height);
void set_thickness_resolution(Figure* fig, double thickness, double resolution);
void set_color(Figure * fig, Color c);
void export_svg(Figure * fig, char * file_name);
void export_eps(Figure * fig, char * file_name);
void draw_fx(Figure * fig, double f(double x), double start_x, double end_x);
double square(double x);
void draw_polyline(Figure * fig, Point2D * poly_line, int n);
void draw_circle(Figure * fig, Point2D * center, double r);
void draw_ellipse(Figure* fig, Point2D center, int a, int b);/*a represents the major and b represents the minor axis*/
void scale_figure(Figure* fig, double scale_x, double scale_y);
void resize_figure(Figure* fig, Point2D start_roi, Point2D end_roi);
void append_figures(Figure * fig1, Figure * fig2);
void import_svg(char * file_name, Figure * fig);
void calling( Figure* fig);
void remove_point(Point2D* points, int start_i, int end_i, int nump);
int main()
{
      Figure* fig;
      fig = start_figure(640,480);
      // import_svg("sd", fig);
      Figure* circle;
      circle = start_figure(640,480);
      Figure* fig_polyline;
      fig_polyline = start_figure(640,480);
      Figure* ellipse;
      ellipse = start_figure(640,480);
      Point2D ellipse_center = {0,-150};
      double a = 50;
      double b = 40;
      Point2D poly_line[4] = {{0,100},{50,25},{50,75},{100,0}};
      Point2D center = {0,0};
      char file_name_svg[20];
      char file_name_eps[20];
      Color c = {127.5, 255, 127.5};
      set_color(fig_polyline, c);/*putting the collor of our figure*/
      set_color(circle, c);/*putting the collor of our figure*/
      set_color(ellipse, c);
      printf("Enter name of your file\n");
      scanf("%s", file_name_svg);
      strcpy(file_name_eps, file_name_svg);
      set_thickness_resolution(fig_polyline, 2, 0.1);/*num for resolution field smaller -> higher resolution and inverse holds*/
      set_thickness_resolution(circle, 2, 0.1);/*num for resolution field smaller -> higher resolution and inverse holds*/
      set_thickness_resolution(fig, 2, 0.1);
      set_thickness_resolution(ellipse, 2, 10);
      draw_polyline(fig_polyline, poly_line, 4);
      draw_circle(circle, &center, 250);
  //    draw_fx(fig, square, -100, 100);
      draw_ellipse(ellipse, ellipse_center, a, b);
      Point2D start_roi = {100,0}, end_roi={200,-200};
      //resize_figure(ellipse, start_roi, end_roi);
      printf("oout of polyline");getchar();
      scale_figure(fig, 0.95, 0.95);
    //  append_figures(circle, fig_polyline);
      export_svg(circle, file_name_svg);
      printf("out of svg");getchar();
      export_eps(circle, file_name_eps);
  return 0;
}
void calling(Figure* fig)
{
  int i,j;
  int start_i, end_i;
  double unknown;
  int shape_points = 0;
  for(i=0,j=0;i<fig->num_of_points[j]; i++)
  {
    shape_points++;
    printf("X:%lf Y:%lf %lf \n", fig->points[i].x, fig->points[i].y, fig->canvas.height);
    if(fig->points[i].x>fig->canvas.width/2 || fig->points[i].x<-(fig->canvas.width/2) || fig->points[i].y>fig->canvas.height/2 || fig->points[i].y<-(fig->canvas.height/2))
    {printf("IN\n");
      start_i = i;
      end_i = i;
      while(end_i<fig->num_of_points[j] && (fig->points[end_i].x>fig->canvas.width/2 || fig->points[end_i].x<-(fig->canvas.width/2) || fig->points[end_i].y>fig->canvas.height/2 || fig->points[end_i].y< -fig->canvas.height/2) )
      {/*removing all points that are out*/
        //IN REMOVE POINT(from end_i to size-free them all, no zombie)free(&fig->points[i]);/*or just +i for an address instead of [i]*/
        //no need there is shape_points here that is counting IN REMOVE POINT --(fig->num_of_points[j]);/*don't change here but later on in remove_point function fig->num_of_points-=(end_i-start_i) and after realloc it with the num of points present in it now*/
        //i++; DONT USE i because AFTER REMOVING POINT I NEED TO CONTINUE FROM WHERE I HAVE LEFT OF
        end_i++;
      }
      printf("START_I: %d END_I:%d\n", start_i, end_i);getchar();
      //if it goes back in in pieces
      if(start_i!=0 && end_i!=fig->num_of_points[j])
      {printf("Making new shape");
        remove_point(fig->points, start_i, end_i, fig->num_of_points[j]);
        fig->points = (Point2D*)realloc(fig->points, (fig->num_of_points[j]-(end_i-start_i))* sizeof(Point2D));/*freeing points that are not necessary*/
        (fig->num_of_shapes)++;
        fig->num_of_points = (int*)realloc(fig->num_of_points, fig->num_of_shapes*sizeof(int));/*allocating space for the points of the next shape*/
        fig->num_of_points[j+1] = fig->num_of_points[j]-shape_points;/*fing->num_of_points holds total number of points*/
        fig->num_of_points[j] = shape_points;
        fig->points = (Point2D*)realloc(fig->points, fig->num_of_points[j+1] * sizeof(Point2D));/*freeing points that are not necessary*/
        fig->color = (Color*)realloc(fig->color, fig->num_of_shapes*sizeof(Color));
        fig->color[j+1] = fig->color[j];
  //UPDATE NUM OF SHAPES WHEN MAKING THIS SHIT!
        fig->thickness = (double*)realloc(fig->thickness, fig->num_of_shapes*sizeof(double));
        fig->thickness[j+1] = fig->thickness[j];
        j++;
        shape_points=0;/*reset for the next shape*/
        printf("OLD NP:%d NEW NP:%d\n", fig->num_of_points[j-1], fig->num_of_points[j]);
        i=0;getchar();
      }
      else/*just removing unecessary chuncks*/
      {
        if(fig->points[end_i-1].x<-fig->canvas.width/2 || fig->points[end_i-1].x>fig->canvas.width)
        {
          if(fig->points[end_i-1].x<-fig->canvas.width/2)
          {
            unknown = ((fig->points[end_i].y - fig->points[end_i-1].y)*( (fig->canvas.width/2)-fig->points[end_i-1].x))/(fig->points[end_i].x-fig->points[end_i-1].x);
            fig->points[end_i-1].y = unknown;
            fig->points[end_i-1].x = -fig->canvas.width/2;
          }
          else if(fig->points[end_i-1].x>fig->canvas.width/2)
          {
            fig->points[end_i-1].x = fig->canvas.width/2;
          }
          end_i--;/*i want to keep this point sitting on the boundary*/
        }
        remove_point(fig->points, start_i, end_i, fig->num_of_points[j]);
        fig->points = (Point2D*)realloc(fig->points, (fig->num_of_points[j]-(end_i-start_i))* sizeof(Point2D));/*freeing points that are not necessary*/
        fig->num_of_points[j] -= (end_i-start_i);
        shape_points=0;
      }
    }
  }
}
void remove_point(Point2D* points, int start_i, int end_i, int nump)/*i - beginning of part of the array to be removed and j the end of part of the array to be removed*/
{
  int i;
  for(i=start_i; end_i+i<nump; i++) points[i] = points[end_i+i];/*making duplicates so that I can later on shrink the size of this array*/
  for(i=nump-(start_i-end_i); i<nump; i++) free(points+i);
  // int num_of_changes;
  // for(j=(size-1)-(j-i); j<size ; j++, i++)/*size-1 representing the index of the last point*/
  // {
  //   points[i] = points[j];
  // }
}
void append_figures(Figure * fig1, Figure * fig2)
{/*"resulting canvas should include both figures without any cropping"-i suppose it means that it should just update the canvas to be the one that is bigger*/
  int i,j;
  int total_points = fig1->num_of_points[0];
  int total_shapes = fig1->num_of_shapes + fig2->num_of_shapes;
  for(i=0; i<fig2->num_of_shapes; i++) total_points += fig2->num_of_points[i];

  if(fig1->canvas.width < fig2->canvas.width) fig1->canvas.width = fig2->canvas.width;
  if(fig1->canvas.height < fig2->canvas.height) fig1->canvas.height = fig2->canvas.height;
  /*setting up the memory*/
  fig1->points = (Point2D*)realloc(fig1->points, total_points*sizeof(Point2D));
  fig1->num_of_points = (int*)realloc(fig1->num_of_points, total_shapes*sizeof(int));
  fig1->color = (Color*)realloc(fig1->color, total_shapes*sizeof(Color));
  fig1->thickness = (double*)realloc(fig1->thickness, total_shapes*sizeof(double));
  /*Appending to fig1*/
  /*points*/
  for(i=fig1->num_of_points[0],j=0; i<total_points; i++,j++)
  {
    fig1->points[i] = fig2->points[j];
  }
  /*num_of_points*/
  for(i=fig1->num_of_shapes,j=0; i<total_shapes; i++, j++)
  {
    fig1->num_of_points[i] = fig2->num_of_points[j];
  }
  /*colors*/
  for(i=fig1->num_of_shapes,j=0; i<total_shapes; i++, j++)
  {
    fig1->color[i] = fig2->color[j];
  }
  /*thickness*/
  for(i=fig1->num_of_shapes,j=0; i<total_shapes; i++, j++)
  {
    fig1->thickness[i] = fig2->thickness[j];
  }
  /*Finished appending*/
  fig1->num_of_shapes = total_shapes;/*updating num of shapes in a figure*/
}
void resize_figure(Figure* fig, Point2D start_roi, Point2D end_roi)
{
  int i;
  double new_x, new_y;
  int num_of_resized_points;
  fig->canvas.width = abs(end_roi.x - start_roi.x);
  fig->canvas.height = abs(end_roi.y - start_roi.y);
  Point2D new_center;
  new_center.x = (end_roi.x + start_roi.x)/2;
  new_center.y = (end_roi.y + start_roi.y)/2;
  for(i=0, num_of_resized_points=0; i<fig->num_of_points[0];i++)
  {
    // if(fig->points[i].x >= start_roi.x && fig->points[i].x <= end_roi && fig->points[i].y>=start_roi.y && fig->points[i].y<=end_roi.y)
    // {
      fig->points[num_of_resized_points].x -= new_center.x;
      fig->points[num_of_resized_points].y -= new_center.y;
      num_of_resized_points++;
    // }
  }
  // int i,j;
  // for(i=0,j++; i<fig->num_of_points[0]; i++)
  // {
  //   if( fig->points[i].x<start_roi.x || fig->points[i].x>end_roi.x || fig->points[i].y<start_roi.y || fig->points[i].y>end_roi.y)/*when just one of the indicies of the point, be it x be it y, is not within the newly provided rectange the whole point should be removed from points array*/
  //   {
  //     j=i;
  //     while( (fig->points[j].x<start_roi.x || fig->points[j].x>end_roi.x || fig->points[j].y<start_roi.y || fig->points[j].y>end_roi.y) && j<fig->num_of_points[0])
  //     {
  //       j++;
  //     }
  //     update(fig->points, fig->num_of_points[0], i, j);
  //     fig->num_of_points[0] -= (j-i);
  //     fig->point = (Point2D*)realloc(fig->points,(fig->num_of_points[0])*(sizeof(Point2D)) );
  //   }
  // }
}
void scale_figure(Figure* fig, double scale_x, double scale_y)
{
  int i;
  Point2D center = {0,0};
  Point2D new_center = {0,0};
  double dist_x, dist_y;
  for(i=0; i<fig->num_of_points[0]; i++)
  {
    center.x += fig->points[i].x;
    center.y += fig->points[i].y;
    fig->points[i].x *= scale_x;
    fig->points[i].y *= scale_y;
  }
  center.x /= fig->num_of_points[0];
  center.y /= fig->num_of_points[0];
  for(i=0; i<fig->num_of_points[0]; i++)
  {
    new_center.x += fig->points[i].x;
    new_center.y += fig->points[i].y;
  }
  new_center.x /= fig->num_of_points[0];
  new_center.y /= fig->num_of_points[0];
  for(i=0; i<fig->num_of_points[0]; i++)
  {
    dist_x = fig->points[i].x - new_center.x;
    dist_y = fig->points[i].y - new_center.y;
    fig->points[i].x = center.x + dist_x;
    fig->points[i].y = center.y + dist_y;
  }
}
void draw_ellipse(Figure* fig, Point2D center, int a, int b)/*a represents the major and b represents the minor axis*/
{
  double i;
  double a2 = a*a;
  double b2 = b*b;
  double precision = 0.0001;
  Point2D start = {center.x-a+precision, sqrt( b2-b2*(pow(center.x-a+precision-center.x,2)/a2) )+center.y};/*could just put a in there but the whole expression put to make it clear*/
  for(i=center.x - a+precision; i<center.x+a-precision; i+=fig->resolution)
  {
    fig->points = (Point2D*)realloc(fig->points, (fig->num_of_points[0]+1)*sizeof(Point2D));
    fig->points[fig->num_of_points[0]].x = i;
    fig->points[fig->num_of_points[0]].y = sqrt( b2-b2*(pow(i-center.x,2)/a2) )+center.y;
    (fig->num_of_points[0])++;
  }
  for(i=center.x+a-precision; i>center.x-a+precision; i-=fig->resolution)/*-0.00001 to avoid making a duplicate*/
  {
    fig->points = (Point2D*)realloc(fig->points, (fig->num_of_points[0]+1)*sizeof(Point2D));
    fig->points[fig->num_of_points[0]].x = i;
    fig->points[fig->num_of_points[0]].y = -sqrt( b2-b2*(pow(i-center.x,2)/a2) )+center.y;/*-sqrt because we are on the other side of the equation we separated as two functions(down part)*/
    (fig->num_of_points[0])++;
  }
  fig->points = (Point2D*)realloc(fig->points, (fig->num_of_points[0]+1)*sizeof(Point2D) );
  fig->points[fig->num_of_points[0]] = start;
  (fig->num_of_points[0])++;
  (fig->num_of_shapes)++;
}
void draw_circle(Figure * fig, Point2D * center, double r)
{
  double i;
  double precision = 0.0001;
  Point2D start = {center->x-r+precision, center->y + sqrt(r*r-(center->x-r+precision-center->x)*(center->x-r+precision-center->x)) };
  for(i=center->x-r+precision; i<center->x+r-precision; i+=fig->resolution)/*Domain for x is from -r to r, also note the precision of double is -0.0001 minus in negative sign so that the root for y value never becomes smaller than 0, avoiding compilation error*/
  {
    fig->points = (Point2D*)realloc(fig->points, (fig->num_of_points[0]+1)*sizeof(Point2D));
    fig->points[fig->num_of_points[0]].x = i;
    fig->points[fig->num_of_points[0]].y = center->y + sqrt(r*r-(i-center->x)*(i-center->x));
    (fig->num_of_points[0])++;
  }
  for(i=center->x+r-precision; i>center->x-r+precision; i-=fig->resolution)/*Domain for x is from -r to r, also note the precision of double is +0.0001 plus in positive sign so that the root for y value never becomes smaller than 0, avoiding compilation error and starting from last elemnt - fig->resolution which the next instance of the point on the x-axis*/
  {/*the other half of a circle*/
    fig->points = (Point2D*)realloc(fig->points, (fig->num_of_points[0]+1)*sizeof(Point2D));
    fig->points[fig->num_of_points[0]].x = i;
    fig->points[fig->num_of_points[0]].y = center->y - sqrt(r*r-(i-center->x)*(i-center->x));
    (fig->num_of_points[0])++;
  }
  /*just making sure it connected*/
  fig->points = (Point2D*)realloc(fig->points, (fig->num_of_points[0]+1)*sizeof(Point2D));
  fig->points[fig->num_of_points[0]] = start;
  (fig->num_of_points[0])++;
  (fig->num_of_shapes)++;
  calling(fig);
}
void draw_polyline(Figure * fig, Point2D * poly_line, int n)
{
  int i;
  for(i=0; i<n; i++)
  {
    fig->points = (Point2D*)realloc(fig->points, sizeof(Point2D)*((fig->num_of_points[0])+1));
    fig->points[i] = poly_line[i];
    (fig->num_of_points[0])++;
  }
  (fig->num_of_shapes)++;
}

void draw_fx(Figure * fig, double f(double x), double start_x, double end_x)
{
  double i;
  for(i=start_x; i<=end_x+0.00001; i+=fig->resolution)
  {
      fig->points = (Point2D*)realloc(fig->points, sizeof(Point2D)*((fig->num_of_points[0])+1) );
      fig->points[fig->num_of_points[0]].x = i;
      fig->points[fig->num_of_points[0]].y = f(i);
      (fig->num_of_points[0])++;
  }
  (fig->num_of_shapes)++;
  calling(fig);
  printf("SHOULD BE REMOVED BUT REALLOC DOESNT WORK:%lf %lf\n", fig->points[fig->num_of_points[0]+1].x, fig->points[fig->num_of_points[0]+1].y);
}
double square(double x)
{
  return (x*x)/5;
}
/*
char* get_file_name(){

}*/
void set_color(Figure * fig, Color c)
{
  fig->color[0] = c;
  return;
}
void export_eps(Figure * fig, char * file_name)
{
  int i,j,total_points;
  strcat(file_name, ".eps");
  FILE* eps_file;
  eps_file = fopen(file_name, "w");
  if(eps_file==NULL) printf("Problem making the file, null pointer returned\n");
  fprintf(eps_file,"%%!PS-Adobe-3.0 EPSF-3.0\n");
  fprintf(eps_file,"%%%%BoundingBox: %lf %lf %lf %lf\n",-fig->canvas.width/2, -fig->canvas.height/2, fig->canvas.width/2, fig->canvas.height/2);

  for(i=0, j=0, total_points=fig->num_of_points[0]; i<fig->num_of_shapes; i++, total_points+=fig->num_of_points[i])
  {
    fprintf(eps_file,"newpath\n%lf %lf moveto\n",fig->points[j].x, fig->points[j].y);
    printf("NUM OF POINTS IS %d\n",fig->num_of_points[i]);
    for(j; j<total_points; j++)
    {
      fprintf(eps_file, "%lf %lf lineto\n", fig->points[j].x, fig->points[j].y);
    }
    fprintf(eps_file,"%lf %lf %lf setrgbcolor\n", (fig->color[i].R)/255, (fig->color[i].G)/255, (fig->color[i].B)/255);/*since eps's setrgbcolor operator works with values 0-1 and standard rgb is within 0-255 the proportion yields 1/255(for converting to eps's rgb format)*/
    fprintf(eps_file, "%lf setlinewidth\nstroke\n",fig->thickness[i]);
  }
  fprintf(eps_file, "showpage");
}
void export_svg(Figure * fig, char * file_name)
{
  int i,j,total_points;
  double center_x = fig->canvas.width/2;/*avoiding doing it in the loop everythime, too many interations for higher resolution*/
  double center_y = fig->canvas.height/2;

  strcat(file_name, ".svg");
  FILE* image_svg;
  image_svg=fopen(file_name,"w");
  if(image_svg == NULL) printf("Problem making the file, null pointer returned\n");
  fprintf(image_svg, "<svg width=\"%lf\" height=\"%lf\" version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\">\n", fig->canvas.width, fig->canvas.height);
  fprintf(image_svg,"<rect x=\"%d\" y=\"%d\" width=\"%lf\" height=\"%lf\" fill=\"%s\"/>\n", 0, 0, fig->canvas.width, fig->canvas.height, fig->canvas.fill);/*0,0 the beginning(top left corner)*/
  for(i=0,j=0, total_points=fig->num_of_points[0]; i<fig->num_of_shapes; i++, total_points+=fig->num_of_points[i])
  {
    for(j; j<total_points-1; j++)
    {
    fprintf(image_svg, "<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" style=\"stroke:rgb(%lf,%lf,%lf);stroke-width:%lf\" />\n", center_x+fig->points[j].x, center_y-fig->points[j].y, center_x+fig->points[j+1].x, center_y-fig->points[j+1].y, fig->color[i].R, fig->color[i].G, fig->color[i].B, fig->thickness[i]);
    if(j+1==fig->num_of_points[i]-1) j++;/*to successfully continue from the next shape(note that points are printed with both j and j+1 indexes, so we are just avoiding j+1 we have already printed*/
    }
    /*other things to be put before closing the svg, consider implementing with stacks*/
  }
  fprintf(image_svg, "</svg>");
}
// typedef enum{width, height, x1, y1, x2, y2, color, thickness}dictionary;
// void import_svg(char * file_name, Figure * fig)
// {
//   FILE* file;
//   char* text;
//   int i;
//   char target[20];/*change this 20 bullshit, make it dynamic if possible, ask hoca*/
//   char* dictionary[] = {"width=","height=","x1=","y1=","x2=","y2=","stroke:rgb(", "stroke-width:"};
//   printf("%ld\n", strlen(dictionary[0]));
//   int dictionary_length = sizeof(dictionary)/sizeof(char*);
//   printf("%d\n", dictionary_length);getchar();
//   file = fopen(file_name, "r");
//   while(fscanf(file,"%s",target)!=NULL)
//   {
//     for(i=0; i<dictionary_length; i++)
//     {
//       if(strncmp(dictionary[i], target, strlen(dictionary[i]))==0 )
//       {
//         switch (i) {
//           case width:
//             fig->width = atio
//           break;
//           case height:
//
//           break;
//           case x1:
//
//           break;
//           case y1:
//
//           break;
//           case x2:
//
//           break;
//           case y2:
//
//           break;
//           case color:
//
//           break;
//           case thickness:
//
//           break;
//         }
//       }
//     }
//   }
// }
void set_thickness_resolution(Figure* fig, double thickness, double resolution)
{
  fig->thickness[0] = thickness;
  fig->resolution = resolution;
}

Figure* start_figure(double width,double height)/*sets up the canvas of the figure*/
{
  Figure* new_figure = (Figure*)malloc(sizeof(Figure));
  new_figure->canvas.width = width;
  new_figure->canvas.height = height;
  new_figure->canvas.fill = "white";/*putting white as background color of the canvas, this can be of course modified*/
  new_figure->num_of_points = (int*)malloc(sizeof(int));
  new_figure->color = (Color*)malloc(sizeof(Color));
  new_figure->thickness = (double*)malloc(sizeof(double));
  new_figure->num_of_shapes = 0;
  return new_figure;
}
