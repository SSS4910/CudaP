/* Buffer tester
 *
 *  by John Stanley
 */

# include <stdio.h>
# include <string.h>
//# include "buffer.c"

struct Buffer { int size; int ready; char* strArray[]; };

int main(void)
{
    struct Buffer buffer1;
    struct Buffer buffer2;
    
    makebuffer(&buffer1);
    makebuffer(&buffer2);
    
    int i; 
    for(i = 0; i < buffer1.size; i++)
    {
        printf("%s \n",buffer1.strArray[i]);
        fflush(stdout);
    }
    



    //buffer1.strArray[0] = "I am the kite";
    //strcpy(buffer1.strArray[0], "I AM THE KITE");
    //printf("%s \n",buffer1.strArray[0]);
 
    for(i = 0; i < buffer1.size; i++)
    {
        printf("%s \n",buffer1.strArray[i]);
        fflush(stdout);
    }


    //free(buffer1.strArray);
    /*for(i = 0; i < buffer1.size; i++)
    {
        free(buffer1.strArray[i]);
    }*/
    return 0;
}


int makebuffer(struct Buffer *in)
{
    in->strArray[5];

    in->size = 5;
    in->ready = 0;
    //in.strArray = (char**) malloc((5 * 50)*sizeof(char));
    /*for(i = 0; i < buffer1.size; i++)
    {
        in.strArray[i] = (char)malloc((50*sizeof(char));
    }*/




    strcpy(in->strArray[0], "I am the night");
    strcpy(in->strArray[1], "But he is the fright");
    strcpy(in->strArray[2], "My god these pants are tight");
    strcpy(in->strArray[3], "Does anyone have a light");
    strcpy(in->strArray[4], "Oh shit thats bright");
    
    //in->ready = 1;


    return 0;
}
