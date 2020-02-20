/* Pract2  RAP 09/10    Javier Ayllon*/

#include <openmpi/mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h> 
#include <assert.h>   
#include <unistd.h> 
#include <math.h>
#include "mpi.h"  
#define NIL (0) 
#define np 7      

/*Variables Globales */

XColor colorX;
Colormap mapacolor;
char cadenaColor[]="#000000";
Display *dpy;
Window w;
GC gc;

long particion=(400*3*sizeof(unsigned char))*floor(400/np);
//long particion=floor((400*400*3)*sizeof(unsigned char)/np);

/*Funciones auxiliares */

void initX() {

      dpy = XOpenDisplay(NIL);
      assert(dpy);

      int blackColor = BlackPixel(dpy, DefaultScreen(dpy));
      int whiteColor = WhitePixel(dpy, DefaultScreen(dpy));

      w = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy), 0, 0,
                                     400, 400, 0, blackColor, blackColor);
      XSelectInput(dpy, w, StructureNotifyMask);
      XMapWindow(dpy, w);
      gc = XCreateGC(dpy, w, 0, NIL);
      XSetForeground(dpy, gc, whiteColor);
      for(;;) {
            XEvent e;
            XNextEvent(dpy, &e);
            if (e.type == MapNotify)
                  break;
      }


      mapacolor = DefaultColormap(dpy, 0);

}

void dibujaPunto(int x,int y, int r, int g, int b) {

        sprintf(cadenaColor,"#%.2X%.2X%.2X",r,g,b);
        XParseColor(dpy, mapacolor, cadenaColor, &colorX);
        XAllocColor(dpy, mapacolor, &colorX);
        XSetForeground(dpy, gc, colorX.pixel);
        XDrawPoint(dpy, w, gc,x,y);
        XFlush(dpy);

}



/* Programa principal */

int main (int argc, char *argv[]) {

  int rank,size;
  MPI_Comm commPadre, intercomm;
  int tag;
  MPI_Status status;
  int buf[5];
  unsigned char bufr[3];
  int errcodes[np];
  int lineas;
  int media;
  int tr;
  int tg;
  int tb;

  MPI_File fh;
  MPI_Offset offset;


  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_get_parent( &commPadre );
  if ( (commPadre==MPI_COMM_NULL)
        && (rank==0) )  {

	initX();

      MPI_Comm_spawn( "exec/pract2", &argv[1], np, MPI_INFO_NULL, 0, MPI_COMM_WORLD, &intercomm, errcodes );
	/* Codigo del maestro */


      
      for(int i=0;i<400*400;i++){
            MPI_Recv(&buf,5,MPI_INT,MPI_ANY_SOURCE,0,intercomm,&status);
            dibujaPunto(buf[0],buf[1],buf[2],buf[3],buf[4]);
      }

      sleep(5);
      
	//dibujaPunto(x,y,r,g,b);  

        }

  //}

 	
  else {
      
      
    /* Codigo de todos los trabajadores */
    /* El archivo sobre el que debemos trabajar es foto.dat */
      MPI_File_open(MPI_COMM_WORLD,"dirs/foto.dat",MPI_MODE_RDONLY,MPI_INFO_NULL,&fh);
      MPI_File_set_view(fh, particion*rank, MPI_UNSIGNED_CHAR, MPI_UNSIGNED_CHAR, "native", MPI_INFO_NULL);
      if(rank==np-1){
            lineas=floor(400/np)+(400-(floor(400/np)*np));
      }else{
            lineas=floor(400/np);
      }
      for(int i=0;i<lineas;i++){
            for(int j=0;j<400;j++){
            MPI_File_read(fh, bufr, 3, MPI_UNSIGNED_CHAR, &status);
            //printf("%d %d %d\n",(int)bufr[0],(int)bufr[1],(int)bufr[2]);
            buf[0]=j;//para mandar las cordenadas correctas
            buf[1]=i+(rank*(ceil(400/np)));
            if(*argv[1]=='n'){
                  buf[2]=(int)bufr[0];
                  buf[3]=(int)bufr[1];
                  buf[4]=(int)bufr[2];
            }else if(*argv[1]=='b'){
                  media=((int)bufr[0]+(int)bufr[1]+(int)bufr[2])/3;
                  buf[2]=media;
                  buf[3]=media;
                  buf[4]=media;  
            }else if(*argv[1]=='s'){
                  tr = (0.393*(int)bufr[0]) + (0.769*(int)bufr[1]) + (0.189*(int)bufr[2]);
                  tg = (0.349*(int)bufr[0]) + (0.686*(int)bufr[1]) + (0.168*(int)bufr[2]);
                  tb = (0.272*(int)bufr[0]) + (0.534*(int)bufr[1]) + (0.131*(int)bufr[2]);
                  
                  if(tr > 255){
                        buf[2] = 255;
                  }else{
                        buf[2] = tr;
                  }
                  
                  if(tg > 255){
                        buf[3] = 255;
                  }else{
                        buf[3] = tg;
                  }

                  if(tb > 255){
                        buf[4] = 255;
                  }else{
                        buf[4] = tb;
                  }
            }else if(*argv[1]=='A'){
                  media=((int)bufr[0]+(int)bufr[1]+(int)bufr[2])/3;
                  buf[2]=0;
                  buf[3]=0;
                  buf[4]=media;  
            }
            else if(*argv[1]=='V'){
                  media=((int)bufr[0]+(int)bufr[1]+(int)bufr[2])/3;
                  buf[2]=0;
                  buf[3]=media;
                  buf[4]=0;  
            }else if(*argv[1]=='R'){
                  media=((int)bufr[0]+(int)bufr[1]+(int)bufr[2])/3;
                  buf[2]=media;
                  buf[3]=0;
                  buf[4]=0;  
            }else if(*argv[1]=='a'){
                  media=((int)bufr[0]+(int)bufr[1]+(int)bufr[2])/3;
                  buf[2]=media;
                  buf[3]=media;
                  buf[4]=0;  
            }else if(*argv[1]=='v'){
                  media=((int)bufr[0]+(int)bufr[1]+(int)bufr[2])/3;
                  buf[2]=media;
                  buf[3]=0;
                  buf[4]=media;  
            }else if(*argv[1]=='c'){
                  media=((int)bufr[0]+(int)bufr[1]+(int)bufr[2])/3;
                  buf[2]=0;
                  buf[3]=media;
                  buf[4]=media;  
            }
            
            MPI_Bsend(&buf,5,MPI_INT,0,0,commPadre);
            }
      }
      MPI_File_close(&fh);

  }

  MPI_Finalize();

}

