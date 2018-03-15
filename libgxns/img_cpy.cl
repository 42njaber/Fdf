
#include <math.h>

_kernel void	img_cpy(_global unsigned char *src_buf,
						_global unsigned char *dst_buf)
{
	double	alpha;

	alpha = (double)src_buf[3] / 255.0;
	dst_buf[3] = 0;
	dst_buf[2] = (char)((double)dst_buf[2] * (1 - alpha) +
						(double)src_buf[2] * alpha);
	dst_buf[1] = (char)((double)dst_buf[1] * (1 - alpha) +
						(double)src_buf[1] * alpha);
	dst_buf[0] = (char)((double)dst_buf[0] * (1 - alpha) +
						(double)src_buf[0] * alpha);
}
