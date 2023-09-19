#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define HEAP_CAP 640000
#define CHUNK_LIST_CAP 1024
#define HEAP_FREED_CAP 1024

#define RED "\033[31m"
#define COLOR "\033[35m"
#define COLORDIFF "\033[32m"
#define COLORALIGN "\033[36m"
#define RESET "\033[0m"

typedef struct
{
	char *start;
	size_t size;
} Chunk;

typedef struct
{
	size_t count;
	Chunk chunks[CHUNK_LIST_CAP];
} Chunk_List;

#define UNIMPLEMENTED                                                 \
	do                                                                \
	{                                                                 \
		fprintf(stderr, "%s %u: %s\n", __FILE__, __LINE__, __func__); \
		abort();                                                      \
	} while (0);

void chunk_list_insert(Chunk_List *list, void *start, size_t size)
{
	assert(list->count < CHUNK_LIST_CAP);
	list->chunks[list->count].start = start;
	list->chunks[list->count].size = size;

	for (size_t i = list->count; i > 0 &&
								 list->chunks[i].start < list->chunks[i - 1].start;
		 --i)
	{
		const Chunk tmp = list->chunks[i];
		list->chunks[i] = list->chunks[i - 1];
		list->chunks[i - 1] = tmp;
	}

	list->count += 1;
}

void chunk_list_merge(Chunk_List *dest, const Chunk_List *src)
{
	dest->count = 0;

	for (size_t i = 0; i < src->count; ++i)
	{
		const Chunk chunk = src->chunks[i];
		if (dest->count > 0)
		{
			Chunk *top_chunk = &dest->chunks[dest->count - 1];
			if (top_chunk->start + top_chunk->size == chunk.start)
				top_chunk->size += chunk.size;
			else
				chunk_list_insert(dest, chunk.start, chunk.size);
		}
		else
		{
			chunk_list_insert(dest, chunk.start, chunk.size);
		}
	}
}

void chunk_list_dump(const Chunk_List *list)
{
	printf("Chunks (%zu)\n", list->count);
	for (size_t i = 0; i < list->count; ++i)
	{
		printf(" start: %p, size: %zu\n", list->chunks[i].start, list->chunks[i].size);
	}
}

int chunk_start_compare(const void *a, const void *b)
{
	const Chunk *a_chunk = a;
	const Chunk *b_chunk = b;
	return (a_chunk->start - b_chunk->start);
}

int chunk_list_find(const Chunk_List *list, void *ptr)
{
	Chunk key = {
		.start = ptr};

	Chunk *result = bsearch(&key, list->chunks,
							list->count, sizeof(list->chunks[0]),
							chunk_start_compare);

	if (result)
	{
		assert(list->chunks <= result);
		return ((result - list->chunks) / sizeof(list->chunks[0]));
	}
	else
		return (-1);
}

void chunk_list_remove(Chunk_List *list, size_t index)
{
	assert(index < list->count);
	for (size_t i = index; i < list->count - 1; ++i)
		list->chunks[i] = list->chunks[i + 1];
	list->count -= 1;
}

char heap[HEAP_CAP] = {0};
size_t heap_size = 0;

Chunk_List alloced_chunks = {0};
Chunk_List freed_chunks = {
	.count = 1,
	.chunks = {
		[0] = {.start = heap, .size = sizeof(heap)},
	}};

void *heap_alloc(size_t size)
{
	if (size > 0)
	{
		for (size_t i = 0; i < freed_chunks.count; ++i)
		{
			const Chunk chunk = freed_chunks.chunks[i];
			if (chunk.size >= size)
			{
				chunk_list_remove(&freed_chunks, i);

				const size_t tail_size = chunk.size - size;
				chunk_list_insert(&alloced_chunks, chunk.start, size);

				if (tail_size > 0)
					chunk_list_insert(&freed_chunks, chunk.start + size, tail_size);

				return chunk.start;
			}
		}
	}

	return (NULL);
}

void heap_free(void *ptr)
{
	if (ptr != NULL)
	{
		const int index = chunk_list_find(&alloced_chunks, ptr);
		// printf("what is the index ? %d\n", index);
		assert(index >= 0);

		chunk_list_insert(&freed_chunks, alloced_chunks.chunks[index].start,
						  alloced_chunks.chunks[index].size);
		chunk_list_remove(&alloced_chunks, (size_t)index);
	}
}

int main(void)
{
	char *str;
	int i;

	printf("Starting break is %p\n", sbrk(0));

	for (i = 0; i < 10; i++)
	{
		void *chunk;

		str = heap_alloc(10);
		strcpy(str, "Holberton");
		str[9] = '\0';
		printf("%p: %s, ", (void *)str, str);
		chunk = (void *)(str - sizeof(size_t));
		printf("chunk addr: %p, ", (void *)chunk);
		printf("size: %lu, ", *((size_t *)chunk));
		printf("%sbreak: %p%s\n", RED, sbrk(0), RESET);
	}

	printf("%sFinal break is %p%s\n", RED, sbrk(0), RESET);
	return (EXIT_SUCCESS);
}

// int main()
// {
// 	int i;

// 	for (i = 0; i < 10; ++i)
// 	{
// 		void *p = heap_alloc(i);
// 		if (i % 2 == 0)
// 			heap_free(p);
// 	}

// 	heap_alloc(420);

// 	for (i = 0; i <= 4; ++i)
// 	{
// 		heap_alloc(i);
// 	}

// 	chunk_list_dump(&alloced_chunks);
// 	chunk_list_dump(&freed_chunks);

// 	// heap_free();
// 	return (0);
// }

// stopped at 1 h 56 , Just Watched then
