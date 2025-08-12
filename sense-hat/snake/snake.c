#define _GNU_SOURCE
#define DEV_INPUT_EVENT "/dev/input"
#define EVENT_DEV_NAME "event"
#define DEV_FB "/dev"
#define FB_DEV_NAME "fb"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <time.h>
#include <poll.h>
#include <dirent.h>
#include <string.h>

#include <linux/input.h>
#include <linux/fb.h>

enum direction_t{
	UP,
	RIGHT,
	DOWN,
	LEFT,
	NONE,
};
struct segment_t {
	struct segment_t *next;
	int x;
	int y;
};
struct snake_t {
	struct segment_t head;
	struct segment_t *tail;
	enum direction_t heading;
};
struct apple_t {
	int x;
	int y;
};

struct fb_t {
	uint16_t pixel[8][8];
};


int running = 1;

struct snake_t snake = {
	{NULL, 4, 4},
	NULL,
	NONE,
};
struct apple_t apple = {
	4, 4,
};

struct fb_t *fb;

static int is_event_device(const struct dirent *dir)
{
	return strncmp(EVENT_DEV_NAME, dir->d_name,
		       strlen(EVENT_DEV_NAME)-1) == 0;
}
static int is_framebuffer_device(const struct dirent *dir)
{
	return strncmp(FB_DEV_NAME, dir->d_name,
		       strlen(FB_DEV_NAME)-1) == 0;
}

static int open_evdev(const char *dev_name)
{
	struct dirent **namelist;
	int i, ndev;
	int fd = -1;

	ndev = scandir(DEV_INPUT_EVENT, &namelist, is_event_device, versionsort);
	if (ndev <= 0)
		return ndev;

	for (i = 0; i < ndev; i++)
	{
		char fname[64];
		char name[256];

		snprintf(fname, sizeof(fname),
			 "%s/%s", DEV_INPUT_EVENT, namelist[i]->d_name);
		fd = open(fname, O_RDONLY);
		if (fd < 0)
			continue;
		ioctl(fd, EVIOCGNAME(sizeof(name)), name);
		if (strcmp(dev_name, name) == 0)
			break;
		close(fd);
	}

	for (i = 0; i < ndev; i++)
		free(namelist[i]);

	return fd;
}

static int open_fbdev(const char *dev_name)
{
	struct dirent **namelist;
	int i, ndev;
	int fd = -1;
	struct fb_fix_screeninfo fix_info;

	ndev = scandir(DEV_FB, &namelist, is_framebuffer_device, versionsort);
	if (ndev <= 0)
		return ndev;

	for (i = 0; i < ndev; i++)
	{
		char fname[64];
		char name[256];

		snprintf(fname, sizeof(fname),
			 "%s/%s", DEV_FB, namelist[i]->d_name);
		fd = open(fname, O_RDWR);
		if (fd < 0)
			continue;
		ioctl(fd, FBIOGET_FSCREENINFO, &fix_info);
		if (strcmp(dev_name, fix_info.id) == 0)
			break;
		close(fd);
		fd = -1;
	}
	for (i = 0; i < ndev; i++)
		free(namelist[i]);

	return fd;
}

void render()
{
	struct segment_t *seg_i;
	memset(fb, 0, 128);
	fb->pixel[apple.x][apple.y]=0xF800;
	for(seg_i = snake.tail; seg_i->next; seg_i=seg_i->next) {
		fb->pixel[seg_i->x][seg_i->y] = 0x7E0;
	}
	fb->pixel[seg_i->x][seg_i->y]=0xFFFF;
}

int check_collision(int appleCheck)
{
	struct segment_t *seg_i;

	if (appleCheck) {
		for (seg_i = snake.tail; seg_i; seg_i=seg_i->next) {
			if (seg_i->x == apple.x && seg_i->y == apple.y)
				return 1;
			}
		return 0;
	}

	for(seg_i = snake.tail; seg_i->next; seg_i=seg_i->next) {
		if (snake.head.x == seg_i->x && snake.head.y == seg_i->y)
			return 1;
	}

	if (snake.head.x < 0 || snake.head.x > 7 ||
	    snake.head.y < 0 || snake.head.y > 7) {
		return 1;
	}
	return 0;
}

void game_logic(void)
{
	struct segment_t *seg_i;
	struct segment_t *new_tail;
	for(seg_i = snake.tail; seg_i->next; seg_i=seg_i->next) {
		seg_i->x = seg_i->next->x;
		seg_i->y = seg_i->next->y;
	}
	if (check_collision(1)) {
		new_tail = malloc(sizeof(struct segment_t));
		if (!new_tail) {
			printf("Ran out of memory.\n");
			running = 0;
			return;
		}
		new_tail->x=snake.tail->x;
		new_tail->y=snake.tail->y;
		new_tail->next=snake.tail;
		snake.tail = new_tail;

		while (check_collision(1)) {
			apple.x = rand() % 8;
			apple.y = rand() % 8;
		}
	}
	switch (snake.heading) {
		case LEFT:
			seg_i->y--;
			break;
		case DOWN:
			seg_i->x++;
			break;
		case RIGHT:
			seg_i->y++;
			break;
		case UP:
			seg_i->x--;
			break;
	}
}

void reset(void)
{
	struct segment_t *seg_i;
	struct segment_t *next_tail;
	seg_i=snake.tail;
	while (seg_i->next) {
		next_tail=seg_i->next;
		free(seg_i);
		seg_i=next_tail;
	}
	snake.tail=seg_i;
	snake.tail->next=NULL;
	snake.tail->x=2;
	snake.tail->y=3;
	apple.x = rand() % 8;
	apple.y = rand() % 8;
	snake.heading = NONE;
}

void change_dir(unsigned int code)
{
	switch (code) {
		case KEY_UP:
			if (snake.heading != DOWN)
				snake.heading = UP;
			break;
		case KEY_RIGHT:
			if (snake.heading != LEFT)
				snake.heading = RIGHT;
			break;
		case KEY_DOWN:
			if (snake.heading != UP)
				snake.heading = DOWN;
			break;
		case KEY_LEFT:
			if (snake.heading != RIGHT)
				snake.heading = LEFT;
			break;
	}
}

void handle_events(int evfd)
{
	struct input_event ev[64];
	int i, rd;

	rd = read(evfd, ev, sizeof(struct input_event) * 64);
	if (rd < (int) sizeof(struct input_event)) {
		fprintf(stderr, "expected %d bytes, got %d\n",
		        (int) sizeof(struct input_event), rd);
		return;
	}
	for (i = 0; i < rd / sizeof(struct input_event); i++) {
		if (ev->type != EV_KEY)
			continue;
		if (ev->value != 1)
			continue;
		switch (ev->code) {
			case KEY_ENTER:
				running = 0;
				break;
			default:
				change_dir(ev->code);
		}
	}
}

int main(int argc, char* args[])
{
	int ret = 0;
	int fbfd = 0;
	struct pollfd evpoll = {
		.events = POLLIN,
	};
	
	srand (time(NULL));

	evpoll.fd = open_evdev("Raspberry Pi Sense HAT Joystick");
	if (evpoll.fd < 0) {
		fprintf(stderr, "Event device not found.\n");
		return evpoll.fd;
	}

	fbfd = open_fbdev("RPi-Sense FB");
	if (fbfd <= 0) {
		ret = fbfd;
		printf("Error: cannot open framebuffer device.\n");
		goto err_ev; 
		
	}
	
	fb = mmap(0, 128, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
	if (!fb) {
		ret = EXIT_FAILURE;
		printf("Failed to mmap.\n");
		goto err_fb;
	}
	memset(fb, 0, 128);

	snake.tail = &snake.head;
	reset();
	while (running) {
		while (poll(&evpoll, 1, 0) > 0)
			handle_events(evpoll.fd);
		game_logic();
		if (check_collision(0)) {
			reset();
		}
		render();
		usleep (300000);
	}
	memset(fb, 0, 128);
	reset();
	munmap(fb, 128);
err_fb:
	close(fbfd);
err_ev:
	close(evpoll.fd);
	return ret;
}
