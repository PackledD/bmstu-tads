#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tree.h"
#include "node_tree.h"
#include "process.h"


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


tree_t* tree_insert(tree_t *tree, tree_t *node)
{
    if (tree == NULL)
    {
        return node;
    }

    int cmp = cmp_str(node->data, tree->data);
    if (cmp > 0)
    {
        tree->right = tree_insert(tree->right, node);
    }
    else if (cmp < 0)
    {
        tree->left = tree_insert(tree->left, node);
    }

    return tree;
}


tree_t* tree_delete(tree_t *tree, char *value)
{
    if (tree == NULL)
    {
        return NULL;
    }

    int cmp = cmp_str(value, tree->data);
    if (cmp == 0)
    {
        if (tree->left && tree->right)
        {
            tree_t *temp = tree->left;
            tree_t *prev = NULL;
            for (; temp->right; temp = temp->right)
            {
                prev = temp;
            }
            if (prev)
            {
                tree->data = temp->data;
                prev->right = NULL;
                node_free(temp);
            }
            else
            {
                temp->right = tree->right;
                tree_t *buf = tree;
                tree = temp;
                node_free(buf);
            }
        }
        else if (tree->left)
        {
            tree_t *temp = tree->left;
            tree->right = temp->right;
            tree->left = temp->left;
            tree->data = temp->data;
            node_free(temp);
        }
        else if (tree->right)
        {
            tree_t *temp = tree->right;
            tree->right = temp->right;
            tree->left = temp->left;
            tree->data = temp->data;
            node_free(temp);
        }
        else
        {
            node_free(tree);
            tree = NULL;
        }
    }
    else if (cmp > 0)
    {
        tree->right = tree_delete(tree->right, value);
    }
    else
    {
        tree->left = tree_delete(tree->left, value);
    }

    return tree;
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


int sizeof_tree(tree_t *tree)
{
    if (!tree)
    {
        return 0;
    }
    int size = sizeof(tree_t);
    if (tree->right)
    {
        size += sizeof_tree(tree->right);
    }
    if (tree->left)
    {
        size += sizeof_tree(tree->left);
    }
    return size;
}
