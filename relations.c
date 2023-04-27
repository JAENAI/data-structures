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
		lst=suptete(lst);
	}
}
listeg rech(listeg lst, void *x, int(*comp)(void *, void *))
{
	listeg new=listegnouv();
	while(lst!=NULL)
	{
		if(comp(x,lst->val)==0)
		{
			adjtete(new,(void*)lst);
		}
		lst=lst->suiv;
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
	struct s_node * liste;
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
	(*g)=(Relations)malloc(sizeof(struct s_relations));
	if((*g)==NULL)
	{
		fprintf(stderr,"ERREUR MALLOC");
		exit(1);
	}
}
void relationFree(Relations *g)
{
	while((*g)->liste!=NULL)
	{
		while(((Sommet)((*g)->liste->val))->larcs!=NULL)
		{
			free(((Arc)(((Sommet)((*g)->liste->val))->larcs->val))->x);
			free(((Sommet)((*g)->liste->val))->larcs->val);
			((Sommet)((*g)->liste->val))->larcs=((Sommet)((*g)->liste->val))->larcs->suiv;
		}
		free(((Sommet)((*g)->liste->val))->x);
		free(((Sommet)((*g)->liste->val))->larcs);
		(*g)->liste=(*g)->liste->suiv;
	}
	free((*g)->liste);
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
	Entite e=creerEntite(nom,t);
	Sommet s=nouvSommet(e);
	g->liste=adjqueue(g->liste,(void *)s);

}
// PRE CONDITION: id doit �tre coh�rent avec les types des sommets correspondants � x et y
//                p.ex si x est de type OBJET, id ne peut pas etre une relation de parente
// PRE CONDITION: strcmp(nom1,nom2)!=0
void adjRelation(Relations g, char *nom1, char *nom2, rtype id)
{
	Relations tmp=g,tmp1=g;
	Entite a,b;
	while(tmp->liste!=NULL)
	{
		if((compSommet((void *)(tmp->liste->val),(void *)nom1))==0)
		{
			a=((Sommet)(tmp->liste->val))->x;
		}
		if((compSommet((void *)(tmp->liste->val),(void *)nom2))==0)
		{
			b=((Sommet)(tmp->liste->val))->x;
		}
		tmp->liste=tmp->liste->suiv;
	}
	while(tmp1->liste!=NULL)
	{
		if((compSommet((void *)(tmp1->liste->val),(void *)nom1))==0)
		{
			Arc new=nouvArc(b,id);
			listeg n1=listegnouv();
			n1=adjtete(n1,(void *)new);
			((Sommet)(tmp1->liste->val))->larcs=adjqueue(((Sommet)(tmp1->liste->val))->larcs,(void*)n1);
		}
		if((compSommet((void *)(tmp1->liste->val),(void *)nom2))==0)
		{
			Arc new1=nouvArc(a,id);
			listeg n2=listegnouv();
			n2=adjtete(n2,(void *)new1);
			((Sommet)(tmp1->liste->val))->larcs=adjqueue(((Sommet)(tmp1->liste->val))->larcs,(void*)n2);
		}
		tmp1->liste=tmp1->liste->suiv;
	}
}

////////////////////////////////////////
// Exercice 4: Explorer les relations entre personnes

// 4.1 listes de relations
listeg en_relation(Relations g, char *x)
{
	Relations tmp=g;
	while(tmp->liste!=NULL)
	{
		if((compSommet((void*)(tmp->liste->val),(void*)x))==0)
		{
			return ((Sommet)(tmp->liste->val))->larcs;
		}
		tmp->liste=tmp->liste->suiv;
	}
	return NULL;
}
listeg chemin2(Relations g, char *x, char *y)
{
	listeg new=NULL;
	listeg rx=en_relation(g,x);
	listeg ry=en_relation(g,y);
	while(rx!=NULL)
	{
		listeg tmp=ry;
		while(tmp!=NULL)
		{
			char * string=((Arc)(tmp->val))->x->nom;
			if((compEntite((void*)(((Arc)(rx->val))->x),(void*)string)==0)
					&&(compEntite((void*)(((Arc)(rx->val))->x),(void*)x)!=0))

			{
				new=adjtete(new,(void*)(((Arc)(tmp->val))->x));
			}
			tmp=tmp->suiv;
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
	listeg tmp1=chemin2(g,x,y);
        while(tmp!=NULL)
        {
                if((compEntite((void*)(((Arc)(tmp->val))->x),(void*)y)==0)
				&&(est_lien_parente(((Arc)(tmp->val))->t)==true))
		{

			return true;
                }
		tmp=tmp->suiv;
        }
	while(tmp1!=NULL)
	{
		if((ont_lien_parente(g,x,((Entite)(tmp1->val))->nom)==true)
				&&(ont_lien_parente(g,y,((Entite)(tmp1->val))->nom)==true))
		{
			return true;
		}
	}
	return false;
}
// PRE CONDITION: les sommets correspondants � x et y sont de type PERSONNE
// PRE CONDITION: strcmp(x,y)!=0
bool se_connaissent_proba(Relations g, char *x, char *y)
{
	listeg new=chemin2(g,x,y);
	if(se_connaissent(g,x,y)==true){
		return false;
	}
	if(new!=NULL)
	{
		while(new!=NULL)
		{
			if((ont_lien_parente(g,x,((Entite)(new->val))->nom)==true)
					&&(ont_lien_parente(g,y,((Entite)(new->val))->nom)==false))
			{
				return true;
			}

			if((ont_lien_parente(g,x,((Entite)(new->val))->nom)==false)
                                        &&(ont_lien_parente(g,y,((Entite)(new->val))->nom)==true))
			{
				return true;
			}

		}
	}
	return false;
}
// PRE CONDITION: les sommets correspondants � x et y sont de type PERSONNE
// PRE CONDITION: strcmp(x,y)!=0
bool se_connaissent_peutetre(Relations g, char *x, char *y)
{
	if((se_connaissent(g,x,y)==false)
		&&(se_connaissent_proba(g,x,y)==false)
			&&(chemin2(g,x,y)!=NULL))
	{
		return true;
	}
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
	afficheEntite((void*)(((Arc)(x))->x));
}
////////////////////////////////////////
// Exercice 6: Parcours
void affiche_degre_relations(Relations r, char *x)
{
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

