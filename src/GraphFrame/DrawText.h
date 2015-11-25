#ifndef __DRAWTEXT__
#define __DRAWTEXT__

#include <wx/string.h> // instead of <string>
using namespace std;

enum drawPosition_t { CENTER, LEFT, RIGHT };
#define FONT_WIDTH 16
#define FONT_HEIGHT 28

#define FONT_SCALE_MIDDLE 0.875
#define FONT_SCALE_SMALL  0.6875 + 0.01

bool init_font();
bool delete_font();
void draw_text(const char *p, float scale, drawPosition_t pos);
void draw_text(string str, float scale, drawPosition_t pos);
void draw_text(float f, float scale, drawPosition_t pos);

#endif // __DRAWTEXT__
