#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>


#define MAZE_SIZE 90


typedef struct
{
    int x;
    int y;
} Place;

typedef struct
{
    bool forward;
    bool right;
    bool left;
    bool back;
} WallsPositions;

typedef struct {
    Place *array;  
    int size;        
    int limit;     
} PlaceStack;

typedef struct Vertex
{   
    bool Viewed;
    Place Parent;
    WallsPositions positions;
} Vertex;

Vertex *Maze[MAZE_SIZE][MAZE_SIZE];



PlaceStack* createStack(int limit) {
    if (limit <= 0) {
        // Tratamento de erro: limit inválida
        fprintf(stderr, "Erro: capacidade inválida na função createStack.\n");
        exit(EXIT_FAILURE);
    }

    PlaceStack* stack = (PlaceStack*)malloc(sizeof(PlaceStack));
    if (stack == NULL) {
        // Tratamento de erro: falha na alocação de memória
        exit(EXIT_FAILURE);
    }

    stack->array = (Place*)malloc(limit * sizeof(Place));
    if (stack->array == NULL) {
        // Tratamento de erro: falha na alocação de memória
        free(stack);
        exit(EXIT_FAILURE);
    }

    stack->size = 0;
    stack->limit = limit;
    return stack;
}

void push(PlaceStack* stack, Place place) {
    if (stack->size < stack->limit) {
        stack->array[stack->size++] = place;
    } else {
        // Tratamento de erro: a stack está cheia
        fprintf(stderr, "Erro: Tentativa de push em uma stack cheia.\n");
        exit(EXIT_FAILURE);
    }
}

Place pop(PlaceStack* stack) {
    if (stack->size > 0) {
        return stack->array[--stack->size];
    } else {
        // Tratamento de erro: tentativa de desemstackr de uma stack vazia
        fprintf(stderr, "Erro: Tentativa de pop de uma stack vazia.\n");
        exit(EXIT_FAILURE);
    }
}

// Lógica de movimento do rato


int forward()
{
    printf("w\n");
    fflush(stdout);
    int result;
    if (scanf("%d", &result) != 1) {
        // Tratamento de erro: entrada inválida
        fprintf(stderr, "Erro ao ler result da função forward.\n");
        exit(EXIT_FAILURE);
    }
    return result;
}

int turnRight(int direction)
{
    printf("r\n");
    fflush(stdout);
    int result;
    if (scanf("%d", &result) != 1) {
        // Tratamento de erro: entrada inválida
        fprintf(stderr, "Erro ao ler result da função turnRight.\n");
        exit(EXIT_FAILURE);
    }
    return (direction + 1) % 4;  // Garante que o resultado esteja no intervalo [0, 3]
}

int turnLeft(int direction)
{
    printf("l\n");
    fflush(stdout);
    int result;
    if (scanf("%d", &result) != 1) {
        // Tratamento de erro: entrada inválida
        fprintf(stderr, "Erro ao ler result da função turnLeft.\n");
        exit(EXIT_FAILURE);
    }
    return (direction - 1 + 4) % 4;  // Garante que o resultado esteja no intervalo [0, 3]
}

// Scannear

void isWall(Place current, int direction)
{
    // Verifica se as coordenadas estão dentro dos limites da matriz
    if (current.x >= 0 && current.x < MAZE_SIZE && current.y >= 0 && current.y < MAZE_SIZE)
    {
        if (direction == 0)
        {
            Maze[current.x][current.y]->positions.forward = true;
        }
        else if (direction == 1 || direction == -3)
        {
            Maze[current.x][current.y]->positions.right = true;
        }
        else if (direction == 2 || direction == -2)
        {
            Maze[current.x][current.y]->positions.back = true;
        }
        else if (direction == 3 || direction == -1)
        {
            Maze[current.x][current.y]->positions.left = true;
        }
        else
        {
            // Tratamento de erro: direção inválida
            fprintf(stderr, "Erro: Direção inválida na função isWall.\n");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        // Tratamento de erro: coordenadas inválidas
        fprintf(stderr, "Erro: Coordenadas inválidas na função isWall.\n");
        exit(EXIT_FAILURE);
    }
}

void isParent(Place current, Place previous)
{
    // Verifica se as coordenadas estão dentro dos limites da matriz
    if (current.x >= 0 && current.x < MAZE_SIZE && current.y >= 0 && current.y < MAZE_SIZE)
    {
        Maze[current.x][current.y]->Parent = previous;
    }
    else
    {
        // Tratamento de erro: coordenadas inválidas
        fprintf(stderr, "Erro: Coordenadas inválidas na função isParent.\n");
        exit(EXIT_FAILURE);
    }
}

bool pathViewed(Place prox)
{
    if (prox.x >= 0 && prox.x < MAZE_SIZE && prox.y >= 0 && prox.y < MAZE_SIZE)
    {
        return Maze[prox.x][prox.y]->Viewed;
    }
    else
    {
        // Tratamento de erro: coordenadas inválidas
        fprintf(stderr, "Erro: Coordenadas inválidas na função pathViewed.\n");
        exit(EXIT_FAILURE);
    }
}

void Viewed(Place current)
{
    // Verifica se as coordenadas estão dentro dos limites da matriz
    if (current.x >= 0 && current.x < MAZE_SIZE && current.y >= 0 && current.y < MAZE_SIZE)
    {
        Maze[current.x][current.y]->Viewed = true;
    }
    else
    {
        // Tratamento de erro: coordenadas inválidas
        fprintf(stderr, "Erro: Coordenadas inválidas na função Viewed.\n");
        exit(EXIT_FAILURE);
    }
}


bool assignedWall(Place prox, int direction)
{
    if (prox.x >= 0 && prox.x < MAZE_SIZE && prox.y >= 0 && prox.y < MAZE_SIZE)
    {
        switch (direction % 4)
        {
        case 0:
            return Maze[prox.x][prox.y]->positions.forward;
        case 1:
        case -3:
            return Maze[prox.x][prox.y]->positions.right;
        case 2:
        case -2:
            return Maze[prox.x][prox.y]->positions.back;
        case 3:
        case -1:
            return Maze[prox.x][prox.y]->positions.left;
        default:
            // Tratamento de erro: direção inválida
            fprintf(stderr, "Erro: Direção inválida na função assignedWall.\n");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        // Tratamento de erro: coordenadas inválidas
        fprintf(stderr, "Erro: Coordenadas inválidas na função assignedWall.\n");
        exit(EXIT_FAILURE);
    }
}

int allAssignedWalls(Place current)
{
    if (current.x >= 0 && current.x < MAZE_SIZE && current.y >= 0 && current.y < MAZE_SIZE)
    {
        int qtdWallsViewed = 0;
        if (Maze[current.x][current.y]->positions.forward)
        {
            qtdWallsViewed++;
        }
        if (Maze[current.x][current.y]->positions.right)
        {
            qtdWallsViewed++;
        }
        if (Maze[current.x][current.y]->positions.left)
        {
            qtdWallsViewed++;
        }
        if (Maze[current.x][current.y]->positions.back)
        {
            qtdWallsViewed++;
        }
        return qtdWallsViewed;
    }
    else
    {
        // Tratamento de erro: coordenadas inválidas
        fprintf(stderr, "Erro: Coordenadas inválidas na função allAssignedWalls.\n");
        exit(EXIT_FAILURE);
    }
}

bool neighbrohoodViewed(Place current, int direction)
{
    if (current.x < 0 || current.x >= MAZE_SIZE || current.y < 0 || current.y >= MAZE_SIZE)
    {
        // Tratamento de erro: coordenadas inválidas
        fprintf(stderr, "Erro: Coordenadas inválidas na função neighbrohoodViewed.\n");
        exit(EXIT_FAILURE);
    }

    int pathsViewed = 0;
    int wallsViewed = allAssignedWalls(current);

    Place positionsPaths[4] = {
        (Place){-1, 0}, // 0
        (Place){0, 1},  // 1
        (Place){1, 0},  // 2
        (Place){0, -1}}; // 3

    int aux[4] = {0, 1, 2, 3};
    for (int i = 0; i < 4; i++)
    {
        Place auxPath = {current.x + positionsPaths[i].x, current.y + positionsPaths[i].y};

        if (auxPath.x >= 0 && auxPath.x < MAZE_SIZE && auxPath.y >= 0 && auxPath.y < MAZE_SIZE)
        {
            if (!assignedWall(current, aux[i]) && pathViewed(auxPath))
            {
                pathsViewed++;
            }
        }
    }

    return (pathsViewed + wallsViewed == 4);
}

Place nextPlace(Place current, int direction)
{
    Place next = current;

    switch (direction % 4)
    {
    case 0:
        next.x -= 1;
        break;
    case 1:
    case -3:
        next.y += 1;
        break;
    case 2:
    case -2:
        next.x += 1;
        break;
    case 3:
    case -1:
        next.y -= 1;
        break;
    default:
        // Tratamento de erro: direção inválida
        fprintf(stderr, "Erro: Direção inválida na função nextPlace.\n");
        exit(EXIT_FAILURE);
    }

    return next;
}

int pointToParent(Place current, int direction)
{
    Place Parent = Maze[current.x][current.y]->Parent;
    Place prox = nextPlace(current, direction);

    while (!((prox.x == Parent.x) && (prox.y == Parent.y)))
    {
        direction = turnLeft(direction);
        prox = nextPlace(current, direction);

        if (prox.x < 0 || prox.x >= MAZE_SIZE || prox.y < 0 || prox.y >= MAZE_SIZE)
        {
            // Tratamento de erro: coordenadas inválidas
            fprintf(stderr, "Erro: Coordenadas inválidas na função pointToParent.\n");
            exit(EXIT_FAILURE);
        }
    }

    return direction;
}

int pointOnStack(Place current, int direction, PlaceStack *stack)
{
    if (stack->size > 0)
    {
        Place parentOnStack = stack->array[stack->size - 1];
        Place prox = nextPlace(current, direction);

        while (!((prox.x == parentOnStack.x) && (prox.y == parentOnStack.y)))
        {
            direction = turnLeft(direction);
            prox = nextPlace(current, direction);

            if (prox.x < 0 || prox.x >= MAZE_SIZE || prox.y < 0 || prox.y >= MAZE_SIZE)
            {
                // Tratamento de erro: coordenadas inválidas
                fprintf(stderr, "Erro: Coordenadas inválidas na função pointOnStack.\n");
                exit(EXIT_FAILURE);
            }
        }

        return direction;
    }
    else
    {
        // Tratamento de erro: pilha vazia
        fprintf(stderr, "Erro: Tentativa de apontar na pilha vazia na função pointOnStack.\n");
        exit(EXIT_FAILURE);
    }
}

// Busca em profundidade

void dfsDescend(PlaceStack *comeBackStack, Place current, int direction)
{
    if (comeBackStack->size == 0)
    {
        return;
    }
    else
    {
        Place parentOnStack = comeBackStack->array[comeBackStack->size - 1];
        direction = pointOnStack(current, direction, comeBackStack);

        if (direction < 0 || direction >= MAZE_SIZE)
        {
            // Tratamento de erro: direção inválida
            fprintf(stderr, "Erro: Direção inválida na função dfsDescend.\n");
            exit(EXIT_FAILURE);
        }

        forward();
        pop(comeBackStack);
        dfsDescend(comeBackStack, parentOnStack, direction);
    }
}

void dfsAscend(PlaceStack *comeBackStack, Place current, int direction, PlaceStack *forwardStack)
{
    if (comeBackStack->size == 0)
    {
        pop(forwardStack);
        dfsDescend(forwardStack, current, direction);
        return;
    }
    else
    {
        Place parentOnStack = comeBackStack->array[comeBackStack->size - 1];
        direction = pointOnStack(current, direction, comeBackStack);

        if (direction < 0 || direction >= MAZE_SIZE)
        {
            // Tratamento de erro: direção inválida
            fprintf(stderr, "Erro: Direção inválida na função dfsAscend.\n");
            exit(EXIT_FAILURE);
        }

        forward();
        pop(comeBackStack);
        push(forwardStack, parentOnStack);
        dfsAscend(comeBackStack, parentOnStack, direction, forwardStack);
    }
}

void DFS(Place current, int direction, PlaceStack *stack, PlaceStack *forwardStack) {
    int result;
    while (!neighbrohoodViewed(current, direction)) {
        Place prox = nextPlace(current, direction);

        if (prox.x < 0 || prox.x >= MAZE_SIZE || prox.y < 0 || prox.y >= MAZE_SIZE) {
            fprintf(stderr, "Erro: Coordenadas inválidas na função DFS.\n");
            exit(EXIT_FAILURE);
        }

        if (!pathViewed(prox) && !assignedWall(prox, direction)) {
            result = forward();

            switch (result) {
                case 2:
                    Viewed(prox);
                    isParent(prox, current);
                    push(forwardStack, prox);
                    dfsAscend(stack, prox, direction, forwardStack);
                    return;

                case 1:
                    Viewed(prox);
                    isParent(prox, current);
                    push(stack, prox);
                    DFS(prox, direction, stack, forwardStack);
                    return;

                case 0:
                    isWall(current, direction);
                    direction = turnRight(direction);
                    break;

                default:
                    fprintf(stderr, "Erro: Resultado inválido na função forward.\n");
                    exit(EXIT_FAILURE);
            }
        } else {
            direction = turnRight(direction);
        }
    }

    Place Parent = Maze[current.x][current.y]->Parent;

    if (Parent.x < 0 || Parent.x >= MAZE_SIZE || Parent.y < 0 || Parent.y >= MAZE_SIZE) {
        fprintf(stderr, "Erro: Coordenadas inválidas na função DFS.\n");
        exit(EXIT_FAILURE);
    }

    direction = pointToParent(current, direction);
    forward();
    pop(stack);
    DFS(Parent, direction, stack, forwardStack);
}

void setupMaze(Vertex* Maze[MAZE_SIZE][MAZE_SIZE]) {
    for (int i = 0; i < MAZE_SIZE; i++) {
        for (int j = 0; j < MAZE_SIZE; j++) {
            Maze[i][j] = malloc(sizeof(Vertex));
            Maze[i][j]->Viewed = false;
        }
    }
}

void cleanupMaze(Vertex* Maze[MAZE_SIZE][MAZE_SIZE]) {
    for (int i = 0; i < MAZE_SIZE; i++) {
        for (int j = 0; j < MAZE_SIZE; j++) {
            free(Maze[i][j]);
        }
    }
}

int main() {
    PlaceStack* placeStack = createStack(MAZE_SIZE * MAZE_SIZE);
    PlaceStack* goalStack = createStack(MAZE_SIZE * MAZE_SIZE);
    int direction = 0;

    // Configuração inicial do labirinto
    setupMaze(Maze);

    Place start = {MAZE_SIZE / 2, MAZE_SIZE / 2}; // Define o ponto de partida
    Viewed(start);
    push(placeStack, start);

    // Executa DFS a partir do ponto de partida
    DFS(start, direction, placeStack, goalStack);

    // Limpeza e liberação de memória
    cleanupMaze(Maze);
    free(placeStack);
    free(goalStack);

    return 0;
}