#define GL_SILENCE_DEPRECATION
#include <nanogui/nanogui.h>
#include "linmath.h"
#include <iostream>
#include <opencv2/imgcodecs.hpp>

using namespace std;
using namespace Eigen;
using namespace cv;

struct MyPoint
{
		float x, y, z;
		float r, g, b;
	void print()
	{
		cout << x << " " << y << " " << z << "       " << r << " " << g << " " << b << endl;
	}
};

class MyGLCanvas : public nanogui::GLCanvas {
public:
	MyGLCanvas(Widget *parent) : nanogui::GLCanvas(parent), mRotation(nanogui::Vector3f(0.f, 0.f, 0.f)) {
		using namespace nanogui;

		readImg("/Users/giulio/git/sampledata/3dmodels/cube.jpg");

//		{
//				{ -60.f, -40.f, 0.f, 1.f, 0.f, 0.f },
//				{  60.f, -40.f, 0.f, 0.f, 1.f, 0.f },
//				{   0.f,  60.f, 0.f, 0.f, 0.f, 1.f }
//		};

		static const char* vertex_shader_text =
		"#version 330\n"
		"uniform mat4 MVP;\n"
		"in vec3 position;\n"
		"in vec3 color;\n"
		"out vec4 frag_color;\n"
		"void main() {\n"
		"    frag_color = vec4(color, 1.0);\n"
		"    gl_Position = MVP * vec4(position, 1.0);\n"
		"}";

		static const char* fragment_shader_text =
		"#version 330\n"
		"out vec4 color;\n"
		"in vec4 frag_color;\n"
		"void main() {\n"
		"    color = frag_color;\n"
		"}";

		glGenVertexArrays(1, &vao);

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    glCompileShader(vertex_shader);

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    glCompileShader(fragment_shader);

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);


		glUseProgram(program);
    glBindVertexArray(vao);


		glGenBuffers(1, &vertex_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(MyPoint) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    mvp_location = glGetUniformLocation(program, "MVP");
    vpos_location = glGetAttribLocation(program, "position");
    vcol_location = glGetAttribLocation(program, "color");

    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*) 0);
    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*) (sizeof(float) * 3));

		glPointSize(10);
	}

	void setRotation(nanogui::Vector3f vRotation) {
		mRotation = vRotation;
	}

	virtual bool mouseButtonEvent(const Vector2i &p, int button, bool down, int modifiers) override
	{
//		cout << button << "  " << down << endl;
//		if (button == GLFW_MOUSE_BUTTON_2 && !down)
//		{
//			angle = 0.f;
//			tran = {0, 0};
//			scale = 500.f;
//		}
		return true;
	}

	/// Handle a mouse drag event (default implementation: do nothing)
	virtual bool mouseDragEvent(const Vector2i &p, const Vector2i &rel, int button, int modifiers) override
	{
		if (button == GLFW_MOUSE_BUTTON_2)
		{
			auto vec = p - size() / 2;
			angle = -atan2(vec[1], vec[0]);
		}
		else if (button == GLFW_MOUSE_BUTTON_3)
		{
			tran += Vector2f((float)rel[0], -(float)rel[1]);
		}
		return true;
	}

	virtual bool scrollEvent(const Vector2i &p, const Vector2f &rel) override
	{
		scale = max(0.f, scale + rel[1]);
		cout << "scale: " << scale << endl;
		return true;
	}

	virtual void drawGL() override {

		float ratio = 1.f; // todo: get this somewhow
		mat4x4 m, v, p, mvp;

		mat4x4_identity(m);
//		mat4x4_scale_aniso(m, m, scale, scale, 1.f);
		mat4x4_translate_in_place(m, tran[0], tran[1], 0.f);
		mat4x4_rotate_X(m, m, mRotation[0]);
		mat4x4_rotate_Z(m, m, angle);

		mat4x4_identity(v);
		vec3 eye = {0, 0, scale};
		vec3 center = {0, 0, 0};
		vec3 up = {0, 1, 0};
		mat4x4_look_at(v, eye, center, up);

//		mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
		mat4x4_perspective(p, 60.f/180.f*3.14159, ratio, 10.f, 1000.f);

		mat4x4_mul(mvp, v, m);
		mat4x4_mul(mvp, p, mvp);

		glUseProgram(program);
    glBindVertexArray(vao);

		glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);
		glDrawArrays(GL_POINTS, 0, vertices.size());
//		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	void readImg(const string &path)
	{
		cout << "reading " << path << endl;
		vertices.clear();
		Mat img = imread(path);
	for (int r = 0; r < img.rows; r++)
			{
				Vec3b *imgPtr = img.ptr<Vec3b>(r);
				for (int c = 0; c < img.cols; c++)
				{
					vertices.push_back({(float)(c - img.cols / 2) * 1.f, (float)(r - img.rows / 2) * 1.f, 0.f, (float)(imgPtr[c][2]) / 255.f, (float)(imgPtr[c][1]) / 255.f, (float)(imgPtr[c][0]) / 255.f});
	//				vertices[vertices.size() - 1].print();
				}
			}
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(MyPoint) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	}

private:
	GLuint vertex_buffer, vertex_shader, fragment_shader, program, vao;
	GLint mvp_location, vpos_location, vcol_location;
	Eigen::Vector3f mRotation;
	float angle = 0.f;
	float scale = 500.f;
	Vector2f tran = {0.f, 0.f};
	vector<MyPoint> vertices;
};


class ExampleApplication : public nanogui::Screen {
public:
	ExampleApplication() : nanogui::Screen(Eigen::Vector2i(800, 600), "NanoGUI Test") {
		using namespace nanogui;

		Window *window = new Window(this, "");
		window->setLayout(new GroupLayout(0, 0, 0, 0));

		mCanvas = new MyGLCanvas(window);
		mCanvas->setBackgroundColor({100, 100, 100, 255});
		mCanvas->setSize({600, 600});

		Window *windowOption = new Window(this, "Option");
		windowOption->setLayout(new GroupLayout(0, 0, 0, 0));
		windowOption->setPosition(Vector2i(600, 0));
		Widget *tools = new Widget(windowOption);
		windowOption->setFixedWidth(200);
		tools->setLayout(new BoxLayout(Orientation::Vertical,
																	 Alignment::Fill, 0, 5));

		Button *b0 = new Button(tools, "Random Color");
		b0->setCallback([this]() { mCanvas->setBackgroundColor(Vector4i(rand() % 256, rand() % 256, rand() % 256, 255)); });

		Button *b1 = new Button(tools, "Random Rotation");
		b1->setCallback([this]() { mCanvas->setRotation(nanogui::Vector3f((rand() % 100) / 100.0f, (rand() % 100) / 100.0f, (rand() % 100) / 100.0f)); });

		Slider *slider = new Slider(tools);
		slider->setValue(0.5f);

		performLayout();
	}

	virtual bool keyboardEvent(int key, int scancode, int action, int modifiers) {
		if (Screen::keyboardEvent(key, scancode, action, modifiers))
			return true;
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
			setVisible(false);
			return true;
		}
		return false;
	}

	virtual void draw(NVGcontext *ctx) {
		/* Draw the user interface */
		Screen::draw(ctx);
	}
private:
	MyGLCanvas *mCanvas;

	virtual bool dropEvent(const std::vector<std::string> &filenames) override
	{
		mCanvas->readImg(filenames[0]);
		return true; /* To be overridden */
	}
};

int main(int argc, char **argv) {
	try {
		nanogui::init();

		/* scoped variables */ {
			nanogui::ref<ExampleApplication> app = new ExampleApplication();
			app->drawAll();
			app->setVisible(true);
			nanogui::mainloop();
		}

		nanogui::shutdown();
	} catch (const std::runtime_error &e) {
		std::string error_msg = std::string("Caught a fatal error: ") + std::string(e.what());
		std::cerr << error_msg << endl;
		return -1;
	}

	return 0;
}
