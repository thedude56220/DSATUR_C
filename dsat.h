
G initChampsDsat(G g);
int sommetNonColorieAvecDsatMax(G g);
G dsat(G g);
G colorationDsat(G g, int som);
int rechercheSommetDegreMax(G g);
G majDeDsatDesVoisinsDeSom (G g, int som, int color);
int * MonJudaColor (int k);
int IndiceMinLibreDansJuda (G g,int * juda);
int colorMinDansVoisinageDeSom(G g, int som);
G IncrementOuPas (G g,int sommetColorie, int unVoisin, int color);
int NbDeVoisinColorie (int v, G g);
