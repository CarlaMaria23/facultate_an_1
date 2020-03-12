#include <stdio.h>
#include <stdlib.h>
#include <math.h>
struct pixel
{
    unsigned char R,G,B;
};
struct fereastra
{
    int cifra;
    int l,c;
    double cor;
};
union numere
{
    unsigned int x;
    unsigned char c[4];
};
void generator(int n,unsigned int **v,unsigned seed)
{
    int k;
    unsigned r=seed;
    (*v)=(unsigned int *)malloc((n+1)*sizeof(unsigned int));
    (*v)[0]=seed;
    for(k=1;k<=n;k++)
    {
        r=r^r<<13;
        r=r^r>>17;
        r=r^r<<5;
        (*v)[k]=r;
    }
}
void liniarizare(char *poz,struct pixel **L)
{
    FILE *in;
    unsigned int W,H;
    in=fopen(poz,"rb");
    if(in==NULL)
    {
        printf("fisierul %s nu s-a putut deschide\n",poz);
        exit(0);
    }
    fseek(in,18*sizeof(char),SEEK_SET);
    fread(&W,sizeof(unsigned int),1,in);
    fseek(in,22*sizeof(char),SEEK_SET);
    fread(&H,sizeof(unsigned int),1,in);
    (*L)=(struct pixel *)malloc(W*H*sizeof(struct pixel));
    int dim_linie=3*W+(4-(3*W)%4)%4;
    fseek(in,54*sizeof(char),SEEK_SET);
    int i,j,k;
    struct pixel x;
    k=0;
    for(i=0;i<H;i++)
    {
        fseek(in,dim_linie*(H-i-1)+54,SEEK_SET);
        for(j=0;j<W;j++)
        {

            fread(&x.B,sizeof(unsigned char),1,in);
            fread(&x.G,sizeof(unsigned char),1,in);
            fread(&x.R,sizeof(unsigned char),1,in);
            (*L)[k]=x;
            k++;
       }
       fseek(in,((4-(3*W)%4)%4)*sizeof(unsigned char),SEEK_CUR);
    }
    fclose(in);
}
void reconstituire(struct pixel *L,unsigned char *h,char *poz)
{
    FILE *out;
    unsigned int W,H;
    int i,j;
    out=fopen(poz,"wb+");
    if(out==NULL)
    {
        printf("fisierul %s nu s-a putut deschide\n",poz);
        exit(0);
    }
    fwrite(h,sizeof(unsigned char),54,out);
    fseek(out,18*sizeof(char),SEEK_SET);
    fread(&W,sizeof(unsigned int),1,out);
    fseek(out,22*sizeof(char),SEEK_SET);
    fread(&H,sizeof(unsigned int),1,out);
    int k=0;
    struct pixel x;
    char a=0;
    fseek(out,54*sizeof(char),SEEK_SET);
    for(i=0;i<H;i++)
    {
        char a=0;
        k=W*(H-i-1);
        for(j=0;j<W;j++)
        {
            x=L[k];
            fwrite(&x.B,sizeof(unsigned char),1,out);
            fwrite(&x.G,sizeof(unsigned char),1,out);
            fwrite(&x.R,sizeof(unsigned char),1,out);
            k++;
        }
        for(j=1;j<=(4-(3*W)%4)%4;j++)
            fwrite(&a,sizeof(char),1,out);
    }
    fclose(out);
}
void antet(char *poz,unsigned char **header)
{
    FILE *in;
    in=fopen(poz,"rb");
    if(in==NULL)
    {
        printf("fisierul %s nu s-a putut deschide\n",poz);
        exit(0);
    }
    (*header)=(char *)malloc(54*sizeof(unsigned char));
    int i;
    unsigned char x;
    for(i=0;i<54;i++)
    {
        fread(&x, sizeof(unsigned char),1,in);
        (*header)[i]=x;
    }
    fclose(in);
}
void criptare(char *poza,char *cript,char *txt)
{
    FILE *in;
    in=fopen(txt,"r");
    if(in==NULL)
    {
        printf("fisierul %s nu s-a putut deschide\n",txt);
        exit(0);
    }
    unsigned int seed;
    unsigned char *h;
    unsigned int *r;
    struct pixel *l;
    unsigned int *p;
    antet(poza,&h);
    union numere a,b;
    a.c[0]=h[18];
    a.c[1]=h[19];
    a.c[2]=h[20];
    a.c[3]=h[21];
    b.c[0]=h[22];
    b.c[1]=h[23];
    b.c[2]=h[24];
    b.c[3]=h[25];
    fscanf(in,"%u",&seed);
    liniarizare(poza,&l);
    generator(2*a.x*b.x-1,&r,seed);
    int i,k,aux;
    p=(int *)malloc(a.x*b.x*sizeof(unsigned int));
    for(i=0;i<a.x*b.x;i++)
        p[i]=i;
    for(i=a.x*b.x-1;i>=1;i--)
    {
        k=r[a.x*b.x-i]%(i+1);
        aux=p[k];
        p[k]=p[i];
        p[i]=aux;
    }
    struct pixel *L=(struct pixel *)malloc(a.x*b.x*sizeof(struct pixel));
    for(i=0;i<a.x*b.x;i++)
        L[p[i]]=l[i];
     for(i=0;i<a.x*b.x;i++)
        l[i]=L[i];
    free(L); free(p);
    union numere SV;
    fscanf(in,"%u",&SV.x);
    l[0].R=(l[0].R^SV.c[2]);
    l[0].G=(l[0].G^SV.c[1]);
    l[0].B=(l[0].B^SV.c[0]);
    SV.x=r[a.x*b.x];
    l[0].R=(l[0].R^SV.c[2]);
    l[0].G=(l[0].G^SV.c[1]);
    l[0].B=(l[0].B^SV.c[0]);
    for(i=1;i<a.x*b.x;i++)
    {
        l[i].R=l[i-1].R^l[i].R;
        l[i].G=l[i-1].G^l[i].G;
        l[i].B=l[i-1].B^l[i].B;
        SV.x=r[a.x*b.x+i];
        l[i].R=(l[i].R^SV.c[2]);
        l[i].G=(l[i].G^SV.c[1]);
        l[i].B=(l[i].B^SV.c[0]);
    }
    reconstituire(l,h,cript);
    free(l);
    free(h);
    free(r);
    fclose(in);
}
void decriptare(char *poza,char *cript,char *txt)
{
    FILE *in;
    in=fopen(txt,"r");
    if(in==NULL)
    {
        printf("fisierul %s nu s-a putut deschide\n",txt);
        return;
    }
    unsigned int seed;
    unsigned char *h;
    unsigned int *r;
    struct pixel *l;
    unsigned int *p;
    antet(cript,&h);
    union numere a,b;
    a.c[0]=h[18];
    a.c[1]=h[19];
    a.c[2]=h[20];
    a.c[3]=h[21];
    b.c[0]=h[22];
    b.c[1]=h[23];
    b.c[2]=h[24];
    b.c[3]=h[25];
    fscanf(in,"%u",&seed);
    generator(2*a.x*b.x-1,&r,seed);
    liniarizare(cript,&l);
    int i,k,aux;
    p=(int *)malloc(a.x*b.x*sizeof(unsigned int));
    for(i=0;i<a.x*b.x;i++)
        p[i]=i;
    for(i=a.x*b.x-1;i>=1;i--)
    {
        k=r[a.x*b.x-i]%(i+1);
        aux=p[k];
        p[k]=p[i];
        p[i]=aux;
    }
    union numere SV;
    fscanf(in,"%u",&SV.x);
    for(i=a.x*b.x-1;i>=1;i--)
    {
        l[i].R=l[i-1].R^l[i].R;
        l[i].G=l[i-1].G^l[i].G;
        l[i].B=l[i-1].B^l[i].B;
        SV.x=r[a.x*b.x+i];
        l[i].R=(l[i].R^SV.c[2]);
        l[i].G=(l[i].G^SV.c[1]);
        l[i].B=(l[i].B^SV.c[0]);
    }
    l[0].R=(l[0].R^SV.c[2]);
    l[0].G=(l[0].G^SV.c[1]);
    l[0].B=(l[0].B^SV.c[0]);
    SV.x=r[a.x*b.x];
    l[0].R=(l[0].R^SV.c[2]);
    l[0].G=(l[0].G^SV.c[1]);
    l[0].B=(l[0].B^SV.c[0]);
    struct pixel *L=(struct pixel *)malloc(a.x*b.x*sizeof(struct pixel));
    for(i=0;i<a.x*b.x;i++)
        L[i]=l[p[i]];
     for(i=0;i<a.x*b.x;i++)
        l[i]=L[i];
    free(L); free(p);
    reconstituire(l,h,poza);
    free(r);
    free(l);
    free(h);
    fclose(in);
}
void chi_patrat(char *poza)
{
    struct pixel *l;
    unsigned char *h;
    liniarizare(poza,&l);
    antet(poza,&h);
    int i;
    int *R,*G,*B;
    B=(int *)calloc(256,sizeof(int));
    G=(int *)calloc(256,sizeof(int));
    R=(int *)calloc(256,sizeof(int));
    double sumaB=0,sumaG=0,sumaR=0;
    double fr;
    union numere a,b;
    a.c[0]=h[18];
    a.c[1]=h[19];
    a.c[2]=h[20];
    a.c[3]=h[21];
    b.c[0]=h[22];
    b.c[1]=h[23];
    b.c[2]=h[24];
    b.c[3]=h[25];
    fr=(a.x*b.x)/256.0000;
    for(i=0;i<a.x*b.x;i++)
    {
        B[l[i].B]++;
        G[l[i].G]++;
        R[l[i].R]++;
    }
    for(i=0;i<=255;i++)
    {
        sumaR=sumaR+((R[i]-fr)*(R[i]-fr))/fr;
        sumaB=sumaB+((B[i]-fr)*(B[i]-fr))/fr;
        sumaG=sumaG+((G[i]-fr)*(G[i]-fr))/fr;
    }
    free(R);
    free(G);
    free(B);
    free(l);
    free(h);
    printf("Chi-squared test on RGB channels for %s:\nB:%f\nG:%f\nR:%f\n",poza,sumaB,sumaG,sumaR);
}
void fereastra(int l,int c,struct pixel C,struct pixel **lin,int W,int H)
{
    int i;
    int x,y;
    x=l-7;y=l+7;
    for(i=0;i<11;i++)
    {
        (*lin)[x*W+c-5+i]=C;
        (*lin)[y*W+c-5+i]=C;
    }
    x=c-5;y=c+5;
    for(i=1;i<14;i++)
    {
        (*lin)[(l-7+i)*W+x]=C;
        (*lin)[(l-7+i)*W+y]=C;
    }
}
int corr(char *I, char *S, double ps,int *n,struct fereastra **v,int cifra)
{
    FILE *in1,*in2;
    in1=fopen(I,"rb");
    in2=fopen(S,"rb");
    if(in1==NULL)
    {
        printf("fisierul %s nu s-a putut deschide\n",I);
        return;
    }
    if(in2==NULL)
    {
        printf("fisierul %s nu s-a putut deschide\n",S);
        return;
    }
    int i,j,k;
    unsigned char *h;
    struct pixel *linI;
    liniarizare(I,&linI);
    struct pixel *linS;
    liniarizare(S,&linS);
    union numere a,b;
    antet(I,&h);
    a.c[0]=h[18];
    a.c[1]=h[19];
    a.c[2]=h[20];
    a.c[3]=h[21];
    b.c[0]=h[22];
    b.c[1]=h[23];
    b.c[2]=h[24];
    b.c[3]=h[25];
    double s=0;
    for(i=0;i<165;i++)
        s=s+linS[i].R;
    s=s/165;
    double devS=0;
    for(i=0;i<165;i++)
        devS=devS+(linS[i].R-s)*(linS[i].R-s);
    devS=devS/164;
    devS=sqrt(devS);
    for(i=0;i<a.x*b.x;i++)
    {
        int l,c;
        l=i/a.x;
        c=i%a.x;
        int x,y;
        if(l-7>=0 && l+7<b.x && c-5>=0 && c+5<a.x)
        {
            double f=0,devf=0;
            x=l-7;
            for(j=0;j<15;j++)
            {
                y=c-5;
                for(k=0;k<11;k++)
                {
                    f=f+linI[x*a.x+y].R;
                    y++;
                }
                x++;
            }
            f=f/165;
            x=l-7;
            for(j=0;j<15;j++)
            {
                y=c-5;
                for(k=0;k<11;k++)
                {
                    devf=devf+(linI[x*a.x+y].R-f)*(linI[x*a.x+y].R-f);
                    y++;
                }
                x++;
            }
            devf=devf/164;
            devf=sqrt(devf);
            double cor=0;
            x=l-7;
            for(j=0;j<15;j++)
            {
                y=c-5;
                for(k=0;k<11;k++)
                {
                    cor=cor+((linI[x*a.x+y].R-f)*(linS[j*11+k].R-s))/(devf*devS);
                    y++;
                }
                x++;
            }
            cor=cor/165;
            if(cor>ps)
            {
                (*n)++;
                struct fereastra *aux;
                aux=(struct fereastra *)realloc((*v),(*n)*sizeof(struct fereastra));
                if(aux==NULL)
                {
                    printf("mrmorie insuficienta");
                    return;
                }
                (*v)=aux;
                (*v)[(*n)-1].c=c;
                (*v)[(*n)-1].l=l;
                (*v)[(*n)-1].cifra=cifra;
                (*v)[(*n)-1].cor=cor;
            }
        }
    }
    free(linS); free(linI); free(h);
    fclose(in1);
    fclose(in2);
}
void grayscale(char *poza,char *gray)
{
    int i,j;
    FILE *in,*out;
    in=fopen(poza,"rb");
    if(in==NULL)
    {
        printf("eroareGRAY");
        return;
    }
    out=fopen(gray,"wb");
    if(out==NULL)
    {
        printf("eroareGRAY02.0");
        return;
    }
    unsigned char R,G,B,x;
    for(i=0;i<54;i++)
    {
        fread(&x,sizeof(unsigned char),1,in);
        fwrite(&x,sizeof(unsigned char),1,out);
    }
    unsigned int H,W;
    fseek(in,18*sizeof(char),SEEK_SET);
    fread(&W,sizeof(unsigned int),1,in);
    fseek(in,22*sizeof(char),SEEK_SET);
    fread(&H,sizeof(unsigned int),1,in);
    fseek(in,54*sizeof(char),SEEK_SET);
    for(i=0;i<H;i++)
    {
        for(j=0;j<W;j++)
        {
            fread(&B,sizeof(unsigned char),1,in);
            fread(&G,sizeof(unsigned char),1,in);
            fread(&R,sizeof(unsigned char),1,in);
            x=0.299*R + 0.587*G + 0.114*B;
            B=G=R=x;
            fwrite(&B,sizeof(unsigned char),1,out);
            fwrite(&G,sizeof(unsigned char),1,out);
            fwrite(&R,sizeof(unsigned char),1,out);
        }
        for(j=0;j<(4-(3*W)%4)%4;j++)
        {
            fread(&x,sizeof(unsigned char),1,in);
            fwrite(&x,sizeof(unsigned char),1,out);
        }
    }
    fclose(in);
    fclose(out);
}
int comp(const void *a,const void *b)
{
    struct fereastra va=*(struct fereastra *)a;
    struct fereastra vb=*(struct fereastra *)b;
    if(va.cor<vb.cor) return 1;
    if(va.cor>vb.cor) return -1;
    return 0;
}
int suprapunere(int f1_l,int f1_c,int f2_l,int f2_c)
{
    double l,c;
    if(abs(f1_c-f2_c)<12)
        c=12-abs(f1_c-f2_c)-1;
    else c=0;
    if(abs(f1_l-f2_l)<16)
        l=16-abs(f1_l-f2_l);
    else l=0;
    double x;
    x=(l*c)/(2*11*15-l*c);
    if(x<0.2) return 0;
    return 1;
}
void eliminare(int *n, struct fereastra **v)
{
    int i,j,k;
    for(i=0;i<(*n)-1;i++)
        for(j=i+1;j<(*n);j++)
        {
            if(suprapunere((*v)[i].l,(*v)[i].c,(*v)[j].l,(*v)[j].c))
            {
                for(k=j;k<(*n)-1;k++)
                    (*v)[k]=(*v)[k+1];
                (*n)--;
                j--;
            }
        }
}
int main()
{
    char *nume,*cript,*cheie,*numedecript;
    nume=(char *)malloc(20*sizeof(char));
    cript=(char *)malloc(20*sizeof(char));
    cheie=(char *)malloc(20*sizeof(char));
    numedecript=(char *)malloc(20*sizeof(char));
    printf("numele pozei de criptat este: ");
    fgets(nume, 20, stdin);
    nume[strlen(nume) - 1] = '\0';
    printf("numele fisierului unde se memoreaza poza criptata este: ");
    fgets(cript, 20, stdin);
    cript[strlen(cript) - 1] = '\0';
    printf("numele fisierului ce contine cheia secreta este :");
    fgets(cheie, 20, stdin);
    cheie[strlen(cheie) - 1] = '\0';
    criptare(nume,cript,cheie);
    printf("numele fisierului unde se memoreaza poza decriptata este:");
    fgets(numedecript, 20, stdin);
    numedecript[strlen(numedecript) - 1] = '\0';
    decriptare(numedecript,cript,cheie);
    free(nume);free(cheie); free(cript);
    char *imagine,*reuniune,*fin;
    imagine=(char *)malloc(20*sizeof(char));
    reuniune=(char *)malloc(20*sizeof(char));
    fin=(char *)malloc(20*sizeof(char));
    printf("numele pozei pentru template matching este: ");
    fgets(nume, 20, stdin);
    nume[strlen(nume) - 1] = '\0';
    struct fereastra *cor=NULL;
    struct pixel *l;
    unsigned char *h;
    liniarizare(imagine,&l);
    antet(imagine,&h);
    union numere a,b;
    a.c[0]=h[18];
    a.c[1]=h[19];
    a.c[2]=h[20];
    a.c[3]=h[21];
    b.c[0]=h[22];
    b.c[1]=h[23];
    b.c[2]=h[24];
    b.c[3]=h[25];
    int n=0;
    int i;
    grayscale(imagine,"gri.bmp");
    for(i=0;i<=9;i++)
    {
        char nume_sablon[] = "cifra0.bmp";
        nume_sablon[5]=nume_sablon[5]+i;;
        grayscale(nume_sablon,"cifra_g.bmp");
        corr("gri.bmp","cifra_g.bmp",0.5,&n,&cor,i);
    }
    struct pixel C;
    qsort(cor,n,sizeof(struct fereastra),comp);
    for(i=0;i<n;i++)
    {
        if(cor[i].cifra==0) {C.R=255;C.G=0;C.B=0;}
        if(cor[i].cifra==1) {C.R=255;C.G=255;C.B=0;}
        if(cor[i].cifra==2) {C.R=0;C.G=255;C.B=0;}
        if(cor[i].cifra==3) {C.R=0;C.G=255;C.B=255;}
        if(cor[i].cifra==4) {C.R=255;C.G=0;C.B=255;}
        if(cor[i].cifra==5) {C.R=0;C.G=0;C.B=255;}
        if(cor[i].cifra==6) {C.R=192;C.G=192;C.B=192;}
        if(cor[i].cifra==7) {C.R=255;C.G=140;C.B=0;}
        if(cor[i].cifra==8) {C.R=128;C.G=0;C.B=128;}
        if(cor[i].cifra==9) {C.R=128;C.G=0;C.B=0;}
        fereastra(cor[i].l,cor[i].c,C,&l,a.x,b.x);
    }
    printf("numele pozei cu reuniunea ferestrelor este: ");
    fgets(reuniune, 20, stdin);
    reuniune[strlen(reuniune) - 1] = '\0';
    reconstituire(l,h,reuniune);
    free(l);
    liniarizare(imagine,&l);
    eliminare(&n,&cor);
    for(i=0;i<n;i++)
    {
        if(cor[i].cifra==0) {C.R=255;C.G=0;C.B=0;}
        if(cor[i].cifra==1) {C.R=255;C.G=255;C.B=0;}
        if(cor[i].cifra==2) {C.R=0;C.G=255;C.B=0;}
        if(cor[i].cifra==3) {C.R=0;C.G=255;C.B=255;}
        if(cor[i].cifra==4) {C.R=255;C.G=0;C.B=255;}
        if(cor[i].cifra==5) {C.R=0;C.G=0;C.B=255;}
        if(cor[i].cifra==6) {C.R=192;C.G=192;C.B=192;}
        if(cor[i].cifra==7) {C.R=255;C.G=140;C.B=0;}
        if(cor[i].cifra==8) {C.R=128;C.G=0;C.B=128;}
        if(cor[i].cifra==9) {C.R=128;C.G=0;C.B=0;}
        fereastra(cor[i].l,cor[i].c,C,&l,a.x,b.x);
    }
    printf("numele pozei cu ferestrele dupa eliminarea non maximelor este: ");
    fgets(fin, 20, stdin);
    fin[strlen(fin) - 1] = '\0';
    reconstituire(l,h,fin);
    free(l); free(h);
    free(imagine);
    free(reuniune);
    free(fin);
    return 0;
}
