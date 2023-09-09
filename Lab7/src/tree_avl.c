#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tree_avl.h"
#include "node_tree_avl.h"
#include "process.h"


void print_tree_avl_prefix(tree_avl_t *tree)
{
    if (tree)
    {
        node_avl_print(tree, "%s ");
    }
    if (tree->left)
    {
        print_tree_avl_prefix(tree->left);
    }
    if (tree->right)
    {
        print_tree_avl_prefix(tree->right);
    }
}


void print_tree_avl_infix(tree_avl_t *tree)
{
    if (tree->left)
    {
        print_tree_avl_infix(tree->left);
    }
    if (tree)
    {
        node_avl_print(tree, "%s ");
    }
    if (tree->right)
    {
        print_tree_avl_infix(tree->right);
    }
}


void print_tree_avl_postfix(tree_avl_t *tree)
{
    if (tree->left)
    {
        print_tree_avl_postfix(tree->left);
    }
    if (tree->right)
    {
        print_tree_avl_postfix(tree->right);
    }
    if (tree)
    {
        node_avl_print(tree, "%s ");
    }
}


void tree_avl_set_new_root(tree_avl_t **tree, tree_avl_t *root)
{
    root->left = *tree;
    *tree = root;
    set_avl_diff(*tree);
}


tree_avl_t *new_avl_tree(tree_avl_t *left, tree_avl_t *right, char *val)
{
    tree_avl_t *res = node_avl_create(val);
    if (res)
    {
        res->left = left;
        res->right = right;
    }
    set_avl_diff(res);
    return res;
}


tree_avl_t* tree_avl_insert(tree_avl_t *tree, tree_avl_t *node)
{
    if (tree == NULL)
    {
        return node;
    }
    int cmp = cmp_str(node->data, tree->data);
    if (cmp > 0)
    {
        tree->right = tree_avl_insert(tree->right, node);
    }
    else if (cmp < 0)
    {
        tree->left = tree_avl_insert(tree->left, node);
    }
    return balance_tree(tree);
}


tree_avl_t* tree_avl_delete(tree_avl_t *tree, char *value)
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
            tree_avl_t *temp = tree->left;
            tree_avl_t *prev = NULL;
            for (; temp->right; temp = temp->right)
            {
                prev = temp;
            }
            if (prev)
            {
                tree->data = temp->data;
                prev->right = NULL;
                node_avl_free(temp);
            }
            else
            {
                temp->right = tree->right;
                tree_avl_t *buf = tree;
                tree = temp;
                node_avl_free(buf);
            }
        }
        else if (tree->left)
        {
            tree_avl_t *temp = tree->left;
            tree->right = temp->right;
            tree->left = temp->left;
            tree->data = temp->data;
            node_avl_free(temp);
        }
        else if (tree->right)
        {
            tree_avl_t *temp = tree->right;
            tree->right = temp->right;
            tree->left = temp->left;
            tree->data = temp->data;
            node_avl_free(temp);
        }
        else
        {
            node_avl_free(tree);
            tree = NULL;
        }
    }
    else if (cmp > 0)
    {
        tree->right = tree_avl_delete(tree->right, value);
    }
    else
    {
        tree->left = tree_avl_delete(tree->left, value);
    }
    set_avl_diff(tree);
    return balance_tree(tree);
}


tree_avl_t* tree_avl_search(tree_avl_t *tree, char *data)
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


void tree_avl_free(tree_avl_t *tree)
{
    if (tree->left)
    {
        tree_avl_free(tree->left);
    }
    if (tree->right)
    {
        tree_avl_free(tree->right);
    }
    free(tree);
}


void set_avl_diff(tree_avl_t *tree)
{
    if (tree)
    {
        tree->diff = avl_get_height(tree->right) - avl_get_height(tree->left);
        if (tree->right)
        {
            set_avl_diff(tree->right);
        }
        if (tree->left)
        {
            set_avl_diff(tree->left);
        }
    }
}


int avl_get_height(tree_avl_t *tree)
{
    if (!tree)
    {
        return 0;
    }
    return 1 + max(avl_get_height(tree->right), avl_get_height(tree->left));
}


tree_avl_t *balance_tree(tree_avl_t *tree)
{
    if (!tree)
    {
        return NULL;
    }
    set_avl_diff(tree);
    if (tree->diff > 1)
    {
        if (tree->right->diff < 0)
        {
            tree->right = right_rotate(tree->right);
        }
        tree = left_rotate(tree);
    }
    else if (tree->diff < -1)
    {
        if (tree->left->diff > 0)
        {
            tree->left = left_rotate(tree->left);
        }
        tree = right_rotate(tree);
    }
    return tree;
}


tree_avl_t *right_rotate(tree_avl_t *tree)
{
    tree_avl_t *new = tree->left;

    tree->left = new->right;
    new->right = tree;

    set_avl_diff(new);
    set_avl_diff(tree);
    return new;
}


tree_avl_t *left_rotate(tree_avl_t *tree)
{
    tree_avl_t *new = tree->right;

    tree->right = new->left;
    new->left = tree;

    set_avl_diff(new);
    set_avl_diff(tree);
    return new;
}


int sizeof_tree_avl(tree_avl_t *tree)
{
    if (!tree)
    {
        return 0;
    }
    int size = sizeof(tree_avl_t);
    if (tree->right)
    {
        size += sizeof_tree_avl(tree->right);
    }
    if (tree->left)
    {
        size += sizeof_tree_avl(tree->left);
    }
    return size;
}

