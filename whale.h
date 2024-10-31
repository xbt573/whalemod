#include <linux/types.h> 

#define WHALE	"       .\n"\
		"      \":\"\n"\
		"    ___:____     |\"\\/\"|\n"\
		"  ,'        `.    \\  /\n"\
		"  |  O        \\___/  |\n"\
		"~^~^~^~^~^~^~^~^~^~^~^~^~\n"
#define DEVICE_NAME "whale"
#define SUCCESS 0

static int whale_open(struct inode *, struct file *);
static int whale_release(struct inode *, struct file *);
static ssize_t whale_read(struct file *, char *, size_t, loff_t *);
static ssize_t whale_write(struct file *, const char *, size_t, loff_t *);

static char *whale_devnode(const struct device *, umode_t *);
