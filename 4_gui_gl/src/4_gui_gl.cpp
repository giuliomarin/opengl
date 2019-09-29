#define GL_SILENCE_DEPRECATION
#include <nanogui/nanogui.h>
#include "linmath.h"
#include <iostream>

using namespace std;

class MyGLCanvas : public nanogui::GLCanvas {
public:
	MyGLCanvas(Widget *parent) : nanogui::GLCanvas(parent), mRotation(nanogui::Vector3f(0.25f, 0.5f, 0.33f)) {
		using namespace nanogui;

		static const struct
		{
				float x, y;
				float r, g, b;
		} vertices[3] =
		{
				{ -0.6f, -0.4f, 1.f, 0.f, 0.f },
				{  0.6f, -0.4f, 0.f, 1.f, 0.f },
				{   0.f,  0.6f, 0.f, 0.f, 1.f }
		};

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
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    mvp_location = glGetUniformLocation(program, "MVP");
    vpos_location = glGetAttribLocation(program, "position");
    vcol_location = glGetAttribLocation(program, "color");

    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*) 0);
    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*) (sizeof(float) * 2));
	}

	void setRotation(nanogui::Vector3f vRotation) {
		mRotation = vRotation;
	}

	virtual void drawGL() override {
		using namespace nanogui;

		float ratio = 1.f; // todo: get this somewhow
		mat4x4 m, p, mvp;

		mat4x4_identity(m);
		mat4x4_rotate_X(m, m, mRotation[0]);
		mat4x4_rotate_Z(m, m, (float) glfwGetTime());
		mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
		mat4x4_mul(mvp, p, m);

		glUseProgram(program);
    glBindVertexArray(vao);

		glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

private:
	GLuint vertex_buffer, vertex_shader, fragment_shader, program, vao;
	GLint mvp_location, vpos_location, vcol_location;
	Eigen::Vector3f mRotation;
};


class ExampleApplication : public nanogui::Screen {
public:
	ExampleApplication() : nanogui::Screen(Eigen::Vector2i(800, 600), "NanoGUI Test") {
		using namespace nanogui;

		Window *window = new Window(this, "GLCanvas Demo");
		window->setPosition(Vector2i(15, 15));
		window->setLayout(new GroupLayout(0, 0, 0, 0));

		mCanvas = new MyGLCanvas(window);
		mCanvas->setBackgroundColor({100, 100, 100, 255});
		mCanvas->setSize({400, 400});

		Widget *tools = new Widget(window);
		tools->setLayout(new BoxLayout(Orientation::Horizontal,
																	 Alignment::Middle, 0, 5));

		Button *b0 = new Button(tools, "Random Color");
		b0->setCallback([this]() { mCanvas->setBackgroundColor(Vector4i(rand() % 256, rand() % 256, rand() % 256, 255)); });

		Button *b1 = new Button(tools, "Random Rotation");
		b1->setCallback([this]() { mCanvas->setRotation(nanogui::Vector3f((rand() % 100) / 100.0f, (rand() % 100) / 100.0f, (rand() % 100) / 100.0f)); });

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
