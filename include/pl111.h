#ifndef __PL111_H
#define __PL111_H

#define PL111_BASE 0x10020000
#define LCD_WIDTH  800
#define LCD_HEIGHT 600

struct lcd_ctrl {
	size_t timing_0;
	size_t timing_1;
	size_t timing_2;
	size_t timing_3;
	void *up_base;
	void *lp_base;
	size_t contrl;
};

struct lcd_h_info {
	union {
		size_t value;
		struct {
			size_t und : 2;
			size_t ppl : 6; /* pixels-per-line */
			size_t hsw : 8; /* horizontal synchronization pulse width */
			size_t hfp : 8; /* horizontal front porch */
			size_t hbp : 8; /* horizontal back porch */
		};
	};
};

struct lcd_w_info {
	union {
		size_t value;
		struct {
			size_t lpp : 10; /* lines per panel */
			size_t vsw : 6; /* vertical synchronization pulse width */
			size_t vfp : 8; /* vertical front porch */
			size_t vbp : 8; /* vertical back porch */
		};
	};
};

struct lcd_ctrl_info {
	union {
		size_t value;
		struct {
			size_t en       : 1;
			size_t bpp      : 3;
			size_t bw       : 1;
			size_t tft      : 1;
			size_t mono8    : 1;
			size_t dual     : 1;
			size_t bgr      : 1;
			size_t bebo     : 1;
			size_t bepo     : 1;
			size_t pwr      : 1;
			size_t vcomp    : 2;
			size_t und      : 2;
			size_t watermark: 1;
		};
	};

};

extern struct lcd_ctrl *g_lcd_ctrl;

static inline void pl111_set_h_info(struct lcd_h_info *hinfo)
{
	g_lcd_ctrl->timing_0 = hinfo->value;
}

static inline void pl111_set_w_info(struct lcd_w_info *winfo)
{
	g_lcd_ctrl->timing_1 = winfo->value;
}

static inline void pl111_set_ctrl_info(struct lcd_ctrl_info *cinfo)
{
	g_lcd_ctrl->contrl = cinfo->value;
}

static inline void pl111_set_frame_buffer(char *frame_buffer)
{
	void *old_fb = g_lcd_ctrl->up_base;
	if (old_fb) {
		kfree(old_fb);
	}
	g_lcd_ctrl->up_base = frame_buffer;
}

void pl111_draw_image(char *image_buffer);
void pl111_draw_background(char *filename);

#endif
