#include "module.h"
#include "mm.h"
#include "string.h"
#include "fs.h"
#include "pl111.h"

struct lcd_ctrl *g_lcd_ctrl;

void pl111_init(void)
{
	printk("lcd init\n");

	g_lcd_ctrl = ioremap(PL111_BASE);

	struct lcd_h_info hinfo = {
		.ppl = LCD_WIDTH / 16 - 1,
		.hsw = 0,
		.hfp = 0,
		.hbp = 0,
	};

	struct lcd_w_info winfo = {
		.lpp = LCD_HEIGHT - 1,
		.vsw = 0,
		.vfp = 0,
		.vbp = 0,
	};

	struct lcd_ctrl_info cinfo = {
		.en = 1,
		.bpp = 0b101, /* 24 bpp tft only*/
		.tft = 1,
		.pwr = 1,
	};

	pl111_set_h_info(&hinfo);
	pl111_set_w_info(&winfo);
	pl111_set_frame_buffer(NULL);
	pl111_set_ctrl_info(&cinfo);
	pl111_draw_background("myos.raw");
}

void pl111_draw_image(char *image_buffer)
{
	size_t fb_bytes = LCD_WIDTH * LCD_HEIGHT * 4;
	size_t nr_fb_pages = PAGE_ALIGN(fb_bytes) >> PAGE_BITS;
	struct page *pages = buddy_alloc_pages(fb_bytes);
	char *frame_buffer = (char *)page_to_phy(pages);

	for (size_t i = 0; i < nr_fb_pages; i++) {
		ioremap(page_to_phy(&pages[i]));
	}

	char *fb = frame_buffer;
	char *p = image_buffer;
	for (int i = 0; i < LCD_HEIGHT; i++) {
		for (int j = 0; j < LCD_WIDTH; j++) {
			*fb++ = *p++;
			*fb++ = *p++;
			*fb++ = *p++;
			*fb++ = 0;
		}
	}
	pl111_set_frame_buffer(frame_buffer);
}

void pl111_draw_background(char *filename)
{
	size_t fb_bytes = LCD_WIDTH * LCD_HEIGHT * 4;
	char *image_buffer = (char *)kalloc(fb_bytes);
	vfs_read(filename, image_buffer, fb_bytes);
	pl111_draw_image(image_buffer);
}

__init_module__(pl111_init);
