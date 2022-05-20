#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "LinkedList.h"
#include "tree.h"

#define TREE_CMD_INDENT_SIZE 4
#define NO_ARG ""
#define PARENT_DIR ".."


FileTree createFileTree(char* rootFolderName) {
	FileTree file_tree;
	// primul NULL->parintele lui root, al doilea NULL->filecontent, ptc e folder
	file_tree.root = create_TN(NULL, rootFolderName, FOLDER_NODE, NULL);
	DIE(!file_tree.root, "malloc createFileTree\n");

	return file_tree;
}
void freeTree_wrapper(TreeNode *current_node)
{
	FolderContent *folder_content = current_node->content;
	linked_list_t *list = folder_content->children;
	ll_node_t *walk = list->head;

	while (walk) {
		TreeNode *curr_TN = walk->data;
		if (curr_TN->type == FILE_NODE) {
			FileContent *file_content = curr_TN->content;
			free(file_content->text);
		} else {
			freeTree_wrapper(curr_TN);
		}
		free(curr_TN->name);
		free(curr_TN->content);
		walk = walk->next;
	}
	ll_free(&list);
}

void freeTree(FileTree fileTree) {
	TreeNode *root = fileTree.root;
	freeTree_wrapper(root);
	free(root->name);
	free(root->content);
	free(root);
}


void ls(TreeNode* currentNode, char* arg) {
	FolderContent *folder_content = currentNode->content;
	linked_list_t *list = folder_content->children;
	if (*arg == 0) {  // afisez num tuturor folderelor/fisierelor din director
		ll_node_t *walk = list->head;
		while (walk) {
			TreeNode *curr_TN = walk->data;
			printf("%s\n", curr_TN->name);
			walk = walk->next;
		}
	} else {
		int ok = 0;
		ll_node_t *walk = list->head;
		while (walk) {
			TreeNode *curr_TN = walk->data;
			if (strcmp(curr_TN->name, arg) == 0) {
				ok = 1;
				if (curr_TN->type == FOLDER_NODE)
					return ls(curr_TN, "\0");
				// altfel daca e file_node
				FileContent *f_content = curr_TN->content;
				printf("%s: %s\n", curr_TN->name, f_content->text);
				return;
			}
			walk = walk->next;
		}
		if (!ok) {
			printf("ls: cannot access '%s': No such file or directory\n", arg);
			return;
		}
	}
}


void pwd(TreeNode* treeNode) {
}

TreeNode* cd_wrapper(TreeNode *currentNode, char *path)
{
    char *token = strtok(path, "/");
	while (token) {
		if (strcmp(token, PARENT_DIR) == 0)
			currentNode = currentNode->parent;
		else
			currentNode = find_name_in_folder(currentNode, token);

		// daca a vrut sa sara dincolo de root sau la un dir care nu exista
		if (!currentNode || currentNode->type == FILE_NODE) {
			return NULL;
		}

		token = strtok(NULL, "/");
	}
	return currentNode;
}
TreeNode* cd(TreeNode* currentNode, char* path)
{
	TreeNode *currentNode_cp = currentNode;
	char *path_cp = strdup(path);
	currentNode = cd_wrapper(currentNode, path);

	if (!currentNode || currentNode->type == FILE_NODE) {
		printf("cd: no such file or directory: %s\n", path_cp);
		free(path_cp);
		return currentNode_cp;
	}

	free(path_cp);
	return currentNode;
}
void tree_wrapper(TreeNode *currentNode, int *dr, int *fl, int tabs)
{
	FolderContent *folder_content = currentNode->content;
	linked_list_t *list = folder_content->children;
	ll_node_t *walk = list->head;

	while (walk) {
		TreeNode *curr_TN = walk->data;
		if (curr_TN->type == FILE_NODE)
			(*fl)++;
		else
			(*dr)++;

		for (int i = 0; i < tabs; i++)
			printf("\t");
		printf("%s\n", curr_TN->name);

		if (curr_TN->type == FOLDER_NODE)
			tree_wrapper(curr_TN, dr, fl, tabs + 1);
		walk = walk->next;
	}
}
void tree(TreeNode* currentNode, char* arg) {
	char *path_cp = strdup(arg);
	if (*arg != 0) {
		currentNode = cd_wrapper(currentNode, arg);
		if (!currentNode || currentNode->type == FILE_NODE) {
			printf("%s [error opening dir]\n\n0 directories, 0 files\n", path_cp);
			free(path_cp);
			return;
		}
	}
	int directories = 0, files = 0;
	tree_wrapper(currentNode, &directories, &files, 0);
	printf("%d directories, %d files\n", directories, files);
	free(path_cp);
}


void mkdir(TreeNode* currentNode, char* folderName) {
	if (find_name_in_folder(currentNode, folderName) != NULL) {
		printf("mkdir: cannot create directory '%s': File exists\n", folderName);
		return;
	}

	FolderContent *folder_content = currentNode->content;
	linked_list_t *list = folder_content->children;

	TreeNode *new_dir = create_TN(currentNode, folderName, FOLDER_NODE, NULL);
	ll_add_nth_node(list, 0, new_dir);
	free(new_dir);
}


void rmrec(TreeNode* currentNode, char* resourceName) {
}


void rm(TreeNode* currentNode, char* fileName) {
}


void rmdir(TreeNode* currentNode, char* folderName) {
}


void touch(TreeNode* currentNode, char* fileName, char* fileContent) {
	FolderContent *folder_content = currentNode->content;
	linked_list_t *list = folder_content->children;
	ll_node_t *walk = list->head;
	// in cazul in care gaseste nod in lista cu num fileName,
	// nu mai face o creare noua
	while (walk) {
		TreeNode *curr_treeNode = walk->data;
		if (strcmp(curr_treeNode->name, fileName) == 0)
			return;  // e deja un folder/file cu numele fileName
		walk = walk->next;
	}
	// creaza nodul in arbore si initializeaza valorile, inclusiv filecontent
	TreeNode *new_TN = create_TN(currentNode, fileName, FILE_NODE, fileContent);

	ll_add_nth_node(list, 0, new_TN);
	free(new_TN);
}

void cp(TreeNode* currentNode, char* source, char* destination) {
}

void mv(TreeNode* currentNode, char* source, char* destination) {
}

TreeNode *create_TN(TreeNode *parent, char *name, enum TreeNodeType type, char *text_content)
{
	TreeNode *new_TN = malloc(sizeof(TreeNode));
	DIE(!new_TN, "malloc new_TN\n");

	new_TN->name = name;
	new_TN->parent = parent;
	new_TN->type = type;

	if (type == FOLDER_NODE) {
		new_TN->content = malloc(sizeof(FolderContent));
		DIE(!new_TN->content, "malloc new_content folder_node\n");
		((FolderContent*)new_TN->content)->children = ll_create(sizeof(TreeNode));
		DIE(!((FolderContent*)new_TN->content)->children, "malloc list folder\n");
	}
	else {
		if (type == FILE_NODE) {
			new_TN->content = malloc(sizeof(FileContent));
			DIE(!new_TN->content, "malloc content file_node\n");

			FileContent *file_content = new_TN->content;
			file_content->text = NULL; // at cand nu avem filecontent

			if (text_content) {
				file_content->text = text_content;
			}
		}
	}
	return new_TN;
}

TreeNode* find_name_in_folder(TreeNode *currentNode, char *name) {
	FolderContent *folder_content = currentNode->content;
	linked_list_t *list = folder_content->children;

	ll_node_t *walk = list->head;
	while (walk) {
		TreeNode *curr_TN = walk->data;
		if (strcmp(curr_TN->name, name) == 0)
			return curr_TN;
		walk = walk->next;
	}
	return NULL;
}
