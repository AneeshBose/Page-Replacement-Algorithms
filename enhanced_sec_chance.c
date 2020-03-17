#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

int FRAME_NO; //number of frames to be in memory defined by the user.

struct frame
{
    int data;
    int use_bit,modify_bit;
};

//Checks if the frame number is in the current set of frames.
bool check(struct frame a[],int ele)
{
    int i;
    for(i=0;i<FRAME_NO;i++)
        if(a[i].data==ele)
            return true;
    return false;
}

void print(struct frame a[])
{
    int i;
    printf("FRAME\t USE_BIT\t MODIFY_BIT\n");
    for(i=0;i<FRAME_NO;i++)
    {
        printf("%d\t %d\t\t %d\n",a[i].data,a[i].use_bit,a[i].modify_bit);
    }
}

int main()
{
    printf("\n*************************ENHANCED SECOND CHANCE ALGORITHM**********************\n");
    printf("\nEnter the number of frames:\n");
    scanf("%d",&FRAME_NO);
    int n;
    printf("\nEnter the number of pages:\n");
    scanf("%d",&n);
    int ref_string[1000],modified[1000];
    int i;
    printf("\nEnter the reference string:\n");
    for(i=0;i<n;i++)
        scanf("%d",&ref_string[i]);

    //Takes the modified bit(the data to be written to memory) by the user.

    printf("\nEnter modified state for each entry in the reference string (1-modified,0-not modified):\n");
    for(i=0;i<n;i++)
        scanf("%d",&modified[i]);
    struct frame frame_set[FRAME_NO];

    //Default initialization
    for(i=0;i<FRAME_NO;i++)
    {
        frame_set[i].data=-1;
        frame_set[i].use_bit=frame_set[i].modify_bit=-1;
    }

    int victimptr=0;int pgfaultcnt=0; i=0; bool found;

    //Puts the first FRAME_NO no. of frames in memory.
    while(victimptr<FRAME_NO && i<n)
    {
        found=check(frame_set,ref_string[i]);
        if(!found)
        {
            printf("Page miss for %d!\n\n",ref_string[i]);
            pgfaultcnt++;
            frame_set[victimptr].data=ref_string[i];
            frame_set[victimptr].use_bit=1;
            if(modified[i])
                frame_set[victimptr].modify_bit=1;
            else
                frame_set[victimptr].modify_bit=0;
            victimptr++;
        }
        else
            printf("Page hit for %d!\n\n",ref_string[i]);
        i++;
        print(frame_set);
    }
    int j; int c=0; int flag=0; victimptr=0;
    //Enhanced second-chance algorithm for the rest of the reference string.

    for(j=i;j<n;j++)
    {
        found=check(frame_set,ref_string[j]);
        if(!found)
        {
            printf("Page miss for %d!\n\n",ref_string[j]);
            pgfaultcnt++; flag=0;

            while(1)
            {
                //Check if the used bit and the modify bit are both 0. Best frame to replace.
                //c keeps track of the victimptr coming back to the same frame twice. This is important as first cycle it checks for (0,0).
                //Next cycle it checks for (0,1).
                c=0;
                while(c<FRAME_NO)
                {
                    if(frame_set[victimptr].use_bit==0 && frame_set[victimptr].modify_bit==0)
                    {
                        flag=1;
                        break;
                    }
                    else
                        victimptr=(victimptr+1)%FRAME_NO;
                    c++;
                }
                c=0;
                if(!flag)
                {
                    while(c<FRAME_NO)
                    {
                        //Else check if used bit is 0 and modify bit is 1. Second best frame.
                        if(frame_set[victimptr].use_bit==0 && frame_set[victimptr].modify_bit==1)
                        {
                            flag=1;
                            break;
                        }
                        else
                        {
                            //If such a frame is also not found, keep clearing the used bits.
                            //The next time we come across this frame, it is sure to have its (used bit,modify-bit) as (0,0) or (0,1).
                            frame_set[victimptr].use_bit=0;
                            victimptr=(victimptr+1)%FRAME_NO;
                        }
                        c++;
                    }
                }
                if(flag)
                    break;
            }
            frame_set[victimptr].data=ref_string[j];
            frame_set[victimptr].use_bit=1;
            if(modified[j])
                frame_set[victimptr].modify_bit=1;
            else
                frame_set[victimptr].modify_bit=0;
            victimptr=(victimptr+1)%FRAME_NO;
        }
        else
            printf("Page hit for %d!\n\n",ref_string[j]);
        print(frame_set);
    }
    printf("\nTotal number of page faults are %d.\n",pgfaultcnt);
}
