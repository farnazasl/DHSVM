/*
 * SUMMARY:      FindValue.c - Initialize lookup tables
 * USAGE:        Part of MWM
 *
 * AUTHOR:       Laura Bowling
 * ORG:          University of Washington, Department of Civil Engineering
 * ORIG-DATE:    Oct-02
 * DESCRIPTION:  Find stochastic values of mass wasting parameters
 * DESCRIP-END.
 * COMMENTS:     
 */


#define BUFSIZE      255
#include <stdio.h> 
#include <stdlib.h> 
#include <math.h> 
#include <sys/types.h> 
#include <time.h>
#include "data.h"
#include "constants.h"


double drand48(void); 
void srand48 (long);


// define the statistical distribution 
#define NORMALDIST(mean, stdev, y) (4.91 * stdev * (pow(y,.14) - pow(( 1 - y ),.14)) + mean )
#define UNIFORMDIST(min, max, y) ((max - min) * y + min) 

float TRIDIST(float min, float max, float mode, float y);

float FindValue(STATSTABLE Stats) {

  float y;
  float temp;

  time_t tloc;

  /* Randomize Random Generator */
  time (&tloc);
  srand48 (tloc);
 
  /** Generate **/
  temp  = drand48();

  if(strcmp(Stats.Distribution,"NORMAL")==0) {
    if(MASSITER == 1)
      y= Stats.mean;
    else
      y = NORMALDIST(Stats.mean, Stats.stdev, temp);
  }
  else if(strcmp(Stats.Distribution,"TRIANGULAR")==0) {
     if(MASSITER == 1)
       y= Stats.mode;
     else
       y = TRIDIST(Stats.min, Stats.max, Stats.mode, temp);
  }
  else if(strcmp(Stats.Distribution,"UNIFORM")==0) {
     if(MASSITER == 1)
       y= Stats.min + (Stats.max - Stats.min)/2.;
     else
       y = UNIFORMDIST(Stats.min, Stats.max, temp);
  }
  else {
    fprintf(stderr,"Not a valid distribution %s.\n", Stats.Distribution);
    exit(0);
  }

  return(y);
}


float TRIDIST(float min, float max, float mode, float y) 
{
  float trivar;

  if (y >= (mode-min)/(max-min)) {
    trivar = max - sqrt((1.-y)*(max-min)*(max-mode));
  }
  else
    trivar = min + sqrt(y*(mode-min)*(max-min));
  
  return(trivar);
}

