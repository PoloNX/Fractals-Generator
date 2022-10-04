
#ifdef GL_ES
precision highp float
#endif

uniform float max_iteration;
uniform float r;
uniform float scale;
uniform sampler2D palette;

uniform vec2 center;
uniform vec2 resolution;

uniform int fractal;
uniform float R;
uniform float n;

int mandelbrot(vec2 p)
{
    int iterations = 0;
	
	float c_real = (resolution.x / resolution.y) * (p.x - 0.5) * scale - center.x / resolution.y;
	float c_imag = (p.y - 0.5) * scale - center.y / resolution.y;

	float z_real = c_real;
	float z_imag = c_imag;

	int i = 0;
	while(i < max_iteration) {
		float z2_real = z_real * z_real - z_imag * z_imag + c_real;
		float z2_imag = 2.0 * z_real * z_imag + c_imag;

		if (z_real * z_real + z_imag * z_imag > 4.0) {
			break;
		}
		
		z_real = z2_real;
		z_imag = z2_imag;
		iterations += 1;

		++i;
	}

	return iterations;
}

int ship(vec2 p) {
	
    int iterations = 0;
	
	float z_real = (resolution.x / resolution.y) * (p.x - 0.5) * scale - center.x / resolution.y;
	float z_imag = (p.y - 0.5) * scale - center.y / resolution.y;

	float x = z_real;
	float y = z_imag;

	while(z_real*z_real + z_imag*z_imag < 4 && iterations < max_iteration) {
		float xtemp = z_real*z_real - z_imag * z_imag + x;
		z_imag = abs(2*z_real*z_imag) - y;
		z_real = xtemp;
		iterations++;
	}

	return iterations;
}

int julia(vec2 p) {
	vec2 z;
	z.x =  (resolution.x / resolution.y) * (p.x - 0.5) * scale;
	z.y = (p.y - 0.5) * scale;
	
	vec2 c;

	int i;
	for(i = 0; i < max_iteration; ++i) {
		c.x	= sin(z.x * z.x - z.y * z.y) + R * cos(n * z.x);
		c.y	= sin(2 * z.x * z.y) + R * sin(n * z.y);
		float x = (z.x* z.x - z.y * z.y) + R;
		float y = (z.x * z.y + z.x * z.y) + n;
		
		if ((x*x+y*y) > 10) break;
		
		z.x = x;
		z.y = y;
	}
	return i;
}
	

void color(int frac) {
    vec2 p  = gl_FragCoord.xy / resolution.xy;
	int iterations;
    if (frac == 0) {
		iterations = mandelbrot(p);
	}
	else if (frac == 1) {
		iterations = ship(p);
	}
	else if (frac == 2) {
		iterations = julia(p);
	}
	
	
    if (iterations == max_iteration) {
        gl_FragColor = vec4(0, 0, 0, 1);
		return;
    }
 
    vec2 p_color = vec2(float(iterations / max_iteration), 0);
    
    vec4 color = texture2D(palette, p_color);

    gl_FragColor = color;
}

void main() {
	color(fractal);
}