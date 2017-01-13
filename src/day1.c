#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "helpers/read_puzzle_input.h"
#include "helpers/linked_list.h"

static const char *DELIMS = ", ";
const char *data_file = "day1.txt";

void turn_right();
void turn_left();
void travel();
void process_line(void *data, size_t data_len);

enum { NORTH, EAST, SOUTH, WEST };

int compare_locations(void* loc1, void* loc2) {
	int l1_x = ((int*)loc1)[0];
	int l1_y = ((int*)loc1)[1];
	
	int l2_x = ((int*)loc2)[0];
	int l2_y = ((int*)loc2)[1];

	return (l1_x == l2_x && l1_y == l2_y);
}

int twice_visited[2] = { -1, -1 };

void delete_location(void *data) { free(data); }

linked_list *visited_locations = NULL;

/* Globals, needed in travel and turn functions */
int facing = NORTH;
int steps[4] = { 0, 0, 0, 0 };
int loc_x = 0, loc_y = 0;

int main(int argc, char **argv) {
	visited_locations = create_list(delete_location, compare_locations);
  linked_list *input_lines = NULL;

  if (argc > 1) {
    input_lines = read_puzzle_input(argv[1]);
  } else {
    input_lines = read_puzzle_input("day1.txt");
  }
	if (input_lines == NULL || input_lines->length == 0) {
		fprintf(stderr, "Unable to read input from %s!\n", "day1.txt");
		exit(1);
	}

	list_each(input_lines, process_line);

	int dist_ns = steps[NORTH] - steps[SOUTH];
	int dist_ew = steps[EAST] - steps[WEST];

	if (dist_ns < 0) { dist_ns *= -1; }
	if (dist_ew < 0) { dist_ew *= -1; }

	printf("The Easter Bunny's hideout is %d blocks away!\n", dist_ns + dist_ew);

	printf("Oops, read the rest of the instructions!\n");
	printf("The distance to the first twice-visited location is %d blocks away!\n", abs(twice_visited[0]) + abs(twice_visited[1]));

	destroy_list(input_lines);
	destroy_list(visited_locations);

	exit(0);
}

void process_line(void *data, size_t data_len) {
	char *contents = calloc(data_len + 1, sizeof(char));
  memcpy(contents, data, data_len);

	char *tok = strtok(contents, DELIMS);
	do {
		switch (tok[0]) {
			case 'R':
				turn_right();
				break;
			case 'L':
				turn_left();
				break;
			default:
				fprintf(stderr, "Unknown directive: %s\n", tok);
				free(contents);
				exit(1);
		}
		travel(facing, atoi(tok+1), steps);
	} while ((tok = strtok(NULL, DELIMS)) != NULL);

  free(contents);
}

void turn_right() { facing += 1; facing = facing % 4; }
void turn_left() { if (facing == 0) { facing = 3; } else { --facing; } }

void travel(int heading, int distance, int *steps) {
	steps[heading] += distance;
	for (int i = 0; i < distance; ++i) {
		switch (heading) {
			case NORTH:
				loc_x += 1;
				break;
			case SOUTH:
				loc_x -= 1;
				break;
			case EAST:
				loc_y += 1;
				break;
			case WEST:
				loc_y -= 1;
				break;
			default:
				fprintf(stderr, "Unknown direction: %d\n", heading);
				exit(1);
		}
		int loc[2] = { loc_x, loc_y };
		if (twice_visited[0] == -1 && list_contains(visited_locations, loc)) {
			twice_visited[0] = loc_x;
			twice_visited[1] = loc_y;
		} else if (twice_visited[0] == -1) {
			list_push(visited_locations, loc, sizeof(loc));
		}
	}
}
