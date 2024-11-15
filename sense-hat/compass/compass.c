#define _GNU_SOURCE
#define DEV_IIO_EVENT "/sys/bus/iio/devices"
#define EVENT_IIO_NAME "iio:device"
#define DEV_INPUT_EVENT "/dev/input"
#define EVENT_DEV_NAME "event"
#define DEV_FB "/dev"
#define FB_DEV_NAME "fb"

#define CYAN (0x7FF)
#define GRENN (0x400)
#define READ (0xF800)

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <time.h>
#include <poll.h>
#include <dirent.h>
#include <string.h>
#include <math.h>

#include <linux/input.h>
#include <linux/fb.h>

struct fb_t {
	uint16_t pixel[8][8];
};

enum work_state {
	RUN,
	CALIBRATION,
	NONE,
};

struct compass {
	struct fb_t *fb;
	int fbfd;
	int sensor_fd;
	int Joystick_fd;
	uint16_t current_pixel[8][8];
	uint8_t x;
	uint8_t y;
	int16_t magnx_offset;
	int16_t magny_offset;
	uint16_t magnz_offset;
	float angle;
	enum work_state state;
	struct pollfd *evpoll;
};


uint16_t panel_pixel[8][8] = {
	0x0000, 0x0000, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x0000, 0x0000,
	0x0000, 0x07FF, 0x0000, 0x0000, 0x0000, 0x0000, 0x07FF, 0x0000,
	0x07FF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x07FF,
	0x07FF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x07FF,
	0x07FF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x07FF,
	0x07FF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x07FF,
	0x0000, 0x07FF, 0x0000, 0x0000, 0x0000, 0x0000, 0x07FF, 0x0000,
	0x0000, 0x0000, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x0000, 0x0000,
};

uint16_t calibration_pixel[8][8] = {
	0x0000, 0x0000, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x0000, 0x0000,
	0x0000, 0x07FF, 0x0000, 0x0000, 0x0000, 0x0000, 0x07FF, 0x0000,
	0x07FF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x07FF,
	0x07FF, 0x0000, 0x0000, 0x0400, 0x0400, 0x0000, 0x0000, 0x07FF,
	0x07FF, 0x0000, 0x0000, 0x0400, 0x0400, 0x0000, 0x0000, 0x07FF,
	0x07FF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x07FF,
	0x0000, 0x07FF, 0x0000, 0x0000, 0x0000, 0x0000, 0x07FF, 0x0000,
	0x0000, 0x0000, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x0000, 0x0000,
};

struct angle_map {
	uint8_t x;
	uint8_t y;
	uint16_t angle;
};

struct angle_map angle_map[] = {
	{4, 0, 18},
	{5, 0, 30},
	{6, 1, 54},
	{7, 2, 72},
	{7, 3, 90},
	{7, 4, 108},
	{7, 5, 120},
	{6, 6, 144},
	{5, 7, 162},
	{4, 7, 180},
	{3, 7, 198},
	{2, 7, 216},
	{1, 6, 240},
	{0, 5, 252},
	{0, 4, 270},
	{0, 3, 288},
	{0, 2, 306},
	{1, 1, 324},
	{2, 0, 342},
	{3, 0, 360},
	{},
};

static int is_framebuffer_device(const struct dirent *dir)
{
	return strncmp(FB_DEV_NAME, dir->d_name,
		       strlen(FB_DEV_NAME)-1) == 0;
}

static int is_event_device(const struct dirent *dir)
{
	return strncmp(EVENT_DEV_NAME, dir->d_name,
		       strlen(EVENT_DEV_NAME)-1) == 0;
}

static int is_iio_device(const struct dirent *dir)
{
	return strncmp(EVENT_IIO_NAME, dir->d_name,
		       strlen(EVENT_IIO_NAME)) == 0;
}

static int open_iiodev(const char *dev_name)
{
	struct dirent **namelist;
	int i, ndev;
	int fd = -1;

	ndev = scandir(DEV_IIO_EVENT, &namelist, is_iio_device, versionsort);
	if (ndev <= 0)
		return ndev;

	for (i = 0; i < ndev; i++)
	{
		char fname[64];
		char name[256] = {0};

		snprintf(fname, sizeof(fname),
			 "%s/%s/name", DEV_IIO_EVENT, namelist[i]->d_name);

		fd = open(fname, O_RDONLY);
		if (fd < 0)
			continue;

		read(fd, name, sizeof(name));
		close(fd);
		if (strncmp(dev_name, name, strlen(name) - 1) == 0) {
			memset(fname, 0 ,sizeof(fname));
			sprintf(fname, "/dev/%s%d", EVENT_IIO_NAME, i);
			fd = open(fname,O_RDONLY);
			break;
		}
	}

	for (i = 0; i < ndev; i++)
		free(namelist[i]);

	return fd;
}

int lsm9ds1_magn_iio_set(const char *dev_name)
{
	struct dirent **namelist;
	int i, ndev;
	int fd = -1;
	char fdir[256] = {0};

	ndev = scandir(DEV_IIO_EVENT, &namelist, is_iio_device, versionsort);
	if (ndev <= 0)
		return ndev;

	for (i = 0; i < ndev; i++)
	{
		char fname[64];
		char name[256] = {0};

		snprintf(fname, sizeof(fname),
			 "%s/%s/name", DEV_IIO_EVENT, namelist[i]->d_name);

		fd = open(fname, O_RDONLY);
		if (fd < 0)
			continue;

		read(fd, name, sizeof(name));
		if (strncmp(dev_name, name, strlen(name) - 1) == 0)
			break;
		close(fd);
	}

	system("mkdir  /sys/kernel/config/iio/triggers/hrtimer/my_trigger");
	sprintf(fdir, "echo my_trigger > /sys/bus/iio/devices/iio:device%d/trigger/current_trigger", i);
	system(fdir);
	memset(fdir, 0, sizeof(fdir));

	sprintf(fdir, "echo 1 > /sys/bus/iio/devices/iio:device%d/scan_elements/in_magn_x_en", i);
	system(fdir);
	memset(fdir, 0, sizeof(fdir));

	sprintf(fdir, "echo 1 > /sys/bus/iio/devices/iio:device%d/scan_elements/in_magn_y_en", i);
	system(fdir);
	memset(fdir, 0, sizeof(fdir));

	sprintf(fdir, "echo 1 > /sys/bus/iio/devices/iio:device%d/scan_elements/in_magn_z_en", i);
	system(fdir);
	memset(fdir, 0, sizeof(fdir));

	sprintf(fdir, "echo 1 > /sys/bus/iio/devices/iio:device%d/buffer/enable", i);
	system(fdir);
	memset(fdir, 0, sizeof(fdir));

	return 0;
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

int render(struct compass *compass)
{
	struct fb_t *fb = compass->fb;

	memset(fb, 0, 128);
	memcpy(fb->pixel, compass->current_pixel, sizeof(compass->current_pixel));

	fb->pixel[compass->x][compass->y] = READ;

	return 0;
}

float compass_get_angle(struct compass *compass)
{
	int ret;
	int16_t temp[10];
	int16_t magnx, magny, magnz;
	float angle;
	ret = read(compass->sensor_fd, temp, sizeof(temp));
	if(ret < 0) {
		printf("read err ... \n");
		return -1;
	}

	magnx = temp[0] - compass->magnx_offset;
	magny = temp[1] - compass->magny_offset;
	magnz = temp[2] - compass->magnz_offset;

	if ((magnx > 0) && (magny > 0))
	{
		angle = (atan((double)magny / magnx) * 180) / 3.14159f;
	}
	else if ((magnx > 0) && (magny < 0))
	{
		angle = 360 + (atan((double)magny / magnx) * 180) / 3.14159f;
	}
	else if ((magnx == 0) && (magny > 0))
	{
		angle = 90;
	}
	else if ((magnx == 0) && (magny < 0))
	{
		angle = 270;
	}
	else if (magnx < 0)
	{
		angle = 180 + (atan((double)magny / magnx) * 180) / 3.14159f;
	}

	if (angle > 360) angle = 360;
	if (angle < 0) angle = 0;

	return angle;
}

int angle_to_coordinate(struct compass *compass)
{
	float angle;
	int i;

	angle = compass_get_angle(compass);
	compass->angle = angle;

	for (i = 0; i < 20; ++i) {
		if (angle <= angle_map[i].angle)
			break;
	}

	compass->x = angle_map[i].x;
	compass->y = angle_map[i].y;
	return 0;
}

void handle_events(struct compass *compass)
{
	struct input_event ev[64];
	int i, rd, evfd = compass->Joystick_fd;

	rd = read(evfd, ev, sizeof(struct input_event) * 64);
	if (rd < (int) sizeof(struct input_event)) {
		fprintf(stderr, "expected %d bytes, got %d\n",
		        (int) sizeof(struct input_event), rd);
		return;
	}

	if (ev->type != EV_KEY) return;
	if (ev->value != 1) return;

	switch (ev->code) {
		case KEY_ENTER:
			if (compass->state == RUN) {
				printf("Enter calibration mode ... \n");
				compass->state = CALIBRATION;
				memcpy(compass->current_pixel, calibration_pixel, sizeof(calibration_pixel));
				render(compass);
			} else {
				printf("Enter compass mode ... \n");
				compass->state = RUN;
				memcpy(compass->current_pixel, panel_pixel, sizeof(panel_pixel));
			}
			break;
		default:
			break;
	}
}

int compass_calibration(struct compass *compass)
{
	int16_t x_min = 0;
	int16_t x_max = 0;
	int16_t y_min = 0;
	int16_t y_max = 0;

	int16_t magnx, magny, magnz;
	uint8_t ret;
	int16_t temp[10];
	while (1)
	{
		while (poll(compass->evpoll, 1, 0) > 0)
			handle_events(compass);
		if (compass->state == RUN) break;

		ret = read(compass->sensor_fd, temp, sizeof(temp));
		if(ret < 0) {
			printf("read err ... \n");
			return -1;
		}

		magnx = temp[0];
		magny = temp[1];
		magnz = temp[2];

		if (ret > 0)
		{
			x_max = x_max < magnx ? magnx : x_max;
			x_min = x_min > magnx ? magnx : x_min;
			y_max = y_max < magny ? magny : y_max;
			y_min = y_min > magny ? magny : y_min;
		}
		usleep (100000);
	}

	compass->magnx_offset = (x_max + x_min) / 2;
	compass->magny_offset = (y_max + y_min) / 2;
	printf(" %d %d \n", compass->magnx_offset, compass->magny_offset);
}

int main(int argc, char* args[])
{
	int ret = 0;
	struct compass *compass;
	struct pollfd evpoll;

	srand (time(NULL));

	compass = malloc(sizeof(*compass));
	if (compass == NULL) {
		printf("Error: cannot malloc memory.\n");
		return 0;
	}

	memset(compass, 0, sizeof(*compass));
	memcpy(compass->current_pixel, panel_pixel, sizeof(panel_pixel));

	compass->sensor_fd = open_iiodev("lsm9ds1_magn");
	if (compass->sensor_fd <= 0) {
		ret = compass->sensor_fd;
		printf("Error: cannot open sensor device.\n");
		goto err_mem;
	}

	lsm9ds1_magn_iio_set("lsm9ds1_magn");

	compass->fbfd = open_fbdev("RPi-Sense FB");
	if (compass->fbfd <= 0) {
		ret = compass->fbfd;
		printf("Error: cannot open framebuffer device.\n");
		goto err_sensor;
	}

	compass->fb = mmap(0, 128, PROT_READ | PROT_WRITE, MAP_SHARED, compass->fbfd, 0);
	if (!compass->fb) {
		ret = EXIT_FAILURE;
		printf("Failed to mmap.\n");
		goto err_fb;
	}

	compass->Joystick_fd = open_evdev("Raspberry Pi Sense HAT Joystick");
	if (compass->Joystick_fd < 0) {
		printf("Event device not found.\n");
		ret = compass->Joystick_fd;
		goto err_mmap;
	}

	evpoll.events = POLLIN;
	evpoll.fd = compass->Joystick_fd;
	compass->evpoll = &evpoll;

	compass->magnx_offset = 785;
	compass->magny_offset = 307;

	while(1) {
		while (poll(&evpoll, 1, 0) > 0)
			handle_events(compass);
		switch (compass->state) {
		case RUN:
			ret = angle_to_coordinate(compass);
			if (ret < 0) {
				break;
			}
			printf("angle: %f \n", compass->angle);
			break;
		case CALIBRATION:
			compass_calibration(compass);
			break;
		default:
			break;
		}

		render(compass);
		usleep (100000);
	}

	printf("error \n");

	memset(compass->fb, 0, 128);
err_mmap:
	munmap(compass->fb, 128);
err_fb:
	close(compass->fbfd);
err_sensor:
	close(compass->sensor_fd);
err_mem:
	free(compass);
	return ret;
}
