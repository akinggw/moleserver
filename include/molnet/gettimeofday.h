#ifndef __GETTIMEOFDAY_C
#define __GETTIMEOFDAY_C

#if defined(_MSC_VER) || defined(_WINDOWS_)
   #include <time.h>
   #if !defined(_WINSOCK2API_) && !defined(_WINSOCKAPI_)
         struct timeval 
         {
            long tv_sec;
            long tv_usec;
         };
   #endif 
#else
   #include <sys/time.h>
#endif 
#if defined(_MSC_VER) || defined(_WINDOWS_)
   int gettimeofday(struct timeval* tv) 
   {
      union {
         long long ns100;
         FILETIME ft;
      } now;
     
      GetSystemTimeAsFileTime (&now.ft);
      tv->tv_usec = (long) ((now.ns100 / 10LL) % 1000000LL);
      tv->tv_sec = (long) ((now.ns100 - 116444736000000000LL) / 10000000LL);
     return (0);
   }
#endif
#endif /* __GETTIMEOFDAY_C */