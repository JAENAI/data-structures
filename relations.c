// relations.c�: d�finit le point d'entr�e pour l'application .
//

typedef int bool;
#define false 0
#define true -1

#include "stdlib.h"
#include "memory.h"
#include "stdio.h"
#include "string.h"

////////////////////////////////////////
// Exercice 1: Classement des Relations

typedef enum {
	FRERE = 2, COUSIN, PARENT, ONCLE, EPOUX, AMI, VIT, CONNAIT,
	CHEF, COLLEGUE, LOCATAIRE, TRAVAILLE, PROPRIETAIRE, SITUE, DECOUVERT
} rtype;

bool est_lien_parente(rtype id) 
{ 
	if((id>=2)&&(id<=6))
	{
		return true;
	}
	return false; 
}
bool est_lien_professionel(rtype id) 
{ 
	if((id==10)||(id==11))
	{
		return true;
	}
	return false; 
}
bool est_lien_connaissance(rtype id) 
{ 
	if((id>=7)&&(id<=9))
	{
		return true;
	}
	return false; 
}
char* toStringRelation(rtype id) 
{ 
	char*r[]=
	{
	"FRERE", "COUSIN", "PARENT", "ONCLE", "EPOUX", "AMI", "VIT", "CONNAIT",
        "CHEF", "COLLEGUE", "LOCATAIRE", "TRAVAILLE", "PROPRIETAIRE"," SITUE"," DECOUVERT"
	};
	if(id<=16)
	{
		return r[id-2];
	}
	return ""; 
}

////////////////////////////////////////
// Exercice 2: Liste de pointeurs

typedef struct s_node {
	void *val;  // pointeur vers objet quelconque
	struct s_node *suiv;
} *listeg;

listeg listegnouv() { return NULL; }
listeg adjtete(listeg lst, void *x)
{
	listeg tmp=(listeg)malloc(sizeof(struct s_node));
	if(tmp==NULL)
	{
		fprintf(stderr,"ERREUR MALLOC");
		exit(1);
	}
	tmp->val=x;
	tmp->suiv=lst;
	return tmp;
}
listeg adjqueue(listeg lst, void *x)
{
	if(lst==NULL)
	{
		return adjtete(lst,x);
	}
	listeg tmp=lst;
	listeg new=(listeg)malloc(sizeof(struct s_node)); 
	if(new==NULL)
	{
		fprintf(stderr,"ERREUR MALLOC");
		exit(1);
	}
	new->val=x;
	new->suiv=NULL;
	while(tmp->suiv!=NULL)
	{
      tmp=tmp->suiv;  
	}
	tmp->suiv=new;
	return lst;
}
listeg suptete(listeg lst)
{
	if(lst==NULL){
		return NULL;
	}
	listeg tmp=lst->suiv;
	free(lst->val);
	free(lst);
	return tmp;
}

void *tete(listeg lst)
{
	if(lst!=NULL)
	{
		return lst->val;
	}
	return NULL;
}
int longueur(listeg lst)
{
	if(lst==NULL)
	{
		return 0;
	}
	return 1+longueur(lst->suiv);
}
bool estvide(listeg lst)
{
	return (lst==NULL);
}
void detruire(listeg lst)
{
	while(lst!=NULL){
		listeg tmp=lst->suiv;
		free(lst->val);
		free(lst);
		lst=tmp;
	}
}
listeg rech(listeg lst, void *x, int(*comp)(void *, void *))
{
	listeg tmp=lst;
	listeg new=listegnouv();
	while(tmp!=NULL)
	{
		if(comp(x,lst->val)==0)
		{
			new=adjqueue(new,(void*)tmp);
		}
		tmp=tmp->suiv;
	}
	return new;
}

////////////////////////////////////////
// Exercice 3: Construction du graphe

#define LONG_NOM_MAX 64
typedef enum { PERSONNE=1, OBJET, ADRESSE, VILLE } etype;
typedef struct s_entite {
	char nom[LONG_NOM_MAX]; // le nom de l�entit� p.ex � Peugeot 106 �
	etype ident; // l�identifiant associ�, p.ex OBJET
} *Entite;
//3.1 les structures de donn�es
typedef struct s_sommet {
	struct s_node *larcs;
	struct s_entite *x;
} *Sommet;

typedef struct s_arc {
	rtype t;
	struct s_entite *x;
} *Arc;

typedef struct s_relations {
	listeg liste;
} *Relations;

//3.2 les constructeurs
Entite creerEntite(char *s, etype e)
{
	Entite ent=(Entite)malloc(sizeof(struct s_entite));
	if(ent==NULL)
	{
		fprintf(stderr,"ERREUR MALLOC");
		exit(1);
	}
	strcpy(ent->nom,s);
	ent->ident=e;
	return ent;
}
Sommet nouvSommet(Entite e)
{
	Sommet s=(Sommet)malloc(sizeof(struct s_sommet));
	if(s==NULL)
	{
		fprintf(stderr,"ERREUR MALLOC");
		exit(1);
	}
	s->x=e;
	s->larcs=NULL;
	return s;
}
Arc nouvArc(Entite e, rtype type)
{
	Arc a=(Arc)malloc(sizeof(struct s_arc));
	if(a==NULL)
	{
		fprintf(stderr,"ERREUR MALLOC");
		exit(1);
	}
	a->t=type;
	a->x=e;
	return a;
}
void relationInit(Relations *g)
{
	if(g==NULL)
	{
		return;
	}
	(*g)=(Relations)malloc(sizeof(struct s_relations));
	if((*g)==NULL)
	{
		fprintf(stderr,"ERREUR MALLOC");
		exit(1);
	}
	(*g)->liste=NULL;
}
void relationFree(Relations *g)
{
	while((*g)->liste!=NULL)
	{
		listeg tmp1=(*g)->liste->suiv;
		free(((Sommet)((*g)->liste->val))->x);
		while(((Sommet)((*g)->liste->val))->larcs!=NULL)
		{
			listeg tmp=((Sommet)((*g)->liste->val))->larcs->suiv;
			free(((Sommet)((*g)->liste->val))->larcs->val);
			free(((Sommet)((*g)->liste->val))->larcs);
			((Sommet)((*g)->liste->val))->larcs=tmp;
		}
		free((*g)->liste->val);
		free((*g)->liste);
		(*g)->liste=tmp1;
	}
	free((*g));
}

//3.3 les comparaisons
int compEntite(void *e, void *string)
{
	int cmp=strcmp(((Entite)e)->nom,(char*)string);
	return cmp;
}
int compSommet(void *s, void *string)
{
	int cmp=strcmp(((Sommet)s)->x->nom,(char*)string);
	return cmp;
}
int compArc(void *a, void *string)
{
	int cmp=strcmp(((Arc)a)->x->nom,(char*)string);
	return cmp;
}

//3.4 ajout d'entites et de relations
void adjEntite(Relations g, char *nom, etype t)
{
	if(g==NULL)
	{
		relationInit(&g);
	}
	//listeg tmp=rech(g->liste,(void *)nom,compEntite);
	listeg tmp=g->liste;
	while(tmp!=NULL)
	{
		if((compEntite((void *)(((Sommet)(tmp->val))->x),(void *)nom))==0)
		{
			printf("Entite existe déjà\n");
			return;
		}
		tmp=tmp->suiv;
	}
	Entite e=creerEntite(nom,t);
	g->liste=adjtete(g->liste,(void *)nouvSommet(e));
}

// PRE CONDITION: id doit �tre coh�rent avec les types des sommets correspondants � x et y
//                p.ex si x est de type OBJET, id ne peut pas etre une relation de parente
// PRE CONDITION: strcmp(nom1,nom2)!=0
void adjRelation(Relations g, char *nom1, char *nom2, rtype id)
{
	//Entite a=tete(rech(g->liste,(void*)nom1,compSommet));
	//Entite b=tete(rech(g->liste,(void*)nom2,compSommet));
	listeg tmp=g->liste;
	Entite a,b;
	while(tmp!=NULL)
	{
		if((compEntite((void *)(((Sommet)(tmp->val))->x),(void *)nom1))==0)
		{
			a=((Sommet)(tmp->val))->x;
		}
		if((compEntite((void *)(((Sommet)(tmp->val))->x),(void *)nom2))==0)
		{
			b=((Sommet)(tmp->val))->x;
		}
		tmp=tmp->suiv;
	}
	//afficheEntite((void *)a);
	//afficheEntite((void *)b);
	listeg tmp1=g->liste;
	while(tmp1!=NULL)
	{
		if((compSommet((void *)(tmp1->val),(void *)nom1))==0)
		{
			Arc new=nouvArc(b,id);
			((Sommet)(tmp1->val))->larcs=adjtete(((Sommet)(tmp1->val))->larcs,(void*)new);
		}
		if((compSommet((void *)(tmp1->val),(void *)nom2))==0)
		{
			Arc new1=nouvArc(a,id);
			((Sommet)(tmp1->val))->larcs=adjtete(((Sommet)(tmp1->val))->larcs,(void*)new1);
		}
		tmp1=tmp1->suiv;
	}
}

////////////////////////////////////////
// Exercice 4: Explorer les relations entre personnes

// 4.1 listes de relations
listeg en_relation(Relations g, char *x)
{
	listeg tmp=g->liste;
	while(tmp!=NULL)
	{
		if((compSommet((void*)(tmp->val),(void*)x))==0)
		{
			return ((Sommet)(tmp->val))->larcs;
		}
		tmp=tmp->suiv;
	}
	return NULL;
}
bool est_dans_larcs(listeg lst,char *x)
{
	while(lst!=NULL)
	{
		if(compEntite((void*)(((Arc)(lst->val))->x),(void*)x)==0)
		{
			return true;
		}
		lst=lst->suiv;
	}
	return false;
}
listeg chemin2(Relations g, char *x, char *y)
{
	listeg new=listegnouv();
	listeg rx=en_relation(g,x);
	listeg ry=en_relation(g,y);
	while(rx!=NULL)
	{
		char * string=((Arc)(rx->val))->x->nom;
		if((est_dans_larcs(ry,string))==true)
		{
			Entite tmp=creerEntite(string,((Arc)(rx->val))->x->ident);
			new=adjtete(new,(void*)tmp);
		}
		rx=rx->suiv;
	}
	return new;
}
// 4.2 verifier un lien de parente
// PRE CONDITION: strcmp(x,y)!=0
bool ont_lien_parente(Relations g, char *x, char *y)
{
        listeg tmp=en_relation(g,x);
        while(tmp!=NULL)
        {
			if((compEntite((void*)(((Arc)(tmp->val))->x),(void*)y)==0)
			&&(est_lien_parente(((Arc)(tmp->val))->t)==true))
			{
				return true;
			}
		tmp=tmp->suiv;
        }

	return false;
}

// 4.3 tester connaissances
// PRE CONDITION: les sommets correspondants � x et y sont de type PERSONNE
// PRE CONDITION: strcmp(x,y)!=0
bool se_connaissent(Relations g, char *x, char *y)
{
    listeg tmp=en_relation(g,x);
    while(tmp!=NULL)
    {
	    if((compEntite((void*)(((Arc)(tmp->val))->x),(void*)y)==0)
				&&((est_lien_parente(((Arc)(tmp->val))->t)==true)
				||(est_lien_professionel(((Arc)(tmp->val))->t)==true)
				|| (est_lien_connaissance(((Arc)(tmp->val))->t)==true)))
		{
			return true;
        }
		tmp=tmp->suiv;
    }
	listeg tmp1=chemin2(g,x,y);
	while(tmp1!=NULL)
	{
		if((ont_lien_parente(g,x,((Entite)(tmp1->val))->nom)==true)
				&&(ont_lien_parente(g,y,((Entite)(tmp1->val))->nom)==true))
		{
			detruire(tmp1);
			return true;
		}
		tmp1=suptete(tmp1);
	}
	return false;
}
// PRE CONDITION: les sommets correspondants � x et y sont de type PERSONNE
// PRE CONDITION: strcmp(x,y)!=0
bool se_connaissent_proba(Relations g, char *x, char *y)
{
	if(se_connaissent(g,x,y)==true)
	{
		return false;
	}
	listeg new=chemin2(g,x,y);
	if(new!=NULL)
	{
		while(new!=NULL)
		{
			if((ont_lien_parente(g,x,((Entite)(new->val))->nom)==true)
					&&(ont_lien_parente(g,y,((Entite)(new->val))->nom)==false))
			{
				detruire(new);
				return true;
			}

			if((ont_lien_parente(g,x,((Entite)(new->val))->nom)==false)
                                        &&(ont_lien_parente(g,y,((Entite)(new->val))->nom)==true))
			{
				detruire(new);
				return true;
			}
			new=suptete(new);
		}
	}
	return false;
}
// PRE CONDITION: les sommets correspondants � x et y sont de type PERSONNE
// PRE CONDITION: strcmp(x,y)!=0
bool se_connaissent_peutetre(Relations g, char *x, char *y)
{
	listeg lst=chemin2(g,x,y);
	if((se_connaissent(g,x,y)==false)
		&&(se_connaissent_proba(g,x,y)==false)
			&&(lst!=NULL))
	{
		detruire(lst);
		return true;
	}
	detruire(lst);
	return false;
}

////////////////////////////////////////
// Exercice 5: Affichages

char * toStringType(etype id)
{
        char*r[]=
        {
        "PERSONNE","OBJET","ADRESSE","VILLE"
        };
        if(id<=4)
        {
                return r[id-1];
        }
        return "";
}

void affichelg(listeg l, void(*aff)(void *))
{
	listeg tmp=l;
	while(tmp!=NULL)
	{
		aff(tmp->val);
		tmp=tmp->suiv;
	}
}

void afficheEntite(void *x)
{
	printf("%s : %s\n",((Entite)(x))->nom,toStringType(((Entite)(x))->ident));

}
void afficheArc(void *x)
{
	printf("--%s-->",toStringRelation(((Arc)(x))->t));
	afficheEntite((void *)(((Arc)(x))->x));
}
////////////////////////////////////////
// Exercice 6: Parcours
void affiche_degre_relations_aux(Relations r,char *y,char *rest)
{
	listeg tmp=en_relation(r, y);
	while(tmp!=NULL)
	{
		if(compEntite((void *)(((Arc)(tmp->val))->x),(void *)rest)!=0)
		{
			afficheEntite((void *)(((Arc)(tmp->val))->x));
		}
		tmp=tmp->suiv;
	}
}
void affiche_degre_relations(Relations r, char *x)
{
	printf("%s\n",x);
	printf("--1\n");
	listeg tmp=en_relation(r, x);
	affiche_degre_relations_aux(r,x,x);
	printf("--2\n");
	while(tmp!=NULL)
	{
		char *string=((Arc)(tmp->val))->x->nom;
		affiche_degre_relations_aux(r,string,x);
		tmp=tmp->suiv;
	}
}


int main()
{
	int i,j;
	Relations r; relationInit(&r);
	// ajouter les entites de l'exemple
	char *tabe[] = {"KARL","LUDOVIC","CELINE","CHLOE","GILDAS","CEDRIC","SEVERINE",
		"PEUGEOT 106" ,"1, RUE DE LA RUE","STRASBOURG" };
	for (i = 0; i < 7; i++) adjEntite(r, tabe[i], PERSONNE);
	adjEntite(r, tabe[7], OBJET);
	adjEntite(r, tabe[8], ADRESSE);
	adjEntite(r, tabe[9], VILLE);
	// ajouter les relations de l'exemple
	adjRelation(r, tabe[0], tabe[1], FRERE);
	adjRelation(r, tabe[0], tabe[2], AMI);
	adjRelation(r, tabe[0], tabe[3], CONNAIT);
	adjRelation(r, tabe[0], tabe[5], COUSIN);
	adjRelation(r, tabe[0], tabe[7], PROPRIETAIRE);
	adjRelation(r, tabe[0], tabe[8], PROPRIETAIRE);
	adjRelation(r, tabe[3], tabe[4], VIT);
	adjRelation(r, tabe[5], tabe[6], EPOUX);
	adjRelation(r, tabe[5], tabe[8], LOCATAIRE);
	adjRelation(r, tabe[7], tabe[8], DECOUVERT);
	adjRelation(r, tabe[8], tabe[9], SITUE);

	// explorer les relations
	printf("%s est en relation avec:\n", tabe[0]);
	affichelg(en_relation(r, tabe[0]),afficheArc);
	printf("\n");

	for (i = 0; i < 7; i++) for (j = i + 1; j < 10; j++)
	{
		printf("<%s> et <%s> ont les relations communes:\n", tabe[i], tabe[j]);
		listeg ch=chemin2(r,tabe[i],tabe[j]);
		affichelg(ch, afficheEntite);
		printf("\n");
		detruire(ch);
	}
	printf("\n\n");

	for (i = 0; i < 10; i++) for (j = i + 1; j < 10; j++)
	{
		printf("<%s> et <%s> ont lien de parente: %s\n",
			tabe[i], tabe[j], ont_lien_parente(r, tabe[i], tabe[j]) ? "vrai" : "faux");
	}
	printf("\n");
	for (i = 0; i < 7; i++)
	{
		for (j = i + 1; j < 7; j++)
		{
			printf("<%s> et <%s> se connaissent: %s\n",
				tabe[i], tabe[j], se_connaissent(r, tabe[i], tabe[j]) ? "vrai" : "faux");
			printf("<%s> et <%s> se connaissent tres probablement: %s\n",
				tabe[i], tabe[j], se_connaissent_proba(r, tabe[i], tabe[j]) ? "vrai" : "faux");
			printf("<%s> et <%s> se connaissent peut etre: %s\n",
				tabe[i], tabe[j], se_connaissent_peutetre(r, tabe[i], tabe[j]) ? "vrai" : "faux");
		}
		printf("\n");
	}

	affiche_degre_relations(r, tabe[3]);

	relationFree(&r);

	printf("\nPRESS RETURN\n");
	char buff[64]; fscanf(stdin, "%s", buff);
    return 0;
}

