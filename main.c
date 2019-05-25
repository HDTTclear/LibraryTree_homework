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
each TrieTree has a pointer which has 26 positions to place abcdefg……xyz.

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
插入字符串str到字典树rootTree中
由于不可能改变根节点，因此这里不需要传入pTree的引用
*/
void insert_TrieTree(TrieTree rootTree,char *str)
{
	int i;
	TrieTree pCur = rootTree;	//当前访问的节点，初始指向根节点
	rootTree->size++;//TODO
	int len = strlen(str);

	for(i=0;i<len;i++)
	{
        int id;
        if (str[i] == 39)
            id = 26;
        else
            id = tolower(str[i]) - 'a';	//定位到字符应该在的位置

		if(!pCur->next[id])
		{	//如果该字符应该在的位置为空，则将字符插入到该位置
			int j;

/*————————————————————创建一个子树的代码——————————————————————————————————————————*/

			//新建一个子树
			TrieNode *pNew = (TrieNode *)malloc(sizeof(TrieNode));
			//一般不会这样，内存申请失败
			if(!pNew)
			{
				printf("pNew malloc fail\n");
				exit(-1);
			}
			//这个子树的初始count当然是1
			pNew->count = 1;
			//把子树的next初始化一下
			for(j=0;j<MAX;j++)
				pNew->next[j] = NULL;
            pNew->isend = 0;

/*————————————————————连接子树和母树的代码——————————————————————————————————*/
            //把子树接到根上
			pCur->next[id] = pNew;
			//指针后移，比较下一个字符
			pCur = pCur->next[id];//请注意指针最后是落在了新建的子树本体上

		}
		else
		{	//如果该字符应该在的位置不空，则继续比较下一个字符
			pCur = pCur->next[id];//也是一样的，指针落在了子树本体上
			pCur->count++;	//每插入一个字符，公共前缀数目就加1
		}
	}
	pCur->isend = 1;//意味着终结，这里有一个单词
}

/*
统计字典树pTree中以str为前缀的字符串的数目
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
            id = tolower(str[i]) - 'a';	//定位到字符应该在的位置

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
            id = tolower(str[i]) - 'a';	//定位到字符应该在的位置
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
销毁字典树
*/
void destroy_TrieTree(TrieTree rootTree)
{
	int i;
	//递归释放每个节点的内存
	for(i=0;i<MAX;i++)
		if(rootTree->next[i])
			destroy_TrieTree(rootTree->next[i]);
	free(rootTree);
}

//创建一个只有根节点的字典树
TrieTree create_rootTree();

//向字典树中插入字符串
void insert_TrieTree(TrieTree,char *);

//统计字典树中有给定公共前缀的字符串的数目
int count_TrieTree(TrieTree ,char *);

//销毁字典树
void destroy_TrieTree(TrieTree);


int itssize(FILE *fp)
{
    // TODO
    char c;//用来循环的变量
    int h = 0;//用来计数的变量
    while((c = fgetc(fp)) != EOF)//文件终结是EOF

     {
        if(c == '\n')//如果检测到换行
        h++;//就计数+1

        else
        {
            c = fgetc(fp);//这是处理最后一行可能没有换行标志，但是文件却已经结束。

            if(c == EOF)
            {
                h++;//最后一行没有换行的话也可以检验一下
                break;
            }       //if的括号

        }          //else的括号

    }               //while的括号

    return h;

}                   //函数的括号


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
    FILE * fp = fopen("D:\\small.txt", "r"); //以只读方式打开文件
    if (fp == NULL)   //文件指针为空时打开失败，返回false
        {
            printf("wrong");
            exit(-1);
        }

    while(fscanf(fp, "%s", str) != EOF){  //循环读取文件内容，每次读取一个单词
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
    fclose(fp);      //关闭文件
	destroy_TrieTree(rootTree);
	return 0;
}
