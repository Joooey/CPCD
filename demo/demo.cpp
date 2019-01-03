/* 
 * ����PL/0Դ�����ļ���
 * �Ƿ�������������
 * �Ƿ�������ű�
 * code.txt������������
 * output.txt���Դ�ļ���������ʾ�����д��͸��ж�Ӧ�����ɴ����׵�ַ
 * result.txt������н��
 * table.txt������ű�
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define bool int
#define true 1
#define false 0

#define norw 13       /* �����ָ��� */
#define txmax 100     /* ���ű����� */
#define nmax 14       /* ���ֵ����λ�� */
#define al 10         /* ��ʶ������󳤶� */
#define maxerr 30     /* ������������� */
#define amax 2048     /* ��ַ�Ͻ�*/
#define levmax 3      /* ����������Ƕ����������*/
#define cxmax 200     /* ��������������� */
#define stacksize 500 /* ����ʱ����ջԪ�����Ϊ500�� */

/* ���� */
enum symbol {
    nul,         ident,     number,     plus,      minus, 
    times,       slash,     oddsym,     eql,       neq, 
    lss,         leq,       gtr,        geq,       lparen, 
    rparen,      comma,     semicolon,  period,    becomes, 
    beginsym,    endsym,    ifsym,      thensym,   whilesym, 
    writesym,    readsym,   dosym,      callsym,   constsym, 
    varsym,      procsym,   
};
#define symnum 32

/* ���ű��е����� */
enum object {
    constant, 
    variable, 
    procedure,
};

/* Ŀ�����ָ�� */
enum fct {
    lit,     opr,     lod, 
    sto,     cal,     ini, 
    jmp,     jpc,     
};
#define fctnum 8

/* Ŀ����� */
struct instruction
{
	enum fct f; /* ���������ָ�� */
	int l;      /* ���ò���������Ĳ�β� */
	int a;      
};

/* ���ű�ṹ */
struct tablestruct
{
	char name[al];	    /* ���� */
	enum object kind;	/* ���ͣ�const��var��procedure */
	int val;            /* ��ֵ����constʹ�� */
	int level;          /* �����㣬��const��ʹ�� */
	int adr;            /* ��ַ����const��ʹ�� */
	int size;           /* ��Ҫ������������ռ�, ��procedureʹ�� */
};

struct tablestruct table[txmax]; /* ���ű� */

FILE* fin;      /* ����Դ�ļ� */
FILE* ftable;	/* ������ű� */
FILE* fcode;    /* ������������ */
FILE* foutput;  /* ����ļ�������ʾ�⣨���д������ж�Ӧ�����ɴ����׵�ַ�����޴� */
FILE* fresult;  /* ���ִ�н�� */
char fname[al];
int err;        /* ��������� */

bool listswitch ;   /* ��ʾ������������ */
bool tableswitch ;  /* ��ʾ���ű���� */
char ch;            /* ��ŵ�ǰ��ȡ���ַ���getch ʹ�� */
enum symbol sym;    /* ��ǰ�ķ��� */
char id[al+1];      /* ��ǰident�������һ���ֽ����ڴ��0 */
int num;            /* ��ǰnumber */
int cc, ll;         /* getchʹ�õļ�������cc��ʾ��ǰ�ַ�(ch)��λ�� */
int cx;             /* ���������ָ��, ȡֵ��Χ[0, cxmax-1]*/
char line[81];      /* ��ȡ�л����� */
char a[al+1];       /* ��ʱ���ţ������һ���ֽ����ڴ��0 */
struct instruction code[cxmax]; /* ����������������� */
char word[norw][al];        /* ������ */
enum symbol wsym[norw];     /* �����ֶ�Ӧ�ķ���ֵ */
enum symbol ssym[256];      /* ���ַ��ķ���ֵ */
char mnemonic[fctnum][5];   /* ���������ָ������ */
bool declbegsys[symnum];    /* ��ʾ������ʼ�ķ��ż��� */
bool statbegsys[symnum];    /* ��ʾ��俪ʼ�ķ��ż��� */
bool facbegsys[symnum];     /* ��ʾ���ӿ�ʼ�ķ��ż��� */

void error(int n); 
void getsym();
void getch();
void init();
void gen(enum fct x, int y, int z);
void test(bool* s1, bool* s2, int n);
int inset(int e, bool* s);
int addset(bool* sr, bool* s1, bool* s2, int n);
int subset(bool* sr, bool* s1, bool* s2, int n);
int mulset(bool* sr, bool* s1, bool* s2, int n);
void block(int lev, int tx, bool* fsys);
void interpret();
void factor(bool* fsys, int* ptx, int lev);
void term(bool* fsys, int* ptx, int lev);
void condition(bool* fsys, int* ptx, int lev);
void expression(bool* fsys, int* ptx, int lev);
void statement(bool* fsys, int* ptx, int lev);
void listcode(int cx0);
void listall();
void vardeclaration(int* ptx, int lev, int* pdx);
void constdeclaration(int* ptx, int lev, int* pdx);
int position(char* idt, int tx);
void enter(enum object k, int* ptx, int lev, int* pdx);
int base(int l, int* s, int b);



int main()
{
    bool nxtlev[symnum];
    
	printf("Input the sourcefile name��");
	scanf("%s", fname);		// �����ļ���  

	if ((fin = fopen(fname, "r")) == NULL)
	{
		printf("Can't open the input file!\n");
		exit(1);
	}

	ch = fgetc(fin);
	if (ch == EOF)
	{
		printf("The input file is empty!\n");
		fclose(fin);
		exit(1);
	}
	rewind(fin);

	if ((foutput = fopen("output.txt", "w")) == NULL)
	{
		printf("Can't open the output file!\n");
		exit(1);
	}

	if ((ftable = fopen("table.txt", "w")) == NULL)
	{
		printf("Can't open ftable.txt file!\n");
		exit(1);
	}

	printf("List object codes?(Y/N)");	/* �Ƿ������������� */
	scanf("%s", fname);
	listswitch = (fname[0]=='y' || fname[0]=='Y');

	printf("List symbol table?(Y/N)");	/* �Ƿ�������ű� */
	scanf("%s", fname);
	tableswitch = (fname[0]=='y' || fname[0]=='Y');        

    init();		//��ʼ��
	err = 0;
	cc = ll = cx = 0;
	ch = ' ';

	getsym();//�ʷ����� 
		
	addset(nxtlev, declbegsys, statbegsys, symnum);
	nxtlev[period] = true;
	block(0, 0, nxtlev);	/* ����ֳ��� */
		
	if (sym != period)
    {
		error(9);
    }

    if (err == 0)
    {
		printf("\n===Parsing success!===\n");
		fprintf(foutput,"\n===Parsing success!===\n");

		if ((fcode = fopen("code.txt", "w")) == NULL)
		{
			printf("Can't open fcode.txt file!\n");
			exit(1);
		}		

		if ((fresult = fopen("result.txt", "w")) == NULL)
		{
			printf("Can't open fresult.txt file!\n");
			exit(1);
		}
		
		listall();	 /* ������д��� */		
		fclose(fcode);

        interpret();	/* ���ý���ִ�г��� */        	
		fclose(fresult);
      }
	else
	{
		printf("\n%d errors in pl/0 program!\n",err);
		fprintf(foutput,"\n%d errors in pl/0 program!\n",err);
	}
		
    fclose(ftable);
	fclose(foutput);
	fclose(fin);

	return 0;
}


/*
 * ������ʵ�ּ��ϵļ������� 
 */
int inset(int e, bool* s)
{
	return s[e];
}

int addset(bool* sr, bool* s1, bool* s2, int n)
{
	int i;
	for (i=0; i<n; i++)
	{
		sr[i] = s1[i]||s2[i];
	}
	return 0;
}

int subset(bool* sr, bool* s1, bool* s2, int n)
{
	int i;
	for (i=0; i<n; i++)
	{
		sr[i] = s1[i]&&(!s2[i]);
	}
	return 0;
}

int mulset(bool* sr, bool* s1, bool* s2, int n)
{
	int i;
	for (i=0; i<n; i++)
	{
		sr[i] = s1[i]&&s2[i];
	}
	return 0;
}

/*
 *	��������ӡ����λ�úʹ������
 */
void error(int n)
{
	char space[81];
	memset(space,32,81);

	space[cc-1]=0; /* ����ʱ��ǰ�����Ѿ����꣬����cc-1 */

	printf("**%s^%d\n", space, n);
	fprintf(foutput,"**%s^%d\n", space, n);

	err = err + 1;
	if (err > maxerr)
	{
		exit(1);
	}
}


/* 
 * �ʷ���������ȡһ������
 */
void getsym()
{
	int i,j,k;

	while (ch == ' ' || ch == 10 || ch == 9)	/* ���˿ո񡢻��к��Ʊ�� */
	{
		getch();
	}
	if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) /* ��ǰ�ĵ����Ǳ�ʶ�����Ǳ����� */
	{			
		k = 0;
		do {
			if(k < al)
			{
				a[k] = ch;
				k++;
			}
			getch();
		} while ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9'));
		a[k] = 0;
		strcpy(id, a);
		i = 0;
		j = norw - 1;
		do {    /* ������ǰ�����Ƿ�Ϊ�����֣�ʹ�ö��ַ����� */
			k = (i + j) / 2;
			if (strcmp(id,word[k]) <= 0)
			{
			    j = k - 1;
			}
			if (strcmp(id,word[k]) >= 0)
			{
			    i = k + 1;
			}
		} while (i <= j);
		if (i-1 > j) /* ��ǰ�ĵ����Ǳ����� */
		{
		    sym = wsym[k];
		}
		else /* ��ǰ�ĵ����Ǳ�ʶ�� */
		{
		    sym = ident; 
		}
	}
	else
	{
		if (ch >= '0' && ch <= '9') /* ��ǰ�ĵ��������� */
		{			
			k = 0;
			num = 0;
			sym = number;
			do {
				num = 10 * num + ch - '0';
				k++;
				getch();;
			} while (ch >= '0' && ch <= '9'); /* ��ȡ���ֵ�ֵ */
			k--;
			if (k > nmax) /* ����λ��̫�� */
			{
			    error(30);
			}
		}
		else
		{
			if (ch == ':')		/* ��⸳ֵ���� */
			{
				getch();
				if (ch == '=')
				{
					sym = becomes;
					getch();
				}
				else
				{
					sym = nul;	/* ����ʶ��ķ��� */
				}
			}
			else
			{
				if (ch == '<')		/* ���С�ڻ�С�ڵ��ڷ��� */
				{
					getch();
					if (ch == '=')
					{
						sym = leq;
						getch();
					}
					else
					{
						sym = lss;
					}
				}
				else
				{
					if (ch == '>')		/* �����ڻ���ڵ��ڷ��� */
					{
						getch();
						if (ch == '=')
						{
							sym = geq;
							getch();
						}
						else
						{
							sym = gtr;
						}
					}
					else
					{
						sym = ssym[ch];		/* �����Ų�������������ʱ��ȫ�����յ��ַ����Ŵ��� */                   
                        if (sym != period)  
                        {
                            getch();        
                        }
                   
					}
				}
			}
		}
	}	
}


/* 
 * ���Ե�ǰ�����Ƿ�Ϸ�
 *
 * ���﷨�����������ںͳ��ڴ����ò��Ժ���test��
 * ��鵱ǰ���ʽ�����˳����﷨��λ�ĺϷ���
 *
 * s1:	��Ҫ�ĵ��ʼ���
 * s2:	���������Ҫ�ĵ��ʣ���ĳһ����״̬ʱ��
 *      �ɻָ��﷨�����������������Ĳ��䵥�ʷ��ż���
 * n:  	�����
 */
void test(bool* s1, bool* s2, int n)
{
	if (!inset(sym, s1))
	{
		error(n);
		/* ����ⲻͨ��ʱ����ͣ��ȡ���ţ�ֱ����������Ҫ�ļ��ϻ򲹾ȵļ��� */
		while ((!inset(sym,s1)) && (!inset(sym,s2)))
		{
			getsym();
		}
	}	
}

/* 
 * �����������
 *
 * lev:    ��ǰ�ֳ������ڲ�
 * tx:     ���ű�ǰβָ��
 * fsys:   ��ǰģ���̷��ż���
 */
void block(int lev, int tx, bool* fsys)
{
	int i;

	int dx;                 /* ��¼���ݷ������Ե�ַ */
	int tx0;                /* ������ʼtx */
	int cx0;                /* ������ʼcx */
	bool nxtlev[symnum];    /* ���¼������Ĳ����У����ż��Ͼ�Ϊֵ�Σ�������ʹ������ʵ�֣�
	                           ���ݽ�������ָ�룬Ϊ��ֹ�¼������ı��ϼ������ļ��ϣ������µĿռ�
	                           ���ݸ��¼�����*/
    
	dx = 3;                 /* �����ռ����ڴ�ž�̬��SL����̬��DL�ͷ��ص�ַRA  */
	tx0 = tx;		        /* ��¼�����ʶ���ĳ�ʼλ�� */
	table[tx].adr = cx;	    /* ��¼��ǰ�����Ŀ�ʼλ�� */
    gen(jmp, 0, 0);         /* ������תָ���תλ��δ֪��0 */

	if (lev > levmax)		/* Ƕ�ײ������� */
    {
        error(32);
    }

	do {

		if (sym == constsym)	/* ���������������ţ���ʼ���������� */
		{
			getsym();

			do {
			    constdeclaration(&tx, lev, &dx);	/* dx��ֵ�ᱻconstdeclaration�ı䣬ʹ��ָ�� */
		        while (sym == comma)  /* �������ż������峣�� */
				{
					getsym();
					constdeclaration(&tx, lev, &dx);
				}
				if (sym == semicolon) /* �����ֺŽ������峣�� */
				{
					getsym();
				}
				else 
                {
                    error(5);   /* ©���˷ֺ� */
                }
			} while (sym == ident);
		}

		if (sym == varsym)		/* ���������������ţ���ʼ����������� */
		{
			getsym();

			do {
			    vardeclaration(&tx, lev, &dx);
				while (sym == comma) 
				{
					getsym();
					vardeclaration(&tx, lev, &dx);
				}
				if (sym == semicolon)
				{
					getsym();
				}
				else
                {
                    error(5); /* ©���˷ֺ� */
                }
			} while (sym == ident);
		}

		while (sym == procsym) /* ���������������ţ���ʼ����������� */
		{
			getsym();

			if (sym == ident)
			{
				enter(procedure, &tx, lev, &dx);	/* ��д���ű� */
				getsym();
			}
            else 
            {
                error(4);	/* procedure��ӦΪ��ʶ�� */
            }

			if (sym == semicolon)
			{
				getsym();
			}
			else 
            {
                error(5);	/* ©���˷ֺ� */
            }

			memcpy(nxtlev, fsys, sizeof(bool) * symnum);
			nxtlev[semicolon] = true;
			block(lev + 1, tx, nxtlev); /* �ݹ���� */
            
			if(sym == semicolon)
			{
				getsym();
				memcpy(nxtlev, statbegsys, sizeof(bool) * symnum);
				nxtlev[ident] = true;
				nxtlev[procsym] = true;
				test(nxtlev, fsys, 6);
			}
			else 
            {
                error(5);	/* ©���˷ֺ� */
            }
		}
		memcpy(nxtlev, statbegsys, sizeof(bool) * symnum);
		nxtlev[ident] = true;
		test(nxtlev, declbegsys, 7);
	} while (inset(sym, declbegsys));	/* ֱ��û���������� */

	code[table[tx0].adr].a = cx;	/* ��ǰ�����ɵ���ת������תλ�øĳɵ�ǰλ�� */
	table[tx0].adr = cx;	        /* ��¼��ǰ���̴����ַ */
	table[tx0].size = dx;	        /* ����������ÿ����һ�����������dx����1�����������Ѿ�������dx���ǵ�ǰ�������ݵ�size */
	cx0 = cx;                       
	gen(ini, 0, dx);	            /* ����ָ���ָ��ִ��ʱ������ջ��Ϊ�����õĹ��̿���dx����Ԫ�������� */

	if (tableswitch)		/* ������ű� */
	{
		for (i = 1; i <= tx; i++)
		{
			switch (table[i].kind)
			{
				case constant:
					printf("    %d const %s ", i, table[i].name);
					printf("val=%d\n", table[i].val);
					fprintf(ftable, "    %d const %s ", i, table[i].name);
					fprintf(ftable, "val=%d\n", table[i].val);
					break;
				case variable:
					printf("    %d var   %s ", i, table[i].name);
					printf("lev=%d addr=%d\n", table[i].level, table[i].adr);
					fprintf(ftable, "    %d var   %s ", i, table[i].name);
					fprintf(ftable, "lev=%d addr=%d\n", table[i].level, table[i].adr);
					break;
				case procedure:
					printf("    %d proc  %s ", i, table[i].name);
					printf("lev=%d addr=%d size=%d\n", table[i].level, table[i].adr, table[i].size);
					fprintf(ftable,"    %d proc  %s ", i, table[i].name);
					fprintf(ftable,"lev=%d addr=%d size=%d\n", table[i].level, table[i].adr, table[i].size);
					break;
			}
		}
		printf("\n");
		fprintf(ftable,"\n");
	}

	/* ����̷���Ϊ�ֺŻ�end */
	memcpy(nxtlev, fsys, sizeof(bool) * symnum);	/* ÿ����̷��ż��϶������ϲ��̷��ż��ϣ��Ա㲹�� */
	nxtlev[semicolon] = true;
	nxtlev[endsym] = true;
	statement(nxtlev, &tx, lev);
	gen(opr, 0, 0);	                    /* ÿ�����̳��ڶ�Ҫʹ�õ��ͷ����ݶ�ָ�� */
	memset(nxtlev, 0, sizeof(bool) * symnum);	/* �ֳ���û�в��ȼ��� */
	test(fsys, nxtlev, 8);            	/* ����̷�����ȷ�� */
	listcode(cx0);                      /* ������ֳ������ɵĴ��� */
}

/* 
 * ���ұ�ʶ���ڷ��ű��е�λ�ã���tx��ʼ������ұ�ʶ��
 * �ҵ��򷵻��ڷ��ű��е�λ�ã����򷵻�0
 * 
 * id:    Ҫ���ҵ�����
 * tx:    ��ǰ���ű�βָ��
 */
int position(char* id, int tx)
{
	int i;
	strcpy(table[0].name, id);
	i = tx;
	while (strcmp(table[i].name, id) != 0)
    {
        i--;
    }
	return i;
}

/*
 * ������������ 
 */
void constdeclaration(int* ptx, int lev, int* pdx)
{
	if (sym == ident)
	{
		getsym();
		if (sym == eql || sym == becomes)
		{
			if (sym == becomes)
            {
                error(1);	/* ��=д����:= */
            }
			getsym();
			if (sym == number)
			{
				enter(constant, ptx, lev, pdx);
				getsym();
			}
			else
            {
                error(2);	/* ���������е�=��Ӧ������ */
            }
		}
		else 
        {
            error(3);	/* ���������еı�ʶ����Ӧ��= */
        }
	}
	else 
    {
        error(4);	/* const��Ӧ�Ǳ�ʶ�� */
    }
}

/*
 * ������������ 
 */
void vardeclaration(int* ptx,int lev,int* pdx)
{
	if (sym == ident)
	{
		enter(variable, ptx, lev, pdx);	// ��д���ű�
		getsym();
	}
    else 
	{
		error(4);	/* var����Ӧ�Ǳ�ʶ�� */
	}
}


/*
 * �������Ŀ����� 
 */
void listall()
{
	int i;
	if (listswitch)
	{
		for (i = 0; i < cx; i++)
		{
			printf("%d %s %d %d\n", i, mnemonic[code[i].f], code[i].l, code[i].a);
			fprintf(fcode,"%d %s %d %d\n", i, mnemonic[code[i].f], code[i].l, code[i].a);
		}
	}
}

/*
 * ��� 
 */
void term(bool* fsys, int* ptx, int lev)
{
	enum symbol mulop;	/* ���ڱ���˳������� */
	bool nxtlev[symnum];

	memcpy(nxtlev, fsys, sizeof(bool) * symnum);
	nxtlev[times] = true;
	nxtlev[slash] = true;
	factor(nxtlev, ptx, lev);	/* �������� */
	while(sym == times || sym == slash)
	{
		mulop = sym;
		getsym();
		factor(nxtlev, ptx, lev);
		if(mulop == times)
		{
			gen(opr, 0, 4);	/* ���ɳ˷�ָ�� */
		}
		else
		{
			gen(opr, 0, 5);	/* ���ɳ���ָ�� */
		}
	}
}

/* 
 * ���Ӵ��� 
 */
void factor(bool* fsys, int* ptx, int lev)
{
	int i;
	bool nxtlev[symnum];
	test(facbegsys, fsys, 24);	/* ������ӵĿ�ʼ���� */
	while(inset(sym, facbegsys)) 	/* ѭ���������� */
	{
		if(sym == ident)	/* ����Ϊ��������� */
		{
			i = position(id, *ptx);	/* ���ұ�ʶ���ڷ��ű��е�λ�� */
			if (i == 0)
			{
				error(11);	/* ��ʶ��δ���� */
			}
			else
			{
				switch (table[i].kind)
				{
					case constant:	/* ��ʶ��Ϊ���� */
						gen(lit, 0, table[i].val);	/* ֱ�Ӱѳ�����ֵ��ջ */
						break;
					case variable:	/* ��ʶ��Ϊ���� */
						gen(lod, lev-table[i].level, table[i].adr);	/* �ҵ�������ַ������ֵ��ջ */
						break;
					case procedure:	/* ��ʶ��Ϊ���� */
						error(21);	/* ����Ϊ���� */
						break;
				}
			}
			getsym();
		}
		else
		{
			if(sym == number)	/* ����Ϊ�� */
			{
				if (num > amax)
				{
					error(31); /* ��Խ�� */
					num = 0;
				}
				gen(lit, 0, num); 
				getsym();
			}
			else
			{
				if (sym == lparen)	/* ����Ϊ���ʽ */
				{
					getsym();
					memcpy(nxtlev, fsys, sizeof(bool) * symnum);
					nxtlev[rparen] = true;
					expression(nxtlev, ptx, lev);
					if (sym == rparen)
					{
						getsym();
					}
					else 
					{
						error(22);	/* ȱ�������� */
					}
				}				
			}
		}
		memset(nxtlev, 0, sizeof(bool) * symnum);
		nxtlev[lparen] = true;
		test(fsys, nxtlev, 23); /* һ�����Ӵ�����ϣ������ĵ���Ӧ��fsys������ */
		                        /* ������ǣ������ҵ���һ�����ӵĿ�ʼ��ʹ�﷨�������Լ���������ȥ */
	}
}

/*
 * ���ͳ��� 
 */
void interpret()
{
	int p = 0; /* ָ��ָ�� */
	int b = 1; /* ָ���ַ */
	int t = 0; /* ջ��ָ�� */
	struct instruction i;	/* ��ŵ�ǰָ�� */
	int s[stacksize];	/* ջ */

	printf("Start pl0\n");
	fprintf(fresult,"Start pl0\n");
	s[0] = 0; /* s[0]���� */
	s[1] = 0; /* �������������ϵ��Ԫ����Ϊ0 */
	s[2] = 0;
	s[3] = 0;
	do {
	    i = code[p];	/* ����ǰָ�� */
		p = p + 1;
		switch (i.f)
		{
			case lit:	/* ������a��ֵȡ��ջ�� */
				t = t + 1;
				s[t] = i.a;				
				break;
			case opr:	/* ��ѧ���߼����� */
				switch (i.a)
				{
					case 0:  /* �������ý����󷵻� */
						t = b - 1;
						p = s[t + 3];
						b = s[t + 2];
						break;
					case 1: /* ջ��Ԫ��ȡ�� */
						s[t] = - s[t];
						break;
					case 2: /* ��ջ�������ջ���������ջԪ�أ����ֵ��ջ */
						t = t - 1;
						s[t] = s[t] + s[t + 1];
						break;
					case 3:/* ��ջ�����ȥջ���� */
						t = t - 1;
						s[t] = s[t] - s[t + 1];
						break;
					case 4:/* ��ջ�������ջ���� */
						t = t - 1;
						s[t] = s[t] * s[t + 1];
						break;
					case 5:/* ��ջ�������ջ���� */
						t = t - 1;
						s[t] = s[t] / s[t + 1];
						break;
					case 6:/* ջ��Ԫ�ص���ż�ж� */
						s[t] = s[t] % 2;
						break;
					case 8:/* ��ջ������ջ�����Ƿ���� */
						t = t - 1;
						s[t] = (s[t] == s[t + 1]);
						break;
					case 9:/* ��ջ������ջ�����Ƿ񲻵� */
						t = t - 1;
						s[t] = (s[t] != s[t + 1]);
						break;
					case 10:/* ��ջ�����Ƿ�С��ջ���� */
						t = t - 1;
						s[t] = (s[t] < s[t + 1]);
						break;
					case 11:/* ��ջ�����Ƿ���ڵ���ջ���� */
						t = t - 1;
						s[t] = (s[t] >= s[t + 1]);
						break;
					case 12:/* ��ջ�����Ƿ����ջ���� */
						t = t - 1;
						s[t] = (s[t] > s[t + 1]);
						break;
					case 13: /* ��ջ�����Ƿ�С�ڵ���ջ���� */
						t = t - 1;
						s[t] = (s[t] <= s[t + 1]);
						break;
					case 14:/* ջ��ֵ��� */
						printf("%d", s[t]);
						fprintf(fresult, "%d", s[t]);
						t = t - 1;
						break;
					case 15:/* ������з� */
						printf("\n");
						fprintf(fresult,"\n");
						break;
					case 16:/* ����һ����������ջ�� */
						t = t + 1;
						printf("?");
						fprintf(fresult, "?");
						scanf("%d", &(s[t]));
						fprintf(fresult, "%d\n", s[t]);						
						break;
				}
				break;
			case lod:	/* ȡ��Ե�ǰ���̵����ݻ���ַΪa���ڴ��ֵ��ջ�� */
				t = t + 1;
				s[t] = s[base(i.l,s,b) + i.a];				
				break;
			case sto:	/* ջ����ֵ�浽��Ե�ǰ���̵����ݻ���ַΪa���ڴ� */
				s[base(i.l, s, b) + i.a] = s[t];
				t = t - 1;
				break;
			case cal:	/* �����ӹ��� */
				s[t + 1] = base(i.l, s, b);	/* �������̻���ַ��ջ����������̬�� */
				s[t + 2] = b;	/* �������̻���ַ��ջ����������̬�� */
				s[t + 3] = p;	/* ����ǰָ��ָ����ջ�������淵�ص�ַ */
				b = t + 1;	/* �ı����ַָ��ֵΪ�¹��̵Ļ���ַ */
				p = i.a;	/* ��ת */
				break;
			case ini:	/* ������ջ��Ϊ�����õĹ��̿���a����Ԫ�������� */
				t = t + i.a;	
				break;
			case jmp:	/* ֱ����ת */
				p = i.a;
				break;
			case jpc:	/* ������ת */
				if (s[t] == 0) 
					p = i.a;
				t = t - 1;
				break;
		}
	} while (p != 0);
	printf("End pl0\n");
	fprintf(fresult,"End pl0\n");
}
