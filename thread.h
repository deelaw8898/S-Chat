typedef long PID;
typedef char PPRIO;
#define PROCESS void
#define HIGH ((PPRIO) 0 ) /*  high priority     */
#define USR  1 /* a user process */
extern void *Send ( PID, void *, int * );
extern void *Receive( PID *, int * );
extern int  Reply( PID, void *, int );
extern PID  Create( void(*)(),  int, char *, void *, PPRIO, int ); 
