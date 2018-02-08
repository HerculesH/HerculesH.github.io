#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct PCB *Tail=NULL;
struct PCB *Head=NULL;
int Total_job=0;
int Total_turnaround_time=0;
int Total_waiting_time=0;
int CLOCK=0;
int CPUreg[8] = {0};
char print;
int digitarray[100];
int count = 0;
int maxcount = 0;

struct PCB {
    int ProcId; 
    int ProcPR; 
    int CPUburst; 
    int Reg[8]; 
    int queueEnterClock; 
    int waitingTime; 
    /* other info */
    struct PCB *next;
    };

void append(struct PCB** head_ref, int ProcessID, int ProcessPrio, int CPUBurstT)
{
  struct PCB* new_node = (struct PCB*) malloc(sizeof(struct PCB));
             
  struct PCB *last = *head_ref; 
  new_node->ProcId  = ProcessID;
  new_node->ProcPR = ProcessPrio;
  new_node->CPUburst = CPUBurstT;
  new_node->queueEnterClock = 0;
  new_node->waitingTime = 0;
  new_node->Reg[8] = ProcessID;

  new_node->next = NULL;
                                              
  if (*head_ref == NULL)
  {
  *head_ref = new_node;
  return;
  }
                                                                                     
  while (last->next != NULL) 
  last = last->next;
                                                                                                          
                                                                            
  last->next = new_node;
  return;
}

void runLL(struct PCB *pcb)
{     
      while (pcb != NULL)
            {
            CPUreg[8] = pcb->Reg[8];
            pcb->Reg[8] = CPUreg[8];
            pcb->waitingTime = pcb->waitingTime + CLOCK - pcb->queueEnterClock;
            Total_waiting_time = Total_waiting_time + pcb->waitingTime;
            CLOCK = CLOCK + pcb->CPUburst;
            Total_turnaround_time = Total_turnaround_time + CLOCK;
            Total_job = Total_job + 1;
            pcb = pcb->next;
            }
}

void printalg()
{
   printf("Student Name: Hercules Hjalmarsson \n");
   printf("Input File Name : input1.txt \n");
   printf("CPU Scheduling Alg : FIFO \n");
}

void printprefM()
{   float AWT = (float)Total_waiting_time / (float)Total_job;
    float ATT = (float)Total_turnaround_time / (float)Total_job;
    float T = (float)Total_job / (float)CLOCK;

    printf("Average Waiting time =  %.2f ms     (%d/%d)\n", AWT,Total_waiting_time,Total_job);     
    printf("Average Turnaround time = %.2f ms   (%d/%d)\n", ATT,Total_turnaround_time,Total_job);  
    printf("Throughput = %.2f jobs per ms       (%d/%d)\n", T,Total_job,CLOCK);
}

void removeLL(struct PCB **head_ref, int PID)
{
    struct PCB* temp = *head_ref, *prev;

    if(temp != NULL && temp->ProcId == PID)
    {
        *head_ref = temp->next;
        free(temp);
        return;
    }

    while (temp != NULL && temp->ProcId != PID)
    {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) return;

    prev->next = temp->next;

    free(temp);
}

void FIFO_Scheduling()
{
    runLL(Head);
    struct PCB *pcb = Head;
    int i = 1;
    int burstcalc = 0;
    while(pcb != NULL)
    {
        burstcalc = burstcalc + pcb->CPUburst;
        printf("Process %d is completed at %d ms\n",i,burstcalc);
        pcb = pcb->next;
        removeLL(&Head,i);
        i++;
    }

}

int main(int argc, char *argv[])
{
    if(argc < 4)
    {
        printf("Error (Not enough arguments)\n");
        return 0;
    }
    
    int i = 0;
    int runFIFO = 0;
    int inputspc = 0;
    int algspc = 0;
    char *inputptr;

    while(i < argc)
    {
        if (strncmp("-alg", argv[i],4) == 0)
        {
		algspc = 1;
	}
            
	if (strncmp("FIFO", argv[i],4) == 0)
        {
                runFIFO = 1;
        }
        
        if (strncmp("-input", argv[i],5) == 0)
        {
		inputspc = 1;
	}
            if (strncmp("input1.txt", argv[i],9) == 0)
            {
                inputptr = argv[i];
                
            }
        
        i++;
    }
    
    if(algspc == 0)
    {
        printf("Error (alg not specified)\n");
        return 0;
    }
    
    if(inputspc == 0)
    {
        printf("Error (input not specified)\n");
        return 0;
    }
    
    FILE *fp;
    fp=fopen(inputptr, "r");
    
    print = fgetc(fp);
    while (print != EOF)
    {
        
        if(isdigit(print))
        {
         int x = print - '0';
         digitarray[count] = x;
         count++;
        }

        print = fgetc(fp);


    }

    fclose(fp);

    maxcount = count;
    count = 0;

    while(count < maxcount)
    {
      append(&Head,digitarray[count],digitarray[count + 1],digitarray[count + 2]);
      count += 3;
    }

    printalg();
    
    if(runFIFO == 1)
    {
        FIFO_Scheduling();
    }
    
    printprefM();

    return 0;
}
