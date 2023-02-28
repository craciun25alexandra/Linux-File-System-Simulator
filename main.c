/* CRACIUN Alexandra-Georgiana -311CB */

#include "liste.h"

int main(){

    char *cerinta = calloc(256, 1);
    TDirTree d = DirectRoot(); //creeaza directorul root
    char *p;
    //cat timp se citeste de la tastatura
    while(fgets(cerinta, 256, stdin)){
        //elimina caracterul de returnare
        if(strchr(cerinta, '\r')) p = strtok(cerinta, "\r");
        //elimina caracterul endl
        if(strchr(cerinta, '\n')) p = strtok(cerinta, "\n");
        p = strtok(cerinta, " ");
        if (strcmp(p, "touch") == 0){
            
            p = strtok(NULL, " ");
            //cauta numele in file uri, returneaza 1 daca gaseste
            if(Touch(&d, p) == 1)
                printf("File %s already exists!\n", p);
            //cauta numele in directoare, returneaza 2 daca gaseste
            else if(Touch(&d, p) == 2)
                printf("Directory %s already exists!\n", p);
            //functia creeaza file ul la apelare daca are nume unic
    }

        else if (strcmp(p, "mkdir") == 0){
            p = strtok(NULL, " ");
            //cauta numele in directoare, returneaza 1 daca gaseste
            if(Mkdir(&d, p)==1)
                printf("Directory %s already exists!\n", p);
            //cauta numele in file uri, returneaza 2 daca gaseste
            else if(Mkdir(&d, p)==2)
                printf("File %s already exists!\n", p);
            //functia creeaza directory ul la apelare daca are nume unic
        }
        else if (strcmp(p, "ls") == 0){

            //parcurgere SRD a directoarelor
            SRDDirectory(d->directories);
            //parcurgere SRD a file urilor
            SRDFiles(d->files);
            printf("\n");
        }
        else if (strcmp(p, "rm") == 0){

            int *ok = calloc(sizeof(int), 1); //semafor
            p = strtok(NULL, " ");
            //sterge file ul daca exista si pastreaza in semafor val de adevar
            DeleteFile(&d->files, p, ok);
            if (!(*ok)) printf("File %s doesn't exist!\n", p);
            free(ok);
        }
        else if (strcmp(p, "rmdir") == 0){
            //analog pt directory
            int *ok = calloc(sizeof(int), 1);
            p = strtok(NULL, " ");
            DeleteDirectory(&d->directories, p, ok);
            if (!(*ok)) printf("Directory %s doesn't exist!\n", p);
            free(ok);
        }
        else if (strcmp(p, "cd") == 0){
            p = strtok(NULL, " ");
            // daca comanda este .. trebuie sa se duca in directorul parinte
            // daca acesta exista
            if (strcmp(p, "..") == 0)
                {if( GoToParent(d) != NULL) d = GoToParent(d);}
            else if (Move(d, p) == NULL) printf("Directory not found!\n");
            //functia returneaza directorul cu numele dat
            else d = Move(d, p);
        }
        else if(strcmp(p, "pwd") == 0){
           
           pwd(d);
           printf("\n");
        }
        else if(strcmp(p, "find") == 0){
            p = strtok(NULL, " ");
            if(strcmp(p, "-d") == 0){
                p = strtok(NULL, " ");
                TDirTree copy = d;
                //cautarea se face de la root
                while(strcmp(copy->name, "root") != 0)
                    copy = GoToParent(copy);
                if(FindDir(copy, p) != NULL){
                    printf("Directory %s found!\n", p);
                    pwd(FindDir(copy,p));
                    printf("\n");}
                else printf("Directory %s not found!\n", p);

            }
            else{
                //analog pt file uri
                p = strtok(NULL, " ");
                TDirTree copy = d;
                while(strcmp(copy->name, "root") != 0)
                    copy = GoToParent(copy);
                if(FindFile(copy, p)!= 0){
                    printf("File %s found!\n", p);
                    pwd(FindFile(copy,p));
                    printf("\n");}
                
                else printf("File %s not found!\n", p);
            }
        }
}
    free(cerinta); //elibereaza variabila folosita pt citirea cerintei
    while(strcmp(d->name, "root") != 0)
        d = GoToParent(d);
    DistrArb(&d);
    return 0;
}