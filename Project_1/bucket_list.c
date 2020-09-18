#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bucket.h" 
#include "max_heap.h"

extern int counter;

//Create an empty bucket
bucket_ptr Create_Bucket(int bucket_size)
{
    bucket_ptr lptr = (bucket_ptr)malloc(sizeof(struct Bucket));
    lptr->name = (bucket_entry*)malloc(((bucket_size-12)/8)*sizeof(bucket_entry));
    for(int i=0;i<(bucket_size-12)/8;i++)
    {
        lptr->name[i] = NULL;
    }
    lptr->entry_counter = 0;
    lptr->next = NULL;
    return lptr;
}

//Insert a bucket record in the Bucket.
void Insert_Bucket_Entry(bucket_ptr head, rec_ptr rec_node, char* name, char* date, int bucket_size)
{
    int max_entries = (bucket_size-12)/8;
    
    bucket_ptr current = head;
    int i;

    //Find the next available position in the bucket array in the chain of buckets and put it there.
    while(current->next != NULL)
    {
        //Check if the disease name is already exists.
        for(i=0; i<max_entries; i++)
        {
            //If there is the name, then only insert the entry date.
            if(strcmp(current->name[i]->name,name) == 0)
            {
                current->name[i]->avl_tree_root = AVL_Insert(current->name[i]->avl_tree_root, rec_node, date);
                return;
            }
        } 
        current = current->next;
    }

    //If there isn't the name in the bucket list or there is only one bucket in the list,
    //check if there is in the last bucket of the list.
    //If not, then check if there is space in the bucket, otherwise create and insert a new bucket to the bucket list.
    for(i=0; i<current->entry_counter; i++)
    {
        //If there is the name, then only insert the entry date.
        if(strcmp(current->name[i]->name,name) == 0)
        {
            current->name[i]->avl_tree_root = AVL_Insert(current->name[i]->avl_tree_root, rec_node, date);
            return;
        }
    }

    if(current->entry_counter < max_entries)
    {
        current->name[current->entry_counter] = (bucket_entry)malloc(sizeof(struct Bucket_Entry));
        current->name[current->entry_counter]->avl_tree_root = NULL;
        current->name[current->entry_counter]->name = (char*)malloc((strlen(name)+1)*sizeof(char));
        strcpy(current->name[current->entry_counter]->name,name);
        current->name[current->entry_counter]->avl_tree_root = AVL_Insert(current->name[current->entry_counter]->avl_tree_root,rec_node, date);
        current->entry_counter++;
    } 
    else
    {
        bucket_ptr new_bucket;
        new_bucket = Create_Bucket(bucket_size);
        new_bucket->entry_counter = 1;
        new_bucket->name[0] = (bucket_entry)malloc(sizeof(struct Bucket_Entry));
        new_bucket->name[0]->avl_tree_root = NULL;
        new_bucket->name[0]->name = (char*)malloc((strlen(name)+1)*sizeof(char));
        strcpy(new_bucket->name[0]->name, name);
        new_bucket->name[0]->avl_tree_root = AVL_Insert(new_bucket->name[0]->avl_tree_root,rec_node, date);
        current->next = new_bucket;
    }
}


//For /numCurrentPatients returns the number of outbreaks who are still hospitallized.
int Search_Disease_Entry(bucket_ptr head, char* disease)
{
    int i;
    if(disease == NULL)  //If the disease is given.
    {
        while(head != NULL)
        {            
            for(i=0;i<head->entry_counter;i++)
            {
                counter = 0;
                AVL_Print_Disease(head->name[i]->avl_tree_root);
                printf("%s %d\n",head->name[i]->name,counter);
            }
            head = head->next;
        }
    }
    else  //If the disease is not given.
    {
        while(head != NULL)
        {
            for(i=0;i<head->entry_counter;i++)
            {
                if(strcmp(head->name[i]->name, disease) == 0)
                {
                    counter = 0;
                    AVL_Disease_Search(head->name[i]->avl_tree_root);
                    printf("%s %d\n",disease,counter); 
                    return 1;
                }
            }
            head = head->next;
        }
    }
    return 0;
}

//Fod deallocate the bucket list.
void Destroy_Bucket_List(bucket_ptr* head)
{
    bucket_ptr current = *head;  
    bucket_ptr next; 
    int i; 
    
    while(current != NULL)  
    {  
        next = current->next;
        for(i=0; i<current->entry_counter; i++)
        {
            free(current->name[i]->name);
            Delete_AVL_Tree(current->name[i]->avl_tree_root);
            free(current->name[i]);
               
        }
        current->entry_counter = 0;
        free(current->name);    
        free(current);  
        current = next;  
    }  
    *head = NULL;
}

int Num_of_Outbreaks(bucket_ptr head, char* entry_date, char* exit_date, char* virusName, char* country)
{
    int i;
    //Case of /globalDiseaseStats:
    if(virusName == NULL)  
    {
        if(entry_date == NULL)  //If entry and exit date are not given.
        {
            while(head != NULL)
            {
                for(i=0;i<head->entry_counter;i++)
                {
                    counter = 0;
                    AVL_Search_Outbreaks(head->name[i]->avl_tree_root);
                    //printf("There are %d outbreaks who was diagnosed with the Disease of %s\n", counter,head->name[i]->name); 
                    printf("%s %d\n",head->name[i]->name,counter);
                }
                head = head->next;
            }
        }   
        else  //If entry and exit date are given.
        {
            while(head != NULL)
            {
                for(i=0;i<head->entry_counter;i++)
                {
                    counter = 0;
                    AVL_Search_Disease_Outbreaks(head->name[i]->avl_tree_root,entry_date,exit_date,country);
                    //printf("There are %d outbreaks who was diagnosed with the Disease of %s\n", counter,head->name[i]->name); 
                    printf("%s %d\n",head->name[i]->name,counter);
                }
                head = head->next;
            }        
        }
    }
    else  //Case of /diseaseFrequency:
    {
        while(head != NULL)
        {
            for(i=0;i<head->entry_counter;i++)
            {
                counter = 0;
                if(strcmp(head->name[i]->name,virusName) == 0)
                {
                    AVL_Search_Disease_Outbreaks(head->name[i]->avl_tree_root,entry_date,exit_date,country);
                    if(country == NULL) printf("%s %d\n", virusName,counter);//printf("There are %d outbreaks who was diagnosed with the Disease of %s\n", counter,virusName);
                    else printf("%s %d\n", virusName,counter);//printf("There are %d outbreaks who was diagnosed with the Disease of %s in country %s\n", counter,virusName,country);
                    return 1; 
                }
            }
            head = head->next;
        }
    }
    return 0;
}

//For /topk-Diseases.
void Topk_Diseases(bucket_ptr* disease_hash_table, int diseaseHashtableNumOfEntries, int k, char* country, char* entry_date, char* exit_date)
{
    int i,j;
    bucket_ptr head;
    max_heap_ptr heap = (max_heap_ptr)malloc(sizeof(struct Max_Heap));
    heap->root = NULL;
    heap->last = NULL;

    for(i=0;i<diseaseHashtableNumOfEntries;i++)
    {
        head = disease_hash_table[i];
    
        if(entry_date == NULL)
        {
            while(head != NULL)
            {
                for(j=0;j<head->entry_counter;j++)
                {
                    counter = 0;
                    AVL_Num_of_Outbreaks_Disease(head->name[j]->avl_tree_root,country);  //Returns the number of outbreaks for a given country.
                    Max_Heap_Insert(heap,counter,head->name[j]->name);  //Put the viruses with them outbreaks in the max heap.
                }
                head = head->next;
            }
        }   
        else
        {
            //Do the same with specific entry and exit date.
            while(head != NULL)
            {
                for(j=0;j<head->entry_counter;j++)
                {
                    counter = 0;
                    AVL_Search_Disease_Outbreaks(head->name[j]->avl_tree_root,entry_date,exit_date,country);
                    Max_Heap_Insert(heap,counter,head->name[j]->name);
                }
                head = head->next;
            }        
        }
    }

    Top_k(heap,k);  //Take the top k viruses for the specific country.

    //Deallocate the memory of max heap.
    if(heap->root != NULL)
    {
        Deallocate_Max_Heap(heap->root);
    }

    free(heap);
}

//Do the same for the Top k countries as the top k diseases.
void Topk_Countries(bucket_ptr* country_hash_table, int countryHashtableNumOfEntries, int k, char* disease, char* entry_date, char* exit_date)
{
    int i,j;
    bucket_ptr head;
    max_heap_ptr heap = (max_heap_ptr)malloc(sizeof(struct Max_Heap));
    heap->root = NULL;
    heap->last = NULL;

    for(i=0;i<countryHashtableNumOfEntries;i++)
    {
        head = country_hash_table[i];
    
        if(entry_date == NULL)
        {
            while(head != NULL)
            {
                for(j=0;j<head->entry_counter;j++)
                {
                    counter = 0;
                    AVL_Num_of_Outbreaks_Countries(head->name[j]->avl_tree_root,disease);
                    Max_Heap_Insert(heap,counter,head->name[j]->name);
                }
                head = head->next;
            }
        }   
        else
        {
            while(head != NULL)
            {
                for(j=0;j<head->entry_counter;j++)
                {
                    counter = 0;
                    AVL_Search_Country_Outbreaks(head->name[j]->avl_tree_root,entry_date,exit_date,disease);
                    Max_Heap_Insert(heap,counter,head->name[j]->name);
                }
                head = head->next;
            }        
        }
    }

    Top_k(heap,k);

    if(heap->root != NULL)
    {
        Deallocate_Max_Heap(heap->root);
    }

    free(heap);
}

void Print_Bucket(bucket_ptr head)
{
    while(head!=NULL)
    {
        for(int i=0;i<head->entry_counter;i++)
        {
            printf("%s \n",head->name[i]->name);
            inOrder(head->name[i]->avl_tree_root);
        }
        head = head->next;
    }
}

int Bucket_List_Size(bucket_ptr head)
{
    int size = 0;
    while(head!=NULL)
    {
        size++;
        head = head->next;
    }
    return size;
}

void Print_Hash_Tables(int diseaseHashtableNumOfEntries, int countryHashtableNumOfEntries, bucket_ptr* diseaseHashTable, bucket_ptr* countryHashTable)
{
    printf("//////////////////////////////////////\n");
    for(int j=0;j<diseaseHashtableNumOfEntries;j++) 
    {
        printf("------------------------------\nBucket %d has %d Bnodes\n", j, Bucket_List_Size(diseaseHashTable[j]));
        Print_Bucket(diseaseHashTable[j]);
    }
    printf("//////////////////////////////////////\n");
    for(int j=0;j<countryHashtableNumOfEntries;j++) 
    {
        printf("------------------------------\nBucket %d has %d Bnodes\n",j, Bucket_List_Size(countryHashTable[j]));
        Print_Bucket(countryHashTable[j]);
    }
    printf("//////////////////////////////////////\n");
}