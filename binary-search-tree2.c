/* binary-search-tree2 */

#include <stdio.h>
#include <stdlib.h>

/* 트리 노드 구조체 선언 */
typedef struct node
{
    int key;
    struct node *left;
    struct node *right;
} Node;

/* for stack (LIFO) */
#define MAX_STACK_SIZE 20
Node *stack[MAX_STACK_SIZE];
int top = -1;

Node *pop();
void push(Node *aNode);

/* for queue (FIFO) */
#define MAX_QUEUE_SIZE 20
Node *queue[MAX_QUEUE_SIZE];
int front = -1;
int rear = -1;

Node *deQueue();
void enQueue(Node *aNode);

int initializeBST(Node **h);
void recursiveInorder(Node *ptr);    /* recursive inorder traversal */
void iterativeInorder(Node *ptr);    /* iterative inorder traversal */
void levelOrder(Node *ptr);          /* level order traversal */
int insert(Node *head, int key);     /* insert a node to the tree */
int deleteNode(Node *head, int key); /* delete the node for the key */
int freeBST(Node *head);             /* free all memories allocated to the tree */
void printStack();

int main()
{
    char command;
    int key;
    Node *head = NULL;

    printf("[----- [JinYeongKim] [2019038030] -----]");
    do
    {
        printf("\n\n");
        printf("----------------------------------------------------------------\n");
        printf("                   Binary Search Tree #2                        \n");
        printf("----------------------------------------------------------------\n");
        printf(" Initialize BST       = z      Print Stack                  = p \n");
        printf(" Insert Node          = i      Delete Node                  = d \n");
        printf(" Recursive Inorder    = r      Iterative Inorder (Stack)    = t \n");
        printf(" Level Order (Queue)  = l      Quit                         = q \n");
        printf("----------------------------------------------------------------\n");

        printf("Command = ");
        scanf(" %c", &command);

        switch (command)
        {
        case 'z':
        case 'Z':
            initializeBST(&head);
            break;
        case 'q':
        case 'Q':
            freeBST(head);
            break;
        case 'i':
        case 'I':
            printf("Your Key = ");
            scanf("%d", &key);
            insert(head, key);
            break;
        case 'd':
        case 'D':
            printf("Your Key = ");
            scanf("%d", &key);
            deleteNode(head, key);
            break;

        case 'r':
        case 'R':
            recursiveInorder(head->left);
            break;
        case 't':
        case 'T':
            iterativeInorder(head->left);
            break;

        case 'l':
        case 'L':
            levelOrder(head->left);
            break;

        case 'p':
        case 'P':
            printStack();
            break;

        default:
            printf("\n       >>>>>   Concentration!!   <<<<<     \n");
            break;
        }

    } while (command != 'q' && command != 'Q');

    return 1;
}

int initializeBST(Node **h)
{

    /* if the tree is not empty, then remove all allocated nodes from the tree*/
    if (*h != NULL)
        freeBST(*h);

    /* create a head node */
    *h = (Node *)malloc(sizeof(Node));
    (*h)->left = NULL; /* root */
    (*h)->right = *h;
    (*h)->key = -9999;

    top = -1;

    front = rear = -1;

    return 1;
}

/* 재귀(순환) 중위 트리 순회 (LVR) */
void recursiveInorder(Node *ptr)
{
    if (ptr)
    {
        recursiveInorder(ptr->left);  // left child가 NULL일 때까지 함수 실행
        printf(" [%d] ", ptr->key);   // left child가 NULL일 때 ptr 출력
        recursiveInorder(ptr->right); //실행되었던 함수를 거슬러 올라가며 ptr 출력
    }
}

/* 반복적(비순환) 중위 트리 순회 (LVR) */
void iterativeInorder(Node *node)
{
    for (;;)
    {
        for (; node; node = node->left)
            push(node); // left child로 이동하며 stack에 삽입
        node = pop();   //스택에서 삭제

        if (!node) //공백 스택일 때 반복 종료
            break;
        printf(" [%d] ", node->key); // pop한 node 출력

        node = node->right; // right child로 이동
    }
}

/* 레벨 순서 순회 */
void levelOrder(Node *ptr)
{
    // int front = rear = -1;

    if (!ptr)
        return; /* empty tree */

    enQueue(ptr);

    for (;;)
    {
        ptr = deQueue(); // FIFO방식으로 먼저 들어온 노드 큐에서 삭제하고 삭제한 그 노드를 방문
        if (ptr)
        {
            printf(" [%d] ", ptr->key); // deQueue 한 노드 출력

            /* 노드의 left child와 right child 가 있다면 큐에 삽입 */
            if (ptr->left)
                enQueue(ptr->left);
            if (ptr->right)
                enQueue(ptr->right);
        }
        else
            break;
    }
}

int insert(Node *head, int key)
{
    /* insert할 트리 노드 메모리 할당 */
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->key = key;
    newNode->left = NULL;
    newNode->right = NULL;

    /* root가 NULL일 때 즉 아무 노드도 없을 때 */
    if (head->left == NULL)
    {
        head->left = newNode;
        return 1;
    }

    /* head->left is the root */
    Node *ptr = head->left;

    Node *parentNode = NULL;

    while (ptr != NULL)
    {

        /* 이미 그 키값의 노드가 존재하면 종료 */
        if (ptr->key == key)
            return 1;

        /* we have to move onto children nodes,
         * keep tracking the parent using parentNode */
        parentNode = ptr;

        /*키값을 비교하여 기존 노드보다 크면 오른쪽 작으면 왼쪽 노드로 이동*/
        if (ptr->key < key)
            ptr = ptr->right;
        else
            ptr = ptr->left;
    }

    /* 새로운 노드와 기존 부모 노드 link */
    if (parentNode->key > key)
        parentNode->left = newNode;
    else
        parentNode->right = newNode;
    return 1;
}

int deleteNode(Node *head, int key)
{
    /*head 초기화가 안 되었거나 아무 노드도 없을 때*/
    if (head == NULL)
    {
        printf("\n Nothing to delete!!\n");
        return -1;
    }

    if (head->left == NULL)
    {
        printf("\n Nothing to delete!!\n");
        return -1;
    }

    /* head->left is the root */
    Node *root = head->left;

    Node *parent = NULL;
    Node *ptr = root;

    /* ptr의 key값을 비교하여 삭제하고자 하는 노드 탐색 */
    while ((ptr != NULL) && (ptr->key != key))
    {
        if (ptr->key != key)
        {

            parent = ptr; /* 부모 노드 임시 저장 */

            if (ptr->key > key)
                ptr = ptr->left;
            else
                ptr = ptr->right;
        }
    }

    /* 위의 while문에서 삭제하고자 하는 노드를 끝까지 찾지 못했을 때 */
    if (ptr == NULL)
    {
        printf("No node for key [%d]\n ", key);
        return -1;
    }

    /* case 1: 삭제해야할 노드가 leaf 노드일 때 */
    if (ptr->left == NULL && ptr->right == NULL)
    {
        /* 삭제하고자 하는 노드가 부모 노드가 존재할 때 */
        if (parent != NULL)
        {
            if (parent->left == ptr)
                parent->left = NULL; // left child delete
            else
                parent->right = NULL; // right child delete
        }
        else
        {
            /* root 노드 하나만 존재하는 트리일 때 root 삭제 */
            head->left = NULL;
        }

        free(ptr);
        return 1;
    }

    /* case 2: 삭제하고자 하는 노드가 하나의 자식만 가질 때 */
    if ((ptr->left == NULL || ptr->right == NULL))
    {
        /* 삭제하고자 하는 노드의 child 노드*/
        Node *child;
        if (ptr->left != NULL)
            child = ptr->left;
        else
            child = ptr->right;

        /* 부모노드가 있을 때 */
        if (parent != NULL)
        {
            if (parent->left == ptr)
                parent->left = child; // ptr delete
            else
                parent->right = child; // ptr delete
        }

        /* root 자기 자신과 하나의 child가 있는 경우 child만 남는다. 즉 child가 root가 된다. */
        else
        {
            root = child;
        }

        free(ptr); // ptr 메모리 해제
        return 1;
    }

    /* case 3: 삭제하고자 하는 노드가 두 개의 자식이 있을 때  */
    /* 왼쪽 서브트리에서 가장 큰 노드 혹은 오른쪽 서브트리에서 가장 작은 노드 둘 중 하나를 부모로 만든다.*/
    /* 여기선 후자의 방법을 택한다. */
    Node *candidate;
    parent = ptr;

    /* 삭제하고자 하는 노드의 오른쪽 자식 노드*/
    candidate = ptr->right;

    /* right subtree에서 가장 작은 노드는 deepest left 노드 */
    while (candidate->left != NULL)
    {
        parent = candidate;
        candidate = candidate->left;
    }

    /* 삭제하고자 하는 노드의 오른쪽 자식 노드가 바로 right subtree에서 가장 작은 노드일 때*/
    if (parent->right == candidate)
        parent->right = candidate->right;
    else
        parent->left = candidate->right;

    /* ptr 삭제-> candiate의 key를 대입 */
    ptr->key = candidate->key;

    free(candidate);
    return 1;
}

/* 트리 노드 삭제 */
void freeNode(Node *ptr)
{
    if (ptr)
    {
        freeNode(ptr->left);
        freeNode(ptr->right);
        free(ptr);
    }
}

/* 이진 탐색 트리 초기화 */
int freeBST(Node *head)
{

    //헤드 노드 밖에 없을 때
    if (head->left == head)
    {
        free(head);
        return 1;
    }

    Node *p = head->left;

    freeNode(p);

    free(head);
    return 1;
}

/* 스택에서 노드 추출 */
Node *pop()
{
    if (top < 0)
        return NULL;
    return stack[top--];
}

/* 스택에 노드 삽입*/
void push(Node *aNode)
{
    stack[++top] = aNode;
}

/* 원형큐에서 노드 추출 (FIFO) */
Node *deQueue()
{
    if (front == rear)
    {
        // printf("\n....Now Queue is empty!!\n" );
        return NULL;
    }

    front = (front + 1) % MAX_QUEUE_SIZE;
    return queue[front];
}

/* 원형 큐에 노드 삽입 */
void enQueue(Node *aNode)
{
    rear = (rear + 1) % MAX_QUEUE_SIZE;
    if (front == rear)
    {
        // printf("\n....Now Queue is full!!\n");
        return;
    }

    queue[rear] = aNode;
}

/* 스택에 있는 노드 출력
iterativeInorder 함수에서 스택에 있는 노드를 pop하면서 출력하는데 스택이 공백이 될 때까지 반복하므로 스택에는 아무 것도 없다.*/
void printStack()
{
    int i = 0;
    printf("--- stack ---\n");
    while (i <= top)
    {
        printf("stack[%d] = %d\n", i, stack[i]->key);
    }
}