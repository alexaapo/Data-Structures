#include <iostream>
#include <fstream> 
#include <string>
#include <bits/stdc++.h>
#include "hash.h"
#include "bucket.h"
#include "avl.h"

using namespace std;

int main(int argc, char** argv)
{
    rec_ptr head = NULL;
    int i;

    if (argc < 9 || argc > 9)
    {
        printf("Invalid Input. PLease give 9 arguments\n");
        return(1);
    }
    else
    {
        int diseaseHashtableNumOfEntries, countryHashtableNumOfEntries;
        char* recid, *fname, *lname, *dis, *coun, *endate, *exdate;
        int i,flag=0, bucket_size;
        string rec_info[7], patientrecordsfile;
        
        //In case we give the arguments with wrong sequence.
        for(i=1;i<=7;i+=2)
        {
	        if(strcmp(argv[i],"-p") == 0)
                patientrecordsfile = (argv[i+1]);
            else if(strcmp(argv[i],"-h1") == 0)
                diseaseHashtableNumOfEntries = atoi(argv[i+1]);
            else if(strcmp(argv[i],"-h2") == 0)
                countryHashtableNumOfEntries = atoi(argv[i+1]);
            else if(strcmp(argv[i],"-b") == 0)
                bucket_size = atoi(argv[i+1]);
            else 
            {   
                printf("Please give an input with this form: ./diseaseMonitor -p patientRecordsFile –h1 diseaseHashtableNumOfEntries –h2 countryHashtableNumOfEntries –b bucketSize\n");
                exit(1);
            }			
        }

        if(bucket_size < 20)
        {
            printf("Bucket size must be at least 20 bytes. Please try again.\n");
            exit(1);
        }

        ifstream patientRecordsFile(patientrecordsfile);
        bucket_ptr disease_array[diseaseHashtableNumOfEntries];
        bucket_ptr country_array[countryHashtableNumOfEntries];

        //Initialize the arrays of buckets.
        for(i=0; i < diseaseHashtableNumOfEntries; i++)
            disease_array[i] = NULL;
        
        for(i=0; i < countryHashtableNumOfEntries; i++)
            country_array[i] = NULL;
        
        //Open the file and get the records.
        if(patientRecordsFile.is_open() && patientRecordsFile.good()) 
        {
            string line = "";
            //Get line by line the records from .txt file.
            while(getline(patientRecordsFile, line))
            {
                stringstream iss(line);
                i=0;
                flag = 0;
                
                do { 
                    //Read a word 
                    string word; 
                    iss >> word; 
                    if(i<=6)  rec_info[i] = word;
                    else if(i>6 && word.length()>0)  flag = 1;
                    i++;
                } while (iss); 


                if(flag)  printf("Arguments > 7, insert exactly 7 arguments!\n");
                else if(i<8) printf("Arguments < 7, insert exactly 7 arguments!\n");
                else
                {
                    //Convert the string to char* to pass as parametre to other functions.
                    recid = new char[rec_info[0].length()+1];
                    strcpy(recid,rec_info[0].c_str());
                    fname = new char[rec_info[1].length()+1];
                    strcpy(fname,rec_info[1].c_str());
                    lname = new char[rec_info[2].length()+1];
                    strcpy(lname,rec_info[2].c_str());
                    dis = new char[rec_info[3].length()+1];
                    strcpy(dis,rec_info[3].c_str());
                    coun = new char[rec_info[4].length()+1];
                    strcpy(coun,rec_info[4].c_str());
                    endate = new char[rec_info[5].length()+1];
                    strcpy(endate,rec_info[5].c_str());
                    exdate = new char[rec_info[6].length()+1];
                    strcpy(exdate,rec_info[6].c_str());

                    //Check if the Record is valid
                    int error = Check_Validity_of_Record(&head,recid,fname,lname,dis,coun,endate,exdate);
                    if(error == 1)
                    {
                        printf("Record ID is already exists!\n");
                        exit(1);
                    }
                    if(error!=0)
                        Print_Error(error); 
                    else //If the records are valid put them in the records list.
                    {
                        rec_ptr record_node;
                        int hash, hash1;   

                        record_node = Insert_Record(&head,recid,fname,lname,dis,coun,endate,exdate);

                        hash = Hash_Function(dis, diseaseHashtableNumOfEntries);
                        hash1 = Hash_Function(coun, countryHashtableNumOfEntries);

                        //For the Hash table of Diseases
                        //Case 1: There is not any created bucket yet.
                        if(disease_array[hash] == NULL)
                        {
                            bucket_ptr new_bucket;

                            new_bucket = Create_Bucket(bucket_size);
                            
                            if(new_bucket != NULL) disease_array[hash] = new_bucket;
                            else printf("Problem with memory!\n");

                            Insert_Bucket_Entry(new_bucket,record_node,dis,endate,bucket_size);
                        }
                        else //Case 2: If there at least one bucket.
                        {
                            bucket_ptr current = disease_array[hash];
                            //Insert the disease to the apropriate bucket.
                            Insert_Bucket_Entry(current,record_node,dis,endate,bucket_size);
                        }

                        //For the Hash table of Countries
                        //Case 1: There is not any created bucket yet.
                        if(country_array[hash1] == NULL)
                        {
                            bucket_ptr new_bucket;

                            new_bucket = Create_Bucket(bucket_size);
                            
                            if(new_bucket != NULL) country_array[hash1] = new_bucket;
                            else printf("Problem with memory!\n");

                            Insert_Bucket_Entry(new_bucket,record_node,coun,endate,bucket_size);
                        }
                        else //Case 2: If there at least one bucket.
                        {
                            bucket_ptr current = country_array[hash1];
                            //Insert the disease to the apropriate bucket.
                            Insert_Bucket_Entry(current,record_node,coun,endate,bucket_size);
                        }                       
                    }
                    
                    delete [] recid;
                    delete [] fname;
                    delete [] lname;
                    delete [] dis;
                    delete [] coun;
                    delete [] endate;
                    delete [] exdate;
                }
            }
            patientRecordsFile.close();  //Close the file.
        }
        else 
            cout << "Failed to open file\n";

        //Begin the application of diseaseMonitor
        while(1)
        {
            cout << "Please give an input:\n";

            string input, user_input[8];
            getline(cin,input);
            stringstream iss(input);
            i=0;
            flag=0;
        
            do 
            { 
                // Read a word 
                string word; 
                iss >> word; 
                if(i<=7)  user_input[i] = word;
                else if(i>7 && word.length()>0)  flag = 1;
                i++;
            } while (iss);
            if(user_input[0] == "/insertPatientRecord")
            {
                int error;

                if(i>9)  printf("Arguments > 8, insert exactly 7 or 8 arguments!\n");
                else if(i<8) printf("Arguments < 7, insert exactly 7 or 8 arguments!\n");
                else
                {  
                    //Convert the string to char* to pass as parametre to other functions.
                    recid = new char[user_input[1].length()+1];
                    strcpy(recid,user_input[1].c_str());
                    fname = new char[user_input[2].length()+1];
                    strcpy(fname,user_input[2].c_str());
                    lname = new char[user_input[3].length()+1];
                    strcpy(lname,user_input[3].c_str());
                    dis = new char[user_input[4].length()+1];
                    strcpy(dis,user_input[4].c_str());
                    coun = new char[user_input[5].length()+1];
                    strcpy(coun,user_input[5].c_str());
                    endate = new char[user_input[6].length()+1];
                    strcpy(endate,user_input[6].c_str());

                    //Check if the Record is valid
                    if(user_input[7] == "")
                    {
                        exdate = new char[2];
                        strcpy(exdate,"-");
                    }
                    else 
                    {
                        exdate = new char[user_input[7].length()+1];
                        strcpy(exdate,user_input[7].c_str());
                    }
                    error = Check_Validity_of_Record(&head,recid,fname,lname,dis,coun,endate,exdate);
                    if(error == 1)
                    {
                        printf("Record ID is already exists!\n");
                        //exit(1);
                        flag = 2;
                    }
                    if(error!=0 && flag!=2)
                    {
                        flag = 3;
                        Print_Error(error); 
                    }
                    else 
                    {
                        rec_ptr record_node;
                        int hash, hash1;   

                        record_node = Insert_Record(&head,recid,fname,lname,dis,coun,endate,exdate);

                        hash = Hash_Function(dis, diseaseHashtableNumOfEntries);
                        hash1 = Hash_Function(coun, countryHashtableNumOfEntries);

                        //For the Hash table of Diseases
                        //Case 1: There is not any created bucket yet.
                        if(disease_array[hash] == NULL)
                        {
                            bucket_ptr new_bucket;

                            new_bucket = Create_Bucket(bucket_size);
                            
                            if(new_bucket != NULL) disease_array[hash] = new_bucket;
                            else printf("Problem with memory!\n");

                            //printf("%s name \n", record_node->diseaseID);

                            Insert_Bucket_Entry(new_bucket,record_node,dis,endate,bucket_size);
                        }
                        else //Case 2: If there at least one bucket.
                        {
                            bucket_ptr current = disease_array[hash];
                            //Insert the disease to the apropriate bucket.
                            Insert_Bucket_Entry(current,record_node,dis,endate,bucket_size);
                        }

                        //For the Hash table of Countries
                        //Case 1: There is not any created bucket yet.
                        if(country_array[hash1] == NULL)
                        {
                            bucket_ptr new_bucket;

                            new_bucket = Create_Bucket(bucket_size);
                            
                            if(new_bucket != NULL) country_array[hash1] = new_bucket;
                            else printf("Problem with memory!\n");

                            Insert_Bucket_Entry(new_bucket,record_node,coun,endate,bucket_size);
                        }
                        else //Case 2: If there at least one bucket.
                        {
                            bucket_ptr current = country_array[hash1];
                            //Insert the disease to the apropriate bucket.
                            Insert_Bucket_Entry(current,record_node,coun,endate,bucket_size);
                        }                       
                    }

                    delete [] recid;
                    delete [] fname;
                    delete [] lname;
                    delete [] dis;
                    delete [] coun;
                    delete [] endate;
                    delete [] exdate;

                    if(flag == 0)  
                        //cout << "Record has inserted successfully!\n";
                        cout << "Record added\n";
                }
            }
            else if(user_input[0] == "/print")  Print_Hash_Tables(diseaseHashtableNumOfEntries,countryHashtableNumOfEntries,disease_array,country_array);
            else if(user_input[0] == "/globalDiseaseStats")
            {
                if(i > 4)  printf("Arguments must be at most 3, insert 1 or 3 arguments!\n");
                else if(i < 2)  printf("Arguments must be at least 1, insert 1 or 3 arguments!\n");
                else if(i == 3)  printf("Arguments must be 1 or 3, insert 1 or 3 arguments!\n");
                else
                {
                    char* entry_date = NULL, *exit_date = NULL;
                    int j, error=0;
                    
                    //If the dates are given check their validity.
                    if(user_input[1] != "")
                    {
                        entry_date = new char[user_input[1].length()+1];
                        strcpy(entry_date,user_input[1].c_str());

                        exit_date = new char[user_input[2].length()+1];
                        strcpy(exit_date,user_input[2].c_str());

                    
                        error = Invalid_Date(entry_date);
                        if(error != 0)  Print_Error(error);
                        else
                        {
                            if(user_input[2] != "-")
                            {
                                error = Invalid_Date(exit_date);
                                if(error != 0)  
                                {
                                    if(error == Invalid_Entry_Day)  Print_Error(Invalid_Exit_Day);
                                    if(error == Invalid_Entry_Month)  Print_Error(Invalid_Exit_Month);
                                    if(error == Invalid_Entry_Year)  Print_Error(Invalid_Exit_Year);
                                    if(error == Invalid_Entry_Date)  Print_Error(Invalid_Exit_Date);
                                } 

                                if(date_to_seconds(entry_date)>date_to_seconds(exit_date))
                                {
                                    error = Entry_Date_after_Exit_Date;
                                    Print_Error(error);
                                }
                            }
                        }
                    }
                    //If all goes well.
                    if(error == 0)
                    {
                        for(j=0;j<diseaseHashtableNumOfEntries;j++)
                        {
                            Num_of_Outbreaks(disease_array[j],entry_date,exit_date,NULL,NULL);
                        }
                    }
                        
                    delete [] entry_date;
                    delete [] exit_date;
                }
            }
            else if(user_input[0] == "/diseaseFrequency")
            {
                if(i > 6)  printf("Arguments must be at most 5, insert 4 or 5 arguments!\n");
                else if(i < 5)  printf("Arguments must be at least 4, insert 4 or 5 arguments!\n");
                else 
                {
                    char* entry_date, *exit_date, *virusName, *country = NULL;
                    int j, error=0;

                    virusName = new char[user_input[1].length()+1];
                    strcpy(virusName,user_input[1].c_str());

                    entry_date = new char[user_input[2].length()+1];
                    strcpy(entry_date,user_input[2].c_str());

                    exit_date = new char[user_input[3].length()+1];
                    strcpy(exit_date,user_input[3].c_str());  

                    error = Invalid_Date(entry_date);
                    if(error != 0)  Print_Error(error);
                    else
                    {
                        if(user_input[3] != "-")
                        {
                            error = Invalid_Date(exit_date);
                            if(error != 0)  
                            {
                                if(error == Invalid_Entry_Day)  Print_Error(Invalid_Exit_Day);
                                if(error == Invalid_Entry_Month)  Print_Error(Invalid_Exit_Month);
                                if(error == Invalid_Entry_Year)  Print_Error(Invalid_Exit_Year);
                                if(error == Invalid_Entry_Date)  Print_Error(Invalid_Exit_Date);
                            } 

                            if(date_to_seconds(entry_date)>date_to_seconds(exit_date))
                            {
                                error = Entry_Date_after_Exit_Date;
                                Print_Error(error);
                            }
                        }
                        
                        error = Invalid_Disease(virusName);
                        if(error != 0)  
                        {
                            if(error%2!=0)
                                Print_Error(Disease_ID_has_blanks);
                            else
                                Print_Error(Invalid_Disease_ID);
                        }
                    } 

                    //If country is given check its validity.
                    if(user_input[4] != "")
                    {
                        country = new char[user_input[4].length()+1];
                        strcpy(country,user_input[4].c_str()); 

                        error = Invalid_Name(country);
                        if(error != 0)  
                        {
                            if(error%2!=0)
                                Print_Error(Country_Name_has_blanks);
                            else
                                Print_Error(Invalid_Country_Name);
                        }
                    }

                    //If all goes well.
                    if(error == 0)
                    {
                        int a;
                        for(j=0;j<diseaseHashtableNumOfEntries;j++)
                        {
                            a = Num_of_Outbreaks(disease_array[j],entry_date,exit_date,virusName,country);
                            if(a == 1) break;
                            if(j == (diseaseHashtableNumOfEntries-1) && a == 0)
                                printf("There is no disease with name: %s.\n",virusName);
                        }
                    }
                        
                    delete [] entry_date;
                    delete [] exit_date;
                    delete [] virusName;
                    if(user_input[4] != "")  delete [] country;
                }

            }
            else if(user_input[0] == "/topk-Diseases")
            {
                if(i > 6)  printf("Arguments must be at most 5, insert 3 or 5 arguments!\n");
                else if(i < 4)  printf("Arguments must be at least 3, insert 3 or 5 arguments!\n");
                else if(i == 5)  printf("Arguments must be 3 or 5, insert 3 or 5 arguments!\n");
                else
                {
                    int k,error=0;
                    char*country,*entry_date=NULL,*exit_date=NULL;

                    k = stoi(user_input[1]);

                    country = new char[user_input[2].length()+1];
                    strcpy(country,user_input[2].c_str());

                    error = Invalid_Name(country);
                    if(error != 0)  
                    {
                        if(error%2!=0)
                            Print_Error(Country_Name_has_blanks);
                        else
                            Print_Error(Invalid_Country_Name);
                    }
                    else
                    {
                        //If country was given.
                        if(user_input[3] != "")
                        {
                            entry_date = new char[user_input[3].length()+1];
                            strcpy(entry_date,user_input[3].c_str());

                            exit_date = new char[user_input[4].length()+1];
                            strcpy(exit_date,user_input[4].c_str());

                            error = Invalid_Date(entry_date);
                            if(error != 0)  Print_Error(error);
                            else
                            {
                                if(user_input[4] != "-")
                                {
                                    error = Invalid_Date(exit_date);
                                    if(error != 0)  
                                    {
                                        if(error == Invalid_Entry_Day)  Print_Error(Invalid_Exit_Day);
                                        if(error == Invalid_Entry_Month)  Print_Error(Invalid_Exit_Month);
                                        if(error == Invalid_Entry_Year)  Print_Error(Invalid_Exit_Year);
                                        if(error == Invalid_Entry_Date)  Print_Error(Invalid_Exit_Date);
                                    } 

                                    if(date_to_seconds(entry_date)>date_to_seconds(exit_date))
                                    {
                                        error = Entry_Date_after_Exit_Date;
                                        Print_Error(error);
                                    }
                                }
                            }
                        }

                        if(error == 0)
                            Topk_Diseases(disease_array, diseaseHashtableNumOfEntries, k, country, entry_date, exit_date);
                    }
                    delete [] country;
                    if(entry_date != NULL)  delete [] entry_date;
                    if(exit_date != NULL)  delete [] exit_date;
                }

            }
            else if(user_input[0] == "/topk-Countries")
            {
                if(i > 6)  printf("Arguments must be at most 5, insert 3 or 5 arguments!\n");
                else if(i < 4)  printf("Arguments must be at least 3, insert 3 or 5 arguments!\n");
                else if(i == 5)  printf("Arguments must be 3 or 5, insert 3 or 5 arguments!\n");
                else
                {
                    int k,error=0;
                    char* disease,*entry_date=NULL,*exit_date=NULL;

                    k = stoi(user_input[1]);

                    disease = new char[user_input[2].length()+1];
                    strcpy(disease,user_input[2].c_str());

                    error = Invalid_Disease(disease);
                    if(error != 0)  
                    {
                        if(error%2!=0)
                            Print_Error(Disease_ID_has_blanks);
                        else
                            Print_Error(Invalid_Disease_ID);
                    }
                    else
                    {
                        //If disease was given.
                        if(user_input[3] != "")
                        {
                            entry_date = new char[user_input[3].length()+1];
                            strcpy(entry_date,user_input[3].c_str());

                            exit_date = new char[user_input[4].length()+1];
                            strcpy(exit_date,user_input[4].c_str());

                            error = Invalid_Date(entry_date);
                            if(error != 0)  Print_Error(error);
                            else
                            {
                                if(user_input[4] != "-")
                                {
                                    error = Invalid_Date(exit_date);
                                    if(error != 0)  
                                    {
                                        if(error == Invalid_Entry_Day)  Print_Error(Invalid_Exit_Day);
                                        if(error == Invalid_Entry_Month)  Print_Error(Invalid_Exit_Month);
                                        if(error == Invalid_Entry_Year)  Print_Error(Invalid_Exit_Year);
                                        if(error == Invalid_Entry_Date)  Print_Error(Invalid_Exit_Date);
                                    } 

                                    if(date_to_seconds(entry_date)>date_to_seconds(exit_date))
                                    {
                                        error = Entry_Date_after_Exit_Date;
                                        Print_Error(error);
                                    }
                                }
                            }
                        }

                        if(error == 0)
                            Topk_Countries(country_array, countryHashtableNumOfEntries, k, disease, entry_date, exit_date);
                    }
                    delete [] disease;
                    if(entry_date != NULL)  delete [] entry_date;
                    if(exit_date != NULL)  delete [] exit_date;
                }
            }
            else if(user_input[0] == "/recordPatientExit")
            {
                int error;

                if(i != 4)  printf("Arguments <> 3, insert exactly 3 arguments!\n");
                else 
                {
                    char* rec_id, *exit_date;
                    
                    exit_date = new char[user_input[2].length()+1];
                    strcpy(exit_date,user_input[2].c_str());

                    rec_id = new char[user_input[1].length()+1];
                    strcpy(rec_id,user_input[1].c_str());

                    error = Search_ID(head,rec_id,exit_date);

                    if( error != 0)  Print_Error(error);
                    else cout << "Record updated\n"; //cout << "New Exit Date was inserted successfully.\n";

                    delete [] exit_date;
                    delete [] rec_id;
                }
            }
            else if(user_input[0] == "/numCurrentPatients")
            {
                if(i > 3)  printf("Arguments must be at most 2, insert 1 or 2 arguments!\n");
                else if(i < 2) printf("Arguments must be at least 1, insert 1 or 2 arguments!\n");
                else
                {
                    char* disease=NULL;
                    int error=0,j;

                    if(user_input[1] != "")
                    {
                        disease = new char[user_input[1].length()+1];
                        strcpy(disease,user_input[1].c_str());

                        error = Invalid_Disease(disease);
                        
                        if(error != 0)
                        {
                            if(error%2!=0)
                                Print_Error(Disease_ID_has_blanks);
                            else
                                Print_Error(Invalid_Disease_ID);
                        }  
                    }
                    
                    if(error == 0)
                    {                 
                        for(j=0;j<diseaseHashtableNumOfEntries;j++)
                        {
                            if(Search_Disease_Entry(disease_array[j],disease)) break;
                            if(j == (diseaseHashtableNumOfEntries-1) && (disease != NULL))
                                printf("There is no disease with name: %s.\n",disease);
                        }
                    }

                    delete [] disease;
                }
                
            }
            else if(user_input[0] == "/exit")  
            {
                cout << "exiting\n";
                break;
            }
            //else cout << "Please give the right input!\n";
            else break;
            if(flag == 2)  break;
        }

        //Deallocate the memory of structs.
        Delete_Record_List(&head);
        for(i=0; i<diseaseHashtableNumOfEntries; i++)
        {
            Destroy_Bucket_List(&disease_array[i]);
        }

        for(i=0; i<countryHashtableNumOfEntries; i++)
        {
            Destroy_Bucket_List(&country_array[i]);
        }
    }    
    return 0;
}