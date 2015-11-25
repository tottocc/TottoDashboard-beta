#include "DrawText.h"

#include <wx/glcanvas.h>
#include "PrimitiveFont-ter-u28n.h"
#include "PrimitiveFont-ter-u22b.h"


PrimitiveFontData *primitive_font_data_ter_u28n;
PrimitiveFontData *primitive_font_data_ter_u22b;
int numOfUsed = 0;


static bool getHeaderKanji(const PrimitiveFontData *primitive_font_data,
	int c,
	int &result_body_start,
	int &result_body_length)
{
	c &= ~0xffff8080;
	c -= primitive_font_data->header_min;
	if (c < 0)
	{
		return !0;
	}

	const char *header = reinterpret_cast<const char*>(primitive_font_data);
	header += primitive_font_data->offset_header;
	const PrimitiveFontDataHeader *p = reinterpret_cast<const PrimitiveFontDataHeader*>(header);
	PrimitiveFontDataHeader tmp = *(p + c);
	result_body_start = tmp & 0x00ffffff;
	result_body_length = tmp >> 24;

	if (result_body_length == 0)
	{
		return true;
	}
	else
	{
		return false;
	}

	/* NOTREACHED */
}

static bool getBody(const PrimitiveFontData *primitive_font_data,
	int body_start,
	int body_length,
	int c,
	int &result_body_start,
	int &result_type,
	int &result_x,
	int &result_y,
	int &result_length)
{
	if (result_body_start >= body_start + body_length)
	{
		return false;
	}
	else
	{
		unsigned int tmp;

		const char *body = reinterpret_cast<const char*>(primitive_font_data);
		body += primitive_font_data->offset_body;
		tmp = *((unsigned short*)(body + result_body_start));
		result_body_start += 2;

		result_type = (tmp >> (0 + 5)) & 0x7;
		result_x = (tmp >> (0 + 0)) & 0x1f;
		result_length = (tmp >> (8 + 5)) & 0x7;
		//		result_y = -((tmp >> (8 + 0)) & 0x1f);
		result_y = ((tmp >> (8 + 0)) & 0x1f);
		result_y = -result_y;

		return true;
	}

	/* NOTREACHED */
}

static void drawKanjiLine(const PrimitiveFontData *primitive_font_data,
	int c)
{
	int body_start;
	int body_length;
	int type;
	int x;
	int y;
	int length;

	// get body_start, body_length and then initialize primitive_font_data

	if (getHeaderKanji(primitive_font_data,
		c,
		body_start,
		body_length))
	{
		return;
	}

	int result_body_start = body_start;
	while (getBody(primitive_font_data,
		body_start,
		body_length,
		c,
		result_body_start,
		type,
		x,
		y,
		length))
	{
		switch (type)
		{
		case PRIMITIVE_TYPE_HLINE:
			glBegin(GL_LINES);
			//glBegin(GL_POINTS);
			glVertex2i(x,
				y);
			glVertex2i(x + length,
				y);
			glEnd();
			break;

		case PRIMITIVE_TYPE_VLINE:
			glBegin(GL_LINES);
			//glBegin(GL_POINTS);
			glVertex2i(x,
				y);
			glVertex2i(x,
				y + -length);
			glEnd();
			break;

		case PRIMITIVE_TYPE_POINT:
			glBegin(GL_POINTS);
			glVertex2i(x,
				y);
			glEnd();
			break;

		case PRIMITIVE_TYPE_SLANT_ULDRLINE:
			glBegin(GL_LINES);
			//glBegin(GL_POINTS);
			glVertex2i(x,
				y);
			glVertex2i(x + length,
				y + -length);
			glEnd();
			break;

		case PRIMITIVE_TYPE_SLANT_URDLLINE:
			glBegin(GL_LINES);
			//glBegin(GL_POINTS);
			glVertex2i(x,
				y);
			glVertex2i(x - length,
				y + -length);
			glEnd();
			break;
		}
	}
}

bool init_font()
{
	numOfUsed++;
	if (numOfUsed == 1) {

		const int margin = 1024;
		primitive_font_data_ter_u28n = reinterpret_cast<PrimitiveFontData*>(malloc(sizeof(data_ter_u28n) + margin));

		uintptr_t tmp = reinterpret_cast<uintptr_t>(primitive_font_data_ter_u28n);
		if (tmp & 0x3)
		{
			fprintf(stderr, "memory alignment error\n");
			return false;
		}

		memcpy(primitive_font_data_ter_u28n, data_ter_u28n, sizeof(data_ter_u28n));

		if (primitive_font_data_ter_u28n == 0)
		{
			fprintf(stderr, "memory allocation error\n");
			return false;
		}

		primitive_font_data_ter_u22b = reinterpret_cast<PrimitiveFontData*>(malloc(sizeof(data_ter_u22b) + margin));

		tmp = reinterpret_cast<uintptr_t>(primitive_font_data_ter_u22b);
		if (tmp & 0x3)
		{
			fprintf(stderr, "memory alignment error\n");
			return false;
		}

		memcpy(primitive_font_data_ter_u22b, data_ter_u22b, sizeof(data_ter_u22b));

		if (primitive_font_data_ter_u22b == 0)
		{
			fprintf(stderr, "memory allocation error\n");
			return false;
		}
	}
	return true;
}

bool delete_font()
{
	numOfUsed--;
	if (numOfUsed == 0) {
		free(primitive_font_data_ter_u28n);
		free(primitive_font_data_ter_u22b);
	}
	return true;
}

void draw_text(const char *p, float scale, drawPosition_t pos)
{
	PrimitiveFontData *font;
	if (scale >= FONT_SCALE_MIDDLE)
		font = primitive_font_data_ter_u28n;
	else
		font = primitive_font_data_ter_u22b;

	glScalef(scale, scale, 1);

	int len = strlen(p);
	if (pos == CENTER) {
		glTranslated(-FONT_WIDTH * len / 2.0, 0, 0);
	}
	else if (pos == RIGHT){
		glTranslated(-FONT_WIDTH * len, 0, 0);
	}
	while (*p) {
		drawKanjiLine(font, *p++);
		glTranslated(FONT_WIDTH, 0, 0);
	}
}

void draw_text(string str, float scale, drawPosition_t pos)
{
	const char *p = str.c_str();
	draw_text(p, scale, pos);
}

void draw_text(float f, float scale, drawPosition_t pos)
{
	char str[15];
	sprintf(str, "%.5g", f);
	draw_text(str, scale, pos);
}
