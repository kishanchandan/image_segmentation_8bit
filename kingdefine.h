#define BMP 1
#define KIN 2
#define GIF 3
#define PCX 4
#define JPG 5

#define GIFHEADLENGTH	13
#define GIFLOCALHEADLENGTH	10

class GIFHEAD
{
public:
	char copyright[6];
	unsigned short int width;
	unsigned short int height;
	unsigned char globe_flag;
	unsigned char background;
	unsigned char tail;
};

class GIFDATAHEAD
{
public:
	unsigned short int left;
	unsigned short int top;
	unsigned short int width;
	unsigned short int height;
	unsigned char local_flag;
};

class GIFCOLOR
{
public:
	unsigned char red;
	unsigned char green;
	unsigned char blue;
};

class PCXHEAD
{
public:
	char manufacture;
	char version;
	char encoding;
	char bitperpixel;
	short int xmin,ymin;
	short int xmax,ymax;
	short int hres;
	short int vres;
	char palette[48];
	char reserved;
	char color_planes;
	short int bytes_per_line;
	short int palette_type;
	char filler[58];
};

