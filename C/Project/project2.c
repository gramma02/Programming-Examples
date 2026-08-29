/* ONOMATEPWNYMO: Grammatikou Petros
   AEM: 03366
 */


#include"project2.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


//STRUCTS
typedef struct
{
    char first[NAMESIZE];
    char last[NAMESIZE];

}name;


typedef struct list
{
    unsigned short int class_id;
    struct list *next;

}classes_node;


typedef struct student_info
{
    unsigned long int aem;
    name full_name;
    unsigned short int failed_classes;
    classes_node *head;
    struct student_info *prev;
    struct student_info *next; 

}student_info;


typedef struct 
{
    int numb_of_entries;
    int k; //the starting size of the hass table
    int load_factor;
    int numb_of_blocks;
    
}Hass_info;



//FUNCTIONS


//INITIALIZE
void initialize(student_info ***ptrs, int size, int n, int starting_pos)
{
    int i;

    for(i=starting_pos; i<size; i++)
        (*ptrs)[i] = NULL;
}

//FIND
student_info* find(student_info** ptrs, unsigned long int aem, int taken_spots, int *comps)
{
    int i;
    (*comps)=0;

    for(i=0; i<taken_spots; i++)
    {
        (*comps)=(*comps)+1;
        if(ptrs[i]!=NULL && aem==ptrs[i]->aem)
            return (ptrs[i]);
    }
    
    return (NULL);
}

//BINARY SEARCH
student_info* binary_search(student_info** ptrs, unsigned long int aem, int taken_places, int *comps)
{
    int start=0;
    int finish=taken_places-1;
    int middle=0;
    (*comps)=0;

    while(start<=finish)
    {
        middle=((start+finish)/2);

        if(ptrs[middle]!=NULL && ptrs[middle]->aem==aem)
        {
            (*comps)=(*comps)+1;
            return(ptrs[middle]);
        }
        else if(ptrs[middle]!=NULL && aem<ptrs[middle]->aem)
        {
            (*comps)=(*comps)+2;
            finish=(middle-1);
        }
        else
        {
            (*comps)=(*comps)+2;
            start=(middle+1);
        }
    }
    return (NULL);
}



//MOD
int mod(student_info ***ptrs, unsigned long int aem, unsigned short int failed, int size, int sorted, int *comps, int taken_places)
{
    student_info *find_result;

    if(sorted)
        find_result=binary_search(*ptrs, aem, taken_places, comps);
    else
        find_result=find(*ptrs, aem, taken_places, comps);


    if(find_result)
    {
        find_result->failed_classes=failed;
        return 1; //success
    }
    return 0; //fail
}



//PRINT
void print(student_info **ptrs, int taken_places)
{
    int i;
	printf("\n##\n");
	
	for(i=0; i<taken_places; i++)
		if(ptrs[i]!=NULL)
			printf("%lu %s %s %d\n", ptrs[i]->aem, ptrs[i]->full_name.first, ptrs[i]->full_name.last, ptrs[i]->failed_classes);
}





//INSERTION SORT
void insertion_sort(student_info***ptrs, int taken_spots, int *comps)
{
    int i, j;
    student_info *k=NULL;
    (*comps)=0;

    for(i=1; i<taken_spots; i++)
    {
        for(j=i; j>0; j--)
        {
            (*comps)=(*comps)+1;
            if((*ptrs)[j]->aem<(*ptrs)[j-1]->aem) //swap
                {
                    k=(*ptrs)[j];
                    (*ptrs)[j]=(*ptrs)[j-1];
                    (*ptrs)[j-1]=k;
                }
            else  //swapnot
                break; 

        }
    }

}

//CLASSES LIST FUNCTIONS

//FIND IN THE CLASSES LIST
classes_node *isreg(classes_node **head, unsigned short int class_id, int *flag)
{
    classes_node *curr=NULL;
    classes_node *prev=NULL;

    if((*head)==NULL) //list empty , insert first
    {
        *flag=-1;
        return (prev);
    }

    prev=*head;
    curr=*head;

    while((curr!=NULL)&&(curr->class_id < class_id))
    {
        prev=curr;
        curr=curr->next;
    }
    

    if(curr==NULL) //Not found because list doesnt contain any larger class_id so we insert last
    {
        *flag=0; //insert inside
        return (prev);
    }
    if(curr->class_id==class_id)
    {
        *flag=1; //Found
        return (prev);
    }
    if(curr->class_id>class_id)
    {
        if(curr==*head)
            *flag=-1; //smaller than everything, insert first
        else
            *flag=0; //found a larger class_id so we must insert right before that one 
        
        return (prev);
    }

    return (prev);

}

//INSERT CLASSES LIST FUNCTIONS
int insert_first(classes_node **head, unsigned short int class_id)
{
    classes_node *newnode;

    newnode=(classes_node*)malloc(sizeof(classes_node));
    
    if(!newnode)
        return 0; //No memory
    
    newnode->class_id=class_id;
    newnode->next=*head;
    *head=newnode;

    return 1; //success
}

int insert_inside(classes_node *prv, unsigned short int class_id)
{
    classes_node *newnode;

    newnode=(classes_node*)malloc(sizeof(classes_node));

    if(!newnode)
        return 0; //No memory

    newnode->class_id=class_id;
    newnode->next=prv->next;
    prv->next=newnode;

    return 1; //success
}

int reg(student_info ***ptrs, int aem, unsigned short int class_id, int sorted, int taken_places, int *comps)
{
    classes_node *isreg_result;
    student_info *find_result;
    int flag=1;

    //Find the student
    if(sorted)
        find_result=binary_search(*ptrs, aem, taken_places, comps);
    else
        find_result=find(*ptrs, aem, taken_places, comps);

    if(find_result)
    {
        isreg_result=isreg(&find_result->head, class_id, &flag);
        //printf("flag: %d", flag);


        if(flag==1)
        {
            return -1; //Fail, class already exists
        }
        if(flag==-1) //it must be unserted first
        {
            //printf("inserted first\n");
            if(insert_first(&find_result->head, class_id))
                return 1; //success
            else
                return -2; //No memory
        }
        if(flag==0) //it must be inserted in front of the is_reg_result aka previus
        {
            //printf("inserted inside\n");
            if(insert_inside(isreg_result, class_id))
                return 1; //success
            else
                return -2; //No memory

        }
    }
    
    return 0; //Fail , student not found
}


//DELETE LIST CLASSES FUNCTIONS
void delete_first(classes_node **head)
{
    classes_node *current;

    current=*head;
    (*head)=(*head)->next;

    free(current);
    
}

void delete_inside(classes_node *reg_result)
{
    classes_node *current;

    current=reg_result->next;
    reg_result->next=current->next;

    free(current);

}

int unreg(student_info ***ptrs, int aem, int class_id, int sorted, int taken_places, int *comps)
{
    student_info *find_result;
    classes_node *isreg_result;
    int flag;

    //Find the student
    if(sorted)
        find_result=binary_search(*ptrs, aem, taken_places, comps);
    else
        find_result=find(*ptrs, aem, taken_places, comps);

    if(find_result)
    {
        isreg_result=isreg(&find_result->head, class_id, &flag);

        if(flag==1)
        {
            if(find_result->head->class_id==class_id)
            {
                //printf("delete FIRST\n");
                delete_first(&find_result->head);
            }
            else
            {
                //printf("delete INSIDE\n");
                delete_inside(isreg_result);
            }
            
            return 1; //success
        }
        else
            return -1; //fail, class not found
    }
    return 0; //Fail, no student found
}

void delete_classes_list(classes_node **head)
{
    while((*head)!=NULL)
    {
        delete_first(head);
    }
}

int list_courses(student_info ***ptrs, int sorted, int taken_places, int *comps, unsigned long int aem)
{
    student_info *find_result;
    classes_node *curr;

     //Find the student
    if(sorted)
        find_result=binary_search(*ptrs, aem, taken_places, comps);
    else
        find_result=find(*ptrs, aem, taken_places, comps);

    if(!(find_result))
    {
        printf("\nL-NOK %lu\n", aem);
        return -1; //Not found
    }
    else
    {
        printf("\nL-OK %s %lu\n", find_result->full_name.last, find_result->aem);

        for(curr=find_result->head; curr!=NULL; curr=curr->next)
            printf("%d\n",curr->class_id);

        return 1; //Done
    }

}



//HASS TABLE FUNCTIONS


//HASS INITIALAZE
void hass_init(student_info **hass_table,Hass_info hass_info)
{
    int i;
    for(i=0; i<hass_info.numb_of_blocks; i++)
    {
        (*hass_table)[i].aem=0;
        (*hass_table)[i].failed_classes=0;
        strcpy((*hass_table)[i].full_name.first,"zzzzzzzzz");
        strcpy((*hass_table)[i].full_name.last,"zzzzzzzzz");
        (*hass_table)[i].head=NULL;
        (*hass_table)[i].next=&(*hass_table)[i];
        (*hass_table)[i].prev=&(*hass_table)[i];
    }
}

//HASS FUNCTION
unsigned long int hash(char *str)
{
    unsigned long int hash = 5381;
    int c;
    while ((c = *str++)) 
    {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}


//PREVIOUS POSITION FINDER IN THE STUDENTS LIST
student_info *pos_finder(student_info *sentinel, char *last_name, unsigned long int aem, int *comps)
{
    student_info *curr;
    sentinel->aem=0;

    (*comps)++;
    if(sentinel->next->aem==0)
        return (sentinel);
    
    for(curr=sentinel->next; curr->aem!=0; curr=curr->next)
    {
        (*comps)++;//for
        (*comps)++;//if
        if(strcmp(last_name, curr->full_name.last)<=0)
        {
            (*comps)++;//if
            if(strcmp(last_name, curr->full_name.last)<0)
                return (curr->prev);
            else //same last name name
            {
                (*comps)++;//if
                if(aem<curr->aem)
                {
                    return (curr->prev);
                }
            }
        }
    }
    return (curr->prev); //in this case it does point to the last block
}


//HASS ADDER
void hass_adder(student_info **hass_table, student_info *new_entry, Hass_info hass_info, int *comps)
{
    student_info *curr;
    unsigned long int block;
    student_info *table = *hass_table;

    block=hash(new_entry->full_name.last) % hass_info.numb_of_blocks;
    //printf("\n block= %lu\nnumb_of_blocks= %d",block, hass_info.numb_of_blocks);
    curr=pos_finder(&table[block], new_entry->full_name.last, new_entry->aem, comps);

    new_entry->next=curr->next;
    new_entry->prev=curr;
    curr->next=new_entry;
    new_entry->next->prev=new_entry;

    table[block].failed_classes++; //here we count the size of the block
}

void hass_remover(student_info **hass_table, student_info *dead_entry, Hass_info hass_info, int *comps)
{
    student_info *curr;
    unsigned long int block;
    student_info *table = *hass_table;

    block=hash(dead_entry->full_name.last) % hass_info.numb_of_blocks;
    curr=pos_finder(&table[block], dead_entry->full_name.last, dead_entry->aem, comps);

    curr->prev->next=curr->next;
    curr->next->prev=curr->prev;
    curr->next=NULL;
    curr->prev=NULL;

    table[block].failed_classes--; //here we count the size of the block
}

//FIND BY NAME
int find_by_name(student_info **hass_table, char *last_name, Hass_info hass_info, int *comps)
{
    student_info *curr;
    unsigned long int block;
    student_info *table = *hass_table;
    (*comps)=0;

    block=hash(last_name) % hass_info.numb_of_blocks;
    curr=pos_finder(&table[block], last_name, 0, comps);  //curr points to one before the first student with that name 

    curr=curr->next; //now it points to the fisrt student with that name

    //printf("\nlast: %s\n block: %d\n", last_name, block);

    (*comps)++;//if
    if(strcmp(curr->full_name.last, last_name))
    {
        printf("\nN-NOK %s\n",last_name);
        return 0; //Not found
    }
    else
    {
        printf("\nN-OK %s\n",last_name);

        
        while(!(strcmp(curr->full_name.last, last_name)))
        {
            printf("%s %lu %d\n", curr->full_name.first, curr->aem, curr->failed_classes);
            curr=curr->next;
            (*comps)++;//while_success
        }
        (*comps)++;//while_fail

        return 1; //success
    }
}


//PRINT BLOCK
void print_block(student_info *block)
{
    student_info *curr;

    for(curr=block->next; curr->aem!=0; curr=curr->next)
    {
        printf(" [ %lu %s %s %d ]",curr->aem, curr->full_name.first, curr->full_name.last, curr->failed_classes);
    }
}


//PRINT BY NAME 
void print_by_name(student_info **hass_table, Hass_info *Hass_info)
{
    int i;
    double load_factor = (double)Hass_info->numb_of_entries / (double)Hass_info->numb_of_blocks;
    int largest_bucket=(*hass_table)[0].failed_classes;

    for(i=1; i<Hass_info->numb_of_blocks; i++)
    {
        if(largest_bucket<(*hass_table)[i].failed_classes)
            largest_bucket=(*hass_table)[i].failed_classes;
    }

    printf("\n##\n");
    printf("%d %d %.2f %d", Hass_info->numb_of_blocks, Hass_info->numb_of_entries, load_factor, largest_bucket);


    for(i=0; i<Hass_info->numb_of_blocks; i++)
    {
        //printf("\n");
        printf("\n%d %d", i, (*hass_table)[i].failed_classes);
        print_block(&(*hass_table)[i]);
        printf("\n");
    }
    //printf("\n");
}


//REHASS
int rehass(student_info **hass_table, Hass_info *hass_info, int taken_spots, student_info **ptrs,Hass_info *Hass_info, int *comps)
{
    student_info *new_hass;
    int i;
    double load_factor = (double)Hass_info->numb_of_entries / (double)Hass_info->numb_of_blocks;

    if(load_factor>=HIGH_LOAD)
    {
        new_hass=(student_info*)malloc(sizeof(student_info)*2*hass_info->numb_of_blocks);
        if(new_hass==NULL)
            return -2; //No Memory

        (*hass_info).numb_of_blocks=(*hass_info).numb_of_blocks*2;
        
        hass_init(&new_hass, *hass_info);

        for(i=0; i<taken_spots; i++)
            hass_adder(&new_hass, ptrs[i], *hass_info, comps);

        free(*hass_table);


        *hass_table=new_hass;
    } 
    else if(load_factor<=LOW_LOAD && (hass_info->numb_of_blocks/2)>=hass_info->k)
    {
        new_hass=(student_info*)malloc(sizeof(student_info)*0.5*hass_info->numb_of_blocks);
        if(new_hass==NULL)
            return -2; //No Memory

        (*hass_info).numb_of_blocks=(*hass_info).numb_of_blocks*0.5;

        hass_init(&new_hass, *hass_info);

        for(i=0; i<taken_spots; i++)
            hass_adder(&new_hass, ptrs[i], *hass_info, comps);

        free(*hass_table);

        *hass_table=new_hass;
    }
    //else
      //  printf("\nrehassNOT");
    
    return 0;
}


//MIXED FUNCTIONS

//ADD
int add(student_info ***ptrs, unsigned long int aem, char *first, char *last, unsigned short int failed, int *size,
 int K,int *taken_spots, int sorted, int *comps, Hass_info *hass_info, student_info **hass_table)
{
    student_info **expansion;
    student_info *find_result;

    if(sorted)
        find_result=binary_search(*ptrs, aem, *taken_spots, comps);
    else
        find_result=find(*ptrs, aem, *taken_spots, comps);


    if(!(find_result))
    {
	    if(*size>*taken_spots) //free spots exist
	    {
            //creation of a struct but not of a pointer
            (*ptrs)[*taken_spots]=(student_info*)malloc((sizeof(student_info)));
            if((*ptrs)[*taken_spots]==NULL)
				return 0; //No Memory
            
            //addition
	        (*ptrs)[*taken_spots]->aem = aem;
	        (*ptrs)[*taken_spots]->failed_classes = failed;
	        strcpy((*ptrs)[*taken_spots]->full_name.first, first);
		    strcpy((*ptrs)[*taken_spots]->full_name.last, last);
            (*ptrs)[*taken_spots]->head=NULL;

            hass_adder(hass_table, (*ptrs)[*taken_spots], *hass_info, comps);
            (*taken_spots)++;
            (*hass_info).numb_of_entries=(*taken_spots);
            (*hass_info).load_factor=(*hass_info).numb_of_entries/(*hass_info).numb_of_blocks;
            //printf("\nload_factor = %d \nnumb_of_blocks = %d\nnumb_of_entries = %d",hass_info->load_factor, hass_info->numb_of_blocks, hass_info->numb_of_entries);

	    }
	    else //Space Expansion Needed
	    {
            //creation of K pointers
	    	expansion = (student_info**)realloc((*ptrs),(sizeof(student_info*))*(*size + K));
	        if(expansion==NULL)
	           return 0; //No Memory

            *ptrs = expansion;

            //Creation of a struct
            (*ptrs)[*taken_spots]=(student_info*)malloc((sizeof(student_info)));
            if((*ptrs)[*taken_spots]==NULL)
				return 0; //No Memory

		    (*size)=(*size)+K;
            initialize(ptrs, *size, NAMESIZE, *taken_spots+1);
		    
		    (*ptrs)[*taken_spots]->aem = aem;
		    (*ptrs)[*taken_spots]->failed_classes = failed;
		    strcpy((*ptrs)[*taken_spots]->full_name.first, first);
		    strcpy((*ptrs)[*taken_spots]->full_name.last, last);
            (*ptrs)[*taken_spots]->head=NULL;

            hass_adder(hass_table, (*ptrs)[*taken_spots], *hass_info, comps);

            (*taken_spots)++;
            (*hass_info).numb_of_entries=(*taken_spots);
            (*hass_info).load_factor=(*hass_info).numb_of_entries/(*hass_info).numb_of_blocks;
            //printf("\nload_factor = %d \nnumb_of_blocks = %d\nnumb_of_entries = %d",hass_info->load_factor, hass_info->numb_of_blocks, hass_info->numb_of_entries);
		  
	    }
        rehass(hass_table, hass_info, *taken_spots, *ptrs, hass_info, comps);

        return 1;

    }
	return 0; //Fail
}


//CLEAR
void clear(student_info ***ptrs, int *size, int *taken_spots, student_info **hass_table, Hass_info *hass_info)
{
    int i;
    classes_node *head=NULL;

    if(*size>0)
    {
        for(i=0; i<(*taken_spots); i++)
        {
            head=NULL;
            if((*ptrs)[i]->head!=NULL)
                head=(*ptrs)[i]->head;

            while(head!=NULL)
            {
                delete_first(&head);
                
                
            }
            free((*ptrs)[i]);
        }    

        (*size)=(*taken_spots)=0;
    }

    //hass things
    free(*hass_table);

    *hass_table=(student_info*)malloc(sizeof(student_info)*hass_info->k);
    hass_info->numb_of_blocks=hass_info->k;
    hass_init(hass_table, *hass_info);

    hass_info->numb_of_entries=0;
    hass_info->load_factor=0;
}



//QUIT
void quit(student_info ***ptrs, int *size, int *taken_spots, student_info **hass_table, Hass_info *hass_info)
{
    clear(ptrs, size, taken_spots, hass_table, hass_info);

    free(*hass_table);

    free((*ptrs));
}



//REMOVE
int rmv(student_info ***ptrs, int *size, int *taken_spots,unsigned long int aem, int sorted,int *comps, int K, Hass_info *hass_info, student_info **hass_table)
{

    student_info *find_result;
    int block;

    if(sorted)
        find_result=binary_search(*ptrs, aem, *taken_spots, comps);
    else
        find_result=find(*ptrs, aem, *taken_spots, comps);


     if(find_result)
    {
        if(((*taken_spots)!=1) && (*ptrs)[(*taken_spots)-1]->aem!=find_result->aem)
        {
            //logistics
            //the ruduction must take place in the block of the one being deleted 
            block=hash(find_result->full_name.last) % hass_info->numb_of_blocks;
            (*hass_table)[block].failed_classes--;

            //copy the last structs data in the place of the one being deleted
            find_result->aem=(*ptrs)[(*taken_spots)-1]->aem; 
            find_result->failed_classes=(*ptrs)[(*taken_spots)-1]->failed_classes;
            strcpy(find_result->full_name.first, (*ptrs)[(*taken_spots)-1]->full_name.first);
            strcpy(find_result->full_name.last, (*ptrs)[(*taken_spots)-1]->full_name.last);

            
            //classes list delete
            while(find_result->head!=NULL)
            {
                delete_first(&find_result->head);
            }
            
            find_result->head=(*ptrs)[(*taken_spots-1)]->head;

            //hass pointers
            //connect the previous and the next object of the find result
            find_result->prev->next=find_result->next;
            find_result->next->prev=find_result->prev;
            find_result->next=NULL;
            find_result->prev=NULL;
            

            //connect the find result to the last's list
            find_result->next=(*ptrs)[(*taken_spots)-1]->next;
            find_result->prev=(*ptrs)[(*taken_spots)-1]->prev;
            (*ptrs)[(*taken_spots)-1]->prev->next=find_result;
            (*ptrs)[(*taken_spots)-1]->next->prev=find_result;

            //NULL
            (*ptrs)[(*taken_spots)-1]->next=NULL;
            (*ptrs)[(*taken_spots)-1]->prev=NULL;



        }
        else
        {
            //classes list delete
            while(find_result->head!=NULL)
            {
                delete_first(&find_result->head);
            }
            
            //logistics
            //the ruduction must take place in the block of the one being deleted 
            block=hash(find_result->full_name.last) % hass_info->numb_of_blocks;
            (*hass_table)[block].failed_classes--;


            (*ptrs)[(*taken_spots)-1]->prev->next=(*ptrs)[(*taken_spots)-1]->next;
            (*ptrs)[(*taken_spots)-1]->next->prev=(*ptrs)[(*taken_spots)-1]->prev;

            //NULL
            (*ptrs)[(*taken_spots)-1]->next=NULL;
            (*ptrs)[(*taken_spots)-1]->prev=NULL;


        }
        
        free((*ptrs)[(*taken_spots)-1]); //free the space of the last struct
        (*ptrs)[(*taken_spots)-1]=NULL; //pointer to NULL 
        
        if((*size)-(*taken_spots)>=(K-1) && *size - K != 0) //if the array positions are more than k reduction is needed
        {
            (*ptrs)=(student_info**)realloc((*ptrs), sizeof(student_info*)*((*size)-K)); //ptrs reduction
            
            if((*ptrs)==NULL)
            {
                perror("");
                return 0; //Fail
            }
            (*size)=(*size)-K; //logistics
        }

        (*taken_spots)--;
        (*hass_info).numb_of_entries=(*taken_spots);
        (*hass_info).load_factor=(*hass_info).numb_of_entries/(*hass_info).numb_of_blocks;
        
        rehass(hass_table, hass_info, *taken_spots, *ptrs, hass_info, comps);


        return 1; //Success
    }

    return 0; //Fail
}





//MAIN
int main(int argc, char *argv[]) 
{
    //variables
    unsigned long int aem=0;
    unsigned short int failed_classes=0;
    char first[NAMESIZE]={'\0'};
    char last[NAMESIZE]={'\0'};
    char choise;
    int number_of_ptrs, K;
    student_info **ptrs;
    int i, taken_spots = 0, size=0;
    student_info *find_result;
    int sorted=0;
    int comps=0;
    char helping_string[200]={'\0'};
    int flag=0;
    unsigned short int class_id;
    student_info *hass_table;
    Hass_info hass_info;
    int reg_result;



    //Comand Line
    if(argc!=4)
        return 42;
    

    number_of_ptrs=atoi(argv[1]);
    K=atoi(argv[2]);
    hass_info.k=atoi(argv[3]);



    //Memory

    //student table
    ptrs=(student_info**)malloc(sizeof(student_info*)*(number_of_ptrs));
    if(ptrs==NULL)
        return 43; //No memory 

    size=number_of_ptrs;

    initialize(&ptrs, size, NAMESIZE, 0);

    //Hass table
    hass_table=(student_info*)malloc(sizeof(student_info)*hass_info.k); //creation of the hass table
    if(hass_table==NULL)
        return 43; //No memory

    hass_info.numb_of_blocks=hass_info.k;

    hass_init(&hass_table, hass_info);

    //for(i=0; i<hass_info.numb_of_blocks; i++)
      //  printf("%lu  %hu  %s  %p  %p  %p\n",hass_table[i].aem, hass_table[i].failed_classes, hass_table[i].full_name.first, hass_table[i].next, hass_table[i].prev, &hass_table[i]);



  
    //MENU
    while(1)
	{
	    scanf(" %c", &choise);

        switch (choise)
        {

            case ('a'):
                
                
                scanf(" %lu", &aem);
                scanf(" %s",helping_string);

                if(strlen(helping_string)>=NAMESIZE)
                    flag=1;

                for(i=0; i<NAMESIZE-1; i++)
                    first[i]=helping_string[i];

                for(i=0; i<50; i++)
                    helping_string[i]='\0';

    
                scanf(" %s",helping_string);

                if(strlen(helping_string)>=NAMESIZE)
                    flag=1;

                for(i=0; i<NAMESIZE-1; i++)
                    last[i]=helping_string[i];

                for(i=0; i<50; i++)
                    helping_string[i]='\0';

                if(flag)
                {
                    printf("\nTrimmed name(s).\n");
                    flag=0;
                }
                    

                scanf(" %hu",&failed_classes);

                if(aem!=0 && failed_classes!=0)
                {
                
                    for(i=0; i<(NAMESIZE-1); i++)
                    {
                        if(islower(first[i]))
                            first[i]=toupper(first[i]);

                        if(islower(last[i]))
                            last[i]=toupper(last[i]);
                    }

                    if(add(&ptrs, aem, first, last, failed_classes, &size, K, &taken_spots, sorted, &comps, &hass_info, &hass_table))
                    {
                        printf("\nA-OK %lu, %d %d\n", ptrs[taken_spots-1]->aem, taken_spots, size);
                        sorted=0;
                    }
                    else
                        printf("\nA-NOK %lu, %d %d\n", aem, taken_spots, size);
                }
                else
                {
                    printf("\nInvalid input.\n");
                    for(i=0; i<NAMESIZE; i++)
                    {
                        first[i]='\0';
                        last[i]='\0';
                    }
                    failed_classes=0;
                }

                    break;


            case ('r'):
                scanf("%lu", &aem);

                if(rmv(&ptrs, &size, &taken_spots, aem, sorted, &comps, K, &hass_info, &hass_table))
                {
                    printf("\nR-OK %lu, %d %d\n", aem, taken_spots, size);
                    sorted=0;
                }
                else
                    printf("\nR-NOK %lu, %d %d\n", aem, taken_spots, size);

                break;


            case ('m'):

                scanf("%lu %hu", &aem, &failed_classes);
                if(failed_classes==0)
                    printf("\nInvalid input.\n");
                else
                {
                    if(mod(&ptrs, aem, failed_classes, size, sorted, &comps, taken_spots))
                        printf("\nM-OK %lu\n", aem);
                    else
                        printf("\nM-NOK %lu\n", aem);
                }
                break;
    

            case ('s'):
                insertion_sort(&ptrs, taken_spots, &comps);
                printf("\nS-OK\n");
                fprintf( stderr, "\n$%d\n", comps);
                sorted=1;
                
                break;
    


            case ('f'):
                scanf(" %lu", &aem);
                if(sorted)
                    find_result=binary_search(ptrs, aem, taken_spots, &comps);
                else
                    find_result=find(ptrs, aem, taken_spots, &comps);

                if(find_result)
                    printf("\nF-OK %s %s %hu\n",find_result->full_name.first, find_result->full_name.last, find_result->failed_classes);
                else
                    printf("\nF-NOK %lu\n", aem);

                fprintf( stderr, "\n$%d\n", comps);
                break;
    

            case ('p'):
                print(ptrs, taken_spots);
                break;
    

            case ('c'):
                clear(&ptrs, &size, &taken_spots, &hass_table, &hass_info);
                printf("\nC-OK\n");
                sorted=0;
                break;
    

            case ('q'):
                quit(&ptrs, &size, &taken_spots, &hass_table, &hass_info);
                return 0;
                break;

            //Classes list choises

            case('g'):
                scanf("%lu %hu",&aem, &class_id);

                reg_result=reg(&ptrs, aem, class_id, sorted, taken_spots, &comps);

                if(reg_result==1)
                    printf("\nG-OK %lu %d\n", aem, class_id);
                else if(reg_result==0)
                    printf("\nG-NOK %lu\n", aem);
                else if(reg_result==-1)
                    printf("\nG-NOK %d\n", class_id);
                else if(reg_result==-2)
                    printf("\nG-NOK MEM\n");
                break;


            case('u'):
                scanf(" %lu %hu",&aem, &class_id);

                reg_result=unreg(&ptrs, aem, class_id, sorted, taken_spots, &comps);

                if(reg_result==1)
                    printf("\nU-OK %lu %d\n", aem, class_id);
                else if(reg_result==0)
                    printf("\nU-NOK %lu\n", aem);
                else if(reg_result==-1)
                    printf("\nU-NOK %hu\n", class_id);

                break;


            case('i'):
                scanf(" %lu %hu",&aem, &class_id);

                //Find the student
                if(sorted)
                    find_result=binary_search(ptrs, aem, taken_spots, &comps);
                else
                    find_result=find(ptrs, aem, taken_spots, &comps);

                if(find_result)
                {
                    isreg(&find_result->head, class_id, &flag);

                    if(flag==1)
                        printf("\nYES\n");
                    else
                        printf("\nNO\n");

                    flag=0;
                }
                else
                    printf("\nI-NOK %lu\n", aem);
                    
                break;


            case('l'):
                scanf(" %lu", &aem);
                list_courses(&ptrs, sorted, taken_spots, &comps, aem);
                break;


            case('t'):
/*                for(i=0; i<hass_info.numb_of_blocks; i++)
                {
                    printf("%d :", i);
                    print_block(&hass_table[i]);
                    printf("\n");
                }
*/
                print_by_name(&hass_table, &hass_info);
                printf("\n");
                break;


            case('n'):
                for(i=0; i<50; i++)
                    helping_string[i]='\0';

    
                scanf(" %s",helping_string);

                if(strlen(helping_string)>=NAMESIZE)
                    flag=1;

                for(i=0; i<NAMESIZE-1; i++)
                    last[i]=helping_string[i];

                for(i=0; i<50; i++)
                    helping_string[i]='\0';

                if(flag)
                    flag=0;

                for(i=0; i<NAMESIZE-1; i++)
                    if(islower(last[i]))
                        last[i]=toupper(last[i]);

                find_by_name(&hass_table, last, hass_info, &comps);

                fprintf(stderr, "\n$%d\n", comps);

                break;
    
    
            default:
                if(choise=='\n')
                    continue;

                printf("\nInvalid choise.\n");
				continue;
        }
    }
    return 0;

}




