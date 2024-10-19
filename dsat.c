#include "dsat.h"

/* 
	les couleurs commencent par 1 et max = n, l'init des couleurs est 0.
	g.content[i].aux contient le degre du sommet i 
 The Dude
*/

G initChampsDsat(G g)
{
	int i,n=g.n;
	for(i=1;i<=n;i++)
		g.content[i].dsat=g.content[i].aux;
	return(g);
}
		
int rechercheSommetDegreMax(G g)
{												/* a la premiere passe on cherche le sommet de dg max */
	int i,indice,max,n=g.n;
	max=0;indice=0;
	for(i=1;i<=n;i++)
		if (g.content[i].aux>max)
			{
			max=g.content[i].aux;
			indice=i;
			}
	return(indice);
}

int sommetNonColorieAvecDsatMax(G g)
{												/* au passe suivante on cherche celui non colorie de dsat max */
	int i,indice,dsatDei,max,n=g.n;
	max=0;indice=0;								/* rappel: dsat(v)= dg(v) si N(v) n'est pas colorie, sinon dsat(v)=card(sommets colorie de N(v))*/
	for(i=1;i<=n;i++)
		if (g.content[i].color==0)
			{
			dsatDei=g.content[i].dsat;
			if  (dsatDei>max)
				{
				max=dsatDei;
				indice=i;
				}
			if  ((dsatDei==max) && (g.content[i].aux>g.content[indice].aux))
				{ /* second criteres */
				max=dsatDei;
				indice=i;
				}			
			}
	return(indice);
}

G IncrementOuPas (G g,int sommetColorie, int unVoisin, int color)
{ 	/* passage delicat vis à vis du critere DSAT */
	struct cellHorizontal *pt;
	
	pt=g.content[unVoisin].next;
	while  (pt!=NULL)
			{
				/*si un voisin de som AUTRE QUE sommetColorie est colorier par color alors pas de dsat++*/
			if (((*pt).sommet!=sommetColorie) && (g.content[(*pt).sommet].color==color))
				return g;
			pt=(*pt).next;
			}
	g.content[unVoisin].dsat++; /* il n'existe pas de voisin de som avec la couleur color donc dsat++*/
	return(g);	
}

int NbDeVoisinColorie (int v, G g)
{
	struct cellHorizontal *pt;
	int cpt=0;
	
	pt=g.content[v].next;
	while  (pt!=NULL)
			{
			if (g.content[(*pt).sommet].color>0)
				cpt++;
			pt=(*pt).next;
			}
	return (cpt);
}

G majDeDsatDesVoisinsDeSom (G g, int som, int color)
	{
	struct cellHorizontal *pt;

	pt=g.content[som].next;
	while  (pt!=NULL)
			{
			/* on realise g.content[(*pt).sommet].dsat++;  que si 
				1. ce som est non-colorie 
				2. si color n'existe pas dans dans N((*pt).sommet)) AUTRE QUE som !!!!! */
			if (g.content[(*pt).sommet].color <= 0)
				if (g.content[(*pt).sommet].dsat==g.content[(*pt).sommet].aux)
					g.content[(*pt).sommet].dsat=1;
				else
					g=IncrementOuPas(g,som,(*pt).sommet,color);
			pt=(*pt).next;
			}
	return(g);
	}
	
int * MonJudaColor (int k)
	{
	int i,*juda=(int *) malloc( (k+1) *sizeof(int));
	
	if (juda==NULL)
		{
		fprintf(stderr,"\n Erreur malloc colorMinDansVoisinageDeSom dans dsat.c g.n=%d\n",k);
		exit(1);
		}
	for(i=1;i<=k;i++)
		juda[i]=0;
	return(juda);
	}

int IndiceMinLibreDansJuda (G g,int * juda)
	{
	int k,indice=1;
	k=g.n;
	while ((indice<=k) && (juda[indice]>0))
		indice++;
	return(indice);
	}

/* 	colorMinDansVoisinageDeSom
on cree un vecteur contenant des 0 (note juda)
quand une couleur i est utilise par un voisin de som, on place juda[i]=1
une fois tous les voisins de som visites, il suffit de cherche le plus petit indice i>0 dans juda
tel que juda[i]=0
*/
int colorMinDansVoisinageDeSom(G g, int som)
	{
	int color,min=1;
	int k=g.n;
	struct cellHorizontal *pt;
	int * juda = MonJudaColor (k);

	pt=g.content[som].next;
	while  (pt!=NULL)
			{
			color=g.content[(*pt).sommet].color;
			if (color>0)
				juda[color]=1;
			pt=(*pt).next;
			}
	color=IndiceMinLibreDansJuda(g,juda);
	free(juda);	/*liberation */
	return (color);
	}
	
G colorationDsat(G g, int som)
	{/*ICI un sommet peut avoir tous ces voisins deja colorie (dsat=degre). som.color=1 peut alors engendrer un conflit*/
	int color;
	
	color=colorMinDansVoisinageDeSom(g,som);
	g.content[som].color=color;
	g=majDeDsatDesVoisinsDeSom(g,som,color);
	if (g.content[som].color>g.nbcol)			/* maj nb couleur du graphe G */
		{
		g.nbcol=g.content[som].color;
		}
	return(g);
	}
	
G dsat(G g)
	{
	int som;
	printf("\nDSAT debut...");
	g=initChampsDsat(g);						/* au depart pour tout v, dsat(v)=0 */
	som=rechercheSommetDegreMax(g);
	while (som!=0)
		{
		g=colorationDsat(g,som); /*displayDsat (g);*/
		som=sommetNonColorieAvecDsatMax(g);
		}
	printf("... Fin \n");
	return(g);
	}		
