#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Trie structure
struct Trie
{
    struct Trie* children[26];
    int isEnd;
    int numOccurences;
};

int readDictionary(char* filename, char** pInWords);
struct Trie* createTrie();
struct Trie* deallocateTrie(struct Trie* pTrie);
int numberOfOccurances(struct Trie* pTrie, char* word);
void insert(struct Trie* pTrie, char* word);

// Inserts the word to the trie structure
void insert(struct Trie* pTrie, char* word)
{
    struct Trie* temp = pTrie;
    for (int i = 0; i < strlen(word); i++) {
        int index = word[i] - 'a';
        if (!temp->children[index]) {
            temp->children[index] = createTrie();
        }
        temp = temp->children[index];
    }
    temp->isEnd = 1;
    temp->numOccurences++;
}

// computes the number of occurances of the word
int numberOfOccurances(struct Trie* pTrie, char* word)
{
    struct Trie* pTemp = pTrie;
    for (int i = 0; i < strlen(word); i++)
    {
        int index = word[i] - 'a';
        if (!pTemp->children[index]) {
            return 0;
        }

        pTemp = pTemp->children[index];
    }
    return (pTemp != NULL && pTemp->isEnd) ? pTemp->numOccurences : 0;
}

// deallocate the trie structure
struct Trie* deallocateTrie(struct Trie* pTrie)
{
    for (int i = 0; i < 26; i++) {
        if (pTrie->children[i]) {
            deallocateTrie(pTrie->children[i]);
        }
    }
    free(pTrie);
}

// Initializes a trie structure
struct Trie* createTrie()
{
    struct Trie* pNode = malloc(sizeof(struct Trie));
    if (pNode == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        return NULL;
    }

    pNode->isEnd = 0;
    pNode->numOccurences = 0;

    for (int i = 0; i < 26; i++) {
        pNode->children[i] = NULL;
    }

    return pNode;
}

// this function will return number of words in the dictionary,
// and read all the words in the dictionary to the structure words
int readDictionary(char* filename, char** pInWords)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Could not open file %s", filename);
        return 0;
    }

    int numWords;
    fscanf(file, "%d", &numWords);

    for (int i = 0; i < numWords; i++)
    {
        pInWords[i] = malloc(100);
        fscanf(file, "%s", pInWords[i]);
    }

    fclose(file);
    return numWords;
}

int main(void)
{
    char* inWords[256];

    //read the number of the words in the dictionary
    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i = 0;i < numWords;++i)
    {
        printf("%s\n", inWords[i]);
    }

    struct Trie* pTrie = createTrie();
    for (int i = 0;i < numWords;i++)
    {
        insert(pTrie, inWords[i]);
    }
    // parse lineby line, and insert each word to the trie data structure
    char* pWords[] = { "notaword", "ucf", "no", "note", "corg" };
    for (int i = 0;i < 5;i++)
    {
        printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
    }
    pTrie = deallocateTrie(pTrie);
    if (pTrie != NULL)
        printf("There is an error in this program\n");
    return 0;
}