#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pulse/simple.h>
#include <pulse/error.h>
#include <pulse/gccmacro.h>

#define BUFSIZE 202400

int main(){
pa_simple *s1,*s2;
pa_sample_spec ss;
ss.format = PA_SAMPLE_S16NE;
ss.channels = 2;
ss.rate = 44100;
int buf[BUFSIZE];

char ch[2];
while(fgets(ch,2,stdin))
{   
    
    s1 = pa_simple_new(NULL,"listen",PA_STREAM_RECORD,NULL,"listen",&ss,NULL,NULL,NULL);
    s2 = pa_simple_new(NULL,"talking",PA_STREAM_PLAYBACK,NULL,"talking",&ss,NULL,NULL,NULL);

    
    printf("you're in!\n");
    if( pa_simple_read(s1,buf,BUFSIZE,NULL)){printf("error recording\n");}
    else{printf("recorded\n");}

    if(pa_simple_write(s2,buf,BUFSIZE,NULL)){printf("error playing\n");}
    else{printf("played\n");}

//    printf("%ld\n",(pa_simple_get_latency(s1,NULL)));

    pa_simple_flush(s1,NULL);
    pa_simple_flush(s2,NULL);
    pa_simple_free(s1);
    pa_simple_free(s2);
    fflush(stdin);
}
return 0;
}











