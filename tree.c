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
	// TODO
	FileTree file_tree;
	// primul NULL->parintele lui root, al doilea NULL->filecontent, ptc e folder
	file_tree.root = create_TN(NULL, rootFolderName, FOLDER_NODE, NULL);
	DIE(!file_tree.root, "malloc createFileTree\n");

	return file_tree;
}

void freeTree(FileTree fileTree) {
	// TODO
}


void ls(TreeNode* currentNode, char* arg) {
	FolderContent *folder_content = currentNode->content;
	linked_list_t *list = folder_content->children;
	if (*arg == 0) { // afisez numele tuturor folderelor/fisierelor din director
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
				printf("%s\n", f_content->text);
				return;
			}
			walk = walk->next;
		}
		if (!ok) {
			printf("ls: cannot acces '%s': No such file or directory\n", arg);
			return;
		}
	}
}


void pwd(TreeNode* treeNode) {
	// TODO
}


TreeNode* cd(TreeNode* currentNode, char* path) {
	// TODO
}


void tree(TreeNode* currentNode, char* arg) {
	// TODO
}


void mkdir(TreeNode* currentNode, char* folderName) {
	if (find_name_in_folder(currentNode, folderName) == 1) {
		printf("mkdir: cannot create directory '%s': File exists\n", folderName);
		return;
	}

	FolderContent *folder_content = currentNode->content;
	linked_list_t *list = folder_content->children;

	TreeNode *new_dir = create_TN(currentNode, folderName, FOLDER_NODE, NULL);
	ll_add_nth_node(list, 0, new_dir);

}


void rmrec(TreeNode* currentNode, char* resourceName) {
	// TODO
}


void rm(TreeNode* currentNode, char* fileName) {
	// TODO
}


void rmdir(TreeNode* currentNode, char* folderName) {
	// TODO
}


void touch(TreeNode* currentNode, char* fileName, char* fileContent) {
	// TODO
	// currentNode->content
	FolderContent *folder_content = currentNode->content;
	linked_list_t *list = folder_content->children;
	ll_node_t *walk = list->head;
	// in cazul in care gaseste nodd in lista cu numele fileName, nu mai face o creare noua
	while (walk) {
		TreeNode *curr_treeNode = walk->data;
		if (strcmp(curr_treeNode->name, fileName) == 0)
			return; // e deja un folder/file cu numele fileName
		walk = walk->next;
	}
	// creaza nodul in arbore si initializeaza valorile, inclusiv filecontent
	TreeNode *new_TN = create_TN(currentNode, fileName, FILE_NODE, fileContent);

	ll_add_nth_node(list, 0, new_TN);
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

			if (text_content)
				file_content->text = text_content;
		}
	}
	return new_TN;
}
void cp(TreeNode* currentNode, char* source, char* destination) {
	// TODO
}

void mv(TreeNode* currentNode, char* source, char* destination) {
	// TODO
}

int find_name_in_folder(TreeNode *currentNode, char *name) {
	FolderContent *folder_content = currentNode->content;
	linked_list_t *list = folder_content->children;

	ll_node_t *walk = list->head;
	while (walk) {
		TreeNode *curr_TN = walk->data;
		if (strcmp(curr_TN->name, name) == 0)
			return 1;
		walk = walk->next;
	}
	return 0;

}