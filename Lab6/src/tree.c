#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "node_tree.h"


void print_tree_prefix(tree_t *tree)
{
    if (tree)
    {
        node_print(tree, "%s ");
    }
    if (tree->left)
    {
        print_tree_prefix(tree->left);
    }
    if (tree->right)
    {
        print_tree_prefix(tree->right);
    }
}


void print_tree_infix(tree_t *tree)
{
    if (tree->left)
    {
        print_tree_infix(tree->left);
    }
    if (tree)
    {
        node_print(tree, "%s ");
    }
    if (tree->right)
    {
        print_tree_infix(tree->right);
    }
}


void print_tree_postfix(tree_t *tree)
{
    if (tree->left)
    {
        print_tree_postfix(tree->left);
    }
    if (tree->right)
    {
        print_tree_postfix(tree->right);
    }
    if (tree)
    {
        node_print(tree, "%s ");
    }
}


void tree_set_new_root(tree_t **tree, tree_t *root)
{
    root->left = *tree;
    *tree = root;
}


tree_t *new_tree(tree_t *left, tree_t *right, char *val)
{
    tree_t *res = node_create(val);
    if (res)
    {
        res->left = left;
        res->right = right;
    }
    return res;
}


int cmp_str(char *a, char *b)
{
    size_t a_len = strlen(a);
    size_t b_len = strlen(b);
    if (a_len > b_len)
    {
        return 1;
    }
    else if (a_len < b_len)
    {
        return -1;
    }
    else
    {
        return strcmp(a, b);
    }
}


tree_t* tree_insert(tree_t *tree, tree_t *node)
{
    if (tree == NULL)
    {
        return node;
    }

    int cmp = cmp_str(node->data, tree->data);
    if (cmp >= 0)
    {
        tree->right = tree_insert(tree->right, node);
    }
    else
    {
        tree->left = tree_insert(tree->left, node);
    }

    return tree;
}


tree_t* tree_delete(tree_t **tree, char *value)
{
    if (*tree == NULL || tree == NULL)
    {
        return NULL;
    }

    int cmp = cmp_str(value, (*tree)->data);
    if (cmp == 0)
    {
        if ((*tree)->left && (*tree)->right)
        {
            tree_t *temp = (*tree)->left;
            tree_t *prev = NULL;
            for (; temp->right; temp = temp->right)
            {
                prev = temp;
            }
            if (prev)
            {
                (*tree)->data = temp->data;
                prev->right = NULL;
                node_free(temp);
            }
            else
            {
                temp->right = (*tree)->right;
                tree_t *buf = *tree;
                *tree = temp;
                node_free(buf);
            }
        }
        else if ((*tree)->left)
        {
            tree_t *temp = (*tree)->left;
            // free((*tree)->data);
            (*tree)->right = temp->right;
            (*tree)->left = temp->left;
            (*tree)->data = temp->data;
            node_free(temp);
        }
        else if ((*tree)->right)
        {
            tree_t *temp = (*tree)->right;
            // free((*tree)->data);
            (*tree)->right = temp->right;
            (*tree)->left = temp->left;
            (*tree)->data = temp->data;
            node_free(temp);
        }
        else
        {
            // free((*tree)->data);
            node_free(*tree);
            *tree = NULL;
        }
    }
    else if (cmp > 0)
    {
        (*tree)->right = tree_delete(&((*tree)->right), value);
    }
    else
    {
        (*tree)->left = tree_delete(&((*tree)->left), value);
    }

    return *tree;
}


tree_t* tree_search(tree_t *tree, char *data)
{
    while (tree != NULL)
    {
        int cmp = cmp_str(data, tree->data);
        if (cmp == 0)
        {
            return tree;
        }
        else if (cmp < 0)
        {
            tree = tree->left;
        }
        else
        {
            tree = tree->right;
        }
    }
    return NULL;
}


void tree_free(tree_t *tree)
{
    if (tree->left)
    {
        tree_free(tree->left);
    }
    if (tree->right)
    {
        tree_free(tree->right);
    }
    free(tree);
}
