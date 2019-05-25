#include<stdio.h>
#define MAX 27
#define LENGTH 45
#include <ctype.h>
#include<ctype.h>


/*
26 lowercase letters. In this program,both capital and lowercase are the same.
so,we need to make the input string all into lowercase.
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/*
the data structure of TrieTree
each TrieTree has a pointer which has 26 positions to place abcdefg����xyz.

and a count int tyoe to store the the number of strings with the common prefix,
from the root node to the current node's string.
*/

typedef struct Node
{
	struct Node *next[MAX];
	//There may be MAX characters below each node.
	int count;
	//The number of strings with the common prefix from the root node to the current node's string
	int size;
	int isend;
}TrieNode,*TrieTree;

/*
create a TrieTree,which has only root node.
and we assign values to each child node.

return the root node.
*/

TrieTree create_rootTree()
{
	TrieTree rootTree = (TrieTree)malloc(sizeof(TrieNode));
	rootTree->count = 0;
	rootTree->size = 0;
	rootTree->isend = 0;
	int i;
	for(i=0;i<MAX;i++)
		rootTree->next[i] = NULL;
	return rootTree;
}

/*
�����ַ���str���ֵ���rootTree��
���ڲ����ܸı���ڵ㣬������ﲻ��Ҫ����pTree������
*/
void insert_TrieTree(TrieTree rootTree,char *str)
{
	int i;
	TrieTree pCur = rootTree;	//��ǰ���ʵĽڵ㣬��ʼָ����ڵ�
	rootTree->size++;//TODO
	int len = strlen(str);

	for(i=0;i<len;i++)
	{
        int id;
        if (str[i] == 39)
            id = 26;
        else
            id = tolower(str[i]) - 'a';	//��λ���ַ�Ӧ���ڵ�λ��

		if(!pCur->next[id])
		{	//������ַ�Ӧ���ڵ�λ��Ϊ�գ����ַ����뵽��λ��
			int j;

/*��������������������������������������������һ�������Ĵ��롪����������������������������������������������������������������������������������*/

			//�½�һ������
			TrieNode *pNew = (TrieNode *)malloc(sizeof(TrieNode));
			//һ�㲻���������ڴ�����ʧ��
			if(!pNew)
			{
				printf("pNew malloc fail\n");
				exit(-1);
			}
			//��������ĳ�ʼcount��Ȼ��1
			pNew->count = 1;
			//��������next��ʼ��һ��
			for(j=0;j<MAX;j++)
				pNew->next[j] = NULL;
            pNew->isend = 0;

/*��������������������������������������������������ĸ���Ĵ��롪������������������������������������������������������������������*/
            //�������ӵ�����
			pCur->next[id] = pNew;
			//ָ����ƣ��Ƚ���һ���ַ�
			pCur = pCur->next[id];//��ע��ָ��������������½�������������

		}
		else
		{	//������ַ�Ӧ���ڵ�λ�ò��գ�������Ƚ���һ���ַ�
			pCur = pCur->next[id];//Ҳ��һ���ģ�ָ������������������
			pCur->count++;	//ÿ����һ���ַ�������ǰ׺��Ŀ�ͼ�1
		}
	}
	pCur->isend = 1;//��ζ���սᣬ������һ������
}

/*
ͳ���ֵ���pTree����strΪǰ׺���ַ�������Ŀ
*/
int count_TrieTree(TrieTree rootTree,char *str)
{
	int i;
	TrieTree pCur = rootTree;
	int len = strlen(str);
	int id;
	for(i=0;i<len;i++)
	{
		//int id = tolower(str[i]) - 'a';
        if (str[i] == 39)
            id = 26;
        else
            id = tolower(str[i]) - 'a';	//��λ���ַ�Ӧ���ڵ�λ��

		if(!pCur->next[id])
			return 0;
		else
			pCur = pCur->next[id];
	}
	return pCur->count;
}

int size_TrieTree(TrieTree rootTree)
{

	return rootTree->size;
}

int find_TrieTree(TrieTree rootTree,char *str)
{
    int i;
	TrieTree pCur = rootTree;
	int len = strlen(str);
	int id;
	for(i=0;i<len;i++)
	{
		//int id = tolower(str[i]) - 'a';
        if (str[i] == 39)
            id = 26;
        else
            id = tolower(str[i]) - 'a';	//��λ���ַ�Ӧ���ڵ�λ��
		if(!pCur->next[id])
			return 0;
		else
			pCur = pCur->next[id];
	}



    int flag;
    flag = pCur->count;
    if (flag == 0)
        return 0;
    else
    {
        return pCur->isend;
    }

}


/*
�����ֵ���
*/
void destroy_TrieTree(TrieTree rootTree)
{
	int i;
	//�ݹ��ͷ�ÿ���ڵ���ڴ�
	for(i=0;i<MAX;i++)
		if(rootTree->next[i])
			destroy_TrieTree(rootTree->next[i]);
	free(rootTree);
}

//����һ��ֻ�и��ڵ���ֵ���
TrieTree create_rootTree();

//���ֵ����в����ַ���
void insert_TrieTree(TrieTree,char *);

//ͳ���ֵ������и�������ǰ׺���ַ�������Ŀ
int count_TrieTree(TrieTree ,char *);

//�����ֵ���
void destroy_TrieTree(TrieTree);


int itssize(FILE *fp)
{
    // TODO
    char c;//����ѭ���ı���
    int h = 0;//���������ı���
    while((c = fgetc(fp)) != EOF)//�ļ��ս���EOF

     {
        if(c == '\n')//�����⵽����
        h++;//�ͼ���+1

        else
        {
            c = fgetc(fp);//���Ǵ������һ�п���û�л��б�־�������ļ�ȴ�Ѿ�������

            if(c == EOF)
            {
                h++;//���һ��û�л��еĻ�Ҳ���Լ���һ��
                break;
            }       //if������

        }          //else������

    }               //while������

    return h;

}                   //����������


int main()
{
	//length of word LENGTH is the longest length of English word
	char str[LENGTH+1];
	//create a root
	TrieTree rootTree = create_rootTree();
    /*
    get words
    */

    /*
	printf("Please input all string:\n");
	while(gets(str) && str[0] != '\0')
		insert_TrieTree(rootTree,str);
    */
    FILE * fp = fopen("D:\\small.txt", "r"); //��ֻ����ʽ���ļ�
    if (fp == NULL)   //�ļ�ָ��Ϊ��ʱ��ʧ�ܣ�����false
        {
            printf("wrong");
            exit(-1);
        }

    while(fscanf(fp, "%s", str) != EOF){  //ѭ����ȡ�ļ����ݣ�ÿ�ζ�ȡһ������
        insert_TrieTree(rootTree,str);
    }



	printf("\nplease intput prefix string:\n");
	int is_in;
    /*
	while(gets(str) && str[0] != '\0')


		is_in = find_TrieTree(rootTree,str);
		if (is_in == 0)
            printf("the word %s is in this dictionary",str);
        else
            printf("the word %s is not in this dictionary",str);
    */

    gets(str);
    is_in = find_TrieTree(rootTree,str);
    printf("isend is %d\n",find_TrieTree(rootTree,str));

    printf("The size of this dictionary is %d\n",size_TrieTree(rootTree));
    fclose(fp);      //�ر��ļ�
	destroy_TrieTree(rootTree);
	return 0;
}
