#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>

#define SCREEN_WIDTH 2560
#define SCREEN_HEIGHT 1600
#define NUM_BARS (SCREEN_WIDTH / 8) 
#define DELAY 10

typedef enum {
	BUBBLE_SORT,
	INSERTION_SORT,
	QUICK_SORT,
	MERGE_SORT
} SortType;

void renderText(SDL_Renderer* renderer, TTF_Font* font, int x, int y, const char* text);
void drawBars(SDL_Renderer* renderer, TTF_Font* font, int* array, int size, int highlighted1, int highlighted2, int comparisons, int swaps, SortType sortType);
void shuffleArray(int* array, int size);
bool bubbleSort(SDL_Renderer* renderer, TTF_Font* font, int* array, int size, SortType sortType);
bool insertionSort(SDL_Renderer* renderer, TTF_Font* font, int* array, int size, SortType sortType);
bool quickSortHelper(SDL_Renderer* renderer, TTF_Font* font, int* array, int low, int high, int* comparisons, int* swaps, SortType sortType, bool* quitFlag);
bool quickSort(SDL_Renderer* renderer, TTF_Font* font, int* array, int size, SortType sortType);
bool mergeSort(SDL_Renderer* renderer, TTF_Font* font, int* array, int size, SortType sortType);
bool mergeSortHelper(SDL_Renderer* renderer, TTF_Font* font, int* array, int left, int right, int* temp, int* comparisons, int* swaps, SortType sortType, bool* quitFlag);

int main()
{
	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();

	SDL_Window* window = SDL_CreateWindow(
			"Sorting Visualizer",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			SCREEN_WIDTH,
			SCREEN_HEIGHT, 
			SDL_WINDOW_SHOWN
			);

	SDL_Renderer* renderer = SDL_CreateRenderer(
			window,
			-1,
			SDL_RENDERER_ACCELERATED
			);

	TTF_Font* font = TTF_OpenFont("Ubuntu.ttf", 16);

	int array[NUM_BARS];
	srand((unsigned int)time(NULL));

	for (int i = 0; i < NUM_BARS; i++)
	{
		array[i] = i + 1;
	}
	shuffleArray(array, NUM_BARS);

	SortType currentAlg = BUBBLE_SORT;
	bool quit = false;
	bool sorting = false;

	drawBars(renderer, font, array, NUM_BARS, -1, -1, 0, 0, currentAlg);

	while (!quit)
	{
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			else if (e.type == SDL_KEYDOWN && !sorting)
			{
				if (e.key.keysym.sym == SDLK_ESCAPE)
				{
					quit = true;
				}
				else if (e.key.keysym.sym == SDLK_SPACE)
				{
					sorting = true;
				}
				else if (e.key.keysym.sym == SDLK_1)
				{
					currentAlg = BUBBLE_SORT;
					drawBars(renderer, font, array, NUM_BARS, -1, -1, 0, 0, currentAlg);
				}
				else if (e.key.keysym.sym == SDLK_2)
				{
					currentAlg = INSERTION_SORT;
					drawBars(renderer, font, array, NUM_BARS, -1, -1, 0, 0, currentAlg);
				}
				else if (e.key.keysym.sym == SDLK_3)
				{
					currentAlg = QUICK_SORT;
					drawBars(renderer, font, array, NUM_BARS, -1, -1, 0, 0, currentAlg);
				}
				else if (e.key.keysym.sym == SDLK_4)
				{
					currentAlg = MERGE_SORT;
					drawBars(renderer, font, array, NUM_BARS, -1, -1, 0, 0, currentAlg);
				}
			}
		}

		if (sorting)
		{
			bool result = false;
			switch (currentAlg)
			{
				case BUBBLE_SORT:
					result = bubbleSort(renderer, font, array, NUM_BARS, currentAlg);
					break;
				case INSERTION_SORT:
					result = insertionSort(renderer, font, array, NUM_BARS, currentAlg);
					break;
				case QUICK_SORT:
					result = quickSort(renderer, font, array, NUM_BARS, currentAlg);
					break;
				case MERGE_SORT:
					result = mergeSort(renderer, font, array, NUM_BARS, currentAlg);
					break;
			}

			if (!result) break;

			sorting = false;

			SDL_Delay(5000);
		 	
			for (int i = 0; i < NUM_BARS; i++)
			{
				array[i] = i + 1;
			}

			shuffleArray(array, NUM_BARS);
			drawBars(renderer, font, array, NUM_BARS, -1, -1, 0, 0, currentAlg);
		}

		SDL_Delay(10);
	}

	TTF_CloseFont(font);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	SDL_Quit();
	return 0;
}

void renderText(SDL_Renderer* renderer, TTF_Font* font, int x, int y, const char* text)
{
	SDL_Color color = {255, 255, 255, 255};
	SDL_Surface* surface = TTF_RenderText_Blended(font, text, color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_Rect dst = {x, y, surface->w, surface->h};
	SDL_RenderCopy(renderer, texture, NULL, &dst);
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
}

void drawBars(SDL_Renderer* renderer, TTF_Font* font, int* array, int size, int highlighted1, int highlighted2, int comparisons, int swaps, SortType sortType)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	int barWidth = SCREEN_WIDTH / size;

	for (int i = 0; i < size; i++)
	{
		if (i == highlighted1 || i == highlighted2)
		{
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		}
		else
		{
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		}

		int barHeight = (array[i] * (SCREEN_HEIGHT - 100)) / NUM_BARS;
		SDL_Rect bar = {i * barWidth, SCREEN_HEIGHT - barHeight, barWidth, barHeight};
		SDL_RenderFillRect(renderer, &bar);
	}

	const char* algName = "Bubble Sort";
	if (sortType == INSERTION_SORT) algName = "Insertion Sort";
	if (sortType == QUICK_SORT)	 algName = "Quick Sort";
	if (sortType == MERGE_SORT)	 algName = "Merge Sort";
	
	char stats[128];
	snprintf(stats, sizeof(stats), "Alg: %s  Number of Bars: %d  Comparisons: %d  Swaps: %d", algName, NUM_BARS, comparisons, swaps);
	renderText(renderer, font, 10, 10, stats);

	SDL_RenderPresent(renderer);
}

void shuffleArray(int* array, int size)
{
	for (int i = size - 1; i > 0; i--)
	{
		int j = rand() % (i + 1);
		int temp = array[i];
		array[i] = array[j];
		array[j] = temp;
	}
}

bool bubbleSort(SDL_Renderer* renderer, TTF_Font* font, int* array, int size, SortType sortType)
{
	int comparisons = 0; 
	int swaps = 0;
	SDL_Event e;

	for (int i = 0; i < size - 1; i++)
	{
		for (int j = 0; j < size - i - 1; j++)
		{
			while (SDL_PollEvent(&e))
			{
				if (e.type == SDL_QUIT) return false;
			}

			comparisons++;
			if (array[j] > array[j + 1])
			{
				int tmp = array[j];
				array[j] = array[j + 1];
				array[j + 1] = tmp;
				swaps++;
			}

			drawBars(renderer, font, array, size, j, j + 1, comparisons, swaps, sortType);
			SDL_Delay(DELAY);
		}
	}

	for (int i = 0; i < size; i++)
	{
		drawBars(renderer, font, array, size, i, -1, comparisons, swaps, sortType);
		SDL_Delay(10);
	}

	drawBars(renderer, font, array, size, -1, -1, comparisons, swaps, sortType);
	return true;
}

bool insertionSort(SDL_Renderer* renderer, TTF_Font* font, int* array, int size, SortType sortType)
{
	int comparisons = 0;
	int swaps = 0;
	SDL_Event e;

	for (int i = 1; i < size; i++)
	{
		int key = array[i];
		int j = i - 1;

		while (j >= 0 && array[j] > key)
		{
			while (SDL_PollEvent(&e))
			{
				if (e.type == SDL_QUIT) return false;
			}
			comparisons++;
			array[j + 1] = array[j];
			swaps++;
			j--;

			drawBars(renderer, font, array, size, j + 1, i, comparisons, swaps, sortType);
			SDL_Delay(DELAY);
		}
		array[j + 1] = key;
		drawBars(renderer, font, array, size, j + 1, i, comparisons, swaps, sortType);
		SDL_Delay(DELAY);
	}

	for (int i = 0; i < size; i++)
	{
		drawBars(renderer, font, array, size, i, -1, comparisons, swaps, sortType);
		SDL_Delay(10);
	}

	drawBars(renderer, font, array, size, -1, -1, comparisons, swaps, sortType);
	return true;
}

bool quickSortHelper(SDL_Renderer* renderer, TTF_Font* font, int* array, int low, int high, int* comparisons, int* swaps, SortType sortType, bool* quitFlag)
{
	SDL_Event e;
	if (*quitFlag) return false;

	if (low < high)
	{
		int pivot = array[high];
		int i = low - 1;

		for (int j = low; j < high; j++)
		{
			while (SDL_PollEvent(&e))
			{
				if (e.type == SDL_QUIT)
				{
					*quitFlag = true;
					return false;
				}
			}

			(*comparisons)++;
			if (array[j] < pivot)
			{
				i++;
				int tmp = array[i];
				array[i] = array[j];
				array[j] = tmp;
				(*swaps)++;
			}

			drawBars(renderer, font, array, NUM_BARS, j, high, *comparisons, *swaps, sortType);
			SDL_Delay(DELAY);
		}

		int tmp = array[i + 1];
		array[i + 1] = array[high];
		array[high] = tmp;
		(*swaps)++;

		drawBars(renderer, font, array, NUM_BARS, i + 1, high, *comparisons, *swaps, sortType);
		SDL_Delay(DELAY);

		quickSortHelper(renderer, font, array, low, i, comparisons, swaps, sortType, quitFlag);
		quickSortHelper(renderer, font, array, i + 2, high, comparisons, swaps, sortType, quitFlag);
	}

	return true;
}

bool quickSort(SDL_Renderer* renderer, TTF_Font* font, int* array, int size, SortType sortType)
{
	int comparisons = 0;
	int swaps = 0;
	bool quitFlag = false;
	bool result = quickSortHelper(renderer, font, array, 0, size - 1, &comparisons, &swaps, sortType, &quitFlag);

	if (quitFlag) return false;

	for (int i = 0; i < size; i++)
	{
		drawBars(renderer, font, array, size, i, -1, comparisons, swaps, sortType);
		SDL_Delay(10);
	}

	drawBars(renderer, font, array, size, -1, -1, comparisons, swaps, sortType);
	return result;
}

bool mergeSortHelper(SDL_Renderer* renderer, TTF_Font* font, int* array, int left, int right, int* temp, int* comparisons, int* swaps, SortType sortType, bool* quitFlag)
{
	SDL_Event e;
	if (*quitFlag) return false;

	if (left < right)
	{
		int mid = (left + right) / 2;

		if (!mergeSortHelper(renderer, font, array, left, mid, temp, comparisons, swaps, sortType, quitFlag)) return false;
		if (!mergeSortHelper(renderer, font, array, mid + 1, right, temp, comparisons, swaps, sortType, quitFlag)) return false;

		int i = left, j = mid + 1, k = left;

		while (i <= mid && j <= right)
		{
			while (SDL_PollEvent(&e))
			{
				if (e.type == SDL_QUIT)
				{
					*quitFlag = true;
					return false;
				}
			}

			(*comparisons)++;
			if (array[i] <= array[j])
				temp[k++] = array[i++];
			else
			{
				temp[k++] = array[j++];
				(*swaps)++;
			}

			drawBars(renderer, font, array, NUM_BARS, i, j, *comparisons, *swaps, sortType);
			SDL_Delay(DELAY);
		}

		while (i <= mid)
		{
			temp[k++] = array[i++];
			drawBars(renderer, font, array, NUM_BARS, i - 1, -1, *comparisons, *swaps, sortType);
			SDL_Delay(DELAY);
		}

		while (j <= right)
		{
			temp[k++] = array[j++];
			drawBars(renderer, font, array, NUM_BARS, j - 1, -1, *comparisons, *swaps, sortType);
			SDL_Delay(DELAY);
		}

		for (int x = left; x <= right; x++)
		{
			array[x] = temp[x];
			drawBars(renderer, font, array, NUM_BARS, x, -1, *comparisons, *swaps, sortType);
			SDL_Delay(DELAY);
		}
	}

	return true;
}

bool mergeSort(SDL_Renderer* renderer, TTF_Font* font, int* array, int size, SortType sortType)
{
	int comparisons = 0;
	int swaps = 0;
	bool quitFlag = false;
	int* temp = (int*)malloc(sizeof(int) * size);
	if (!temp) return false;

	bool result = mergeSortHelper(renderer, font, array, 0, size - 1, temp, &comparisons, &swaps, sortType, &quitFlag);

	free(temp);

	if (quitFlag) return false;

	for (int i = 0; i < size; i++)
	{
		drawBars(renderer, font, array, size, i, -1, comparisons, swaps, sortType);
		SDL_Delay(10);
	}

	drawBars(renderer, font, array, size, -1, -1, comparisons, swaps, sortType);
	return result;
}
