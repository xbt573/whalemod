#ifndef WHALE_H_
#define WHALE_H_

#define WHALE1 "       .                  \n"
#define WHALE2 "      \":\"               \n"
#define WHALE3 "    ___:____     |\"\\/\"|\n"
#define WHALE4 "  ,'        `.    \\  /   \n"
#define WHALE5 "  |  O        \\___/  |   \n"
#define WHALE6 "~^~^~^~^~^~^~^~^~^~^~^~^~ \n"
#define WHALE WHALE1 WHALE2 WHALE3 WHALE4 WHALE5 WHALE6
#define WHALESIZE sizeof(WHALE)

int init_module(void);
void cleanup_module(void);

static int whale_device_open(struct inode *, struct file *);
static int whale_device_release(struct inode *, struct file *);
static ssize_t whale_device_read(struct file *, char *, size_t, loff_t *);
static ssize_t whale_device_write(struct file *, const char *, size_t, loff_t *);
static int whale_uevent(const struct device *dev, struct kobj_uevent_env *env);

static int whalestream_device_open(struct inode *, struct file *);
static int whalestream_device_release(struct inode *, struct file *);
static ssize_t whalestream_device_read(struct file *, char *, size_t, loff_t *);
static ssize_t whalestream_device_write(struct file *, const char *, size_t, loff_t *);
static int whalestream_uevent(const struct device *dev, struct kobj_uevent_env *env);

#define SUCCESS 0
#define WHALE_DEVICE_NAME "whale"
#define WHALESTREAM_DEVICE_NAME "whalestream"
#define BUF_LEN WHALESIZE

#endif // WHALE_H_
