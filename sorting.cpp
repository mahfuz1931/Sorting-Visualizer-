#include <SDL.h>
#include <iostream>
#include <limits>
#include <time.h>
#include <string>
#include <cstring>
using namespace std;

const int SCREEN_WIDTH = 910;
const int SCREEN_HEIGHT = 750;

const int arrSize = 130;
const int rectSize = 7;

int arr[arrSize];
int Barr[arrSize];

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

bool complete = false;

bool init()
{
    bool success = true;
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        cout << "Couldn't initialize SDL. SDL_Error: " << SDL_GetError() << endl;
        success = false;
    }
    else
    {
        cout << "SDL Initialized successfully.\n";
        
        if (!(SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")))
        {
            cout << "Warning: Linear Texture Filtering not enabled.\n";
        }

        window = SDL_CreateWindow("Sorting Visualizer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == NULL)
        {
            cout << "Couldn't create window. SDL_Error: " << SDL_GetError() << endl;
            success = false;
        }
        else
        {
            cout << "Window created successfully.\n";
            
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if (renderer == NULL)
            {
                cout << "Couldn't create renderer. SDL_Error: " << SDL_GetError() << endl;
                success = false;
            }
            else
            {
                cout << "Renderer created successfully.\n";
            }
        }
    }

    return success;
}

void close()
{
    if (renderer != NULL)
    {
        SDL_DestroyRenderer(renderer);
        renderer = NULL;
    }

    if (window != NULL)
    {
        SDL_DestroyWindow(window);
        window = NULL;
    }

    SDL_Quit();
    cout << "SDL resources cleaned up and SDL quit.\n";
}

void visualize(int x = -1, int y = -1, int z = -1)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    int j = 0;
    for (int i = 0; i <= SCREEN_WIDTH - rectSize; i += rectSize)
    {
        SDL_PumpEvents();

        SDL_Rect rect = {i, 0, rectSize, arr[j]};
        if (complete)
        {
            SDL_SetRenderDrawColor(renderer, 100, 180, 100, 0);
            SDL_RenderDrawRect(renderer, &rect);
        }
        else if (j == x || j == z)
        {
            SDL_SetRenderDrawColor(renderer, 100, 180, 100, 0);
            SDL_RenderFillRect(renderer, &rect);
        }
        else if (j == y)
        {
            SDL_SetRenderDrawColor(renderer, 165, 105, 189, 0);
            SDL_RenderFillRect(renderer, &rect);
        }
        else
        {
            SDL_SetRenderDrawColor(renderer, 170, 183, 184, 0);
            SDL_RenderDrawRect(renderer, &rect);
        }
        j++;
    }
    SDL_RenderPresent(renderer);
}

void inplaceHeapSort(int* input, int n)
{
    for (int i = 1; i < n; i++)
    {
        int childIndex = i;
        int parentIndex = (childIndex - 1) / 2;

        while (childIndex > 0)
        {
            if (input[childIndex] > input[parentIndex])
            {
                swap(input[parentIndex], input[childIndex]);
            }
            else
            {
                break;
            }

            visualize(parentIndex, childIndex);
            SDL_Delay(40);

            childIndex = parentIndex;
            parentIndex = (childIndex - 1) / 2;
        }
    }

    for (int heapLast = n - 1; heapLast >= 0; heapLast--)
    {
        swap(input[0], input[heapLast]);

        int parentIndex = 0;
        int leftChildIndex = 2 * parentIndex + 1;
        int rightChildIndex = 2 * parentIndex + 2;

        while (leftChildIndex < heapLast)
        {
            int maxIndex = parentIndex;

            if (input[leftChildIndex] > input[maxIndex])
            {
                maxIndex = leftChildIndex;
            }
            if (rightChildIndex < heapLast && input[rightChildIndex] > input[maxIndex])
            {
                maxIndex = rightChildIndex;
            }
            if (maxIndex == parentIndex)
            {
                break;
            }

            swap(input[parentIndex], input[maxIndex]);

            visualize(maxIndex, parentIndex, heapLast);
            SDL_Delay(40);

            parentIndex = maxIndex;
            leftChildIndex = 2 * parentIndex + 1;
            rightChildIndex = 2 * parentIndex + 2;
        }
    }
}

int partition_array(int a[], int si, int ei)
{
    int count_small = 0;

    for (int i = si + 1; i <= ei; i++)
    {
        if (a[i] <= a[si])
        {
            count_small++;
        }
    }
    int c = si + count_small;
    swap(a[c], a[si]);
    visualize(c, si);

    int i = si, j = ei;

    while (i < c && j > c)
    {
        if (a[i] <= a[c])
        {
            i++;
        }
        else if (a[j] > a[c])
        {
            j--;
        }
        else
        {
            swap(a[i], a[j]);

            visualize(i, j);
            SDL_Delay(70);

            i++;
            j--;
        }
    }
    return c;
}

void quickSort(int a[], int si, int ei)
{
    if (si >= ei)
    {
        return;
    }

    int c = partition_array(a, si, ei);
    quickSort(a, si, c - 1);
    quickSort(a, c + 1, ei);
}

void merge2SortedArrays(int a[], int si, int ei)
{
    int size_output = (ei - si) + 1;
    int* output = new int[size_output];

    int mid = (si + ei) / 2;
    int i = si, j = mid + 1, k = 0;
    while (i <= mid && j <= ei)
    {
        if (a[i] <= a[j])
        {
            output[k] = a[i];
            visualize(i, j);
            i++;
        }
        else
        {
            output[k] = a[j];
            visualize(i, j);
            j++;
        }
        k++;
    }
    while (i <= mid)
    {
        output[k++] = a[i++];
        visualize();
    }
    while (j <= ei)
    {
        output[k++] = a[j++];
        visualize();
    }
    for (int l = si, x = 0; l <= ei; l++, x++)
    {
        a[l] = output[x];
        visualize(l);
        SDL_Delay(15);
    }
    delete[] output;
}

void mergeSort(int a[], int si, int ei)
{
    if (si >= ei)
    {
        return;
    }
    int mid = (si + ei) / 2;

    mergeSort(a, si, mid);
    mergeSort(a, mid + 1, ei);

    merge2SortedArrays(a, si, ei);
}

void bubbleSort()
{
    for (int i = 0; i < arrSize - 1; i++)
    {
        for (int j = 0; j < arrSize - 1 - i; j++)
        {
            if (arr[j + 1] < arr[j])
            {
                swap(arr[j], arr[j + 1]);
                visualize(j + 1, j, arrSize - i);
            }
            SDL_Delay(1);
        }
    }
}

void insertionSort()
{
    for (int i = 1; i < arrSize; i++)
    {
        int j = i - 1;
        int temp = arr[i];
        while (j >= 0 && arr[j] > temp)
        {
            arr[j + 1] = arr[j];
            j--;
            visualize(i, j + 1);
            SDL_Delay(5);
        }
        arr[j + 1] = temp;
    }
}

void selectionSort()
{
    for (int i = 0; i < arrSize - 1; i++)
    {
        int minIndex = i;
        for (int j = i + 1; j < arrSize; j++)
        {
            if (arr[j] < arr[minIndex])
            {
                minIndex = j;
                visualize(i, minIndex);
            }
            SDL_Delay(1);
        }
        swap(arr[i], arr[minIndex]);
    }
}

void loadArr()
{
    memcpy(arr, Barr, sizeof(int) * arrSize);
}

void randomizeAndSaveArray()
{
    unsigned int seed = (unsigned)time(NULL);
    srand(seed);
    for (int i = 0; i < arrSize; i++)
    {
        int random = rand() % SCREEN_HEIGHT;
        Barr[i] = random;
    }
}

void execute()
{
    if (!init())
    {
        cout << "SDL Initialization Failed.\n";
        return;
    }

    cout << "Randomizing array and starting main loop.\n";
    randomizeAndSaveArray();
    loadArr();

    SDL_Event e;
    bool quit = false;
    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
                complete = false;
            }
            else if (e.type == SDL_KEYDOWN)
            {
                switch (e.key.keysym.sym)
                {
                    case (SDLK_q):
                        quit = true;
                        complete = false;
                        cout << "\nEXITING SORTING VISUALIZER.\n";
                        break;
                    case (SDLK_0):
                        randomizeAndSaveArray();
                        complete = false;
                        loadArr();
                        cout << "\nNEW RANDOM LIST GENERATED.\n";
                        break;
                    case (SDLK_1):
                        loadArr();
                        cout << "\nSELECTION SORT STARTED.\n";
                        complete = false;
                        selectionSort();
                        complete = true;
                        cout << "\nSELECTION SORT COMPLETE.\n";
                        break;
                    case (SDLK_2):
                        loadArr();
                        cout << "\nINSERTION SORT STARTED.\n";
                        complete = false;
                        insertionSort();
                        complete = true;
                        cout << "\nINSERTION SORT COMPLETE.\n";
                        break;
                    case (SDLK_3):
                        loadArr();
                        cout << "\nBUBBLE SORT STARTED.\n";
                        complete = false;
                        bubbleSort();
                        complete = true;
                        cout << "\nBUBBLE SORT COMPLETE.\n";
                        break;
                    case (SDLK_4):
                        loadArr();
                        cout << "\nHEAP SORT STARTED.\n";
                        complete = false;
                        inplaceHeapSort(arr, arrSize);
                        complete = true;
                        cout << "\nHEAP SORT COMPLETE.\n";
                        break;
                    case (SDLK_5):
                        loadArr();
                        cout << "\nMERGE SORT STARTED.\n";
                        complete = false;
                        mergeSort(arr, 0, arrSize - 1);
                        complete = true;
                        cout << "\nMERGE SORT COMPLETE.\n";
                        break;
                    case (SDLK_6):
                        loadArr();
                        cout << "\nQUICK SORT STARTED.\n";
                        complete = false;
                        quickSort(arr, 0, arrSize - 1);
                        complete = true;
                        cout << "\nQUICK SORT COMPLETE.\n";
                        break;
                }
            }
        }
        visualize();
    }

    cout << "Exiting main loop, cleaning up resources.\n";
    close();
}

bool controls()
{
    cout << "ENTER YOUR CHOICE:\n\n";
    cout << "Press (0) : Randomize List\n";
    cout << "Press (1) : Selection Sort\n";
    cout << "Press (2) : Insertion Sort\n";
    cout << "Press (3) : Bubble Sort\n";
    cout << "Press (4) : Heap Sort\n";
    cout << "Press (5) : Merge Sort\n";
    cout << "Press (6) : Quick Sort\n";
    cout << "Press (Q) : Quit\n";

    string choice;
    getline(cin, choice);

    if (choice == "Q" || choice == "q")
    {
        return false;
    }
    return true;
}

int main(int argc, char* args[])
{
    cout << "==============================Sorting Visualizer==============================\n\n";
    cout << "Press ENTER to start the program...";
    cin.ignore();

    while (true)
    {
        if (controls())
            execute();
        else
        {
            cout << "\nEXITING PROGRAM.\n";
            break;
        }
    }

    cout << "Program ended successfully.\n";
    return 0;
}
