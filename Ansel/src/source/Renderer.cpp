#include "../headers/Renderer.h"

namespace Ansel
{
	Shader* Renderer::shader;

	void Renderer::prepare() {
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Renderer::init() {
		shader = new Shader("assets/shaders/shader.vert", "assets/shaders/shader.frag");
	}

	void Renderer::loadShader(Shader* s) {
		shader = s;
	}

	void Renderer::Render(Model model) {
		shader->bind();

		RawModel rawModel = *model.getRawModel();
		
		VAO *vao = rawModel.getVAO();
		vao->bind();

		/*
		for (int i = 0; i < 16; i++) {
			if (vao->VBOS.at(i).count != 0) {
				glEnableVertexAttribArray(i);
			}
		}
		*/

		if (vao->bufferExists(Buffer::INDICES)) {
			vao->getBuffer(Buffer::INDICES).bind();

			glDrawElements(GL_TRIANGLES, rawModel.getVertexCount(), GL_UNSIGNED_INT, (void*)0);
		}
		else
			glDrawArrays(GL_TRIANGLES, 0, 3);

		/*
		for (int i = 0; i < 16; i++) {
			if (vao->VBOS.at(i).count != 0) {
				glDisableVertexAttribArray(i);
			}
		}
		*/

		vao->unbind();
	}
}