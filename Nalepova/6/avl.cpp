#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h> 
#include <stdlib.h> 
typedef struct tree
{
	struct tree * left;
	struct tree * right;
	int height;
	int data;
}tree;
int GetHeight(tree * p)
{
	if (p != NULL)
		return p->height;
	else
		return 0;
}
void FixHeight(tree * p)
{
	if (GetHeight(p->right) >= GetHeight(p->left))
	{
		p->height = GetHeight(p->right) + 1;
	}
	else if (GetHeight(p->right) < GetHeight(p->left))
	{
		p->height = GetHeight(p->left) + 1;
	}
}
tree * CreateNode(int key)
{
	tree * p = (tree*)malloc(sizeof(tree));
	p->left = NULL;
	p->right = NULL;
	p->height = 1;
	p->data = key;
	return p;
}
void DeleteNode(tree * p)
{
	if (p->left != NULL)
		DeleteNode(p->left);
	if (p->right != NULL)
		DeleteNode(p->right);
	free(p);
}
tree * LeftRotation(tree * p)
{
	tree * q = p->right;
	p->right = q->left;
	q->left = p;
	FixHeight(p);
	FixHeight(q);
	return q;
}
tree * RightRotation(tree * p)
{
	tree * q = p->left;
	p->left = q->right;
	q->right = p;
	FixHeight(p);
	FixHeight(q);
	return q;
}

tree * Balance(tree * p)
{
	FixHeight(p);
	if (p != NULL)
	{
		if ((p->right) != NULL)
		{
			if (GetHeight(p->right) - GetHeight(p->left) == 2)
			{
				if (GetHeight(p->right->right) < GetHeight(p->right->left))
				{
					p->right = RightRotation(p->right);
					return LeftRotation(p);
				}
				else return LeftRotation(p);
			}
		}
		if ((p->left) != NULL)
		{
			if (GetHeight(p->right) - GetHeight(p->left) == -2)
			{
				if (GetHeight(p->left->left) < GetHeight(p->left->right))
				{
					p->left = LeftRotation(p->left);
					return RightRotation(p);
				}
				else return RightRotation(p);
			}
		}
	}
	return p;
}
tree * InsertNode(tree * p, int key)
{
	if (p == NULL)
		return CreateNode(key);

	if (key <= (p->data))
	{
		p->left = InsertNode(p->left, key);
	}
	else if (key >= (p->data))
	{
		p->right = InsertNode(p->right, key);
	}
	return Balance(p);
}
int main()
{
	FILE *out, *in;
	in = fopen("in.txt", "r");
	out = fopen("out.txt", "w");
	tree * p = NULL;
	int amountOfTops;
	fscanf(in,"%d", &amountOfTops);
	
	if (amountOfTops == 0)
	{
		printf("%d", 0);
		return 0;
	}
	int temp;
	for (int i = 0; i < amountOfTops; i++)
	{
		fscanf(in,"%d", &temp);
		p = InsertNode(p, temp);
	}
	
	int h = GetHeight(p);
	if(p != NULL)
	{
		DeleteNode(p);
		fprintf(out, "%d\n", h);
	}
	fclose(in);
	fclose(out);
	return 0;
}

