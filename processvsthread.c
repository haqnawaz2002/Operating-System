// #include <stdlib.h>
// #include <stdio.h>
// #include <unistd.h>
// #include <pthread.h>

// void* routine(){
//     printf("id is %d\n",getpid());
// }



// int main()
// {
//     pthread_t t1,t2;
//     if(pthread_create(&t1,NULL,&routine,NULL)){
//         return 1;
//     }
//     if(pthread_join(t1,NULL)){
//         return 2;
//     }


//     return 0;

// }





// PROCESSES

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main()
{
    int pid = fork();
    if(pid == -1){
        return 1;
    }
    printf("hello from process %d\n",getpid());
    if(pid != 0){
        wait(NULL);
    }
}