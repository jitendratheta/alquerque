//
//  constant.h
//  Alquerque
//
//  Created by Jitendra S. Kushwaha on 16/02/17.
//  Copyright © 2017 Jitendra S. Kushwaha. All rights reserved.
//

#ifndef constant_h
#define constant_h

#include <iostream>
using namespace std;

#define DBG

#ifdef DBG
#define DB(x) cerr<<#x<<" : "<<x<<endl<<flush;
#define DB2(x,y) cerr<<#x<<" : "<<x<<" "<<#y<<" : "<<y<<endl<<flush;
#define DB3(x,y,z) cerr<<#x<<" : "<<x<<" "<<#y<<" : "<<y<<" "<<#z<<" : "<<z<<endl<<flush;
#define DB4(w,x,y,z) cerr<<#w<<" : "<<w<<" "<<#x<<" : "<<x<<" "<<#y<<" : "<<y<<" "<<#z<<" : "<<z<<endl<<flush;
#define DB5(v,w,x,y,z) cerr<<#v<<" : "<<v<<" "<<#w<<" : "<<w<<" "<<#x<<" : "<<x<<" "<<#y<< \
" : "<<y<<" "<<#z<<" : "<<z<<endl<<flush;
#define DBAR(arr,a,b) cerr<<#arr<<" : ";FOR(dbar,a,b) cerr<<arr[dbar]<<" ";cerr<<endl;
#define DBAR2(arr,a,b,x,y) cerr<<#arr<<endl;FOR(dbar,a,b){ FOR(dbar2,x,y)cerr<<arr[dbar][dbar2]<<" ";cerr<<endl;}

#else
#define DB(x)
#define DB2(x,y)
#define DB3(x,y,z)
#define DB4(w,x,y,z)
#define DB5(v,w,x,y,z)
#define DBAR(arr,a,b)
#define DBAR2(arr,a,b,x,y)
#endif

#endif /* constant_h */
