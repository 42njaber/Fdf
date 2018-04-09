float4					vec_mat_mult(float16 mat, float4 vec);
void					put_px(float2 px, uchar4 color, __global uchar4 *buf, int2 img_size);
uchar4					choose_color(float z);
uchar4 __OVERLOAD__		mix(uchar4 c1, uchar4 c2, float gradiant);

__constant uchar4 colors[5] = {(uchar4)(0x44, 0x44, 0x44, 0x00), (uchar4)(0xDD, 0xDD, 0xDD, 0x00),
			(uchar4)(0x00, 0xFF, 0xFF, 0x00), (uchar4)(0x00, 0x77, 0xFF, 0x00), (uchar4)(0x00, 0x00, 0xFF, 0x00)};

float4					vec_mat_mult(float16 mat, float4 vec)
{
	float4	ret;
	float	w;

	w = (mat.sc * vec.x) + (mat.sd * vec.y) + (mat.se * vec.z) + mat.sf;
	ret.x = ((mat.s0 * vec.x) + (mat.s1 * vec.y) + (mat.s2 * vec.z) + mat.s3) / w;
	ret.y = ((mat.s4 * vec.x) + (mat.s5 * vec.y) + (mat.s6 * vec.z) + mat.s7) / w;
	ret.z = ((mat.s8 * vec.x) + (mat.s9 * vec.y) + (mat.sa * vec.z) + mat.sb) / w;
	ret.w = 1;
	return (ret);
}

void					put_px(float2 px, uchar4 color, __global uchar4 *buf, int2 img_size)
{
	float	a;
	float4	tmp;
	uchar4	buf_tmp;

	if (px.x < 0 || px.y < 0 || px.x >= img_size.x || px.y >= img_size.y)
		return ;
	a = (1 - color.a / (float)255);
	buf_tmp = buf[(int)px.x + (int)px.y * img_size.x];
	tmp = (float4)(fmin(color.r * a + buf_tmp.r, 255),
				   fmin(color.g * a + buf_tmp.g, 255),
				   fmin(color.b * a + buf_tmp.b, 255), 0);
	buf[(int)px.x + (int)px.y * img_size.x] = (uchar4)(tmp.r, tmp.g, tmp.b, 0);
}

uchar4 __OVERLOAD__ mix(uchar4 c1, uchar4 c2, float gradiant)
{
	float4	tmp;
	float4	tmp1;
	float4	tmp2;

	tmp1 = (float4)(c1.x, c1.y, c1.z, c1.w);
	tmp2 = (float4)(c2.x, c2.y, c2.z, c2.w);
	tmp = mix(tmp1, tmp2, gradiant);
	return ((uchar4)(tmp.x, tmp.y, tmp.z, tmp.w));
}

uchar4					choose_color(float z)
{
	uchar4			ret;

	if (z < -20)
		ret = (uchar4)colors[0];
	else if (z < -10)
		ret = (uchar4)mix(colors[0], colors[1], (z + 20) / 10);
	else if (z < 0)
		ret = (uchar4)mix(colors[1], colors[2], (z + 10) / 10);
	else if (z < 10)
		ret = (uchar4)mix(colors[2], colors[3], z / 10);
	else if (z < 20)
		ret = (uchar4)mix(colors[3], colors[4], (z - 10) / 10);
	else
		ret = (uchar4)colors[4];
	return (ret);
}

__kernel void			draw_vbo(
						   __global uchar4 *buf,
						   __global float4 *vbo,
						   __global uint2 *vbo_idx,
						   uint pixel_sz, uint line,
						   int2 img_size,
						   int antialiasing,
						   int is_perspective_active,
						   float16 transformationMatrix,
						   float16 perspectiveMatrix,
						   float16 alignMatrix,
						   float zoom
						  )
{
	int		id;
	float4	v1;
	float4	v2;
	float4	px;
	float4	unit;
	float	offset;
	uchar4	color;

	id = get_global_id(0);
	px = vbo[vbo_idx[id].x];
	v1.z = px.z;
	px = vec_mat_mult(transformationMatrix, px);
	v1.w = px.z;
	px = vec_mat_mult(perspectiveMatrix, px);
	px = vec_mat_mult(alignMatrix, px);
	v1.xy = px.xy;
	px = vbo[vbo_idx[id].y];
	v2.z = px.z;
	px = vec_mat_mult(transformationMatrix, px);
	v2.w = px.z;
	px = vec_mat_mult(perspectiveMatrix, px);
	px = vec_mat_mult(alignMatrix, px);
	v2.xy = px.xy;
	if (is_perspective_active)
	{
		if (v1.w <= 1 && v2.w <= 1)
			return ;
		else if (v1.w <= 1)
			v1 = mix(v2, v1, (float)(1 - (v1.w - 1) / (v1.w - v2.w)) * sign(v1.w));
		else if (v2.w <= 1)
			v2 = mix(v1, v2, (float)(1 - (1 - v2.w) / (v1.w - v2.w)) * sign(v2.w));
	}
	if (v1.x < 0 || v1.y < 0 || v1.x >= img_size.x || v1.y >= img_size.y)
	{
		if (v2.x < 0 || v2.y < 0 || v2.x >= img_size.x || v2.y >= img_size.y)
		{
			if ((v1.x < 0 && v2.x < 0) || (v1.y < 0 && v2.y < 0) ||
					(v1.x >= img_size.x && v2.x >= img_size.x) || (v1.y >= img_size.y && v2.y >= img_size.y) ||
					(fabs(mix(v1.y, v2.y, v1.x / (v1.x - v2.x)) - img_size.y / 2) > img_size.y / 2 &&
					fabs(mix(v1.y, v2.y, (v1.x - img_size.x) / (v1.x - v2.x)) - img_size.y / 2) > img_size.y / 2 &&
					((v1.y < img_size.y && v2.y < img_size.y) || (v1.y >= 0 && v2.y >= 0))))
				return ;
			v2 = mix(v2, v1, fmax((fabs(v2.x - img_size.x / 2) - img_size.x / 2) / fabs(v1.x - v2.x),
					(fabs(v2.y - img_size.y / 2) - img_size.y / 2) / fabs(v1.y - v2.y)));
		}
		v1 = mix(v1, v2, fmax((fabs(v1.x - img_size.x / 2) - img_size.x / 2) / fabs(v2.x - v1.x),
				(fabs(v1.y - img_size.y / 2) - img_size.y / 2) / fabs(v2.y - v1.y)));
	}
	else if (v2.x < 0 || v2.y < 0 || v2.x >= img_size.x || v2.y >= img_size.y)
		v2 = mix(v2, v1, fmax((fabs(v2.x - img_size.x / 2) - img_size.x / 2) / fabs(v1.x - v2.x),
					(fabs(v2.y - img_size.y / 2) - img_size.y / 2) / fabs(v1.y - v2.y)));
	if (fabs(v1.x - v2.x) > fabs(v1.y - v2.y))
		unit = (float4)(sign(v2.x - v1.x), (v2.y - v1.y) / fabs(v1.x - v2.x), (v2.z - v1.z) / fabs(v1.x - v2.x), (v2.w - v1.w) / fabs(v1.x - v2.x));
	else
		unit = (float4)((v2.x - v1.x) / fabs(v1.y - v2.y), sign(v2.y - v1.y), (v2.z - v1.z) / fabs(v1.y - v2.y), (v2.w - v1.w) / fabs(v1.y - v2.y));
	px = v1;
	while (distance(v1.xy, v2.xy) > distance(v1.xy, px.xy) - 0.5)
	{
		color = (uchar4)(choose_color(px.z).rgb, 0xFF * (1 - (1 / fmax((float)0, (float)pow(1 + px.w / zoom, 2) - 2))));
		if (antialiasing == 1)
		{
			if (fabs(unit.x) == 1)
			{
				offset = fmod((float)(px.y + 0.5), (float)1);
				put_px(px.xy + (float2)(0, -0.5), (uchar4)(color.rgb, color.a + (0xFF - color.a) * offset), buf, img_size);
				put_px(px.xy + (float2)(0, 0.5), (uchar4)(color.rgb, color.a + (0xFF - color.a) * (1 - offset)), buf, img_size);
			}
			else
			{
				offset = fmod((float)(px.x + 0.5), (float)1);
				put_px(px.xy + (float2)(-0.5, 0), (uchar4)(color.rgb, color.a + (0xFF - color.a) * offset), buf, img_size);
				put_px(px.xy + (float2)(0.5, 0), (uchar4)(color.rgb, color.a + (0xFF - color.a) * (1 - offset)), buf, img_size);
			}
		}
		else if (antialiasing == 2)
		{
			put_px(px.xy, color, buf, img_size);
			if (fabs(unit.x) == 1)
			{
				offset = fmod(px.y, 1);
				put_px(px.xy + (float2)(0, -1), (uchar4)(color.rgb, color.a + (0xFF - color.a) * offset), buf, img_size);
				put_px(px.xy + (float2)(0, 1), (uchar4)(color.rgb, color.a + (0xFF - color.a) * (1 - offset)), buf, img_size);
			}
			else
			{
				offset = fmod(px.x, 1);
				put_px(px.xy + (float2)(-1, 0), (uchar4)(color.rgb, color.a + (0xFF - color.a) * offset), buf, img_size);
				put_px(px.xy + (float2)(1, 0), (uchar4)(color.rgb, color.a + (0xFF - color.a) * (1 - offset)), buf, img_size);
			}
		}
		else
			put_px(px.xy, color, buf, img_size);
		px += unit;
	}
}

__kernel void	clear_buf(__global uchar4 *buf,
							uchar4 val)
{
	int		id;

	id = get_global_id(0);
	buf[id] = (uchar4)(val.r, val.g, val.b, 0x00);
}
