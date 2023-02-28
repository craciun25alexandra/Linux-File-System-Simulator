/* CRACIUN Alexandra-Georgiana -311CB */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct directory{

    char *name;
    struct directory *left, *right, *parent, *directories;
    struct file *files;
}TDirectory, *TDirTree;

typedef struct file{
	char *name;
	struct directory *parent;
    struct file *left, *right;
}TFile, *TFileTree;

TDirTree DirectRoot(){

    TDirTree root = (TDirTree)malloc(sizeof(TDirectory));
    if(!root) return NULL;
    root->name = strdup ("root"); //aloca spatiu si copiaza numele
    root->left = NULL;
    root->right = NULL;
    root->parent = NULL;
    root->files = NULL;
    root->directories = NULL;
    return root;
}

int Touch(TDirTree *d, char* nume){

    
    TDirTree verif = (*d)->directories;
    //verifica daca numele se afla printre directoare
    while(verif){
        if(strcmp(nume, verif->name)==0) return 2;
        else if (strcmp(nume, verif->name) > 0) 
                verif = verif->right;
        else verif = verif->left;
    }
    //daca este primul file adaugat
    if((*d)->files == NULL){
        (*d)->files = calloc(sizeof(TFile), 1);
        (*d)->files->name = strdup(nume);
        (*d)->files->left = (*d)->files->right = NULL;
        (*d)->files->parent = (*d)->directories;
        return 0;}
    TFileTree aux, aux2;
    aux = (*d)->files;
    while(aux){
        aux2 = aux;
        //daca mai exista un file cu acelasi nume
        if(strcmp(nume, aux->name)==0) return 1;
        //cauta l in files
        else if(strcmp(nume, aux->name) > 0) 
                aux = aux->right;
        else aux = aux->left;
    }
    //creeaza file ul
    aux = calloc(sizeof(TFile), 1);
    aux->name = strdup(nume); //aloca si pune numele
    aux->left = aux->right = NULL;
    aux->parent = (*d)->directories;
    //daca trebuie pus in stanga sau in dreapta ultimului file
    if (strcmp(nume, aux2->name) > 0)
        aux2->right = aux;
    else aux2->left = aux;
    //returneaza 0, s a efectuat crearea
    return 0;
    }

int Mkdir(TDirTree *d, char* nume){
    //analog pt directoare
    TFileTree verif = (*d)->files;
    while(verif){
        if(strcmp(nume, verif->name)==0) return 2;
        else if ( strcmp(nume, verif->name) > 0) 
                verif = verif->right;
        else verif = verif->left;
    }
    
    if((*d)->directories == NULL){
        (*d)->directories = calloc(sizeof(TDirectory), 1);
        (*d)->directories->name = strdup(nume);
        (*d)->directories->left = (*d)->directories->right = NULL;
        (*d)->directories->parent = (*d);
        return 0;
    }
    TDirTree aux, aux2;
    aux = (*d)->directories;
    while(aux){

        aux2 = aux;
        if(strcmp(nume, aux->name)==0) return 1;
        else if ( strcmp(nume, aux->name) > 0) 
                aux = aux->right;
        else aux = aux->left;
    }
    aux = calloc(sizeof(TDirectory), 1);
    aux->name = strdup(nume);
    aux->left = aux->right = NULL;
    aux->parent = (*d);
    if (strcmp(nume, aux2->name)>0)
        aux2->right = aux;
    else aux2->left = aux;
    return 0;
    }


void SRDDirectory(TDirTree a)
{
    //printare srd pt directoare
	if(!a) return;
	if(!a->left && !a->right){
        printf("%s ", a->name);
        return;}
	SRDDirectory(a->left);
    printf("%s ", a->name);
	SRDDirectory(a->right);
}
void SRDFiles(TFileTree a)
{
    //printare srd pt fileuri
	if(!a)  return;
	if(!a->left && !a->right){
        printf("%s ", a->name);
        return;}
	SRDFiles(a->left);
    printf("%s ", a->name);
	SRDFiles(a->right);
}
//gaseste cel mai mic file lexicografic
TFileTree FindMinFile(TFileTree a){

    while (a->left != NULL) {
        a = a->left;
    }
    return a;
}

TFileTree DeleteFile(TFileTree* d, char* nume, int*ok){

    if(!(*d)) return (*d);
    // cauta directorul
    else if (strcmp(nume, (*d)->name) > 0)
        (*d)->right = DeleteFile( &(*d)->right, nume, ok);
    else if (strcmp(nume, (*d)->name) < 0)
        (*d)->left = DeleteFile( &(*d)->left, nume, ok);
    else{
        //daca e frunza, se elimina
        if((*d)->left == NULL && (*d)-> right == NULL){
            free((*d)->name); //elibereaza spatiul pt nume
            free(*d); //elibereaza spatiul pt celula
            *d = NULL;
        }
        //daca are doar un fiu
        else if((*d)->left == NULL){
            TFileTree aux = (*d);
            //se pune subarorele drept in locul ei si se elibereaza
            (*d) = (*d)->right;
            free(aux->name);
            free(aux);
            aux = NULL;
        }
        //analog pt fiul in cealalta parte
          else if((*d)->right == NULL){
            TFileTree aux = (*d);
            (*d) = (*d)->left;
            free(aux->name);
            free(aux);
            aux = NULL;
        }
        //daca are mai multi fii
        else{
            //se gaseste minimul din subarborele drept(cu val mai mari)
            TFileTree aux = FindMinFile((*d)->right);
            free((*d)->name);
            //pune numele file ului gasit(cel care ii ia locul)
            (*d)->name = strdup(aux->name);
            //elimina dublura(celula cu numele care a fost inlocuit)
            (*d)->right = DeleteFile(&(*d)->right, aux->name, ok);
        }
    *ok = 1; //s a efectuat stergerea
    }
    return (*d); //returneaza arborele actualizat  
}
TDirTree FindMinDirectory(TDirTree a){

    while (a->left != NULL) {
        a = a->left;
    }
    return a;
}
TDirTree DeleteDirectory(TDirTree* d, char* nume, int*ok){
    //analog pt directoare
    if(!(*d)) return (*d);
    else if (strcmp(nume, (*d)->name)>0) (*d)->right = DeleteDirectory( &(*d)->right, nume, ok);
    else if (strcmp(nume, (*d)->name)<0) (*d)->left = DeleteDirectory( &(*d)->left, nume, ok);
    else{

        if((*d)->left == NULL && (*d)-> right == NULL){
            free((*d)->name);
            free(*d);
            *d = NULL;
        }
        else if((*d)->left == NULL){
            TDirTree aux = (*d);
            (*d) = (*d)->right;
            free(aux->name);
            free(aux);
            aux = NULL;
        }
          else if((*d)->right == NULL){
            TDirTree aux = (*d);
            (*d) = (*d)->left;
            free(aux->name);
            free(aux);
            aux = NULL;
        }
        else{
            TDirTree aux = FindMinDirectory((*d)->right);
            free((*d)->name);
            (*d)->name = strdup(aux->name);
            (*d)->right = DeleteDirectory(&(*d)->right, aux->name, ok);
        }
    *ok = 1;
    }
    return (*d);    
}

TDirTree GoToParent(TDirTree d){

   if(d->parent) return d->parent;
   else return NULL;
}

TDirTree Move(TDirTree d, char* nume){

    TDirTree a = d->directories;
    if (!a) return NULL;
    while(a){
        //se plimba prin arbore pana gaseste numele dat
        if(strcmp(nume, a->name) > 0)
            a = a->right;
        else if(strcmp(nume, a->name) < 0)
            a = a->left;
        else break;
    }
    return a;
}
TDirTree FindDir(TDirTree d, char *nume){

    if(d && strcmp(d->name, nume)==0) return d;
    else{
        TDirTree a = d->directories;
        TDirTree sol = NULL;
        while(a && !sol){
            // cauta prin arbore numele dat
            if(strcmp(a->name, nume) <0 && !sol)
                a = a->right;
            else if(strcmp(a->name, nume) >0 && !sol)
                a = a->left;
            else if(strcmp(a->name, nume) ==0 && !sol)
                sol = a;
        }
        //daca nu a fost gasit, cauta prin directoarele directoarelor si prin
        //fratii acestuia
        if(!sol && d->directories) sol = FindDir(d->directories, nume);
        if(!sol && d->left) sol = FindDir(d->left, nume);
        if(!sol && d->right) sol = FindDir(d->right, nume);
        return sol;
    }}

TDirTree FindFile(TDirTree d, char *nume){
        //analog pt file uri
        TFileTree a = d->files;
        TDirTree sol = NULL;
        while(a && !sol){
            if(strcmp(a->name, nume) <0)
                a = a->right;
            else if(strcmp(a->name, nume) >0)
                a = a->left;
            else sol = d;
        }
        if(!sol && d->directories) sol = FindFile(d->directories, nume);
        if(!sol && d->left) sol = FindFile(d->left, nume);
        if(!sol && d->right) sol = FindFile(d->right, nume);
        return sol;
}
void pwd (TDirTree d){

    if(d && strcmp(d->name, "root")==0){
        printf("/root");
        return;}
    //se duce recursiv din parinte in parinte pana la root si afiseaza traseul
    if(d && d->parent) pwd(d->parent);
    if(d) printf("/%s", d->name);

}

void distrugeF(TFileTree r) 	
{
	if (!r) return;
	distrugeF (r->left); //elibereaza copilul stang  
	distrugeF (r->right); //-//- drept
	free(r->name); //pt fiecare file se elibereaza numele si celula
    free(r);             
}

void distrugeD(TDirTree r){ 	
	if (!r) return;
    //analog cu Fileul, doar ca acceseaza si campurile directories si files
	distrugeD (r->left);     
	distrugeD (r->right);
    distrugeD (r->directories);
    distrugeF (r->files);  
	free(r->name);
    free(r);             
}

void DistrArb(TDirTree *a) 
{
	if (!(*a)) return; 
	distrugeD ((*a)->directories);
    distrugeF((*a)->files);   
	free((*a)->name);
    free((*a));               
}
